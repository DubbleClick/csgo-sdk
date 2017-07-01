#pragma once

#define Miscellaneous CMiscellaneous::GetInstance()

class CMiscellaneous {
	IMPLEMENT_SINGLETON(CMiscellaneous);
public:
	void Tick();
	void FixMovement(QAngle& wish_angle);
	void AntiAim();

private:
	void Bunnyhop();
	void Fakelag();
	void KnifeTrigger();
	void Autostrafe();
	void Airstuck();
};
