#include "../../csgo-sdk.h"

TraceRayFn oTraceRay;
void __fastcall Hooks::TraceRay(void* ths, void* ecx, Ray_t const& ray, unsigned int mask, CTraceFilter* filter, trace_t& trace) {
	oTraceRay(ths, ray, mask, filter, trace);
	if (G::UsingBulletHandler)
		trace.surface.flags |= SURF_SKY;
}
