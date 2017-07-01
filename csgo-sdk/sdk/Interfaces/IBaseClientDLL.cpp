#include "../../csgo-sdk.h"

ClientClass* IBaseClientDll::GetAllClasses() {
	typedef ClientClass*(__thiscall* OriginalFn)(PVOID);
	return Util::GetVFunc<OriginalFn>(this, 8)(this);
}
