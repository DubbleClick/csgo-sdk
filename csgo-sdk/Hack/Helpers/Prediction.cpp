#include "../../csgo-sdk.h"

void CPredictionSystem::StartPrediction() {
	m_flOldCurtime = Interfaces->Globals->curtime;
	m_flOldFrametime = Interfaces->Globals->frametime;

	if (!m_nPredictionRandomSeed) {
		m_nPredictionRandomSeed = *(int**)(Util::FindPattern("client.dll", "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 2);
		//m_nPredictionRandomSeed = *reinterpret_cast< int** >(reinterpret_cast< uint32_t >(U::GetVFunc<uint32_t>((void*)Interfaces->Prediction, 19)) + 0x30);
	}

	*m_nPredictionRandomSeed = MD5_PseudoRandom(G::UserCmd->command_number) & 0x7FFFFFFF;
	Interfaces->Globals->curtime = G::LocalPlayer->m_nTickBase() * Interfaces->Globals->interval_per_tick;
	Interfaces->Globals->frametime = Interfaces->Globals->interval_per_tick;

	Interfaces->GameMovement->StartTrackPredictionErrors(G::LocalPlayer);

	memset(&m_MoveData, 0, sizeof(m_MoveData));
	Interfaces->MoveHelper->SetHost(G::LocalPlayer);
	Interfaces->Prediction->SetupMove(G::LocalPlayer, G::UserCmd, Interfaces->MoveHelper, &m_MoveData);
	Interfaces->GameMovement->ProcessMovement(G::LocalPlayer, &m_MoveData);
	Interfaces->Prediction->FinishMove(G::LocalPlayer, G::UserCmd, &m_MoveData);
}

void CPredictionSystem::EndPrediction() {
	Interfaces->GameMovement->FinishTrackPredictionErrors(G::LocalPlayer);
	Interfaces->MoveHelper->SetHost(nullptr);

	*m_nPredictionRandomSeed = -1;
	Interfaces->Globals->curtime = m_flOldCurtime;
	Interfaces->Globals->frametime = m_flOldFrametime;
}
