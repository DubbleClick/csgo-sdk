#include "../../csgo-sdk.h"

bool CBacktracking::bTickIsValid(int tick) {
	static auto cl_interp_ratio = Interfaces->Cvar->FindVar("cl_interp_ratio");
	static auto cl_updaterate = Interfaces->Cvar->FindVar("cl_updaterate");

	float m_flLerpTime = cl_interp_ratio->GetFloat() / cl_updaterate->GetFloat();
	if (Config->Ragebot.NoInterpolation)
		tick += TIME_TO_TICKS(m_flLerpTime);

	return (fabsf(TICKS_TO_TIME(Interfaces->Globals->tickcount) - TICKS_TO_TIME(tick)) <= 0.2f);
}

void CBacktracking::SaveTemporaryRecord(CBaseEntity* ent, CTickRecord record) {
	if (!record.m_flSimulationTime)
		record = CTickRecord(ent);
	arr_infos.at(ent->entindex()).temporaryRecord = record;
}

void CBacktracking::RestoreTemporaryRecord(CBaseEntity* ent) {
	ApplyTickRecordCM(ent, arr_infos.at(ent->entindex()).temporaryRecord);
}

void CBacktracking::ApplyTickrecord(CBaseEntity* ent, CTickRecord record) {
	ApplyTickRecordFSN(ent, record);
	ApplyTickRecordCM(ent, record);
}

void CBacktracking::ApplyTickRecordFSN(CBaseEntity* pEntity, CTickRecord record) {
	*(QAngle*)((uintptr_t)pEntity + offsets->m_angEyeAngles) = record.m_angEyeAngles;
	*(float*)((uintptr_t)pEntity + offsets->m_flSimulationTime) = record.m_flSimulationTime;
	*(Vector*)((uintptr_t)pEntity + offsets->m_vecOrigin) = record.m_vecOrigin;
	*(Vector*)((uintptr_t)pEntity + offsets->m_vecVelocity) = record.m_vecVelocity;
	*(float*)((uintptr_t)pEntity + offsets->m_flDuckAmount) = record.m_flDuckAmount;
	*(float*)((uintptr_t)pEntity + offsets->m_flDuckSpeed) = record.m_flDuckSpeed;
	pEntity->SetPoseParameters(record.m_flPoseParameter);

	if (pEntity->GetAbsOrigin() == Vector(0, 0, 0) || (record.m_vecOrigin - record.m_vecAbsOrigin).LengthSqr() > 100)
		pEntity->SetAbsOrigin(record.m_vecOrigin);
	if (pEntity->GetAbsAngles() != pEntity->GetEyeAngles())
		pEntity->SetAbsAngles(record.m_angEyeAngles);
}

void CBacktracking::ApplyTickRecordCM(CBaseEntity* pEntity, CTickRecord record) {
	pEntity->SetAbsAngles(record.m_angEyeAngles);
	pEntity->SetAbsOrigin(record.m_vecOrigin);
	if (pEntity->GetAbsOrigin() == Vector(0, 0, 0) || (record.m_vecOrigin - record.m_vecAbsOrigin).LengthSqr() > 100)
		pEntity->SetAbsOrigin(record.m_vecOrigin);
	if (pEntity->GetAbsAngles() != pEntity->GetEyeAngles())
		pEntity->SetAbsAngles(record.m_angEyeAngles);
	*(Vector*)((uintptr_t)pEntity + offsets->m_vecOrigin) = record.m_vecOrigin;
	pEntity->SetPoseParameters(record.m_flPoseParameter);
}

int CBacktracking::ShotBackTrackedTick(CBaseEntity* ent) {
	if (!ent)
		return 0;

	C_PlayerStored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
	if (bTickIsValid(pCPlayer->TickCount))
		return pCPlayer->TickCount;

	return 0;
}

void CBacktracking::ShotBackTrackStoreFSN(CBaseEntity* ent) {
	if (!ent)
		return;

	static std::array<float, 33> lastsimulationtimes;

	if (lastsimulationtimes[ent->entindex()] == ent->GetSimulationTime())
		return;

	lastsimulationtimes[ent->entindex()] = ent->GetSimulationTime();

	CBaseCombatWeapon* pWeapon = ent->GetWeapon();
	bool PlayerShot = false;
	C_PlayerStored* pCPlayer = &arr_infos[ent->GetIndex()].player;
	if (ent->GetEyeAngles() != pCPlayer->EyeAngles)
		pCPlayer->EyeAnglesUpdated = true;

	if (!pWeapon)
		return;

	if (pCPlayer->CurrentWeapon == pWeapon && pWeapon->IsGun()) {
		//don't count weapon changes
		if (pWeapon->GetClipOne() == pCPlayer->BulletsLeft - 1) {
			if (pCPlayer->EyeAnglesUpdated) {
				//Don't count reloading
				pCPlayer->IsFiring = true;
				PlayerShot = true;
			}
		}
		else
			pCPlayer->IsFiring = false;
	}
	else {
		pCPlayer->IsFiring = false;
	}

	if (PlayerShot) {
		if (Config->Ragebot.NoInterpolation)
			pCPlayer->TickCount = TIME_TO_TICKS(ent->GetSimulationTime() - Interfaces->Globals->interval_per_tick) + TIME_TO_TICKS(Lagcompensation->GetLerpTime()) + 1;
		else
			pCPlayer->TickCount = G::UserCmd->tick_count;
		pCPlayer->ValidTick = CTickRecord(ent);
		pCPlayer->ValidTick.tickcount = pCPlayer->TickCount;
		pCPlayer->ValidTick.m_angEyeAngles = ent->GetEyeAngles();			//CURRENT angles yaw and pitch
																			//pCPlayer->ValidTick.m_flPoseParameter = std::array<float, 24>();
																			//if (ent->GetSimulationTime() - pCPlayer->SimulationTime > Interfaces->Globals->interval_per_tick) //if psilent or fake lag
																			//pCPlayer->ValidTick.m_angEyeAngles.x = pCPlayer->EyeAngles.x; //LAST angles pitch!
		arr_infos[ent->entindex()].shot = true;
	}

	pCPlayer->BulletsLeft = pWeapon->GetClipOne();
	pCPlayer->EyeAngles = ent->GetEyeAngles();
	pCPlayer->CurrentWeapon = pWeapon;

	if (!bTickIsValid(pCPlayer->TickCount)) {
		pCPlayer->TickCount = 0;
		arr_infos[ent->entindex()].shot = false;
	}
}

void CBacktracking::OverridePosesFsnRenderStart(CBaseEntity* ent) {
	if (!ent)
		return;
	C_PlayerStored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
	pCPlayer->ValidTick.m_flPoseParameter = ent->GetPoseParameters();
}

void CBacktracking::ShotBackTrackBeforeAimbot(CBaseEntity* ent) {
	if (!ent || !G::LocalPlayer || !G::LocalPlayer->GetWeapon() || !Interfaces->Globals)
		return;
	bool can_shoot = (G::LocalPlayer->GetWeapon()->GetNextPrimaryAttack() <= (G::LocalPlayer->GetTickBase() * Interfaces->Globals->interval_per_tick));
	C_PlayerStored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
	if (G::UserCmd)
		G::UserCmd->tick_count = G::UserCmd->tick_count;
	if (can_shoot && bTickIsValid(pCPlayer->TickCount)) {
		ApplyTickRecordFSN(ent, pCPlayer->ValidTick);
	}
}

void CBacktracking::ShotBackTrackAimbotStart(CBaseEntity* ent) {
	if (!ent || !G::LocalPlayer || !G::LocalPlayer->GetWeapon() || !Interfaces->Globals)
		return;
	bool can_shoot = (G::LocalPlayer->GetWeapon()->GetNextPrimaryAttack() <= (G::LocalPlayer->GetTickBase() * Interfaces->Globals->interval_per_tick));
	C_PlayerStored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
	if (G::UserCmd)
		G::UserCmd->tick_count = G::UserCmd->tick_count;
	if (can_shoot && bTickIsValid(pCPlayer->TickCount)) {
		SaveTemporaryRecord(ent);
		ApplyTickRecordCM(ent, pCPlayer->ValidTick);
	}
}

void CBacktracking::ShotBackTrackAimbotEnd(CBaseEntity* ent) {
	return;
	if (!ent)
		return;
	C_PlayerStored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
	if (!bTickIsValid(pCPlayer->TickCount))
		return;

	RestoreTemporaryRecord(ent);
}

void CBacktracking::ClearRecord(CBaseEntity* Entity) {
	arr_infos[Entity->GetIndex()] = CBacktrackInfo();
}
