#pragma once

struct CTickRecord {
	CTickRecord() {}
	CTickRecord(CBaseEntity* ent) {
		m_flLowerBodyYawTarget = ent->GetLowerBodyYawTarget();
		m_angEyeAngles = ent->GetEyeAngles();
		m_flCycle = ent->GetCycle();
		m_flSimulationTime = ent->GetSimulationTime();
		m_nSequence = ent->GetSequence();
		m_vecOrigin = ent->GetOrigin();
		m_vecVelocity = ent->GetVelocity();
		m_flPoseParameter = ent->GetPoseParameters();
		m_angAbsAngles = ent->GetAbsAngles();
		m_vecAbsOrigin = ent->GetAbsOrigin();
		m_flDuckSpeed = ent->GetDuckSpeed();
		m_flDuckAmount = ent->GetDuckAmount();
	}

	explicit operator bool() const noexcept {
		return m_flSimulationTime > 0.f;
	}

	bool operator>(const CTickRecord& others) {
		return (m_flSimulationTime > others.m_flSimulationTime);
	}
	bool operator>=(const CTickRecord& others) {
		return (m_flSimulationTime >= others.m_flSimulationTime);
	}
	bool operator<(const CTickRecord& others) {
		return (m_flSimulationTime < others.m_flSimulationTime);
	}
	bool operator<=(const CTickRecord& others) {
		return (m_flSimulationTime <= others.m_flSimulationTime);
	}
	bool operator==(const CTickRecord& others) {
		return (m_flSimulationTime == others.m_flSimulationTime);
	}

	float m_flDuckAmount = 0.f;
	float m_flDuckSpeed = 0.f;
	float m_flLowerBodyYawTarget = 0.f;
	QAngle m_angEyeAngles = QAngle(0, 0, 0);
	float m_flCycle = 0.f;
	float m_flSimulationTime = 0.f;
	int m_nSequence = 0;
	Vector m_vecOrigin = Vector(0, 0, 0);
	Vector m_vecAbsOrigin = Vector(0, 0, 0);
	Vector m_vecVelocity = Vector(0, 0, 0);
	std::array<float, 24> m_flPoseParameter = {};
	QAngle m_angAbsAngles = QAngle(0, 0, 0);
	int tickcount = 0;
};

struct C_PlayerStored {
	CBaseCombatWeapon* CurrentWeapon = nullptr;
	int BulletsLeft = 0;
	bool EyeAnglesUpdated = false;
	bool IsFiring = false;
	int TickCount = 0;
	float SimulationTime = 0;
	QAngle EyeAngles = QAngle(0, 0, 0);
	CTickRecord ValidTick = CTickRecord();
};

struct CBacktrackInfo {
	CBacktrackInfo() : shot(false), player(C_PlayerStored()), tick_count(0), temporaryRecord(CTickRecord()) {}

	bool shot = false;
	C_PlayerStored player = C_PlayerStored();
	int tick_count = 0;
	CTickRecord temporaryRecord = CTickRecord();


	CBaseEntity* Entity;
	std::deque<CTickRecord> TickRecords;
};

#define Backtracking CBacktracking::GetInstance()

class CBacktracking {
	friend class CLagcompensation;
	friend class CResolver;

	IMPLEMENT_SINGLETON(CBacktracking);

public:
	int ShotBackTrackedTick(CBaseEntity* ent);
	void ShotBackTrackStoreFSN(CBaseEntity* ent);
	void OverridePosesFsnRenderStart(CBaseEntity* ent);
	void ShotBackTrackBeforeAimbot(CBaseEntity* ent);
	void ShotBackTrackAimbotStart(CBaseEntity* ent);
	void ShotBackTrackAimbotEnd(CBaseEntity* ent);
	void ClearRecord(CBaseEntity* Entity);

public:
	bool bTickIsValid(int tick);
	void SaveTemporaryRecord(CBaseEntity* ent, CTickRecord record = CTickRecord());
	void RestoreTemporaryRecord(CBaseEntity* ent);

private:
	void ApplyTickrecord(CBaseEntity* ent, CTickRecord record);
	void ApplyTickRecordFSN(CBaseEntity* pEntity, CTickRecord record);
	void ApplyTickRecordCM(CBaseEntity* pEntity, CTickRecord record);

private:
	std::array<CBacktrackInfo, 32> arr_infos;
};
