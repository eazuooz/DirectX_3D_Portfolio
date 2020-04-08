struct VTX_IN
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD; // 텍스처 좌표
};

struct VTX_OUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD; // 텍스처 좌표
};


// 행렬
cbuffer MATDATA : register(b0)
{
    matrix WVP;
}

VTX_OUT VS_TEX(VTX_IN _in)
{
    VTX_OUT Out = (VTX_OUT) 0.0f;
    Out.vUv = _in.vUv;
    Out.vPos = mul(_in.vPos, WVP);

    return Out;
}


///// PIX



struct PIX_OUT
{
    float4 vOutColor : SV_Target;
};

Texture2D Tex : register(t0); 
SamplerState Smp : register(s0); 

PIX_OUT PS_TEX(VTX_OUT _in)
{
    PIX_OUT Out = (PIX_OUT) 0.0f;

    if ((_in.vUv.x < 0.01) || (_in.vUv.x > 0.99))
    {
        Out.vOutColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
        return Out;
    }

    if ((_in.vUv.y < 0.01) || (_in.vUv.y > 0.99))
    {
        Out.vOutColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
        return Out;
    }

    Out.vOutColor = Tex.Sample(Smp, _in.vUv); // 이부분
    // Out.vOutColor.w = 1.0f;
    // Out.vOutColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    return Out;
}