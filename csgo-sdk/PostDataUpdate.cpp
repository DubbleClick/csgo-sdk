#include "Refund.h"

PostDataUpdateFn oPostDataUpdate;
void __stdcall Hooks::PostDataUpdate(CTEFireBullets* thisptr, DataUpdateType_t updateType) {
	oPostDataUpdate(thisptr, updateType);
}