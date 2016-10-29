#pragma once

#include "IBaseClientDLL.h"
#include "IClientEntityList.h"
#include "IClientModeShared.h"
#include "ICvar.h"
#include "INetChannelInfo.h"
#include "IEngineClient.h"
#include "IEngineTrace.h"
#include "IGlobalVarsBase.h"
#include "IInputSystem.h"
#include "Interfaces.h"
#include "ISurface.h"
#include "IVPanel.h"
#include "IVModelInfo.h"
#include "IVModelRender.h"
#include "IMaterialSystem.h"
#include "IMaterial.h"
#include "IRenderView.h"
#include "IPrediction.h"
#include "VPhysics.h"
#include "IVDebugOverlay.h"
#include "IStudioRender.h"
#include "VEngineEffects.h"
#include "IMDLCache.h"
#include "IMoveHelper.h"
#include "GameMovement.h"
#include "IPrediction.h"
#include "CHLClient.h"

#define Interfaces CInterfaces::GetInstance()

class CInterfaces {
private:
	typedef void*(__cdecl* InstantiateInterfaceFn)();

	class InterfaceReg {
	public:
		InstantiateInterfaceFn m_CreateFn;
		const char* m_pName;
		InterfaceReg* m_pNext;
	};

	IMPLEMENT_SINGLETON_INITIALIZED(CInterfaces);

	bool Initialize();

public:
	template<typename T>
	T GetInterface(const char* module, const char* name) {
		while (!GetModuleHandleA(module))
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		auto ci_addr = reinterpret_cast<uintptr_t>(GetProcAddress(GetModuleHandleA(module), "CreateInterface"));
		auto jmp = ci_addr + 5 + *reinterpret_cast<uintptr_t*>(ci_addr + 5) + 4;
		auto interface_regs = **reinterpret_cast<InterfaceReg***>(jmp + 6);

		for (auto cur = interface_regs; cur != nullptr; cur = cur->m_pNext) {
			if (strstr(cur->m_pName, name) && (strlen(cur->m_pName) - strlen(name)) <= 3)
				return reinterpret_cast<T>(cur->m_CreateFn());
		}

		return 0;
	}

public:
	IBaseClientDll*			Client;
	IClientModeShared*		ClientMode;
	IClientEntityList*		ClientEntityList;
	ICVar*					Cvar;
	IInputSystem*			InputSystem;
	IEngineClient*			EngineClient;
	IEngineTrace*			EngineTrace;
	IGlobalVarsBase*		Globals;
	ISurface*				Surface;
	IVPanel*				VPanel;
	IVModelRender*			ModelRender;
	IVModelInfo*			ModelInfo;
	IMaterialSystem*		MaterialSystem;
	IMaterial*				Material;
	IVRenderView*			RenderView;
	IPrediction*			Prediction;
	IPhysicsSurfaceProps*	Physprops;
	IVDebugOverlay*			DebugOverlay;
	IStudioRender*			StudioRender;
	IVEfx*					Effects;
	IMDLCache*				MdlCache;
	IGameMovement*			GameMovement;
	IMoveHelper*			MoveHelper;
	IInput*					Input;
	IGameRules*				GameRules;
	CHLClient*				HLClient;
	CBaseEntityList*		BaseEntityList;
};
