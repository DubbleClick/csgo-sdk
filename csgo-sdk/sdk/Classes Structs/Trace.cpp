#include "../../csgo-sdk.h"

bool IsEntityBreakable(IHandleEntity* pBaseEntity) {
	using IsBreakableEntityFn = bool(__thiscall*)(IHandleEntity*);
	static IsBreakableEntityFn fnIsBreakableEntity = reinterpret_cast<IsBreakableEntityFn>(offsets->IsEntityBreakableEx);

	return fnIsBreakableEntity(pBaseEntity);
}

bool StandardFilterRules(IHandleEntity* pHandleEntity, int fContentsMask, int collisionGroup) {
	CBaseEntity* pCollide = dynamic_cast<CBaseEntity*>(pHandleEntity);

	if (!pCollide)
		return true;

	const SolidType_t solid = pCollide->GetSolid();
	const model_t* pModel = pCollide->GetModel();
	auto mod = Interfaces->ModelInfo->GetModelType(pModel);

	if ((mod != mod_brush) || (solid != SOLID_BSP && solid != SOLID_VPHYSICS)) {
		if ((fContentsMask & CONTENTS_MONSTER) == 0)
			return false;
	}

	if (!(fContentsMask & CONTENTS_WINDOW) && pCollide->IsTransparent())
		return false;

	if (!(fContentsMask & CONTENTS_MOVEABLE) && (pCollide->GetMoveType() == MOVETYPE_PUSH))// !(touch->flags & FL_WORLDBRUSH) )
		return false;

	return true;
}

bool PassServerEntityFilter(const IHandleEntity *pTouch, const IHandleEntity *pPass) {
	if (!pPass)
		return true;

	if (pTouch == pPass)
		return false;

	CBaseEntity *pEntTouch = (CBaseEntity*)pTouch;
	CBaseEntity *pEntPass = (CBaseEntity*)pPass;
	if (!pEntTouch || !pEntPass)
		return true;

	if (pEntTouch->GetOwnerEntity() == pEntPass)
		return false;

	if (pEntPass->GetOwnerEntity() == pEntTouch)
		return false;

	return true;
}

bool GameRulesShouldCollide(int collisionGroup0, int collisionGroup1) {
	uintptr_t shouldcollide = reinterpret_cast<uintptr_t>(Interfaces->GameRules);
	__asm {
		push collisionGroup1
		push collisionGroup0
		mov ecx, shouldcollide
		mov ecx, [ecx]
		mov eax, [ecx]
		mov eax, [eax + 0x70]
		call eax
	}
}

bool BaseShouldHitEntity(IHandleEntity* pSkip, IHandleEntity* pHandleEntity, int m_collisionGroup, int contentsMask) {
	if (!StandardFilterRules(pHandleEntity, contentsMask, m_collisionGroup))
		return false;

	if (pSkip) {
		if (!PassServerEntityFilter(pHandleEntity, pSkip)) {
			return false;
		}
	}

	CBaseEntity* pEntity = dynamic_cast<CBaseEntity*>(pHandleEntity);//EntityFromEntityHandle(pHandleEntity);
	if (!pEntity)
		return false;
	if (!pEntity->ShouldCollide(m_collisionGroup, contentsMask))
		return false;
	if (!GameRulesShouldCollide(m_collisionGroup, pEntity->GetCollisionGroup()))
		return false;

	return true;
}
