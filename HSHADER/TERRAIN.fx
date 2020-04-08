#include "LIGHT.fx"
#include "BASECB.fx"
#include "BASEMATH.fx"

#define BUMPOK 1.0F

////////// VTX
struct VTX_IN
{
    // 전용시맨틱이 존재한다.
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD;
    float4 vCol : COLOR;
    float4 vNormal : NORMAL;
    float4 vBNormal : BINORMAL;
    float4 vTangent : TANGENT;
};

struct VTX_OUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD;
    float4 vCol : COLOR;
    float4 vViewPos : POSITION;
    float4 vNormal : NORMAL;
    float4 vBNormal : BINORMAL;
    float4 vTangent : TANGENT;
};

VTX_OUT VS_TERRAIN(VTX_IN _in)
{
    VTX_OUT Out = (VTX_OUT) 0.0f;
    
    Out.vUv = _in.vUv;
    Out.vPos = mul(_in.vPos, WVP);

    Out.vViewPos = mul(_in.vPos, WV);
    Out.vNormal = normalize(mul(_in.vNormal, WV));
    Out.vTangent = normalize(mul(_in.vTangent, WV));
    Out.vBNormal = normalize(mul(_in.vBNormal, WV));
    Out.vNormal.w = Out.vTangent.w = Out.vBNormal.w = 0.0f;

    // RENDERDATA.y == 0.0f 이면 포워드
    // 1이면 디퍼드이다.

    if (RENDERDATA.x == 0.0F )
    {
        float4 CP = mul(CAMPOS, View);
        // CP = CAMPOS;
        for (int i = 0; i < LightData.LightSettingData.x; ++i)
        {
            LIGHTRESULT LR = CalLightVTX(Out.vViewPos, Out.vNormal, CP, LightData.ArrLight[i]);
            Out.vCol += LR.vDiff + LR.vSpec + LR.vAmbi;
        }

    }


    return Out;
}


///// PIX



struct PIX_FORWARDOUT
{
    float4 vOutColor : SV_Target;
};

struct PIX_DEFERREDOUT
{
    float4 Diff : SV_Target0;
    float4 Pos : SV_Target1;
    float4 Normal : SV_Target2;
    float4 Depth : SV_Target3;
    float4 Test : SV_Target4;
};


struct FLOORDATA
{
    float OnOff; // 1이면 켜는것이고 0이면 안켜진거.
    float Detail; // 디테일
    float BUMP; // 디테일 1.0f; 2.0f
    float Temp; // 짝 맞춤. 16바이트
};
	// 0번째 층이 base층이고.
cbuffer TERRAINDATA : register(b1)
{
    float4 TerrainSize;
    FLOORDATA ArrData[4];
};


// 크기가 같아야 한다.
// Texture2DArray

Texture2D F0Tex : register(t0);
Texture2D F0Bump : register(t1);

Texture2D F1Tex : register(t2);
Texture2D F1Bump : register(t3);
Texture2D F1Sp : register(t4);

Texture2D F2Tex : register(t5);
Texture2D F2Bump : register(t6);
Texture2D F2Sp : register(t7);

Texture2D F3Tex : register(t8);
Texture2D F3Bump : register(t9);
Texture2D F3Sp : register(t10);

SamplerState LSmp : register(s0);
SamplerState PSmp : register(s1);

PIX_FORWARDOUT PS_TERRAIN(VTX_OUT _in)
{
    PIX_FORWARDOUT Out = (PIX_FORWARDOUT) 0.0f;
    Out.vOutColor = F0Tex.Sample(LSmp, _in.vUv); // 이부분

    if (RENDERDATA.y == BUMPOK)
    {
        CalBump(_in.vNormal, F0Bump, PSmp, _in.vUv.xy, _in.vNormal, _in.vTangent, _in.vBNormal);
    }

    if (RENDERDATA.x == 0.0f)
    {
        Out.vOutColor *= _in.vCol;
    }
    else if (RENDERDATA.x == 1.0f)
    {
        float4 LightColor = (float4) 0.0f;

        for (int i = 0; i < LightData.LightSettingData.x; ++i)
        {
            LIGHTRESULT LR = CalLightPIX(_in.vViewPos, _in.vNormal, CAMPOS, LightData.ArrLight[i]);
            LightColor += LR.vDiff + LR.vSpec + LR.vAmbi;
        }
        Out.vOutColor.xyz *= LightColor.xyz;
    }

    return Out;
}


PIX_DEFERREDOUT PS_DTERRAIN(VTX_OUT _in)
{
    PIX_DEFERREDOUT Out = (PIX_DEFERREDOUT) 0.0f;

    float2 SpUv = _in.vUv;
    SpUv.x /= TerrainSize.x + 1.0f;
    SpUv.y /= TerrainSize.z + 1.0f;

    float2 DUv = _in.vUv * ArrData[0].Detail;
    float4 TerrainColor = F0Tex.Sample(LSmp, DUv); // 이부분

    float4 ResultNormal = _in.vNormal;
    float4 Normal0 = _in.vNormal;

    if (ArrData[0].BUMP == BUMPOK)
    {
        // 범프가 있다로 체크하겠다.
        CalBump(Normal0, F0Bump, PSmp, DUv, _in.vNormal, _in.vTangent, _in.vBNormal);
        ResultNormal = Normal0;

    }

    //sp == splatting
    if (ArrData[1].OnOff == 1.0f)
    {

        float4 PrevColor = TerrainColor; // 이부분

        DUv = _in.vUv * ArrData[1].Detail;

        float4 SpValue = F1Sp.Sample(LSmp, SpUv); // 이부분
        float DefSp = 1.0F - SpValue.x;
        PrevColor.xyz *= DefSp;
        PrevColor.xyz *= DefSp;
        
        float4 SpDifValue = F1Tex.Sample(LSmp, DUv); // 이부분
        SpDifValue.xyz *= SpValue.x;

        TerrainColor.xyz = PrevColor.xyz + SpDifValue.xyz;

        float4 Normal1 = _in.vNormal;

        if (ArrData[1].BUMP == BUMPOK)
        {
        // 범프가 있다로 체크하겠다.
            CalBump(Normal1, F1Bump, PSmp, DUv, _in.vNormal, _in.vTangent, _in.vBNormal);
        }

        ResultNormal.xyz = normalize((Normal0.xyz * DefSp) + (Normal1.xyz * SpValue.x));
        ResultNormal.w = 0.0f;
    }

    Out.Pos = _in.vViewPos; // 이부분 float4개
    Out.Normal = ResultNormal; // 이부분 float4개
    Out.Depth.x = _in.vViewPos.z; // 이부분 float4개
    // Out.Depth.y = 1.0f;

    // 안해주면 타겟에 안보임
    Out.Diff = TerrainColor; // 이부분
    Out.Pos.w = 1.0f;
    Out.Normal.w = 1.0f;
    Out.Depth.w = 1.0f;
    if (Out.Normal.x > 0.5f)
    {
        Out.Test = Out.Normal;

    }
    
    //Out.Normal.y += 0.3f;
    // Out.Diff = float4(1.0f, 0.0f, 0.0f, 1.0f); // 이부분


    return Out;
}