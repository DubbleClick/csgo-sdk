#include "../../csgo-sdk.h"

void CNetvarManager::Initialize() {
	m_vTables.clear();

	if (Interfaces->Client == nullptr)
		return;

	auto pClass = Interfaces->Client->GetAllClasses();

	if (pClass == nullptr)
		return;

	while (pClass) {
		auto pTable = pClass->m_pRecvTable;
		m_vTables.push_back(pTable);
		pClass = pClass->m_pNext;
	}
}

CNetvarManager::~CNetvarManager() {
	m_vTables.clear();
}

int CNetvarManager::GetOffset(const char* szTableName, const char* szPropName) {
	int offset = GetProp(szTableName, szPropName);
	if (offset)
		return offset;

	return 0;
}

int CNetvarManager::GetProp(const char* szTableName, const char* szPropName, RecvProp** pProp) {
	auto pTable = GetTable(szTableName);

	if (pTable == nullptr)
		return 0;

	int offset = GetProp(pTable, szPropName, pProp);
	if (!offset)
		return 0;

	return offset;
}

RecvVarProxyFn CNetvarManager::HookProp(const char* tablename, const char* propname, RecvVarProxyFn fun) {
	RecvProp* prop;
	GetProp(tablename, propname, &prop);
	auto oldfn = reinterpret_cast<RecvVarProxyFn>(prop->m_ProxyFn);

	prop->m_ProxyFn = fun;

	return oldfn;
}

int CNetvarManager::GetProp(RecvTable* pTable, const char* szPropName, RecvProp** pProp) {
	int extra = 0;

	for (int i = 0; i < pTable->m_nProps; i++) {
		auto recvProp = &pTable->m_pProps[i];
		auto pChild = recvProp->m_pDataTable;

		if (pChild && (pChild->m_nProps > 0)) {
			int temp = GetProp(pChild, szPropName, pProp);

			if (temp)
				extra += (recvProp->m_Offset + temp);
		}

		if (_stricmp(recvProp->m_pVarName, szPropName))
			continue;

		if (pProp != nullptr)
			*pProp = recvProp;

		return (recvProp->m_Offset + extra);
	}

	return extra;
}

RecvTable* CNetvarManager::GetTable(const char* szTableName) {
	if (m_vTables.empty())
		return nullptr;

	for (auto pTable : m_vTables) {
		if (pTable == nullptr)
			continue;

		if (!_stricmp(pTable->m_pNetTableName, szTableName))
			return pTable;
	}

	return nullptr;
}
