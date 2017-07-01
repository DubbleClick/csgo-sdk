#pragma once

struct Hitbox {
	Hitbox(void) {
		hitbox = -1;
	}

	Hitbox(int newHitBox) {
		hitbox = newHitBox;
	}

	int  hitbox;
	Vector points[9];
};

struct BestPoint {
	BestPoint(void) {
		hitbox = -1;
		point = Vector(0, 0, 0);
		index = -1;
		dmg = -1;
		flags = 0;
	}

	BestPoint(int newHitBox) {
		hitbox = newHitBox;
		point = Vector(0, 0, 0);
		index = -1;
		dmg = -1;
		flags = 0;
	}

	Vector point;
	int  index;
	int  dmg;
	int  flags;
	int  hitbox;
};

#define Aimbot CAimbot::GetInstance()

class CAimbot {
	IMPLEMENT_SINGLETON(CAimbot);

public:
	void Tick();

private:
	int BestAimPointAll(CBaseEntity* target, Vector& hitbox);
	int BestAimPointHitbox(CBaseEntity* target, Vector& hitbox);
	bool EntityIsValid(CBaseEntity* i);
	void FindTarget();
	void GoToTarget();
	bool GetHitbox(CBaseEntity* target, Hitbox* hitbox);
	bool GetBestPoint(CBaseEntity* target, Hitbox* hitbox, BestPoint* point);

private:
	std::vector<int> m_hitboxes;
	int m_bestdmg = 0;
	Vector m_besthitbox = Vector(0, 0, 0);
	CBaseEntity* m_bestent = nullptr;
};
