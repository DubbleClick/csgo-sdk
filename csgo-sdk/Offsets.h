#pragma once

#define offsets COffsets::GetInstance()

class COffsets {
	IMPLEMENT_SINGLETON_INITIALIZED(COffsets);

public:
	bool Initialize();

public:
	uintptr_t m_flSimulationTime;
	uintptr_t m_hObserverTarget;
	uintptr_t m_nWritableBones;
	uintptr_t m_iMatchStats_HeadShotKills;
	uintptr_t m_iMatchStats_Kills;
	uintptr_t m_iObserverMode;
	uintptr_t m_fsnViewAngles;
	uintptr_t m_dwGetSolid;
	uintptr_t m_nDeltaTick;
	uintptr_t m_iHealth;
	uintptr_t m_iTeamNum;
	uintptr_t m_bDormant;
	uintptr_t m_bGunGameImmunity;
	uintptr_t m_lifeState;
	uintptr_t m_fFlags;
	uintptr_t m_Local;
	uintptr_t m_nTickBase;
	uintptr_t m_nForceBone;
	uintptr_t m_mBoneMatrix;
	uintptr_t m_nModelIndex;
	uintptr_t m_viewPunchAngle;
	uintptr_t m_aimPunchAngle;
	uintptr_t m_vecOrigin;
	uintptr_t m_vecViewOffset;
	uintptr_t m_vecVelocity;
	uintptr_t m_szLastPlaceName;
	uintptr_t m_flNextPrimaryAttack;
	uintptr_t m_hActiveWeapon;
	uintptr_t m_fAccuracyPenalty;
	uintptr_t m_Collision;
	uintptr_t m_iShotsFired;
	uintptr_t m_iWeaponID;
	uintptr_t m_nMoveType;
	uintptr_t m_nHitboxSet;
	uintptr_t m_bHasHelmet;
	uintptr_t m_ArmorValue;
	uintptr_t m_CollisionGroup;
	uintptr_t m_iClass;
	uintptr_t m_bIsBroken;
	uintptr_t m_angEyeAngles;
	uintptr_t m_flLowerBodyYawTarget;
	uintptr_t m_hOwnerEntity;
	uintptr_t m_flC4Blow;
	uintptr_t m_flFlashDuration;
	uintptr_t m_iGlowIndex;
	uintptr_t m_iCrosshairId;
	uintptr_t m_nSequence;
	uintptr_t m_flCycle;
	uintptr_t m_flPoseParameter;
	uintptr_t m_flDuckSpeed;
	uintptr_t m_flDuckAmount;

	uintptr_t m_bPinPulled;
	uintptr_t m_fThrowTime;
	uintptr_t m_flPostponeFireReadyTime;
	uintptr_t m_bIsScoped;
	uintptr_t m_iItemDefinitionIndex;
	uintptr_t m_iClip1;
	uintptr_t m_bReloadVisuallyComplete;
	uintptr_t m_zoomLevel;
	uintptr_t m_fogEnable;
	uintptr_t m_pCurrentCommand;
	uintptr_t m_vecMins;
	uintptr_t m_vecMaxs;

	//sigs
	uintptr_t d3d9Device;
	uintptr_t SendPacket;
	uintptr_t GlowManager;
	uintptr_t LoadFromBufferEx;
	uintptr_t InitKeyValuesEx;
	uintptr_t ServerRankRevealAllEx;
	uintptr_t IsReadyEx;
	uintptr_t IsEntityBreakableEx;
	uintptr_t dwKeyValuesConstructor;
	uintptr_t dwKeyValuesFindKey;
	uintptr_t ChangeClanTagEx;
	uintptr_t InvalidateBoneCacheEx;
	uintptr_t m_dwOcclusionArray;
	uintptr_t m_iDidCheckForOcclusion;
	uintptr_t bInterpolate;
	uintptr_t SetLastTimeStampInterpolate;
	uintptr_t SetLastTimeStampFSN;
	uintptr_t UpdateClientsideAnimationIndex;
	uintptr_t s_bOverridePostProcessingDisable;
	uintptr_t SetAbsOriginEx;
};
