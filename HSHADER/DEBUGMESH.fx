////////// VTX
// 뎁스 끄고 그릴거다.

struct VTX_IN
{
    float4 vPos : POSITION;
};

struct VTX_OUT
{
    float4 vPos : SV_POSITION;
};

// 행렬
cbuffer MATDATA : register(b0)
{
    matrix WVP;
}

//VTX_OUT VS_TEX(float4 vPos : POSITION, float2 vUv : POSITION)
// VTX_OUT VS_DEBUGMESH(float4 vPos : POSITION)
VTX_OUT VS_DEBUGMESH(VTX_IN _In)
{
    // 일부분만 랜더링 한다는 이야기는
    // 

    // 0 0 | 1 0
    // 0 1 | 1 1

    //   0.25 0.25   | 0.75    0.25 
    //   0.25 0.75F   | 0.75  0.75

    VTX_OUT Out = (VTX_OUT) 0.0f;
    Out.vPos = mul(_In.vPos, WVP);

    return Out;
}


///// PIX



struct PIX_OUT
{
    float4 vOutColor : SV_Target;
};

cbuffer COLOR : register(b1)
{
    float4 COLOR;
}

PIX_OUT PS_DEBUGMESH(VTX_OUT _in)
{
    PIX_OUT Out = (PIX_OUT) 0.0f;
    Out.vOutColor = float4(1.0f, 0.0f, 1.0f, 1.0f); // 이부분

    return Out;
}