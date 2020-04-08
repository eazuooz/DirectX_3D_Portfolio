#include "BASECB.fx"
#include "LIGHT.fx"

// =================
// Shadow Map Shader
// =================
struct VTX_IN
{
    // Àü¿ë½Ã¸ÇÆ½ÀÌ Á¸ÀçÇÑ´Ù.
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD; // ÅØ½ºÃ³ ÁÂÇ¥
    float4 vCol : COLOR; // »ö±ò
    float4 vNormal : NORMAL; // »ö±ò
    float4 vBNormal : BINORMAL; // »ö±ò
    float4 vTangent : TANGENT; // »ö±ò
    float4 vBlendWeight : BLENDWEIGHTS;
    float4 vBlendIndex : BLENDINDICES;

};

struct VS_ShadowOut
{
    float4 vPos  : SV_Position;
    float2 vUv   : TEXCOORD; // ÅØ½ºÃ³ ÁÂÇ¥
    float4 vProj : POSITION;
    //float4 vCol : COLOR; // »ö±ò
    
};




VS_ShadowOut VS_SHADOW(VTX_IN _In)
{
    //Skinning(_In.vPos, _In.vNormal, _In.vBlendWeight, _In.vBlendIndex);
    
    //_In.vNormal.x = 1.0f;


    
    VS_ShadowOut output = (VS_ShadowOut) 0.f;
    
   
    output.vPos = mul(_In.vPos, WVP);
    output.vProj = output.vPos;
    //output.vCol = mul(_In.vCol, WVP);
    
    
    return output;
}



float4 PS_SHADOW(VS_ShadowOut _input) : SV_Target
{
    
    
    float4 RETURN = float4(_input.vProj.z / _input.vProj.w, 0.0f, 0.0f, 0.0f);
    
    //RETURN.x += 0.02f;
    
    //RETURN.x *= 0.1f;
    
    //if (_input.vCol.a == 0.0f)
    //{
    //    RETURN.x = -10.0f;
    //}
    
    //RETURN.x += 0.5f;
    //_input
    
    return RETURN;

    //_input.vProj.z += 0.01f;
    
    //return float4(_input.vProj.z, 1.0f, 1.0f, 1.0f);
    // return float4(1.0f, 0.f, 0.f, 1.f);
}

