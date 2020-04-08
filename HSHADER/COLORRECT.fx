// 버텍스 쉐이더에서 받을 인풋
// 구조체
// : POSITION 시맨틱이라고 하는데.
// 이 데이터를 어떻게 처리해야할지에 대한 정보.

struct VTX_IN
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD; // 텍스처 좌표
    float4 vCol : COLOR; // 색깔
};

struct VTX_OUT
{
    float4 vPos : SV_POSITION;
	// SV가 붙으면 시스템 벨류로서 넘긴다.
	// 다음 픽셀쉐이더에 넘기기 위한 데이터라고 보면 된다.
    float2 vUv : TEXCOORD; // 텍스처 좌표
    float4 vCol : COLOR; // 색깔
};

// VTX_OUT VTXFunc(VTXCOL_IN) 
// {
// }

// RenderPipe() 
// {
//    PIXFunc(VTXFunc());
// }

// 정보가 남아있지는 않는다.

// 내가 세팅해 줄수 있는 변수를 여기에 넣어 놓는다.
// matrix BASE 내가 세팅해줄 수 있다.

cbuffer MATDATA : register(b0)
{
    matrix WVP;
}

// static float4x4
static matrix BASE =
{
    { 1.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 1.0f },
};

VTX_OUT VS_COLORRECT(VTX_IN _in)
{
	// 안될 것이다.
    // a += 1;
	// 쉐이더에서는 구조체를 대부분
	// 바이트 단위로 처리하는 방식을 사용하기 때문
    VTX_OUT Out = (VTX_OUT) 0.0f;
    Out.vCol = _in.vCol;
    Out.vUv = _in.vUv;

    // 우리가 계산한 행렬을 
    // Out.vPos = _in.vPos;
    Out.vPos = mul(_in.vPos, WVP);
	// 스위즐링 문법
    // Out.vCol.xyz

    return Out;
}

struct PIX_OUT
{
    float4 vOutColor : SV_Target;
	// float4 vOutColor2 : SV_Target1;
	// float4 vOutColor2 : SV_Target2;
};

cbuffer COLOR : register(b1)
{
    float4 COLOR;
    // x y는 시작점의 정보.
    // z w에는 크기를 넣어줄 것이다.
    // 시작점은?
    // 0.25 0.25 시작점
    // 0.5 0.5 크기
}

PIX_OUT PS_COLORRECT(VTX_OUT _in)
{
    matrix Mtrl;

    PIX_OUT Out = (PIX_OUT) 0.0f;
    // Out.vOutColor = Out.vOutColor = float4(0.0f, 0.0f, 1.0f, 1.0f);;
    Out.vOutColor = COLOR;

    //if (_in.vUv.x < 0.1f)
    //{
    //    Out.vOutColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    //}
    //if (_in.vUv.x > 0.9f)
    //{
    //    Out.vOutColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    //}
    //if (_in.vUv.y < 0.1f)
    //{
    //    Out.vOutColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    //}
    //if (_in.vUv.y > 0.9f)
    //{
    //    Out.vOutColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    //}
    return Out;
}