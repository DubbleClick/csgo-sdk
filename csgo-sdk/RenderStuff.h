#pragma once

class IMaterial;
class CBaseEntity;

class IRefCounted {
public:
	virtual int AddRef() = 0;
	virtual int Release() = 0;
};

class IWorldRenderList : public IRefCounted {};

struct VisibleFogVolumeInfo_t {
	int		m_nVisibleFogVolume;
	int		m_nVisibleFogVolumeLeaf;
	bool	m_bEyeInFogVolume;
	float	m_flDistanceToWater;
	float	m_flWaterHeight;
	IMaterial* m_pFogVolumeMaterial;
};

typedef unsigned short LeafIndex_t;
enum {
	INVALID_LEAF_INDEX = (LeafIndex_t)~0
};

struct WorldListLeafData_t {
	LeafIndex_t	leafIndex;
	int16	waterData;
	uint16 	firstTranslucentSurface;
	uint16	translucentSurfaceCount;
};

struct WorldListInfo_t {
	int		m_ViewFogVolume;
	int		m_LeafCount;
	bool	m_bHasWater;
	WorldListLeafData_t	*m_pLeafDataList;
};

struct VisOverrideData_t {
	Vector		m_vecVisOrigin;
	float		m_fDistToAreaPortalTolerance;
};

struct BrushVertex_t {
	Vector		m_Pos;
	Vector		m_Normal;
	Vector		m_TangentS;
	Vector		m_TangentT;
	Vector2D	m_TexCoord;
	Vector2D	m_LightmapCoord;

private:
	BrushVertex_t(const BrushVertex_t& src);
};

class IBrushSurface {
public:
	virtual void ComputeTextureCoordinate(Vector const& worldPos, Vector2D& texCoord) = 0;
	virtual void ComputeLightmapCoordinate(Vector const& worldPos, Vector2D& lightmapCoord) = 0;

	virtual int  GetVertexCount() const = 0;
	virtual void GetVertexData(BrushVertex_t* pVerts) = 0;

	virtual IMaterial* GetMaterial() = 0;
};

class IBrushRenderer {
public:
	virtual bool RenderBrushModelSurface(CBaseEntity* pBaseEntity, IBrushSurface* pBrushSurface) = 0;
};

struct model_t {
	void* fnHandle;
	char szName[260];
	int nLoadFlags;
	int nServerCount;
	int type;
	int flags;
	Vector vecMins;
	Vector vecMaxs;
	float radius;
	byte __pad0x1C[0x1C];
};
typedef unsigned short ModelInstanceHandle_t;

struct ModelRenderInfo_t {
	Vector origin;
	QAngle angles;
	void *pRenderable;
	const model_t *pModel;
	const matrix3x4_t *pModelToWorld;
	const matrix3x4_t *pLightingOffset;
	const Vector *pLightingOrigin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	ModelInstanceHandle_t instance;
	ModelRenderInfo_t() {
		pModelToWorld = NULL;
		pLightingOffset = NULL;
		pLightingOrigin = NULL;
	}
};
