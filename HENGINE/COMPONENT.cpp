#include "COMPONENT.h"

Component::Component() 
{
}


Component::~Component()
{
}

void Component::ComponentInit() 
{
	m_pTrans = m_pActor->GetTransform();
}

void Component::SceneChangeStart() 
{
	// 엑터에 바뀐씬이 세팅된 다음에 실행된다
	// 기억해 놓으세요.
}