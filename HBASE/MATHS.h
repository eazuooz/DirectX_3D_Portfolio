#pragma once

#include <Windows.h>
#include <math.h>
#include <vector>
#include "JTEMPLATE.h"

#include "DEBUG.h"
#include <DirectXCollision.h> // 충돌용 로직
#include <DirectXCollision.inl>
#include <DirectXPackedVector.h> // d3dvector 벡터를 사용하기 위한 구조체

// _XM_NO_INTRINSICS_ SSE연산 사용하지 않겠다는 것.
//#ifdef X32
//#define _XM_NO_INTRINSICS_
//#endif

class Constant 
{
public:
	// 라디안에 이걸 곱해주면 디그리가 된다.
	// (180.0f / 파이)
	static const float PI;
	// 3.14라디안에 이걸 곱해주면? 180F
	// 180;
	static const float R2D;
	// 90도에 이걸 곱해주면? 1.57                  
	// 180도에 이걸 곱해주면? 3.14
	static const float D2R;
};

struct BoardIndex
{
public:
	union
	{
		struct
		{
			int x;
			int z;
		};
	};

public:
	BoardIndex operator-(int _Num)
	{
		BoardIndex temp;
		temp.x -= _Num;
		temp.z -= _Num;


		return temp;
	}

	BoardIndex operator-(BoardIndex& _Other)
	{
		BoardIndex temp;

		temp.x =  /*this.*/x - _Other.x;
		temp.z =  /*this.*/z - _Other.z;

		return temp;
	}

	void operator-=(int _Value)
	{
		x -= _Value;
		z -= _Value;
	}

	bool operator==(BoardIndex& _Other)
	{
		return (this->x == _Other.x) && (this->z == _Other.z);
	}

	bool operator!=(BoardIndex& _Other)
	{
		return (this->x != _Other.x) || (this->z != _Other.z);
	}

public:

	BoardIndex() : x(0), z(0)
	{
	}
	BoardIndex(size_t _Z, size_t _X) : x((int)_X), z((int)_Z)
	{
	}

	BoardIndex(int _Z, int _X) : x(_X), z(_Z)
	{
	}
};

struct Vector2 
{
public:
	union 
	{
		struct 
		{
			float x;
			float y;
		};

	};

	Vector2() : x(0.0F), y(0.0F)
	{
	}


	Vector2(float _x, float _y) : x(_x), y(_y)
	{	
	}
};

class Vector3 
{
public:
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		Vector2 Vec2;
	};
};

// class HMAT;
struct Vector4 
{
private:
	friend class GameWindow;

private:
	static const Vector4 ILEFTCON();
	static const Vector4 IDOWNCON();
	static const Vector4 IRIGHTCON();
	static const Vector4 IUPCON();

public:
	static const Vector4 ILEFT;
	static const Vector4 IDOWN;
	static const Vector4 IRIGHT;
	static const Vector4 IUP;
	
	static const Vector4 ARRIDIR[4];

	static const Vector4 LEFT;
	static const Vector4 RIGHT;
	static const Vector4 UP;
	static const Vector4 DOWN;
	static const Vector4 FORWARD;
	static const Vector4 BACK;
	static const Vector4 ZERO;

	static Vector4 RED;
	static Vector4 BLUE;
	static Vector4 GREEN;
	static Vector4 WHITE;
	static Vector4 GRAY;
	static Vector4 BLACK;
	static Vector4 MAGENTA;

public:
	static Vector4 DLEFT;
	static Vector4 DRIGHT;
	static Vector4 DUP;
	static Vector4 DDOWN;
	static Vector4 DFORWARD;
	static Vector4 DBACK;

private:
	static void TimeUpdate();

public:
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};

		struct
		{
			Vector2 Pos;
			Vector2 Size;
		};

		float Arr[4];
		Vector2 Vec2;
		Vector3 Vec3;
		DirectX::XMFLOAT4 xmFloat4;
		DirectX::XMFLOAT3 xmFloat3;
		// DirectX::XMVECTOR XMVEC;

#ifdef X64
		DirectX::XMVECTOR xmVector;
#endif


		struct
		{
			int ix;
			int iy;
			int iz;
			int iw;
		};

		struct
		{
			__int64 KeyPos;
			__int64 KeyScale;
		};

	};

public:
	float HX() { return x * 0.5f; }
	float HY() { return y * 0.5f; }
	float HZ() { return z * 0.5f; }
	float HW() { return w * 0.5f; }

	int IX() { return (int)x; }
	int IY() { return (int)y; }
	int IZ() { return (int)z; }
	int IW() { return (int)w; }
	unsigned int UIX() { return (unsigned int)x; }
	unsigned int UIY() { return (unsigned int)y; }
	unsigned int UIZ() { return (unsigned int)z; }
	unsigned int UIW() { return (unsigned int)w; }

public:
	void CheckToModifyMax(float _MinValue, float _MaxValue) 
	{
		if (x < _MinValue) { x = _MinValue; }
		if (x > _MaxValue) { x = _MaxValue; }
		if (y < _MinValue) { y = _MinValue; }
		if (y > _MaxValue) { y = _MaxValue; }
		if (z < _MinValue) { z = _MinValue; }
		if (z > _MaxValue) { z = _MaxValue; }
		if (w < _MinValue) { w = _MinValue; }
		if (w > _MaxValue) { w = _MaxValue; }
	}

	UINT ConToUIntColorRGBA() const
	{
		Vector4 ColorVec4 = *this;

		ColorVec4.CheckToModifyMax(0.0f, 1.0f);

		UINT Color = 0;
		unsigned char* P = (unsigned char*)&Color;

		P[0] = (unsigned char)(ColorVec4.x * 255);
		P[1] = (unsigned char)(ColorVec4.y * 255);
		P[2] = (unsigned char)(ColorVec4.z * 255);
		P[3] = (unsigned char)(ColorVec4.w * 255);

		return Color;
	}

	static Vector4 Normalization(const Vector4& _Value)
	{
		float dis = _Value.Distance();
		return { _Value.x / dis, _Value.y / dis, _Value.z / dis };
	}

	// 교환법칙이 성립하지 않는다.
	static Vector4 CrossProduct(const Vector4& _Left, const Vector4& _Right)
	{
		// 외적은 서로가 역백터라면 0벡터가 나온다.
		return DirectX::XMVector3Cross(_Left, _Right);
		// 형변환 연산자 
		 ;

		//Return.x = (_Left.y * _Right.z) - (_Left.z * _Right.y);
		//Return.y = (_Left.z * _Right.x) - (_Left.x * _Right.z);
		//Return.z = (_Left.x * _Right.y) - (_Left.y * _Right.x);
		//Return.w = 0.0f;
	}

	// 내적의 결과는 항상 스칼라.
	static float DotProduct(const Vector4& _Left, const Vector4& _Right)
	{
		// 외적은 서로가 역백터라면 0벡터가 나온다.

		// DotResult 안에 들어있는 4개의 값이??? 모두다 같은 값. 
		DirectX::XMVECTOR DotResult = DirectX::XMVector3Dot(_Left, _Right);
		return DotResult.m128_f32[0];
		// return _Left.x * _Right.x + _Left.y * _Right.y + _Left.z * _Right.z;
	}

public:
	// 내부가 다 float일때 int로 변경해주는 함수
	void ConToInt(float _DefValue = 0.0f)
	{
		ix = (int)(x + _DefValue);
		iy = (int)(y + _DefValue);
		iz = (int)(z + _DefValue);
		iw = (int)(w + _DefValue);
	}

	void Set(const float _Value) 
	{
		x = y = z = _Value;
	}

	void RtoQ(Vector4 Rot)
	{
#ifdef X64
		*this = Rot;
		xmVector = DirectX::XMQuaternionRotationRollPitchYaw(x, y, z);
#else
		*this = Rot;
		DirectX::XMStoreFloat4(&XMF4, DirectX::XMQuaternionRotationRollPitchYaw(x, y, z));
#endif
	}

	void RtoQ()
	{
		// 피타고라스의 정리
		// 제곱근 x * x + y*y

#ifdef X64
		xmVector = DirectX::XMQuaternionRotationRollPitchYaw(x, y, z);
#else
		DirectX::XMStoreFloat4(&XMF4, DirectX::XMQuaternionRotationRollPitchYaw(x, y, z));
#endif


		;
		// X Y

	}

	void QtoR()
	{
		Vector4 Q = *this;
		QtoR(Q);
	}

	void QtoR(const Vector4& _Q)
	{
		float dx = _Q.x;
		float dy = _Q.y;
		float dz = _Q.z;
		float dw = _Q.w;

		float sqx = _Q.x * _Q.x;
		float sqy = _Q.y * _Q.y;
		float sqz = _Q.z * _Q.z;
		float sqw = _Q.w * _Q.w;


		x = asinf(2.0f * (dw * dx - dy * dz));
		y = atan2f(2.0f * (dx * dz + dw * dy), -sqx - sqy + sqz + sqw);
		z = atan2f(2.0f * (dx * dy + dw * dz), -sqx + sqy - sqz + sqw);
	}


	float Distance() const // ||v|| 벡터의 길이
	{
		// 피타고라스의 정리
		// 제곱근 x * x + y*y

		return sqrtf((x * x) + (y * y) + (z * z));
		// X Y

	}

	void Normalization()
	{
		float Len = Distance();

		if (0 != Len)
		{
			x /= Len;
			y /= Len;
			z /= Len;
			w = 1.0f;
		}
	}

	void Normal3DToWZero()
	{
		float Len = Distance();

		if (0 != Len)
		{
			x /= Len;
			y /= Len;
			z /= Len;
			w = 0.0f;
		}
	}

	Vector4 ReturnNormalization()
	{
		// 피타고라스의 정리
		// 제곱근 x * x + y*y
		Vector4 Vec = *this;
		Vec.Normalization();
		return Vec;
		// X Y

	}

// + and +=

	//operator int() {
	//}

	operator DirectX::XMVECTOR() const
	{
#ifdef X64
		return xmVector;
#else
		// 16바이트 정렬로 변경한걸 리턴.
		return DirectX::XMLoadFloat4(&XMF4);
#endif
	}

	bool operator==(const Vector4& _Other)
	{
		return x == _Other.x && y == _Other.y && z == _Other.z;
	}

	bool operator!=(const Vector4& _Other)
	{
		return !operator==(_Other);
	}

	Vector4 operator+(const float _Scale)
	{
		return { x + _Scale, y + _Scale, z + _Scale };
	}
	Vector4 operator/(const float _Scale)
	{
		return { x / _Scale, y / _Scale, z / _Scale };
	}

	Vector4 operator+(const Vector2& _Scale)
	{
		return { x + _Scale.x, y + _Scale.y };
	}

	Vector4 operator+(const Vector3& _Scale)
	{
		return { x + _Scale.x, y + _Scale.y, z + _Scale.z };
	}

	Vector4 operator+(const Vector4& _Scale)
	{
		return { x + _Scale.x, y + _Scale.y, z + _Scale.z };
	}


	Vector4& operator+=(const float _Scale)
	{
		x += _Scale;
		y += _Scale;
		z += _Scale;

		return *this;
	}

	Vector4& operator+=(const Vector2& _Scale)
	{
		x += _Scale.x;
		y += _Scale.y;

		return *this;
	}

	Vector4& operator+=(const Vector3& _Scale)
	{
		x += _Scale.x;
		y += _Scale.y;
		z += _Scale.z;
		return *this;
	}

	Vector4& operator+=(const Vector4& _Scale)
	{
		x += _Scale.x;
		y += _Scale.y;
		z += _Scale.z;
		return *this;
	}


// - and -=
	Vector4 operator-() const
	{
		return { -x, -y, -z};
	}

	Vector4 operator-(const float _Scale) const
	{
		return { x - _Scale, y - _Scale, z - _Scale };
	}

	Vector4 operator-(const Vector2& _Scale) const
	{
		return { x - _Scale.x, y - _Scale.y };
	}

	Vector4 operator-(const Vector3& _Scale) const
	{
		return { x - _Scale.x, y - _Scale.y, z - _Scale.z };
	}
	
	Vector4 operator-(const Vector4& _Scale) const
	{
		return { x - _Scale.x, y - _Scale.y, z - _Scale.z };
	}


	Vector4& operator-=(const float _Scale)
	{
		x -= _Scale;
		y -= _Scale;
		z -= _Scale;

		return *this;
	}

	Vector4& operator-=(const Vector2& _Scale)
	{
		x -= _Scale.x;
		y -= _Scale.y;

		return *this;
	}

	Vector4& operator-=(const Vector3& _Scale)
	{
		x -= _Scale.x;
		y -= _Scale.y;
		z -= _Scale.z;
		return *this;
	}

	Vector4& operator-=(const Vector4& _Scale)
	{
		x -= _Scale.x;
		y -= _Scale.y;
		z -= _Scale.z;
		return *this;
	}

// * and *=
	Vector4 operator*(const float _Scale) const 
	{
		return {x * _Scale, y * _Scale, z * _Scale};
	}

	Vector4 operator*(const Vector2& _Scale) const
	{
		return { x * _Scale.x, y * _Scale.y};
	}

	Vector4 operator*(const Vector3& _Scale) const
	{
		return { x * _Scale.x, y * _Scale.y, z * _Scale.z };
	}

	Vector4 operator*(const Vector4& _Scale) const
	{
		return { x * _Scale.x, y * _Scale.y, z * _Scale.z };
	}


	Vector4& operator*=(const float _Scale)
	{
		x *= _Scale;
		y *= _Scale;
		z *= _Scale;

		return *this;
	}

	Vector4& operator*=(const Vector2& _Scale)
	{
		x *= _Scale.x;
		y *= _Scale.y;

		return *this;
	}

	Vector4& operator*=(const Vector3& _Scale)
	{
		x *= _Scale.x;
		y *= _Scale.y;
		z *= _Scale.z;
		return *this;
	}

	Vector4& operator*=(const Vector4& _Scale)
	{
		x *= _Scale.x;
		y *= _Scale.y;
		z *= _Scale.z;
		return *this;
	}

// Rot
	// 회전시키고 싶다.
	// 40도 회전해
	// 길이 1일 벡터가 원점에서 회전한 백터를 만든다.
	Vector4& RotateX_Degree(float _Deg)
	{
		return RotateX_Radian(_Deg * Constant::D2R);
	}

	Vector4& RotateX_Radian(float _Rad)
	{
		// 여기서 회전행렬 만들어서 그냥 MUL해버린다.

		Vector4 TempAngle = *this;

		// 회전된 각도의 벡터로 변경된다.
		y = (TempAngle.y * cosf(_Rad)) - (TempAngle.z * sinf(_Rad));
		z = (TempAngle.y * sinf(_Rad)) + (TempAngle.z * cosf(_Rad));
		return *this;
	}

	Vector4& RotateY_Degree(float _Deg)
	{
		return RotateY_Radian(_Deg * Constant::D2R);
	}

	Vector4& RotateY_Radian(float _Rad)
	{
		Vector4 TempAngle = *this;

		// 회전된 각도의 벡터로 변경된다.
		x = (TempAngle.x * cosf(_Rad)) - (TempAngle.z * sinf(_Rad));
		z = (TempAngle.x * sinf(_Rad)) + (TempAngle.z * cosf(_Rad));
		return *this;
	}

	Vector4& RotateZ_Degree(float _Deg)
	{
		return RotateZ_Radian(_Deg * Constant::D2R);
	}

	Vector4& RotateZ_Radian(float _Rad)
	{
		Vector4 TempAngle = *this;

		// 회전된 각도의 벡터로 변경된다.
		x = (TempAngle.x * cosf(_Rad)) - (TempAngle.y * sinf(_Rad));
		y = (TempAngle.x * sinf(_Rad)) + (TempAngle.y * cosf(_Rad));
		return *this;
	}

	Vector4& RotationDegree(Vector4& _Deg) 
	{
		return RotationRadian(_Deg * Constant::D2R);
	}

	Vector4& RotationRadian(const Vector4& _Rad)
	{
		RotateX_Radian(_Rad.x);
		RotateY_Radian(_Rad.y);
		RotateZ_Radian(_Rad.z);
		return *this;
	}

	// UP기준으로
	static Vector4 CalDToDir2D(float _Deg)
	{
		return CalRToDir2D(_Deg * Constant::D2R);
	}

	static Vector4 CalRToDir2D(float _Rad)
	{
		Vector4 Return;
		Return.x = cosf(_Rad);
		Return.y = sinf(_Rad);
		return Return;
	}

	Vector4& DToDir2D(float _Deg)
	{
		return RToDir2D(_Deg * Constant::D2R);
	}

	Vector4& RToDir2D(float _Rad)
	{
		x = cosf(_Rad);
		y = sinf(_Rad);
		return *this;
	}

public:
	Vector4() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {	}
	Vector4(float _Value) : x(_Value), y(_Value), z(_Value), w(1.0f) {	}
	Vector4(float _x, float _y) : x(_x), y(_y), z(0.0f), w(1.0f) {	}
	Vector4(float _x, float _y, float _z) : x(_x), y(_y), z(_z), w(1.0f)	{	}
	Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {	}

	Vector4(const DirectX::XMVECTOR& _XMVEC) 
	{	
#ifdef X64
		xmVector = _XMVEC;
#else
		DirectX::XMStoreFloat4(&XMF4, _XMVEC);
#endif
	}

public:
	POINT ConvertPOINT() 
	{
		POINT ReturnPoint;
		ReturnPoint.x = (int)x;
		ReturnPoint.y = (int)y;

		return ReturnPoint;
	}
};

// 1x3는 여기에 곱할수 있어 없어?
// 4X4에 최적화된 코드만 여기에 칠것이다. 
class Matrix 
{
public:
	union
	{
		float Arr2D[4][4];
		struct
		{
			float _00;
			float _01;
			float _02;
			float _03;
			float _10;
			float _11;
			float _12;
			float _13;
			float _20;
			float _21;
			float _22;
			float _23;
			float _30;
			float _31;
			float _32;
			float _33;
		};
		DirectX::XMFLOAT4X4 DXF4;
		// 16바이트 
		// 이녀석은 64비트일때만 있어야 한다.
#ifdef X64
		DirectX::XMMATRIX DXMAT;
#endif

		Vector4 ArrVec[4];
		float Arr1D[16];
		Vector2 Vec2;
		Vector3 Vec3;
	};

public:
	operator DirectX::XMMATRIX() const
	{
#ifdef X64
		return DXMAT;
#else
		// 16바이트 정렬로 변경한걸 리턴.
		return DirectX::XMLoadFloat4x4(&DXF4);
#endif
	}

public:
	Matrix& operator=(float _Value) 
	{
		// 32비트에서는 128짜리를 지원하지 않는다.
		// int128 Test;

		for (size_t i = 0; i < 16; i++)
		{
			Arr1D[i] = _Value;
		}
		return *this;
	}

	Matrix operator*(const Matrix& _Other)
	{
		Matrix NewMat;
#ifdef X64
		NewMat.DXMAT = DirectX::XMMatrixMultiply(DXMAT, _Other.DXMAT);
#else
		DirectX::XMStoreFloat4x4(&NewMat.DXF4, DirectX::XMMatrixMultiply(*this, _Other));
#endif

		return NewMat;
	}



	Vector4 operator*(const Vector4& _Other) 
	{
		Vector4 NewVec;
		for (size_t y = 0; y < 1; y++)
		{
			for (size_t x = 0; x < 4; x++)
			{
				float Sum = 0.0f;
				for (size_t i = 0; i < 4; i++)
				{
					Sum += _Other.Arr[i] * Arr2D[i][x];
				}

				NewVec.Arr[x] = Sum;
			}
		}

		return NewVec;
	}


	Matrix& operator = (const DirectX::XMMATRIX& _Other)
	{
#ifdef X64
		DXMAT = _Other;
#else
		DirectX::XMStoreFloat4x4(&DXF4, _Other);
#endif
		return *this;
	}


	Matrix& operator*=(const Matrix& _Other)
	{
#ifdef X64
		DXMAT = DirectX::XMMatrixMultiply(DXMAT, _Other.DXMAT);
#else
		DirectX::XMStoreFloat4x4(&DXF4, DirectX::XMMatrixMultiply(*this, _Other));
#endif
		return *this;
	}

public:
	Vector4 Foword() 
	{
		return ArrVec[2];
	}

	Vector4 Up() 
	{
		return ArrVec[1];
	}

	Vector4 Right() 
	{
		return ArrVec[0];
	}


public:

	void Identity()
	{
#ifdef X64
		DXMAT = DirectX::XMMatrixIdentity();
#else
		DirectX::XMStoreFloat4x4(&DXF4, DirectX::XMMatrixIdentity());
#endif
	}

	void Inverse()
	{
#ifdef X64
		DXMAT = DirectX::XMMatrixInverse(nullptr, DXMAT);
#else
		DirectX::XMStoreFloat4x4(&DXF4, DirectX::XMMatrixInverse(nullptr, DXMAT));
#endif
		//Arr2D[0][0] = cosf(_Rad);
		//Arr2D[0][2] = -sinf(_Rad);
		//Arr2D[2][0] = sinf(_Rad);
		//Arr2D[2][2] = cosf(_Rad);
	}

	void Translation(const Vector4& _Other)
	{
		Identity();
#ifdef X64
		DXMAT = DirectX::XMMatrixTranslation(_Other.x, _Other.y, _Other.z);
#else
		DirectX::XMStoreFloat4x4(&DXF4, DirectX::XMMatrixTranslation(_Other.x, _Other.y, _Other.z));
#endif

		//Arr2D[3][0] = _Other.x;
		//Arr2D[3][1] = _Other.y;
		//Arr2D[3][2] = _Other.z;
	}

	void Scaling(const Vector4& _Other)
	{
		Identity();
#ifdef X64
		DXMAT = DirectX::XMMatrixScaling(_Other.x, _Other.y, _Other.z);
#else
		DirectX::XMStoreFloat4x4(&DXF4, DirectX::XMMatrixScaling(_Other.x, _Other.y, _Other.z));
#endif
	}

	void RotateX_Degree(const float& _Deg)
	{
		return RotateX_Radian(_Deg * Constant::D2R);
	}

	void RotateX_Radian(const float& _Rad) 
	{
		Identity();

#ifdef X64
		DXMAT = DirectX::XMMatrixRotationX(_Rad);
#else
		DirectX::XMStoreFloat4x4(&DXF4, DirectX::XMMatrixRotationX(_Rad));
#endif
		//Arr2D[1][1] = cosf(_Rad);
		//Arr2D[1][2] = sinf(_Rad);
		//Arr2D[2][1] = -sinf(_Rad);
		//Arr2D[2][2] = cosf(_Rad);
	}

	void RotateY_Degree(const float& _Deg)
	{
		return RotateY_Radian(_Deg * Constant::D2R);
	}

	void RotateY_Radian(const float& _Rad)
	{
		Identity();

#ifdef X64
		DXMAT = DirectX::XMMatrixRotationY(_Rad);
#else
		DirectX::XMStoreFloat4x4(&DXF4, DirectX::XMMatrixRotationY(_Rad));
#endif
		//Arr2D[0][0] = cosf(_Rad);
		//Arr2D[0][2] = -sinf(_Rad);
		//Arr2D[2][0] = sinf(_Rad);
		//Arr2D[2][2] = cosf(_Rad);
	}

	void RotateZ_Degree(const float& _Deg)
	{
		return RotateZ_Radian(_Deg * Constant::D2R);
	}

	void RotateZ_Radian(const float& _Rad)
	{
		Identity();
#ifdef X64
		DXMAT = DirectX::XMMatrixRotationZ(_Rad);
#else
		DirectX::XMStoreFloat4x4(&DXF4, DirectX::XMMatrixRotationZ(_Rad));
#endif
		//Arr2D[0][0] = cosf(_Rad);
		//Arr2D[0][1] = sinf(_Rad);
		//Arr2D[1][0] = -sinf(_Rad);
		//Arr2D[1][1] = cosf(_Rad);
	}

	void RotationDegree(const Vector4& _Deg)
	{
		RotationRadian(_Deg * Constant::D2R);
	}

	void RotationRadian(const Vector4& _Rad)
	{
		Identity();

#ifdef X64
		DXMAT = DirectX::XMMatrixRotationRollPitchYaw(_Rad.x, _Rad.y, _Rad.z);
#else
		DirectX::XMStoreFloat4x4(&DXF4, DirectX::XMMatrixRotationRollPitchYaw(_Rad.x, _Rad.y, _Rad.z));
#endif

		//HMAT X;
		//HMAT Y;
		//HMAT Z;

		//X.DRotX(_Rad.x);
		//Y.RotateY_Degree(_Rad.y);
		//Z.RotateZ_Degree(_Rad.z);

		//*this = X * Y * Z;
	}

	// XYZ -> 디그리 오일러 값넣어준다.
	// -> 라디안 오일러 값으로
	// -> 쿼터니언으로 변경.
	// -> 그 쿼터니언으로 회전.
	void QuaternionRotate_Degree(const Vector4& _Deg)
	{
		Vector4 Rot = (_Deg * Constant::D2R);
		
		
		RtoQRot(Rot);
	}

	void QuaternionRotate_Radian(const Vector4& _Rad)
	{
#ifdef X64
		DXMAT = DirectX::XMMatrixRotationQuaternion(_Rad);
#else
		DirectX::XMStoreFloat4x4(&DXF4, DirectX::XMMatrixRotationQuaternion(_Rad));
#endif
	}

	void RtoQRot(const Vector4& _Rad)
	{
#ifdef X64
		DXMAT = DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYaw(_Rad.x, _Rad.y, _Rad.z));
#else
		DirectX::XMStoreFloat4x4(&DXF4, DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYaw(_Rad.x, _Rad.y, _Rad.z)));
#endif
	}

	void TransPose() 
	{
#ifdef X64
		DXMAT = DirectX::XMMatrixTranspose(*this);
#else
		DirectX::XMStoreFloat4x4(&DXF4, DirectX::XMMatrixTranspose(*this));
#endif

		//float Temp;
		//for (size_t y = 0; y < 4; y++)
		//{
		//	for (size_t x = y; x < 4; x++)
		//	{
		//		//if (y == x)
		//		//{
		//		//	continue;
		//		//}

		//		Temp = Arr2D[x][y];
		//		Arr2D[x][y] = Arr2D[y][x];
		//		Arr2D[y][x] = Temp;
		//	}
		//}
	}

	void SetViewMatrix(Vector4 _EyePos, Vector4 _EyeDir, Vector4 _EyeUp)
	{
		Identity();

#ifdef X64
		DXMAT = DirectX::XMMatrixLookToLH(_EyePos, _EyeDir, _EyeUp);
#else
		DirectX::XMStoreFloat4x4(&DXF4, DirectX::XMMatrixLookToLH(_EyePos, _EyeDir, _EyeUp));
#endif


		// 줌인 줌아웃 만들어 보세요.
		// 만들어보고 안되면 오세요.

		// View행렬은
		// 누구의 역행렬인가?
		// 카메라 그자체의 행렬의 역행렬이다.

		// _Dir의 벡터로서의 길이 1일것만 보장되면 된다.
		// _Up의 벡터로서의 길이가 1일것만 보장되면 된다.

		// _Pos카메라의 위치
		// _Dir바라보는 방향
		// Y축 기저벡터
		// _Up바라보는 방향에서 x축으로 90도 돈 벡터
		//_EyeDir.Normal3D();
		//_EyeUp.Normal3D();
		//ArrVec[2] = _EyeDir;
		//ArrVec[1] = _EyeUp;
		//// 외적 의 속성은 2개의 벡터를 넣어주면 
		//// 두벡터 모두에 수직인 벡터를 리턴해주는 공식이다.
		//ArrVec[0] = HVEC4::Cross3D(ArrVec[1], ArrVec[2]);

		//TransPose();


		//// 월드를 기준으로 이동하던 오브젝트들이
		//// 뷰행렬은 로컬을 기준으로 이동을 하기 때문에.
		//// ArrVec[3] = -_EyePos;
		//ArrVec[3] = 
		//{
		//	// ArrVec[0~2]의 크기가 1이므로 정사영을 내린것이다.
		//	// 내적의 정사영을 이해못하면 뷰행렬도 이해할수 없다.
		//	-HVEC4::Dot3D(ArrVec[0], _EyePos), // 
		//	-HVEC4::Dot3D(ArrVec[1], _EyePos), // 0.0F;
		//	-HVEC4::Dot3D(ArrVec[2], _EyePos), // 
		//	1.0f,
		//};
		// 전치행렬 
		// 12 -> 21
	}


	// 원근투영행렬
	// 원근감 행렬
	// Z축에 의해서 X랑 Y 가 바뀌는 행렬이 이녀석
	// float _Width 윈도우 너비 아닌다. (윈도우 너비를 넣어줄 거지만)
	// 아니어도 된다.
	// _Near 근평면
	// _Far 원평면
	void SetOrthgonalMatrix(float _Width, float _Height, float _Near, float _Far)
	{
		Identity();
		// 지금 사용하려는 함수 내부에서 하고 할거다.

		// 이런함수들이 문제다.
		// 이 함수들이 내부에서 좀 특별한 연산을 한다.
		// SSE 연산이라고 하는데.
		// FLOAT4개나 4X4 
		// 이 연산을 사용하려면 16바이트로 데이터 메모리가 정렬되어 있어야 한다.

#ifdef X64
		DXMAT = DirectX::XMMatrixOrthographicLH(_Width, _Height, _Near, _Far);
#else
		DirectX::XMStoreFloat4x4(&DXF4, DirectX::XMMatrixOrthographicLH(_Width, _Height, _Near, _Far));
#endif
	}

	// Normal
	Vector4 MulZero(Vector4 _Value)
	{
		_Value.w = 0.0f;
		_Value = DirectX::XMVector4Transform(_Value, DXMAT);
		return _Value;
	}

	// coord
	Vector4 MulOne(Vector4 _Value)
	{
		_Value.w = 1.0f;
		_Value = DirectX::XMVector3TransformCoord(_Value, DXMAT);
		return _Value;
	}

	// coord
	Vector4 Mul(Vector4 _Value)
	{
		_Value = DirectX::XMVector4Transform(_Value, DXMAT);
		return _Value;
	}

	void SetPerspectiveMatrix(float _Fov, float _Width, float _Height, float _Near, float _Far)
	{
		Identity();
		// 수직 시야각
		float A = _Fov * Constant::D2R;
		float R = _Width / _Height;
		// 수평 시야각
		// 모든 애들은 -1 ~ 1 공간으로 간다.
		// z는 0~ 1공간으로 간다.
		// 여기에서 왜? Z가 없을까?
		// 그 정해져 있ㅎ다.
		// Z가 2번 영향을 미치게 된다.
		// 오브젝트 각자각자의 
		// / Z만 하면 끝나는데 / Z

		// 1. 각 정점의 Z가 필요한데. 여기서?????
		// 못구해 그러므로
		// 2. 각정점의 Z를 W에다가 보관넣는 식이. Arr2D[2][3] = 1.0f;
		// 3. 즉 여기에서는 뭐만 빼놓은 식이 나온거냐???
		// 비율이기 때문에 / Z를 해야 최종 수치가 나오는데.
		// 그 Z를 쓰면 그자체가 되기 때문에
		// Z만 뺀 식을 만들고
		// Z는 이 다음에 바로
		// 나눠주게 된다.

		// X
		Arr2D[0][0] = 1 / (R * tanf(A / 2)); // * X / Z : 200.0F
		// Y
		Arr2D[1][1] = 1 / (tanf(A / 2)); // * Y / Z : 200.0F

		// 200.0F에 있었으면
		// 원래 Z가 있어야 한다.
		// -0.5~0.5f 
		Arr2D[2][2] = _Far/ (_Far - _Near); // / Z
		// + 0~0.5f
		Arr2D[3][2] = -(_Near * _Far) / (_Far - _Near);

		// z만 살려서 마지막에 적용시킨다.
		Arr2D[2][3] = 1.0f;

		// 이거 설명
		// 이거 안하면 w에 1값이 더해진다.
		// w에 z를 담아놨는데 0.5 * 50 25가 나와야 하는데 26이 나와.
		Arr2D[3][3] = 0.0f;
	}

	// -1 ~ 1 로 쪼글아 들은 정점들을 2D 화면에 맞게 펼쳐주는 용도이다.
	// 최종 2D 화면 
	void SetViewPortMatrix(float _StartX, float _StartY, float _Width, float _Height, float _MinZ, float _MaxZ)
	{
		Identity();
		Arr2D[0][0] = _Width / 2;
		Arr2D[1][1] = -(_Height / 2); // 이게 Y축을 반전시킨다.
		Arr2D[2][2] = _MaxZ - _MinZ;

		// 위치랑도 관련있다.
		Arr2D[3][0] = _StartX + Arr2D[0][0];
		Arr2D[3][1] = _StartY + (_Height / 2);
		Arr2D[3][2] = _MinZ;
	}

	void GetWorldMatrix(Vector4 _Scale, Vector4 _Rot, Vector4 _Pos)
	{
		Matrix Scale;
		Matrix Rot;
		Matrix Pos;

		Scale.Scaling(_Scale);
		Rot.RotationDegree(_Rot);
		Pos.Translation(_Pos);

		*this = Scale * Rot * Pos;
	}

public:
	Matrix() 
	{
		Identity();
	}

	Matrix(const DirectX::XMMATRIX& _Other) 
	{ 
#ifdef X64
		DXMAT = _Other;
#else
		DirectX::XMStoreFloat4x4(&DXF4, _Other);
#endif
	}

};
//
//class HMATH
//{
//private:
//	static bool RectToRect();
//
//public:
//	HMATH();
//	~HMATH();
//};
//
