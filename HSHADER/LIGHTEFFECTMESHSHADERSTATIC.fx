#include "LIGHT.fx"
#include "BASECB.fx"
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
    float4 vBlendWeight : BLENDWEIGHTS;
    float4 vBlendIndex : BLENDINDICES;

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

Texture2D AniMatTex : register(t10);

matrix TexToMat(int idx)
{
    matrix BoneMat =
    {
        AniMatTex.Load(int3((idx * 4), 0, 0)),
        AniMatTex.Load(int3((idx * 4) + 1, 0, 0)),
        AniMatTex.Load(int3((idx * 4) + 2, 0, 0)),
        AniMatTex.Load(int3((idx * 4) + 3, 0, 0)),
    };

    return BoneMat;
}

// 레퍼런스
void Skinning(inout float4 vPos, inout float4 vNormal, inout float4 vWeights, inout float4 vIndices)
{
    float4 CalPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 CalNormal = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float fWeights[4];
    fWeights[0] = vWeights.x;
    fWeights[1] = vWeights.y;
    fWeights[2] = vWeights.z;
    fWeights[3] = 1.f - vWeights.x - vWeights.y - vWeights.z;

    vPos.w = 1.0F;
    vNormal.w = 0.0F;

    for (int i = 0; i < 4; ++i)
    {
        matrix matBone = TexToMat((int) vIndices[i]);

        CalPos += fWeights[i] * mul(vPos, matBone);
        CalNormal.xyz += fWeights[i] * mul(vNormal, matBone);
    }

    CalPos.w = 1.0F;
    CalNormal.w = 0.0F;


    vPos = CalPos;


    return;
}


// 텍스처중 일부분만 렌더링 하기 위한 FLOAT4가 필요.
cbuffer CUTDATA : register(b1)
{
    float4 CutUv;
}

cbuffer TIMEDATA : register(b2)
{
    float4 TimeX;
}



VTX_OUT VS_ANIMESH(VTX_IN _in)
{
    //여기서 타임 상수버퍼를 받아서 나무잎같은것도 흔들리게 할수 있다.

    VTX_OUT Out = (VTX_OUT) 0.0f;
    
    Out.vUv.x = (_in.vUv.x);
    Out.vUv.y = (_in.vUv.y);
 
    Out.vPos = mul(_in.vPos, WVP);

    Out.vViewPos = mul(_in.vPos, WV);
    Out.vNormal = normalize(mul(-_in.vNormal, WV));
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

