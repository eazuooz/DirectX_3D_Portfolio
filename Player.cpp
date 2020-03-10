#include "Player.h"

Player Player::m_inst = Player();

Player::Player()
{

}

Player::~Player()
{
}

std::wstring Player::GetName(int _Idx)
{
	if (_Idx > 0 && _Idx <= 22)
	{
		return spriteNameMap.find(_Idx)->second;
	}
	
	return nullptr;
}

void Player::Init()
{




	spriteNameMap.insert(std::map<int, std::wstring>::value_type(1, L"Unicorn.png"));
	spriteNameMap.insert(std::map<int, std::wstring>::value_type(2, L"GodofWar.png"));
	spriteNameMap.insert(std::map<int, std::wstring>::value_type(3, L"SwordMaster.png"));
	spriteNameMap.insert(std::map<int, std::wstring>::value_type(4, L"ManaMagician.png"));
	spriteNameMap.insert(std::map<int, std::wstring>::value_type(5, L"LittleDragon.png"));
	spriteNameMap.insert(std::map<int, std::wstring>::value_type(6, L"ShiningArcher.png"));
	spriteNameMap.insert(std::map<int, std::wstring>::value_type(7, L"BladeKnight.png"));
	spriteNameMap.insert(std::map<int, std::wstring>::value_type(8, L"DemonLarva.png"));
	spriteNameMap.insert(std::map<int, std::wstring>::value_type(9, L"Warewolf.png"));
	spriteNameMap.insert(std::map<int, std::wstring>::value_type(10, L"FireMagician.png"));
	spriteNameMap.insert(std::map<int, std::wstring>::value_type(11, L"WindRanger.png"));
	spriteNameMap.insert(std::map<int, std::wstring>::value_type(12, L"ThunderSpirit.png"));
	spriteNameMap.insert(std::map<int, std::wstring>::value_type(13, L"ToenailDruid.png"));
	spriteNameMap.insert(std::map<int, std::wstring>::value_type(14, L"Siren.png"));
	spriteNameMap.insert(std::map<int, std::wstring>::value_type(15, L"PirateCaptain.png"));
	spriteNameMap.insert(std::map<int, std::wstring>::value_type(16, L"TortolanElder.png"));
	spriteNameMap.insert(std::map<int, std::wstring>::value_type(17, L"DoomArbiter.png"));
	spriteNameMap.insert(std::map<int, std::wstring>::value_type(18, L"TideHunter.png"));
	spriteNameMap.insert(std::map<int, std::wstring>::value_type(19, L"Egg.png"));
	spriteNameMap.insert(std::map<int, std::wstring>::value_type(20, L"GodofThunder.png"));
	spriteNameMap.insert(std::map<int, std::wstring>::value_type(21, L"ShadowDemon.png"));
	spriteNameMap.insert(std::map<int, std::wstring>::value_type(22, L"WalrusChamp.png"));
}

void Player::Update()
{
}


