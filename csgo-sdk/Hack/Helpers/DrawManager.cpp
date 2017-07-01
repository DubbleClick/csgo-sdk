#include "../../csgo-sdk.h"

// marc is faggot

bool CDrawManager::Initialize(IDirect3DDevice9* device) {
	static bool initialized = false;
	if (device && !initialized) {
		initialized = true;
		m_pDevice = device;
		CreateObjects();
		AddFont("Default", 12, false, false, "Tahoma"); //default font
		return true;
	}
	if (device == m_pDevice && initialized)
		return true;

	return false;
}

void CDrawManager::OnLostDevice() {
	ReleaseObjects();
}

void CDrawManager::OnResetDevice() {
	CreateObjects();
}

void CDrawManager::ReleaseObjects() {
	if (!m_bCanDraw) return;
	if (m_pStateBlock) m_pStateBlock->Release();
	for (auto font : m_pFonts)
		font.second->Release();
	m_pFonts.clear();
}

void CDrawManager::CreateObjects() {
	if (FAILED(m_pDevice->CreateStateBlock(D3DSBT_ALL, &m_pStateBlock)) || FAILED(D3DXCreateFontA(m_pDevice, 12, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial", &m_pFonts["Default"]))) {
		m_bCanDraw = false;
	}
	else m_bCanDraw = true;
}

void CDrawManager::AddFont(std::string dictname, int pt, bool bold, bool italic, std::string fontname) {
	if (fontname.empty())
		fontname = dictname;
	if (m_pFonts.find(dictname) == m_pFonts.end()) {
		ID3DXFont* font;
		if (FAILED(D3DXCreateFontA(m_pDevice, pt, 0, bold ? FW_BOLD : FW_NORMAL, 0, italic, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, fontname.c_str(), &font))) {
			throw "Failed to create font";
		}
		m_pFonts[dictname] = font;
	}
}

void CDrawManager::BeginRendering() {
	if (!m_bCanDraw) return;
	m_pStateBlock->Capture();
	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, false);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
}

void CDrawManager::EndRendering() {
	if (m_bCanDraw)
		m_pStateBlock->Apply();
}

void CDrawManager::RenderLine(D3DCOLOR color, int x1, int y1, int x2, int y2) {
	D3DVertex vertices[2] = {
		D3DVertex(static_cast<float>(x1), static_cast<float>(y1), 1.0f, color),
		D3DVertex(static_cast<float>(x2), static_cast<float>(y2), 1.0f, color)
	};
	m_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, sizeof(D3DVertex));
}

void CDrawManager::RenderRect(D3DCOLOR color, int x, int y, int w, int h) {
	D3DVertex vertices[5] = {
		D3DVertex(static_cast<float>(x), static_cast<float>(y), 1.0f, color),
		D3DVertex(static_cast<float>(x + w), static_cast<float>(y), 1.0f, color),
		D3DVertex(static_cast<float>(x + w),static_cast<float>(y + h), 1.0f, color),
		D3DVertex(static_cast<float>(x), static_cast<float>(y + h), 1.0f, color),
		D3DVertex(static_cast<float>(x), static_cast<float>(y), 1.0f, color)
	};
	m_pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, vertices, sizeof(D3DVertex));
}

void CDrawManager::RenderCircle(D3DCOLOR color, int x, int y, int r, int resolution) {
	float curPointX;
	float curPointY;
	float oldPointX;
	float oldPointY;
	for (int i = 0; i <= resolution; ++i) {
		curPointX = x + r * cos(2 * M_PI * i / resolution);
		curPointY = y - r * sin(2 * M_PI * i / resolution);
		if (i > 0) {
			RenderLine(color, static_cast<int>(curPointX), static_cast<int>(curPointY), static_cast<int>(oldPointX), static_cast<int>(oldPointY));
		}
		oldPointX = curPointX;
		oldPointY = curPointY;
	}
}

void CDrawManager::RenderText(D3DCOLOR color, int x, int y, bool centered, const char* fmt, ...) {
	char buffer[512];
	va_list args;
	va_start(args, fmt);
	vsprintf_s(buffer,sizeof(buffer), fmt, args);
	va_end(args);
	auto drawShadow = [&](RECT rect) {
		rect.left++;
		m_pFonts["Default"]->DrawTextA(NULL, buffer, -1, &rect, DT_TOP | DT_LEFT | DT_NOCLIP, 0xFF000000);
		rect.top++;
		m_pFonts["Default"]->DrawTextA(NULL, buffer, -1, &rect, DT_TOP | DT_LEFT | DT_NOCLIP, 0xFF000000);
	};
	if (centered) {
		RECT rec = { 0, 0, 0, 0 };
		m_pFonts["Default"]->DrawTextA(NULL, buffer, -1, &rec, DT_CALCRECT | DT_NOCLIP, color);
		rec = { x - rec.right / 2, y, 0, 0 };
		drawShadow(rec);
		m_pFonts["Default"]->DrawTextA(NULL, buffer, -1, &rec, DT_TOP | DT_LEFT | DT_NOCLIP, color);
	}
	else {
		RECT rec = { x, y, 1000, 100 };
		drawShadow(rec);
		m_pFonts["Default"]->DrawTextA(NULL, buffer, -1, &rec, DT_TOP | DT_LEFT | DT_NOCLIP, color);
	}
}

void CDrawManager::FillTriangle(int x, int y, int w, int h, Color clr) {
	D3DVertex vertices[3] = {
		D3DVertex(static_cast<float>(x), static_cast<float>(y), 1.f, clr.GetD3DColor()),
		D3DVertex(static_cast<float>(x + w), static_cast<float>(y), 1.f, clr.GetD3DColor()),
		D3DVertex(static_cast<float>(x + w), static_cast<float>(y + h), 1.f, clr.GetD3DColor())
	};
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 3, vertices, sizeof(D3DVertex));
}

void CDrawManager::DrawLine(int x1, int y1, int x2, int y2, Color clr) {
	RenderLine(clr.GetD3DColor(), x1, y1, x2, y2);
}

void CDrawManager::DrawGradientLine(int x1, int y1, int x2, int y2, Color from, Color to) {
	D3DVertex vertices[2] = {
		D3DVertex(static_cast<float>(x1), static_cast<float>(y1), 1.0f, from.GetD3DColor()),
		D3DVertex(static_cast<float>(x2), static_cast<float>(y2), 1.0f, to.GetD3DColor())
	};
	m_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, sizeof(D3DVertex));
}

void CDrawManager::DrawRect(int x, int y, int w, int h, Color clr) {
	RenderRect(clr.GetD3DColor(), x, y, w, h);
}

void CDrawManager::DrawCircle(int x, int y, int radius, Color clr, int resolution) {
	RenderCircle(clr.GetD3DColor(), x, y, radius, resolution);
}

void CDrawManager::DrawText(int x, int y, const char * fmt, ...) {
	char buffer[512] = {};
	va_list args;
	va_start(args, fmt);
	vsprintf_s(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	DrawText(Color::White().GetD3DColor(), x, y, FNT_CENTERED, buffer);
}

void CDrawManager::DrawText(Color clr, int x, int y, unsigned fntflags, const char* fmt, ...) {
	char buffer[512] = {};
	va_list args;
	va_start(args, fmt);
	vsprintf_s(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	DrawText(clr, "Default", x, y, fntflags, buffer);
}

void CDrawManager::DrawText(Color clr, std::string font, int x, int y, unsigned fntflags, const char* fmt, ...) {
	if (m_pFonts.find(font) == m_pFonts.end())
		AddFont(font);
	ID3DXFont* CurrentFont = (font == "Default" ? m_pFonts["Default"] : m_pFonts[font]);
	char buffer[512] = {};
	va_list args;
	va_start(args, fmt);
	vsprintf_s(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	auto drawShadow = [&](RECT rect) {
		rect.left++;
		CurrentFont->DrawTextA(NULL, buffer, -1, &rect, DT_TOP | DT_LEFT | DT_NOCLIP, 0xFF000000);
		rect.top++;
		CurrentFont->DrawTextA(NULL, buffer, -1, &rect, DT_TOP | DT_LEFT | DT_NOCLIP, 0xFF000000);
	};
	auto drawOutline = [&](RECT rect) {
		rect.left--;
		CurrentFont->DrawTextA(NULL, buffer, -1, &rect, DT_TOP | DT_LEFT | DT_NOCLIP, 0xFF000000);
		rect.left += 2;
		CurrentFont->DrawTextA(NULL, buffer, -1, &rect, DT_TOP | DT_LEFT | DT_NOCLIP, 0xFF000000);
		rect.left--;
		rect.top--;
		CurrentFont->DrawTextA(NULL, buffer, -1, &rect, DT_TOP | DT_LEFT | DT_NOCLIP, 0xFF000000);
		rect.top += 2;
		CurrentFont->DrawTextA(NULL, buffer, -1, &rect, DT_TOP | DT_LEFT | DT_NOCLIP, 0xFF000000);
	};
	if (fntflags & FNT_CENTERED) {
		RECT rec = { 0, 0, 0, 0 };
		CurrentFont->DrawTextA(NULL, buffer, -1, &rec, DT_CALCRECT | DT_NOCLIP, clr.GetD3DColor());
		rec = { x - rec.right / 2, y, 0, 0 };
		if (fntflags & FNT_SHADOWED) drawShadow(rec);
		if (fntflags & FNT_OUTLINED) drawOutline(rec);
		CurrentFont->DrawTextA(NULL, buffer, -1, &rec, DT_TOP | DT_LEFT | DT_NOCLIP, clr.GetD3DColor());
	}
	else {
		RECT rec = { x, y, 1000, 100 };
		if (fntflags & FNT_SHADOWED) drawShadow(rec);
		if (fntflags & FNT_OUTLINED) drawOutline(rec);
		CurrentFont->DrawTextA(NULL, buffer, -1, &rec, DT_TOP | DT_LEFT | DT_NOCLIP, clr.GetD3DColor());
	}
}

void CDrawManager::FillRect(int x, int y, int w, int h, Color clr) {
	FillRect(clr.GetD3DColor(), x, y, w, h);
}

void CDrawManager::FillCircle(int x, int y, int radius, Color clr, int resolution) {
	resolution = min(resolution, 197);
	D3DVertex vertices[200];
	float x1 = static_cast<float>(x), x2, y1 = static_cast<float>(y), y2;
	for (int i = 0; i <= resolution; i += 3) {
		float angle = (i / 57.3f);
		x2 = x + (radius * sin(angle));
		y2 = y + (radius * cos(angle));
		vertices[i] = D3DVertex(static_cast<float>(x), static_cast<float>(y), 1.f, clr);
		vertices[i + 1] = D3DVertex(x1, y1, 1.f, clr);
		vertices[i + 2] = D3DVertex(x2, y2, 1.f, clr);
		y1 = y2;
		x1 = x2;
	}
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, resolution, vertices, sizeof(D3DVertex));
}

void CDrawManager::FillRect(D3DCOLOR color, int x, int y, int w, int h) {
	D3DVertex vertices[6] = {
		D3DVertex(static_cast<float>(x),static_cast<float>(y), 1.0f, color),
		D3DVertex(static_cast<float>(x + w),static_cast<float>(y), 1.0f, color),
		D3DVertex(static_cast<float>(x), static_cast<float>(y + h), 1.0f, color),
		D3DVertex(static_cast<float>(x + w), static_cast<float>(y + h), 1.0f, color),
		D3DVertex(static_cast<float>(x), static_cast<float>(y), 1.0f, color),
		D3DVertex(static_cast<float>(x + w),static_cast<float>(y), 1.0f, color),
	};
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 4, vertices, sizeof(D3DVertex));
}

void CDrawManager::FillGradientRect(int x, int y, int w, int h, Color from, Color to) {
	FillGradientRect(x, y, w, h, from, from, to, to);
}

void CDrawManager::FillGradientRect(int x, int y, int w, int h, Color c1, Color c2, Color c3, Color c4) {
	D3DVertex vertices[6] = {
		D3DVertex(static_cast<float>(x), static_cast<float>(y), 0.0f, c1.GetD3DColor()),
		D3DVertex(static_cast<float>(x + w), static_cast<float>(y), 0.0f, c2.GetD3DColor()),
		D3DVertex(static_cast<float>(x), static_cast<float>(y + h), 0.0f, c4.GetD3DColor()),
		D3DVertex(static_cast<float>(x + w), static_cast<float>(y + h), 0.0f, c3.GetD3DColor()),
		D3DVertex(static_cast<float>(x), static_cast<float>(y), 1.0f, c1.GetD3DColor()),
		D3DVertex(static_cast<float>(x + w), static_cast<float>(y), 0.0f, c2.GetD3DColor()),
	};
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 4, vertices, sizeof(D3DVertex));
}
