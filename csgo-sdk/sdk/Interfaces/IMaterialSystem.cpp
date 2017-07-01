#include "../../csgo-sdk.h"

IMaterial* IMaterialSystem::CreateMaterial(bool flat, bool ignorez, bool wireframed) {
	std::string type = (flat) ? "UnlitGeneric" : "VertexLitGeneric";

	std::string matdata = "\"" + type + "\"\n{\n\t\"$basetexture\" \"vgui/white_additive\"\n\t\"$envmap\"  \"\"\n\t\"$model\" \"1\"\n\t\"$flat\" \"1\"\n\t\"$nocull\"  \"0\"\n\t\"$selfillum\" \"1\"\n\t\"$halflambert\" \"1\"\n\t\"$nofog\"  \"0\"\n\t\"$znearer\" \"0\"\n\t\"$wireframe\" \"" + std::to_string(wireframed) + "\"\n\t\"$ignorez\" \"" + std::to_string(ignorez) + "\"\n}\n";

	static int created = 0;
	std::string matname = "custom_" + std::to_string(created);
	++created;

	KeyValues* pKeyValues = new KeyValues(matname.c_str());
	Util::InitKeyValues(pKeyValues, type.c_str());
	Util::LoadFromBuffer(pKeyValues, matname.c_str(), matdata.c_str());

	typedef IMaterial*(__thiscall* OriginalFn)(void*, const char *pMaterialName, KeyValues *pVMTKeyValues);
	IMaterial* createdMaterial = Util::GetVFunc<OriginalFn>(this, 83)(this, matname.c_str(), pKeyValues);

	createdMaterial->IncrementReferenceCount();

	return createdMaterial;
}

IMaterial* IMaterialSystem::FindMaterial(const char* pMaterialName, const char *pTextureGroupName, bool complain, const char *pComplainPrefix) {
	typedef IMaterial*(__thiscall* OriginalFn)(void*, char const* pMaterialName, const char *pTextureGroupName, bool complain, const char *pComplainPrefix);
	return Util::GetVFunc<OriginalFn>(this, 84)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
}

MaterialHandle_t IMaterialSystem::FirstMaterial() {
	return Util::GetVFunc<MaterialHandle_t(__thiscall*)(void*)>(this, 86)(this);
}

MaterialHandle_t IMaterialSystem::NextMaterial(MaterialHandle_t h) {
	return Util::GetVFunc<MaterialHandle_t(__thiscall*)(void*, MaterialHandle_t)>(this, 87)(this, h);
}

MaterialHandle_t IMaterialSystem::InvalidMaterial() {
	return Util::GetVFunc<MaterialHandle_t(__thiscall*)(void*)>(this, 88)(this);
}

IMaterial* IMaterialSystem::GetMaterial(MaterialHandle_t h) {
	return Util::GetVFunc<IMaterial*(__thiscall*)(void*, MaterialHandle_t)>(this, 89)(this, h);
}
