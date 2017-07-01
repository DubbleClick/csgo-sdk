#pragma once

class CHudChat {
public:
	void ChatPrintf(const char* fmt, ...) {
		va_list va_alist;
		char* szBuffer = new char[2048];
		va_start(va_alist, fmt);
		int len = vsprintf_s(szBuffer, sizeof(szBuffer), fmt, va_alist);
		va_end(va_alist);
		szBuffer[len] = '\0';
		//Util::GetVFunc<void(__cdecl*)(void*, int, int, const char*)>(this, 26)(this, 0, 0, szBuffer);
	}
	void ChatPrintf(int a, int b, const char* fmt) {
		//Util::GetVFunc<void(__cdecl*)(void*, int, int, const char*)>(this, 26)(this, a, b, fmt);
	}

	void ChatPrintfW(const wchar_t* fmt, ...) {
		va_list va_alist;
		wchar_t* wszBuffer = new wchar_t[2048];
		va_start(va_alist, fmt);
		int len = vswprintf_s(wszBuffer, 2048, fmt, va_alist);
		va_end(va_alist);
		wszBuffer[len] = L'\0';
		//Util::GetVFunc<void(__thiscall*)(void*, int, int, const wchar_t*)>(this, 27)(this, 0, 0, wszBuffer);
	}

	void ChatPrintfW(int a, int b, const wchar_t* fmt) {
		//Util::GetVFunc<void(__thiscall*)(void*, int, int, const wchar_t*)>(this, 27)(this, 0, 0, fmt);
	}
};