#pragma once

#include "../sdk/Interfaces/IBaseClientDLL.h"
#include "../sdk/Interfaces/IClientEntityList.h"
#include "../sdk/Interfaces/IClientModeShared.h"
#include "../sdk/Interfaces/ICvar.h"
#include "../sdk/Interfaces/INetChannelInfo.h"
#include "../sdk/Interfaces/IEngineClient.h"
#include "../sdk/Interfaces/IEngineTrace.h"
#include "../sdk/Interfaces/IGlobalVarsBase.h"
#include "../sdk/Interfaces/IInputSystem.h"
#include "../sdk/Interfaces/ISurface.h"
#include "../sdk/Interfaces/IVPanel.h"
#include "../sdk/Interfaces/IVModelInfo.h"
#include "../sdk/Interfaces/IVModelRender.h"
#include "../sdk/Interfaces/IMaterialSystem.h"
#include "../sdk/Interfaces/IMaterial.h"
#include "../sdk/Interfaces/IRenderView.h"
#include "../sdk/Interfaces/IPrediction.h"
#include "../sdk/Interfaces/VPhysics.h"
#include "../sdk/Interfaces/IVDebugOverlay.h"
#include "../sdk/Interfaces/IStudioRender.h"
#include "../sdk/Interfaces/VEngineEffects.h"
#include "../sdk/Interfaces/IMDLCache.h"
#include "../sdk/Interfaces/IMoveHelper.h"
#include "../sdk/Interfaces/GameMovement.h"
#include "../sdk/Interfaces/IPrediction.h"
#include "../sdk/Interfaces/CHLClient.h"

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
