// Hook.h - Class to handle old/new vtables and writing them
class CHook
{
public:

	CHook(void* pInstance);
	~CHook();

	template <typename T>
	T HookMethod(void* pHooked, int nIndex) {
		m_pCustomVMT[nIndex] = (int)pHooked;
		return reinterpret_cast<T>(m_pOldVmt[nIndex]);
	};

	void Unhook();
	void Rehook();

	template<typename T>
	T GetMethod(int nIndex)
	{
		return reinterpret_cast<T>(m_pOldVmt[nIndex]);
	}

private:

	CHook(const CHook&) = delete;
	CHook& operator=(const CHook&) = delete;

	template<typename T>
	T GetMethod(const void* pInstance, int nIndex)
	{
		int* vmt = *(int**)pInstance;
		return reinterpret_cast<T>(vmt[nIndex]);
	}

	int** m_pInstance;
	int* m_pCustomVMT;
	int* m_pOldVmt;
	int m_nSize;
};