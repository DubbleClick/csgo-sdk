#include "../csgo-sdk.h"

CBaseEntity*				G::LocalPlayer;
CUserCmd*					G::UserCmd;
HMODULE						G::Dll;
HWND						G::Window;
bool						G::UsingBulletHandler;
std::vector<CBaseEntity*>	G::EnemyList(16);
std::vector<CBaseEntity*>	G::TeamList(16);
std::string					G::DebugString;
bool						G::InAntiAim;
bool						G::Aimbotting;
bool						G::SendPacket;
char						G::DllPath[MAX_PATH];
QAngle						G::VisualAngle;
