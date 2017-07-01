#include "../../csgo-sdk.h"

void OpenMenu();

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
WNDPROC oWndProc;
LRESULT __stdcall Hooks::WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	OpenMenu();
	if (Config->Menu.Opened &&  ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

void OpenMenu() {

	static bool is_down = false;
	static bool is_clicked = false;

	if (GetAsyncKeyState(VK_INSERT) & 0x8000) {
		is_clicked = false;
		is_down = true;
	}
	else if (!(GetAsyncKeyState(VK_INSERT) & 0x8000) && is_down) {
		is_clicked = true;
		is_down = false;
	}
	else {
		is_clicked = false;
		is_down = false;
	}

	if (is_clicked) {
		Config->Menu.Opened = !Config->Menu.Opened;
		std::string msg = std::string("cl_mouseenable ") + std::to_string(!Config->Menu.Opened);
		Interfaces->EngineClient->ClientCmd_Unrestricted(msg.c_str());
	}
}
