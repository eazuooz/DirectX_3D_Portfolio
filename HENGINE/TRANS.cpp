#include "TRANS.h"
#include "ACTOR.h"


Transform::Transform() : 
	m_LocalScale(1.0f), 
	m_RotCheck(true), 
	m_PosCheck(true), 
	m_ScaleCheck(true), 
	m_bUpdate(false),
	m_pParentTrans(nullptr)
{
}

Transform::~Transform()
{
}

void Transform::Init() 
{
	if (nullptr != GetActor()->GetTransform())
	{
		BOOM;
	}

	GetActor()->SetTransform(this);
}

void Transform::UpdateCheck() 
{
	m_PosCheck = true;
	m_RotCheck = true;
	m_ScaleCheck = true;
	m_bUpdate = true;

	if (nullptr != GetActor()->m_ParentActor && 
		true == GetActor()->m_ParentActor->IsUpdate() &&
		nullptr != GetActor()->m_ParentActor->GetTransform() )
	{
		GetActor()->m_ParentActor->GetTransform()->UpdateCheck();
	}
	
}

void Transform::UpdateTransform()
{
	if (true == m_ScaleCheck)
	{
		m_ScaleMat.Scaling(m_LocalScale);
		m_bUpdate = true;
		m_ScaleCheck = false;
	}

	if (true == m_RotCheck)
	{
		m_ROTX.RotateX_Degree(m_LocalRot.x);
		m_ROTY.RotateY_Degree(m_LocalRot.y);
		m_ROTZ.RotateZ_Degree(m_LocalRot.z);
		// m_ROTMAT = m_ROTX * m_ROTY * m_ROTZ;

		// m_Rot.x
		// m_Rot.y
		// m_Rot.z
		// X Y Z 가 모두 회전한 회전행렬 
		// -> X Y Z가 분리된 회전행렬을 만들어 낸다.
		m_RotMat.QuaternionRotate_Degree(m_LocalRot);

		m_bUpdate = true;
		m_RotCheck = false;
	}

	if (true == m_PosCheck)
	{
		m_PosMat.Translation(m_LocalPos);

		m_bUpdate = true;
		m_PosCheck = false;
	}

	// 크기와 회전은 분리해 내기 힘들다. 
	if (true == m_bUpdate && nullptr == m_pParentTrans)
	{
		m_WorldMatrix = m_ScaleMat * m_RotMat * m_PosMat;
	}
	else if (nullptr != m_pParentTrans && true == m_pParentTrans->m_bUpdate)
	{
		m_ParentMatrix = m_pParentTrans->GetWorldMatrix();
		m_WorldMatrix = m_ScaleMat * m_RotMat * m_PosMat * m_ParentMatrix;
	}

	if (nullptr != m_pParentTrans && true == m_pParentTrans->m_bUpdate)
	{
		m_Position = m_WorldMatrix.ArrVec[3];
		m_Rotation = m_pParentTrans->GetLocalRotation() + GetLocalRotation();
		m_Scale = m_pParentTrans->GetLocalScale() * GetLocalScale();
		m_bUpdate = true;
	}
	else if (true == m_bUpdate
		&& nullptr == m_pParentTrans)
	{
		m_Position = m_WorldMatrix.ArrVec[3];
		m_Rotation = GetLocalRotation();
		m_Scale = GetLocalScale();
		m_bUpdate = true;
	}
}

void Transform::RenderBefore()
{
	// 카메라가 있어주는게 좋겠네요.
	UpdateTransform();
}

void Transform::RenderAfter() 
{
	m_bUpdate = false;
}

void Transform::UpdateTransToSelect(bool _Scale, bool _Rot, bool _Pos) 
{
	if (true == m_ScaleCheck)
	{
		m_ScaleMat.Scaling(m_LocalScale);
		m_bUpdate = true;
		m_ScaleCheck = false;
	}

	if (true == m_RotCheck)
	{
		m_ROTX.RotateX_Degree(m_LocalRot.x);
		m_ROTY.RotateY_Degree(m_LocalRot.y);
		m_ROTZ.RotateZ_Degree(m_LocalRot.z);
		// m_ROTMAT = m_ROTX * m_ROTY * m_ROTZ;

		// m_Rot.x
		// m_Rot.y
		// m_Rot.z
		// X Y Z 가 모두 회전한 회전행렬 
		// -> X Y Z가 분리된 회전행렬을 만들어 낸다.
		m_RotMat.QuaternionRotate_Degree(m_LocalRot);

		m_bUpdate = true;
		m_RotCheck = false;
	}

	if (true == m_PosCheck)
	{
		m_PosMat.Translation(m_LocalPos);

		m_bUpdate = true;
		m_PosCheck = false;
	}

	// 크기와 회전은 분리해 내기 힘들다. 
	if (true == m_bUpdate && nullptr == m_pParentTrans)
	{
		m_WorldMatrix = m_ScaleMat * m_RotMat * m_PosMat;
	}
	else if (nullptr != m_pParentTrans && true == m_pParentTrans->m_bUpdate)
	{
		m_ParentMatrix = m_pParentTrans->WMATToSelect(_Scale, _Rot, _Pos);
		m_WorldMatrix = m_ScaleMat * m_RotMat * m_PosMat * m_ParentMatrix;
	}

	if (nullptr != m_pParentTrans && m_pParentTrans->m_bUpdate)
	{
		m_Position = m_WorldMatrix.ArrVec[3];
		m_Rotation = m_pParentTrans->GetLocalRotation() + GetLocalRotation();
		m_Scale = m_pParentTrans->GetLocalScale() * GetLocalScale();
		m_bUpdate = true;
	}
	else if (true == m_bUpdate
		&& nullptr == m_pParentTrans)
	{
		m_Position = m_WorldMatrix.ArrVec[3];
		m_Rotation = GetLocalRotation();
		m_Scale = GetLocalScale();
		m_bUpdate = true;
	}
}