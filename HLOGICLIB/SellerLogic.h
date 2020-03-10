#pragma once
#include <ACTOR.h>
#include <SPRITERENDER.h>
#include <2DANICOM.h>
#include "LOGICHEADER.h"

class SellerLogic 
{
private:
	static SellerLogic m_inst;

public:
	static SellerLogic& Inst()
	{
		return m_inst;
	}

private:
	class SellerData
	{
	public:
		HPTR<Actor> actor;
		HPTR<SpriteRenderer> spritePtr;
		HPTR<SpriteAnimaion> spriteAni;
		int index;


		void Release()
		{
			actor->Die();
			actor = nullptr;
			spritePtr = nullptr;
			spriteAni = nullptr;
			index = -1;
		}

		SellerData() : index(0), spritePtr(nullptr)
		{

		}
	};

private:
	Scene* scene;
	std::vector<SellerData> sellerActorArr;
	
public:
	void AllOff();
	void AllOn();

	void OffIndex(int _Idx);
	void OnIndex(int _Idx);

	bool GetIdxUpdate(int _Idx);

	int GetChessIdx(int _Idx);

public:
	void Init(Scene* _Scene);
	void Update();

public:
	void PieceChoice(int _Idx);
	void ReLoad();

	HPTR<Actor> CreatePiece(int _IDX);

public:
	SellerLogic();
	~SellerLogic();
};

