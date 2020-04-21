#pragma once
#include "COL.h"
class BoundingCircle :
	public Collider
{
public:
	friend class BoudingRect;

private:
	DirectX::BoundingSphere m_ColliderData;

public:
	void ColBefore() override;
	bool GeometryCheck(Collider* _Other) override;

public:
	BoundingCircle();
	~BoundingCircle();
};

