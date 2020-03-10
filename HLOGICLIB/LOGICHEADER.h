#pragma once

#ifdef X32
#ifdef DEBUGMODE
#pragma comment(lib, "\\X32\\ENGINED32.lib")
#elif RELEASEMODE
#pragma comment(lib, "\\X32\\ENGINER32.lib")
#endif
#elif X64
#ifdef DEBUGMODE
#pragma comment(lib, "\\X64\\ENGINED64.lib")
#elif RELEASEMODE
#pragma comment(lib, "\\X64\\ENGINER64.lib")
#endif
#endif

#ifdef X32
#ifdef DEBUGMODE
#pragma comment(lib, "\\X32\\LOGICLIBD32.lib")
#elif RELEASEMODE
#pragma comment(lib, "\\X32\\LOGICLIBR32.lib")
#endif
#elif X64
#ifdef DEBUGMODE
#pragma comment(lib, "\\X64\\LOGICLIBD64.lib")
#elif RELEASEMODE
#pragma comment(lib, "\\X64\\LOGICLIBR64.lib")
#endif
#endif



enum RENDERGROUP
{
	RG_GRID,
	RG_BACKGROUND,
	RG_PLAYER,
	RG_MONSTER,
	RG_UI,
};

enum COLGROUP
{
	CG_PLAYER,
	CG_MONSTER,
};

class FbxPath
{
public:
	wchar_t LoadPath[256];
	wchar_t SavePath[256];

	FbxPath() : LoadPath{ 0, }, SavePath{ 0, }
	{

	}
};

class EtcPath
{
public:
	wchar_t LoadPath[256];

	EtcPath() :LoadPath{ 0, }
	{

	}
};


class LOGICVALUE 
{
public:
	static void Init();

private:
};


enum PIECEKINDS
{
	UNICORN = 1,
	GODOFWAR,
	SWORDMASTER,
	MANAMAGICIAN,
	LITTLEDRAGON,
	SHININGARCHER,
	BLADEKNIGHT,
	DEMONLARVA,
	WAREWOLF,
	FIREMAGICIAN,
	WINDRANGER,
	THUNDERSPIRIT,
	TOENAILDRUID,
	SIREN,
	PIRATECAPTAIN,
	TORTOLANELDER,
	DOOMARBITER,
	TIDEHUNTER,
	EGG,
	GODOFTHUNDER,
	SHADOWDEMON,
	WALRUSCHAMP,
};
