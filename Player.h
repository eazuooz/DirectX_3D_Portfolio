#pragma once
#include <vector>
#include <map>
#include <ACTOR.h>


class Player
{
private:
	static Player m_inst;

public:
	static Player& Inst()
	{
		return m_inst;
	}

private:
	//std::vector<HPTR<Actor>> pieceArr;
	std::map<int, std::wstring> spriteNameMap;

public:
	std::wstring GetName(int _Idx);

public:
	void Init();
	void Update();

public:
	Player();
	~Player();
};

