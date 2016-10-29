#include "csgo-sdk.h"

void CMiscellaneous::Tick() {
	if (Config->Misc.AirStuck)
		Airstuck();
	if (Config->Misc.AutoStrafe)
		Autostrafe();
	if (Config->Misc.Bhop)
		Bunnyhop();
	if (Config->Misc.Knifebot)
		KnifeTrigger();
	if (Config->Misc.FakeLag.Value)
		Fakelag();
}

void CMiscellaneous::Airstuck() {
	if (GetAsyncKeyState(Config->Misc.AirStuckKey) & 0x8000)
		G::UserCmd->tick_count = INT_MAX;
}

void CMiscellaneous::Autostrafe() {
	static float m_circle_yaw;
	static float m_previous_yaw;

	auto get_angle_from_speed = [](float speed) {
		auto ideal_angle = RAD2DEG(std::atan2(30.f, speed));
		std::clamp(ideal_angle, 0.f, 90.f);
		return ideal_angle;
	};

	auto get_velocity_step = [](Vector velocity, float speed, float circle_yaw) {
		auto velocity_degree = RAD2DEG(std::atan2(velocity.x, velocity.y));
		auto step = 1.5f;

		Vector start = G::LocalPlayer->GetOrigin(), end = start;

		Ray_t ray;
		CGameTrace trace;
		CTraceFilter filter;

		while (true) {
			end.x += (std::cos(DEG2RAD(velocity_degree + circle_yaw)) * speed);
			end.y += (std::sin(DEG2RAD(velocity_degree + circle_yaw)) * speed);
			end *= Interfaces->Globals->frametime;

			ray.Init(start, end, Vector(-20.f, -20.f, 0.f), Vector(20.f, 20.f, 32.f));
			Interfaces->EngineTrace->TraceRay(ray, CONTENTS_SOLID, &filter, &trace);

			if (trace.fraction < 1.f || trace.allsolid || trace.startsolid)
				break;

			step -= Interfaces->Globals->frametime;

			if (step == 0.f)
				break;

			start = end;
			velocity_degree += (velocity_degree + circle_yaw);
		}

		return step;
	};

	auto set_button_state = []() {
		G::UserCmd->buttons &= ~(IN_MOVELEFT | IN_MOVERIGHT | IN_FORWARD | IN_BACK);

		if (G::UserCmd->sidemove < 0.f)
			G::UserCmd->buttons |= IN_MOVELEFT;
		else
			G::UserCmd->buttons |= IN_MOVERIGHT;

		if (G::UserCmd->forwardmove < 0.f)
			G::UserCmd->buttons |= IN_BACK;
		else
			G::UserCmd->buttons |= IN_FORWARD;
	};

	if (G::LocalPlayer->GetMoveType() != MOVETYPE_WALK || G::LocalPlayer->GetFlags() & FL_ONGROUND)
		return;

	auto velocity = G::LocalPlayer->GetVelocity();
	velocity.z = 0;

	static bool flip = false;
	auto turn_direction_modifier = flip ? 1.f : -1.f;
	flip = !flip;

	if (G::UserCmd->forwardmove > 0.f)
		G::UserCmd->forwardmove = 0.f;

	auto speed = velocity.Length2D();

	// circle strafe
	if (GetAsyncKeyState(Config->Misc.CircleStrafeKey)) {
		auto ideal_speed_angle = get_angle_from_speed(speed);
		m_circle_yaw = Math::NormalizeFloat(m_circle_yaw + ideal_speed_angle);
		auto step = get_velocity_step(velocity, speed, ideal_speed_angle);

		if (step != 0.f)
			m_circle_yaw += (((Interfaces->Globals->frametime * 128.f) * step) * step);

		G::UserCmd->viewangles.y = Math::NormalizeFloat(m_circle_yaw);
		G::UserCmd->sidemove = -450.f;
		set_button_state();
		return;
	}

	auto ideal_move_angle = RAD2DEG(std::atan2(15.f, speed));
	std::clamp(ideal_move_angle, 0.f, 90.f);

	auto yaw_delta = Math::NormalizeFloat(G::UserCmd->viewangles.y - m_previous_yaw);
	auto abs_yaw_delta = abs(yaw_delta);
	m_circle_yaw = m_previous_yaw = G::UserCmd->viewangles.y;

	if (yaw_delta > 0.f)
		G::UserCmd->sidemove = -450.f;

	else if (yaw_delta < 0.f)
		G::UserCmd->sidemove = 450.f;

	if (abs_yaw_delta <= ideal_move_angle || abs_yaw_delta >= 30.f) {
		QAngle velocity_angles;
		Math::VectorAngles(velocity, velocity_angles);

		auto velocity_angle_yaw_delta = Math::NormalizeFloat(G::UserCmd->viewangles.y - velocity_angles.y);
		auto velocity_degree = get_angle_from_speed(speed) * 2/*menu->main.misc.retrack_speed.value()*/;

		if (velocity_angle_yaw_delta <= velocity_degree || speed <= 15.f) {
			if (-(velocity_degree) <= velocity_angle_yaw_delta || speed <= 15.f) {
				G::UserCmd->viewangles.y += (ideal_move_angle * turn_direction_modifier);
				G::UserCmd->sidemove = 450.f * turn_direction_modifier;
			}

			else {
				G::UserCmd->viewangles.y = velocity_angles.y - velocity_degree;
				G::UserCmd->sidemove = 450.f;
			}
		}

		else {
			G::UserCmd->viewangles.y = velocity_angles.y + velocity_degree;
			G::UserCmd->sidemove = -450.f;
		}
	}

	set_button_state();
}

void CMiscellaneous::Bunnyhop() {
	if (!(G::LocalPlayer->GetFlags() & FL_ONGROUND))
		G::UserCmd->buttons &= ~IN_JUMP;
}

void CMiscellaneous::Fakelag() {
	int choke = std::min<int>(Config->Misc.FakeLag.Adaptive ? static_cast<int>(std::ceilf(64 / (G::LocalPlayer->GetVelocity().Length() * Interfaces->Globals->interval_per_tick))) : Config->Misc.FakeLag.Value, 14);

	if (Config->Misc.FakeLag.Adaptive && choke > 13) {
		return;
	}

	static int choked = 0;

	Vector curpos = G::LocalPlayer->GetOrigin();

	static Vector orecords[2];

	if (choked > choke) {
		G::SendPacket = true;
		choked = 0;

		static bool index = false;

		orecords[index ? 0 : 1] = curpos;

		index = !index;
	}
	else {
		G::SendPacket = false;
		choked++;
	}
}

void TraceKnife(bool stab, trace_t* tr) {
	// Attack range of knife attack
	float range = stab ? 32.0f : 48.0f;

	// Calculte end spot
	Vector myPos = G::LocalPlayer->GetEyePosition();
	Vector vecAim;
	Math::AngleVectors(G::UserCmd->viewangles, vecAim);
	Vector end = myPos + vecAim * range;

	// Trace attack
	Ray_t ray;
	ray.Init(G::LocalPlayer->GetEyePosition(), end);
	CTraceFilter filter;
	filter.pSkip = G::LocalPlayer;
	Interfaces->EngineTrace->TraceRay(ray, 0x46004003, &filter, tr);

	// If that failed, try a hull trace
	if (tr->fraction >= 1.0f) {
		static const Vector mins = Vector(-16.0f, -16.0f, -18.0f);
		static const Vector maxs = Vector(16.0f, 16.0f, 18.0f);

		ray.Init(G::LocalPlayer->GetEyePosition(), end, mins, maxs);

		Interfaces->EngineTrace->TraceRay(ray, 0x200400B, &filter, tr);
	}
}

bool IsBehind(CBaseEntity* pTarget) {
	//Vector vecLOS = pTarget->CalcAbsolutePosition() - vars.vecMe;
	Vector vecLOS = pTarget->GetAbsOrigin() - G::LocalPlayer->GetAbsOrigin();
	vecLOS.z = 0.0f;
	vecLOS.Normalize();

	Vector vTargetForward;
	QAngle angAbsRotation = pTarget->GetAbsAngles();
	Math::AngleVectors(pTarget->GetAbsAngles(), vTargetForward);
	vTargetForward.z = 0.0f; // BUG? vTargetForward.AsVector2D() isn't normalized.

							 // CSS:0.8, CSGO:0.475
	return (vecLOS.Dot(vTargetForward) > 0.475f);
}

void CMiscellaneous::KnifeTrigger() {
	trace_t tr;
	TraceKnife(false, &tr);
	if (!tr.m_pEnt || !tr.m_pEnt->IsEnemy())
		return; //don't attack

	CBaseEntity* pl = tr.m_pEnt;
	CBaseCombatWeapon* pWeapon = G::LocalPlayer->GetWeapon();

	//------------------------------------------------
	// Swing and stab damage

	struct table_t {
		// [first][armor][back]
		unsigned char swing[2][2][2];
		// [armor][back]
		unsigned char stab[2][2];
	};
	static const table_t table = {
		{ { { 25,90 },{ 21,76 } },{ { 40,90 },{ 34,76 } } },
		{ { 65,180 },{ 55,153 } }
	};

	bool armor = (pl->GetArmor() > 0);
	bool first = pWeapon->GetNextPrimaryAttack() + 0.4f < Interfaces->Globals->curtime; //fuck prediction
	bool back = IsBehind(pl);

	int stab_dmg = table.stab[armor][back];
	int slash_dmg = table.swing[first][armor][back]; //first slash, then swings
	int swing_dmg = table.swing[false][armor][back];

	int health = pl->GetHealth();
	bool stab;

	if (health <= slash_dmg) stab = false;
	else if (health <= stab_dmg) stab = true;
	else if (health > (slash_dmg + swing_dmg + stab_dmg)) stab = true;
	else stab = false;

	G::UserCmd->buttons |= stab ? IN_ATTACK2 : IN_ATTACK;
}

void CMiscellaneous::FixMovement(QAngle& wish_angle) {
	Vector view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
	auto viewangles = G::UserCmd->viewangles;
	viewangles.Normalize();

	Math::AngleVectors(wish_angle, &view_fwd, &view_right, &view_up);
	Math::AngleVectors(viewangles, &cmd_fwd, &cmd_right, &cmd_up);

	float v8 = sqrtf((view_fwd.x * view_fwd.x) + (view_fwd.y * view_fwd.y));
	float v10 = sqrtf((view_right.x * view_right.x) + (view_right.y * view_right.y));
	float v12 = sqrtf(view_up.z * view_up.z);

	Vector norm_view_fwd((1.f / v8) * view_fwd.x, (1.f / v8) * view_fwd.y, 0.f);
	Vector norm_view_right((1.f / v10) * view_right.x, (1.f / v10) * view_right.y, 0.f);
	Vector norm_view_up(0.f, 0.f, (1.f / v12) * view_up.z);

	float v14 = sqrtf((cmd_fwd.x * cmd_fwd.x) + (cmd_fwd.y * cmd_fwd.y));
	float v16 = sqrtf((cmd_right.x * cmd_right.x) + (cmd_right.y * cmd_right.y));
	float v18 = sqrtf(cmd_up.z * cmd_up.z);

	Vector norm_cmd_fwd((1.f / v14) * cmd_fwd.x, (1.f / v14) * cmd_fwd.y, 0.f);
	Vector norm_cmd_right((1.f / v16) * cmd_right.x, (1.f / v16) * cmd_right.y, 0.f);
	Vector norm_cmd_up(0.f, 0.f, (1.f / v18) * cmd_up.z);

	float v22 = norm_view_fwd.x * G::UserCmd->forwardmove;
	float v26 = norm_view_fwd.y * G::UserCmd->forwardmove;
	float v28 = norm_view_fwd.z * G::UserCmd->forwardmove;
	float v24 = norm_view_right.x * G::UserCmd->sidemove;
	float v23 = norm_view_right.y * G::UserCmd->sidemove;
	float v25 = norm_view_right.z * G::UserCmd->sidemove;
	float v30 = norm_view_up.x * G::UserCmd->upmove;
	float v27 = norm_view_up.z * G::UserCmd->upmove;
	float v29 = norm_view_up.y * G::UserCmd->upmove;

	G::UserCmd->forwardmove = ((((norm_cmd_fwd.x * v24) + (norm_cmd_fwd.y * v23)) + (norm_cmd_fwd.z * v25))
							   + (((norm_cmd_fwd.x * v22) + (norm_cmd_fwd.y * v26)) + (norm_cmd_fwd.z * v28)))
		+ (((norm_cmd_fwd.y * v30) + (norm_cmd_fwd.x * v29)) + (norm_cmd_fwd.z * v27));
	G::UserCmd->sidemove = ((((norm_cmd_right.x * v24) + (norm_cmd_right.y * v23)) + (norm_cmd_right.z * v25))
							+ (((norm_cmd_right.x * v22) + (norm_cmd_right.y * v26)) + (norm_cmd_right.z * v28)))
		+ (((norm_cmd_right.x * v29) + (norm_cmd_right.y * v30)) + (norm_cmd_right.z * v27));
	G::UserCmd->upmove = ((((norm_cmd_up.x * v23) + (norm_cmd_up.y * v24)) + (norm_cmd_up.z * v25))
						  + (((norm_cmd_up.x * v26) + (norm_cmd_up.y * v22)) + (norm_cmd_up.z * v28)))
		+ (((norm_cmd_up.x * v30) + (norm_cmd_up.y * v29)) + (norm_cmd_up.z * v27));

	G::UserCmd->forwardmove = std::clamp(G::UserCmd->forwardmove, -450.f, 450.f);
	G::UserCmd->sidemove = std::clamp(G::UserCmd->sidemove, -450.f, 450.f);
	G::UserCmd->upmove = std::clamp(G::UserCmd->upmove, -320.f, 320.f);
}

void CMiscellaneous::AntiAim() {
	if (!G::LocalPlayer || !G::LocalPlayer->GetWeapon())
		return;
	bool can_shoot = (G::LocalPlayer->GetWeapon()->GetNextPrimaryAttack() <= (G::LocalPlayer->GetTickBase() * Interfaces->Globals->interval_per_tick));
	if ((!(G::UserCmd->buttons & IN_ATTACK) || !(can_shoot)) && !(G::UserCmd->buttons & IN_USE)) { //shooting or pressing E
		if (Config->Ragebot.AntiAim.Yaw || Config->Ragebot.AntiAim.Pitch) {
			G::Aimbotting = false;
			G::InAntiAim = true;
			Antiaim->Run();
		}
		else G::InAntiAim = false;
	}
}
