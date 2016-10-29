#pragma once

#define Autowall CAutowall::GetInstance()

class CAutowall {
public:
	static std::shared_ptr<CAutowall> GetInstance() {
		static std::shared_ptr<CAutowall> instance(new CAutowall);
		return instance;
	}

private:
	CAutowall() { }
	CAutowall(CAutowall const&) = delete;
	void operator=(CAutowall const&) = delete;
public:
	~CAutowall() {}

public:
	float GetDamage(const Vector& point);
	float GetDamage(const Vector& point, bool& penetrated, CBaseEntity* ent = nullptr, Hitboxes hbox = HITBOX_CHEST);

private:
	const float GetHitgroupDamageMultiplier(int hitgroup);
	void ScaleDamage(int hitgroup, CBaseEntity *enemy, float weapon_armor_ratio, float &current_damage);
	CBaseEntity* Run(const Vector& pTargetPos, CBaseCombatWeapon* weapon, float* damage, int& hitgroup, bool& penetrated, CBaseEntity* ent = nullptr, Hitboxes hbox = HITBOX_BODY);
};