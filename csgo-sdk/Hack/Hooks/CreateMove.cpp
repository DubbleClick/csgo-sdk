#include "../../csgo-sdk.h"

void ClampUserCmd(CUserCmd* pCmd) {
	pCmd->forwardmove = std::clamp(pCmd->forwardmove, -450.f, 450.f);
	pCmd->sidemove = std::clamp(pCmd->sidemove, -450.f, 450.f);
	pCmd->upmove = std::clamp(pCmd->upmove, -320.f, 320.f);
	pCmd->viewangles.Clamp();
}

CreateMoveFn oCreateMove;
bool __stdcall Hooks::CreateMove(float flInputSampleTime, CUserCmd* cmd) {
	uintptr_t* framePointer;
	__asm mov framePointer, ebp;
	QAngle org_angle = cmd->viewangles;


	if (!cmd->command_number || !Interfaces->EngineClient->IsInGame())
		return oCreateMove(flInputSampleTime, cmd);

	G::LocalPlayer = Interfaces->ClientEntityList->GetClientEntity(Interfaces->EngineClient->GetLocalPlayer());
	G::UserCmd = cmd;
	G::SendPacket = true;

	Util::BuildEntityLists();

	Miscellaneous->Tick();
	QAngle wish_angle = cmd->viewangles;
	cmd->viewangles = org_angle;

	EnginePrediction->StartPrediction();

	if (Config->Ragebot.Enabled)
		Aimbot->Tick();

	Miscellaneous->AntiAim();

	Miscellaneous->FixMovement(wish_angle);

	EnginePrediction->EndPrediction();

	if (Config->Visual.Enabled && Config->Visual.GrenadePredictionEnabled) GrenadePrediction->Tick(G::UserCmd->buttons);


	if (Config->Ragebot.Enabled && G::Aimbotting && G::UserCmd->buttons & IN_ATTACK)
		*(bool*)(*framePointer - 0x1C) = false;
	*(bool*)(*framePointer - 0x1C) = G::SendPacket;

	if (Config->Misc.ShowRealAA) {
		if (!G::SendPacket)
			G::VisualAngle = cmd->viewangles;
	}
	else if (G::SendPacket)
		G::VisualAngle = cmd->viewangles;

	ClampUserCmd(cmd);

	if (!Config->Ragebot.Silent)
		Interfaces->EngineClient->SetViewAngles(cmd->viewangles);

	return false;
}

