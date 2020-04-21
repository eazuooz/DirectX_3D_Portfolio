#pragma once
#include <vector>
#include "TEX.h"

// 텍스처를 쪼갠 정보를 들고있을 애.
class Texture;
class Sprite : public Resources
{
private:
	HPTR<Texture> m_Tex;
	std::vector<Vector4> m_CutVec;

	int m_LastIdx;

public:
	unsigned int MultiCount() 
	{
		return (unsigned int)m_CutVec.size();
	}

	int GetLastIdx()
	{
		return m_LastIdx;
	}

public:
	// for문으로 자르는거.
	// 21 50 잘라주세요

	// SOLID 중에 인터페이스 분리원칙.

	bool Create(const wchar_t* _TexName, unsigned int _XCount, unsigned int _YCount, unsigned int _Start = -1, unsigned int _End = -1);

	// 1 12 5 자르겠다.
	void PushCutData(unsigned int _XCount, unsigned int _YCount, unsigned int _Index);
	void PushCutData(Vector4 _CutData, unsigned int _Index);

public:
	Vector4 GetCutData(unsigned int _Index);

	void Update(HSHADER::SHADERTYPE _ShaderType, int _SetIndex);


public:
	Sprite();
	~Sprite();
};

