#pragma once

#define square( x ) ( x * x )

extern "C" typedef int(__cdecl* RandomSeedFn)(int iSeed);
extern "C" typedef float(__cdecl* RandomFloatFn)(float flMinVal, float flMaxVal);
extern "C" typedef float(__cdecl* RandomFloatExpFn)(float flMinVal, float flMaxVal, float flExponent);
extern "C" typedef int(__cdecl* RandomIntFn)(int iMinVal, int iMaxVal);
extern "C" typedef float(__cdecl* RandomGaussianFloatFn)(float flMean, float flStdDev);

namespace Math
{
	extern inline void SinCos(float radians, float *sine, float *cosine);
	extern float DotProduct(Vector a, Vector b);
	extern void VectorAngles(const Vector& forward, QAngle& angles);
	extern void AngleVectors(const QAngle& angles, Vector& forward);
	extern void AngleVectors(const QAngle& angles, Vector* forward, Vector* right, Vector* up);
	extern QAngle CalcAngle(Vector v1, Vector v2);
	extern float GetFov(const QAngle& viewAngle, const QAngle& aimAngle);
	extern float VectorDistance(Vector v1, Vector v2);
	extern Vector VectorTransform(const Vector& in1, const  matrix3x4a_t& in2);
	extern float GetDelta(float hspeed, float maxspeed, float airaccelerate);
	extern Vector ExtrapolateTick(Vector p0, Vector v0);
	extern float GetHitchance(CBaseEntity* ent, QAngle angle);

	extern RandomSeedFn RandomSeed;
	extern RandomFloatFn RandomFloat;
	extern RandomFloatExpFn RandomFloatExp;
	extern RandomIntFn RandomInt;
	extern RandomGaussianFloatFn RandomGaussianFloat;

	inline float NormalizeFloat(float angle) {
		auto revolutions = angle / 360.f;
		if (angle > 180.f || angle < -180.f) {
			revolutions = round(abs(revolutions));
			if (angle < 0.f)
				angle = (angle + 360.f * revolutions);
			else
				angle = (angle - 360.f * revolutions);
			return angle;
		}
		return angle;
	}

	inline float ClampYaw(float yaw) {
		while (yaw > 180.f)
			yaw -= 360.f;
		while (yaw < -180.f)
			yaw += 360.f;
		return yaw;
	}

	template <class T>
	__forceinline T Lerp(float flPercent, T const& A, T const& B) {
		return A + (B - A) * flPercent;
	}
}
