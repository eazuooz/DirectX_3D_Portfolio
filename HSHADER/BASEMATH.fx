

void CalBump(inout float4 _Normal, Texture2D _BumpTex, SamplerState _Smp, float2 _Uv
, float4 _ViewNormal, float4 _ViewTangent, float4 _ViewBNormal)
{
    float4 BumpNormal = _BumpTex.Sample(_Smp, _Uv);

    BumpNormal = BumpNormal * 2.0F - 1.0F;
    BumpNormal = normalize(BumpNormal);
    BumpNormal.w = 0.0F;

    // 탄젠트 스페이스 매트릭스
    float3x3 TSM = float3x3(_ViewTangent.xyz, _ViewBNormal.xyz, _ViewNormal.xyz);
    _Normal.xyz = mul(BumpNormal.xyz, TSM);
    _Normal.w = 0.0f;
    _Normal = normalize(_Normal);
    // 저 범프 텍스처는 
}