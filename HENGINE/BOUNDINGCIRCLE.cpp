#include "BOUNDINGCIRCLE.h"
#include "BOUNDINGRECT.h"
#include "TRANS.h"
#include "GRAPHICDEBUG.h"
// 맵은 내부에서 충돌체와 랜더링 
// 역활을 동시에 할것인지 체크해봐라.

BoundingCircle::BoundingCircle()
{
}

BoundingCircle::~BoundingCircle()
{
}

void BoundingCircle::ColBefore()
{
// xyz중 제일 큰녀석의 절반
	DebugCheck();
	UpdateTransform();

	m_ColliderData.Center = GetTransform()->GetPosition().xmFloat3;
	m_ColliderData.Center.z = 0.0f;
	m_ColliderData.Radius = GetScale().x * 0.5f;

	if (GraphicDebug::IsDebugCheck())
	{
		GraphicDebug::DrawCircle(Camera::GetMainCam(), GetWorldMatrix());
	}
}

bool BoundingCircle::GeometryCheck(Collider* _Other)
{
	if (true == _Other->IsType<BoundingCircle>())
	{
		return m_ColliderData.Intersects(((BoundingCircle*)_Other)->m_ColliderData);
	}
	else if (true == _Other->IsType<BoudingRect>())
	{
		return m_ColliderData.Intersects(((BoudingRect*)_Other)->m_ColliderData);
	}

	return false;
}