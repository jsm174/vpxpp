#pragma once

#include "def.h"
#include "misc.h"

#include <stdio.h>
#include <string.h>

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