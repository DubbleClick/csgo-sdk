#include "../../csgo-sdk.h"

float CLagcompensation::GetLerpTime() const {
	static ConVar* cl_interp = Interfaces->Cvar->FindVar("cl_interp");
	static ConVar* cl_updaterate = Interfaces->Cvar->FindVar("cl_updaterate");
	static ConVar* cl_interp_ratio = Interfaces->Cvar->FindVar("cl_interp_ratio");
	static ConVar* sv_maxupdaterate = Interfaces->Cvar->FindVar("sv_maxupdaterate");
	static ConVar* sv_minupdaterate = Interfaces->Cvar->FindVar("sv_minupdaterate");
	static ConVar* sv_client_min_interp_ratio = Interfaces->Cvar->FindVar("sv_client_min_interp_ratio");
	static ConVar* sv_client_max_interp_ratio = Interfaces->Cvar->FindVar("sv_client_max_interp_ratio");

	auto Interp = cl_interp->GetFloat();
	auto UpdateRate = cl_updaterate->GetFloat();
	auto InterpRatio = static_cast<float>(cl_interp_ratio->GetInt());
	auto MaxUpdateRate = static_cast<float>(sv_maxupdaterate->GetInt());
	auto MinUpdateRate = static_cast<float>(sv_minupdaterate->GetInt());
	auto ClientMinInterpRatio = sv_client_min_interp_ratio->GetFloat();
	auto ClientMaxInterpRatio = sv_client_max_interp_ratio->GetFloat();

	if (ClientMinInterpRatio > InterpRatio)
		InterpRatio = ClientMinInterpRatio;

	if (InterpRatio > ClientMaxInterpRatio)
		InterpRatio = ClientMaxInterpRatio;

	if (MaxUpdateRate <= UpdateRate)
		UpdateRate = MaxUpdateRate;

	if (MinUpdateRate > UpdateRate)
		UpdateRate = MinUpdateRate;

	auto v20 = InterpRatio / UpdateRate;
	if (v20 <= Interp)
		return Interp;

	else
		return v20;
}

