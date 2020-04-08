////////// VTX
struct VTX_IN
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD; // 텍스처 좌표
    float4 vCol : COLOR; // 색깔

    // 물체마다 들어오게 된다.
    // 아래쪽에 있는 녀석은 물체당 하나씩 넣어주게 된다.
    // 버텍스 버퍼의 크기가 변경되는 것으 아니다.
    // 하나씩 넣어주게 됩니다.
    // row_major 요 표현을 쓰면 중간 메모리 구분을 없애고 한번에 세트로 받을수 있게 된다.
    // 물체의 개수만큼 인스턴싱용 버퍼에 담겨서 들어온다.
    row_major matrix MATWVP : WVP;
    float4 vSpriteUv : SPRITEUV;
};

struct VTX_OUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD; // 텍스처 좌표
    float4 vCol : COLOR; // 색깔
};

//// 행렬
//cbuffer MATDATA : register(b0)
//{
//    matrix WVP;
//}
//// 텍스처중 일부분만 렌더링 하기 위한 FLOAT4가 필요.
//cbuffer CUTDATA : register(b1)
//{
//    float4 CutUv;
//    // x y는 시작점의 정보.
//    // z w에는 크기를 넣어줄 것이다.
//    // 시작점은?
//    // 0.25 0.25 시작점
//    // 0.5 0.5 크기
//}


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
    Out.vUv.x = (_in.vUv.x * _in.vSpriteUv.z) + _in.vSpriteUv.x;
    //           0           0.5        0.25
    Out.vUv.y = (_in.vUv.y * _in.vSpriteUv.w) + _in.vSpriteUv.y;

    // Out.vUv = _in.vUv;

    Out.vPos = mul(_in.vPos, _in.MATWVP);

    return Out;
}


///// PIX


