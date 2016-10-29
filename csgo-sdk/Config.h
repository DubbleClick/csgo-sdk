#pragma once

#define Config CConfig::GetInstance()

class CConfig {
	IMPLEMENT_SINGLETON(CConfig);

public:
	struct Ragebot_s {
		bool 	Enabled;
		bool 	AutoFire;
		int 	HoldKey;
		int 	Filter; // will hold flags for team/enemy/etc.
		bool 	Silent;
		bool	RCS;
		bool 	AutoWall;
		int  	AutoWallDmg;
		int 	TargetMethod;
		int		Hitbox;
		float 	FOV = 180.f;
		int		HitChanceAmt;
		bool	NoRecoil = true;
		bool 	AutoStop;
		bool 	AutoCrouch;
		bool	AutoScope;
		bool 	FriendlyFire;
		bool	Aimstep;
		float	AimstepAmount;
		bool	HitScanAll;
		bool	Multipoint;
		bool	SmartScan;
		bool	BodyAim;
		bool	Extrapolate;
		bool	UntrustedCheck;
		bool	NoInterpolation;
		bool	OldAutowall;
		int 	ResolveMode;
		bool	BacktrackOnShot;
		size_t  ResolveTicks = 8;
		bool	LagCompensation;
		size_t	LagCompensationTicks = 13;
		bool	ResolveOnLbyUpdate;
		struct {
			bool	Enabled;
			int		Pitch;
			int		Yaw;
			int		YawFake;
			int		YawDynamic;
			bool	AimAtPlayer;
		} AntiAim;

		struct {
			bool Pelvis = true;
			bool Head = true;
			bool Chest = true;
			bool Feet = true;
			bool Arms = true;
		} Hitboxes;
	} Ragebot;

	struct {
		bool 	Enabled;
		bool	Corners;
		bool	Box;
		bool	Dlights;
		bool	Footsteps;
		bool	DeadMau5Esp;
		bool	GrenadePredictionEnabled;
		bool	BulletTrace;
		bool	LBYDifference;
		float   BulletTraceLength = 3000.f;
		int		CrosshairType;
		int		CrosshairStyle;
		float   CrosshairColor[4];

		bool	Hitboxes;
		bool	HitboxesWallcheck;
		bool	Skeleton;
		float 	TeamColor[4];
		float 	TeamColorWall[4];
		float 	EnemyColor[4];
		float 	EnemyColorWall[4];

		struct {
			bool	Enabled;
			float 	TeamColor[4];
			float 	TeamColorWall[4];
			float 	EnemyColor[4];
			float 	EnemyColorWall[4];
			float	WeaponColor[4];
			float	C4Color[4];
		} Glow;

		struct {
			bool Enemies;
			bool Teammates;
			bool Weapons;
			bool Decoy;
			bool C4;
			bool VisibleOnly;
		} Filter;

		struct {
			bool	Enabled;
			int 	Mode;
			bool	Wireframe;
			bool	XQZ;
			float 	TeamColor[4];
			float 	TeamColorWall[4];
			float  	EnemyColor[4];
			float 	EnemyColorWall[4];
		} Chams;

		struct {
			bool 	Name;
			bool 	Healthbar;
			bool	Healthtext;
			bool 	Weapon;
			bool	Flashed;
			bool	Armor;
		} Info;

		struct {
			bool	Hands;
			bool	Weapon;
			bool	VisualRecoil;
			bool	Flash;
			bool    Smoke;
			bool	Walls;
			bool	Sky;
			bool	Scope;
			bool	PostProcessing;
		} Removals;

	} Visual;

	struct {
		bool	Knifebot;
		bool 	Bhop;
		bool 	AutoStrafe;
		bool	ShakeJump;
		bool	ChatSpam;
		bool	ClantagSpam;
		bool	NameSpam;
		int		ChatSpamMode;
		bool	Watermark;
		bool	Ranks;
		bool	AirStuck;
		int		AirStuckKey;
		bool	AutoAccept;
		bool	Slide;
		bool	FreeCam;
		float	CamSpeed;
		bool	FoVEnabled;
		int		FoV = 90;
		bool	AutoBlocker;
		bool	AchievementSpam;
		int		cfg = 1;
		bool	ThirdPerson;
		bool	ThirdPersonSmart = false;
		int		CircleStrafeKey = VK_MBUTTON;
		float	AutoStrafeRetrackSpeed = 2.f;
		bool	ShowRealAA;
		
		struct {
			bool Enabled;
			int Key;
			bool Adaptive;
			int Value;
		} FakeLag;
	} Misc;

	struct {
		bool	Opened = false;
		bool	Disco;
		int 	Key;
		bool	Ragebot = false;
		bool	Legitbot = false;
		bool	Visual = false;
		bool	Misc = false;
		float	ControlColor[4];
	} Menu;

	struct {
		QAngle m_vecOldPunch;
	} Restore;

	public:

	void Load(std::string path);
	void Save(std::string path);
	void Setup();
};
