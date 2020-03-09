#pragma once
#include <time.h>

class Well512Random
{
public:
	static Well512Random& Instance()
	{
		static Well512Random mRandGen = Well512Random();
		return mRandGen;
	}

public:
	enum __tag
	{
		DIFFER_VALUE = 100,
	};

private:
	explicit Well512Random()
		: state()
		, index(0)
	{
		//시드값 자동 생성
		unsigned int s = static_cast<unsigned int>(time(NULL));

		for (int i = 0; i < 16; i++)
		{
			state[i] = s;
			s += s + __tag::DIFFER_VALUE;
		}
	}

	//explicit Well512Random(unsigned int nSeed)
	//	: state( )
	//	, index(0)
	//{
	//	//시드 값을 받아 초기화
	//	unsigned int s = nSeed;

	//	for ( int i = 0; i < 16; i++ )
	//	{
	//		state[i] = s;
	//		s += s + __tag::DIFFER_VALUE;
	//	}
	//}

	~Well512Random() = default;

public:
	unsigned int GetValue()
	{
		unsigned int a, b, c, d;

		a = state[index];
		c = state[(index + 13) & 15];
		b = a ^ c ^ (a << 16) ^ (c << 15);
		c = state[(index + 9) & 15];
		c ^= (c >> 11);
		a = state[index] = b ^ c;
		d = a ^ ((a << 5) & 0xda442d24U);
		index = (index + 15) & 15;
		a = state[index];
		state[index] = a ^ b ^ d ^ (a << 2) ^ (b << 18) ^ (c << 28);

		return state[index];
	}

	unsigned int GetValue(unsigned int nMinValue, unsigned int nMaxValue)
	{
		return nMinValue + (GetValue() % (nMaxValue - nMinValue));
	}

	unsigned int GetValue(unsigned int nMaxValue)
	{
		return GetValue() % nMaxValue;
	}

	//Float Value
	float GetFloatValue()
	{
		unsigned int nRandValue = GetValue();

		union
		{
			unsigned long ul; float f;
		} p;
		p.ul = (((nRandValue *= 16807) & 0x007fffff) - 1) | 0x3f800000;

		return p.f - 1.0f;
	}

	float GetFloatValue(float fMinValue, float fMaxValue)
	{
		return fMinValue + GetFloatValue() * (fMaxValue - fMinValue);
	}

	float GetFloatValue(float fMaxValue)
	{
		return GetFloatValue() * fMaxValue;
	}

protected:
	unsigned long state[16];
	unsigned int index;
};
