#pragma once

#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

using InitKeyValuesFn = void(__thiscall*)(void* thisptr, const char* name);
using LoadFromBufferFn = void(__thiscall*)(void* thisptr, const char* resourceName, const char* pBuffer, void* pFileSystem, const char* pPathID, void* pfnEvaluateSymbolProc);

namespace Util
{
	extern uintptr_t FindPattern(std::string moduleName, std::string pattern);

	template <typename T, int i = -1>
	constexpr T GetVFunc(void *vTable, int index = 0) noexcept {
		return (*reinterpret_cast<T**>(vTable))[(i >= 0 ? i : index)];
	}

	extern InitKeyValuesFn InitKeyValuesEx;
	extern LoadFromBufferFn LoadFromBufferEx;
	extern void InitKeyValues(KeyValues* pKeyValues, const char* name);
	extern void LoadFromBuffer(KeyValues* pKeyValues, const char* resourceName, const char* pBuffer, void* pFileSystem = nullptr, const char* pPathID = NULL, void* pfnEvaluateSymbolProc = nullptr);
	extern void BuildEntityLists();
	extern void DebugPrint(std::string text);
	extern void ToggleThirdPerson(bool on, bool kill = false);
}
