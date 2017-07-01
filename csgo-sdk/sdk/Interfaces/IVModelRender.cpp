#include "../../csgo-sdk.h"

void IVModelRender::ForcedMaterialOverride(IMaterial *mat) {
	typedef void(__thiscall *OrigFn)(void *, IMaterial *, int, int);
	Util::GetVFunc<OrigFn>(this, 1)(this, mat, 0, 0);
}

void IVModelRender::DrawModelExecute(void* ctx, void *state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld) {
	typedef void(__thiscall* OriginalFn)(void*, void* ctx, void *state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);
	return Util::GetVFunc<OriginalFn>(this, 21)(this, ctx, state, pInfo, pCustomBoneToWorld);
}
