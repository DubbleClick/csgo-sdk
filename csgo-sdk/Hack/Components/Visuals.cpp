#include "../../csgo-sdk.h"

void CVisuals::Tick() {
	if (!Config->Visual.Enabled || !G::LocalPlayer || !G::LocalPlayer->GetWeapon() || !Interfaces->EngineClient->IsInGame())
		return;

	DrawGlow();
	if (Config->Visual.GrenadePredictionEnabled) {
		GrenadePrediction->PaintGrenadeTrajectory();
	}

	for (int i = 0; i < Interfaces->ClientEntityList->GetHighestEntityIndex(); i++) {
		PlayerESP(i);
		WorldESP(i);
	}


	int w, h;
	Interfaces->EngineClient->GetScreenSize(w, h);

	if (Config->Visual.LBYDifference) {
		DrawLBYDelta(w, h);
	}

#ifdef _DEBUG
	D::DrawString(F::ESP, w / 2, 10, Color::Red(), FONT_CENTER, G::DebugString.c_str());
#endif
}

bool CVisuals::ScreenTransform(const Vector& point, Vector& screen) {
	float w;
	const VMatrix& worldToScreen = Interfaces->EngineClient->WorldToScreenMatrix();

	screen.x = worldToScreen[0][0] * point[0] + worldToScreen[0][1] * point[1] + worldToScreen[0][2] * point[2] + worldToScreen[0][3];
	screen.y = worldToScreen[1][0] * point[0] + worldToScreen[1][1] * point[1] + worldToScreen[1][2] * point[2] + worldToScreen[1][3];
	w = worldToScreen[3][0] * point[0] + worldToScreen[3][1] * point[1] + worldToScreen[3][2] * point[2] + worldToScreen[3][3];
	screen.z = 0.0f;

	bool behind = false;

	if (w < 0.001f) {
		behind = true;
		screen.x *= 100000;
		screen.y *= 100000;
	}
	else {
		behind = false;
		float invw = 1.0f / w;
		screen.x *= invw;
		screen.y *= invw;
	}

	return behind;
}

bool CVisuals::WorldToScreen(const Vector &origin, Vector &screen) {
	if (!ScreenTransform(origin, screen)) {
		int ScreenWidth, ScreenHeight;
		Interfaces->EngineClient->GetScreenSize(ScreenWidth, ScreenHeight);
		float x = float(ScreenWidth) / 2;
		float y = (float)ScreenHeight / 2;
		x += 0.5f * screen.x * ScreenWidth + 0.5f;
		y -= 0.5f * screen.y * ScreenHeight + 0.5f;
		screen.x = x;
		screen.y = y;
		return true;
	}

	return false;
}

void CVisuals::DrawGlow() {
	static CGlowObjectManager* GlowObjectManager = (CGlowObjectManager*)offsets->GlowManager;

	for (int i = 0; i < GlowObjectManager->size; i++) {
		CGlowObjectManager::GlowObjectDefinition_t* glowEntity = &GlowObjectManager->m_GlowObjectDefinitions[i];
		CBaseEntity* Entity = glowEntity->GetEntity();

		if (glowEntity->IsEmpty() || !Entity)
			continue;

		switch (Entity->GetClientClass()->m_ClassID) {

		case 1:
			if (Config->Visual.Filter.Weapons)
				glowEntity->Set(Color(Config->Visual.Glow.WeaponColor));

			break;

		case 29:
			if (Config->Visual.Filter.C4)
				glowEntity->Set(Color(Config->Visual.Glow.C4Color));

			break;

		case 35:
			if (Config->Visual.Glow.Enabled) {
				if (!Config->Visual.Filter.Teammates && !Entity->IsEnemy())
					break;

				if (!Config->Visual.Filter.Enemies && Entity->IsEnemy())
					break;

				if (Entity->IsVisible(HEAD_0))
					glowEntity->Set(Entity->IsEnemy() ? Color(Config->Visual.Glow.EnemyColor) : Color(Config->Visual.Glow.TeamColor));
				else
					glowEntity->Set(Entity->IsEnemy() ? Color(Config->Visual.Glow.EnemyColorWall) : Color(Config->Visual.Glow.TeamColorWall));

			}
			break;

		case 39:
			if (Config->Visual.Filter.Weapons)
				glowEntity->Set(Color(Config->Visual.Glow.WeaponColor));

			break;

		case 41:
			if (Config->Visual.Filter.Decoy)
				glowEntity->Set(Color::Purple());

			break;

		case 105:
			if (Config->Visual.Filter.C4)
				glowEntity->Set(Color(Config->Visual.Glow.C4Color));
			break;

		default:
			if (Config->Visual.Filter.Weapons) {
				if (strstr(Entity->GetClientClass()->m_pNetworkName, "CWeapon"))
					glowEntity->Set(Color(Config->Visual.Glow.WeaponColor));
			}
			break;
		}
	}
}

void CVisuals::PlayerESP(int index) {
	CBaseEntity* Entity = Interfaces->ClientEntityList->GetClientEntity(index);

	if (EntityIsInvalid(Entity))
		return;

	if (!Entity->IsPlayer())
		return;

	if (Config->Visual.Filter.Decoy && !Entity->IsEnemy())
		return;

	if (!G::LocalPlayer->IsAlive()) {
		if (Interfaces->ClientEntityList->GetClientEntityFromHandle(G::LocalPlayer->GetObserverTarget()) == Entity)
			return;
	}

	if (!Config->Visual.Filter.Teammates && !Entity->IsEnemy())
		return;

	if (!Config->Visual.Filter.Enemies && Entity->IsEnemy())
		return;

	if (Config->Visual.Filter.VisibleOnly && !Entity->IsVisible(HEAD_0))
		return;

	Vector max = Entity->GetCollideable()->OBBMaxs();
	Vector pos, pos3D;
	Vector top, top3D;
	pos3D = Entity->GetOrigin();
	top3D = pos3D + Vector(0, 0, max.z);
	if (!WorldToScreen(pos3D, pos) || !WorldToScreen(top3D, top))
		return;

	float height = (pos.y - top.y);
	float width = height / 4.f;

	if (Config->Visual.Box)
		if (Entity->IsVisible(HEAD_0))
			PlayerBox(top.x, top.y, width, height, Entity->IsEnemy() ? Color(Config->Visual.EnemyColor) : Color(Config->Visual.TeamColor));
		else
			PlayerBox(top.x, top.y, width, height, Entity->IsEnemy() ? Color(Config->Visual.EnemyColorWall) : Color(Config->Visual.TeamColorWall));

	if (Config->Visual.Corners)
		if (Entity->IsVisible(HEAD_0))
			PlayerCorners(top.x, top.y, width, height, Entity->IsEnemy() ? Color(Config->Visual.EnemyColor) : Color(Config->Visual.TeamColor));
		else
			PlayerCorners(top.x, top.y, width, height, Entity->IsEnemy() ? Color(Config->Visual.EnemyColorWall) : Color(Config->Visual.TeamColorWall));

	if (Config->Visual.Hitboxes)
		Hitboxes(Entity);

	if (Config->Visual.Skeleton)
		Skeleton(Entity, Color::White());

	if (Config->Visual.Info.Name) {
		player_info_t pinfo;
		Interfaces->EngineClient->GetPlayerInfo(index, &pinfo);
		D::DrawString(F::ESP, static_cast<int>(top.x), static_cast<int>(top.y) - 6, Color::White(), FONT_CENTER, "%s", pinfo.name);
	}

	if (Config->Visual.Info.Healthtext)
		Healthtext(pos, top, Entity->GetHealth());

	if (Config->Visual.Info.Armor)
		Armortext(pos, top, Entity->GetArmor());

	int bottom = 0;

	if (Config->Visual.Info.Weapon)
		D::DrawString(F::ESP, static_cast<int>(top.x), static_cast<int>(top.y) + static_cast<int>(height) + 8 + (10 * bottom++), Color::White(), FONT_CENTER, "%s", Entity->GetWeapon()->GetWeaponName().c_str());

	if (Config->Visual.Dlights) {
		int rr = 255;
		int gg = 255;
		int bb = 255;
		int aa = 255;


		ColorRGBExp32 clrLight;
		clrLight.r = static_cast<unsigned char>(Entity->IsEnemy() ? Config->Visual.Glow.EnemyColor[0] * 255 : Config->Visual.Glow.TeamColor[0] * 255);
		clrLight.g = static_cast<unsigned char>(Entity->IsEnemy() ? Config->Visual.Glow.EnemyColor[1] * 255 : Config->Visual.Glow.TeamColor[1] * 255);
		clrLight.b = static_cast<unsigned char>(Entity->IsEnemy() ? Config->Visual.Glow.EnemyColor[2] * 255 : Config->Visual.Glow.TeamColor[2] * 255);

		clrLight.exponent = static_cast<signed char>(10.f);

		dlight_t* pElight = Interfaces->Effects->CL_AllocElight(Entity->entindex());
		pElight->origin = Entity->GetBonePosition(8) + Vector(0.0f, 0.0f, 35.0f);
		pElight->radius = 100.0f;
		pElight->color = clrLight;
		pElight->die = Interfaces->Globals->curtime + 0.05f;
		pElight->decay = pElight->radius / 5.0f;
		pElight->key = Entity->entindex();

		if (Entity->IsEnemy()) clrLight.g = 0; else clrLight.g = 0;

		dlight_t* pDlight = Interfaces->Effects->CL_AllocDlight(Entity->entindex());
		pDlight->origin = Entity->GetOrigin();
		pDlight->radius = 50.0f;
		pDlight->color = clrLight;
		pDlight->die = Interfaces->Globals->curtime + 0.05f;
		pDlight->decay = pDlight->radius / 5.0f;
		pDlight->key = Entity->entindex();
	}
}

void CVisuals::WorldESP(int index) {
	CBaseEntity* Entity = Interfaces->ClientEntityList->GetClientEntity(index);

	if (!Entity)
		return;

	Vector pos, pos3D;
	pos3D = Entity->GetOrigin();

	if (pos3D.Empty())
		return;

	if (!WorldToScreen(pos3D, pos))
		return;

	auto owner = Entity->GetOwnerEntity();

	if (!owner.IsValid()) {
		if (Config->Visual.Filter.Weapons) {
			if (strstr(Entity->GetClientClass()->m_pNetworkName, "CWeapon")) {
				D::DrawString(F::ESP, static_cast<int>(pos.x), static_cast<int>(pos.y), Color::White(), FONT_RIGHT, std::string(Entity->GetClientClass()->m_pNetworkName).substr(7).c_str());
			}

			if (Entity->GetClientClass()->m_ClassID == 1) {
				D::DrawString(F::ESP, static_cast<int>(pos.x), static_cast<int>(pos.y), Color::White(), FONT_RIGHT, "AK-47");
			}

			if (Entity->GetClientClass()->m_ClassID == 39) {
				D::DrawString(F::ESP, static_cast<int>(pos.x), static_cast<int>(pos.y), Color::White(), FONT_RIGHT, "Deagle");
			}
		}

		if (Entity->GetClientClass()->m_ClassID == 29 && Config->Visual.Filter.C4)
			D::DrawString(F::ESP, static_cast<int>(pos.x), static_cast<int>(pos.y), Color::LightBlue(), FONT_RIGHT, "C4");
	}

	if (Entity->GetClientClass()->m_ClassID == 41 && Config->Visual.Filter.Decoy) {
		CBaseEntity* OwnerEntity = Interfaces->ClientEntityList->GetClientEntity(owner.ToInt() & 0xFFF);
	}

	if (strstr(Entity->GetClientClass()->m_pNetworkName, "CPlantedC4") && Config->Visual.Filter.C4)
		D::DrawString(F::ESP, static_cast<int>(pos.x), static_cast<int>(pos.y), Color::Red(), FONT_CENTER, "%.1f", Entity->GetBombTimer());
}

void CVisuals::DrawLBYDelta(int w, int h) {
	if (!(Config->Ragebot.AntiAim.YawFake || Config->Ragebot.AntiAim.YawDynamic || Config->Ragebot.AntiAim.Yaw))
		return;
	if (!G::SendPacket) {
		auto dif = static_cast<int>(abs(Math::ClampYaw(G::LocalPlayer->GetEyeAngles().y - (G::LocalPlayer->IsMovingOnGround() ? G::LocalPlayer->GetEyeAngles().y : G::LocalPlayer->GetLowerBodyYawTarget()))));
		auto msg = std::wstring(L"Dif: ") + std::wstring(std::to_wstring(dif));
		std::string msga = std::string(msg.begin(), msg.end());
		int wid, heig;
		Interfaces->Surface->GetTextSize(F::ESP, msg.c_str(), wid, heig);
		D::DrawString(F::ESP, w - wid - 2, heig / 2 + 2, dif > 35 ? Color::Green() : Color::Red(), 0, msga.c_str());
	}
	else if (!G::InAntiAim) {
		auto dif = static_cast<int>(abs(Math::ClampYaw(G::LocalPlayer->GetEyeAngles().y - (G::LocalPlayer->IsMovingOnGround() ? G::LocalPlayer->GetEyeAngles().y : G::LocalPlayer->GetLowerBodyYawTarget()))));
		auto msg = std::wstring(L"Dif: ") + std::wstring(std::to_wstring(dif));
		std::string msga = std::string(msg.begin(), msg.end());
		int wid, heig;
		Interfaces->Surface->GetTextSize(F::ESP, msg.c_str(), wid, heig);
		D::DrawString(F::ESP, w - wid - 2, heig / 2 + 2, dif > 35 ? Color::Green() : Color::Red(), 0, msga.c_str());
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


bool CVisuals::EntityIsInvalid(CBaseEntity* Entity) {
	if (!Entity)
		return true;
	if (Entity == G::LocalPlayer)
		return true;
	if (Entity->GetHealth() <= 0)
		return true;
	if (Entity->GetDormant())
		return true;

	if (Config->Visual.Filter.VisibleOnly && !Entity->IsVisible(Config->Ragebot.Hitbox))
		return true;


	return false;
}

void CVisuals::PlayerBox(float x, float y, float w, float h, Color clr) {
	Interfaces->Surface->DrawSetColor(clr);
	Interfaces->Surface->DrawOutlinedRect(x - w, y, x + w, y + h);

	Interfaces->Surface->DrawSetColor(Color::Black());
	Interfaces->Surface->DrawOutlinedRect(x - w - 1, y - 1, x + w + 1, y + h + 1);
	Interfaces->Surface->DrawOutlinedRect(x - w + 1, y + 1, x + w - 1, y + h - 1);
}

void CVisuals::PlayerCorners(float X, float Y, float W, float H, Color clr) {
	X -= W;
	W *= 2.f;
	float lineW = (W / 5.f);
	float lineH = (H / 6.f);
	float lineT = 1;

	//outline
	D::DrawLine(static_cast<int>(X - lineT), static_cast<int>(Y - lineT), static_cast<int>(X + lineW), static_cast<int>(Y - lineT), Color::Black()); //top left
	D::DrawLine(static_cast<int>(X - lineT), static_cast<int>(Y - lineT), static_cast<int>(X - lineT), static_cast<int>(Y + lineH), Color::Black());
	D::DrawLine(static_cast<int>(X - lineT), static_cast<int>(Y + H - lineH), static_cast<int>(X - lineT), static_cast<int>(Y + H + lineT), Color::Black()); //bot left
	D::DrawLine(static_cast<int>(X - lineT), static_cast<int>(Y + H + lineT), static_cast<int>(X + lineW), static_cast<int>(Y + H + lineT), Color::Black());
	D::DrawLine(static_cast<int>(X + W - lineW), static_cast<int>(Y - lineT), static_cast<int>(X + W + lineT), static_cast<int>(Y - lineT), Color::Black()); // top right
	D::DrawLine(static_cast<int>(X + W + lineT), static_cast<int>(Y - lineT), static_cast<int>(X + W + lineT), static_cast<int>(Y + lineH), Color::Black());
	D::DrawLine(static_cast<int>(X + W + lineT), static_cast<int>(Y + H - lineH), static_cast<int>(X + W + lineT), static_cast<int>(Y + H + lineT), Color::Black()); // bot right
	D::DrawLine(static_cast<int>(X + W - lineW), static_cast<int>(Y + H + lineT), static_cast<int>(X + W + lineT), static_cast<int>(Y + H + lineT), Color::Black());

	//inline
	D::DrawLine(static_cast<int>(X), static_cast<int>(Y), static_cast<int>(X), static_cast<int>(Y + lineH), clr);//top left
	D::DrawLine(static_cast<int>(X), static_cast<int>(Y), static_cast<int>(X + lineW), static_cast<int>(Y), clr);
	D::DrawLine(static_cast<int>(X + W - lineW), static_cast<int>(Y), static_cast<int>(X + W), static_cast<int>(Y), clr); //top right
	D::DrawLine(static_cast<int>(X + W), static_cast<int>(Y), static_cast<int>(X + W), static_cast<int>(Y + lineH), clr);
	D::DrawLine(static_cast<int>(X), static_cast<int>(Y + H - lineH), static_cast<int>(X), static_cast<int>(Y + H), clr); //bot left
	D::DrawLine(static_cast<int>(X), static_cast<int>(Y + H), static_cast<int>(X + lineW), static_cast<int>(Y + H), clr);
	D::DrawLine(static_cast<int>(X + W - lineW), static_cast<int>(Y + H), static_cast<int>(X + W), static_cast<int>(Y + H), clr);//bot right
	D::DrawLine(static_cast<int>(X + W), static_cast<int>(Y + H - lineH), static_cast<int>(X + W), static_cast<int>(Y + H), clr);
}

void CVisuals::HealthBar(Vector bot, Vector top, float health) {
	float h = (bot.y - top.y);
	float offset = (h / 4.f) + 5;
	float w = h / 64.f;

	unsigned int hp = static_cast<unsigned int>(h - (unsigned int)((h * health) / 100)); // Percentage

	int Red = static_cast<int>(255 - (health*2.55f));
	int Green = static_cast<int>(health*2.55f);

	D::DrawRect(static_cast<int>(top.x - offset) - 1, static_cast<int>(top.y - 1), 3, static_cast<int>(h + 2), Color::Black());

	D::DrawLine(static_cast<int>(top.x - offset), static_cast<int>(top.y + hp), static_cast<int>(top.x - offset), static_cast<int>(top.y + h), Color(Red, Green, 0, 255));
}

void CVisuals::Healthtext(Vector bot, Vector top, int health) {
	D::DrawString(F::ESP, static_cast<int>(bot.x), static_cast<int>(bot.y) - 6, Color::White(), FONT_CENTER, "%d", health);
}

void CVisuals::Armortext(Vector bot, Vector top, int armor) {
	D::DrawString(F::ESP, static_cast<int>(bot.x), static_cast<int>(bot.y) - 18, Color::Yellow(), FONT_CENTER, "%d", armor);
}

void CVisuals::Skeleton(CBaseEntity *Entity, Color color) {
	studiohdr_t* pStudioModel = Interfaces->ModelInfo->GetStudioModel(Entity->GetModel());
	if (pStudioModel) {
		static matrix3x4_t pBoneToWorldOut[128];
		if (Entity->SetupBones(pBoneToWorldOut, 128, 256, Interfaces->Globals->curtime)) {
			for (int i = 0; i < pStudioModel->numbones; i++) {
				mstudiobone_t* pBone = pStudioModel->pBone(i);
				if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
					continue;

				Vector vBonePos1;
				if (!WorldToScreen(Vector(pBoneToWorldOut[i][0][3], pBoneToWorldOut[i][1][3], pBoneToWorldOut[i][2][3]), vBonePos1))
					continue;

				Vector vBonePos2;
				if (!WorldToScreen(Vector(pBoneToWorldOut[pBone->parent][0][3], pBoneToWorldOut[pBone->parent][1][3], pBoneToWorldOut[pBone->parent][2][3]), vBonePos2))
					continue;

				D::DrawLine((int)vBonePos1.x, (int)vBonePos1.y, (int)vBonePos2.x, (int)vBonePos2.y, color);
			}
		}
	}
}

void CVisuals::DrawHitbox(matrix3x4a_t* matrix, Vector bbmin, Vector bbmax, int bone, Color color) {
	Vector points[] = {
		Vector(bbmin.x, bbmin.y, bbmin.z),
		Vector(bbmin.x, bbmax.y, bbmin.z),
		Vector(bbmax.x, bbmax.y, bbmin.z),
		Vector(bbmax.x, bbmin.y, bbmin.z),
		Vector(bbmax.x, bbmax.y, bbmax.z),
		Vector(bbmin.x, bbmax.y, bbmax.z),
		Vector(bbmin.x, bbmin.y, bbmax.z),
		Vector(bbmax.x, bbmin.y, bbmax.z)
	};

	Vector pointsTransformed[8];

	for (int index = 0; index < 8; ++index) {
		if (index != 0)
			points[index] = ((((points[index] + points[0]) * .5f) + points[index]) * .5f);

		pointsTransformed[index] = Math::VectorTransform(points[index], matrix[bone]);
	}

	Draw3DBox(pointsTransformed, color);
}

void CVisuals::Hitboxes(CBaseEntity *Entity) {
	matrix3x4a_t matrix[MAXSTUDIOBONES];

	if (!Entity->SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, Interfaces->EngineClient->GetLastTimeStamp()))
		return;

	studiohdr_t* pStudioModel = Interfaces->ModelInfo->GetStudioModel(Entity->GetModel());
	if (!pStudioModel)
		return;

	mstudiohitboxset_t* set = pStudioModel->pHitboxSet(Entity->GetHitboxSet());
	if (!set)
		return;

	for (int i = 0; i < set->numhitboxes; i++) {
		mstudiobbox_t* hitbox = set->pHitbox(i);
		if (!hitbox)
			return;

		int bone = hitbox->bone;

		Vector vMaxUntransformed = hitbox->bbmax;
		Vector vMinUntransformed = hitbox->bbmin;

		if (hitbox->radius != -1.f) {
			vMinUntransformed -= Vector(hitbox->radius, hitbox->radius, hitbox->radius);
			vMaxUntransformed += Vector(hitbox->radius, hitbox->radius, hitbox->radius);
		}

		Color color = (Entity->IsEnemy() ? Color(Config->Visual.EnemyColor) : Color(Config->Visual.TeamColor));
		if (Config->Visual.HitboxesWallcheck) {
			Ray_t ray;
			trace_t tr;
			Vector hitboxposition(matrix[hitbox->bone].m_flMatVal[0][3], matrix[hitbox->bone].m_flMatVal[1][3], matrix[hitbox->bone].m_flMatVal[2][3]);
			ray.Init(G::LocalPlayer->GetEyePosition(), hitboxposition);

			CTraceFilterSkipTwoEntities filter(G::LocalPlayer, Entity, COLLISION_GROUP_NONE);

			Interfaces->EngineTrace->TraceRay(ray, MASK_VISIBLE, &filter, &tr);

			if (tr.fraction != 1.0f) color = (Entity->IsEnemy() ? Color(Config->Visual.EnemyColorWall) : Color(Config->Visual.TeamColorWall));
		}

		DrawHitbox(matrix, vMinUntransformed, vMaxUntransformed, hitbox->bone, color);
	}
}

void CVisuals::Draw3DBox(Vector* boxVectors, Color color) {
	Vector boxVectors0, boxVectors1, boxVectors2, boxVectors3,
		boxVectors4, boxVectors5, boxVectors6, boxVectors7;


	if (WorldToScreen(boxVectors[0], boxVectors0) &&
		WorldToScreen(boxVectors[1], boxVectors1) &&
		WorldToScreen(boxVectors[2], boxVectors2) &&
		WorldToScreen(boxVectors[3], boxVectors3) &&
		WorldToScreen(boxVectors[4], boxVectors4) &&
		WorldToScreen(boxVectors[5], boxVectors5) &&
		WorldToScreen(boxVectors[6], boxVectors6) &&
		WorldToScreen(boxVectors[7], boxVectors7)) {

		/*
		.+--5---+
		.8 4    6'|
		+--7---+'  11
		9   |  10  |
		|  ,+--|3--+
		|.0    | 2'
		+--1---+'
		*/

		Vector2D lines[12][2];
		//bottom of box
		lines[0][0] = { boxVectors0.x, boxVectors0.y };
		lines[0][1] = { boxVectors1.x, boxVectors1.y };
		lines[1][0] = { boxVectors1.x, boxVectors1.y };
		lines[1][1] = { boxVectors2.x, boxVectors2.y };
		lines[2][0] = { boxVectors2.x, boxVectors2.y };
		lines[2][1] = { boxVectors3.x, boxVectors3.y };
		lines[3][0] = { boxVectors3.x, boxVectors3.y };
		lines[3][1] = { boxVectors0.x, boxVectors0.y };

		lines[4][0] = { boxVectors0.x, boxVectors0.y };
		lines[4][1] = { boxVectors6.x, boxVectors6.y };

		// top of box
		lines[5][0] = { boxVectors6.x, boxVectors6.y };
		lines[5][1] = { boxVectors5.x, boxVectors5.y };
		lines[6][0] = { boxVectors5.x, boxVectors5.y };
		lines[6][1] = { boxVectors4.x, boxVectors4.y };
		lines[7][0] = { boxVectors4.x, boxVectors4.y };
		lines[7][1] = { boxVectors7.x, boxVectors7.y };
		lines[8][0] = { boxVectors7.x, boxVectors7.y };
		lines[8][1] = { boxVectors6.x, boxVectors6.y };


		lines[9][0] = { boxVectors5.x, boxVectors5.y };
		lines[9][1] = { boxVectors1.x, boxVectors1.y };

		lines[10][0] = { boxVectors4.x, boxVectors4.y };
		lines[10][1] = { boxVectors2.x, boxVectors2.y };

		lines[11][0] = { boxVectors7.x, boxVectors7.y };
		lines[11][1] = { boxVectors3.x, boxVectors3.y };

		for (int i = 0; i < 12; i++) {

			D::DrawLine(static_cast<int>(lines[i][0].x), static_cast<int>(lines[i][0].y), static_cast<int>(lines[i][1].x), static_cast<int>(lines[i][1].y), color);
		}
	}
}

HFont F::ESP = 0;

void D::SetupFonts() {
	if (!F::ESP)
		Interfaces->Surface->SetFontGlyphSet(F::ESP = Interfaces->Surface->Create_Font(), "Tahoma", 14, FW_DONTCARE, NULL, NULL, FONTFLAG_OUTLINE);
}

void D::DrawString(HFont font, int x, int y, Color color, uintptr_t alignment, const char* msg, ...) {
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, msg);
	_vsnprintf_s(buf, sizeof(buf), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	int r = 255, g = 255, b = 255, a = 255;
	color.GetColor(r, g, b, a);

	int width, height;
	Interfaces->Surface->GetTextSize(font, wbuf, width, height);

	if (alignment & FONT_RIGHT)
		x -= width;
	if (alignment & FONT_CENTER)
		x -= width / 2;

	Interfaces->Surface->DrawSetTextFont(font);
	Interfaces->Surface->DrawSetTextColor(r, g, b, a);
	Interfaces->Surface->DrawSetTextPos(x, y - height / 2);
	Interfaces->Surface->DrawPrintText(wbuf, wcslen(wbuf));
}

void D::DrawStringUnicode(HFont font, int x, int y, Color color, bool bCenter, const wchar_t* msg, ...) {
	int r = 255, g = 255, b = 255, a = 255;
	color.GetColor(r, g, b, a);

	int iWidth, iHeight;

	Interfaces->Surface->GetTextSize(font, msg, iWidth, iHeight);
	Interfaces->Surface->DrawSetTextFont(font);
	Interfaces->Surface->DrawSetTextColor(r, g, b, a);
	Interfaces->Surface->DrawSetTextPos(!bCenter ? x : x - iWidth / 2, y - iHeight / 2);
	Interfaces->Surface->DrawPrintText(msg, wcslen(msg));
}

void D::DrawRect(int x, int y, int w, int h, Color col) {
	Interfaces->Surface->DrawSetColor(col);
	Interfaces->Surface->DrawFilledRect(x, y, x + w, y + h);
}

int D::CreateTexture(void* tex, int w, int h) {
	int iTexture = Interfaces->Surface->CreateNewTextureID(true);
	Interfaces->Surface->DrawSetTextureRGBA(iTexture, (unsigned char*)tex, w, h);
	return iTexture;
}

void D::DrawTexturedRect(float x, float y, float w, float h, int TextureID) {
	D::DrawTexturedRect(x, y, w, h, TextureID, Color(255, 255, 255, 255));
}

void D::DrawTexturedRect(float x, float y, float w, float h, int TextureID, Color clr) {
	Interfaces->Surface->DrawSetColor(clr);
	Interfaces->Surface->DrawSetTexture(TextureID);
	Interfaces->Surface->DrawTexturedRect(x, y, x + w, y + h);
}

void D::DrawRectRainbow(int x, int y, int width, int height, float flSpeed, float &flRainbow) {
	Color colColor(0, 0, 0);

	flRainbow += flSpeed;
	if (flRainbow > 1.f) flRainbow = 0.f;

	for (int i = 0; i < width; i++) {
		float hue = (1.f / (float)width) * i;
		hue -= flRainbow;
		if (hue < 0.f) hue += 1.f;

		Color colRainbow = colColor.FromHSB(hue, 1.f, 1.f);
		D::DrawRect(x + i, y, 1, height, colRainbow);
	}
}

void D::DrawRectGradientHorizontal(int x, int y, int width, int height, Color color1, Color color2) {
	float flDifferenceR = (float)(color2.r() - color1.r()) / (float)width;
	float flDifferenceG = (float)(color2.g() - color1.g()) / (float)width;
	float flDifferenceB = (float)(color2.b() - color1.b()) / (float)width;

	for (float i = 0.f; i < width; i++) {
		Color colGradient = Color(color1.r() + (flDifferenceR * i), color1.g() + (flDifferenceG * i), color1.b() + (flDifferenceB * i), color1.a());
		D::DrawRect(x + i, y, 1, height, colGradient);
	}
}

void D::DrawPixel(int x, int y, Color col) {
	Interfaces->Surface->DrawSetColor(col);
	Interfaces->Surface->DrawFilledRect(x, y, x + 1, y + 1);
}

void D::DrawOutlinedRect(int x, int y, int w, int h, Color col) {
	Interfaces->Surface->DrawSetColor(col);
	Interfaces->Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void D::DrawOutlinedCircle(int x, int y, int r, Color col) {
	//Interfaces->Surface->DrawSetColor(col);
	//Interfaces->Surface->DrawOutlinedCircle(x, y, r, 1);

	float step = (M_PI * 2.0f) / ((float)r * 50.f);

	for (float a = 0; a < (M_PI * 2.0f); a += step) {
		Vector2D start(r * cosf(a) + x, r * sinf(a) + y);
		Vector2D end(r * cosf(a + step) + x, r * sinf(a + step) + y);
		DrawLine(start.x, start.y, end.x, end.y, col);
	}
}

void D::DrawLine(int x0, int y0, int x1, int y1, Color col) {
	Interfaces->Surface->DrawSetColor(col);
	Interfaces->Surface->DrawLine(x0, y0, x1, y1);
}

void D::DrawCorner(int iX, int iY, int iWidth, int iHeight, bool bRight, bool bDown, Color colDraw) {
	int iRealX = bRight ? iX - iWidth : iX;
	int iRealY = bDown ? iY - iHeight : iY;

	if (bDown && bRight)
		iWidth = iWidth + 1;

	D::DrawRect(iRealX, iY, iWidth, 1, colDraw);
	D::DrawRect(iX, iRealY, 1, iHeight, colDraw);

	D::DrawRect(iRealX, bDown ? iY + 1 : iY - 1, !bDown && bRight ? iWidth + 1 : iWidth, 1, Color(0, 0, 0, 255));
	D::DrawRect(bRight ? iX + 1 : iX - 1, bDown ? iRealY : iRealY - 1, 1, bDown ? iHeight + 2 : iHeight + 1, Color(0, 0, 0, 255));
}

void D::DrawPolygon(int count, Vertex_t* Vertexs, Color color) {
	static int Texture = Interfaces->Surface->CreateNewTextureID(true);
	unsigned char buffer[4] = { 255, 255, 255, 255 };

	Interfaces->Surface->DrawSetTextureRGBA(Texture, buffer, 1, 1);
	Interfaces->Surface->DrawSetColor(color);
	Interfaces->Surface->DrawSetTexture(Texture);

	Interfaces->Surface->DrawTexturedPolygon(count, Vertexs);
}

void D::DrawRoundedBox(int x, int y, int w, int h, int r, int v, Color col) {
	std::vector<Vertex_t> p;
	for (int _i = 0; _i < 3; _i++) {
		int _x = x + (_i < 2 && r || w - r);
		int _y = y + (_i % 3 > 0 && r || h - r);
		for (int i = 0; i < v; i++) {
			int a = RAD2DEG((i / v) * -90 - _i * 90);
			p.push_back(Vertex_t(Vector2D(_x + sin(a) * r, _y + cos(a) * r)));
		}
	}

	D::DrawPolygon(4 * (v + 1), &p[0], col);
	/*
	function DrawRoundedBox(x, y, w, h, r, v, col)
	local p = {};
	for _i = 0, 3 do
	local _x = x + (_i < 2 && r || w - r)
	local _y = y + (_i%3 > 0 && r || h - r)
	for i = 0, v do
	local a = math.rad((i / v) * - 90 - _i * 90)
	table.insert(p, {x = _x + math.sin(a) * r, y = _y + math.cos(a) * r})
	end
	end

	surface.SetDrawColor(col.r, col.g, col.b, 255)
	draw.NoTexture()
	surface.DrawPoly(p)
	end
	*/

	// Notes: amount of vertexes is 4(v + 1) where v is the number of vertices on each corner bit.
	// I did it in lua cause I have no idea how the vertex_t struct works and i'm still aids at C++
}

bool D::ScreenTransform(const Vector &point, Vector &screen) // tots not pasted
{
	float w;
	const VMatrix& worldToScreen = Interfaces->EngineClient->WorldToScreenMatrix();

	screen.x = worldToScreen[0][0] * point[0] + worldToScreen[0][1] * point[1] + worldToScreen[0][2] * point[2] + worldToScreen[0][3];
	screen.y = worldToScreen[1][0] * point[0] + worldToScreen[1][1] * point[1] + worldToScreen[1][2] * point[2] + worldToScreen[1][3];
	w = worldToScreen[3][0] * point[0] + worldToScreen[3][1] * point[1] + worldToScreen[3][2] * point[2] + worldToScreen[3][3];
	screen.z = 0.0f;

	bool behind = false;

	if (w < 0.001f) {
		behind = true;
		screen.x *= 100000;
		screen.y *= 100000;
	}
	else {
		behind = false;
		float invw = 1.0f / w;
		screen.x *= invw;
		screen.y *= invw;
	}

	return behind;
}

bool D::WorldToScreen(const Vector &origin, Vector &screen) {
	if (!ScreenTransform(origin, screen)) {
		int ScreenWidth, ScreenHeight;
		Interfaces->EngineClient->GetScreenSize(ScreenWidth, ScreenHeight);
		float x = ScreenWidth / 2;
		float y = ScreenHeight / 2;
		x += 0.5f * screen.x * ScreenWidth + 0.5f;
		y -= 0.5f * screen.y * ScreenHeight + 0.5f;
		screen.x = x;
		screen.y = y;
		return true;
	}

	return false;
}

int D::GetStringWidth(HFont font, const char* msg, ...) {
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, msg);
	_vsnprintf_s(buf, sizeof(buf), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	int iWidth, iHeight;

	Interfaces->Surface->GetTextSize(font, wbuf, iWidth, iHeight);

	return iWidth;
}

void D::Draw3DBox(Vector* boxVectors, Color color) {
	Vector boxVectors0, boxVectors1, boxVectors2, boxVectors3,
		boxVectors4, boxVectors5, boxVectors6, boxVectors7;


	if (D::WorldToScreen(boxVectors[0], boxVectors0) &&
		D::WorldToScreen(boxVectors[1], boxVectors1) &&
		D::WorldToScreen(boxVectors[2], boxVectors2) &&
		D::WorldToScreen(boxVectors[3], boxVectors3) &&
		D::WorldToScreen(boxVectors[4], boxVectors4) &&
		D::WorldToScreen(boxVectors[5], boxVectors5) &&
		D::WorldToScreen(boxVectors[6], boxVectors6) &&
		D::WorldToScreen(boxVectors[7], boxVectors7)) {

		/*
		.+--5---+
		.8 4    6'|
		+--7---+'  11
		9   |  10  |
		|  ,+--|3--+
		|.0    | 2'
		+--1---+'
		*/

		Vector2D lines[12][2];
		//bottom of box
		lines[0][0] = { boxVectors0.x, boxVectors0.y };
		lines[0][1] = { boxVectors1.x, boxVectors1.y };
		lines[1][0] = { boxVectors1.x, boxVectors1.y };
		lines[1][1] = { boxVectors2.x, boxVectors2.y };
		lines[2][0] = { boxVectors2.x, boxVectors2.y };
		lines[2][1] = { boxVectors3.x, boxVectors3.y };
		lines[3][0] = { boxVectors3.x, boxVectors3.y };
		lines[3][1] = { boxVectors0.x, boxVectors0.y };

		lines[4][0] = { boxVectors0.x, boxVectors0.y };
		lines[4][1] = { boxVectors6.x, boxVectors6.y };

		// top of box
		lines[5][0] = { boxVectors6.x, boxVectors6.y };
		lines[5][1] = { boxVectors5.x, boxVectors5.y };
		lines[6][0] = { boxVectors5.x, boxVectors5.y };
		lines[6][1] = { boxVectors4.x, boxVectors4.y };
		lines[7][0] = { boxVectors4.x, boxVectors4.y };
		lines[7][1] = { boxVectors7.x, boxVectors7.y };
		lines[8][0] = { boxVectors7.x, boxVectors7.y };
		lines[8][1] = { boxVectors6.x, boxVectors6.y };


		lines[9][0] = { boxVectors5.x, boxVectors5.y };
		lines[9][1] = { boxVectors1.x, boxVectors1.y };

		lines[10][0] = { boxVectors4.x, boxVectors4.y };
		lines[10][1] = { boxVectors2.x, boxVectors2.y };

		lines[11][0] = { boxVectors7.x, boxVectors7.y };
		lines[11][1] = { boxVectors3.x, boxVectors3.y };

		for (int i = 0; i < 12; i++) {
			D::DrawLine(lines[i][0].x, lines[i][0].y, lines[i][1].x, lines[i][1].y, color);
		}
	}
}

void D::DrawCircle(float x, float y, float r, float s, Color color) {
	float Step = M_PI * 2.0 / s;
	for (float a = 0; a < (M_PI*2.0); a += Step) {
		float x1 = r * cos(a) + x;
		float y1 = r * sin(a) + y;
		float x2 = r * cos(a + Step) + x;
		float y2 = r * sin(a + Step) + y;

		DrawLine(x1, y1, x2, y2, color);
	}
}
