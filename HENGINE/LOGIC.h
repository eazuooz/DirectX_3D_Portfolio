#pragma once
// 상속받을 애가.
#include "TRANS.h"
#include <JTIME.h>
#include <INPUT.h>
#include "ACTOR.h"

class Actor;
class Logic : public Component
{
public:
	Scene* GetScene();
	HPTR<Actor> CreateObject(const wchar_t* _Name = L"GameObject");

public:
	//virtual void UpdateAfter();
	//virtual void Update();
	//virtual void UpdateBefore();
	//virtual void FinalUpdate();
	//virtual void RenderBefore();
	//virtual void RenderAfter();
	//virtual void ColBefore();
	//virtual void ColAfter();
	//virtual void Release();
	//virtual void ReleaseAfter();


public:
	Logic();
	~Logic();
};

// 
//class MOVEOBJLOGIC : public HLOGIC
//{
//
//};

