#pragma once

class CBaseCombatWeapon;
enum SolidType_t;

class IHandleEntity;
class IClientNetworkable;
class IClientRenderable;
class ICollideable;
class IClientEntity;
class IClientThinkable;
class IClientUnknown;
class IClientModelRenderable;
class IClientAlphaProperty;
class CBaseHandle;
class CBaseAnimating;
class C_BaseAnimatingOverlay;
struct Ray_t;
class CGameTrace;
typedef CGameTrace trace_t;
class CMouthInfo;
class IClientEntityInternal;
struct SpatializationInfo_t;

class ICollideable {
public:
	// Gets at the entity handle associated with the collideable
	virtual IHandleEntity*		GetEntityHandle() = 0;

	virtual const Vector&		OBBMins() const = 0;
	virtual const Vector&		OBBMaxs() const = 0;

	virtual void				WorldSpaceTriggerBounds(Vector *pVecWorldMins, Vector *pVecWorldMaxs) const = 0;
	virtual bool				TestCollision(const Ray_t &ray, unsigned int fContentsMask, trace_t& tr) = 0;
	virtual bool				TestHitboxes(const Ray_t &ray, unsigned int fContentsMask, trace_t& tr) = 0;
	virtual int					GetCollisionModelIndex() = 0;
	virtual const model_t*		GetCollisionModel() = 0;

	virtual const Vector&		GetCollisionOrigin() const = 0;
	virtual const QAngle&		GetCollisionAngles() const = 0;
	virtual const matrix3x4_t&	CollisionToWorldTransform() const = 0;

	virtual SolidType_t			GetSolid() const = 0;
	virtual int					GetSolidFlags() const = 0;
	virtual IClientUnknown*		GetIClientUnknown() = 0;
	virtual int					GetCollisionGroup() const = 0;
	virtual void				WorldSpaceSurroundingBounds(Vector *pVecMins, Vector *pVecMaxs) = 0;
	virtual bool				ShouldTouchTrigger(int triggerSolidFlags) const = 0;
	virtual const matrix3x4_t*	GetRootParentToWorldTransform() const = 0;
};

class IHandleEntity {
public:
	virtual ~IHandleEntity() {}
	virtual void				SetRefEHandle(const CBaseHandle &handle) = 0;
	virtual const CBaseHandle&	GetRefEHandle() const = 0;
};

class IClientRenderable {
public:
	virtual IClientUnknown*		GetIClientUnknown() = 0;

	virtual Vector const&		GetRenderOrigin(void) = 0;
	virtual QAngle const&		GetRenderAngles(void) = 0;
	virtual bool				ShouldDraw(void) = 0;
	virtual int					GetRenderFlags(void) = 0; // ERENDERFLAGS_xxx
	virtual void				Unused(void) const {}
};

class IClientNetworkable {
public:
	virtual IClientUnknown*	GetIClientUnknown() = 0;
	virtual void			Release() = 0;
	virtual ClientClass*	GetClientClass() = 0;
	virtual void			NotifyShouldTransmit(int state) = 0;
	virtual void			OnPreDataChanged(int updateType) = 0;
	virtual void			OnDataChanged(int updateType) = 0;
	virtual void			PreDataUpdate(int updateType) = 0;
	virtual void			PostDataUpdate(int updateType) = 0;
	virtual bool			IsDormant(void) = 0;
	virtual int				entindex(void) const = 0;
	virtual void			ReceiveMessage(int classID, byte& msg) = 0; //bf_read&
	virtual void*			GetDataTableBasePtr() = 0;
	virtual void			SetDestroyedOnRecreateEntities(void) = 0;

	virtual void			OnDataUnchangedInPVS() = 0;
};

class IClientUnknown : public IHandleEntity {
public:
	virtual ICollideable*			GetCollideable() = 0;
	virtual IClientNetworkable*		GetClientNetworkable() = 0;
	virtual IClientRenderable*		GetClientRenderable() = 0;
	virtual IClientEntity*			GetIClientEntity() = 0;
	virtual CBaseEntity*			GetBaseEntity() = 0;
	virtual IClientThinkable*		GetClientThinkable() = 0;
	//virtual IClientModelRenderable*	GetClientModelRenderable() = 0;
	virtual IClientAlphaProperty*	GetClientAlphaProperty() = 0;
};

class IClientThinkable {
public:
	virtual IClientUnknown*		GetIClientUnknown() = 0;
	virtual void				ClientThink() = 0;
	virtual void*				GetThinkHandle() = 0;
	virtual void				SetThinkHandle(void* hThink) = 0;
	virtual void				Release() = 0;
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable {
public:
	virtual void				Release(void) = 0;
	virtual const Vector&		GetAbsOrigin(void) const = 0;
	virtual const QAngle&		GetAbsAngles(void) const = 0;
	virtual CMouthInfo*			GetMouth(void) = 0;
	virtual bool				GetSoundSpatialization(SpatializationInfo_t& info) = 0;
	virtual bool				IsBlurred(void) = 0;
};