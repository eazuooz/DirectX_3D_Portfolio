#include "BOUNDINGBOX.h"
#include "TRANS.h"
#include "BOUNDINGSPHERE.h"
#include "GRAPHICDEBUG.h"


BoundingBox::BoundingBox()
{
}


BoundingBox::~BoundingBox()
{
}


void BoundingBox::ColBefore()
{
	// z값에 따라서 안보일수 있다.

	DebugCheck();
	UpdateTransform();
	// z값에 따라서 안보일수 있다.

	m_ColliderData.Center = GetPosition().xmFloat3;
	m_ColliderData.Extents = (GetScale() * 0.5f).xmFloat3;
	// 사원수에 대해서 설명해준다.
	// 복소수 기반의 회전 행렬3X3의 축소판
	Rot = GetTransform()->GetRadRotation();
	DirectX::XMStoreFloat4(&m_ColliderData.Orientation, DirectX::XMQuaternionRotationRollPitchYaw(Rot.x, Rot.y, Rot.z));
	//Rot = Trans()->WRot();
	//DirectX::XMStoreFloat4(&ColData.Orientation, DirectX::XMQuaternionRotationRollPitchYaw(Rot.x, Rot.y, Rot.z));

	if (GraphicDebug::IsDebugCheck())
	{
		GraphicDebug::DrawBox(Camera::GetMainCam(), GetWorldMatrix());
	}
}

bool BoundingBox::GeometryCheck(Collider* _Other)
{
	if (true == _Other->IsType<BoundingBox>())
	{
		return m_ColliderData.Intersects(((BoundingBox*)_Other)->m_ColliderData);
	} else if (true == _Other->IsType<BoundingSphere>())
	{
		return m_ColliderData.Intersects(((BoundingSphere*)_Other)->m_ColliderData);
	}

	return nullptr;
}