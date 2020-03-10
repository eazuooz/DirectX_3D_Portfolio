#include "StatusLogic.h"

#include  "LOGICHEADER.h"

StatusLogic::StatusLogic() : hp(100), mp(0), att(0), curTime(500.0f),
continueTime(0.0f), type(PIECETYPE::ALLY), statCBData({ 0.0f, 0.0f, 1.0f }), pieceIdx(-1), logicDie(false), piece_NUMBER(-1)
{

}

//StatusLogic::StatusLogic(int _ATT) : hp(100), mp(0), att(_ATT), curTime(0.0f), maxTime(0.0f)
//{
//
//}


StatusLogic::~StatusLogic()
{

}

void StatusLogic::SetStatus(const float _HP, const float _ATT, const float _CoolTime,const float _MaxTime, int _Idx)
{
	maxHp = _HP * 2.0f;
	hp		= _HP * 2.0f;
	att		= _ATT ;
	coolTime = _CoolTime;
	continueTime = _MaxTime;
	maxMp = 100.0f;

	pieceIdx = _Idx;

	//statCBData.x = 1.0f;
//statCBData.z = 1.0f;
	int level = GetActor()->GetLevel();
	int spriteIdx = 0;
	if (level == 1)
	{
		spriteIdx = 1;
	}
	else if (level == 2)
	{
		spriteIdx = 2;
	}
	else if (level == 3)
	{
		spriteIdx = 3;
	}

	if (type == PIECETYPE::ENEMY)
	{

	}
	else
	{
		starSprite = GetActor()->AddComponent<SpriteRenderer>(RG_PLAYER);

		starSprite->SetSprite(L"Stars.png", spriteIdx);
		starSprite->RenderData(L"DTEXMESH", L"SPRITECOOLPATH");
		starSprite->LinkCB(L"VS_CUTDATA", &(starSprite->GetCutData()));
		starSprite->LinkCB(L"PS_STATDATA", &statCBData);
		starSprite->LinkTexture(L"PS_MAPTEX", L"Stars.png");
		starSprite->LinkSampler(L"PS_LSMP", L"LSMP");
		starSprite->LinkSampler(L"PS_PSMP", L"PSMP");

		starSprite->SetLoclaPosition({ 0.0F, 180.0F, 0.0f });
		starSprite->SetLocalScale({ 30.0F, 30.0F, 1.0F, 1.0f });
		starSprite->BillBoardOn();
	}

	hpSprite = GetActor()->AddComponent<SpriteRenderer>(RG_PLAYER);

	hpSprite->SetSprite(L"HP_Prograss.png");
	hpSprite->RenderData(L"DTEXMESH", L"SPRITEHPPATH");
	hpSprite->LinkCB(L"VS_CUTDATA", &(hpSprite->GetCutData()));
	hpSprite->LinkCB(L"PS_STATDATA", &statCBData);
	hpSprite->LinkTexture(L"PS_MAPTEX", L"HP_Prograss.png");
	hpSprite->LinkSampler(L"PS_LSMP", L"LSMP");
	hpSprite->LinkSampler(L"PS_PSMP", L"PSMP");

	hpSprite->SetLoclaPosition({ 0.0F, 160.0F, 0.0f });
	hpSprite->SetLocalScale({ 50.0F, 5.0F, 1.0F, 1.0f });
	hpSprite->BillBoardOn();

	mpSprite = GetActor()->AddComponent<SpriteRenderer>(RG_PLAYER);

	mpSprite->SetSprite(L"MP_Prograss.png");
	mpSprite->RenderData(L"DTEXMESH", L"SPRITEMPPATH");
	mpSprite->LinkCB(L"VS_CUTDATA", &(hpSprite->GetCutData()));
	mpSprite->LinkCB(L"PS_STATDATA", &statCBData);
	mpSprite->LinkTexture(L"PS_MAPTEX", L"MP_Prograss.png");
	mpSprite->LinkSampler(L"PS_LSMP", L"LSMP");
	mpSprite->LinkSampler(L"PS_PSMP", L"PSMP");

	mpSprite->SetLoclaPosition({ 0.0F, 155.0F, 0.0f });
	mpSprite->SetLocalScale({ 50.0F, 5.0F, 1.0F, 1.0f });
	mpSprite->BillBoardOn();


	coolSprite = GetActor()->AddComponent<SpriteRenderer>(RG_PLAYER);

	coolSprite->SetSprite(L"COOL_Prograss.png");
	coolSprite->RenderData(L"DTEXMESH", L"SPRITECOOLPATH");
	coolSprite->LinkCB(L"VS_CUTDATA", &(hpSprite->GetCutData()));
	coolSprite->LinkCB(L"PS_STATDATA", &statCBData);
	coolSprite->LinkTexture(L"PS_MAPTEX", L"COOL_Prograss.png");
	coolSprite->LinkSampler(L"PS_LSMP", L"LSMP");
	coolSprite->LinkSampler(L"PS_PSMP", L"PSMP");

	coolSprite->SetLoclaPosition({ 0.0F, 150.0F, 0.0f });
	coolSprite->SetLocalScale({ 50.0F, 5.0F, 1.0F, 1.0f });
	coolSprite->BillBoardOn();
}

void StatusLogic::SetNUMBER(int _num)
{
	piece_NUMBER = _num;
}

void StatusLogic::Init()
{
	


	



	



}

void StatusLogic::Update()
{
	
	if (hp <= 0.0f)
	{
		statCBData.x = 0.0f;
	}
	else
	{
		statCBData.x = (hp / maxHp);
	}
	
	if (mp <= 0.0f)
	{
		statCBData.y = 0.0f;
	}
	else
	{
		statCBData.y = (mp / maxMp);
	}
	
	curTime += Time::DeltaTime();
	if (curTime < coolTime)
	{
		statCBData.z = (curTime / coolTime);
	}
	else if(curTime > coolTime)
	{
		statCBData.z = 1.0f;
	}
	

	hpSprite->LinkCB(L"PS_STATDATA", &statCBData);
	mpSprite->LinkCB(L"PS_STATDATA", &statCBData);
	coolSprite->LinkCB(L"PS_STATDATA", &statCBData);
}