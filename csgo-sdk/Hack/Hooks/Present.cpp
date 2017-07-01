#include "../../csgo-sdk.h"
#include "../MENU.h"

#define charenc(s) s
void RetaliateMenu(IDirect3DDevice9* pDevice);
void GUI_Init(IDirect3DDevice9* pDevice);
static const char* tabNames[] = { " HvH ", " Visuals ", " Misc ", " Colours " };
static const int numTabs = sizeof(tabNames) / sizeof(tabNames[0]);
static const char* tabTooltips[numTabs] = { "", "", "", "" };
static int tabItemOrdering[numTabs] = { 0, 1, 2, 3 };
static int selectedTab = 0;
static int optionalHoveredTab = 0;
static ImFont* defaultFont;
static ImFont* arialFont;
static ImFont* titleFont;

PresentFn oPresent;
HRESULT __stdcall Hooks::Present(IDirect3DDevice9* pDevice, RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) {
	DO_ONCE{
		Drawmanager->Initialize(pDevice);
		GUI_Init(pDevice);
	}


	Drawmanager->BeginRendering();
	//Draw Shit
	//Drawmanager->AddFont("Tahoma", 18, true, false);


	//Drawmanager->DrawText(Color::Purple(), "Tahoma", 100, 80, FNT_OUTLINED, "csgo-sdk");
	//Drawmanager->FillGradientRect(0, 0, 300, 200, Color::Yellow(), Color::Red(), Color::Blue(), Color::Green());

	Drawmanager->EndRendering();

	//Visuals->Tick();

	RetaliateMenu(pDevice);


	return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

void RetaliateMenu(IDirect3DDevice9* pDevice) {
	DWORD dwOld_D3DRS_COLORWRITEENABLE;

	pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE);
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);


	ImGui::GetIO().MouseDrawCursor = Config->Menu.Opened;

	ImGui_ImplDX9_NewFrame();

	if (Config->Menu.Opened) {
		int pX, pY;
		Interfaces->InputSystem->GetCursorPosition(&pX, &pY);
		ImGuiIO& io = ImGui::GetIO();

		io.MousePos.x = (float)(pX);
		io.MousePos.y = (float)(pY);
		ImGuiStyle& style = ImGui::GetStyle();

		ImGui::PushFont(titleFont);

		ImGui::Begin(charenc("csgo-sdk"), &Config->Menu.Opened, ImVec2(918, 350), style.Colors[ImGuiCol_WindowBg].w, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar);
		{
			ImGui::PushFont(defaultFont);

			ImGui::PushFont(arialFont);
			const bool tabSelectedChanged = ImGui::MyTabLabels(numTabs, tabNames, selectedTab, tabTooltips, false, &optionalHoveredTab, &tabItemOrdering[0], false, false);
			ImGui::PopFont();

			ImGui::BeginChild("##Child", ImVec2(0, 0), false, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar);

			if (selectedTab == 0) { //rage selected
				ImGui::Columns(2, charenc("AimbotAndTarget"), true);
				ImGui::PushFont(arialFont);
				ImGui::Text(charenc("Aimbot"));
				ImGui::PopFont();
				ImGui::BeginChild("##Aimbot", ImVec2(0.f, ImGui::GetTextLineHeightWithSpacing() * 7 - 2), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
				ImGui::PushItemWidth(ImGui::GetWindowWidth()*0.8f);
				ImGui::Checkbox("Enabled", &Config->Ragebot.Enabled);
				ImGui::Checkbox(charenc("Silent"), &Config->Ragebot.Silent);
				ImGui::SameLine(ImGui::GetWindowWidth()*0.5f);
				ImGui::Checkbox(charenc("Auto Fire"), &Config->Ragebot.AutoFire);
				ImGui::SliderFloat(charenc("FOV"), &Config->Ragebot.FOV, 1, 180, "%.0f");
				ImGui::Combo(charenc("Resolver"), &Config->Ragebot.ResolveMode, resolvermodes, ARRAYSIZE(resolvermodes));
				ImGui::Combo(charenc("Hold Key"), &Config->Ragebot.HoldKey, keyNames, ARRAYSIZE(keyNames));
				// 				ImGui::Checkbox(charenc("Aim Step"), &Config->Ragebot.Aimstep);
				// 				ImGui::SliderInt(charenc("Angle Per Tick"), (int*)&Config->Ragebot.AimstepAmount, 1.f, 180.f, "%.0f");
				ImGui::PopItemWidth();
				ImGui::EndChild();
				ImGui::NextColumn();

				ImGui::PushFont(arialFont);
				ImGui::Text(charenc("Target"));
				ImGui::PopFont();
				ImGui::BeginChild(charenc("##Target"), ImVec2(0.f, ImGui::GetTextLineHeightWithSpacing() * 7 - 2), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
				ImGui::PushItemWidth(ImGui::GetWindowWidth()*0.8f);
				ImGui::Checkbox(charenc("Friendly Fire"), &Config->Ragebot.FriendlyFire);
				ImGui::SameLine(ImGui::GetWindowWidth()*0.5f);
				ImGui::Checkbox(charenc("Auto Wall"), &Config->Ragebot.AutoWall);
				ImGui::SliderInt(charenc("Damage"), &Config->Ragebot.AutoWallDmg, 1, 100);
				ImGui::Checkbox(charenc("Multipoint"), &Config->Ragebot.Multipoint);
				ImGui::SameLine(ImGui::GetWindowWidth()*0.5f);
				ImGui::Checkbox(charenc("Hit Scan"), &Config->Ragebot.HitScanAll);
				ImGui::Combo(charenc("Mode"), &Config->Ragebot.TargetMethod, targetMode, ARRAYSIZE(targetMode));
				ImGui::Combo(charenc("Hitbox"), &Config->Ragebot.Hitbox, aimBones, ARRAYSIZE(aimBones));
				ImGui::PopItemWidth();
				ImGui::EndChild();
				ImGui::Columns(1);

				ImGui::Columns(2, charenc("AccuracyAndHitboxes"));

				ImGui::PushFont(arialFont);
				ImGui::Text(charenc("Accuracy"));
				ImGui::PopFont();
				ImGui::BeginChild(charenc("##Accuracy"), ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 4 + 5), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
				ImGui::PushItemWidth(ImGui::GetWindowWidth()*0.8f);
				ImGui::Checkbox(charenc("Auto Stop"), &Config->Ragebot.AutoStop);
				ImGui::SameLine(ImGui::GetWindowWidth()*0.5f);
				ImGui::Checkbox(charenc("Auto Crouch"), &Config->Ragebot.AutoCrouch);
				ImGui::Checkbox(charenc("Disable Interp"), &Config->Ragebot.NoInterpolation);
				ImGui::SameLine(ImGui::GetWindowWidth()*0.5f);
				ImGui::Checkbox(charenc("Auto Scope"), &Config->Ragebot.AutoScope);
				ImGui::SliderInt(charenc("Hitchance"), &Config->Ragebot.HitChanceAmt, 0, 100);
				ImGui::PopItemWidth();
				ImGui::EndChild();

				ImGui::NextColumn();

				ImGui::PushFont(arialFont);
				ImGui::Text(charenc("Scan"));
				ImGui::PopFont();
				ImGui::BeginChild(charenc("##Hitboxes"), ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 4 + 5), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
				ImGui::PushItemWidth(ImGui::GetWindowWidth()*0.8f);
				ImGui::Checkbox(charenc("Head"), &Config->Ragebot.Hitboxes.Head);
				ImGui::SameLine(ImGui::GetWindowWidth()*0.5f);
				ImGui::Checkbox("Extrapolate", &Config->Ragebot.Extrapolate);
				ImGui::Checkbox(charenc("Chest"), &Config->Ragebot.Hitboxes.Chest);
				ImGui::SameLine(ImGui::GetWindowWidth()*0.5f);
				ImGui::Checkbox(charenc("Pelvis"), &Config->Ragebot.Hitboxes.Pelvis);
				ImGui::Checkbox(charenc("Arms"), &Config->Ragebot.Hitboxes.Arms);
				ImGui::SameLine(ImGui::GetWindowWidth()*0.5f);
				ImGui::Checkbox(charenc("Feet"), &Config->Ragebot.Hitboxes.Feet);
				ImGui::PopItemWidth();
				ImGui::EndChild();


				ImGui::Columns(1);

			}

			else if (selectedTab == 1) { //Visuals

				ImGui::Columns(2, "##VisualsAndInfo");
				ImGui::PushItemWidth(ImGui::GetWindowWidth()*0.8f);
				ImGui::PushFont(arialFont);
				ImGui::Text(charenc("Visuals"));
				ImGui::PopFont();
				ImGui::BeginChild(charenc("##Visuals"), ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 8), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
				ImGui::Columns(2, charenc("##VisualsColumn1"), false);
				ImGui::Checkbox(charenc("Enable Visuals"), &Config->Visual.Enabled);
				ImGui::Checkbox(charenc("Box"), &Config->Visual.Box);
				ImGui::Checkbox(charenc("Hitboxes"), &Config->Visual.Hitboxes);
				ImGui::Checkbox(charenc("Skeleton"), &Config->Visual.Skeleton);
				ImGui::Checkbox(charenc("Grenade Prediction"), &Config->Visual.GrenadePredictionEnabled);
				ImGui::NextColumn();
				ImGui::Checkbox(charenc("Glow"), &Config->Visual.Glow.Enabled);
				ImGui::Checkbox(charenc("Corners"), &Config->Visual.Corners);
				ImGui::Checkbox(charenc("HitboxWallcheck"), &Config->Visual.HitboxesWallcheck);
				ImGui::Checkbox(charenc("Dlights"), &Config->Visual.Dlights);
				ImGui::Checkbox(charenc("LBY Difference"), &Config->Visual.LBYDifference);
				ImGui::Columns(1);
				ImGui::EndChild();
				// ImGui::SliderFloat(charenc("Trace Length"), &Config->Visuals.BulletTraceLength, 1.f, 3000.f, "%.0f");
				ImGui::NextColumn();

				ImGui::PushFont(arialFont);
				ImGui::Text(charenc("Info"));
				ImGui::PopFont();
				ImGui::BeginChild("##Info", ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 8), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
				ImGui::Columns(2, charenc("##VisualsColumn2"), false);
				ImGui::Checkbox(charenc("Name"), &Config->Visual.Info.Name);
				ImGui::Checkbox(charenc("Health (Text)"), &Config->Visual.Info.Healthtext);
				ImGui::Checkbox(charenc("Armor"), &Config->Visual.Info.Armor);
				ImGui::Checkbox(charenc("Weapon"), &Config->Visual.Info.Weapon);
				ImGui::Checkbox(charenc("Flashed"), &Config->Visual.Info.Flashed);
				ImGui::NextColumn();
				ImGui::Checkbox(charenc("Enemies"), &Config->Visual.Filter.Enemies);
				ImGui::Checkbox(charenc("Team"), &Config->Visual.Filter.Teammates);
				ImGui::Checkbox(charenc("Weapons"), &Config->Visual.Filter.Weapons);
				ImGui::Checkbox(charenc("C4"), &Config->Visual.Filter.C4);
				ImGui::Checkbox(charenc("Visible Only"), &Config->Visual.Filter.VisibleOnly);
				ImGui::Columns(1);
				ImGui::EndChild();

				ImGui::Columns(1); //VisualsAndInfo

				ImGui::Columns(2, "##RemovalsAndReserved");
				ImGui::PushFont(arialFont);
				ImGui::Text(charenc("Removals"));
				ImGui::PopFont();
				ImGui::BeginChild("##Removals", ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 4 + 6 ), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
				ImGui::Columns(2, charenc("##VisualsColumn4"), false);
				//ImGui::Checkbox(charenc("Hands##Removals"), &Config->Visual.Removals.Hands);
				//ImGui::Checkbox(charenc("Weapon##Removals"), &Config->Visual.Removals.Weapon);
				ImGui::Checkbox(charenc("Smoke##Removals"), &Config->Visual.Removals.Smoke);
				ImGui::Checkbox(charenc("Scope##Removals"), &Config->Visual.Removals.Scope);
				ImGui::Checkbox(charenc("Postprocessing##Removals"), &Config->Visual.Removals.PostProcessing);
				ImGui::NextColumn();
				ImGui::Checkbox(charenc("Recoil##Removals"), &Config->Visual.Removals.VisualRecoil);
				ImGui::Checkbox(charenc("Flash##Removals"), &Config->Visual.Removals.Flash);
				ImGui::Checkbox(charenc("Sky##Removals"), &Config->Visual.Removals.Sky);
				ImGui::Columns(1);
				ImGui::EndChild();

				ImGui::NextColumn();

				
				ImGui::Columns(1);//RemovalsAndReserved

			}

			else if (selectedTab == 2) { //misc

				ImGui::Columns(2, "VariousAndCamera");
				ImGui::PushFont(arialFont);
				ImGui::Text(charenc("Various"));
				ImGui::PopFont();
				ImGui::BeginChild("##Various", ImVec2(0, 16 * 7 +4), true, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
				ImGui::PushItemWidth(ImGui::GetWindowWidth()*0.8f);
				ImGui::Checkbox(charenc("Bunnyhop"), &Config->Misc.Bhop);
				ImGui::SameLine(ImGui::GetWindowWidth()*0.5f);
				ImGui::Checkbox(charenc("Autostrafe"), &Config->Misc.AutoStrafe);
				ImGui::Checkbox(charenc("Knifebot"), &Config->Misc.Knifebot);
				ImGui::SameLine(ImGui::GetWindowWidth()*0.5f);
				ImGui::Checkbox(charenc("Airstuck"), &Config->Misc.AirStuck);
				ImGui::Combo(charenc("Key"), &Config->Misc.AirStuckKey, keyNames, ARRAYSIZE(keyNames));
				ImGui::Checkbox(charenc("Ranks"), &Config->Misc.Ranks);
				ImGui::SameLine(ImGui::GetWindowWidth()*0.5f);
				ImGui::Checkbox("Adaptive", &Config->Misc.FakeLag.Adaptive);
				ImGui::SliderInt("Fakelag", &Config->Misc.FakeLag.Value, 0, 13);
				ImGui::PopItemWidth();
				ImGui::EndChild();

				ImGui::NextColumn();

				ImGui::PushFont(arialFont);
				ImGui::Text(charenc("Camera"));
				ImGui::PopFont();
				ImGui::BeginChild("##Camera", ImVec2(0, 16 * 6), true, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
				ImGui::PushItemWidth(ImGui::GetWindowWidth()*0.8f);
				ImGui::Checkbox(charenc("Thirdperson"), &Config->Misc.ThirdPerson);
				ImGui::SameLine(ImGui::GetWindowWidth()*0.5f);
				ImGui::Checkbox(charenc("Autoperson"), &Config->Misc.ThirdPersonSmart);
				ImGui::SliderInt(charenc("FoV"), &Config->Misc.FoV, 50, 130);
				ImGui::Checkbox("Show Real Angles", &Config->Misc.ShowRealAA);
				ImGui::PopItemWidth();
				ImGui::EndChild();

				ImGui::Columns(1); //Various and Camera

				ImGui::Columns(2, "AntiaimAndNothing");
				ImGui::PushFont(arialFont);
				ImGui::Text(charenc("Anti-Aim"));
				ImGui::PopFont();
				ImGui::BeginChild(charenc("##Anti-Aim"), ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 5 + 9), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
				ImGui::PushItemWidth(ImGui::GetWindowWidth()*0.8f);
				ImGui::Combo(charenc("Pitch"), &Config->Ragebot.AntiAim.Pitch, antiaimpitch, ARRAYSIZE(antiaimpitch));
				ImGui::Combo(charenc("Yaw"), &Config->Ragebot.AntiAim.Yaw, antiaimyaw, ARRAYSIZE(antiaimyaw));
				ImGui::Combo("FakeYaw", &Config->Ragebot.AntiAim.YawFake, antiaimyawfake, ARRAYSIZE(antiaimyawfake));
				ImGui::Combo("DynamicYaw", &Config->Ragebot.AntiAim.YawDynamic, antiaimyawdynamic, ARRAYSIZE(antiaimyawdynamic));
				ImGui::PopItemWidth();
				ImGui::EndChild();

				ImGui::NextColumn();

				ImGui::Columns(1);
			}

			if (selectedTab == 3) { // Colours

				ImGui::Columns(2, "##Draw");
				ImGui::PushFont(arialFont);
				ImGui::Text("Draw: Team");
				ImGui::PopFont();
				ImGui::BeginChild("Team##ColorsDraw", ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 3 + 3), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
				ImGui::MyColorEdit4(charenc("Visible##DrawTeam"), Config->Visual.TeamColor, ImGui::ImGuiColorEditFlags_Alpha | ImGui::ImGuiColorEditFlags_RGB | ImGui::ImGuiColorEditFlags_NoOptions);
				ImGui::MyColorEdit4(charenc("Invisible##DrawTeam"), Config->Visual.TeamColorWall, ImGui::ImGuiColorEditFlags_Alpha | ImGui::ImGuiColorEditFlags_RGB | ImGui::ImGuiColorEditFlags_NoOptions);
				ImGui::EndChild();

				ImGui::NextColumn();
				ImGui::PushFont(arialFont);
				ImGui::Text("Draw: Enemies");
				ImGui::PopFont();
				ImGui::BeginChild("Enemies##ColorsDraw", ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 3 + 3), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
				ImGui::MyColorEdit4(charenc("Visible##DrawEnemy"), Config->Visual.EnemyColor, ImGui::ImGuiColorEditFlags_Alpha | ImGui::ImGuiColorEditFlags_RGB | ImGui::ImGuiColorEditFlags_NoOptions);	ImGui::Columns(1);
				ImGui::MyColorEdit4(charenc("Invisible##DrawEnemy"), Config->Visual.EnemyColorWall, ImGui::ImGuiColorEditFlags_Alpha | ImGui::ImGuiColorEditFlags_RGB | ImGui::ImGuiColorEditFlags_NoOptions);
				ImGui::EndChild();

				ImGui::Columns(1); //draw

				ImGui::Columns(2, "##Glow");
				ImGui::PushFont(arialFont);
				ImGui::Text("Glow: Team");
				ImGui::PopFont();
				ImGui::BeginChild("Team##ColorsGlow", ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 3 + 3), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
				ImGui::MyColorEdit4(charenc("Visible##GlowTeam"), Config->Visual.Glow.TeamColor, ImGui::ImGuiColorEditFlags_Alpha | ImGui::ImGuiColorEditFlags_RGB | ImGui::ImGuiColorEditFlags_NoOptions);
				ImGui::MyColorEdit4(charenc("Invisible##GlowTeam"), Config->Visual.Glow.TeamColorWall, ImGui::ImGuiColorEditFlags_Alpha | ImGui::ImGuiColorEditFlags_RGB | ImGui::ImGuiColorEditFlags_NoOptions);
				ImGui::EndChild();

				ImGui::NextColumn();
				ImGui::PushFont(arialFont);
				ImGui::Text("Glow: Enemies");
				ImGui::PopFont();
				ImGui::BeginChild("Enemies##ColorsGlow", ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 3 + 3), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
				ImGui::MyColorEdit4(charenc("Visible##GlowEnemy"), Config->Visual.Glow.EnemyColor, ImGui::ImGuiColorEditFlags_Alpha | ImGui::ImGuiColorEditFlags_RGB | ImGui::ImGuiColorEditFlags_NoOptions);
				ImGui::MyColorEdit4(charenc("Invisible##GlowEnemy"), Config->Visual.Glow.EnemyColorWall, ImGui::ImGuiColorEditFlags_Alpha | ImGui::ImGuiColorEditFlags_RGB | ImGui::ImGuiColorEditFlags_NoOptions);
				ImGui::EndChild();
				ImGui::Columns(1);

#if 0
				ImGui::Columns(2, "##ChamsColors");
				ImGui::PushFont(arialFont);
				ImGui::Text("Chams: Team");
				ImGui::PopFont();
				ImGui::BeginChild("Team##ColorsChams", ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 3 + 3), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
				ImGui::MyColorEdit3(charenc("Visible##ChamsTeam"), Config->Visual.Chams.TeamColor, ImGui::ImGuiColorEditFlags_RGB | ImGui::ImGuiColorEditFlags_NoOptions);
				ImGui::MyColorEdit3(charenc("Invisible##ChamsTeam"), Config->Visual.Chams.TeamColorWall, ImGui::ImGuiColorEditFlags_RGB | ImGui::ImGuiColorEditFlags_NoOptions);
				ImGui::EndChild();

				ImGui::NextColumn();
				ImGui::PushFont(arialFont);
				ImGui::Text("Chams: Enemies");
				ImGui::PopFont();
				ImGui::BeginChild("Enemies##ColorsChams", ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 3 + 3), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
				ImGui::MyColorEdit3(charenc("Visible##ChamsEnemy"), Config->Visual.Chams.EnemyColor, ImGui::ImGuiColorEditFlags_RGB | ImGui::ImGuiColorEditFlags_NoOptions);
				ImGui::MyColorEdit3(charenc("Invisible##ChamsEnemy"), Config->Visual.Chams.EnemyColorWall, ImGui::ImGuiColorEditFlags_RGB | ImGui::ImGuiColorEditFlags_NoOptions);
				ImGui::EndChild();
				ImGui::Columns(1);
#endif

				//ImGui::NewLine();

				ImGui::Columns(2, "##ConfigAndMenucolors");
				ImGui::PushFont(arialFont);
				ImGui::Text("Config");
				ImGui::PopFont();
				ImGui::BeginChild("##Config", ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 4 + 8), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
				ImGui::Combo(charenc("Config"), &Config->Misc.cfg, configNames, ARRAYSIZE(configNames));
				static bool saveConfigEnabled = false;
				static int selectedConfig = 1;
				if (ImGui::Button(charenc("Load Config"), ImVec2(93.f, 20.f))) {
					ConfigLoader->Load(configNames[Config->Misc.cfg]);
					selectedConfig = Config->Misc.cfg;
				}
				ImGui::SameLine(ImGui::GetWindowWidth()*0.623f - 1);
				static int warningticks = 0;
				static std::string warningmsg = "";
				if (warningticks > 0) {
					ImGui::PushFont(arialFont);
					ImGui::TextColored(ImVec4(1, 0, 0, 1), warningmsg.c_str());
					ImGui::PopFont();
					warningticks--;
				}
				else {
					if (ImGui::Button(charenc("Save Config"), ImVec2(93.f, 20.f))) {
						if (saveConfigEnabled && selectedConfig == Config->Misc.cfg) {
							ConfigLoader->Save(configNames[Config->Misc.cfg]); saveConfigEnabled = false;
						}
						else {
							warningticks = 200;
							if (!saveConfigEnabled)
								warningmsg = charenc("Unlock config save.");
							else
								warningmsg = charenc("Wrong config selected.");
							style.Colors[ImGuiCol_CheckMark] = ImVec4(Config->Menu.ControlColor[0], Config->Menu.ControlColor[1], Config->Menu.ControlColor[2], Config->Menu.ControlColor[3]);
							style.Colors[ImGuiCol_SliderGrab] = style.Colors[ImGuiCol_CheckMark];
							style.Colors[ImGuiCol_SliderGrabActive] = style.Colors[ImGuiCol_SliderGrab]; style.Colors[ImGuiCol_SliderGrabActive].w = 1.f;
							style.Colors[ImGuiCol_TitleBgActive] = style.Colors[ImGuiCol_WindowBg];
						}
					}
				}

				ImGui::Checkbox("Rainbows", &GLOBAL_RAINBOW);
				ImGui::SameLine(ImGui::GetWindowWidth()*0.623f - 1);
				ImGui::Checkbox("Unlock Save", &saveConfigEnabled);
				ImGui::EndChild();

				ImGui::NextColumn();

				ImGui::PushFont(arialFont);
				ImGui::Text("Menu");
				ImGui::PopFont();
				ImGui::BeginChild("##MenuColors", ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 4 + 8), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
				ImGui::MyColorEdit4(charenc("Text"), (float*)&style.Colors[ImGuiCol_Text], ImGui::ImGuiColorEditFlags_Alpha | ImGui::ImGuiColorEditFlags_RGB | ImGui::ImGuiColorEditFlags_NoOptions);
				ImGui::MyColorEdit4(charenc("Background"), (float*)&style.Colors[ImGuiCol_WindowBg], ImGui::ImGuiColorEditFlags_Alpha | ImGui::ImGuiColorEditFlags_RGB | ImGui::ImGuiColorEditFlags_NoOptions);
				ImGui::MyColorEdit4(charenc("Controls"), (float*)Config->Menu.ControlColor, ImGui::ImGuiColorEditFlags_Alpha | ImGui::ImGuiColorEditFlags_RGB | ImGui::ImGuiColorEditFlags_NoOptions);
				ImGui::EndChild();

				ImGui::Columns(1);//Config and Menucolors
						}

			ImGui::EndChild();
			ImGui::PopFont();
					}
		ImGui::End(); // end test window
		ImGui::PopFont();

				}
	ImGui::Render();
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);
			}

void GUI_Init(IDirect3DDevice9* pDevice) {
	ImGui_ImplDX9_Init(G::Window, pDevice);

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.03f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(1.00f, 0.99f, 0.97f, 0.23f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.08f, 0.39f, 1.00f, 0.31f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.08f, 0.39f, 1.00f, 0.91f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.92f, 1.00f, 0.93f, 0.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.08f, 0.39f, 1.00f, 0.40f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.08f, 0.39f, 1.00f, 0.68f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.08f, 0.39f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.08f, 0.39f, 1.00f, 0.69f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.08f, 0.39f, 1.00f, 0.69f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.39f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.77f, 0.77f, 0.77f, 0.20f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.67f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.08f, 0.39f, 1.00f, 0.17f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.08f, 0.39f, 1.00f, 0.23f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.08f, 0.39f, 1.00f, 0.42f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.08f, 0.39f, 1.00f, 0.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.08f, 0.39f, 1.00f, 0.00f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.08f, 0.39f, 1.00f, 0.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.70f, 0.70f, 0.90f, 0.00f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 0.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);



	style.WindowRounding = 7.f;
	style.ChildWindowRounding = 7.f;
	style.WindowTitleAlign = ImGuiAlign_Center;
	style.FramePadding = ImVec2(4, 2);
	style.ScrollbarSize = 13.f;
	style.ScrollbarRounding = 16.f;
	style.GrabMinSize = 7.f;
	style.GrabRounding = 4.f;

	defaultFont = ImGui::GetIO().Fonts->AddFontDefault();
	arialFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 16);
	titleFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\ALGER.TTF", 24);
}
