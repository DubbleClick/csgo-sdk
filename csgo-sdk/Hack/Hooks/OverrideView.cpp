#include "../../csgo-sdk.h"

void UpdateThirdPerson(CViewSetup*);

OverrideViewFn oOverrideView;
void __stdcall Hooks::OverrideView(CViewSetup* vsView) {
	if (G::LocalPlayer && G::LocalPlayer->GetWeapon() && (G::LocalPlayer->GetWeapon()->GetZoomLevel() == 0 || !G::LocalPlayer->GetWeapon()->IsGun()))
		vsView->fov = (float)Config->Misc.FoV;

	if (Interfaces->EngineClient->IsInGame())
		UpdateThirdPerson(vsView);

	if (Config->Visual.Enabled && Config->Visual.GrenadePredictionEnabled)
		GrenadePrediction->PredictGrenade();

	oOverrideView(vsView);
}

#define CAM_HULL_OFFSET		14.0    // the size of the bounding hull used for collision checking
#define CAM_MIN_DIST 30.0
#define CAM_ANGLE_MOVE .5
#define CAMERA_UP_OFFSET	25.0f
#define CAMERA_OFFSET_LERP_TIME 0.5f

bool ThirdPerson = false; //Global bool that determines whether or not third person is enabled or disabled
QAngle camAngles = QAngle(0, 0, 0);
Vector camOrigin = Vector(0, 0, 0);
static const Vector CAM_HULL_MIN = Vector(-CAM_HULL_OFFSET, -CAM_HULL_OFFSET, -CAM_HULL_OFFSET);
static const Vector CAM_HULL_MAX = Vector(CAM_HULL_OFFSET, CAM_HULL_OFFSET, CAM_HULL_OFFSET);
// What is the current camera offset from the view origin?
Vector m_vecCameraOffset = Vector(0, 0, 0);
Vector m_vecDesiredCameraOffset;
static float m_flFraction = 0;
static float m_flUpFraction = 0;
static float m_flTargetFraction = 0;
static float m_flTargetUpFraction = 0;
static float m_flLerpTime = 0;
static float m_flUpLerpTime = 0;
static float idealpitch = 0.0f;
static float idealyaw = 0.0f;
static float idealdist = 150.0f;
const bool cl_thirdperson = false;
bool m_bOverrideThirdPerson = false;

ConVar* cam_idealdist = nullptr;
ConVar* cam_idealdistright = nullptr;
ConVar* cam_idealdistup = nullptr;
ConVar* cam_ideallag = nullptr;
ConVar* cam_idealyaw = nullptr;
ConVar* cam_idealpitch = nullptr;
ConVar* cam_collision = nullptr;
ConVar* cam_snapto = nullptr;
ConVar* cam_idealdelta = nullptr;
ConVar* c_minyaw = nullptr;
ConVar* c_maxyaw = nullptr;
ConVar* c_minpitch = nullptr;
ConVar* c_maxpitch = nullptr;
ConVar* c_mindistance = nullptr;
ConVar* c_maxdistance = nullptr;

inline void SetDesiredCameraOffset(const Vector& vecOffset) { m_vecDesiredCameraOffset = vecOffset; }
inline const Vector& GetDesiredCameraOffset(void) { return m_vecDesiredCameraOffset; }
inline void SetCameraOffsetAngles(const Vector& vecOffset) { m_vecCameraOffset = vecOffset; }
inline const Vector& GetCameraOffsetAngles(void) { return m_vecCameraOffset; }

float MoveToward(float cur, float goal, float lag) {
	if (cur != goal) {
		if (abs(cur - goal) > 180.0) {
			if (cur < goal)
				cur += 360.0;
			else
				cur -= 360.0;
		}

		if (cur < goal) {
			if (cur < goal - 1.0)
				cur += (goal - cur) / lag;
			else
				cur = goal;
		}
		else {
			if (cur > goal + 1.0)
				cur -= (cur - goal) / lag;
			else
				cur = goal;
		}
	}


	// bring cur back into range
	if (cur < 0)
		cur += 360.0;
	else if (cur >= 360)
		cur -= 360;

	return cur;
}

void PositionCamera(CBaseEntity *pPlayer, const QAngle& angles) {
	if (pPlayer) {
		trace_t trace;

		Vector camForward, camRight, camUp;

		// find our player's origin, and from there, the eye position
		Vector origin = pPlayer->GetOrigin();
		origin += pPlayer->GetViewOffset();

		Math::AngleVectors((QAngle)angles, &camForward, &camRight, &camUp); //+ punchangle

		Vector endPos = origin;

		Vector vecCamOffset = endPos + (camForward * -GetDesiredCameraOffset()[0]) + (camRight * GetDesiredCameraOffset()[1]) + (camUp * GetDesiredCameraOffset()[2]);

		// use our previously #defined hull to collision trace
		CTraceFilter traceFilter;
		traceFilter.pSkip = (IHandleEntity*)pPlayer;
		traceFilter.m_iCollisionGroup = COLLISION_GROUP_NONE;
		Ray_t ray;
		ray.Init(endPos, vecCamOffset, CAM_HULL_MIN, CAM_HULL_MAX);
		Interfaces->EngineTrace->TraceRay(ray, MASK_SOLID & ~CONTENTS_MONSTER, (ITraceFilter*)&traceFilter, &trace);

		if (trace.fraction != m_flTargetFraction) {
			m_flLerpTime = Interfaces->Globals->curtime;
		}

		m_flTargetFraction = trace.fraction;
		m_flTargetUpFraction = 1.0f;

		//If we're getting closer to a wall snap the fraction right away.
		if (m_flTargetFraction < m_flFraction) {
			m_flFraction = m_flTargetFraction;
			m_flLerpTime = Interfaces->Globals->curtime;
		}

		// move the camera closer if it hit something
		if (trace.fraction < 1.0) {
			m_vecCameraOffset[2] *= trace.fraction;

			Ray_t ray2;
			ray2.Init(endPos, endPos + (camForward * (-GetDesiredCameraOffset()[0])), CAM_HULL_MIN, CAM_HULL_MAX);

			Interfaces->EngineTrace->TraceRay(ray2, MASK_SOLID & ~CONTENTS_MONSTER, &traceFilter, &trace);

			if (trace.fraction != 1.0f) {
				if (trace.fraction != m_flTargetUpFraction) {
					m_flUpLerpTime = Interfaces->Globals->curtime;
				}

				m_flTargetUpFraction = trace.fraction;

				if (m_flTargetUpFraction < m_flUpFraction) {
					m_flUpFraction = trace.fraction;
					m_flUpLerpTime = Interfaces->Globals->curtime;
				}
			}
		}
	}
}

void CAM_Think() {
	Vector idealAngles = Vector(0.0f, 0.0f, idealdist);
#if 0
	Vector idealAngles = Vector(idealpitch, idealyaw, idealdist);
	POINT cp;
	//get windows cursor position
	GetCursorPos(&cp);

	const int m_nCameraX = cp.x;
	const int m_nCameraY = cp.y;

	const int x = CenterOfScreen.x;
	const int y = CenterOfScreen.y;

	//keep the camera within certain limits around the player (ie avoid certain bad viewing angles)  
	if (m_nCameraX > x) {
		//if ((idealAngles[YAW]>=225.0)||(idealAngles[YAW]<135.0))
		if (idealAngles[1] < c_maxyaw->GetFloat()) {
			idealAngles[1] += CAM_ANGLE_MOVE *((m_nCameraX - x) / 2);
		}
		if (idealAngles[1] > c_maxyaw->GetFloat()) {
			idealAngles[1] = c_maxyaw->GetFloat();
		}
	}
	else if (m_nCameraX < x) {
		//if ((idealAngles[YAW]<=135.0)||(idealAngles[YAW]>225.0))
		if (idealAngles[1] > c_minyaw->GetFloat()) {
			idealAngles[1] -= CAM_ANGLE_MOVE * ((x - m_nCameraX) / 2);
		}
		if (idealAngles[1] < c_minyaw->GetFloat()) {
			idealAngles[1] = c_minyaw->GetFloat();
		}
	}

	//check for y delta values and adjust accordingly
	//eventually adjust PITCH based on amount of movement
	//also make sure camera is within bounds
	if (m_nCameraY > y) {
		if (idealAngles[0] < c_maxpitch->GetFloat()) {
			idealAngles[0] += (CAM_ANGLE_MOVE)* ((m_nCameraY - y) / 2);
		}
		if (idealAngles[0] > c_maxpitch->GetFloat()) {
			idealAngles[0] = c_maxpitch->GetFloat();
		}
	}
	else if (m_nCameraY < y) {
		if (idealAngles[0] > c_minpitch->GetFloat()) {
			idealAngles[0] -= (CAM_ANGLE_MOVE)*((y - m_nCameraY) / 2);
		}
		if (idealAngles[0] < c_minpitch->GetFloat()) {
			idealAngles[0] = c_minpitch->GetFloat();
		}
	}

	//Nathan code here
	if ((GetAsyncKeyState(VK_NUMPAD8) & (1 << 16)))
		idealAngles[0] += cam_idealdelta->GetFloat();
	else if ((GetAsyncKeyState(VK_NUMPAD2) & (1 << 16)))
		idealAngles[0] -= cam_idealdelta->GetFloat();

	if ((GetAsyncKeyState(VK_NUMPAD4) & (1 << 16)))
		idealAngles[1] -= cam_idealdelta->GetFloat();
	else if ((GetAsyncKeyState(VK_NUMPAD6) & (1 << 16)))
		idealAngles[1] += cam_idealdelta->GetFloat();

	if ((GetAsyncKeyState(VK_ADD) & (1 << 16))) {
		idealAngles[2] -= 2 * cam_idealdelta->GetFloat();
		if (idealAngles[2] < CAM_MIN_DIST) {
			// If we go back into first person, reset the angle
			idealAngles[0] = 0;
			idealAngles[1] = 0;
			idealAngles[2] = CAM_MIN_DIST;
	}

}
	else if ((GetAsyncKeyState(VK_SUBTRACT) & (1 << 16)))
		idealAngles[2] += 2 * cam_idealdelta->GetFloat();
#endif

	QAngle viewangles;
	Interfaces->EngineClient->GetViewAngles(viewangles);
#if 0
	// bring the pitch values back into a range that MoveToward can handle
	if (idealAngles[0] > 180)
		idealAngles[0] -= 360;
	else if (idealAngles[0] < -180)
		idealAngles[0] += 360;

	// bring the yaw values back into a range that MoveToward can handle
	// --
	// Vitaliy: going with >= 180 and <= -180.
	// This introduces a potential discontinuity when looking directly at model face
	// as camera yaw will be jumping from +180 to -180 and back, but when working with
	// the camera allows smooth rotational transitions from left to right and back.
	// Otherwise one of the transitions that has ">"-comparison will be locked.
	// --
	if (idealAngles[1] >= 180)
		idealAngles[1] -= 360;
	else if (idealAngles[1] <= -180)
		idealAngles[1] += 360;

	// clamp pitch, yaw and dist...
	idealAngles[0] = std::clamp(idealAngles[0], c_minpitch->GetFloat(), c_maxpitch->GetFloat());
	idealAngles[1] = std::clamp(idealAngles[1], c_minyaw->GetFloat(), c_maxyaw->GetFloat());
	idealAngles[2] = std::clamp(idealAngles[2], c_mindistance->GetFloat(), c_maxdistance->GetFloat());
#endif
	// update ideal angles
	idealpitch = idealAngles[0];
	idealyaw = idealAngles[1];
	idealdist = idealAngles[2];

	// Move the CameraOffset "towards" the idealAngles
	// Note: CameraOffset = viewangle + idealAngle
	Vector camOffset;

	if (cam_snapto->GetInt()) {
		camOffset[1] = idealyaw + viewangles[1];
		camOffset[0] = idealpitch + viewangles[0];
		camOffset[2] = idealdist;
	}
	else {
		float lag = max(1, 1 + cam_ideallag->GetFloat());
		camOffset = GetCameraOffsetAngles();

		if (camOffset[1] - viewangles[1] != idealyaw)
			camOffset[1] = MoveToward(camOffset[1], idealyaw + viewangles[1], lag);

		if (camOffset[0] - viewangles[0] != idealpitch)
			camOffset[0] = MoveToward(camOffset[0], idealpitch + viewangles[0], lag);

		if (abs(camOffset[2] - idealdist) < 2.0)
			camOffset[2] = idealdist;
		else
			camOffset[2] += (idealdist - camOffset[2]) / lag;
	}

	if (cam_collision->GetInt()) {
		QAngle desiredCamAngles;
		if (m_bOverrideThirdPerson == false) {
			desiredCamAngles = viewangles;
		}
		else {
			desiredCamAngles = QAngle(camOffset[0], camOffset[1], camOffset[2]);
		}
		CBaseEntity *pCameraTarget = G::LocalPlayer;
		auto hObserverTarget = G::LocalPlayer->GetObserverTarget();
		if (hObserverTarget) {
			CBaseEntity* t = Interfaces->ClientEntityList->GetClientEntityFromHandle((DWORD)hObserverTarget);
			if (t)
				pCameraTarget = t;
		}
		PositionCamera(pCameraTarget, desiredCamAngles);
	}

	SetCameraOffsetAngles(camOffset);
}

Vector GetFinalCameraOffset(void) {
	Vector desired = GetDesiredCameraOffset();

	if (/*ThirdPersonOffsetChk.Checked &&*/1 && m_flUpFraction != 1.0f) {
		desired.z += CAMERA_UP_OFFSET;
	}

	return desired;
}

float RemapValClamped(float val, float A, float B, float C, float D) {
	if (A == B)
		return val >= B ? D : C;
	float cVal = (val - A) / (B - A);
	cVal = std::clamp(cVal, 0.0f, 1.0f);

	return C + (D - C) * cVal;
}

Vector GetDistanceFraction(void) {
	if (m_bOverrideThirdPerson == true) {
		return Vector(m_flTargetFraction, m_flTargetFraction, m_flTargetFraction);
	}

	float flFraction = m_flFraction;
	float flUpFraction = m_flUpFraction;

	float flFrac = RemapValClamped(Interfaces->Globals->curtime - m_flLerpTime, 0, CAMERA_OFFSET_LERP_TIME, 0, 1);

	flFraction = Math::Lerp(flFrac, m_flFraction, m_flTargetFraction);

	if (flFrac == 1.0f) {
		m_flFraction = m_flTargetFraction;
	}

	flFrac = RemapValClamped(Interfaces->Globals->curtime - m_flUpLerpTime, 0, CAMERA_OFFSET_LERP_TIME, 0, 1);

	flUpFraction = 1.0f - Math::Lerp(flFrac, m_flUpFraction, m_flTargetUpFraction);

	if (flFrac == 1.0f) {
		m_flUpFraction = m_flTargetUpFraction;
	}

	return Vector(flFraction, flFraction, flUpFraction);
}

__forceinline void VectorMA(const Vector& start, float scale, const Vector& direction, Vector& dest) {
	dest.x = start.x + direction.x*scale;
	dest.y = start.y + direction.y*scale;
	dest.z = start.z + direction.z*scale;
}

void GetCameraCommands() {
	cam_idealdist = Interfaces->Cvar->FindVar("cam_idealdist");
	cam_idealdistright = Interfaces->Cvar->FindVar("cam_idealdistright");
	cam_idealdistup = Interfaces->Cvar->FindVar("cam_idealdistup");
	cam_ideallag = Interfaces->Cvar->FindVar("cam_ideallag");
	cam_idealpitch = Interfaces->Cvar->FindVar("cam_idealpitch");
	cam_idealyaw = Interfaces->Cvar->FindVar("cam_idealyaw");
	cam_collision = Interfaces->Cvar->FindVar("cam_collision");
	cam_snapto = Interfaces->Cvar->FindVar("cam_snapto");
	c_minyaw = Interfaces->Cvar->FindVar("c_minyaw");
	c_maxyaw = Interfaces->Cvar->FindVar("c_maxyaw");
	c_minpitch = Interfaces->Cvar->FindVar("c_minpitch");
	c_maxpitch = Interfaces->Cvar->FindVar("c_maxpitch");
	c_mindistance = Interfaces->Cvar->FindVar("c_mindistance");
	c_maxdistance = Interfaces->Cvar->FindVar("c_maxdistance");
	cam_idealdelta = Interfaces->Cvar->FindVar("cam_idealdelta");
}

void CAM_ToThirdPerson(void) {
	if (cl_thirdperson == false) {
		SetDesiredCameraOffset(Vector(idealdist, cam_idealdistright->GetFloat(), cam_idealdistup->GetFloat()));
		m_bOverrideThirdPerson = true;
	}

	QAngle viewangles;
	Interfaces->EngineClient->GetViewAngles(viewangles);
	SetCameraOffsetAngles(Vector(viewangles[1], viewangles[0], CAM_MIN_DIST));
	*(int*)((uintptr_t)G::LocalPlayer + offsets->m_iObserverMode) = 1;
	//G::LocalPlayer->SetObserverMode(1);
}

int LocalPlayerOriginalObserverMode = 0;
void CAM_ToFirstPerson(void) {
	if (cl_thirdperson == false)
		m_bOverrideThirdPerson = false;

	SetDesiredCameraOffset(Vector(0, 0, 0));
	*(int*)((uintptr_t)G::LocalPlayer + offsets->m_iObserverMode) = LocalPlayerOriginalObserverMode;
	//G::LocalPlayer->SetObserverMode(LocalPlayerOriginalObserverMode);
}

void UpdateThirdPerson(CViewSetup *pSetup) {
	if (!c_maxpitch) { //do once
		GetCameraCommands();
	}

	static int delaytick = 100;

	if (Config->Misc.ThirdPersonSmart) {
		if (!G::LocalPlayer->IsAlive() && G::LocalPlayer->GetObserverTarget() != INVALID_EHANDLE_INDEX)
			Config->Misc.ThirdPerson = true;
		else /*if (G::LocalPlayer->GetObserverTarget == -1)*/
			Config->Misc.ThirdPerson = false;
	}

	if (!Config->Misc.ThirdPerson) {
		if (ThirdPerson) {
			*(int*)((uintptr_t)G::LocalPlayer + offsets->m_iObserverMode) = G::LocalPlayer->IsAlive() ? 0 : 4;
			CAM_ToFirstPerson();
			ThirdPerson = false;
		}
		Util::ToggleThirdPerson(false);
		return;
	}
	else {
		if (G::LocalPlayer->IsAlive())
			*(int*)((uintptr_t)G::LocalPlayer + offsets->m_iObserverMode) = 0;
		else if (G::LocalPlayer->GetObserverTarget() != -1)
			*(int*)((uintptr_t)G::LocalPlayer + offsets->m_iObserverMode) = 5;
	}

	if (!Config->Misc.ThirdPerson) {
		ThirdPerson = false;
		return;
	}
	//Config->thirdperson must be on here or Config->smartperson + alive

	if (G::LocalPlayer->IsAlive()) {
		delaytick = 1000;
		Util::ToggleThirdPerson(true);
		return;
	}

	if (!ThirdPerson) {
		CAM_ToThirdPerson();
		ThirdPerson = true;
	}

	if (!ThirdPerson) {
		return;
	}

	if (delaytick > 0)
		delaytick--;

	if (delaytick <= 0) {
		return;
	}
	//U::ToggleThirdPerson(false);

	CBaseEntity* pCameraTarget = G::LocalPlayer;
	auto hObserverTarget = G::LocalPlayer->GetObserverTarget();
	if (hObserverTarget) {
		CBaseEntity* t = Interfaces->ClientEntityList->GetClientEntityFromHandle((DWORD)hObserverTarget);
		if (t)
			pCameraTarget = t;
	}
	pSetup->origin = pCameraTarget->GetEyePosition();

	CAM_Think();

	const Vector cam_ofs = GetCameraOffsetAngles();
	Vector cam_ofs_distance = GetFinalCameraOffset();

	cam_ofs_distance *= GetDistanceFraction();

	Vector camForward, camRight, camUp;

	if (m_bOverrideThirdPerson == false) {
		Interfaces->EngineClient->GetViewAngles(camAngles);
	}
	else {
		camAngles = QAngle(GetCameraOffsetAngles()[0], GetCameraOffsetAngles()[1], 0.0f);
	}

	// get the forward vector
	Math::AngleVectors(camAngles, &camForward, &camRight, &camUp);

	VectorMA(pSetup->origin, -cam_ofs_distance[0], camForward, pSetup->origin);
	VectorMA(pSetup->origin, cam_ofs_distance[1], camRight, pSetup->origin);
	VectorMA(pSetup->origin, cam_ofs_distance[2], camUp, pSetup->origin);

	camOrigin = pSetup->origin;

	// Override angles from third person camera
	pSetup->angles = camAngles.Clamped();
}
