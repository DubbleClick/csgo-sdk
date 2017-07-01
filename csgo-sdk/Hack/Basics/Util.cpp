#include "../../csgo-sdk.h"

InitKeyValuesFn Util::InitKeyValuesEx;
LoadFromBufferFn Util::LoadFromBufferEx;

// learn_more
uintptr_t Util::FindPattern(std::string moduleName, std::string pattern) {
	const char* pat = pattern.c_str();
	uintptr_t firstMatch = 0;
	uintptr_t rangeStart = (uintptr_t)GetModuleHandleA(moduleName.c_str());
	MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	uintptr_t rangeEnd = rangeStart + miModInfo.SizeOfImage;
	for (uintptr_t pCur = rangeStart; pCur < rangeEnd; pCur++) {
		if (!*pat)
			return firstMatch;

		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat)) {
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;

			else
				pat += 2;    //one ?
		}
		else {
			pat = pattern.c_str();
			firstMatch = 0;
		}
	}
	return 0;
}

void Util::InitKeyValues(KeyValues* pKeyValues, const char* name) {
	Util::InitKeyValuesEx = reinterpret_cast<InitKeyValuesFn>(offsets->InitKeyValuesEx);
	Util::InitKeyValuesEx(pKeyValues, name);
}

void Util::LoadFromBuffer(KeyValues* pKeyValues, const char* resourceName, const char* pBuffer, void* pFileSystem, const char* pPathID, void* pfnEvaluateSymbolProc) {
	Util::LoadFromBufferEx = reinterpret_cast<LoadFromBufferFn>(offsets->LoadFromBufferEx);
	Util::LoadFromBufferEx(pKeyValues, resourceName, pBuffer, pFileSystem, pPathID, pfnEvaluateSymbolProc);
}

void Util::BuildEntityLists() {
	G::TeamList.clear();
	G::EnemyList.clear();

	for (int i = 0; i < 32; i++) {
		CBaseEntity* Entity = Interfaces->ClientEntityList->GetClientEntity(i);
		if (Entity && Entity->GetHealth() && Entity->IsPlayer())
			if (Entity->IsEnemy())
				G::EnemyList.push_back(Entity);
			else
				G::TeamList.push_back(Entity);
	}

	if (Config->Ragebot.FriendlyFire) {
		G::EnemyList.insert(
			G::EnemyList.end(),
			std::make_move_iterator(G::TeamList.begin()),
			std::make_move_iterator(G::TeamList.end())
		);
	}

	/*for (auto* b = const_cast<CEntInfo*>(Interfaces->BaseEntityList->FirstEntInfo()); b != nullptr; b = b->m_pNext) {
		if (b->m_pEntity != nullptr && !b->m_pEntity->IsDormant() && b->m_pEntity->IsAlive())
			if (b->m_pEntity->IsEnemy())
				G::EnemyList.push_back(b->m_pEntity);

			else
				G::TeamList.push_back(b->m_pEntity);
	}*/
}


void Util::DebugPrint(std::string text) {
#ifdef _DEBUG
	std::cout << text << std::endl;
#endif
}

void Util::ToggleThirdPerson(bool on, bool kill /*= false*/) {
	static SpoofedConvar* g_pSvCheatsVar;
	if (kill) {
		if (g_pSvCheatsVar)
			delete g_pSvCheatsVar;
		return;
	}
	static bool oldon = false;
	if (oldon == on) {
		return;
	}
	else
		oldon = on;
	if (on) {
		if (!g_pSvCheatsVar) {
			g_pSvCheatsVar = new SpoofedConvar("sv_cheats");
			g_pSvCheatsVar->SetBool(true);
			g_pSvCheatsVar->SetInt(1);
		}
		Interfaces->EngineClient->ClientCmd_Unrestricted("thirdperson");
	}
	else Interfaces->EngineClient->ClientCmd_Unrestricted("firstperson");
}


