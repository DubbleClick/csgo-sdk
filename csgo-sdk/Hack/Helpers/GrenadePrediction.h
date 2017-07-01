#pragma once

#include "../../sdk/Classes Structs/Trace.h"
#include "../../sdk/Interfaces/IEngineTrace.h"

#define GrenadePrediction CGrenadePrediction::GetInstance()

class CGrenadePrediction {
	IMPLEMENT_SINGLETON(CGrenadePrediction);

public:
	void Tick(int buttons);
	void PredictGrenade();
	void PaintGrenadeTrajectory();
private:
	void Setup(CBaseEntity* pl, Vector& vecSrc, Vector& vecThrow, const QAngle& angEyeAngles);
	void Simulate();
	int  Step(Vector& vecSrc, Vector& vecThrow, int tick, float interval);
	bool CheckDetonate(const Vector& vecThrow, const trace_t& tr, int tick, float interval);
	void AddGravityMove(Vector& move, Vector& vel, float frametime, bool onground);
	void HullTrace(Vector src, Vector end, ITraceFilter* filter, trace_t& tr);
	void PushEntity(Vector& src, const Vector& move, trace_t& tr);
	void ResolveFlyCollisionCustom(trace_t& tr, Vector& vecVelocity, float interval);
	unsigned char PhysicsClipVelocity(const Vector& in, const Vector& normal, Vector& out, float overbounce);
};
