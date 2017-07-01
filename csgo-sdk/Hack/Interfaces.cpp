#include "../csgo-sdk.h"

bool CInterfaces::Initialize() {
	Client = GetInterface<IBaseClientDll*>("client.dll", "VClient");
	ClientMode = **(IClientModeShared***)((*(uintptr_t**)Client)[10] + 0x5);
	Globals = **(IGlobalVarsBase***)((*(uintptr_t**)Client)[0] + 0x1B);
	Input = *(IInput**)(Util::FindPattern("client.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 0x1);

	ClientEntityList = GetInterface<IClientEntityList*>("client.dll", "VClientEntityList");
	Prediction = GetInterface<IPrediction*>("client.dll", "VClientPrediction");
	GameMovement = GetInterface<IGameMovement*>("client.dll", "GameMovement");

	Effects = GetInterface<IVEfx*>("engine.dll", "VEngineEffects");
	EngineClient = GetInterface<IEngineClient*>("engine.dll", "VEngineClient");
	EngineTrace = GetInterface<IEngineTrace*>("engine.dll", "EngineTraceClient");
	RenderView = GetInterface<IVRenderView*>("engine.dll", "VEngineRenderView");
	ModelRender = GetInterface<IVModelRender*>("engine.dll", "VEngineModel");
	ModelInfo = GetInterface<IVModelInfo*>("engine.dll", "VModelInfoClient");
	DebugOverlay = GetInterface<IVDebugOverlay*>("engine.dll", "VDebugOverlay");

	Cvar = GetInterface<ICVar*>("vstdlib.dll", "VEngineCvar");
	InputSystem = GetInterface<IInputSystem*>("inputsystem.dll", "InputSystemVersion");
	Surface = GetInterface<ISurface*>("vguimatsurface.dll", "VGUI_Surface");
	VPanel = GetInterface<IVPanel*>("vgui2.dll", "VGUI_Panel");
	MaterialSystem = GetInterface<IMaterialSystem*>("materialsystem.dll", "VMaterialSystem");
	Physprops = GetInterface<IPhysicsSurfaceProps*>("vphysics.dll", "VPhysicsSurfaceProps");
	StudioRender = GetInterface<IStudioRender*>("studiorender.dll", "VStudioRender");
	MdlCache = GetInterface<IMDLCache*>("datacache.dll", "MDLCache");

	MoveHelper = **(IMoveHelper***)(Util::FindPattern("client.dll", "8B 0D ? ? ? ? 8B 46 08 68") + 2);
	GameRules = *(IGameRules**)(Util::FindPattern("client.dll", "A1 ? ? ? ? 85 C0 0F 84 ? ? ? ? 80 B8 ? ? ? ? ? 0F 84 ? ? ? ? 0F 10 05") + 1);

	//BaseEntityList = *(CBaseEntityList**)(Util::FindPattern("client.dll", "BB ? ? ? ? 83 FF 01 0F 8C ? ? ? ? 3B F8") + 1);


	DEBUGPRINT(Client, ClientMode, Globals, ClientEntityList, Cvar, EngineClient, EngineTrace, InputSystem, Surface, VPanel, RenderView, ModelRender,
		MaterialSystem, ModelInfo, Physprops, DebugOverlay, StudioRender, Effects, EngineSound, MdlCache, Prediction, GameMovement, MoveHelper, Input, GameRules);

	return true;
}
