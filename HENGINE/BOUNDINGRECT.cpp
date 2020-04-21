#include "BOUNDINGRECT.h"
#include "TRANS.h"
#include "GRAPHICDEBUG.h"



BoudingRect::BoudingRect()
{
}


BoudingRect::~BoudingRect()
{
}

void BoudingRect::ColBefore()
{
	DebugCheck();
	UpdateTransform();
	// z값에 따라서 안보일수 있다.

	m_ColliderData.Center = GetPosition().xmFloat3;
	m_ColliderData.Center.z = 0.0f;

	m_ColliderData.Extents = (GetScale() * 0.5f).xmFloat3;
	m_ColliderData.Extents.z = 1.0f;
	// 사원수에 대해서 설명해준다.
	// 복소수 기반의 회전 행렬3X3의 축소판
	// 회전

	Rot = GetTransform()->GetRadRotation();
	DirectX::XMStoreFloat4(&m_ColliderData.Orientation, DirectX::XMQuaternionRotationRollPitchYaw(Rot.x, Rot.y, Rot.z));

	if (GraphicDebug::IsDebugCheck())
	{
		GraphicDebug::DrawRect(Camera::GetMainCam(), GetWorldMatrix());
	}
}

bool BoudingRect::GeometryCheck(Collider* _Other)
{
	// hvec4
	// hvec2
	// 선생님은 다운캐스팅이 싫어요
	if (true == _Other->IsType<BoudingRect>())
	{
		return m_ColliderData.Intersects(((BoudingRect*)_Other)->m_ColliderData);
	}

	return nullptr;
}