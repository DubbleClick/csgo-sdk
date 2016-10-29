#pragma once

#define Netvars CNetvarManager::GetInstance()

class CNetvarManager {
	IMPLEMENT_SINGLETON_INITIALIZED(CNetvarManager);

public:
	int GetOffset(const char* szTableName, const char* szPropName);
	RecvVarProxyFn HookProp(const char* tablename, const char* propname, RecvVarProxyFn fun);
	CNetvarManager::~CNetvarManager();

private:
	void Initialize();
#undef GetProp
	int GetProp(const char* szTableName, const char* szPropName, RecvProp** pProp = NULL);
	int GetProp(RecvTable* pTable, const char* szPropName, RecvProp** pProp = NULL);
	RecvTable* GetTable(const char* szTableName);
	std::vector<RecvTable*> m_vTables = {};
};

