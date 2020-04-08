cbuffer MATDATA : register(b0)
{
    matrix Pos;
    matrix Scale;
    matrix Rot;
    matrix World;
    matrix View;
    matrix Proj;
    matrix WV;
    matrix VP;
    matrix WVP;
    float4 CAMPOS;
    float4 RENDERDATA;
    // LIGHTDATA[1000] // 0 1 2 3
    // int 빛 몇개에 영향을 받느냐? // 0 1 2 3
    // int 빛 몇번몇번 몇번 빛의 영향을 받느냐?
    // int[1000] // 0 3 4
}

    // 정역문제 플랫 4번
    // RENDERDATA.x 버텍스 계산(고로 8 번의 연산) / 픽셀 계산(퐁)1
    // RENDERDATA.y 범프 on1 / 범프 off