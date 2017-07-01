#pragma once

class IMoveHelper;
class CMoveData;

class IPrediction
{
public:
	void SetupMove(CBaseEntity *player, CUserCmd *ucmd, IMoveHelper *pHelper, CMoveData *move);
	void FinishMove(CBaseEntity *player, CUserCmd *ucmd, CMoveData *move);
};
