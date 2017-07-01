#include "../../csgo-sdk.h"

#define strenc(s) std::string(s)

void CConfigLoader::Setup() {
	SetupValue(Config->Ragebot.Enabled, false, strenc("Ragebot"), strenc("Enabled"));
	SetupValue(Config->Ragebot.AutoFire, false, strenc("Ragebot"), strenc("AutoFire"));
	SetupValue(Config->Ragebot.HoldKey, 0, strenc("Ragebot"), strenc("HoldKey"));
	SetupValue(Config->Ragebot.FOV, 180.f, strenc("Ragebot"), strenc("FOV"));
	SetupValue(Config->Ragebot.Silent, true, strenc("Ragebot"), strenc("Silent"));
	SetupValue(Config->Ragebot.TargetMethod, 0, strenc("Ragebot"), strenc("TargetMethod"));
	SetupValue(Config->Ragebot.Hitbox, 6, strenc("Ragebot"), strenc("Hitbox"));
	SetupValue(Config->Ragebot.FriendlyFire, false, strenc("Ragebot"), strenc("FriendlyFire"));
	SetupValue(Config->Ragebot.AutoStop, false, strenc("Ragebot"), strenc("AutoStop"));
	SetupValue(Config->Ragebot.AutoCrouch, false, strenc("Ragebot"), strenc("AutoCrouch"));
	SetupValue(Config->Ragebot.AutoWall, true, strenc("Ragebot"), strenc("AutoWall"));
	SetupValue(Config->Ragebot.AutoWallDmg, 10, strenc("Ragebot"), strenc("AutoWallDamage"));
	SetupValue(Config->Ragebot.HitChanceAmt, 50, strenc("Ragebot"), strenc("HitChanceAmount"));
	SetupValue(Config->Ragebot.Aimstep, false, strenc("Ragebot"), strenc("Aimstep"));
	SetupValue(Config->Ragebot.AimstepAmount, 15.f, strenc("Ragebot"), strenc("AimstepAmount"));
	SetupValue(Config->Ragebot.HitScanAll, false, strenc("Ragebot"), strenc("HitScanAll"));
	SetupValue(Config->Ragebot.SmartScan, false, strenc("Ragebot"), strenc("Smartscan"));
	SetupValue(Config->Ragebot.OldAutowall, false, strenc("Ragebot"), strenc("Oldautowall"));
	SetupValue(Config->Ragebot.Multipoint, false, strenc("Ragebot"), strenc("HitScanHitbox"));
	SetupValue(Config->Ragebot.UntrustedCheck, true, strenc("Ragebot"), strenc("Anti-Untrust"));
	SetupValue(Config->Ragebot.AntiAim.Enabled, false, strenc("Ragebot"), strenc("AntiaimEnabled"));
	SetupValue(Config->Ragebot.AntiAim.Pitch, 0, strenc("Ragebot"), strenc("AntiaimPitch"));
	SetupValue(Config->Ragebot.AntiAim.Yaw, 0, strenc("Ragebot"), strenc("AntiaimYaw"));
	SetupValue(Config->Ragebot.AntiAim.YawDynamic, 0, strenc("Ragebot"), strenc("AntiaimYawDynamic"));
	SetupValue(Config->Ragebot.AntiAim.YawFake, 0, strenc("Ragebot"), strenc("AntiaimYawFake"));
	SetupValue(Config->Ragebot.ResolveMode, 0, strenc("Ragebot"), strenc("ResolveMode"));
	SetupValue(Config->Ragebot.Extrapolate, false, strenc("Ragebot"), strenc("Extrapolate"));
	SetupValue(Config->Ragebot.BodyAim, false, strenc("Ragebot"), strenc("BodyAim"));
	SetupValue(Config->Ragebot.NoInterpolation, false, strenc("Ragebot"), strenc("DisableInterpolation"));
	SetupValue(Config->Ragebot.AutoScope, false, strenc("Ragebot"), strenc("AutoScope"));



	SetupValue(Config->Visual.Enabled, true, strenc("Visuals"), strenc("Enabled"));
	SetupValue(Config->Visual.LBYDifference, true, strenc("Visuals"), strenc("LBYDifference"));
	SetupValue(Config->Visual.Box, true, strenc("Visuals"), strenc("Box"));
	SetupValue(Config->Visual.Hitboxes, false, strenc("Visuals"), strenc("Hitboxes"));
	SetupValue(Config->Visual.HitboxesWallcheck, false, strenc("Visuals"), strenc("HitboxesWallcheck"));
	SetupValue(Config->Visual.Skeleton, false, strenc("Visuals"), strenc("Skeleton"));
	SetupValue(Config->Visual.DeadMau5Esp, false, strenc("Visuals"), strenc("DeadMau5ESP"));
	SetupValue(Config->Visual.BulletTrace, false, strenc("Visuals"), strenc("BulletTrace"));
	SetupValue(Config->Visual.BulletTraceLength, 200.f, strenc("Visuals"), strenc("BulletTraceLength"));
	SetupValue(Config->Visual.Glow.Enabled, true, strenc("Visuals"), strenc("GlowEnabled"));
	SetupValue(Config->Visual.Corners, true, strenc("Visuals"), strenc("Glow Enabled"));
	SetupValue(Config->Visual.Dlights, true, strenc("Visuals"), strenc("Dlights"));
	SetupValue(Config->Visual.CrosshairType, 0, strenc("Visuals"), strenc("CrosshairType"));
	SetupValue(Config->Visual.CrosshairStyle, 0, strenc("Visuals"), strenc("CrosshairStyle"));
	SetupValue(Config->Visual.Info.Name, true, strenc("Visuals"), strenc("InfoName"));
	SetupValue(Config->Visual.Info.Healthbar, true, strenc("Visuals"), strenc("InfoHealth"));
	SetupValue(Config->Visual.Info.Healthtext, true, strenc("Visuals"), strenc("InfoHealthText"));
	SetupValue(Config->Visual.Info.Weapon, true, strenc("Visuals"), strenc("InfoWeapon"));
	SetupValue(Config->Visual.Info.Flashed, true, strenc("Visuals"), strenc("InfoFlashed"));
	SetupValue(Config->Visual.Filter.Enemies, true, strenc("Visuals"), strenc("FilterEnemies"));
	SetupValue(Config->Visual.Filter.Teammates, true, strenc("Visuals"), strenc("FilterFriendlies"));
	SetupValue(Config->Visual.Filter.Weapons, true, strenc("Visuals"), strenc("FilterWeapons"));
	SetupValue(Config->Visual.Filter.Decoy, true, strenc("Visuals"), strenc("FilterDecoy"));
	SetupValue(Config->Visual.Filter.C4, true, strenc("Visuals"), strenc("FilterC4"));
	SetupValue(Config->Visual.Chams.Enabled, true, strenc("Visuals"), strenc("ChamsEnabled"));
	SetupValue(Config->Visual.Chams.Mode, 0, strenc("Visuals"), strenc("ChamsMode"));
	SetupValue(Config->Visual.Chams.XQZ, true, strenc("Visuals"), strenc("ChamsXQZ"));
	SetupValue(Config->Visual.Chams.Wireframe, true, strenc("Visuals"), strenc("ChamsWireframe"));
	SetupValue(Config->Visual.Removals.Hands, false, strenc("Visuals"), strenc("RemoveHands"));
	SetupValue(Config->Visual.Removals.Weapon, false, strenc("Visuals"), strenc("RemoveWeapon"));
	SetupValue(Config->Visual.Removals.VisualRecoil, true, strenc("Visuals"), strenc("RemoveRecoil"));
	SetupValue(Config->Visual.Removals.Flash, true, strenc("Visuals"), strenc("RemoveFlash"));
	SetupValue(Config->Visual.Removals.Smoke, false, strenc("Visuals"), strenc("RemoveSmoke"));
	SetupValue(Config->Visual.Removals.Walls, false, strenc("Visuals"), strenc("Walls"));
	SetupValue(Config->Visual.Removals.Sky, true, strenc("Visuals"), strenc("RemoveSky"));
	SetupValue(Config->Visual.Removals.Scope, true, strenc("Visuals"), strenc("RemoveScope"));

	SetupValue(Config->Visual.TeamColor[0], 0.f, strenc("Colors"), strenc("TeamColor[0]"));
	SetupValue(Config->Visual.TeamColor[1], 0.f, strenc("Colors"), strenc("TeamColor[1]"));
	SetupValue(Config->Visual.TeamColor[2], 0.f, strenc("Colors"), strenc("TeamColor[2]"));
	SetupValue(Config->Visual.TeamColor[3], 0.f, strenc("Colors"), strenc("TeamColor[3]"));
	SetupValue(Config->Visual.TeamColorWall[0], 0.f, strenc("Colors"), strenc("TeamColorWall[0]"));
	SetupValue(Config->Visual.TeamColorWall[1], 0.f, strenc("Colors"), strenc("TeamColorWall[1]"));
	SetupValue(Config->Visual.TeamColorWall[2], 0.f, strenc("Colors"), strenc("TeamColorWall[2]"));
	SetupValue(Config->Visual.TeamColorWall[3], 0.f, strenc("Colors"), strenc("TeamColorWall[3]"));
	SetupValue(Config->Visual.EnemyColor[0], 0.f, strenc("Colors"), strenc("EnemyColor[0]"));
	SetupValue(Config->Visual.EnemyColor[1], 0.f, strenc("Colors"), strenc("EnemyColor[1]"));
	SetupValue(Config->Visual.EnemyColor[2], 0.f, strenc("Colors"), strenc("EnemyColor[2]"));
	SetupValue(Config->Visual.EnemyColor[3], 0.f, strenc("Colors"), strenc("EnemyColor[3]"));
	SetupValue(Config->Visual.EnemyColorWall[0], 0.f, strenc("Colors"), strenc("EnemyColorWall[0]"));
	SetupValue(Config->Visual.EnemyColorWall[1], 0.f, strenc("Colors"), strenc("EnemyColorWall[1]"));
	SetupValue(Config->Visual.EnemyColorWall[2], 0.f, strenc("Colors"), strenc("EnemyColorWall[2]"));
	SetupValue(Config->Visual.EnemyColorWall[3], 0.f, strenc("Colors"), strenc("EnemyColorWall[3]"));
	SetupValue(Config->Visual.Glow.TeamColor[0], 0.f, strenc("Colors"), strenc("GlowTeamColor[0]"));
	SetupValue(Config->Visual.Glow.TeamColor[1], 0.f, strenc("Colors"), strenc("GlowTeamColor[1]"));
	SetupValue(Config->Visual.Glow.TeamColor[2], 0.f, strenc("Colors"), strenc("GlowTeamColor[2]"));
	SetupValue(Config->Visual.Glow.TeamColor[3], 0.f, strenc("Colors"), strenc("GlowTeamColor[3]"));
	SetupValue(Config->Visual.Glow.TeamColorWall[0], 0.f, strenc("Colors"), strenc("GlowTeamColorWall[0]"));
	SetupValue(Config->Visual.Glow.TeamColorWall[1], 0.f, strenc("Colors"), strenc("GlowTeamColorWall[1]"));
	SetupValue(Config->Visual.Glow.TeamColorWall[2], 0.f, strenc("Colors"), strenc("GlowTeamColorWall[2]"));
	SetupValue(Config->Visual.Glow.TeamColorWall[3], 0.f, strenc("Colors"), strenc("GlowTeamColorWall[3]"));
	SetupValue(Config->Visual.Glow.EnemyColor[0], 0.f, strenc("Colors"), strenc("GlowEnemyColor[0]"));
	SetupValue(Config->Visual.Glow.EnemyColor[1], 0.f, strenc("Colors"), strenc("GlowEnemyColor[1]"));
	SetupValue(Config->Visual.Glow.EnemyColor[2], 0.f, strenc("Colors"), strenc("GlowEnemyColor[2]"));
	SetupValue(Config->Visual.Glow.EnemyColor[3], 0.f, strenc("Colors"), strenc("GlowEnemyColor[3]"));
	SetupValue(Config->Visual.Glow.EnemyColorWall[0], 0.f, strenc("Colors"), strenc("GlowEnemyColorWall[0]"));
	SetupValue(Config->Visual.Glow.EnemyColorWall[1], 0.f, strenc("Colors"), strenc("GlowEnemyColorWall[1]"));
	SetupValue(Config->Visual.Glow.EnemyColorWall[2], 0.f, strenc("Colors"), strenc("GlowEnemyColorWall[2]"));
	SetupValue(Config->Visual.Glow.EnemyColorWall[3], 0.f, strenc("Colors"), strenc("GlowEnemyColorWall[3]"));
	SetupValue(Config->Visual.Glow.WeaponColor[0], 0.f, strenc("Colors"), strenc("WeaponColor[0]"));
	SetupValue(Config->Visual.Glow.WeaponColor[1], 0.f, strenc("Colors"), strenc("WeaponColor[1]"));
	SetupValue(Config->Visual.Glow.WeaponColor[2], 0.f, strenc("Colors"), strenc("WeaponColor[2]"));
	SetupValue(Config->Visual.Glow.WeaponColor[3], 0.f, strenc("Colors"), strenc("WeaponColor[3]"));
	SetupValue(Config->Visual.Glow.C4Color[0], 0.f, strenc("Colors"), strenc("C4Color[0]"));
	SetupValue(Config->Visual.Glow.C4Color[1], 0.f, strenc("Colors"), strenc("C4Color[1]"));
	SetupValue(Config->Visual.Glow.C4Color[2], 0.f, strenc("Colors"), strenc("C4Color[2]"));
	SetupValue(Config->Visual.Glow.C4Color[3], 0.f, strenc("Colors"), strenc("C4Color[3]"));
	SetupValue(Config->Visual.CrosshairColor[0], 0.f, strenc("Colors"), strenc("CrosshairColor[0]"));
	SetupValue(Config->Visual.CrosshairColor[1], 0.f, strenc("Colors"), strenc("CrosshairColor[1]"));
	SetupValue(Config->Visual.CrosshairColor[2], 0.f, strenc("Colors"), strenc("CrosshairColor[2]"));
	SetupValue(Config->Visual.CrosshairColor[3], 0.f, strenc("Colors"), strenc("CrosshairColor[3]"));

	SetupValue(Config->Menu.ControlColor[0], 0.f, strenc("Colors"), strenc("ControlColor[0]"));
	SetupValue(Config->Menu.ControlColor[1], 0.f, strenc("Colors"), strenc("ControlColor[1]"));
	SetupValue(Config->Menu.ControlColor[2], 0.f, strenc("Colors"), strenc("ControlColor[2]"));
	SetupValue(Config->Menu.ControlColor[3], 0.f, strenc("Colors"), strenc("ControlColor[3]"));

	SetupValue(Config->Misc.Knifebot, false, strenc("Misc"), strenc("Knifebot"));
	SetupValue(Config->Misc.ShowRealAA, false, "Misc", "ShowRealAA");
	SetupValue(Config->Misc.AutoBlocker, false, strenc("Misc"), strenc("AutoBlocker"));
	SetupValue(Config->Misc.Bhop, true, strenc("Misc"), strenc("Bhop"));
	SetupValue(Config->Misc.AutoStrafe, false, strenc("Misc"), strenc("AutoStrafe"));
	SetupValue(Config->Misc.Watermark, true, strenc("Misc"), strenc("Watermark"));
	SetupValue(Config->Misc.Ranks, true, strenc("Misc"), strenc("Ranks"));
	SetupValue(Config->Misc.AutoAccept, true, strenc("Misc"), strenc("AutoAccept"));
	SetupValue(Config->Misc.AchievementSpam, false, strenc("Misc"), strenc("AchievementSpam"));
	SetupValue(Config->Misc.AirStuck, false, strenc("Misc"), strenc("AirStuck"));
	SetupValue(Config->Misc.AirStuckKey, 0, strenc("Misc"), strenc("AirStuckKey"));
	SetupValue(Config->Misc.Slide, false, strenc("Misc"), strenc("Slide"));
	SetupValue(Config->Misc.ShakeJump, false, strenc("Misc"), strenc("Shakejump"));
	SetupValue(Config->Misc.FreeCam, false, strenc("Misc"), strenc("Freecam"));
	SetupValue(Config->Misc.ThirdPerson, false, strenc("Misc"), strenc("Thirdperson"));
	SetupValue(Config->Misc.CamSpeed, 0.f, strenc("Misc"), strenc("Camspeed"));
	SetupValue(Config->Misc.FoVEnabled, false, strenc("Misc"), strenc("FoVEnabled"));
	SetupValue(Config->Misc.FoV, 90, strenc("Misc"), strenc("FoV"));

	SetupValue(Config->Menu.Disco, false, strenc("Menu"), strenc("Disco"));
}

void CConfigLoader::SetupValue(int &value, int def, std::string category, std::string name) {
	value = def;
	ints.push_back(new ConfigValue<int>(category, name, &value));
}

void CConfigLoader::SetupValue(float &value, float def, std::string category, std::string name) {
	value = def;
	floats.push_back(new ConfigValue<float>(category, name, &value));
}

void CConfigLoader::SetupValue(bool &value, bool def, std::string category, std::string name) {
	value = def;
	bools.push_back(new ConfigValue<bool>(category, name, &value));
}

void CConfigLoader::Save(const char* filename) {
	std::string path(G::DllPath);
	std::string file = path.substr(0, path.find_last_of("\\")) + std::string("\\") + filename + std::string(".ini");

	for (auto value : ints)
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void CConfigLoader::Load(const char* filename) {
	std::string path(G::DllPath);
	std::string file = path.substr(0, path.find_last_of("\\")) + std::string("\\") + filename + std::string(".ini");

	char value_l[32] = { '\0' };

	for (auto value : ints) {
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats) {
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = static_cast<float>(atof(value_l));
	}

	for (auto value : bools) {
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
}

#undef strenc
