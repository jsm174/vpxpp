#pragma once

#include "misc.h"

#include <string>

struct SaveMaterial
{
	SaveMaterial()
	{
		memset(this, 0, sizeof(SaveMaterial));
	}

	char szName[MAXNAMEBUFFER];
	COLORREF cBase;
	COLORREF cGlossy;
	COLORREF cClearcoat;
	float fWrapLighting;
	bool bIsMetal;
	float fRoughness;
	unsigned char fGlossyImageLerp;
	float fEdge;
	unsigned char fThickness;
	float fOpacity;
	unsigned char bOpacityActive_fEdgeAlpha;
};

struct SavePhysicsMaterial
{
	SavePhysicsMaterial()
	{
		memset(this, 0, sizeof(SavePhysicsMaterial));
	}

	char szName[MAXNAMEBUFFER];
	float fElasticity;
	float fElasticityFallOff;
	float fFriction;
	float fScatterAngle;
};

class Material
{
public:
	inline Material() : m_fWrapLighting(0.0f), m_fRoughness(0.0f), m_fGlossyImageLerp(1.0f), m_fThickness(0.05f), m_fEdge(1.0f), m_fEdgeAlpha(1.0f), m_fOpacity(1.0f), m_cBase(0xB469FF), m_cGlossy(0), m_cClearcoat(0), m_bIsMetal(false), m_bOpacityActive(false), m_fElasticity(0.0f), m_fElasticityFalloff(0.0f), m_fFriction(0.0f), m_fScatterAngle(0.0f)
	{
		m_szName = "dummyMaterial";
	}

	inline Material(float wrapLighting, float roughness, float glossyImageLerp, float thickness, float edge, float edgeAlpha, float opacity,
					COLORREF base, COLORREF glossy, COLORREF clearcoat, bool isMetal, bool opacityActive,
					float elasticity, float elasticityFalloff, float friction, float scatterAngle) : m_fWrapLighting(wrapLighting), m_fRoughness(roughness), m_fGlossyImageLerp(glossyImageLerp), m_fThickness(thickness), m_fEdge(edge), m_fEdgeAlpha(edgeAlpha), m_fOpacity(opacity), m_cBase(base), m_cGlossy(glossy), m_cClearcoat(clearcoat), m_bIsMetal(isMetal), m_bOpacityActive(opacityActive), m_fElasticity(elasticity), m_fElasticityFalloff(elasticityFalloff), m_fFriction(friction), m_fScatterAngle(scatterAngle)
	{
		m_szName = "dummyMaterial";
	}

	inline Material(const Material* const pmat) : m_fWrapLighting(pmat->m_fWrapLighting), m_fRoughness(pmat->m_fRoughness), m_fGlossyImageLerp(pmat->m_fGlossyImageLerp), m_fThickness(pmat->m_fThickness), m_fEdge(pmat->m_fEdge), m_fEdgeAlpha(pmat->m_fEdgeAlpha), m_fOpacity(pmat->m_fOpacity), m_cBase(pmat->m_cBase), m_cGlossy(pmat->m_cGlossy), m_cClearcoat(pmat->m_cClearcoat), m_bIsMetal(pmat->m_bIsMetal), m_bOpacityActive(pmat->m_bOpacityActive), m_fElasticity(pmat->m_fElasticity), m_fElasticityFalloff(pmat->m_fElasticityFalloff), m_fFriction(pmat->m_fFriction), m_fScatterAngle(pmat->m_fScatterAngle)
	{
		m_szName = pmat->m_szName;
	}

	std::string m_szName;
	float m_fWrapLighting;
	float m_fRoughness;
	float m_fGlossyImageLerp;
	float m_fThickness;
	float m_fEdge;
	float m_fEdgeAlpha;
	float m_fOpacity;
	COLORREF m_cBase;
	COLORREF m_cGlossy;
	COLORREF m_cClearcoat;
	bool m_bIsMetal;
	bool m_bOpacityActive;

	float m_fElasticity;
	float m_fElasticityFalloff;
	float m_fFriction;
	float m_fScatterAngle;
};