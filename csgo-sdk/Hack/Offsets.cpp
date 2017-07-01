#include "../csgo-sdk.h"

bool COffsets::Initialize() {
	m_fsnViewAngles = Netvars->GetOffset("DT_CSPlayer", "deadflag") + 4;
	m_iHealth = Netvars->GetOffset("DT_CSPlayer", "m_iHealth");
	m_iTeamNum = Netvars->GetOffset("DT_CSPlayer", "m_iTeamNum");
	m_ArmorValue = Netvars->GetOffset("DT_CSPlayer", "m_ArmorValue");
	m_bHasHelmet = Netvars->GetOffset("DT_CSPlayer", "m_bHasHelmet");
	m_iClass = Netvars->GetOffset("DT_CSPlayer", "m_iClass");
	m_nSequence = Netvars->GetOffset("DT_CSPlayer", "m_nSequence");
	m_flCycle = Netvars->GetOffset("DT_CSPlayer", "m_flCycle");
	m_flPoseParameter = Netvars->GetOffset("DT_CSPlayer", "m_flPoseParameter");
	m_bDormant = 0xE9;
	m_bGunGameImmunity = Netvars->GetOffset("DT_CSPlayer", "m_bGunGameImmunity");
	m_lifeState = Netvars->GetOffset("DT_CSPlayer", "m_lifeState");
	m_fFlags = Netvars->GetOffset("DT_CSPlayer", "m_fFlags");
	m_iObserverMode = Netvars->GetOffset("DT_CSPlayer", "m_iObserverMode");
	m_hObserverTarget = Netvars->GetOffset("DT_CSPlayer", "m_hObserverTarget");
	m_Local = Netvars->GetOffset("DT_BasePlayer", "m_Local");
	m_nTickBase = Netvars->GetOffset("DT_CSPlayer", "m_nTickBase");
	m_flSimulationTime = Netvars->GetOffset("DT_CSPlayer", "m_flSimulationTime");
	m_nForceBone = Netvars->GetOffset("DT_CSPlayer", "m_nForceBone");
	m_angEyeAngles = Netvars->GetOffset("DT_CSPlayer", "m_angEyeAngles");
	m_flLowerBodyYawTarget = Netvars->GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget");
	m_flFlashDuration = Netvars->GetOffset("DT_CSPlayer", "m_flFlashDuration");
	m_iGlowIndex = m_flFlashDuration + 0x18;
	m_mBoneMatrix = m_nForceBone + 0x1C;
	m_nWritableBones = m_nForceBone + 0x20;
	m_iShotsFired = Netvars->GetOffset("DT_CSPlayer", "m_iShotsFired");
	m_nModelIndex = Netvars->GetOffset("DT_BasePlayer", "m_nModelIndex");
	m_viewPunchAngle = Netvars->GetOffset("DT_BasePlayer", "m_Local") + 0x64;
	m_aimPunchAngle = Netvars->GetOffset("DT_BasePlayer", "m_Local") + 0x70;
	m_vecOrigin = Netvars->GetOffset("DT_BasePlayer", "m_vecOrigin");
	m_vecViewOffset = Netvars->GetOffset("DT_CSPlayer", "m_vecViewOffset[0]");
	m_vecVelocity = Netvars->GetOffset("DT_CSPlayer", "m_vecVelocity[0]");
	m_szLastPlaceName = Netvars->GetOffset("DT_CSPlayer", "m_szLastPlaceName");
	m_hActiveWeapon = Netvars->GetOffset("DT_CSPlayer", "m_hActiveWeapon");
	m_fAccuracyPenalty = Netvars->GetOffset("DT_WeaponCSBase", "m_fAccuracyPenalty");
	m_Collision = Netvars->GetOffset("DT_BasePlayer", "m_Collision");
	m_CollisionGroup = Netvars->GetOffset("DT_BasePlayer", "m_CollisionGroup");
	m_iWeaponID = Netvars->GetOffset("DT_WeaponCSBase", "m_fAccuracyPenalty") + 0x30;
	m_hOwnerEntity = Netvars->GetOffset("DT_BaseEntity", "m_hOwnerEntity");
	m_nMoveType = 0x258;
	m_nHitboxSet = Netvars->GetOffset("DT_BasePlayer", "m_nHitboxSet");
	m_bIsBroken = Netvars->GetOffset("DT_BreakableSurface", "m_bIsBroken");
	m_flC4Blow = Netvars->GetOffset("DT_PlantedC4", "m_flC4Blow");
	m_pCurrentCommand = Netvars->GetOffset("DT_BasePlayer", "m_nButtons") + 0x4;
	m_bIsScoped = Netvars->GetOffset("DT_CSPlayer", "m_bIsScoped");

	m_bReloadVisuallyComplete = Netvars->GetOffset("DT_WeaponCSBase", "m_bReloadVisuallyComplete");
	m_flNextPrimaryAttack = Netvars->GetOffset("DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
	m_iClip1 = Netvars->GetOffset("DT_BaseCombatWeapon", "m_iClip1");
	m_flPostponeFireReadyTime = Netvars->GetOffset("DT_WeaponCSBase", "m_flPostponeFireReadyTime");
	m_zoomLevel = Netvars->GetOffset("DT_WeaponCSBaseGun", "m_zoomLevel");
	m_iItemDefinitionIndex = Netvars->GetOffset("DT_BaseAttributableItem", "m_iItemDefinitionIndex");
	m_fogEnable = Netvars->GetOffset("DT_FogController", "m_fog.enable");

	GlowManager = (*(uintptr_t*)(Util::FindPattern("client.dll", "A1 ? ? ? ? A8 01 75 4B") + 0x1)) + 4;
	d3d9Device = **(uintptr_t**)(Util::FindPattern("shaderapidx9.dll", "A1 ?? ?? ?? ?? 50 8B 08 FF 51 0C") + 1);
	LoadFromBufferEx = Util::FindPattern("client.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04");
	InitKeyValuesEx = Util::FindPattern("client.dll", "55 8B EC 51 33 C0 C7 45");
	ServerRankRevealAllEx = Util::FindPattern("client.dll", "55 8B EC 8B 0D ? ? ? ? 68 ? ? ? ? ");
	IsReadyEx = Util::FindPattern("client.dll", "55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ?? ?? ?? ?? 57 8B 8E");
	IsEntityBreakableEx = Util::FindPattern("client.dll", "55 8B EC 51 56 8B F1 85 F6 74 68");
	ChangeClanTagEx = Util::FindPattern("engine.dll", "53 56 57 8B DA 8B F9 FF 15");
	m_dwOcclusionArray = *(uintptr_t*)(Util::FindPattern("client.dll", "A1 ? ? ? ? 8B B7 ? ? ? ? 89 75 F8") + 0x1);
	m_iDidCheckForOcclusion = *(uintptr_t*)(Util::FindPattern("client.dll", "A1 ? ? ? ? 8B B7 ? ? ? ? 89 75 F8") + 0x7);
	InvalidateBoneCacheEx = Util::FindPattern("client.dll", "80 3D ? ? ? ? 00 74 16 A1");
	SetLastTimeStampInterpolate = Util::FindPattern("client.dll", "FF D0 A1 ?? ?? ?? ?? B9 ?? ?? ?? ?? D9 1D ?? ?? ?? ?? FF 50 34 85 C0 74 22 8B 0D ?? ?? ?? ??") + 2;
	SetLastTimeStampFSN = Util::FindPattern("client.dll", "FF D0 8B 0D ?? ?? ?? ?? 6A 01 68 AC 01 00 00 D9 1D ?? ?? ?? ?? 8B 01 FF 50 40") + 2;
	bInterpolate = *(uintptr_t*)(Util::FindPattern("client.dll", "A0 ?? ?? ?? ?? B9 ?? ?? ?? ?? 88 45 FD A1 ?? ?? ?? ?? FF 50 34 0F B6 15 ?? ?? ?? ?? 85 C0") + 1);

	UpdateClientsideAnimationIndex = (*(uintptr_t*)(Util::FindPattern("client.dll", "74 0B 8B 0C F0 8B 01 FF 90") + 0x9)) / 4;
	s_bOverridePostProcessingDisable = *(uintptr_t*)(Util::FindPattern("client.dll", "80 3D ? ? ? ? ? 53 56 57 0F 85") + 2);

	SetAbsOriginEx = Util::FindPattern("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 ?? ?? ?? ?? 00 8B 7D 08 F3 0F 10 07 0F 2E 86 A0 00 00 00 9F F6 C4 44 7A 28 F3 0F 10 47 04 0F 2E 86 A4 00 00 00 9F F6 C4 44 7A 16 F3 0F 10 47 08 0F 2E 86 A8 00 00 00 9F F6 C4 44 0F 8B 63 01 00 00 6A 01");
	return true;
}
