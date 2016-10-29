#pragma once

class CHudChat;

class IClientModeShared {
public:
	char __pad[0x1C];
	CHudChat* m_pChatElement;
};