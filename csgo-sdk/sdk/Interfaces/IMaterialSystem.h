#pragma once

typedef unsigned short MaterialHandle_t;

class IMaterialSystem
{
public:
	IMaterial* CreateMaterial( bool flat, bool ignorez, bool wireframed );
	IMaterial* FindMaterial( char const* pMaterialName, const char *pTextureGroupName, bool complain = true, const char *pComplainPrefix = NULL );
	MaterialHandle_t FirstMaterial();
	MaterialHandle_t InvalidMaterial();
	MaterialHandle_t NextMaterial(MaterialHandle_t h);
	IMaterial* GetMaterial(MaterialHandle_t h);
};