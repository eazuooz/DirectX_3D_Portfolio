#include "HMESH.h"

HMESH::HMESH()
{
}


HMESH::~HMESH()
{
}

void HMESH::UpdateAllVB() 
{
	GraphicDevice::MainContext()->IASetVertexBuffers(0, (unsigned int)m_ArrVBBuffer.size(), &m_ArrVBBuffer[0], &m_ArrVBSize[0], &m_ArrVBOffSet[0]);
}

void HMESH::UpdateVB(UINT _Index) 
{
	GraphicDevice::MainContext()->IASetVertexBuffers(0, 1, &m_ArrVBBuffer[_Index], &m_ArrVBSize[_Index], &m_ArrVBOffSet[_Index]);
}

void HMESH::UpdateIB(UINT _Index) 
{
	GraphicDevice::MainContext()->IASetIndexBuffer(m_ArrIBBuffer[_Index], m_ArrIB[_Index]->IBType(), m_ArrIBOffSet[_Index]);
}

void HMESH::UpdateIB() {
	GraphicDevice::MainContext()->IASetIndexBuffer(m_ArrIBBuffer[0], m_ArrIB[0]->IBType(), m_ArrIBOffSet[0]);
}

void HMESH::Update(UINT _VtxIndex, UINT _IdxIndex) 
{
	UpdateVB(_VtxIndex);
	UpdateIB(_IdxIndex);
}

// 인스턴싱용 버텍스 버퍼가 몇번째인지 알아야 가능하다.
void HMESH::RenderInstancing(UINT _IdxIndex)
{
	if (nullptr == m_INSTBUFFER)
	{
		BOOM;
	}

	GraphicDevice::MainContext()->DrawIndexedInstanced(
		m_ArrIB[_IdxIndex]->IDXCount(),
		m_INSTBUFFER->CurDataCount(),
		0, 0, 0);
}

void HMESH::Render(UINT _IdxIndex) 
{
	GraphicDevice::MainContext()->DrawIndexed(m_ArrIB[_IdxIndex]->IDXCount(), 0, 0);
}

void HMESH::RenderAuto()
{
	GraphicDevice::MainContext()->DrawAuto();
}

void HMESH::AddVB(const wchar_t* _Name)
{
	HPTR<HVBBUFFER> VBUFFER = Resources::Find<HVBBUFFER>(_Name);

	if (nullptr == VBUFFER)
	{
		BOOM;
	}


	m_ArrVBBuffer.push_back(VBUFFER->Buffer());
	m_ArrVBOffSet.push_back(0);
	m_ArrVBSize.push_back(VBUFFER->VTXSize());

	m_ArrVB.push_back(VBUFFER);
}

void HMESH::AddINST(const wchar_t* _Name)
{
	if (nullptr != m_INSTBUFFER)
	{
		BOOM;
	}

	HPTR<HINSTBUFFER> INSTBUFFER = Resources::Find<HINSTBUFFER>(_Name);

	if (nullptr == INSTBUFFER)
	{
		BOOM;
	}

	m_INSTBUFFER = INSTBUFFER;
	m_ArrVBBuffer.push_back(INSTBUFFER->Buffer());
	m_ArrVBOffSet.push_back(0);
	m_ArrVBSize.push_back(INSTBUFFER->VTXSize());

	m_ArrVB.push_back(INSTBUFFER.PTR);
}

void HMESH::AddIB(const wchar_t* _Name)
{
	HPTR<HIBBUFFER> IBUFFER = Resources::Find<HIBBUFFER>(_Name);

	if (nullptr == IBUFFER)
	{
		BOOM;
	}

	m_ArrIBBuffer.push_back(IBUFFER->Buffer());
	m_ArrIBOffSet.push_back(0);
	m_ArrIB.push_back(IBUFFER);
}

