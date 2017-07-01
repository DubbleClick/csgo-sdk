#include "../../csgo-sdk.h"

void FixTraceRay(Vector end, Vector start, trace_t* oldtrace, CBaseEntity* ent) {
	auto mins = ent->m_vecMins();
	auto maxs = ent->m_vecMaxs();

	auto center = (maxs + mins) * 0.5f;
	auto origin = ent->GetAbsOrigin();

	auto v15(center + origin);

	auto delta(end - start);
	delta.Normalize();

	auto v20 = (v15 - start);
	auto v22 = delta.Dot(v20);

	float v23;

	if (v22 >= 0.f) {
		if (v22 <= delta.Length()) {
			auto fuck(v15 - ((delta * v22) + start));
			v23 = fuck.Length();
		}

		else {
			auto lol(v15 - end);
			v23 = -lol.Length();
		}
	}

	else
		v23 = -v20.Length();

	if (v23 <= 60.f) {
		Ray_t ray;
		ray.Init(start, end);

		trace_t trace;
		Interfaces->EngineTrace->ClipRayToEntity(ray, 0x4600400B, (IHandleEntity*)ent, &trace);

		if (oldtrace->fraction > trace.fraction)
			*oldtrace = trace;
	}
}


bool DidHitNonWorldEntity(CBaseEntity* m_pEnt) {
	return m_pEnt != NULL && m_pEnt != Interfaces->ClientEntityList->GetClientEntity(0);
}

bool TraceToExitRebuilt(Vector &end, trace_t &enter_trace, Vector start, Vector dir, trace_t *exit_trace) {
	float distance = 0.0f;

	while (distance <= 90.0f) {
		distance += 4.0f;
		end = start + dir * distance;

		int point_contents = Interfaces->EngineTrace->GetPointContents(end, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL);

		if (point_contents & MASK_SHOT_HULL && (!(point_contents & CONTENTS_HITBOX)))
			continue;

		auto new_end = end - (dir * 4.0f);

		Ray_t ray;
		ray.Init(end, new_end);
		Interfaces->EngineTrace->TraceRay(ray, MASK_SHOT, 0, exit_trace);

		if (exit_trace->startsolid && exit_trace->surface.flags & SURF_HITBOX) {
			ray.Init(end, start);
			CTraceFilter filter;
			filter.pSkip = (IHandleEntity*)exit_trace->m_pEnt;
			Interfaces->EngineTrace->TraceRay(ray, 0x600400B, &filter, exit_trace);

			if ((exit_trace->fraction < 1.0f || exit_trace->allsolid) && !exit_trace->startsolid) {
				end = exit_trace->endpos;
				return true;
			}
			continue;
		}

		if (!(exit_trace->fraction < 1.0 || exit_trace->allsolid || exit_trace->startsolid) || exit_trace->startsolid) {
			if (exit_trace->m_pEnt) {
				if (DidHitNonWorldEntity(enter_trace.m_pEnt))
					return true;
			}
			continue;
		}

		if (((exit_trace->surface.flags >> 7) & 1) && !((enter_trace.surface.flags >> 7) & 1))
			continue;

		if (exit_trace->plane.normal.Dot(dir) <= 1.0f) {
			float fraction = exit_trace->fraction * 4.0f;
			end = end - (dir * fraction);
			return true;
		}
	}
	return false;
}

bool HandleBreakablePenetration(trace_t& enter_trace, Vector* vecDir, int enter_material, float flPenetrationModifier, float flPenetration, Vector& result, float* bulletdamage) {
	Vector end;
	trace_t trace_exit;
	if (!TraceToExitRebuilt(end, enter_trace, enter_trace.endpos, *vecDir, &trace_exit))
		return true;

	float damage = *bulletdamage;
	surfacedata_t* exit_surface_data = Interfaces->Physprops->GetSurfaceData(trace_exit.surface.surfaceProps);
	auto exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if (((enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))//|| (enter_material == 70))
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else {
		combined_penetration_modifier = (flPenetrationModifier + exit_surf_penetration_mod) * 0.5f;
	}

	if (enter_material == exit_material) {
		if (exit_material == 87 || exit_material == 85)
			combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)
			combined_penetration_modifier = 2.0f;
	}

	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	float v35 = (damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / flPenetration) * 1.25f);
	float thickness = (trace_exit.endpos - enter_trace.endpos).Length();

	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;

	float lost_damage = fmaxf(0.0f, v35 + thickness);

	if (lost_damage > damage)
		return true;

	if (lost_damage >= 0.0f)
		*bulletdamage -= lost_damage;

	if (*bulletdamage < 1.0f)
		return true;

	result = trace_exit.endpos;
	return false;
}

bool WallIsTooThick(Vector &start, Vector &dir, float flMaxDistance) {
	float flDistance = 0;
	Vector end;

	while (flDistance <= flMaxDistance) {
		flDistance += 24.0f;

		end = start + (dir * flDistance);

		if ((Interfaces->EngineTrace->GetPointContents(end) & MASK_SOLID) == 0) {
			return false;
		}
	}

	return true;
}

const float CAutowall::GetHitgroupDamageMultiplier(int iHitGroup) {
	switch (iHitGroup) {
	case HITGROUP_HEAD:
		return 4.0f;
	case HITGROUP_STOMACH:
		return 1.25f;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		return 0.75f;
	case HITGROUP_CHEST:
	case HITGROUP_LEFTARM:
	case HITGROUP_RIGHTARM:
	default: return 1.0f;
	}
	return 1.0f;
}

void CAutowall::ScaleDamage(int hitgroup, CBaseEntity *enemy, float weapon_armor_ratio, float &current_damage) {
	current_damage *= GetHitgroupDamageMultiplier(hitgroup);
	float m_flArmor = (float)enemy->GetArmor();
	if (m_flArmor > 0.0f && hitgroup < HITGROUP_LEFTLEG)  //don't do gear, or legs
	{
		if (hitgroup == HITGROUP_HEAD && !enemy->HasHelmet())
			return;

		float armorscaled = (weapon_armor_ratio * 0.5f) * current_damage;
		if ((current_damage - armorscaled) * 0.5f > m_flArmor)
			armorscaled = current_damage - (m_flArmor * 2.0f);
		current_damage = armorscaled;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CBaseEntity* CAutowall::Run(const Vector& pTargetPos, CBaseCombatWeapon* weapon, float* damage, int& hitgroup, bool& penetrated, CBaseEntity* ent, Hitboxes hbox) {
	static auto HandleBulletPenetration = reinterpret_cast<bool(__thiscall*)(CBaseEntity*, float&, int &, char*, trace_t*, Vector*,
															 float _setto0f_8, float, float, int, int, float, int*, Vector*, float, float, float*)>(
		Util::FindPattern("client.dll", "53 8B DC 83 EC 08 83 E4 F8 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 83 EC 78 8B 53 14"));

	static auto UTIL_ClipTraceToPlayers = [](const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr) {
		static uintptr_t clptrtp = Util::FindPattern("client.dll", "53 8B DC 83 EC ? 83 E4 ? 83 C4 ? 55 8B 6B 04 89 6C 24 ? 8B EC 81 EC ? ? ? ? 8B 43 10 56 57 52 F3 0F 10 40");
		__asm {
			push tr
			push filter
			push mask
			lea edx, vecAbsEnd
			lea ecx, vecAbsStart
			call clptrtp
			add esp, 0xC
		}
	};

	static auto UTIL_TraceLineIgnoreTwoEntities = [](const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, CBaseEntity* ignore, CBaseEntity* ignore2, int collisionGroup, trace_t* ptr) {
		Ray_t ray;
		ray.Init(vecAbsStart, vecAbsEnd);
		CTraceFilterSkipTwoEntities traceFilter(ignore, ignore2, collisionGroup);
		Interfaces->EngineTrace->TraceRay(ray, mask, &traceFilter, ptr);
	};

	penetrated = true;
	if (!weapon)
		return nullptr;
	WeaponInfo_t* wpn_data = weapon->GetCSWpnData();
	if (!wpn_data)
		return nullptr;

	trace_t enter_tr;
	Vector vecSrc = G::LocalPlayer->GetEyePosition(), result = vecSrc;
	Vector vecDir = (pTargetPos - G::LocalPlayer->GetEyePosition()).Normalized();

	int hitsleft = 4;
	CBaseEntity* lastEntityHit = nullptr;
	float m_flPenetration = wpn_data->m_flPenetration;
	float bulletdmg = static_cast<float>(wpn_data->m_iDamage);
	float tmpdmg = bulletdmg;
	float flDistance = wpn_data->m_flRange;
	float flTraveledDistance = 0.0f;

	while (bulletdmg > 0.0f) {
		flDistance -= flTraveledDistance;
		Vector EndPos = vecSrc + vecDir * flDistance;

		UTIL_TraceLineIgnoreTwoEntities(result, EndPos, MASK_SHOT, G::LocalPlayer, lastEntityHit, COLLISION_GROUP_NONE, &enter_tr);

		constexpr const float rayExtension = 40.0f;
		if (ent != nullptr && hbox == HITBOX_HEAD) {
			FixTraceRay(EndPos + vecDir * rayExtension, vecSrc, &enter_tr, ent);
		}
		else {
			CTraceFilterSkipTwoEntities filter(G::LocalPlayer, lastEntityHit, COLLISION_GROUP_NONE);
			UTIL_ClipTraceToPlayers(vecSrc, EndPos + vecDir * rayExtension, 0x4600400B, &filter, &enter_tr);
		}

		surfacedata_t* entersurf = Interfaces->Physprops->GetSurfaceData(enter_tr.surface.surfaceProps);
		float flPenetrationModifier = entersurf->game.flPenetrationModifier;
		flTraveledDistance += enter_tr.fraction * flDistance;
		//bulletdmg *= powf(wpn_data->m_flRangeModifier, (flTraveledDistance / 500));
		bulletdmg = powf(wpn_data->m_flRangeModifier, (flTraveledDistance / 500)) * tmpdmg;

		if (enter_tr.hitgroup != 0 && enter_tr.m_pEnt != nullptr) {
			if (enter_tr.m_pEnt->IsEnemy()) {
				ScaleDamage(enter_tr.hitgroup, enter_tr.m_pEnt, wpn_data->m_flArmorRatio, bulletdmg);
				*damage = bulletdmg;
				hitgroup = enter_tr.hitgroup;
				penetrated = !(hitsleft == 4);
				return enter_tr.m_pEnt;
			}
			else if (enter_tr.m_pEnt->IsPlayer())
				break; //break if we don't want to penetrate teammates
		}

		lastEntityHit = enter_tr.m_pEnt;
		G::UsingBulletHandler = true;
		int material = entersurf->game.material;
		char unkFlags = (enter_tr.contents >> 3) & CONTENTS_SOLID;

		if (enter_tr.contents & CONTENTS_WINDOW /*|| IsEntityFuncBreak(enter_tr.m_pEnt)*/) { //latter doesn't seem to be needed
			if (HandleBreakablePenetration(enter_tr, &vecDir, material, flPenetrationModifier, m_flPenetration, result, &bulletdmg)) {
				break;
			}
		}
		else if (HandleBulletPenetration(G::LocalPlayer, m_flPenetration, material, &unkFlags, &enter_tr, &vecDir, 0.f, flPenetrationModifier, entersurf->game.flDamageModifier, 0, 0x1002, m_flPenetration, &hitsleft, &result, 0.f, 0.f, &bulletdmg)) {
			break;	//Cant penetrate
		}

		tmpdmg = bulletdmg;
		G::UsingBulletHandler = false;
	}
	penetrated = !(hitsleft == 4);
	G::UsingBulletHandler = false;
	hitgroup = 0;
	*damage = 0.f;
	return nullptr;
}

float CAutowall::GetDamage(const Vector& point, bool& penetrated, CBaseEntity* ent, Hitboxes hbox) {
	float dmg;
	int hitgrp;
	Run(point, G::LocalPlayer->GetWeapon(), &dmg, hitgrp, penetrated, ent, hbox);
	return dmg;
}

float CAutowall::GetDamage(const Vector& point) {
	bool penetrated;
	return GetDamage(point, penetrated);
}
