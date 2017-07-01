#include "../../csgo-sdk.h"

EndSceneFn oEndScene;
HRESULT __stdcall Hooks::EndScene(IDirect3DDevice9* pDevice) {
	return oEndScene(pDevice);
}
