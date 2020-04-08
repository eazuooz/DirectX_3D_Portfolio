#include "BASECB.fx"
////////// VTX
struct VTX_IN
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD; // 텍스처 좌표
    float4 vCol : COLOR; // 색깔
};

struct VTX_OUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD; // 텍스처 좌표
    float4 vCol : COLOR; // 색깔
};

// 텍스처중 일부분만 렌더링 하기 위한 FLOAT4가 필요.
cbuffer CUTDATA : register(b1)
{
    float4 CutUv;
    // x y는 시작점의 정보.
    // z w에는 크기를 넣어줄 것이다.
    // 시작점은?
    // 0.25 0.25 시작점
    // 0.5 0.5 크기
}

cbuffer UIDATA : register(b2)
{
    float4 m_UIData;
    // x y는 시작점의 정보.
    // z w에는 크기를 넣어줄 것이다.
    // 시작점은?
    // 0.25 0.25 시작점
    // 0.5 0.5 크기
}


VTX_OUT VS_TEX(VTX_IN _in)
{
    // 일부분만 랜더링 한다는 이야기는
    // 

    // 0 0 | 1 0
    // 0 1 | 1 1

    //   0.25 0.25   | 0.75    0.25 
    //   0.25 0.75F   | 0.75  0.75

    VTX_OUT Out = (VTX_OUT) 0.0f;
    Out.vCol = _in.vCol;
    

    //           1           0.5        0.25
    Out.vUv.x = (_in.vUv.x * CutUv.z) + CutUv.x;
    //           0           0.5        0.25
    Out.vUv.y = (_in.vUv.y * CutUv.w) + CutUv.y;

    // Out.vUv = _in.vUv;

    Out.vPos = mul(_in.vPos, WVP);

    //if (m_UIData.x == 1.0f)
    //{
    //    Out.vPos.z = 0.0f;
    //}

    return Out;
}


///// PIX



struct PIX_OUT
{
    float4 vOutColor : SV_Target;
    // float4 vOutColor : SV_Target1;
    // float4 vOutColor : SV_Target2;
    // float4 vOutColor : SV_Target3;
    // float4 vOutColor : SV_Target4;
    // float4 vOutColor : SV_Target5;
    // float4 vOutColor : SV_Target6;
    // float4 vOutColor : SV_Target7;
};

Texture2D Tex : register(t0); 
SamplerState LSmp : register(s0); 
SamplerState PSmp : register(s1);

PIX_OUT PS_TEX(VTX_OUT _in)
{
    PIX_OUT Out = (PIX_OUT) 0.0f;
    Out.vOutColor = Tex.Sample(LSmp, _in.vUv); // 이부분

    // Out.vOutColor.a = 0.0f;
    

    //Out.vOutColor.a = 0.6f;
    if (0.0f == Out.vOutColor.a)
    {
        // 이픽셀을 무시해라!!!!!!
        clip(-1);
    }
    

    return Out;
}

PIX_OUT PS_HALFTRANS(VTX_OUT _in)
{
    PIX_OUT Out = (PIX_OUT) 0.0f;
    Out.vOutColor = Tex.Sample(LSmp, _in.vUv); // 이부분

    // Out.vOutColor.a = 0.0f;
    

    //Out.vOutColor.a = 0.6f;
    if (0.0f == Out.vOutColor.a)
    {
        // 이픽셀을 무시해라!!!!!!
        clip(-1);
    }
    
    Out.vOutColor.a = 0.51f;

    return Out;
}