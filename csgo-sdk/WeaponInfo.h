#pragma once


class WeaponInfo_t {
public:
	char _pad1[6]; //0x0000
	char m_pszName[32]; //0x0006
	char _pad2[48]; //0x0026
	char m_pszHudName[32]; //0x0056
	char _pad3[48]; //0x0076
	char m_pszModelName[32]; //0x00A6
	char _pad4[1826]; //0x00C6
	char _pad10[0x50];
	int m_WeaponType; //0x07E8
	char _pad5[0x4]; //0x07EC
	int m_iWeaponId; //0x007F0
	bool m_bIsFullAuto; //0x07F4
	char _pad6[0x7]; //0x07F5
	int m_iPrice; //0x07FC
	float m_flArmorRatio; //0x0800
	char _pad7[0x10]; //0x0804
	float m_flPenetration; //0x0814
	int m_iDamage; //0x0818
	float m_flRange; //0x081C
	float m_flRangeModifier; //0x0820
	char _pad9[0x4]; //0x0824
	float m_flCycleTime; //0x0828
};

class CHudTexture {
public:
	char type[64]; //0x0000
	char subtype[64]; //0x0040
	char unknowndata00[2]; //0x0080
	char charinFont; //0x0082
	char unknowndata01[1]; //0x0083
};//Size=0x00AC
