#include "../../csgo-sdk.h"

void IPrediction::SetupMove(CBaseEntity *player, CUserCmd *ucmd, IMoveHelper *pHelper, CMoveData *move) {
	return Util::GetVFunc<void(__thiscall*)(void*, CBaseEntity*, CUserCmd*, IMoveHelper*, CMoveData*)>(this, 20)(this, player, ucmd, pHelper, move);
}

void IPrediction::FinishMove(CBaseEntity *player, CUserCmd *ucmd, CMoveData *move) {
	return Util::GetVFunc<void(__thiscall*)(void*, CBaseEntity*, CUserCmd*, CMoveData*)>(this, 21)(this, player, ucmd, move);
}
