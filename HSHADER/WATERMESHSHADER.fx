#include "LIGHT.fx"
#include "BASECB.fx"
#include "BASEMATH.fx"

#define BUMPOK 1.0F

////////// VTX
struct VTX_IN
{
    // 전용시맨틱이 존재한다.
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD; // 텍스처 좌표
    float4 vCol : COLOR; // 색깔
    float4 vNormal : NORMAL; // 색깔
    float4 vBNormal : BINORMAL; // 색깔
    float4 vTangent : TANGENT; // 색깔
};

struct VTX_OUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD; // 텍스처 좌표
    float4 vCol : COLOR; // 색깔
    float4 vViewPos : POSITION; // 텍스처 좌표
    float4 vNormal : NORMAL; // 색깔
    float4 vBNormal : BINORMAL; // 색깔
    float4 vTangent : TANGENT; // 색깔};
};
// 행렬

// 텍스처중 일부분만 렌더링 하기 위한 FLOAT4가 필요.
cbuffer CUTDATA : register(b1)
{
    float4 CutUv;
}

cbuffer TIMEDATA : register(b2)
{
    float4 TimeX;
}


float hash(float n)
{
    return frac(sin(n) * 43758.5453);
}

float noise(float3 x)
{
    // The noise function returns a value in the range -1.0f -> 1.0f

    float3 p = floor(x);
    float3 f = frac(x);

    f = f * f * (3.0 - 2.0 * f);
    float n = p.x + p.y * 57.0 + 113.0 * p.z;

    return lerp(lerp(lerp(hash(n + 0.0), hash(n + 1.0), f.x),
                   lerp(hash(n + 57.0), hash(n + 58.0), f.x), f.y),
               lerp(lerp(hash(n + 113.0), hash(n + 114.0), f.x),
                   lerp(hash(n + 170.0), hash(n + 171.0), f.x), f.y), f.z);
}


VTX_OUT VS_MESH(VTX_IN _in)
{
    float gWaveHeight = 50.0f;

    float gWaveFrequency = 500.0f;
    float gSpeed = 0.15f;
    // 빛을 하나 정의해 보자.
    // float4

    // 빛을 버텍스에서 계산하면 고로쉐이딩

    VTX_OUT Out = (VTX_OUT) 0.0f;
    
    float3 bbbb = float3(TimeX.x * 2.0f + ((_in.vUv.x * CutUv.z) + CutUv.x) * gWaveFrequency, 
                            TimeX.x * 2.0f + ((_in.vUv.y * CutUv.w) + CutUv.y) * gWaveFrequency, 2.0f);
    
    //float cosTime = gWaveHeight * cos(TimeX.x * 2.0f + ((_in.vUv.x * CutUv.z) + CutUv.x) * gWaveFrequency);
    float cosTime = gWaveHeight * noise(bbbb);
    
    
    
    //float sinTime = 300.0f * sin(TimeX.x * 2.0f + ((_in.vUv.x * CutUv.z) + CutUv.x) * 1000.0f);
    
    _in.vPos.y += (cosTime);
    //_in.vPos.z += cosTime;
    //_in.vPos.z += sinTime;
    
    Out.vUv.x = (_in.vUv.x * CutUv.z) + CutUv.x + float(TimeX.x * gSpeed);
    Out.vUv.y = (_in.vUv.y * CutUv.w) + CutUv.y;
    //Out.vUv.y = _in.vUv.y;
    
    
    //Out.vUv = float2(_in.vUv.x + TimeX.x * 1.0f, _in.vUv.y);
    
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
    //Out.vCol.a = 0.5f;

    return Out;
}


///// PIX



struct PIX_FORWARDOUT
{
    // 포워드 타겟
    // 랜더타겟 하나인거다.
    float4 vOutColor : SV_Target;
    // float4 vViewPosTarget : SV_Target1;
    // float4 vNormal : SV_Target2;
    //float4 vOutColor : SV_Target2;
    //float4 vOutColor : SV_Target3;
    // float4 vOutColor2 : SV_Target1;
};

struct PIX_DEFERREDOUT
{
    // 포워드 타겟
    // 텍스처
    float4 Diff : SV_Target0;
    float4 Pos : SV_Target1;
    float4 Normal : SV_Target2;
    float4 Depth : SV_Target3;
};

Texture2D Tex : register(t0);
Texture2D Bump : register(t1);
SamplerState LSmp : register(s0);
SamplerState PSmp : register(s1);
// SamplerState PointSmp : register(s1);

PIX_FORWARDOUT PS_MESH(VTX_OUT _in)
{
    PIX_FORWARDOUT Out = (PIX_FORWARDOUT) 0.0f;
    
    
    
    
    //float2 tUv = float2(_in.vUv.x + cos(TimeX.x) * 0.3f, _in.vUv.y + cos(TimeX.x));
    float2 tUv = float2(_in.vUv.x, _in.vUv.y);
    
    Out.vOutColor = Tex.Sample(LSmp, tUv); // 이부분

    if (RENDERDATA.y == BUMPOK)
    {
        // 범프가 있다로 체크하겠다.
        CalBump(_in.vNormal, Bump, PSmp, tUv.xy, _in.vNormal, _in.vTangent, _in.vBNormal);
        // 노말이 바뀌는 것.
        // _in.vNormal =

    }


    if (RENDERDATA.x == 0.0f)
    {
        Out.vOutColor *= _in.vCol;
    }
    else if (RENDERDATA.x == 1.0f)
    {
        // float4 CP = mul(CAMPOS, View);
        // CP = CAMPOS;
        float4 LightColor = (float4) 0.0f;

        for (int i = 0; i < LightData.LightSettingData.x; ++i)
        {
            LIGHTRESULT LR = CalLightPIX(_in.vViewPos, _in.vNormal, CAMPOS, LightData.ArrLight[i]);
            LightColor += LR.vDiff + LR.vSpec + LR.vAmbi;
        }

        // LightColor.w = 1.0f;
        // 텍스처와      라이트의    혼합이   최종공식이다.
        Out.vOutColor.xyz *= LightColor.xyz;
    }

    return Out;
}

PIX_DEFERREDOUT PS_DMESH(VTX_OUT _in)
{
    PIX_DEFERREDOUT Out = (PIX_DEFERREDOUT) 0.0f;
    Out.Diff = Tex.Sample(LSmp, _in.vUv); // 이부분
    // 깊이를 같이 저장할수가 없다.
    // Out.Pos = _in.vPos; // 실수함.
    Out.Pos = _in.vViewPos; // 이부분 float4개

    if (RENDERDATA.y == BUMPOK)
    {
        CalBump(Out.Normal, Bump, PSmp, _in.vUv.xy, _in.vNormal, _in.vTangent, _in.vBNormal);
    }
    else
    {
        Out.Normal = _in.vNormal; // 이부분 float4개
    }

    Out.Depth.x = _in.vViewPos.z; // 이부분 float4개
    // Out.Depth.y = 1.0f;

    // 안해주면 타겟에 안보임
    Out.Pos.w = 1.0f;
    Out.Normal.w = 1.0f;
    Out.Depth.w = 1.0f;

    
    //if (Out.Diff.a == 0.0f)
    //{
    //  clip(-1);
    //}
    
    //Out.Diff.g += 0.5f;
    Out.Diff.a = 0.6f;
    
    return Out;
}