#include "../../csgo-sdk.h"

void CAimbot::Tick() {
	if (!G::LocalPlayer || G::LocalPlayer->GetWeapon() == nullptr || !G::UserCmd)
		return;

	if (Config->Ragebot.HoldKey && !GetAsyncKeyState(Config->Ragebot.HoldKey))
		return;

	bool can_shoot = (G::LocalPlayer->GetWeapon()->GetNextPrimaryAttack() <= (G::LocalPlayer->GetTickBase() * Interfaces->Globals->interval_per_tick));

	if (!G::LocalPlayer->GetWeapon()->IsGun() || G::LocalPlayer->GetWeapon()->IsEmpty())
		goto AntiAim;

	if (m_bestent && !G::LocalPlayer->GetWeapon()->IsReloading()) {
		if (Config->Ragebot.AutoStop) {
			G::UserCmd->forwardmove = 0;
			G::UserCmd->sidemove = 0;
			G::UserCmd->upmove = 0;
			G::UserCmd->buttons = 0;
		}

		if (Config->Ragebot.AutoCrouch)
			G::UserCmd->buttons |= IN_DUCK;
	}
	if (can_shoot) {
		FindTarget();

		if (Config->Ragebot.AutoScope && m_bestent && G::LocalPlayer->GetWeapon()->IsSniper() && G::LocalPlayer->GetWeapon()->GetZoomLevel() == 0) {
			G::UserCmd->buttons |= IN_ATTACK2;
			m_bestent = nullptr;
			goto AntiAim;
		}

		if (m_bestent && G::LocalPlayer->GetWeapon()->IsGun() && !m_besthitbox.Empty())
			GoToTarget();

		if (G::UserCmd->buttons & IN_ATTACK) {
			if (Config->Ragebot.NoInterpolation && m_bestent) {
				auto tartime = TIME_TO_TICKS(m_bestent->GetSimulationTime() + Lagcompensation->GetLerpTime());
				if (Backtracking->bTickIsValid(tartime))
					G::UserCmd->tick_count = tartime;
			}
		}
	}

AntiAim:
	return;
}

void CAimbot::FindTarget() {
	m_besthitbox = Vector(0, 0, 0);
	m_bestent = nullptr;
	m_bestdmg = -1;

	for (const auto& Entity : G::EnemyList) {
		if(!EntityIsValid(Entity))
			continue;

		Vector hitbox = Entity->GetBonePosition(Config->Ragebot.Hitbox);

		if (Config->Ragebot.TargetMethod == 0) {
			float bestfov = Config->Ragebot.FOV;
			QAngle va;
			Interfaces->EngineClient->GetViewAngles(va);
			float fov = Math::GetFov(va, Math::CalcAngle(G::LocalPlayer->GetEyePosition(), hitbox));
			if (fov < bestfov) {
				int tmpdmg;
				if (Config->Ragebot.HitScanAll)
					tmpdmg = BestAimPointAll(Entity, m_besthitbox);
				else if (Config->Ragebot.Multipoint)
					tmpdmg = BestAimPointHitbox(Entity, m_besthitbox);
				else
					tmpdmg = static_cast<int>(Autowall->GetDamage(hitbox));

				if ((tmpdmg > Config->Ragebot.AutoWallDmg || tmpdmg > Entity->GetHealth()) && tmpdmg > m_bestdmg) {
					m_bestdmg = tmpdmg;
					m_bestent = Entity;
					bestfov = fov;
					if (!Config->Ragebot.HitScanAll && !Config->Ragebot.Multipoint)
						m_besthitbox = hitbox;

					if (tmpdmg >= Entity->GetHealth())
						break;
				}
			}
		}
		else if (Config->Ragebot.TargetMethod == 1) {
			float bestdist = G::LocalPlayer->GetWeapon()->GetCSWpnData()->m_flRange;
			QAngle va;
			Interfaces->EngineClient->GetViewAngles(va);
			float dist = Math::VectorDistance(G::LocalPlayer->GetEyePosition(), hitbox);
			if (dist < bestdist) {
				int tmpdmg;
				if (Config->Ragebot.HitScanAll)
					tmpdmg = BestAimPointAll(Entity, m_besthitbox);
				else if (Config->Ragebot.Multipoint)
					tmpdmg = BestAimPointHitbox(Entity, m_besthitbox);
				else
					tmpdmg = static_cast<int>(Autowall->GetDamage(hitbox));


				if ((tmpdmg > Config->Ragebot.AutoWallDmg || tmpdmg > Entity->GetHealth()) && tmpdmg > m_bestdmg) {
					m_bestdmg = tmpdmg;
					m_bestent = Entity;
					bestdist = dist;
					if (tmpdmg >= Entity->GetHealth())
						break;
				}
			}
		}
		else if (Config->Ragebot.TargetMethod == 2) { //damage
			int tmpdmg;
			if (Config->Ragebot.HitScanAll)
				tmpdmg = BestAimPointAll(Entity, m_besthitbox);
			else if (Config->Ragebot.Multipoint)
				tmpdmg = BestAimPointHitbox(Entity, m_besthitbox);
			else
				tmpdmg = static_cast<int>(Autowall->GetDamage(hitbox));

			if ((tmpdmg > Config->Ragebot.AutoWallDmg || tmpdmg > Entity->GetHealth()) && tmpdmg > m_bestdmg) {
				m_bestdmg = tmpdmg;
				m_bestent = Entity;
				if (!Config->Ragebot.HitScanAll)
					m_besthitbox = hitbox;
				if (tmpdmg >= Entity->GetHealth())
					break;
			}
		}
	}
}

void CAimbot::GoToTarget() {
	bool auto_shoot = false;

	CBaseCombatWeapon* weapon = G::LocalPlayer->GetWeapon();
	bool can_shoot = (weapon->GetNextPrimaryAttack() <= (G::LocalPlayer->GetTickBase() * Interfaces->Globals->interval_per_tick));
	static CBaseCombatWeapon* oldWeapon;
	if (weapon != oldWeapon) {
		oldWeapon = weapon;
		G::UserCmd->buttons &= ~IN_ATTACK;
		return;
	}
	
	if (weapon->IsPistol() && G::UserCmd->tick_count % 2) {
		static int lastshot;
		if (G::UserCmd->buttons & IN_ATTACK)
			lastshot++;

		if (!G::UserCmd->buttons & IN_ATTACK || lastshot > 1) {
			G::UserCmd->buttons &= ~IN_ATTACK;
			lastshot = 0;
		}
		return;
	}

	if (Config->Ragebot.Extrapolate)
		m_besthitbox = m_bestent->GetPredicted(m_besthitbox);

	QAngle aim_angle = Math::CalcAngle(G::LocalPlayer->GetEyePosition(), m_besthitbox);

	if (can_shoot && static_cast<int>(Math::GetHitchance(m_bestent, aim_angle)) >= Config->Ragebot.HitChanceAmt) {
		G::Aimbotting = true;
		G::UserCmd->viewangles = aim_angle;
		if(Config->Ragebot.NoRecoil)
			G::UserCmd->viewangles -= G::LocalPlayer->GetPunch() * 2.f;
		auto_shoot = true;
	}
	else {
		G::Aimbotting = false;
	}

	if (auto_shoot && can_shoot && Config->Ragebot.AutoFire)
		//if (!Config->Ragebot.HitChance || M::GetHitchance(m_bestent) >= Config->Ragebot.HitChanceAmt) //checked earlier
		G::UserCmd->buttons |= IN_ATTACK;
}

bool CAimbot::GetHitbox(CBaseEntity* target, Hitbox* hitbox) {
	matrix3x4a_t matrix[MAXSTUDIOBONES];

	if (!target->SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0))
		return false;


	studiohdr_t* hdr = Interfaces->ModelInfo->GetStudioModel(target->GetModel());

	if (!hdr)
		return false;

	mstudiohitboxset_t* hitboxSet = hdr->pHitboxSet(target->GetHitboxSet());
	mstudiobbox_t* untransformedBox = hitboxSet->pHitbox(hitbox->hitbox);

	Vector bbmin = untransformedBox->bbmin;
	Vector bbmax = untransformedBox->bbmax;

	if (untransformedBox->radius != -1.f) {
		bbmin -= Vector(untransformedBox->radius, untransformedBox->radius, untransformedBox->radius);
		bbmax += Vector(untransformedBox->radius, untransformedBox->radius, untransformedBox->radius);
	}

	Vector points[9] = { ((bbmin + bbmax) * .5f),
		Vector(bbmin.x, bbmin.y, bbmin.z),
		Vector(bbmin.x, bbmax.y, bbmin.z),
		Vector(bbmax.x, bbmax.y, bbmin.z),
		Vector(bbmax.x, bbmin.y, bbmin.z),
		Vector(bbmax.x, bbmax.y, bbmax.z),
		Vector(bbmin.x, bbmax.y, bbmax.z),
		Vector(bbmin.x, bbmin.y, bbmax.z),
		Vector(bbmax.x, bbmin.y, bbmax.z) };

	for (int index = 0; index < 9; index++) {
		if (index != 0)
			points[index] = ((((points[index] + points[0]) * .5f) + points[index]) * .5f);

		hitbox->points[index] = Math::VectorTransform(points[index], matrix[untransformedBox->bone]);
	}

	return true;
}

bool CAimbot::GetBestPoint(CBaseEntity* target, Hitbox* hitbox, BestPoint* point) {
	if (hitbox->hitbox == HITBOX_HEAD) {
		Vector high = ((hitbox->points[3] + hitbox->points[5]) * .5f);

		float pitch = target->GetEyeAngles().x;
		if ((pitch > 0.f) && (pitch <= 89.f)) {
			Vector height = (((high - hitbox->points[0]) / 3) * 4);
			Vector new_height = (hitbox->points[0] + (height * (pitch / 89.f)));

			hitbox->points[0] = new_height;
		}
	}

	for (int index = 0; index < 9; index++) {
		int temp_damage = static_cast<int>(Autowall->GetDamage(hitbox->points[index]));
		if (point->dmg < temp_damage) {
			point->dmg = temp_damage;
			point->point = hitbox->points[index];
			point->index = index;
			if (temp_damage >= target->GetHealth())
				break;
		}
	}

	return (point->dmg >= Config->Ragebot.AutoWallDmg);
}


int CAimbot::BestAimPointAll(CBaseEntity* target, Vector& hitbox) {
	m_hitboxes.clear();
	if (Config->Ragebot.Hitboxes.Pelvis)
		m_hitboxes.push_back(HITBOX_PELVIS);
	if (Config->Ragebot.Hitboxes.Chest)
		m_hitboxes.push_back(HITBOX_CHEST);
	if (Config->Ragebot.Hitboxes.Head)
		m_hitboxes.push_back(HITBOX_HEAD);
	if (Config->Ragebot.Hitboxes.Arms) {
		m_hitboxes.push_back(HITBOX_RIGHT_FOREARM);
		m_hitboxes.push_back(HITBOX_LEFT_FOREARM);
	}
	if (Config->Ragebot.Hitboxes.Feet) {
		m_hitboxes.push_back(HITBOX_RIGHT_CALF);
		m_hitboxes.push_back(HITBOX_LEFT_CALF);
	}

	BestPoint aim_point;
	int bestdmg = 0, tempdmg = 0;

	for (auto hitbx : m_hitboxes) {
		Hitbox curhitbox(hitbx);
		if (!GetHitbox(target, &curhitbox))
			continue;

		if (Config->Ragebot.Multipoint) {
			GetBestPoint(target, &curhitbox, &aim_point);
			if (aim_point.dmg > Config->Ragebot.AutoWallDmg)
				hitbox = aim_point.point;
			if (aim_point.dmg >= target->GetHealth())
				return aim_point.dmg;
		}
		else {
			//if (Math::GetHitchance(target, Math::CalcAngle(G::LocalPlayer->GetEyePosition(), curhitbox.points[0])) < Config->Ragebot.HitChanceAmt)
			//	continue;
			tempdmg = static_cast<int>(Autowall->GetDamage(curhitbox.points[0]));
			if (tempdmg > bestdmg) {
				bestdmg = tempdmg;
				hitbox = curhitbox.points[0];
				if (bestdmg >= target->GetHealth())
					return bestdmg;
			}
		}
	}

	return bestdmg;
}


int CAimbot::BestAimPointHitbox(CBaseEntity* target, Vector& hitbox) {
	BestPoint aim_point;

	int hitboxnum;

	switch (Config->Ragebot.Hitbox) {
	case 0:
		hitboxnum = HITBOX_PELVIS;
		break;
	case 2:
		hitboxnum = HITBOX_NECK;
		break;
	case 1:
		hitboxnum = HITBOX_BODY;
		break;
	case 3:
		hitboxnum = HITBOX_PELVIS;
		break;
	case 4:
	case 5:
		hitboxnum = HITBOX_BODY;
		break;
	case 6:
	case 7:
		hitboxnum = HITBOX_CHEST;
		break;
	default:
		hitboxnum = HITBOX_HEAD;
	}

	Hitbox hitboxx(hitboxnum);
	if (!GetHitbox(target, &hitboxx))
		return 0;

	if (!GetBestPoint(target, &hitboxx, &aim_point))
		return 0;

	if (aim_point.dmg >= Config->Ragebot.AutoWallDmg) {
		hitbox = aim_point.point;
		return aim_point.dmg;
	}

	return 0;
}

bool CAimbot::EntityIsValid(CBaseEntity* Entity) {
	if (!Entity)
		return false;
	if (Entity == G::LocalPlayer)
		return false;

	if (!Config->Ragebot.FriendlyFire) {
		if (Entity->GetTeam() == G::LocalPlayer->GetTeam())
			return false;
	}

	if (Entity->GetDormant())
		return false;
	if (Entity->GetImmune())
		return false;
	if (Entity->GetHealth() <= 0)
		return false;


	return true;
}
