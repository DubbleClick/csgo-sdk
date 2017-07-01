#pragma once


struct BrushSideInfo_t {
	Vector plane;			// The plane of the brush side
	float planec;
	unsigned short bevel;	// Bevel plane?
	unsigned short thin;	// Thin?
};

enum DebugTraceCounterBehavior_t {
	kTRACE_COUNTER_SET = 0,
	kTRACE_COUNTER_INC,
};

enum TraceType_t {
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

struct Ray_t {
	Ray_t() {}

	VectorAligned		m_Start = VectorAligned();
	VectorAligned		m_Delta;
	VectorAligned		m_StartOffset;
	VectorAligned		m_Extents;
	const matrix3x4_t	*m_pWorldAxisTransform = nullptr;
	bool				m_IsRay;
	bool				m_IsSwept;

	void Init(Vector vecStart, Vector vecEnd) {
		m_Delta = VectorAligned(vecEnd - vecStart);
		m_IsSwept = (m_Delta.LengthSqr() != 0);
		m_Extents.Zero();
		m_pWorldAxisTransform = nullptr;
		m_IsRay = true;
		m_StartOffset.Zero();
		m_Start = vecStart;
	}

	void Init(Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs) {
		m_Delta = end - start;

		m_pWorldAxisTransform = nullptr;
		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents = maxs - mins;
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);

		m_StartOffset = maxs + mins;
		m_StartOffset *= 0.5f;
		m_Start = static_cast<VectorAligned>(start) + m_StartOffset;
		m_StartOffset *= -1.0f;
	}

};

struct cplane_t {
	Vector	normal;
	float	dist;
	byte	type;
	byte	signbits;
	byte	pad[2];
};

///////////////////////////////////////////////////////////////////////////////

enum MoveCollide_t {
	MOVECOLLIDE_DEFAULT = 0,

	// These ones only work for MOVETYPE_FLY + MOVETYPE_FLYGRAVITY
	MOVECOLLIDE_FLY_BOUNCE,	// bounces, reflects, based on elasticity of surface and object - applies friction (adjust velocity)
	MOVECOLLIDE_FLY_CUSTOM,	// Touch() will modify the velocity however it likes
	MOVECOLLIDE_FLY_SLIDE,  // slides along surfaces (no bounce) - applies friciton (adjusts velocity)

	MOVECOLLIDE_COUNT,		// Number of different movecollides

	// When adding new movecollide types, make sure this is correct
	MOVECOLLIDE_MAX_BITS = 3
};

// edict->solid values
// NOTE: Some movetypes will cause collisions independent of SOLID_NOT/SOLID_TRIGGER when the entity moves
// SOLID only effects OTHER entities colliding with this one when they move - UGH!

// Solid type basically describes how the bounding volume of the object is represented
// NOTE: SOLID_BBOX MUST BE 2, and SOLID_VPHYSICS MUST BE 6
// NOTE: These numerical values are used in the FGD by the prop code (see prop_dynamic)
enum SolidType_t {
	SOLID_NONE = 0,	// no solid model
	SOLID_BSP = 1,	// a BSP tree
	SOLID_BBOX = 2,	// an AABB
	SOLID_OBB = 3,	// an OBB (not implemented yet)
	SOLID_OBB_YAW = 4,	// an OBB, constrained so that it can only yaw
	SOLID_CUSTOM = 5,	// Always call into the entity for tests
	SOLID_VPHYSICS = 6,	// solid vphysics object, get vcollide from the model and collide with that
	SOLID_LAST,
};

enum SolidFlags_t {
	FSOLID_CUSTOMRAYTEST = 0x0001,	// Ignore solid type + always call into the entity for ray tests
	FSOLID_CUSTOMBOXTEST = 0x0002,	// Ignore solid type + always call into the entity for swept box tests
	FSOLID_NOT_SOLID = 0x0004,	// Are we currently not solid?
	FSOLID_TRIGGER = 0x0008,	// This is something may be collideable but fires touch functions
	// even when it's not collideable (when the FSOLID_NOT_SOLID flag is set)
	FSOLID_NOT_STANDABLE = 0x0010,	// You can't stand on this
	FSOLID_VOLUME_CONTENTS = 0x0020,	// Contains volumetric contents (like water)
	FSOLID_FORCE_WORLD_ALIGNED = 0x0040,	// Forces the collision rep to be world-aligned even if it's SOLID_BSP or SOLID_VPHYSICS
	FSOLID_USE_TRIGGER_BOUNDS = 0x0080,	// Uses a special trigger bounds separate from the normal OBB
	FSOLID_ROOT_PARENT_ALIGNED = 0x0100,	// Collisions are defined in root parent's local coordinate space
	FSOLID_TRIGGER_TOUCH_DEBRIS = 0x0200,	// This trigger will touch debris objects
	FSOLID_MAX_BITS = 10
};

enum modtype_t {
	mod_bad = 0,
	mod_brush,
	mod_sprite,
	mod_studio
};

//-----------------------------------------------------------------------------
// A couple of inline helper methods
//-----------------------------------------------------------------------------
inline bool IsSolid(SolidType_t solidType, int nSolidFlags) {
	return (solidType != SOLID_NONE) && ((nSolidFlags & FSOLID_NOT_SOLID) == 0);
}

bool IsEntityBreakable(IHandleEntity* ent);

bool StandardFilterRules(IHandleEntity* pHandleEntity, int fContentsMask, int collisionGroup);

bool PassServerEntityFilter(const IHandleEntity *pTouch, const IHandleEntity *pPass);

bool BaseShouldHitEntity(IHandleEntity* pSkip, IHandleEntity *pHandleEntity, int m_collisionGroup, int contentsMask);

bool GameRulesShouldCollide(int collisionGroup0, int collisionGroup1);

class IGameRules {

};

///////////////////////////////////////////////////////////////////////////////



class ITraceFilter {
public:
	ITraceFilter(void* skip = nullptr, int col = COLLISION_GROUP_NONE) : pSkip(skip), m_iCollisionGroup(col) {}
public:
	virtual bool ShouldHitEntity(IHandleEntity* pEntityHandle, int contentsMask) {
		return !(pEntityHandle == pSkip);
		return BaseShouldHitEntity(static_cast<IHandleEntity*>(pSkip), pEntityHandle, m_iCollisionGroup, contentsMask);
	}
	virtual TraceType_t	GetTraceType() {
		return TRACE_EVERYTHING;
	}

	int m_iCollisionGroup;
	void* pSkip;
};

class CTraceFilter : public ITraceFilter {
public:
	CTraceFilter(void* skip = nullptr, int col = COLLISION_GROUP_NONE) {
		pSkip = skip;
		m_iCollisionGroup = col;
	} 
};

class CTraceFilterIgnoreGrenades {
public:
	virtual bool ShouldHitEntity(IHandleEntity* pEntityHandle, int contentsMask) {
		if (!pEntityHandle) {
			return !(pEntityHandle == pSkip);
		}

		//return BaseShouldHitEntity( pSkip, ( IHandleEntity* ) pEntityHandle, m_icollisionGroup, contentsMask );

		ClientClass *clclass = ((CBaseEntity*)pEntityHandle)->GetClientClass();

		if (clclass) {
			if (strstr(clclass->m_pNetworkName, "CBaseCSGrenadeProjectile"))
				return false;

			//if ( clclass->m_ClassID == ClassID::_CBaseCSGrenadeProjectile )
			//	return false;
		}

		return !(pEntityHandle == pSkip);

		//return BaseShouldHitEntity( pSkip, ( IHandleEntity* ) pEntityHandle, m_icollisionGroup, contentsMask );
	}

	virtual TraceType_t    GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	IHandleEntity* pSkip;
	int m_icollisionGroup;
};

class CGrenadePredictTraceFilter {
public:
	virtual bool ShouldHitEntity(IHandleEntity* pEntityHandle, int contentsMask) {
		if (!pEntityHandle)
			return !(pEntityHandle == pSkip);
		//return BaseShouldHitEntity( pSkip, ( IHandleEntity* ) pEntityHandle, m_icollisionGroup, contentsMask );


		//if (contentsMask & CONTENTS_WINDOW)
		//    return false;

		if (pEntityHandle == pSkip)
			return false;

		if (((CBaseEntity*)pEntityHandle)->IsPlayer())
			return false;

		ClientClass *clclass = ((CBaseEntity*)pEntityHandle)->GetClientClass();

		if (clclass) {
			if (strstr(clclass->m_pNetworkName, "CBaseCSGrenadeProjectile"))
				return false;

			//if ( ReadInt( ( uintptr_t ) &clclass->m_ClassID ) == ClassID::_CBaseCSGrenadeProjectile )
			//return false;
		}

		return !(pEntityHandle == pSkip);

		//return BaseShouldHitEntity( pSkip, ( IHandleEntity* ) pEntityHandle, m_icollisionGroup, contentsMask );
	}

	virtual TraceType_t    GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	IHandleEntity* pSkip;
	int m_icollisionGroup;
};

class CTraceFilterSkipTwoEntities : public CTraceFilter
{
public:
	CTraceFilterSkipTwoEntities(IHandleEntity *pPassEnt1, IHandleEntity *pPassEnt2, int collisionGroup) {
		pSkip = pPassEnt1;
		pSkip2 = pPassEnt2;
		m_iCollisionGroup = collisionGroup;
	};

	virtual bool ShouldHitEntity(IHandleEntity *pEntityHandle, int contentsMask) {
		return !(pEntityHandle == pSkip || pEntityHandle == pSkip2);
		if (!PassServerEntityFilter(pEntityHandle, pSkip2))
			return false;
		return BaseShouldHitEntity(pEntityHandle, (IHandleEntity*)pSkip, m_iCollisionGroup, contentsMask);
	}

	//virtual void SetPassEntity2(const IHandleEntity *pPassEntity2) { pSkip2 = pPassEntity2; }

public:
	const IHandleEntity *pSkip2;
};

class CTraceFilterNoPlayers : public CTraceFilter {
public:
	virtual bool ShouldHitEntity(IHandleEntity* pEntityHandle, int contentsMask) {
		if (!pEntityHandle)
			return false;
		if (reinterpret_cast<CBaseEntity*>(pEntityHandle)->IsPlayer())
			return false;
		return true;
	}
};

class CTraceFilterPlayersOnly : public ITraceFilter {
public:
	virtual bool ShouldHitEntity(CBaseEntity* pEntityHandle, int contentsMask) {
		if (!pEntityHandle)
			return false;
		if (pEntityHandle->IsPlayer())
			return true;
		return false;
	}

	virtual TraceType_t	GetTraceType() {
		return TRACE_ENTITIES_ONLY;
	}
};

class CTraceFilterEntitiesOnly : public ITraceFilter {
public:
	virtual bool ShouldHitEntity(IHandleEntity* pEntityHandle, int contentsMask) {
		if (!pEntityHandle)
			return false;
		return true;
	}

	virtual TraceType_t	GetTraceType() {
		return TRACE_ENTITIES_ONLY;
	}
};

class CTraceFilterIgnoreWorld : public CTraceFilter {
public:
	virtual bool ShouldHitEntity(IHandleEntity* pEntityHandle, int contentsMask) {
		if (!pEntityHandle)
			return false;
		return true;
	}

	virtual TraceType_t	GetTraceType() {
		return TRACE_ENTITIES_ONLY;
	}
};

typedef bool(*ShouldHitFunc_t)(IHandleEntity *pHandleEntity, int contentsMask);


class CTraceFilterSimple : public CTraceFilter {
public:
	CTraceFilterSimple(const CBaseEntity*passentity, int collisionGroup, ShouldHitFunc_t pExtraShouldHitCheckFn = NULL) {
		m_pPassEnt = passentity;
		m_collisionGroup = collisionGroup;
		m_pExtraShouldHitCheckFunction = pExtraShouldHitCheckFn;
	}
	virtual bool ShouldHitEntity(CBaseEntity* pHandleEntity, int contentsMask) {
		return !(pHandleEntity == m_pPassEnt);
	}

	virtual void SetPassEntity(const IHandleEntity *pPassEntity) { m_pPassEnt = pPassEntity; }
	virtual void SetCollisionGroup(int iCollisionGroup) { m_collisionGroup = iCollisionGroup; }

	const IHandleEntity* GetPassEntity(void) { return m_pPassEnt; }

private:
	const IHandleEntity* m_pPassEnt;
	int m_collisionGroup;
	ShouldHitFunc_t m_pExtraShouldHitCheckFunction;

};

class CAutowallFilter : public CTraceFilter {
public:
	bool ShouldHitEntity(IHandleEntity* ent, unsigned int mask) {
		return (ent != me && ent->GetRefEHandle() != me->GetRefEHandle() && ent->GetRefEHandle() != reinterpret_cast<IHandleEntity*>(mywep)->GetRefEHandle());
	}

	CBaseEntity* me;
	CBaseCombatWeapon* mywep;
};

class CBaseTrace {
public:
	Vector			startpos;
	Vector			endpos;
	cplane_t		plane;

	float			fraction;

	int				contents;
	unsigned short	dispFlags;

	bool			allsolid;
	bool			startsolid;

	CBaseTrace() {}
};

struct csurface_t {
	const char*		name;
	short			surfaceProps;
	unsigned short	flags;
};

class CGameTrace : public CBaseTrace {
public:
	bool                    DidHitWorld() const;
	bool                    DidHitNonWorldEntity() const;
	int                     GetEntityIndex() const;
	bool                    DidHit() const {
		return fraction < 1.f || allsolid || startsolid;
	}
	bool					IsVisible() const;

public:

	float                   fractionleftsolid;
	csurface_t              surface;
	int                     hitgroup;
	short                   physicsbone;
	unsigned short          worldSurfaceIndex;
	CBaseEntity*			m_pEnt;
	int                     hitbox;

	CGameTrace() {}

private:
	CGameTrace(const CGameTrace& vOther);
};

typedef CGameTrace trace_t;