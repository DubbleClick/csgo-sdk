#include "../../csgo-sdk.h"

RunCommandFn oRunCommand;
void __fastcall Hooks::RunCommand(void* ecx, void* edx, CBaseEntity* pEntity, CUserCmd* pCmd, IMoveHelper* moveHelper) {

	//if (pEntity == Interfaces->ClientEntityList->GetClientEntity(Interfaces->EngineClient->GetLocalPlayer())) {
	//Vars.m_vecOldPunch = pEntity->GetPunch();
	//}

	oRunCommand(ecx, edx, pEntity, pCmd, moveHelper);
}
