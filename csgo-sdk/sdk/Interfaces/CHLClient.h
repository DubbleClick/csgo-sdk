#pragma once

class CHLClient {
public:
};

class CVerifiedUserCmd {
public:
	CUserCmd			m_cmd;
	unsigned long		m_crc;
};

class IInput {
public:
	IInput(void);
	~IInput(void);

	virtual	void		Init_All(void) = 0;
	virtual	void		Shutdown_All(void) = 0;
	virtual	int			GetButtonBits(int) = 0;
	virtual	void		CreateMove(int sequence_number, float input_sample_frametime, bool active) = 0;
	virtual	void		ExtraMouseSample(float frametime, bool active) = 0;
	virtual	bool		WriteUsercmdDeltaToBuffer(byte *buf, int from, int to, bool isnewcommand) = 0; //bf_write*
	virtual	void		EncodeUserCmdToBuffer(byte& buf, int slot) = 0; //bf_write&
	virtual	void		DecodeUserCmdFromBuffer(byte& buf, int slot) = 0; //bf_read&

	virtual	CUserCmd*	GetUserCmd(int sequence_number) = 0;

public:
	bool set_camera_in_third_person(const bool active);
	bool set_camera_offset(const QAngle& view_angles, const float range);

private:
//	PAD(0x4);                                  /// 0x00 - VTable
public:
	bool              m_bTrackIRAvailable;        /// 0x04
	bool              m_bMouseInitialized;        /// 0x05
	bool              m_bMouseActive;             /// 0x06
	bool              m_bJoystickAdvancedInit;    /// 0x07
private:
	PAD(0x2C);                                 /// 0x08
public:
	void*             m_Keys;                    /// 0x34
private:
	PAD(0x64);                                 /// 0x38
public:
	bool              m_bCameraInterceptingMouse; /// 0x9C
	bool              m_bCameraInThirdPerson;     /// 0x9D
	bool              m_bCameraMovingWithMouse;   /// 0x9E
	Vector			  m_vecCameraOffset;            /// 0xA0
	bool              m_bCameraDistanceMove;      /// 0xAC
	int32_t           m_nCameraOldX;              /// 0xB0
	int32_t           m_nCameraOldY;              /// 0xB4
	int32_t           m_nCameraX;                 /// 0xB8
	int32_t           m_nCameraY;                 /// 0xBC
	bool              m_bCameraIsOrthographic;    /// 0xC0
	QAngle            m_angPreviousViewAngles;      /// 0xC4
	QAngle            m_angPreviousViewAnglesTilt;  /// 0xD0
	float             m_flLastForwardMove;         /// 0xDC
	int32_t           m_ClearInputState;         /// 0xE0
private:
	PAD(0x8);                                  /// 0xE4
public:
	CUserCmd*         m_cmds;                /// 0xEC
	CVerifiedUserCmd* m_pVerifiedCommands;        /// 0xF0

	__forceinline CUserCmd* GetUserCmd(int nSlot, int sequence_number) {
		return &m_cmds[sequence_number % 150];
	}
};