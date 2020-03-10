#include <DIRECTORY.h>
#include <TEX.h>
#include <FBX.h>
#include <SPRITE.h>
#include <INPUT.h>
#include <SOUND.h>

#include "LOGICHEADER.h"

void LOGICVALUE::Init()
{
	Sound::Init();

	Directory Dic;
	Dic.ReplaceParentPath();
	Dic.MoveDirectory(L"BIN");
	Dic.MoveDirectory(L"TEX");
	Dic.MoveDirectory(L"LOAD");

	Path::Root;

	////////////////// 리소스 로드
	Path::CreatePath(L"TEX");
	Path::CreatePath(L"STAND", L"TEX\\stand");
	Path::CreatePath(L"LOAD", L"TEX\\LOAD");
	Path::CreatePath(L"Sound");
	Path::CreatePath(L"Mesh");


	////////////////// 키입력 이닛

	Input::CreateKey(L"LOADING",	VK_F1);
	Input::CreateKey(L"LOBY",		VK_F2);
	Input::CreateKey(L"COLLECT",	VK_F3);
	Input::CreateKey(L"START",		VK_F4);
	Input::CreateKey(L"PLAY",		VK_F5);
	Input::CreateKey(L"ENDING",		VK_F6);

	Input::CreateKey(L"LCLICK", VK_LBUTTON);
	Input::CreateKey(L"RCLICK", VK_RBUTTON);
	Input::CreateKey(L"EXIT", VK_ESCAPE);

	Input::CreateKey(L"LEFT", VK_LEFT);
	Input::CreateKey(L"RIGHT", VK_RIGHT);
	Input::CreateKey(L"UP", VK_UP);
	Input::CreateKey(L"DOWN", VK_DOWN);

	Input::CreateKey(L"GRAPHICDEBUG", VK_UP);


	Input::CreateKey(L"1", '1');
	Input::CreateKey(L"2", '2');
	Input::CreateKey(L"3", '3');
	Input::CreateKey(L"4", '4');
	Input::CreateKey(L"5", '5');

	Input::CreateKey(L"CHEAT", VK_CONTROL);
	Input::CreateKey(L"SKIP", 'K');


	Input::CreateKey(L"RELOAD", 'R');



	


	//{
	//	std::vector<HBFILE> AllFileList;
	//	Dic.MoveParent();
	//	Dic.Move(L"Mesh");

	//	Dic.AllFiles(AllFileList, L"*.FBX");
	//	std::wstring Name;
	//	for (size_t i = 0; i < AllFileList.size(); i++)
	//	{
	//		Name = AllFileList[i].FileName();

	//		HRES::Load<HFBX>(HPATH::PlusPath(L"Mesh", Name.c_str()));
	//	}
	//}
/*
	{
		std::vector<HBFILE> AllFileList;
		Dic.MoveParent();
		Dic.Move(L"Sound");

		Dic.AllFiles(AllFileList, L"*.*");
		std::wstring Name;
		for (size_t i = 0; i < AllFileList.size(); i++)
		{
			Name = AllFileList[i].FileName();

			HRES::Load<HSOUND>(HPATH::PlusPath(L"Sound", Name.c_str()));
		}
	}*/

	//Resources::Create<Sprite>(L"BUMPTEST.png", L"BUMPTEST.png", 1, 1);
}