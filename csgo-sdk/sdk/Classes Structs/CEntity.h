#pragma once

#include "../../hack/Basics/NetvarManager.h"

#define ADD_NETVAR(type,func,table,prop) \
type& func() { \
	static uint32_t offset = Netvars->GetOffset(table, prop);\
	return *(type*)((uint32_t)this + offset);\
}

class CBaseEntity : public IClientEntity {
public:
	IClientUnknown*			GetIClientUnknown() { return this; }
	IClientNetworkable*		GetIClientNetworkable() { return reinterpret_cast<IClientNetworkable*>((uintptr_t) this + 0x8); }
	CBaseAnimating*			GetBaseAnimating() { return reinterpret_cast<CBaseAnimating*>((uintptr_t) this + 0x4); }
	C_BaseAnimatingOverlay*	GetBaseAnimatingOverlay() { return reinterpret_cast<C_BaseAnimatingOverlay*>(this); }

	ADD_NETVAR(int, m_iHealth, "DT_CSPlayer", "m_iHealth");
	ADD_NETVAR(int, m_iTeam, "DT_CSPlayer", "m_iTeam");
	ADD_NETVAR(int, m_fFlags, "DT_CSPlayer", "m_fFlags");
	ADD_NETVAR(int, m_nTickBase, "DT_CSPlayer", "m_nTickBase");
	ADD_NETVAR(int, m_iShotsFired, "DT_CSPlayer", "m_iShotsFired");
	ADD_NETVAR(int, m_nMoveType, "DT_CSPlayer", "m_nMoveType");
	ADD_NETVAR(int, m_nModelIndex, "DT_BasePlayer", "m_nModelIndex");
	ADD_NETVAR(int, m_nHitboxSet, "DT_BasePlayer", "m_nHitboxSet");
	ADD_NETVAR(int, m_ArmorValue, "DT_CSPlayer", "m_ArmorValue");
	ADD_NETVAR(int, m_CollisionGroup, "DT_CSPlayer", "m_CollisionGroup");
	ADD_NETVAR(int, m_iGlowIndex, "DT_CSPlayer", "m_iGlowIndex");
	ADD_NETVAR(int, m_lifeState, "DT_CSPlayer", "m_lifeState");
	ADD_NETVAR(int, m_nSequence, "DT_CSPlayer", "m_nSequence");

	ADD_NETVAR(float, m_flFlashDuration, "DT_CSPlayer", "m_flFlashDuration");
	ADD_NETVAR(float, m_flSimulationTime, "DT_CSPlayer", "m_flSimulationTime");
	ADD_NETVAR(float, m_flLowerBodyYawTarget, "DT_CSPlayer", "m_flLowerBodyYawTarget");
	ADD_NETVAR(float, m_flCycle, "DT_CSPlayer", "m_flCycle");

	ADD_NETVAR(CBaseHandle, m_hOwnerEntity, "DT_CSPlayer", "m_hOwnerEntity");
	ADD_NETVAR(CBaseHandle, m_hObserverTarget, "DT_CSPlayer", "m_hObserverTarget");

	ADD_NETVAR(bool, m_bGunGameImmunity, "DT_CSPlayer", "m_bGunGameImmunity");
	ADD_NETVAR(bool, m_bDormant, "DT_CSPlayer", "m_bDormant");
	ADD_NETVAR(bool, m_bIsBroken, "DT_BreakableSurface", "m_bIsBroken");
	ADD_NETVAR(bool, m_bHasHelmet, "DT_CSPlayer", "m_bHasHelmet");

	ADD_NETVAR(Vector, m_vecOrigin, "DT_BasePlayer", "m_vecOrigin");
	ADD_NETVAR(Vector, m_vecViewOffset, "DT_BasePlayer", "m_vecViewOffset[0]");
	ADD_NETVAR(Vector, m_vecVelocity, "DT_BasePlayer", "m_vecVelocity[0]");
	ADD_NETVAR(Vector, m_vecMins, "DT_BasePlayer", "m_vecMins");
	ADD_NETVAR(Vector, m_vecMaxs, "DT_BasePlayer", "m_vecMaxs");

	ADD_NETVAR(QAngle, m_angEyeAngles, "DT_CSPlayer", "m_angEyeAngles");
	ADD_NETVAR(ICollideable*, m_Collision, "DT_BasePlayer", "m_Collision");
	ADD_NETVAR(const char*, m_szLastPlaceName, "DT_BasePlayer", "m_szLastPlaceName");
	ADD_NETVAR(CBaseHandle, m_hActiveWeapon, "DT_CSPlayer", "m_hActiveWeapon");

	using flArr24 = std::array<float, 24>;
	ADD_NETVAR(flArr24, m_flPoseParameter, "DT_CSPlayer", "m_flPoseParameter");
	

	int					GetIndex() { return *(int*)((uintptr_t)this + 0x64); }
	int					GetHealth();
	int					GetTeam();
	int					GetFlags();
	int					GetTickBase();
	int					GetShotsFired();
	int					GetCrosshairId();
	int					GetMoveType();
	int					GetModelIndex();
	int					GetHitboxSet();
	int					GetUserID();
	int					GetArmor();
	int					GetCollisionGroup();
	int					PhysicsSolidMaskForEntity();
	CBaseHandle GetOwnerEntity();
	int					GetGlowIndex();
	int					GetHeadshots();
	int					GetKills();
	float				GetCycle();
	std::array<float, 24> GetPoseParameters();
	void SetPoseParameters(std::array<float, 24> arr);
	int					GetSequence();
	float				GetHeadshotPercentage();
	bool				IsAlive();
	bool				ShouldCollide(int, int);
	bool				IsTransparent();
	SolidType_t			GetSolid();
	bool				GetDormant();
	bool				GetImmune();
	bool				IsEnemy();
	bool				IsVisible(int bone);
	bool				IsVisible(Vector pos);
	bool				IsBroken();
	bool				HasHelmet();
	bool				IsFlashed();
	bool				IsPlayer();
	void				UpdateClientSideAnimation();
	unsigned int		GetObserverTarget();
	float				GetFlashDuration();
	float				GetBombTimer();
	QAngle				GetViewPunch();
	QAngle				GetPunch();
	QAngle				GetEyeAngles();
	float				GetLowerBodyYawTarget();
	void				SetAbsAngles(const QAngle&);
	void				SetAbsOrigin(const Vector&);
	Vector				GetMins();
	Vector				GetMaxs();
	Vector				GetOrigin();
	Vector				GetViewOffset();
	Vector				GetEyePosition();
	Vector				GetBonePosition(int iBone);
	float				GetSimulationTime();
	bool				SetupBones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);
	Vector				GetVelocity();
	Vector				GetPredicted(Vector p0);
	ICollideable*		GetCollideable();
	player_info_t		GetPlayerInfo();
	model_t*			GetModel();
	std::string			GetName();
	std::string			GetSteamID();
	std::string			GetLastPlace();
	CBaseCombatWeapon*	GetWeapon();
	void				Precache(const char*);
	bool				IsCrouching();
	float GetDuckSpeed();
	float GetDuckAmount();
	bool IsMovingOnGround();
};

class CBaseAnimating : public CBaseEntity {

};

class CBaseCombatWeapon {
public:
	byte			__pad[0x64];
	int				index;
	int&			GetWeaponID();
	float&			GetNextPrimaryAttack();
	float&			GetAccuracyPenalty();
	int&			GetItemDefinitionIndex();
	bool			IsEmpty();
	bool			IsReloading();
	void			UpdateAccuracyPenalty();
	float			GetWeaponSpread();
	float			GetWeaponCone();
	int				GetZoomLevel();
	WeaponInfo_t*	GetCSWpnData();
	bool			IsGun();
	bool			IsGrenade();
	bool			IsSniper();
	bool			IsPistol();
	bool			IsKnife();
	std::string		GetWeaponName();
	float			GetInaccuracy();
	int				GetClipOne();
	bool GetPinPulled();
	float GetThrowTime();
	float GetFireReadyTime();
	bool CanFirePostpone();
	bool IsInThrow();
};

#undef ADD_NETVAR
