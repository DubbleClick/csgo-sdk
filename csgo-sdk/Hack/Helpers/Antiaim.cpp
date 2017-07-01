#include "../../csgo-sdk.h"

#define RANDOM_FLOAT( min, max ) (( min + 1 ) + ( ( ( float ) rand( ) ) / ( float ) RAND_MAX ) * ( max - ( min + 1 ) ))

float QAngle::RealYawDifference(QAngle dst, QAngle add) {
	dst += add;

	dst.x = 0.f;

	Vector ang, aim;

	Math::AngleVectors(*this, aim);
	Math::AngleVectors(dst, ang);

	return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}

float QAngle::Difference(QAngle dst, QAngle add) {
	dst += add;

	Vector ang, aim;

	Math::AngleVectors(*this, aim);
	Math::AngleVectors(dst, ang);

	return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}


void CAntiAim::Run() {
	if (G::UserCmd->buttons & IN_USE)
		return;

	if (G::LocalPlayer->GetImmune() || (G::LocalPlayer->GetFlags() & FL_FROZEN))
		return;

	CBaseCombatWeapon* pWeapon = G::LocalPlayer->GetWeapon();

	if (!pWeapon)
		return;

	if (pWeapon->GetItemDefinitionIndex() == WEAPON_REVOLVER) {
		if (G::UserCmd->buttons & IN_ATTACK2)
			return;

		if (pWeapon->CanFirePostpone() && (G::UserCmd->buttons & IN_ATTACK))
			return;
	}
	else if (pWeapon->IsGrenade()) {
		if (pWeapon->IsInThrow())
			return;
	}
	else {
		if (pWeapon->IsKnife() && ((G::UserCmd->buttons & IN_ATTACK) || (G::UserCmd->buttons & IN_ATTACK2)))
			return;
		else if ((G::UserCmd->buttons & IN_ATTACK) && (pWeapon->GetItemDefinitionIndex() != WEAPON_C4 || Config->Ragebot.AntiAim.Pitch != AA_PITCH_OFF))
			return;
	}

	if (G::LocalPlayer->GetMoveType() == MOVETYPE_NOCLIP || G::LocalPlayer->GetMoveType() == MOVETYPE_LADDER)
		return;

	if (Config->Ragebot.AntiAim.Pitch == AA_PITCH_OFF && Config->Ragebot.AntiAim.YawFake == AA_FAKEYAW_FORWARD) {
		INetChannelInfo *nci = Interfaces->EngineClient->GetNetChannelInfo();
		if (nci) {
			if (nci->GetAvgLoss(FLOW_INCOMING) > 0 || nci->GetAvgLoss(FLOW_OUTGOING) > 0)
				return;
		}
	}

	float pos = G::UserCmd->viewangles.y + 180.f;


	if ((Config->Ragebot.AntiAim.YawDynamic > AA_DYNAMICYAW_OFF) && (Config->Ragebot.AntiAim.YawFake > AA_FAKEYAW_OFF) && G::LocalPlayer->GetVelocity().Length2D() > 0.1)
		G::UserCmd->viewangles = QAngle(GetPitch(), G::UserCmd->command_number % 2 ? GetDynamicYaw(pos) : pos, 0);
	else
		G::UserCmd->viewangles = QAngle(GetPitch(), Config->Ragebot.AntiAim.YawFake > AA_FAKEYAW_OFF ? (G::UserCmd->command_number % 2 ? GetFakeYaw(pos) : GetYaw(pos)) : GetYaw(pos), 0);

	if (Config->Ragebot.AntiAim.YawFake > AA_FAKEYAW_OFF)
		G::SendPacket = G::UserCmd->command_number % 2;

	G::InAntiAim = true;
}

float CAntiAim::GetPitch() {
	switch (Config->Ragebot.AntiAim.Pitch) {
	case AA_PITCH_OFF:
		return G::UserCmd->viewangles.x;
		break;
	case AA_PITCH_DYNAMIC:
		return G::LocalPlayer->GetWeapon()->IsSniper() ? (G::LocalPlayer->GetWeapon()->GetZoomLevel() != 0 ? 87.f : 85.f) : 88.99f;
		break;
	case AA_PITCH_DOWN:
		return 88.99f;
		break;
	case AA_PITCH_STRAIGHT:
		return 0.f;
		break;
	case AA_PITCH_UP:
		return -88.99f;
		break;
	}

	return G::UserCmd->viewangles.x;
}

float CAntiAim::GetYaw(float pos) {
	static bool state = false; state = !state;

	switch (Config->Ragebot.AntiAim.Yaw) {
	case AA_YAW_BACKWARD:
		return pos;
		break;
	case AA_YAW_JITTER:
		return (state) ? pos + 30.f : pos - 30.f;
		break;
	case AA_YAW_INVADE_JITTER:
	{
		QAngle lby = QAngle(0, G::LocalPlayer->GetLowerBodyYawTarget(), 0);
		QAngle set = (state) ? QAngle(0, pos + 30.f, 0) : QAngle(0, pos - 30.f, 0);
		return (set.RealYawDifference(lby) < 27.5f) ? pos : set.y;
		break;
	}
	case AA_YAW_RANDOM_JITTER:
		return pos + RANDOM_FLOAT(-135.f, 135.f);
		break;
	case AA_YAW_SYNCHRONIZE:
	{
		QAngle lby = QAngle(0, G::LocalPlayer->GetLowerBodyYawTarget(), 0);
		QAngle set = QAngle(0, pos - 180.f, 0);
		return (set.RealYawDifference(lby) < 75.f || set.RealYawDifference(lby) > 165.f) ? lby.y + 90.f : lby.y + 180.f;
		break;
	}
	case AA_YAW_SPIN:
		return fmod((G::LocalPlayer->GetTickBase() * Interfaces->Globals->interval_per_tick) / 1.f * 360.0f, 360.0f) + 180;
		break;
	case AA_YAW_FASTSPIN:
		return fmod((G::LocalPlayer->GetTickBase() * Interfaces->Globals->interval_per_tick) / 0.1f * 360.0f, 360.0f) + 180;
		break;
	case AA_YAW_MANUAL:
	{
		static bool dir = false;

		if (G::UserCmd->sidemove > 1) dir = false; else if (G::UserCmd->sidemove < -1) dir = true;

		return (dir) ? pos - 270.f : pos - 90.f;
		break;
	}
	}

	return 0;
}

float CAntiAim::GetFakeYaw(float pos) {
	static bool state;
	state = !state;

	switch (Config->Ragebot.AntiAim.YawFake) {
	case AA_FAKEYAW_FORWARD:
		return G::UserCmd->viewangles.y;
		break;
	case AA_FAKEYAW_JITTER:
		return (state) ? pos + 30.f : pos - 30.f;
		break;
	case AA_FAKEYAW_EVADE:
		return (state) ? pos + 80.f : pos - 80.f;
		break;
	case AA_FAKEYAW_SYNCHRONIZE:
		return G::LocalPlayer->GetLowerBodyYawTarget();
		break;
	case AA_FAKEYAW_SPIN:
		return fmod((G::LocalPlayer->GetTickBase() * Interfaces->Globals->interval_per_tick) / 1.f * 360.0f, 360.0f);
		break;
	case AA_FAKEYAW_FASTSPIN:
		return fmod((G::LocalPlayer->GetTickBase() * Interfaces->Globals->interval_per_tick) / 0.1f * 360.0f, 360.0f);
		break;
	}

	return 0;
}

float CAntiAim::GetDynamicYaw(float pos) {
	static bool state = false; state = !state;

	if (Config->Ragebot.AntiAim.Pitch == AA_PITCH_OFF && Config->Ragebot.AntiAim.YawFake == AA_FAKEYAW_FORWARD)
		return G::UserCmd->viewangles.y;

	switch (Config->Ragebot.AntiAim.YawDynamic) {
	case AA_DYNAMICYAW_FORWARD:
		return pos - 180.f;
		break;
	case AA_DYNAMICYAW_EVADE:
		return (state) ? pos + 90.f : pos - 90.f;
		break;
	case AA_DYNAMICYAW_STATICEVADE:
	{
		static bool staticstate = false;

		if (!(G::UserCmd->command_number % 5)) staticstate = !staticstate;

		return (staticstate) ? pos + 90.f : pos - 90.f;
		break;
	}
	case AA_DYNAMICYAW_DYNAMICEVADE:
	{
		float cvel = std::clamp(G::LocalPlayer->GetVelocity().Length2D() / 2, 35.f, 135.f);
		return (state) ? pos + cvel : pos - cvel;
		break;
	}
	}

	return 0;
}

#undef RANDOM_FLOAT
