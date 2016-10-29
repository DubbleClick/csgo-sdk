#pragma once

#define EnginePrediction CPredictionSystem::GetInstance()

class CPredictionSystem {
	IMPLEMENT_SINGLETON(CPredictionSystem);

public:
	void StartPrediction();

	void EndPrediction();

private:
	float m_flOldCurtime;
	float m_flOldFrametime;
	CMoveData m_MoveData;
	int* m_nPredictionRandomSeed = nullptr;
	CBaseEntity* m_pPredictionPlayer = nullptr;
};