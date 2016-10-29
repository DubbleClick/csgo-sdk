#pragma once

#define TIME_TO_TICKS( dt )        ( (int)( 0.5f + (float)(dt) / Interfaces->Globals->interval_per_tick ) )
#define TICKS_TO_TIME( t )		( Interfaces->Globals->interval_per_tick *( t ) )
#define ROUND_TO_TICKS( t )		( Interfaces->Globals->interval_per_tick * TIME_TO_TICKS( t ) )
#define TICK_NEVER_THINK		(-1)


class VarMapEntry_t {
public:
	unsigned short type;
	unsigned short m_bNeedsToInterpolate; // Set to false when this var doesn't
										  // need Interpolate() called on it anymore.
	void *data;
	void *watcher;
};

struct VarMapping_t {
	VarMapping_t() {
		m_nInterpolatedEntries = 0;
	}

	VarMapEntry_t* m_Entries;
	int m_nInterpolatedEntries;
	float m_lastInterpolationTime;
};


#define Lagcompensation CLagcompensation::GetInstance()

class CLagcompensation {
	friend class CResolver;
	friend class CBacktracking;

	VarMapping_t* GetVarMap(void* pBaseEntity) {
		return reinterpret_cast<VarMapping_t*>((DWORD)pBaseEntity + 0x24); //0x4C );
	}

	IMPLEMENT_SINGLETON(CLagcompensation);
public:
	float GetLerpTime() const;

	void DisableInterpolation(CBaseEntity* pEntity) {
		VarMapping_t* map = GetVarMap(pEntity);
		if (!map) return;
		for (int i = 0; i < map->m_nInterpolatedEntries; i++) {
			VarMapEntry_t *e = &map->m_Entries[i];
			e->m_bNeedsToInterpolate = false;
		}
	}
};

