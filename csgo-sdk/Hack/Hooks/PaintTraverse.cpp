#include "../../csgo-sdk.h"

PaintTraverseFn oPaintTraverse;
void __fastcall Hooks::PaintTraverse(void* ecx, void* edx, unsigned int panel, bool forceRepaint, bool allowForce) {
	if (Config->Visual.Removals.Scope && G::LocalPlayer && G::LocalPlayer->GetWeapon()) {
		static unsigned int removePanel;
		if (!removePanel) {
			if (!strcmp("HudZoom", Interfaces->VPanel->GetName(panel)))
				removePanel = panel;
		}
		bool orgScoped = *(bool*)((uintptr_t)G::LocalPlayer + offsets->m_bIsScoped);
		if (panel == removePanel) {
			if (G::LocalPlayer && G::LocalPlayer->GetWeapon()->IsSniper() && G::LocalPlayer->GetWeapon()->GetZoomLevel() > 0) {
				*(bool*)((uintptr_t)G::LocalPlayer + offsets->m_bIsScoped) = false;
				return;
			}
		}
		else {
			*(bool*)((uintptr_t)G::LocalPlayer + offsets->m_bIsScoped) = orgScoped;
		}
	}
	oPaintTraverse(ecx, edx, panel, forceRepaint, allowForce);

	static unsigned int drawPanel;
	if (!drawPanel) {
		const char* panelname = Interfaces->VPanel->GetName(panel);

		if (panelname[0] == 'M' && panelname[2] == 't')
			drawPanel = panel;
	}

	if (panel != drawPanel)
		return;


	if (Config->Visual.Removals.Scope && (G::LocalPlayer && G::LocalPlayer->GetWeapon() && G::LocalPlayer->GetWeapon()->IsSniper() && G::LocalPlayer->GetWeapon()->GetZoomLevel() > 0)) {
		int screenX, screenY;
		Interfaces->EngineClient->GetScreenSize(screenX, screenY);
		D::DrawLine(screenX / 2, 0, screenX / 2, screenY, Color::Black());
		D::DrawLine(0, screenY / 2, screenX, screenY / 2, Color::Black());
	}

	Visuals->Tick();
}
