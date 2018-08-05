#include "csgo-sdk.h"

std::unique_ptr<CHook> VPanel;
std::unique_ptr<CHook> ClientMode;
std::unique_ptr<CHook> Client;
std::unique_ptr<CHook> ModelRender;
std::unique_ptr<CHook> Surface;
std::unique_ptr<CHook> D3D9;
std::unique_ptr<CHook> MaterialSystem;
std::unique_ptr<CHook> EngineTrace;
std::unique_ptr<CHook> EngineClient;
std::unique_ptr<CHook> Prediction;

void SetupExports() {
	Math::RandomSeed = (RandomSeedFn)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomSeed");
	Math::RandomFloat = (RandomFloatFn)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomFloat");
	Math::RandomFloatExp = (RandomFloatExpFn)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomFloatExp");
	Math::RandomInt = (RandomIntFn)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomInt");
	Math::RandomGaussianFloat = (RandomFloatFn)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomGaussianFloat");
}

void SetupHooks() {
	oLowerBodyYawTargetProxy = Netvars->HookProp("DT_BasePlayer", "m_flLowerBodyYawTarget", Hooks::LowerBodyYawTargetProxy);

	VPanel = std::make_unique<CHook>(Interfaces->VPanel);
	Client = std::make_unique<CHook>(Interfaces->Client);
	ClientMode = std::make_unique<CHook>(Interfaces->ClientMode);
	ModelRender = std::make_unique<CHook>(Interfaces->ModelRender);
	Surface = std::make_unique<CHook>(Interfaces->Surface);
	D3D9 = std::make_unique<CHook>(reinterpret_cast<void*>(offsets->d3d9Device));
	MaterialSystem = std::make_unique<CHook>(Interfaces->MaterialSystem);
	EngineClient = std::make_unique<CHook>(Interfaces->EngineClient);
	EngineTrace = std::make_unique<CHook>(Interfaces->EngineTrace);
	Prediction = std::make_unique<CHook>(Interfaces->Prediction);

	oWndProc = (WNDPROC)SetWindowLongPtr(G::Window, GWL_WNDPROC, (LONG_PTR)Hooks::WndProc);
	oPaintTraverse = VPanel->HookMethod<PaintTraverseFn>(Hooks::PaintTraverse, 41);
	oCreateMove = ClientMode->HookMethod<CreateMoveFn>(Hooks::CreateMove, 24);
	oOverrideView = ClientMode->HookMethod<OverrideViewFn>(Hooks::OverrideView, 18);
	oFrameStageNotify = Client->HookMethod<FrameStageNotifyFn>(Hooks::FrameStageNotify, 36);
	oRunCommand = Prediction->HookMethod<RunCommandFn>(Hooks::RunCommand, 19);
	oReset = D3D9->HookMethod<ResetFn>(Hooks::Reset, 16);
	oPresent = D3D9->HookMethod<PresentFn>(Hooks::Present, 17);
	oTraceRay = EngineTrace->HookMethod<TraceRayFn>(Hooks::TraceRay, 5);
}

void Setup() {
#ifdef _DEBUG
	AllocConsole();
	FILE* pOut;
	freopen_s(&pOut, "CONOUT$", "w", stdout);
	freopen_s(&pOut, "CONOUT$", "w", stderr);
	fclose(pOut);
#endif

	SetupExports();
	Interfaces;
	Netvars;
	offsets;
	Drawmanager;
	Visuals;
	EnginePrediction;
	Lagcompensation;
	Resolver;
	ConfigLoader;
	D::SetupFonts();
	SetupHooks();
}

void Terminate() {
	Util::ToggleThirdPerson(false, true);
	if (oLowerBodyYawTargetProxy)
		Netvars->HookProp("DT_BasePlayer", "m_flLowerBodyYawTarget", oLowerBodyYawTargetProxy);
	VPanel->Unhook();
	Client->Unhook();
	ClientMode->Unhook();
	ModelRender->Unhook();
	Surface->Unhook();
	D3D9->Unhook();
	MaterialSystem->Unhook();
	EngineClient->Unhook();
	EngineTrace->Unhook();
	Prediction->Unhook();
	SetWindowLongPtr(G::Window, GWL_WNDPROC, (LONG_PTR)oWndProc);

#ifdef _DEBUG
	FreeConsole();
#endif
}

DWORD __stdcall CheatThread() {
	Setup();

	while (!(GetAsyncKeyState(VK_END) & 0x8000)) {
		Sleep(10);
	}

	Terminate();
	FreeLibraryAndExitThread(G::Dll, NULL);
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		while (!(G::Window = FindWindow(L"Valve001", NULL))) Sleep(200);
		G::Dll = (HMODULE)hModule;
		GetModuleFileNameA(G::Dll, G::DllPath, sizeof(G::DllPath));
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CheatThread, 0, 0, 0);
		break;
	case DLL_THREAD_ATTACH:  break;
	case DLL_THREAD_DETACH:  break;
	case DLL_PROCESS_DETACH:  break;
	}

	return TRUE;
}
