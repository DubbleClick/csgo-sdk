#include "../../csgo-sdk.h"

RandomSeedFn Math::RandomSeed;
RandomFloatFn Math::RandomFloat;
RandomFloatExpFn Math::RandomFloatExp;
RandomIntFn Math::RandomInt;
RandomGaussianFloatFn Math::RandomGaussianFloat;

void inline Math::SinCos(float radians, float *sine, float *cosine) {
	*sine = sin(radians);
	*cosine = cos(radians);
}

float Math::DotProduct(Vector a, Vector b) {
	float one[] = { a.x, a.y, a.z };
	float two[] = { b.x, b.y, b.z };

	return std::inner_product(one, one + 3, two, 0.0f);
}

void Math::VectorAngles(const Vector& forward, QAngle& angles) {
	if (forward.y == 0 && forward.x == 0) {
		angles.x = forward.z > 0.f ? 270.f : 90.f;
		angles.y = 0;
	}

	else {
		angles.x = (atan2(-forward.z, forward.Length2D()) * 180 / M_PI);
		if (angles.x < 0)
			angles.x += 360;

		angles.y = (atan2(forward.y, forward.x) * 180 / M_PI);
		if (angles.y < 0)
			angles.y += 360;
	}

	angles.z = 0;
}

void Math::AngleVectors(const QAngle& angles, Vector& forward) {
	float sp, sy, cp, cy;

	SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
	SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);

	forward.x = cp*cy;
	forward.y = cp*sy;
	forward.z = -sp;
}

void Math::AngleVectors(const QAngle& angles, Vector* forward, Vector* right, Vector* up) {
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
	SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);
	SinCos(DEG2RAD(angles[ROLL]), &sr, &cr);

	if (forward) {
		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}

	if (right) {
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up) {
		up->x = (cr*sp*cy + -sr*-sy);
		up->y = (cr*sp*sy + -sr*cy);
		up->z = cr*cp;
	}
}

float Math::GetFov(const QAngle& viewAngle, const QAngle& aimAngle) {
	Vector ang, aim;

	AngleVectors(viewAngle, aim);
	AngleVectors(aimAngle, ang);

	return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}

QAngle Math::CalcAngle(Vector src, Vector dst) {
	QAngle angles;
	VectorAngles((dst - src), angles);
	angles.Normalize();
	return angles;
}

float Math::VectorDistance(Vector v1, Vector v2) {
	return sqrtf(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2) + pow(v1.z - v2.z, 2));
}

Vector Math::VectorTransform(const Vector& in1, const matrix3x4a_t& in2) {
	return Vector(in1.Dot(in2.m_flMatVal[0]) + in2.m_flMatVal[0][3], in1.Dot(in2.m_flMatVal[1]) + in2.m_flMatVal[1][3], in1.Dot(in2.m_flMatVal[2]) + in2.m_flMatVal[2][3]);
}

float Math::GetDelta(float hspeed, float maxspeed, float airaccelerate) {
	auto term = (30.0 - (airaccelerate * maxspeed / 66.0)) / hspeed;

	if (term < 1.0f && term > -1.0f) {
		return acosf(static_cast<float>(term));
	}

	return 0.f;
}

Vector Math::ExtrapolateTick(Vector p0, Vector v0) {
	return p0 + (v0 * Interfaces->Globals->interval_per_tick);
}

inline float Math::GetHitchance(CBaseEntity* ent, QAngle angle) {
	CBaseCombatWeapon* Weapon = G::LocalPlayer->GetWeapon();

	//angle.Normalize();
	Vector Forward, Right, Up, Dir;
	AngleVectors(angle, &Forward, &Right, &Up);

	int SeedsHit = 0;
	Weapon->UpdateAccuracyPenalty();
	float wSpread = Weapon->GetWeaponSpread();
	float wCone = Weapon->GetWeaponCone();
	Vector myEyePos = G::LocalPlayer->GetEyePosition();

	for (int i = 0; i < 256; i++) {
		RandomSeed(i + 1);

		float A = RandomFloat(0.0f, 1.0f);
		float B = RandomFloat(0.f, 2.0f * M_PI);
		float C = RandomFloat(0.0f, 1.0f);
		float D = RandomFloat(0.f, 2.0f * M_PI);

		float Spread = A * wSpread;
		float Cone = C * wCone;

		Vector vSpread((cos(B) * Spread) + (cos(D) * Cone), (sin(B) * Spread) + (sin(D) * Cone), 0), vDir;

		vDir.x = Forward.x + (Right.x * vSpread.x) + (Up.x * vSpread.y);
		vDir.y = Forward.y + (Right.y * vSpread.x) + (Up.y * vSpread.y);
		vDir.z = Forward.z + (Right.z * vSpread.x) + (Up.z * vSpread.y);

		Vector TraceEnd = myEyePos + (vDir * Weapon->GetCSWpnData()->m_flRange);

		CGameTrace tr;
		Ray_t ray;
		ray.Init(myEyePos, TraceEnd);
		CTraceFilterEntitiesOnly filter;
		filter.pSkip = G::LocalPlayer;
		filter.m_iCollisionGroup = COLLISION_GROUP_NONE;

		Interfaces->EngineTrace->ClipRayToEntity(ray, MASK_SHOT, ent, &tr);

		if (tr.m_pEnt == ent)
			SeedsHit++;
	}

	return ((SeedsHit / 256.f) * 100.f);
}
