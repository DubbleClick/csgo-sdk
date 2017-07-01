#pragma once

class IVEfx {
public:
	virtual void F0();
	virtual void F1();
	virtual void F2();
	virtual void F3();
	virtual dlight_t* CL_AllocDlight(int key);
	virtual dlight_t* CL_AllocElight(int key);
	virtual void F6();
	virtual void F7();
	virtual dlight_t* GetElightByKey(int key);
};