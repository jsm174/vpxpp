#pragma once

#include "def.h"
#include "misc.h"

#include <string.h>
#include <stdio.h>

struct SaveMaterial //!! could still squeeze some stuff in here, due to implicit padding after the bool & unsigned chars!
{
	SaveMaterial() { memset(this,0,sizeof(SaveMaterial)); }

	char szName[MAXNAMEBUFFER];
	COLORREF cBase; // can be overridden by texture on object itself
	COLORREF cGlossy; // specular of glossy layer
	COLORREF cClearcoat; // specular of clearcoat layer
	float fWrapLighting; // wrap/rim lighting factor (0(off)..1(full))
	bool  bIsMetal; // is a metal material or not
	float fRoughness; // roughness of glossy layer (0(diffuse)..1(specular))
	unsigned char fGlossyImageLerp; // use image also for the glossy layer (0(no tinting at all)..1(use image)), stupid quantization because of legacy loading/saving
	float fEdge; // edge weight/brightness for glossy and clearcoat (0(dark edges)..1(full fresnel))
	unsigned char fThickness; // thickness for transparent materials (0(paper thin)..1(maximum)), stupid quantization because of legacy loading/saving
	float fOpacity; // opacity (0..1)
	unsigned char bOpacityActive_fEdgeAlpha; // lowest bit = on/off, upper 7bits = edge weight for fresnel (0(no opacity change)..1(full fresnel)), stupid encoding because of legacy loading/saving
};

struct SavePhysicsMaterial
{
	SavePhysicsMaterial() { memset(this,0,sizeof(SavePhysicsMaterial)); }

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

	//physics
	float m_fElasticity;
	float m_fElasticityFalloff;
	float m_fFriction;
	float m_fScatterAngle;
};