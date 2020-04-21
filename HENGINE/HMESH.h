#pragma once
#include "ENGINEMESHDATA.h"
#include "GRAPHICDEVICE.h"
#include <vector>
#include "HVBBUFFER.h"
#include "HIBBUFFER.h"
#include "HINSTBUFFER.h"

class HMESH : public Resources
{
private:

private:
	//class HINSTANCINGBUFFER : public VBUFFER
	//{
	//};


private:
	// 인풋 어셈블러에 세팅하는 용도
	std::vector<HPTR<HIBBUFFER>> m_ArrIB;
	std::vector<HPTR<HVBBUFFER>> m_ArrVB;

	std::vector<ID3D11Buffer*> m_ArrVBBuffer;
	std::vector<unsigned int> m_ArrVBOffSet; // 시작값
	std::vector<unsigned int> m_ArrVBSize;

	std::vector<ID3D11Buffer*> m_ArrIBBuffer;
	std::vector<unsigned int> m_ArrIBOffSet;

	// 하나의 인스턴싱 버퍼를 가지고 있는다.
	HPTR<HINSTBUFFER> m_INSTBUFFER;

public:
	HPTR<HINSTBUFFER> INSTBUFFER() {
		return m_INSTBUFFER;
	}

public:
	bool Create()
	{
		return true;
	}

	void AddVB(const wchar_t* _Name);
	void AddINST(const wchar_t* _Name);
	void AddIB(const wchar_t* _Name);


public:
	void UpdateAllVB();
	void UpdateVB(UINT _Index);
	void UpdateIB(UINT _Index);
	void UpdateIB();

	void Update(UINT _VtxIndex, UINT _IdxIndex);

	void Render(UINT _IdxIndex);

	void RenderInstancing(UINT _IdxIndex);

	void RenderAuto();


public:
	HMESH();
	~HMESH();
};

