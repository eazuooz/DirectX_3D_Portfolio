#pragma once
#include "LOGIC.h"
#include "CAMERA.h"

class FreeCamera : public Logic
{
private:
	Vector4 m_StartPos;
	Vector4 m_StartRot;
	bool isInit;

	// HPTR<HCAM> m_HCAM;
	float m_Speed;
	float m_RotXSpeed;
	float m_RotYSpeed;

	HPTR<Transform> m_Trace;

public:
	void Trace(HPTR< Transform> _Trace, Vector4 _Pos, Vector4 _Rot);

public:
	void Init() override;
	void Update() override;


public:
	FreeCamera();
	~FreeCamera();
};

