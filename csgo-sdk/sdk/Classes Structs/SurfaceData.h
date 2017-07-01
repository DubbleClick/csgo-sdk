#pragma once

struct surfacephysicsparams_t {
	float			friction;
	float			elasticity;
	float			density;
	float			thickness;
	float			dampening;
};

struct surfaceaudioparams_t {
	float			reflectivity;
	float			hardnessFactor;
	float			roughnessFactor;
	float			roughThreshold;
	float			hardThreshold;
	float			hardVelocityThreshold;
};

struct surfacesoundnames_t {
	unsigned short	walkStepLeft;
	unsigned short	walkStepRight;
// 	unsigned short	runStepLeft;
// 	unsigned short	runStepRight;
	unsigned short	impactSoft;
	unsigned short	impactHard;
	unsigned short	scrapeSmooth;
	unsigned short	scrapeRough;
	unsigned short	bulletImpact;
	unsigned short	rolling;
	unsigned short	breakSound;
	unsigned short	strainSound;
};

struct surfacesoundhandles_t {
	short	walkStepLeft;
	short	walkStepRight;
// 	short	runStepLeft;
// 	short	runStepRight;
	short	impactSoft;
	short	impactHard;
	short	scrapeSmooth;
	short	scrapeRough;
	short	bulletImpact;
	short	rolling;
	short	breakSound;
	short	strainSound;
};

struct surfacegameprops_t {
public:
	float	maxSpeedFactor;
	float	jumpFactor;
	float	unk1;
	float	flPenetrationModifier;
	float	flDamageModifier;
	unsigned short	material; // +84 // Indicates whether or not the player is on a ladder.
	unsigned char	climbable;
	unsigned char	pad;
};

struct surfacedata_t {
	surfacephysicsparams_t	physics;
	surfaceaudioparams_t	audio;
	surfacesoundnames_t		sounds;
	surfacegameprops_t		game;
	surfacesoundhandles_t	soundhandles;
};