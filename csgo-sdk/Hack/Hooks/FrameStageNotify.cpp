#include "../../csgo-sdk.h"

std::vector<const char*> smoke_materials = {
	//"effects/overlaysmoke",
	"particle/beam_smoke_01",
	"particle/particle_smokegrenade",
	"particle/particle_smokegrenade1",
	"particle/particle_smokegrenade2",
	"particle/particle_smokegrenade3",
	"particle/particle_smokegrenade_sc",
	"particle/smoke1/smoke1",
	"particle/smoke1/smoke1_ash",
	"particle/smoke1/smoke1_nearcull",
	"particle/smoke1/smoke1_nearcull2",
	"particle/smoke1/smoke1_snow",
	"particle/smokesprites_0001",
	"particle/smokestack",
	"particle/vistasmokev1/vistasmokev1",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	"particle/vistasmokev1/vistasmokev1_fire",
	"particle/vistasmokev1/vistasmokev1_nearcull",
	"particle/vistasmokev1/vistasmokev1_nearcull_fog",
	"particle/vistasmokev1/vistasmokev1_nearcull_nodepth",
	"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev4_emods_nocull",
	"particle/vistasmokev1/vistasmokev4_nearcull",
	"particle/vistasmokev1/vistasmokev4_nocull"
};

void ToggleSky() {
	static float oldclrs[2048][3];
	for (MaterialHandle_t i = Interfaces->MaterialSystem->FirstMaterial(); i != Interfaces->MaterialSystem->InvalidMaterial(); i = Interfaces->MaterialSystem->NextMaterial(i)) {
		IMaterial* pMaterial = Interfaces->MaterialSystem->GetMaterial(i);

		if (!pMaterial)
			continue;

		if (strstr(pMaterial->GetTextureGroupName(), "SkyBox textures")) {
			if (oldclrs[i][0] == 0.f && oldclrs[i][1] == 0.f && oldclrs[i][2] == 0.f)
				pMaterial->GetColorModulation(&oldclrs[i][0], &oldclrs[i][1], &oldclrs[i][2]);
			float col[3] = { 0, 0, 0 };
			if (!Config->Visual.Removals.Sky) {
				col[0] = oldclrs[i][0];
				col[1] = oldclrs[i][1];
				col[2] = oldclrs[i][2];
			}

			pMaterial->ColorModulate(col[0], col[1], col[2]);
			//Interfaces->ModelRender->ForcedMaterialOverride(pMaterial);
		}
	}
}

FrameStageNotifyFn oFrameStageNotify;
void __stdcall Hooks::FrameStageNotify(ClientFrameStage_t stage) {
	QAngle aim_punch_old;
	QAngle view_punch_old;

	QAngle* aim_punch = nullptr;
	QAngle* view_punch = nullptr;


	if (!(G::LocalPlayer && Interfaces->EngineClient->IsInGame()))
		return oFrameStageNotify(stage);

	if (stage == FRAME_RENDER_START) {
		if (Config->Visual.Removals.PostProcessing) {
			*(bool*)offsets->s_bOverridePostProcessingDisable = true;
		}
		else
			*(bool*)offsets->s_bOverridePostProcessingDisable = false;

		static bool bOldSkyEnable = false;
		if (bOldSkyEnable != Config->Visual.Removals.Sky) {
			ToggleSky();
			bOldSkyEnable = Config->Visual.Removals.Sky;
		}

		if (G::LocalPlayer->IsAlive()) {

			if (Config->Visual.Removals.Scope) {
				if (G::LocalPlayer->GetWeapon()->GetZoomLevel() != 0)
					*(bool*)((uintptr_t)G::LocalPlayer + offsets->m_bIsScoped) = false;
			}

			if (Config->Visual.Removals.Flash && G::LocalPlayer->GetFlashDuration() > 0)
				*(float*)((uintptr_t)G::LocalPlayer + offsets->m_flFlashDuration) = 0.f;

			if (Config->Visual.Removals.VisualRecoil) {
				aim_punch = (QAngle*)((uintptr_t)G::LocalPlayer + offsets->m_aimPunchAngle);
				view_punch = (QAngle*)((uintptr_t)G::LocalPlayer + offsets->m_viewPunchAngle);

				aim_punch_old = *aim_punch;
				view_punch_old = *view_punch;

				*aim_punch = QAngle(0.f, 0.f, 0.f);
				*view_punch = QAngle(0.f, 0.f, 0.f);
			}

			if (*(bool*)((uintptr_t)Interfaces->Input + 0xA5) || Config->Misc.ThirdPerson)   //Check for thirdperson
				*(QAngle*)((uintptr_t)G::LocalPlayer + offsets->m_fsnViewAngles) = G::VisualAngle;  //deadflag netvar + 4

			static bool oldEnable = false;
			if (oldEnable != Config->Visual.Removals.Smoke) {
				for (auto material_name : smoke_materials) {
					IMaterial* mat = Interfaces->MaterialSystem->FindMaterial(material_name, TEXTURE_GROUP_OTHER);
					mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, Config->Visual.Removals.Smoke ? true : false);
				}

				oldEnable = Config->Visual.Removals.Smoke;
			}
			if (Config->Visual.Removals.Smoke) {
				static int* smokecount = *(int**)(Util::FindPattern("client.dll", "A3 ? ? ? ? 57 8B CB") + 0x1);
				*smokecount = 0;
			}
		}
	}

	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
		for (int i = 1; i < 32; i++) {
			CBaseEntity* pEntity = Interfaces->ClientEntityList->GetClientEntity(i);

			if (!pEntity)
				continue;

			if (!Config->Ragebot.FriendlyFire && pEntity->GetTeam() == G::LocalPlayer->GetTeam())
				continue;

			if (Config->Ragebot.NoInterpolation)
				Lagcompensation->DisableInterpolation(pEntity);
		}
	}

	oFrameStageNotify(stage);

	if (stage == FRAME_NET_UPDATE_END) {
		for (int i = 1; i <= 32; i++) {
			CBaseEntity* pEntity = Interfaces->ClientEntityList->GetClientEntity(i);

			if (!pEntity)
				continue;

			if (!Config->Ragebot.FriendlyFire && pEntity->GetTeam() == G::LocalPlayer->GetTeam())
				continue;

			if (Config->Ragebot.ResolveMode)
				Resolver->Resolve(pEntity);

			//pEntity->UpdateClientSideAnimation();
		}
	}
}
