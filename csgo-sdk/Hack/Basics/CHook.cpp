#include "../../csgo-sdk.h"

CHook::CHook(void* pInstance) {
	m_pInstance = nullptr;
	m_pCustomVMT = m_pOldVmt = nullptr;
	m_nSize = -1;

	if (pInstance != nullptr) {
		m_pInstance = (int**)pInstance;
		m_pOldVmt = *m_pInstance;

		MEMORY_BASIC_INFORMATION memInfo = { NULL };

		do {
			m_nSize++;
			VirtualQuery(reinterpret_cast<LPCVOID>(m_pOldVmt[m_nSize]), &memInfo, sizeof(memInfo));

		} while (memInfo.Protect == PAGE_EXECUTE_READ || memInfo.Protect == PAGE_EXECUTE_READWRITE);

		if (m_nSize >= 0) {
			m_pCustomVMT = new int[m_nSize];

			memcpy(m_pCustomVMT, m_pOldVmt, sizeof(int) * m_nSize);

			*m_pInstance = m_pCustomVMT;
		}
	}
}

CHook::~CHook() {
	if (m_pCustomVMT == nullptr || m_pOldVmt == nullptr || *m_pInstance == nullptr)
		return;

	if (reinterpret_cast<int>(*m_pInstance) == reinterpret_cast<int>(m_pCustomVMT)) {
		*m_pInstance = m_pOldVmt;

		delete m_pCustomVMT;

		m_pInstance = nullptr;
		m_pCustomVMT = m_pOldVmt = nullptr;
		m_nSize = -1;
	}
}

/*
template <typename T> T CHook::HookMethod(void* pHooked, int nIndex)
{
	m_pCustomVMT[nIndex] = (int)pHooked;
	return reinterpret_cast<T>(m_pOldVmt[nIndex]);
}*/

void CHook::Unhook() {
	*m_pInstance = m_pOldVmt;
}

void CHook::Rehook() {
	*m_pInstance = m_pCustomVMT;
}
