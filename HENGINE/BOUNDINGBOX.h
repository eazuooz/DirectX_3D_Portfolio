#pragma once
#include "COL.h"
// 2틀 내가 만들었으면의 4배 
// 아무리 많아도 괜찮다.
// #include ""

class BoundingBox : public Collider
{
public:
	friend class BoundingSphere;

private:
	DirectX::BoundingOrientedBox m_ColliderData;
	Vector4 Rot;

public:
	void ColBefore() override;
	bool GeometryCheck(Collider* _Other) override;

public:
	BoundingBox();
	~BoundingBox();
};

