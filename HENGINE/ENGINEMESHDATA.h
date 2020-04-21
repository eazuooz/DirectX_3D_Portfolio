#pragma once
#include <MATHS.h>
#include <d3d11_4.h>

class EngineVertex
{
private:
	EngineVertex() {}
	~EngineVertex() {}
public:
	class VertexDebug
	{
	public:
		Vector4 Pos;

	public:
		static unsigned int MEMSIZE()
		{
			return (unsigned int)sizeof(VertexDebug);
		}
	};


	class VertexColor
	{
	public:
		Vector4 Pos;

	public:
		static unsigned int MEMSIZE()
		{
			return (unsigned int)sizeof(VertexColor);
		}
	};


	class VertexGrid
	{
	public:
		Vector4 Pos;
		Vector2 vUv; // 나중에 설명

	public:
		static unsigned int MEMSIZE()
		{
			return (unsigned int)sizeof(VertexGrid);
		}
	};


	class Vertex2D
	{
	public:
		Vector4 Pos;
		Vector2 vUv; // 나중에 설명
		Vector4 Color;

	public:
		static unsigned int MEMSIZE()
		{
			return (unsigned int)sizeof(Vertex2D);
		}
	};

	class Vertex3D
	{
	public:
		Vector4 Pos;
		Vector2 vUv; // 나중에 설명
		Vector4 Color;
		Vector4 Normal; // 빛을 반사할 벡터 포워드
		Vector4 BiNormal; // 라이트
		Vector4 Tangent; // 업

	public:
		static unsigned int MEMSIZE()
		{
			return (unsigned int)sizeof(Vertex3D);
		}
	};

	class VertexFBX
	{
	public:
		Vector4 Pos;
		Vector4 UV;
		Vector4 Color;
		Vector4 Normal;
		Vector4 Tangent;
		Vector4 Binormal;
		Vector4 BlendWeight; // 계산된 가중치 내가 애니메이션의 본에 얼만큼 영향을 받는지
		Vector4 BlendIndex;  // 내가 어떤 본에 영향을 

	public:
		static unsigned int MEMSIZE()
		{
			return (unsigned int)sizeof(VertexFBX);
		}
	};
};

class EngineIndex
{
private:
	EngineIndex() {}
	~EngineIndex() {}
public:
	// 이젠 안쓴다.
	class IDX16
	{
		unsigned short _0, _1, _2;
	};

	// 통상적으로 삼각형 그리는데 3개의 정수가 필요하기 때문에
	// 이런 클래스를 만든것뿐.
	// UINT Arr[3][2]
	class Index32
	{
	public:
		unsigned int m_0, m_1, m_2;

	public:
		static unsigned int MEMCOUNT()
		{
			return 3;
		}

		static unsigned int MEMSIZE()
		{
			return (unsigned int)sizeof(unsigned int);
		}

		static DXGI_FORMAT FORMAT()
		{
			return DXGI_FORMAT_R32_UINT;
		}

	public:
		Index32(unsigned int _0, unsigned int _1, unsigned int _2)
			: m_0(_0), m_1(_1), m_2(_2)
		{

		}

	};
};

