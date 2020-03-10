#pragma once
#include <LOGIC.h>
#include <TERRAIN.h>

class TestPlayer : public Logic
{
public:
	HPTR<Terrain> m_Map;

public:
	void Init() override;
	void Update() override;
public:
	TestPlayer();
	~TestPlayer();
};

