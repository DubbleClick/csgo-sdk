#include "../../csgo-sdk.h"

const char* IVPanel::GetName(int iIndex) {
	typedef const char*(__thiscall* OriginalFn)(void*, int);
	return Util::GetVFunc<OriginalFn>(this, 36)(this, iIndex);
}
