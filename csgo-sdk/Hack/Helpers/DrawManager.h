#pragma once

#define Drawmanager CDrawManager::GetInstance()
#undef DrawText

enum TextFlags {
	FNT_OUTLINED = (1 << 1),
	FNT_SHADOWED = (1 << 2),
	FNT_CENTERED = (1 << 3),
};

class CDrawManager {
public:
	static std::shared_ptr<CDrawManager> GetInstance() {
		static std::shared_ptr<CDrawManager> instance(new CDrawManager);
		return instance;
	}

private:
	CDrawManager() { Initialize(nullptr); }
	CDrawManager(IDirect3DDevice9* pDevice);
	CDrawManager(CDrawManager const&) = delete;
	void operator=(CDrawManager const&) = delete;
public: 
	~CDrawManager() { ReleaseObjects(); }

public:
	bool Initialize(IDirect3DDevice9* pDevice);
	void ReleaseObjects();
	void CreateObjects();
	void AddFont(std::string dictname, int pt = 12, bool bold = false, bool italic = false, std::string fontname = "");
	void OnLostDevice();
	void OnResetDevice();
	void BeginRendering();
	void EndRendering();
	void RenderLine(D3DCOLOR color, int x1, int y1, int x2, int y2);
	void RenderRect(D3DCOLOR color, int x, int y, int w, int h);
	void RenderCircle(D3DCOLOR color, int x, int y, int r, int resolution = 64);
	void RenderText(D3DCOLOR color, int x, int y, bool centered, const char* fmt, ...);
	void FillRect(D3DCOLOR color, int x, int y, int w, int h);
	void DrawLine(int x1, int y1, int x2, int y2, Color clr = Color::White());
	void DrawGradientLine(int x1, int y1, int x2, int y2, Color from = Color::White(), Color to = Color::Black());
	void DrawRect(int x, int y, int w, int h, Color clr = Color::White());
	void DrawCircle(int x, int y, int radius, Color clr = Color::White(), int resolution = 64);
	void DrawText(int x, int y, const char* fmt, ...);
	void DrawText(Color clr, int x, int y, unsigned fntflags, const char* fmt, ...);
	void DrawText(Color clr, std::string font, int x, int y, unsigned fntflags, const char* fmt, ...);
	void FillTriangle(int x, int y, int w, int h, Color clr);
	void FillRect(int x, int y, int w, int h, Color clr = Color::White());
	void FillCircle(int x, int y, int radius, Color clr = Color::White(), int resolution = 64);
	void FillGradientRect(int x, int y, int w, int h, Color from = Color::White(), Color to = Color::Black());
	void FillGradientRect(int x, int y, int w, int h, Color c1, Color c2, Color c3, Color c4);

private:
	IDirect3DDevice9*					m_pDevice;
	IDirect3DStateBlock9*				m_pStateBlock;
	std::map<std::string, ID3DXFont*>	m_pFonts;
	bool								m_bCanDraw;
};
struct D3DVertex {
	D3DVertex() {};
	D3DVertex(float _x, float _y, float _z, D3DCOLOR _color)
		: x(_x), y(_y), z(_z), color(_color) {}
	D3DVertex(float _x, float _y, float _z, Color _color)
		: x(_x), y(_y), z(_z), color(_color.GetD3DColor()) {}
	//D3DVertex(int _x, int _y, int _z, Color _color)
	//	: x(static_cast<float>(_x)), y(static_cast<float>(_y)), z(static_cast<float>(_z)), color(_color.GetD3DColor()) {}

	float x = 0;
	float y = 0;
	float z = 0;
	float rhw = 0;
	D3DCOLOR color = 0x00000000;
};
