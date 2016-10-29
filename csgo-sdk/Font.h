#pragma once

enum FontDrawType_t {
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,
	FONT_DRAW_TYPE_COUNT = 2,
};

typedef unsigned long HFont;

struct Vector2D {
public:
	float x, y;

	Vector2D() {}
	Vector2D(float x_, float y_) { x = x_; y = y_; }
};

struct FontVertex_t {
	Vector2D m_Position;
	Vector2D m_TexCoord;

	FontVertex_t() {}
	FontVertex_t(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0)) {
		m_Position = pos;
		m_TexCoord = coord;
	}
	void Init(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0)) {
		m_Position = pos;
		m_TexCoord = coord;
	}
};

typedef FontVertex_t Vertex_t;