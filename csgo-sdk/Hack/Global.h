#pragma once

namespace G
{
	extern CBaseEntity*					LocalPlayer;
	extern CUserCmd*					UserCmd;
	extern HMODULE						Dll;
	extern HWND							Window;
	extern bool							UsingBulletHandler;
	extern std::vector<CBaseEntity*>	EnemyList;
	extern std::vector<CBaseEntity*>	TeamList;
	extern std::string					DebugString;
	extern bool							InAntiAim;
	extern bool							Aimbotting;
	extern bool							SendPacket;
	extern char							DllPath[MAX_PATH];
	extern QAngle						VisualAngle;
}
