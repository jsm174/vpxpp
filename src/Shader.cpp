#include "Shader.h"

#include "Player.h"
#include "RenderDevice.h"
#include "extern.h"

#include <assert.h>
#include <float.h>
#include <fstream>
#include <iostream>

static const std::map<bgfx::RendererType::Enum, const char*> rendererTypeExtensionMap{
	{bgfx::RendererType::Direct3D11, "d3d11"},
	{bgfx::RendererType::Direct3D12, "d3d12"},
	{bgfx::RendererType::Direct3D9, "d3d9"},
	{bgfx::RendererType::OpenGL, "gl"},
	{bgfx::RendererType::Metal, "mtl"},
	{bgfx::RendererType::Noop, "noop"},
	{bgfx::RendererType::Vulkan, "vk"},
};

Shader::Shader(RenderDevice* renderDevice) : currentMaterial(-FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX,
															 0xCCCCCCCC, 0xCCCCCCCC, 0xCCCCCCCC, false, false, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX)
{
	m_renderDevice = renderDevice;
	// TODO: m_shader = 0;
	for (unsigned int i = 0; i < TEXTURESET_STATE_CACHE_SIZE; ++i)
	{
		currentTexture[i] = 0;
	}
	currentFlasherMode = -FLT_MAX;
	currentAlphaTestValue = -FLT_MAX;
	currentDisableLighting = vec4(-FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX);
	currentFlasherData = vec4(-FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX);
	currentFlasherColor = vec4(-FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX);
	currentLightColor = vec4(-FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX);
	currentLightColor2 = vec4(-FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX);
	currentLightData = vec4(-FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX);
	currentLightImageMode = ~0u;
	currentLightBackglassMode = ~0u;
	currentTechnique[0] = 0;
}

Shader::~Shader()
{
	// TODO: if (m_shader)
	// TODO: {
	// TODO: 	this->Unload();
	// TODO: }
}

bool Shader::Load(const BYTE* shaderCodeName, UINT codeSize)
{
	// TODO: LPD3DXBUFFER pBufferErrors;
	// DWORD dwShaderFlags = 0;
	// HRESULT hr;

	// hr = D3DXCreateEffect(m_renderDevice->GetCoreDevice(), shaderCodeName, codeSize, NULL, NULL, dwShaderFlags, NULL, &m_shader, &pBufferErrors);
	// if (FAILED(hr))
	// {
	// 	if (pBufferErrors)
	// 	{
	// 		LPVOID pCompileErrors = pBufferErrors->GetBufferPointer();
	// 		g_pvp->MessageBox((const char*)pCompileErrors, "Compile Error", MB_OK | MB_ICONEXCLAMATION);
	// 	}
	// 	else
	// 		g_pvp->MessageBox("Unknown Error", "Compile Error", MB_OK | MB_ICONEXCLAMATION);

	// 	return false;
	// }

	return true;
}

bool Shader::Load(std::vector<D3DXHANDLE> shaders)
{
	bgfx::RendererType::Enum type = bgfx::getRendererType();

	char filename[512];

	for (int i = 0; i < shaders.size(); i++)
	{
		bgfx::ShaderHandle vertexShader = BGFX_INVALID_HANDLE;
		bgfx::ShaderHandle fragmentShader = BGFX_INVALID_HANDLE;

		{
			snprintf(filename, sizeof(filename), "vs_%s_%s.bin", shaders.at(i), rendererTypeExtensionMap.at(type));

			std::ifstream stream(filename);
			if (!stream.fail())
			{
				stream.seekg(0, std::ios::end);
				size_t size = stream.tellg();
				stream.seekg(0);
				char* buffer = (char*)malloc(size);
				stream.read((char*)buffer, size);
				stream.close();
				const bgfx::Memory* mem = bgfx::alloc(size);
				bx::memCopy(mem->data, buffer, size);
				vertexShader = bgfx::createShader(mem);
				free(buffer);
			}
		}

		{
			snprintf(filename, sizeof(filename), "fs_%s_%s.bin", shaders.at(i), rendererTypeExtensionMap.at(type));

			std::ifstream stream(filename);
			if (!stream.fail())
			{
				stream.seekg(0, std::ios::end);
				size_t size = stream.tellg();
				stream.seekg(0);
				char* buffer = (char*)malloc(size);
				stream.read((char*)buffer, size);
				stream.close();
				const bgfx::Memory* mem = bgfx::alloc(size);
				bx::memCopy(mem->data, buffer, size);
				fragmentShader = bgfx::createShader(mem);
				free(buffer);
			}
		}

		if (bgfx::isValid(vertexShader) && bgfx::isValid(fragmentShader))
		{
			bgfx::ProgramHandle program = bgfx::createProgram(vertexShader, fragmentShader);
			if (bgfx::isValid(program))
			{
				m_programMap.insert(std::pair<D3DXHANDLE, bgfx::ProgramHandle>(shaders.at(i), program));
			}
		}
	}

	return true;
}

void Shader::Unload()
{
	// TODO: SAFE_RELEASE(m_shader);
}

void Shader::SetTexture(const D3DXHANDLE texelName, Texture* texel, const bool linearRGB)
{
	const unsigned int idx = texelName[strlen(texelName) - 1] - '0';
	assert(idx < TEXTURESET_STATE_CACHE_SIZE);

	if (!texel || !texel->m_pdsBuffer)
	{
		currentTexture[idx] = NULL;

		std::cout << "Setting Texture NULL: " << texelName << std::endl;

		// TODO: CHECKD3D(m_shader->SetTexture(texelName, NULL));

		m_renderDevice->m_curTextureChanges++;

		return;
	}

	if (texel->m_pdsBuffer != currentTexture[idx])
	{
		currentTexture[idx] = texel->m_pdsBuffer;

		std::cout << "Setting Texture: " << texelName << std::endl;

		// TODO: CHECKD3D(m_shader->SetTexture(texelName, m_renderDevice->m_texMan.LoadTexture(texel->m_pdsBuffer, linearRGB)));

		bgfx::UniformHandle handle = bgfx::createUniform(texelName, bgfx::UniformType::Sampler);
		bgfx::setTexture(0, handle, m_renderDevice->m_texMan.LoadTexture(texel->m_pdsBuffer, linearRGB));

		m_renderDevice->m_curTextureChanges++;
	}
}

// void Shader::SetTexture(const D3DXHANDLE texelName, D3DTexture* texel)
// {
// 	const unsigned int idx = texelName[strlen(texelName) - 1] - '0';
// 	assert(idx < TEXTURESET_STATE_CACHE_SIZE);

// 	currentTexture[idx] = NULL;

// 	CHECKD3D(m_shader->SetTexture(texelName, texel));

// 	m_renderDevice->m_curTextureChanges++;
// }

void Shader::SetMaterial(const Material* const mat)
{
	COLORREF cBase;
	COLORREF cGlossy;
	COLORREF cClearcoat;

	float fWrapLighting;
	float fRoughness;
	float fGlossyImageLerp;
	float fThickness;
	float fEdge;
	float fEdgeAlpha;
	float fOpacity;

	bool bIsMetal;
	bool bOpacityActive;

	if (mat)
	{
		fWrapLighting = mat->m_fWrapLighting;
		fRoughness = exp2f(10.0f * mat->m_fRoughness + 1.0f);
		fGlossyImageLerp = mat->m_fGlossyImageLerp;
		fThickness = mat->m_fThickness;
		fEdge = mat->m_fEdge;
		fEdgeAlpha = mat->m_fEdgeAlpha;
		fOpacity = mat->m_fOpacity;
		cBase = mat->m_cBase;
		cGlossy = mat->m_cGlossy;
		cClearcoat = mat->m_cClearcoat;
		bIsMetal = mat->m_bIsMetal;
		bOpacityActive = mat->m_bOpacityActive;
	}
	else
	{
		fWrapLighting = 0.0f;
		fRoughness = exp2f(10.0f * 0.0f + 1.0f);
		fGlossyImageLerp = 1.0f;
		fThickness = 0.05f;
		fEdge = 1.0f;
		fEdgeAlpha = 1.0f;
		fOpacity = 1.0f;
		cBase = g_pvp->m_dummyMaterial.m_cBase;
		cGlossy = 0;
		cClearcoat = 0;
		bIsMetal = false;
		bOpacityActive = false;
	}

	if (fRoughness != currentMaterial.m_fRoughness ||
		fEdge != currentMaterial.m_fEdge ||
		fWrapLighting != currentMaterial.m_fWrapLighting ||
		fThickness != currentMaterial.m_fThickness)
	{
		const vec4 rwem(fRoughness, fWrapLighting, fEdge, fThickness);
		SetVector("Roughness_WrapL_Edge_Thickness", &rwem);
		currentMaterial.m_fRoughness = fRoughness;
		currentMaterial.m_fWrapLighting = fWrapLighting;
		currentMaterial.m_fEdge = fEdge;
		currentMaterial.m_fThickness = fThickness;
	}

	const float alpha = bOpacityActive ? fOpacity : 1.0f;
	if (cBase != currentMaterial.m_cBase || alpha != currentMaterial.m_fOpacity)
	{
		const vec4 cBaseF = convertColor(cBase, alpha);
		SetVector("cBase_Alpha", &cBaseF);
		currentMaterial.m_cBase = cBase;
		currentMaterial.m_fOpacity = alpha;
	}

	if (!bIsMetal)
	{
		if (cGlossy != currentMaterial.m_cGlossy ||
			fGlossyImageLerp != currentMaterial.m_fGlossyImageLerp)
		{
			const vec4 cGlossyF = convertColor(cGlossy, fGlossyImageLerp);
			SetVector("cGlossy_ImageLerp", &cGlossyF);
			currentMaterial.m_cGlossy = cGlossy;
			currentMaterial.m_fGlossyImageLerp = fGlossyImageLerp;
		}
	}

	if (cClearcoat != currentMaterial.m_cClearcoat ||
		(bOpacityActive && fEdgeAlpha != currentMaterial.m_fEdgeAlpha))
	{
		const vec4 cClearcoatF = convertColor(cClearcoat, fEdgeAlpha);
		SetVector("cClearcoat_EdgeAlpha", &cClearcoatF);
		currentMaterial.m_cClearcoat = cClearcoat;
		currentMaterial.m_fEdgeAlpha = fEdgeAlpha;
	}

	if (bOpacityActive)
	{
		g_pplayer->m_pin3d.EnableAlphaBlend(false);
	}
	else
	{
		g_pplayer->m_pin3d.DisableAlphaBlend();
	}
}

//ID3DXEffect* Core() const
//{
//	return m_shader;
//}

void Shader::Begin(const unsigned int pass)
{
	unsigned int cPasses;
	// TODO: CHECKD3D(m_shader->Begin(&cPasses, 0));
	// TODO: CHECKD3D(m_shader->BeginPass(pass));
}

void Shader::End()
{
	if (bgfx::isValid(m_program))
	{
		bgfx::submit(0, m_program);
	}

	// TODO: CHECKD3D(m_shader->EndPass());
	// TODO: CHECKD3D(m_shader->End());
}

void Shader::SetDisableLighting(const vec4& value)
{
	if (currentDisableLighting.x != value.x || currentDisableLighting.y != value.y)
	{
		currentDisableLighting = value;
		SetVector("fDisableLighting_top_below", &value);
	}
}

void Shader::SetAlphaTestValue(const float value)
{
	if (currentAlphaTestValue != value)
	{
		currentAlphaTestValue = value;
		SetFloat("alphaTestValue", value);
	}
}

void Shader::SetFlasherColorAlpha(const vec4& color)
{
	if (currentFlasherColor.x != color.x || currentFlasherColor.y != color.y || currentFlasherColor.z != color.z || currentFlasherColor.w != color.w)
	{
		currentFlasherColor = color;
		SetVector("staticColor_Alpha", &color);
	}
}

void Shader::SetFlasherData(const vec4& color, const float mode)
{
	if (currentFlasherData.x != color.x || currentFlasherData.y != color.y || currentFlasherData.z != color.z || currentFlasherData.w != color.w)
	{
		currentFlasherData = color;
		SetVector("alphaTestValueAB_filterMode_addBlend", &color);
	}
	if (currentFlasherMode != mode)
	{
		currentFlasherMode = mode;
		SetFloat("flasherMode", mode);
	}
}

void Shader::SetLightColorIntensity(const vec4& color)
{
	if (currentLightColor.x != color.x || currentLightColor.y != color.y || currentLightColor.z != color.z || currentLightColor.w != color.w)
	{
		currentLightColor = color;
		SetVector("lightColor_intensity", &color);
	}
}

void Shader::SetLightColor2FalloffPower(const vec4& color)
{
	if (currentLightColor2.x != color.x || currentLightColor2.y != color.y || currentLightColor2.z != color.z || currentLightColor2.w != color.w)
	{
		currentLightColor2 = color;
		SetVector("lightColor2_falloff_power", &color);
	}
}

void Shader::SetLightData(const vec4& color)
{
	if (currentLightData.x != color.x || currentLightData.y != color.y || currentLightData.z != color.z || currentLightData.w != color.w)
	{
		currentLightData = color;
		SetVector("lightCenter_maxRange", &color);
	}
}

void Shader::SetLightImageBackglassMode(const bool imageMode, const bool backglassMode)
{
	if (currentLightImageMode != (unsigned int)imageMode || currentLightBackglassMode != (unsigned int)backglassMode)
	{
		currentLightImageMode = (unsigned int)imageMode;
		currentLightBackglassMode = (unsigned int)backglassMode;
		SetBool("lightingOff", imageMode || backglassMode);
	}
}

void Shader::SetTechnique(const D3DXHANDLE technique)
{
	if (strcmp(currentTechnique, technique))
	{
		// TODO: strncpy_s(currentTechnique, technique, sizeof(currentTechnique) - 1);
		strncpy(currentTechnique, technique, sizeof(currentTechnique) - 1);

		// TODO: CHECKD3D(m_shader->SetTechnique(technique));

		if (m_programMap.count(technique))
		{
			m_program = m_programMap.at(technique);
		}
		else
		{
			m_program = BGFX_INVALID_HANDLE;
		}

		m_renderDevice->m_curTechniqueChanges++;
	}
}

void Shader::SetMatrix(const D3DXHANDLE hParameter, const D3DXMATRIX* pMatrix)
{
	bgfx::UniformHandle handle = bgfx::createUniform(hParameter, bgfx::UniformType::Mat4);
	bgfx::setUniform(handle, pMatrix);
	//bgfx::destroy(handle);

	// TODO: m_shader->SetMatrix(hParameter, pMatrix);
	m_renderDevice->m_curParameterChanges++;
}

void Shader::SetVector(const D3DXHANDLE hParameter, const vec4* pVector)
{
	bgfx::UniformHandle handle = bgfx::createUniform(hParameter, bgfx::UniformType::Vec4);
	bgfx::setUniform(handle, pVector);
	//bgfx::destroy(handle);

	// TODO: CHECKD3D(m_shader->SetVector(hParameter, pVector));
	m_renderDevice->m_curParameterChanges++;
}

void Shader::SetFloat(const D3DXHANDLE hParameter, const float f)
{
	vec4 vec(f, 0, 0, 0);

	bgfx::UniformHandle handle = bgfx::createUniform(hParameter, bgfx::UniformType::Vec4);
	bgfx::setUniform(handle, &vec);

	// TODO: CHECKD3D(m_shader->SetFloat(hParameter, f));
	m_renderDevice->m_curParameterChanges++;
}

void Shader::SetInt(const D3DXHANDLE hParameter, const int i)
{
	const vec4 vec(i, 0, 0, 0);

	bgfx::UniformHandle handle = bgfx::createUniform(hParameter, bgfx::UniformType::Vec4);
	bgfx::setUniform(handle, &vec);

	// TODO: CHECKD3D(m_shader->SetInt(hParameter, i));
	m_renderDevice->m_curParameterChanges++;
}

void Shader::SetBool(const D3DXHANDLE hParameter, const bool b)
{
	const vec4 vec(b ? 1 : 0, 0, 0, 0);

	bgfx::UniformHandle handle = bgfx::createUniform(hParameter, bgfx::UniformType::Vec4);
	bgfx::setUniform(handle, &vec);

	// TODO: CHECKD3D(m_shader->SetBool(hParameter, b));
	m_renderDevice->m_curParameterChanges++;
}

void Shader::SetValue(const D3DXHANDLE hParameter, const void* pData, const unsigned int Bytes)
{
	// TODO: CHECKD3D(m_shader->SetValue(hParameter, pData, Bytes));
	m_renderDevice->m_curParameterChanges++;
}
