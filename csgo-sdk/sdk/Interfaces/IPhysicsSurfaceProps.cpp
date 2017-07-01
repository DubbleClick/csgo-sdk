#include "../../csgo-sdk.h"

surfacedata_t* IPhysicsSurfaceProps::GetSurfaceData(int surfaceDataIndex) {
	typedef surfacedata_t*(__thiscall* OriginalFn)(void*, int);
	return Util::GetVFunc<OriginalFn>(this, 5)(this, surfaceDataIndex);
}
