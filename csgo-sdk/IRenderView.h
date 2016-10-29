#pragma once

//typedef VPlane Frustum[ FRUSTUM_NUMPLANES ];

class IVRenderView {
public:
	virtual void			DrawBrushModel(CBaseEntity *baseentity, model_t *model, const Vector& origin, const QAngle& angles, bool sort) = 0;
	virtual void			DrawIdentityBrushModel(IWorldRenderList *pList, model_t *model) = 0;
	virtual void			TouchLight(struct dlight_t *light) = 0;
	virtual void			Draw3DDebugOverlays(void) = 0;

	virtual void			SetBlend(float blend) = 0;
	virtual float			GetBlend(void) = 0;

	virtual void			SetColorModulation(float const* blend) = 0;
	virtual void			GetColorModulation(float* blend) = 0;

	virtual void			SceneBegin(void) = 0;
	virtual void			SceneEnd(void) = 0;

	virtual void			GetVisibleFogVolume(const Vector& eyePoint, VisibleFogVolumeInfo_t *pInfo) = 0;
	virtual IWorldRenderList* CreateWorldList() = 0;

	virtual void			BuildWorldLists(IWorldRenderList *pList, WorldListInfo_t* pInfo, int iForceFViewLeaf, const VisOverrideData_t* pVisData = NULL, bool bShadowDepth = false, float *pReflectionWaterHeight = NULL) = 0;
	virtual void			DrawWorldLists(IWorldRenderList *pList, unsigned long flags, float waterZAdjust) = 0;
	virtual int				GetNumIndicesForWorldLists(IWorldRenderList *pList, unsigned long nFlags) = 0;

	virtual void			DrawTopView(bool enable) = 0;
	virtual void			TopViewBounds(Vector2D const& mins, Vector2D const& maxs) = 0;
	virtual void			DrawLights(void) = 0;
	virtual void			DrawMaskEntities(void) = 0;
	virtual void			DrawTranslucentSurfaces(IWorldRenderList *pList, int *pSortList, int sortCount, unsigned long flags) = 0;
	virtual void			DrawLineFile(void) = 0;
	virtual void			DrawLightmaps(IWorldRenderList *pList, int pageId) = 0;
	virtual void			ViewSetupVis(bool novis, int numorigins, const Vector origin[]) = 0;
	// Return true if any of these leaves are visible in the current PVS.
	virtual bool			AreAnyLeavesVisible(int *leafList, int nLeaves) = 0;
	virtual	void			VguiPaint(void) = 0;

	virtual void			ViewDrawFade(byte *color, IMaterial *pMaterial) = 0;
	virtual void			OLD_SetProjectionMatrix(float fov, float zNear, float zFar) = 0;
	virtual ColorVec		GetLightAtPoint(Vector& pos) = 0;
	virtual int				GetViewEntity(void) = 0;
	virtual bool			IsViewEntity(int entindex) = 0;
	virtual float			GetFieldOfView(void) = 0;
	virtual unsigned char	**GetAreaBits(void) = 0;
	virtual void			SetFogVolumeState(int nVisibleFogVolume, bool bUseHeightFog) = 0;
	virtual void			InstallBrushSurfaceRenderer(IBrushRenderer* pBrushRenderer) = 0;
	virtual void			DrawBrushModelShadow(IClientRenderable *pRenderable) = 0;
	virtual	bool			LeafContainsTranslucentSurfaces(IWorldRenderList *pList, int sortIndex, unsigned long flags) = 0;
	virtual bool			DoesBoxIntersectWaterVolume(const Vector &mins, const Vector &maxs, int leafWaterDataID) = 0;
	virtual void			SetAreaState(unsigned char chAreaBits[MAX_AREA_STATE_BYTES], unsigned char chAreaPortalBits[MAX_AREA_PORTAL_STATE_BYTES]) = 0;

	virtual void			VGui_Paint(int mode) = 0;
};