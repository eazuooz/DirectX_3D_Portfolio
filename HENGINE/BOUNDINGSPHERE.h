#pragma once
#include "COL.h"

class BoundingSphere :
	public Collider
{
public:
	friend class BoundingBox;

private:
	DirectX::BoundingSphere m_ColliderData;

public:
	void ColBefore() override;
	bool GeometryCheck(Collider* _Other) override;


public:
	BoundingSphere();
	~BoundingSphere();
};

