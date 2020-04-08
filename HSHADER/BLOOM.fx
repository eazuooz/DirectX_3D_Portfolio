#include "BASECB.fx"
#include "LIGHT.fx"

// 가우시안을 할때 가우시안 필터용 텍스처.
static float g_vGAU[25] =
{
    1.0F, 2.0f, 4.0f, 2.0f, 1.0f, // 10
    2.0F, 4.0f, 8.0f, 4.0f, 2.0f, // 20
    4.0F, 8.0f, 16.0f, 8.0f, 4.0f, // 40
    2.0F, 4.0f, 8.0f, 4.0f, 2.0f, // 20
    1.0F, 2.0f, 4.0f, 2.0f, 1.0f, // 10
};


////////// VTX
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
cbuffer FilterSize : register(b0)
{
    float4 m_FilterSize;
}


// 빛에 전체 연산을 하겠다.
VTX_OUT VS_BLOOMSMALL(VTX_IN _in)
{
    VTX_OUT Out = (VTX_OUT) 0.0f;
    Out.vUv = _in.vUv;
    Out.vPos = _in.vPos;
    return Out;
}

struct PS_OUTPUT
{
    float4 vCol : SV_Target;
};

// G버퍼
Texture2D LightDiffTarget : register(t0);
Texture2D LightSpecTarget : register(t1);
SamplerState LSmp : register(s0);

PS_OUTPUT PS_BLOOMSMALL(VTX_OUT _in)
{
    PS_OUTPUT outData = (PS_OUTPUT) 0.0f;
    // 캠타겟의 색깔일 것이다.
    // 이전까지 처리된 컬러를 그대로 가져오고.

    //_in.vUv.x *= 800 / 128.0f;
    //_in.vUv.y *= 600 / 128.0f;

    float4 SpecColor = LightSpecTarget.Sample(LSmp, _in.vUv);
    float4 DiffColor = LightDiffTarget.Sample(LSmp, _in.vUv);

    // float Slume = (Speclu.r + Speclu.g + Speclu.b) * 0.33333333334f;
    // float Dlume = (Difflu.r + Difflu.g + Difflu.b) * 0.33333333334f;

    float Lum = 0.0f;
    float PixelU = 1.0f / m_FilterSize.x;
    float PixelV = 1.0f / m_FilterSize.y;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            float2 vUv = _in.vUv + float2((i - 1) * PixelU, (j - 1) * PixelV);
            Lum += LightSpecTarget.Sample(LSmp, vUv) * g_vGAU[i * 5 + j];
        }
    }
    Lum /= 50.0f;

    if (0.8f >= Lum.x)
    {
        outData.vCol = float4(0.0f, 0.0f, 0.0f, 0.0f);
        return outData;
    }
    outData.vCol = float4(Lum, Lum, Lum, Lum);

    //if (0.9f >= SpecColor.r)
    //{
    //    outData.vCol = float4(0.0f, 0.0f, 0.0f, 0.0f);
    //    return outData;
    //}

    //outData.vCol = float4(1.0f, 1.0f, 1.0f, 1.0f);

    // outData.vCol = SpecColor;

    // 디퓨즈 컬러만 추출한다.
    
    return outData;

}



Texture2D SmallTarget : register(t0);   //축소한 타겟
Texture2D OriTarget : register(t1);     //원래 이미지


PS_OUTPUT PS_BLOOMEFFECT(VTX_OUT _in)
{
	// 아웃풋이 들어온다.
    PS_OUTPUT outData = (PS_OUTPUT) 0.0f;
    // 캠타겟의 색깔일 것이다.
    // 이전까지 처리된 컬러를 그대로 가져오고.
    
    // getdimensions()
    // CamTarget.GetDimensions
    float4 Ori = OriTarget.Sample(LSmp, _in.vUv);


    float4 Lum = SmallTarget.Sample(LSmp, _in.vUv);
    //float4 Lum = 0.0f;
    //float PixelU = 1.0f / 64.0f;
    //float PixelV = 1.0f / 64.0f;
    //for (int i = 0; i < 5; ++i)
    //{
    //    for (int j = 0; j < 5; ++j)
    //    {
    //        float2 vUv = _in.vUv + float2((i - 1) * PixelU, (j - 1) * PixelV);
    //        Lum += SmallTarget.Sample(LSmp, vUv) * g_vGAU[i * 5 + j];
    //    }
    //}
    //Lum /= 30.0f;

    outData.vCol = Ori + Lum;
    outData.vCol.w = 1.0f;

    // 뭔가 변화를 주고

    return outData;
}