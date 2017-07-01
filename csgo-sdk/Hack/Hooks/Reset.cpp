#include "../../csgo-sdk.h"

ResetFn oReset;
long __stdcall Hooks::Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {

	Drawmanager->OnLostDevice();
	ImGui_ImplDX9_InvalidateDeviceObjects();

	auto res = oReset(pDevice, pPresentationParameters);
	
	Drawmanager->OnResetDevice();
	ImGui_ImplDX9_CreateDeviceObjects();

	return res;
}
