#pragma once

#include "BaseTexture.h"
#include "Material.h"
#include "Texture.h"
#include "misc.h"

#include <bgfx/bgfx.h>

#include <map>
#include <vector>

class RenderDevice;

class Shader
{
public:
	Shader(RenderDevice* renderDevice);
	~Shader();

	bool Load(const BYTE* shaderCodeName, UINT codeSize);
	bool Load(std::vector<D3DXHANDLE> shaders);

	void Unload();

	//ID3DXEffect* Core() const;

	void Begin(const unsigned int pass);
	void End();

	void SetTexture(const D3DXHANDLE texelName, Texture* texel, const bool linearRGB);
	// TODO: void SetTexture(const D3DXHANDLE texelName, D3DTexture* texel);
	void SetMaterial(const Material* const mat);

	void SetDisableLighting(const vec4& value);
	void SetAlphaTestValue(const float value);
	void SetFlasherColorAlpha(const vec4& color);
	void SetFlasherData(const vec4& color, const float mode);
	void SetLightColorIntensity(const vec4& color);
	void SetLightColor2FalloffPower(const vec4& color);
	void SetLightData(const vec4& color);
	void SetLightImageBackglassMode(const bool imageMode, const bool backglassMode);
	void SetTechnique(const D3DXHANDLE technique);
	void SetMatrix(const D3DXHANDLE hParameter, const D3DXMATRIX* pMatrix);
	void SetVector(const D3DXHANDLE hParameter, const vec4* pVector);
	void SetFloat(const D3DXHANDLE hParameter, const float f);
	void SetInt(const D3DXHANDLE hParameter, const int i);
	void SetBool(const D3DXHANDLE hParameter, const bool b);
	void SetValue(const D3DXHANDLE hParameter, const void* pData, const unsigned int Bytes);

private:
	// TODO: ID3DXEffect* m_shader;
	RenderDevice* m_renderDevice;

	Material currentMaterial;

	vec4 currentDisableLighting;

	BaseTexture* currentTexture[TEXTURESET_STATE_CACHE_SIZE];
	float currentAlphaTestValue;
	char currentTechnique[64];

	vec4 currentFlasherColor;
	vec4 currentFlasherData;
	float currentFlasherMode;

	vec4 currentLightColor;
	vec4 currentLightColor2;
	vec4 currentLightData;
	unsigned int currentLightImageMode;
	unsigned int currentLightBackglassMode;

	bgfx::ProgramHandle m_program;
	std::map<D3DXHANDLE, bgfx::ProgramHandle> m_programMap;
};
