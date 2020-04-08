
struct LIGHTRESULT
{
    // 전용시맨틱이 존재한다.
    float4 vDiff; // 확산광 난반사광
    float4 vSpec; // 정반사광
    float4 vAmbi; // 주변광 환경광
};

struct LIGHTDATA
{
    // 빛을 전부다 표현하려고 만들었으므로
    float4 Color; // 빛의 색깔
    float4 LightPos; // 빛의 위치 (디렉션 라이트면 의미가 없다)
    float4 LightDir; // 빛이 바라보는 방향(포인트 라이트면 그때그때마다 재계산 디렉션이면 항상 똑같다)
    // L의 반대
    float4 LightDirInv; // -LightDir (람베르트 조명공식에서 L)
    float Range; // 범위
    float DiffPower; // 빛의 강도 // 전반적으로 밝아지고
    float SpecPower; // 빛의 강도 // 내가 바라보는 방향과 변이 
    
    int Type;
    int SpecPow;
    int temp1;
    int temp2;
    int temp3;
};

struct ALLLIGHTDATA
{
    LIGHTDATA ArrLight[512];
    int4 LightSettingData;
};


// 10번 이상은 시스템 데이터
cbuffer LIGHTSETTING : register(b10)
{
    ALLLIGHTDATA LightData;
};

// 상수버퍼는 나중에 만든다.

// in out을 
// 고로의 단점을 쉽게 알수 있다.
LIGHTRESULT CalLightVTX(
float4 _vViewPos,  // 나는 빛을 반사하는 면중 하나인데 나는 여기야
float4 _vViewNormal,  // 나는 이쪽을 바라보고 있어
float4 _vCamPos,  // 그걸 여기서 보고 있어
LIGHTDATA _Light // 그걸 이 빛이 비추고 있어
)
{
    LIGHTRESULT RLR = (LIGHTRESULT) 0.0F;

    float RangeFactor = 0.0f;
    float Dis = 0.0f;

    if (0 >= _Light.Range)
    {
        _Light.Range = 1.0f;
    }
        

    //_vViewNormal.xyz = normalize(_vViewNormal.xyz);
    //_Light.LightDirInv.xyz = normalize(_Light.LightDirInv.xyz);

    // 0 DIR

    // 버텍스일때는 
    switch (_Light.Type)
    {
    case 1:
       _Light.LightDirInv = normalize(_Light.LightPos - _vViewPos);

       Dis = abs(length(_vViewPos - _Light.LightPos));
       if (Dis >= _Light.Range)
       {
            // 범위 바깥으로 나갔다.
            RangeFactor = 0.0f;
       }
       else
       {
            // 범위 안에 들어왔다.
       RangeFactor = 1.0f - (_Light.Range / Dis);
       }

       RangeFactor = abs(RangeFactor);
       if (1 <= RangeFactor)
       {
           RangeFactor = 1.0f;
       }
       break;
    }

    RLR.vDiff = max(0, dot(_vViewNormal.xyz, _Light.LightDirInv.xyz)) * _Light.DiffPower * RangeFactor;

    // 정반사 광을 계산하려면
    // 당연히 내가 어디서 보고 있는지
    float3 vR; // 반사방향
    float3 vE; // 내 눈의 방향

    // 역으로 내적한다.
    // 이것도
    vR = normalize(2.0f * dot(_Light.LightDirInv, _vViewNormal) * _vViewNormal - _Light.LightDirInv);
    // 내가 바라보는 방향
    vE = normalize(_vCamPos.xyz - _vViewPos.xyz);

    // 값을 정밀하게 만들려고 하는것.
    // 정반사광의 거침을 좀 잡으려고 하는것. 제일 빛나는 부분이 부드러워 진다.
    // 스펙큘러를 구했다.
    // 즉 가장 빛나는 부분이 줄어들게 된다.
    RLR.vSpec = dot(vE.xyz, vR.xyz) * _Light.SpecPower * RangeFactor;

    if (0 >= RLR.vSpec.x)
    {
        RLR.vSpec.xyz = 0.0f;
    }
    else
    {
        RLR.vSpec.xyz = pow(RLR.vSpec.x, _Light.SpecPow);
    }

    RLR.vAmbi = float4(0.25f, 0.25f, 0.25f, 0.0f);


    RLR.vDiff.w = RLR.vAmbi.w = RLR.vSpec.w = 1.0f;
    
    //RLR.vDiff = ceil(RLR.vDiff * 0.5f) / 0.5f;

    return RLR;
}

LIGHTRESULT CalLightPIX(
float4 _vViewPos, // 나는 빛을 반사하는 면중 하나인데 나는 여기야
float4 _vViewNormal, // 나는 이쪽을 바라보고 있어
float4 _vCamPos, // 그걸 여기서 보고 있어
LIGHTDATA _Light // 그걸 이 빛이 비추고 있어
)
{
    LIGHTRESULT RLR = (LIGHTRESULT) 0.0F;

    float RangeFactor = 1.0f;
    float Dis = 0.0f;

    if (0 >= _Light.Range)
    {
        _Light.Range = 1.0f;
    }
        

    //_vViewNormal.xyz = normalize(_vViewNormal.xyz);
    //_Light.LightDirInv.xyz = normalize(_Light.LightDirInv.xyz);

    // 0 DIR

    // 버텍스일때는 
    switch (_Light.Type)
    {
        case 1:
            _Light.LightDirInv.xyz = normalize(_Light.LightPos.xyz - _vViewPos.xyz);

            Dis = abs(length(_vViewPos.xyz - _Light.LightPos.xyz));
            if (Dis >= _Light.Range)
            {
            // 범위 바깥으로 나갔다.
                RangeFactor = 0.0f;
            }
            else
            {
            // 범위 안에 들어왔다.
                RangeFactor = 1.0f - (_Light.Range / Dis);
            }

            RangeFactor = abs(RangeFactor);
            if (1 <= RangeFactor)
            {
                RangeFactor = 1.0f;
            }
            break;
    }

    RLR.vDiff = max(0, dot(_vViewNormal.xyz, _Light.LightDirInv.xyz)) * _Light.DiffPower * RangeFactor;

    // 정반사 광을 계산하려면
    // 당연히 내가 어디서 보고 있는지
    float3 vR; // 반사방향
    float3 vE; // 내 눈의 방향

    // 역으로 내적한다.
    // 이것도
    vR = normalize(2.0f * _vViewNormal.xyz * dot(_Light.LightDirInv.xyz, _vViewNormal.xyz) - _Light.LightDirInv.xyz);
    // 내가 바라보는 방향
    vE = normalize(_vCamPos.xyz - _vViewPos.xyz);

    // 값을 정밀하게 만들려고 하는것.
    // 정반사광의 거침을 좀 잡으려고 하는것. 제일 빛나는 부분이 부드러워 진다.
    // 스펙큘러를 구했다.
    // 즉 가장 빛나는 부분이 줄어들게 된다.
    RLR.vSpec = dot(vE.xyz, vR.xyz) * _Light.SpecPower * RangeFactor;

    if (0 >= RLR.vSpec.x)
    {
        RLR.vSpec.xyz = 0.0f;
    }
    else
    {
        RLR.vSpec.xyz = pow(RLR.vSpec.x, _Light.SpecPow);
    }

    
    
    
    RLR.vAmbi = float4(0.25f, 0.25f, 0.25f, 0.0f);
    
    
    //외곽선
    //float checkCos = dot(_vViewNormal.xyz, vE.xyz);
    //float degree = acos(checkCos) * (180.0f / 3.141592f);
    //if (85.0f < degree && 95.0f > degree)
    //{
    //    RLR.vDiff.xyz = float3(0.0f, 0.0f, 0.0f);
    //}
    
    //if (checkCos < 0.1f && checkCos > 0.0f)
    //{
    //    RLR.vDiff.xyz = float3(0.0f, 0.0f, 0.0f);
    //}
    
    RLR.vDiff.w = RLR.vAmbi.w = RLR.vSpec.w = 1.0f;
        
    //RLR.vDiff.xyz = ceil(RLR.vDiff.xyz * 2.0f) / 2.0f;
    
    return RLR;
}