#pragma once

using CreateMoveFn = bool(__stdcall*)(float, CUserCmd*);
extern CreateMoveFn oCreateMove;

using FrameStageNotifyFn = void(__stdcall*)(ClientFrameStage_t);
extern FrameStageNotifyFn oFrameStageNotify;

using PaintTraverseFn = void(__fastcall*)(void*, void*, unsigned int, bool, bool);
extern PaintTraverseFn oPaintTraverse;

using OverrideViewFn = void(__stdcall*)(CViewSetup*);
extern OverrideViewFn oOverrideView;

using InPredictionFn = bool(__stdcall*)();
extern InPredictionFn oInPrediction;

using EndSceneFn = long(__stdcall*)(IDirect3DDevice9* device);
extern EndSceneFn oEndScene;

using ResetFn = long(__stdcall*)(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pp);
extern ResetFn oReset;

typedef void(__thiscall* TraceRayFn)(void*, Ray_t const&, unsigned int, CTraceFilter*, trace_t&);
extern TraceRayFn oTraceRay;

typedef void(__fastcall* RunCommandFn)(void*, void*, CBaseEntity*, CUserCmd*, IMoveHelper*);
extern RunCommandFn oRunCommand;

typedef HRESULT(__stdcall* PresentFn)(IDirect3DDevice9*, RECT*, CONST RECT*, HWND, CONST RGNDATA*);
extern PresentFn oPresent;

extern WNDPROC oWndProc;
extern RecvVarProxyFn oLowerBodyYawTargetProxy;

namespace Hooks {
	extern bool __stdcall CreateMove(float flInputSampleTime, CUserCmd* cmd);
	extern void __stdcall FrameStageNotify(ClientFrameStage_t stage);
	extern void __fastcall PaintTraverse(void* ecx, void* edx, unsigned int panel, bool forceRepaint, bool allowForce);
	extern void __stdcall OverrideView(CViewSetup* vsView);
	extern void __fastcall TraceRay(void* thisptr, void* ecx, Ray_t const& ray, unsigned int mask, CTraceFilter* filter, trace_t& trace);
	extern void __fastcall RunCommand(void* ecx, void* edx, CBaseEntity* pEntity, CUserCmd* pCmd, IMoveHelper* moveHelper);

	extern HRESULT __stdcall EndScene(IDirect3DDevice9* pDevice);
	extern HRESULT __stdcall Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	extern HRESULT __stdcall Present(IDirect3DDevice9* pDevice, RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
	extern LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	extern void LowerBodyYawTargetProxy(const CRecvProxyData* pData, void* pStruct, void* pOut);

};
