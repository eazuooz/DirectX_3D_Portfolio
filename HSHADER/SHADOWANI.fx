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
    float4 vPos : SV_Position;
    float2 vUv : TEXCOORD; // ÅØ½ºÃ³ ÁÂÇ¥
    float4 vProj : POSITION;
    //float4 vCol : COLOR; // »ö±ò
};


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

// ·¹ÆÛ·±½º
void Skinning(inout float4 vPos, inout float4 vNormal, inout float4 vWeights, inout float4 vIndices)
{
    float4 CalPos = float4(1.0f, 1.0f, 1.0f, 0.0f);
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
        
        //CalPos += fWeights[i] * vPos;
        //CalNormal.xyz += fWeights[i] * vNormal;
    }

    CalPos.w = 1.0F;
    CalNormal.w = 0.0F;


    vPos = CalPos;


    return;
}

VS_ShadowOut VS_SHADOW(VTX_IN _In)
{
    Skinning(_In.vPos, _In.vNormal, _In.vBlendWeight, _In.vBlendIndex);
    
    //_In.vNormal.x = 1.0f;


    
    VS_ShadowOut output = (VS_ShadowOut) 0.f;
    
   
    output.vPos = mul(_In.vPos, WVP);
    output.vProj = output.vPos;
    
    
    //output.vCol = mul(_In.vCol, WVP);
    
    return output;
}



float4 PS_SHADOW(VS_ShadowOut _input) : SV_Target
{
    
    //if (_input.vCol.a == 0.0f)
    //{
    //    clip(-1);
    //}
    
    
    //float4 RETURNPXL = float4(_input.vProj.z / _input.vProj.w, 0.0f, 0.0f, 0.0f);
    
    
    
    
    return float4(_input.vProj.z / _input.vProj.w, 0.0f, 0.0f, 0.0f);

}

