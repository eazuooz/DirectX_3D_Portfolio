#pragma once
#include <LOGIC.h>
#include <SPRITERENDER.h>


enum PIECETYPE
{
	ALLY,
	ENEMY,
};

class StatusLogic : public Logic
{
private:
	HPTR<SpriteRenderer> hpSprite;
	HPTR<SpriteRenderer> mpSprite;
	HPTR<SpriteRenderer> coolSprite;

	HPTR<SpriteRenderer> starSprite;

public:
	void StatusAllOff()
	{
		hpSprite->UpdateOff();
		mpSprite->UpdateOff();
		coolSprite->UpdateOff();

		if (starSprite != nullptr)
		{
			starSprite->UpdateOff();
		}
	}

private:
	bool logicDie;
	PIECETYPE  type;
	int pieceIdx;

	int piece_NUMBER;

	float maxHp;
	float maxMp;

	float hp;
	float mp;
	float att;

	//스킬 쿨타임 현재타임, 최대타임
	float curTime;
	float coolTime;
	float continueTime; //지속시간

	Vector4 statCBData;

public:
	PIECETYPE GetType()
	{
		return type;
	}

	int GetPieceIdx()
	{
		return pieceIdx;
	}

	void DefMonster()
	{
		type = PIECETYPE::ENEMY;
	}

	void LogicOff()
	{
		logicDie = true;
	}
	bool IsLogicDeath()
	{
		return logicDie;
	}
	void	SetHp(float _Hp)
	{
		hp = _Hp;
	}
	void	SetAtt(float _Att)
	{
		att = _Att;
	}
	float		GetHp()
	{
		return hp;
	}
	float		GetAtt()
	{
		return att;
	}
	void	TakeAwayHp(float _Hp)
	{
		hp -= _Hp;
	}
	void	AddHp(float _Hp)
	{
		hp += _Hp;
	}
	void	AddMp(float _Mp)
	{
		mp += _Mp;
	}
	void SetMp(float _MP)
	{
		mp = _MP;
	}

	int GetNUMBER()
	{
		return piece_NUMBER;
	}

	void InitStatus()
	{
		hp = maxHp;
		mp = 0;
		curTime = 500.0f;
		statCBData = Vector4{ 0.0f, 0.0f, 1.0f };
	}

	void InitCurTime()
	{
		curTime = 0.0f;
	}

	float		GetMp()
	{
		return mp;
	}
	float		GetCurTime()
	{
		return curTime;
	}
	float		GetCoolTime()
	{
		return coolTime;
	}
	float		GetContinueTime()
	{
		return continueTime;
	}

public:
	void SetStatus(const float _HP, const float _ATT, const float _CoolTime, const float _MaxTime, int _Idx = 0);
	void SetNUMBER(int _num);

public:
	void Init() override;
	void Update() override;

public:
	//StatusLogic(int _ATT);
	StatusLogic();
	~StatusLogic();
};

