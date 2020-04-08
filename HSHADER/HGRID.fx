#include "BASECB.fx"

// 행렬



struct VTX_GRIDOUT
{
    // 시스템 포지션은 기본적으로 w가 나뉘게 되서 들어오는걸.
    // 기억해야 한다.
    // 이녀석은 레스터 라이저에 등록된다.
    // 
    float4 vPos : SV_POSITION; //  애는 그걸 해야하는 포지션입니다.
    float4 vWorld : POSITION; // 텍스처 좌표
};


VTX_GRIDOUT VS_GRID(float4 _in : POSITION)
{
    VTX_GRIDOUT Out = (VTX_GRIDOUT) 0.0f;

    Out.vPos = mul(_in, WVP);
    Out.vWorld = mul(_in, World);
    // 보간이 되기는 보간이 된다.
    return Out;
}


///// PIX

cbuffer GRIDDATA : register(b1)
{
    // 넣어줍시다.
    float Step; // 100~ 1000 ~ 10000
    float Borader; // 굵기
    float MinA; // 굵기
    float MaxA; // 굵기
    float4 vColor;
    float4 vCamPos; // 캠의 Y
}


struct PIX_OUT
{
    float4 vOutColor : SV_Target;
};

PIX_OUT PS_GRID(VTX_GRIDOUT _in)
{
    PIX_OUT Out = (PIX_OUT) 0.0f;

    // 일정 단위로 올라갈때마다.
    //// 5로 나눠서 0일때만 칠하면 된다.
    //// 나머지가 되는 이유???
    //// 그래픽카드는 float 연산에 최적화 되어있다.
    //if (abs((_in.vWorld.x % Step * 10.0f)) <= 0.5f || abs((_in.vWorld.z % Step * 10.0f)) <= 0.5f)
    //{
    //    // vColor.w = Alpha;
    //    Out.vOutColor = vColor;
        
    //}
    //else 

    float MinX = abs((_in.vWorld.x % Step));
    float MinZ = abs((_in.vWorld.z % Step));

    float MaxX = abs((_in.vWorld.x % (Step * 10.0F)));
    float MaxZ = abs((_in.vWorld.z % (Step * 10.0F)));

    float Len = length(_in.vWorld.xz - vCamPos.xz);

    // 0 ~ 1.0f;
    // 0 ~ 10000.0f
    float LenA = (10000.0f - Len) / 10000.0f;
    
    if (MaxX <= Borader || MaxZ <= Borader)
    {
        // 전단계를 먼저 그린다.
        // vColor.w = Alpha;
        Out.vOutColor = vColor;
        Out.vOutColor.w = 1.0F * LenA;
        return Out;
    }
    
    if (MinX <= Borader || MinZ <= Borader)
    {
        // 전단계를 먼저 그린다.
        // vColor.w = Alpha;
        Out.vOutColor = vColor;
        Out.vOutColor.w = MinA * LenA;
        return Out;
    }
    

    clip(-1);

    return Out;
}