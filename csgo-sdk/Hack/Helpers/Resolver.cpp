#include "../../csgo-sdk.h"

void CResolver::Resolve_Bruteforce(CBaseEntity* pEntity) {
	auto getdst = [](int a, int b) {
		int x = abs(b - a) % 360;
		return x > 180 ? 360 - x : x;
	};

	auto record = Backtracking->arr_infos.at(pEntity->GetIndex());

	int avg = 1;
	int count = 1;

	float prevlby = 0.f;

	for (auto& r : record.TickRecords) {
		if (r.m_flLowerBodyYawTarget == prevlby)
			continue;

		prevlby = r.m_flLowerBodyYawTarget;

		avg += getdst(static_cast<int>(r.m_angEyeAngles.y), static_cast<int>(r.m_flLowerBodyYawTarget));

		count++;
	}

	avg /= count;

	float add = 0;

	if (avg > 145)
		add = 180;
	else if (avg > 110)
		add = 135;
	else if (avg > 75)
		add = 90;
	else if (avg > 40)
		add = 45;

	pEntity->m_angEyeAngles().y += (G::UserCmd->command_number % 2) ? add : -add;
	pEntity->m_angEyeAngles().y = Math::ClampYaw(pEntity->m_angEyeAngles().y);
}

void CResolver::Resolve_Predict(CBaseEntity* pEntity) {
	auto record = Backtracking->arr_infos.at(pEntity->GetIndex());

	float prevlby = 0.f;

	int avg = 1;
	int count = 1;

	for (auto& r : record.TickRecords) {
		if (r.m_flLowerBodyYawTarget == prevlby)
			continue;

		prevlby = r.m_flLowerBodyYawTarget;

		float diff = r.m_flLowerBodyYawTarget - prevlby;

		avg += static_cast<int>(diff);
		count++;
	}

	avg /= count;

	pEntity->m_angEyeAngles().y += avg;
	pEntity->m_angEyeAngles().NormalizeYaw();
}

void CResolver::Resolve(CBaseEntity * pEntity) {
	int resolvermode = Config->Ragebot.ResolveMode;

	if (resolvermode == RESOLVERMODE_OFF)
		return;

	if (pEntity->IsMovingOnGround())
		pEntity->m_angEyeAngles().y = pEntity->m_flLowerBodyYawTarget();
	else {
		if (Backtracking->arr_infos.at(pEntity->GetIndex()).TickRecords.size() < 3)
			return;

		switch (resolvermode) {
		case RESOLVERMODE_PREDICT:
			Resolve_Predict(pEntity);
			break;
		case RESOLVERMODE_BRUTEFORCE:
			Resolve_Bruteforce(pEntity);
			break;
		default:
			return;
			break;
		}
	}
}

void CResolver::Store(CBaseEntity * Entity) {
	if (!G::UserCmd || !Entity || Entity->GetIndex() < 1 || Entity->GetIndex() > 32)
		return;

	auto record = &Backtracking->arr_infos.at(Entity->GetIndex());

	record->Entity = Entity;

	if (record->TickRecords.size() > 1 && record->TickRecords.front().tickcount == G::UserCmd->tick_count)
		return;

	record->TickRecords.push_front(CTickRecord(Entity));

	while ((int)record->TickRecords.size() > Config->Ragebot.ResolveTicks)
		record->TickRecords.pop_back();
}

/*
void CResolver::Resolve(CBaseEntity* ent) {
	auto cur = m_arrInfos.at(ent->GetIndex()).m_sRecords;
	if (cur.size() < 2)
		return;

	if (IsEntityMoving(ent))
		ent->m_angm_angEyeAngles().y = ent->Getm_flLowerBodyYawTarget();
	else if (HasStaticRealAngle(cur))
		ent->m_angm_angEyeAngles().y = (cur.front().m_flm_flLowerBodyYawTarget);
	else if (HasStaticYawDifference(cur))
		ent->m_angm_angEyeAngles().y = (ent->Getm_angEyeAngles().y - (cur.front().m_angm_angEyeAngles.y - cur.front().m_flm_flLowerBodyYawTarget));
	else if (HasSteadyDifference(cur)) {
		float tickdif = (float)cur.front().tickcount - (float)cur.at(1).tickcount;
		float lbydif = GetDelta(cur.front().m_flm_flLowerBodyYawTarget, cur.at(1).m_flm_flLowerBodyYawTarget);
		float ntickdif = (float)G::UserCmd->tick_count - (float)cur.front().tickcount;
		ent->m_angm_angEyeAngles().y = ((lbydif / tickdif) * ntickdif);
	}
	else if (DeltaKeepsChanging(cur))
		ent->m_angm_angEyeAngles().y = (ent->Getm_angEyeAngles().y - GetDeltaByComparingTicks(cur));
	else if (LBYKeepsChanging(cur))
		ent->m_angm_angEyeAngles().y = (GetLBYByComparingTicks(cur));
	else
		ent->m_angm_angEyeAngles().y = (ent->Getm_angEyeAngles().y + 180);
}


bool& CResolver::LowerBodyYawChanged(CBaseEntity* ent) {
	return m_arrInfos.at(ent->GetIndex()).m_bLowerBodyYawChanged;
}

void CResolver::StoreVars(CBaseEntity* ent) {
	if (m_arrInfos.at(ent->GetIndex()).m_sRecords.size() >= static_cast<size_t>(Config->Ragebot.ResolveTicks))
		m_arrInfos.at(ent->GetIndex()).m_sRecords.pop_back();
	m_arrInfos.at(ent->GetIndex()).m_sRecords.push_front(CTickRecord(ent));
}

void CResolver::StoreVars(CBaseEntity* ent, QAngle ang, float lby, float simtime, float tick) {
	if (m_arrInfos.at(ent->GetIndex()).m_sRecords.size() >= static_cast<size_t>(Config->Ragebot.ResolveTicks))
		m_arrInfos.at(ent->GetIndex()).m_sRecords.pop_back();
	m_arrInfos.at(ent->GetIndex()).m_sRecords.push_front(CTickRecord(ent));
}

bool& CResolver::BacktrackThisTick(CBaseEntity* ent) {
	return m_arrInfos.at(ent->GetIndex()).m_bBacktrackThisTick;
}

bool CResolver::HasStaticRealAngle(const std::deque<CTickRecord>& l, float tolerance) {
	auto minmax = std::minmax_element(std::begin(l), std::end(l), [](const CTickRecord& t1, const CTickRecord& t2) { return t1.m_flm_flLowerBodyYawTarget < t2.m_flm_flLowerBodyYawTarget; });
	return (fabs(minmax.first->m_flm_flLowerBodyYawTarget - minmax.second->m_flm_flLowerBodyYawTarget) <= tolerance);
}

bool CResolver::HasStaticRealAngle(int index, float tolerance) {
	return HasStaticRealAngle(m_arrInfos[index].m_sRecords, tolerance);
}

bool CResolver::HasStaticYawDifference(const std::deque<CTickRecord>& l, float tolerance) {
	for (auto i = l.begin(); i < l.end() - 1;) {
		if (GetDelta(LBYDelta(*i), LBYDelta(*++i)) > tolerance)
			return false;
	}
	return true;
}

bool CResolver::HasSteadyDifference(const std::deque<CTickRecord>& l, float tolerance) {
	size_t misses = 0;
	for (size_t i = 0; i < l.size() - 1; i++) {
		float tickdif = static_cast<float>(l.at(i).m_flSimulationTime - l.at(i + 1).tickcount);
		float lbydif = GetDelta(l.at(i).m_flm_flLowerBodyYawTarget, l.at(i + 1).m_flm_flLowerBodyYawTarget);
		float ntickdif = static_cast<float>(G::UserCmd->tick_count - l.at(i).tickcount);
		if (((lbydif / tickdif) * ntickdif) > tolerance) misses++;
	}
	return (misses <= (l.size() / 3));
}

int CResolver::GetDifferentDeltas(const std::deque<CTickRecord>& l, float tolerance) {
	std::vector<float> vec;
	for (auto var : l) {
		float curdelta = LBYDelta(var);
		bool add = true;
		for (auto fl : vec) {
			if (!IsDifferent(curdelta, fl, tolerance))
				add = false;
		}
		if (add)
			vec.push_back(curdelta);
	}
	return vec.size();
}

int CResolver::GetDifferentLBYs(const std::deque<CTickRecord>& l, float tolerance) {
	std::vector<float> vec;
	for (auto var : l) {
		float curyaw = var.m_flm_flLowerBodyYawTarget;
		bool add = true;
		for (auto fl : vec) {
			if (!IsDifferent(curyaw, fl, tolerance))
				add = false;
		}
		if (add)
			vec.push_back(curyaw);
	}
	return vec.size();
}

float CResolver::GetLBYByComparingTicks(const std::deque<CTickRecord>& l) {
	int modulo = Config->Ragebot.ResolveModulo;
	int difangles = GetDifferentLBYs(l);
	int inc = modulo * difangles;
	for (auto var : l) {
		for (int lasttick = var.tickcount; lasttick < G::UserCmd->tick_count; lasttick += inc) {
			if (lasttick == G::UserCmd->tick_count)
				return var.m_flm_flLowerBodyYawTarget;
		}
	}
	return 0.f;
}

float CResolver::GetDeltaByComparingTicks(const std::deque<CTickRecord>& l) {
	int modulo = Config->Ragebot.ResolveModulo;
	int difangles = GetDifferentDeltas(l);
	int inc = modulo * difangles;
	for (auto var : l) {
		for (int lasttick = var.tickcount; lasttick < G::UserCmd->tick_count; lasttick += inc) {
			if (lasttick == G::UserCmd->tick_count)
				return LBYDelta(var);
		}
	}
	return 0.f;
}

bool CResolver::DeltaKeepsChanging(const std::deque<CTickRecord>& cur, float tolerance) {
	return (GetDifferentDeltas(cur) > (int) cur.size() / 2);
}

bool CResolver::LBYKeepsChanging(const std::deque<CTickRecord>& cur, float tolerance) {
	return (GetDifferentLBYs(cur, tolerance) > (int)cur.size() / 2);
}

bool CResolver::IsEntityMoving(CBaseEntity* ent) {
	return (ent->GetVelocity().Length2D() > 0.1f && ent->GetFlags() & FL_ONGROUND);
}
*/
