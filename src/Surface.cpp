#include "Surface.h"
#include "RegUtil.h"

#include "DragPoint.h"
#include "Inlines.h"
#include "Player.h"
#include "extern.h"
#include "mesh.h"

#include "misc.h"

#include <iostream>

const ItemTypeEnum Surface::ItemType = eItemSurface;
const int Surface::TypeNameID = 104;
const int Surface::ToolID = 110;
const int Surface::CursorID = 138;
const unsigned Surface::AllowedViews = 1;

const WORD Surface::rgiSlingshot[24] = {0, 4, 3, 0, 1, 4, 1, 2, 5, 1, 5, 4, 4, 8, 5, 4, 7, 8, 3, 7, 4, 3, 6, 7};
IndexBuffer* Surface::slingIBuffer = NULL;

Surface* Surface::COMCreate()
{
	/*CComObject<Surface>* obj = 0;
	if ((((HRESULT)(CComObject<Surface>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Surface();
}

IEditable* Surface::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Surface::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Surface* obj = Surface::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Surface::Surface()
{
	// TODO: m_menuid = IDR_SURFACEMENU;

	m_d.m_collidable = true;
	m_d.m_slingshotAnimation = true;
	m_d.m_inner = true;
	m_d.m_isBottomSolid = false;
	m_slingshotVBuffer = NULL;
	m_VBuffer = NULL;
	m_IBuffer = NULL;
	// TODO: m_propPhysics = NULL;
	m_d.m_overwritePhysics = true;
}

Surface::~Surface()
{
}

bool Surface::IsTransparent() const
{
	bool result = false;
	if (m_d.m_sideVisible)
	{
		result = m_ptable->GetMaterial(m_d.m_szSideMaterial)->m_bOpacityActive;
	}
	if (m_d.m_topBottomVisible)
	{
		result = result || m_ptable->GetMaterial(m_d.m_szTopMaterial)->m_bOpacityActive;
	}
	return result;
}

HRESULT Surface::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_ptable = ptable;

	m_isWall = true;

	const float width = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Wall", "Width", 50.f) : 50.f;
	const float length = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Wall", "Length", 50.f) : 50.f;

	/* 	TODO:
	CComObject<DragPoint>* pdp;
	CComObject<DragPoint>::CreateInstance(&pdp);
	if (pdp)
	{
		pdp->AddRef();
		pdp->Init(this, x - width, y - length, 0.f, false);
		m_vdpoint.push_back(pdp);
	}
	CComObject<DragPoint>::CreateInstance(&pdp);
	if (pdp)
	{
		pdp->AddRef();
		pdp->Init(this, x - width, y + length, 0.f, false);
		m_vdpoint.push_back(pdp);
	}
	CComObject<DragPoint>::CreateInstance(&pdp);
	if (pdp)
	{
		pdp->AddRef();
		pdp->Init(this, x + width, y + length, 0.f, false);
		m_vdpoint.push_back(pdp);
	}
	CComObject<DragPoint>::CreateInstance(&pdp);
	if (pdp)
	{
		pdp->AddRef();
		pdp->Init(this, x + width, y - length, 0.f, false);
		m_vdpoint.push_back(pdp);
	} */

	SetDefaults(fromMouseClick);

	return InitVBA(true, 0, NULL);
}

HRESULT Surface::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			GetPTable()->GetUniqueName(eItemSurface, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t*)m_wzName,
			//             sizeof(m_wzName) / sizeof(m_wzName[0]));
		}
	}
	InitScript();
	return ((HRESULT)0L);
}

PinTable* Surface::GetPTable()
{
	return m_ptable;
}

HRESULT Surface::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	SetDefaults(false);

	m_ptable = pTable;

	BiffReader biffReader(pStream, this, pId, version);
	biffReader.Load();

	if (!m_d.m_inner)
	{
		const size_t cvertex = m_vdpoint.size();

		float miny = FLT_MAX;
		size_t minyindex = 0;

		for (size_t i = 0; i < cvertex; i++)
		{
			float y;
			m_vdpoint[i]->get_Y(&y);
			if (y < miny)
			{
				miny = y;
				minyindex = i;
			}
		}

		float tmpx;
		m_vdpoint[minyindex]->get_X(&tmpx);
		const float tmpy = miny;

		std::reverse(m_vdpoint.begin(), m_vdpoint.end());

		/* TODO:		
		CComObject<DragPoint>* pdp;
		CComObject<DragPoint>::CreateInstance(&pdp);
		if (pdp)
		{
			pdp->AddRef();
			pdp->Init(this, m_ptable->m_left, m_ptable->m_top, 0.f, false);
			m_vdpoint.insert(m_vdpoint.begin() + (cvertex - minyindex - 1), pdp);
		}
		CComObject<DragPoint>::CreateInstance(&pdp);
		if (pdp)
		{
			pdp->AddRef();
			pdp->Init(this, m_ptable->m_right, m_ptable->m_top, 0.f, false);
			m_vdpoint.insert(m_vdpoint.begin() + (cvertex - minyindex - 1), pdp);
		}
		CComObject<DragPoint>::CreateInstance(&pdp);
		if (pdp)
		{
			pdp->AddRef();
			pdp->Init(this, m_ptable->m_right + 1.0f, m_ptable->m_bottom, 0.f, false); //!!! +1 needed for whatever reason (triangulation screwed up)
			m_vdpoint.insert(m_vdpoint.begin() + (cvertex - minyindex - 1), pdp);
		}
		CComObject<DragPoint>::CreateInstance(&pdp);
		if (pdp)
		{
			pdp->AddRef();
			pdp->Init(this, m_ptable->m_left, m_ptable->m_bottom, 0.f, false);
			m_vdpoint.insert(m_vdpoint.begin() + (cvertex - minyindex - 1), pdp);
		}
		CComObject<DragPoint>::CreateInstance(&pdp);
		if (pdp)
		{
			pdp->AddRef();
			pdp->Init(this, m_ptable->m_left - 1.0f, m_ptable->m_top, 0.f, false); //!!! -1 needed for whatever reason (triangulation screwed up)
			m_vdpoint.insert(m_vdpoint.begin() + (cvertex - minyindex - 1), pdp);
		}
		CComObject<DragPoint>::CreateInstance(&pdp);
		if (pdp)
		{
			pdp->AddRef();
			pdp->Init(this, tmpx, tmpy, 0.f, false);
			m_vdpoint.insert(m_vdpoint.begin() + (cvertex - minyindex - 1), pdp);
		} */

		m_d.m_inner = true;
	}

	return S_OK;
}

HRESULT Surface::InitPostLoad()
{
	return S_OK;
}

void Surface::SetDefaults(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	static const char strKeyName[] = "DefaultProps\\Wall";

	HRESULT hr;

	m_d.m_tdr.m_TimerEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "TimerEnabled", false) : false;
	m_d.m_tdr.m_TimerInterval = fromMouseClick ? pRegUtil->LoadValueIntWithDefault(strKeyName, "TimerInterval", 100) : 100;
	m_d.m_hitEvent = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "HitEvent", false) : false;
	m_d.m_threshold = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "HitThreshold", 2.0f) : 2.0f;
	m_d.m_slingshot_threshold = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "SlingshotThreshold", 0.0f) : 0.0f;
	m_d.m_inner = true;

	hr = pRegUtil->LoadValue(strKeyName, "TopImage", m_d.m_szImage);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_d.m_szImage.clear();
	}

	hr = pRegUtil->LoadValue(strKeyName, "SideImage", m_d.m_szSideImage);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_d.m_szSideImage.clear();
	}

	m_d.m_droppable = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "Droppable", false) : false;
	m_d.m_flipbook = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "Flipbook", false) : false;
	m_d.m_isBottomSolid = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "IsBottomSolid", true) : false;

	m_d.m_heightbottom = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "HeightBottom", 0.0f) : 0.0f;
	m_d.m_heighttop = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "HeightTop", 50.0f) : 50.0f;

	m_d.m_displayTexture = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "DisplayTexture", false) : false;
	m_d.m_slingshotforce = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "SlingshotForce", 80.0f) : 80.0f;
	m_d.m_slingshotAnimation = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "SlingshotAnimation", true) : true;

	SetDefaultPhysics(fromMouseClick);

	m_d.m_topBottomVisible = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "Visible", true) : true;
	m_d.m_sideVisible = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "SideVisible", true) : true;
	m_d.m_collidable = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "Collidable", true) : true;
	m_d.m_disableLightingTop = dequantizeUnsigned<8>(fromMouseClick ? pRegUtil->LoadValueIntWithDefault(strKeyName, "DisableLighting", 0) : 0);
	m_d.m_disableLightingBelow = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "DisableLightingBelow", 0.f) : 0.f;
	m_d.m_reflectionEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "ReflectionEnabled", true) : true;
}

void Surface::SetDefaultPhysics(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	static const char strKeyName[] = "DefaultProps\\Wall";
	m_d.m_elasticity = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "Elasticity", 0.3f) : 0.3f;
	m_d.m_elasticityFalloff = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "ElasticityFallOff", 0.0f) : 0.0f;
	m_d.m_friction = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "Friction", 0.3f) : 0.3f;
	m_d.m_scatter = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "Scatter", 0) : 0;
}

bool Surface::LoadToken(const int id, BiffReader* const pBiffReader)
{
	switch (id)
	{
	case FID(PIID):
		pBiffReader->GetInt((int*)pBiffReader->m_pData);
		break;
	case FID(HTEV):
		pBiffReader->GetBool(m_d.m_hitEvent);
		break;
	case FID(DROP):
		pBiffReader->GetBool(m_d.m_droppable);
		break;
	case FID(FLIP):
		pBiffReader->GetBool(m_d.m_flipbook);
		break;
	case FID(ISBS):
		pBiffReader->GetBool(m_d.m_isBottomSolid);
		break;
	case FID(CLDW):
		pBiffReader->GetBool(m_d.m_collidable);
		break;
	case FID(TMON):
		pBiffReader->GetBool(m_d.m_tdr.m_TimerEnabled);
		break;
	case FID(TMIN):
		pBiffReader->GetInt(m_d.m_tdr.m_TimerInterval);
		break;
	case FID(THRS):
		pBiffReader->GetFloat(m_d.m_threshold);
		break;
	case FID(IMAG):
		pBiffReader->GetString(m_d.m_szImage);
		break;
	case FID(SIMG):
		pBiffReader->GetString(m_d.m_szSideImage);
		break;
	case FID(SIMA):
		pBiffReader->GetString(m_d.m_szSideMaterial);
		break;
	case FID(TOMA):
		pBiffReader->GetString(m_d.m_szTopMaterial);
		break;
	case FID(MAPH):
		pBiffReader->GetString(m_d.m_szPhysicsMaterial);
		break;
	case FID(SLMA):
		pBiffReader->GetString(m_d.m_szSlingShotMaterial);
		break;
	case FID(HTBT):
		pBiffReader->GetFloat(m_d.m_heightbottom);
		break;
	case FID(HTTP):
		pBiffReader->GetFloat(m_d.m_heighttop);
		break;
	case FID(INNR):
		pBiffReader->GetBool(m_d.m_inner);
		break; //!! Deprecated, do not use anymore
	case FID(NAME):
		pBiffReader->GetWideString(m_wzName, sizeof(m_wzName) / sizeof(wchar_t));
		break;
	case FID(DSPT):
		pBiffReader->GetBool(m_d.m_displayTexture);
		break;
	case FID(SLGF):
		pBiffReader->GetFloat(m_d.m_slingshotforce);
		break;
	case FID(SLTH):
		pBiffReader->GetFloat(m_d.m_slingshot_threshold);
		break;
	case FID(ELAS):
		pBiffReader->GetFloat(m_d.m_elasticity);
		break;
	case FID(ELFO):
		pBiffReader->GetFloat(m_d.m_elasticityFalloff);
		break;
	case FID(WFCT):
		pBiffReader->GetFloat(m_d.m_friction);
		break;
	case FID(WSCT):
		pBiffReader->GetFloat(m_d.m_scatter);
		break;
	case FID(VSBL):
		pBiffReader->GetBool(m_d.m_topBottomVisible);
		break;
	case FID(OVPH):
		pBiffReader->GetBool(m_d.m_overwritePhysics);
		break;
	case FID(SLGA):
		pBiffReader->GetBool(m_d.m_slingshotAnimation);
		break;
	case FID(DILI):
	{
		int tmp;
		pBiffReader->GetInt(tmp);
		m_d.m_disableLightingTop = (tmp == 1) ? 1.f : dequantizeUnsigned<8>(tmp);
		break;
	}
	case FID(DILB):
		pBiffReader->GetFloat(m_d.m_disableLightingBelow);
		break;
	case FID(SVBL):
		pBiffReader->GetBool(m_d.m_sideVisible);
		break;
	case FID(REEN):
		pBiffReader->GetBool(m_d.m_reflectionEnabled);
		break;
	default:
	{
		LoadPointToken(id, pBiffReader, pBiffReader->m_version);
		ISelect::LoadToken(id, pBiffReader);
		break;
	}
	}
	return true;
}

ItemTypeEnum Surface::GetItemType() const
{
	return eItemSurface;
}

IEditable* Surface::GetIEditable()
{
	return static_cast<IEditable*>(this);
}

IHitable* Surface::GetIHitable()
{
	return static_cast<IHitable*>(this);
}

void Surface::WriteRegDefaults()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	const char* strKeyName = m_isWall ? "DefaultProps\\Wall" : "DefaultProps\\Target";

	pRegUtil->SaveValueBool(strKeyName, "TimerEnabled", m_d.m_tdr.m_TimerEnabled);
	pRegUtil->SaveValueInt(strKeyName, "TimerInterval", m_d.m_tdr.m_TimerInterval);
	pRegUtil->SaveValueBool(strKeyName, "HitEvent", m_d.m_hitEvent);
	pRegUtil->SaveValueFloat(strKeyName, "HitThreshold", m_d.m_threshold);
	pRegUtil->SaveValueFloat(strKeyName, "SlingshotThreshold", m_d.m_slingshot_threshold);
	pRegUtil->SaveValue(strKeyName, "TopImage", m_d.m_szImage);
	pRegUtil->SaveValue(strKeyName, "SideImage", m_d.m_szSideImage);
	pRegUtil->SaveValueBool(strKeyName, "Droppable", m_d.m_droppable);
	pRegUtil->SaveValueBool(strKeyName, "Flipbook", m_d.m_flipbook);
	pRegUtil->SaveValueBool(strKeyName, "IsBottomSolid", m_d.m_isBottomSolid);
	pRegUtil->SaveValueFloat(strKeyName, "HeightBottom", m_d.m_heightbottom);
	pRegUtil->SaveValueFloat(strKeyName, "HeightTop", m_d.m_heighttop);
	pRegUtil->SaveValueBool(strKeyName, "DisplayTexture", m_d.m_displayTexture);
	pRegUtil->SaveValueFloat(strKeyName, "SlingshotForce", m_d.m_slingshotforce);
	pRegUtil->SaveValueBool(strKeyName, "SlingshotAnimation", m_d.m_slingshotAnimation);
	pRegUtil->SaveValueFloat(strKeyName, "Elasticity", m_d.m_elasticity);
	pRegUtil->SaveValueFloat(strKeyName, "ElasticityFallOff", m_d.m_elasticityFalloff);
	pRegUtil->SaveValueFloat(strKeyName, "Friction", m_d.m_friction);
	pRegUtil->SaveValueFloat(strKeyName, "Scatter", m_d.m_scatter);
	pRegUtil->SaveValueBool(strKeyName, "Visible", m_d.m_topBottomVisible);
	pRegUtil->SaveValueBool(strKeyName, "SideVisible", m_d.m_sideVisible);
	pRegUtil->SaveValueBool(strKeyName, "Collidable", m_d.m_collidable);
	const int tmp = quantizeUnsigned<8>(clamp(m_d.m_disableLightingTop, 0.f, 1.f));
	pRegUtil->SaveValueInt(strKeyName, "DisableLighting", (tmp == 1) ? 0 : tmp); // backwards compatible saving
	pRegUtil->SaveValueFloat(strKeyName, "DisableLightingBelow", m_d.m_disableLightingBelow);
	pRegUtil->SaveValueBool(strKeyName, "ReflectionEnabled", m_d.m_reflectionEnabled);
}

void Surface::GetHitShapes(std::vector<HitObject*>& pvho)
{
}

void Surface::GetHitShapesDebug(std::vector<HitObject*>& pvho)
{
}

//
// license:GPLv3+
// Ported at: VisualPinball.Engine/VPT/Surface/SurfaceMeshGenerator.cs
//

void Surface::GenerateMesh(std::vector<Vertex3D_NoTex2>& topBuf, std::vector<Vertex3D_NoTex2>& sideBuf, std::vector<WORD>& topBottomIndices, std::vector<WORD>& sideIndices)
{
	std::vector<RenderVertex> vvertex;
	GetRgVertex(vvertex);
	float* rgtexcoord = NULL;

	Texture* const pinSide = m_ptable->GetImage(m_d.m_szSideImage);
	if (pinSide)
	{
		GetTextureCoords(vvertex, &rgtexcoord);
	}

	m_numVertices = (unsigned int)vvertex.size();
	Vertex2D* const rgnormal = new Vertex2D[m_numVertices];

	for (unsigned int i = 0; i < m_numVertices; i++)
	{
		const RenderVertex* const pv1 = &vvertex[i];
		const RenderVertex* const pv2 = &vvertex[(i < m_numVertices - 1) ? (i + 1) : 0];
		const float dx = pv1->x - pv2->x;
		const float dy = pv1->y - pv2->y;

		const float inv_len = 1.0f / sqrtf(dx * dx + dy * dy);

		rgnormal[i].x = dy * inv_len;
		rgnormal[i].y = dx * inv_len;
	}

	sideBuf.resize(m_numVertices * 4);
	memset(sideBuf.data(), 0, sizeof(Vertex3D_NoTex2) * m_numVertices * 4);
	Vertex3D_NoTex2* verts = sideBuf.data();

	const float bottom = m_d.m_heightbottom + m_ptable->m_tableheight;
	const float top = m_d.m_heighttop + m_ptable->m_tableheight;

	int offset = 0;
	for (unsigned int i = 0; i < m_numVertices; i++, offset += 4)
	{
		const RenderVertex* const pv1 = &vvertex[i];
		const RenderVertex* const pv2 = &vvertex[(i < m_numVertices - 1) ? (i + 1) : 0];

		const int a = (i == 0) ? (m_numVertices - 1) : (i - 1);
		const int c = (i < m_numVertices - 1) ? (i + 1) : 0;

		Vertex2D vnormal[2];
		if (pv1->smooth)
		{
			vnormal[0].x = (rgnormal[a].x + rgnormal[i].x) * 0.5f;
			vnormal[0].y = (rgnormal[a].y + rgnormal[i].y) * 0.5f;
		}
		else
		{
			vnormal[0].x = rgnormal[i].x;
			vnormal[0].y = rgnormal[i].y;
		}

		if (pv2->smooth)
		{
			vnormal[1].x = (rgnormal[i].x + rgnormal[c].x) * 0.5f;
			vnormal[1].y = (rgnormal[i].y + rgnormal[c].y) * 0.5f;
		}
		else
		{
			vnormal[1].x = rgnormal[i].x;
			vnormal[1].y = rgnormal[i].y;
		}

		vnormal[0].Normalize();
		vnormal[1].Normalize();

		{
			verts[offset].x = pv1->x;
			verts[offset].y = pv1->y;
			verts[offset].z = bottom;
			verts[offset + 1].x = pv1->x;
			verts[offset + 1].y = pv1->y;
			verts[offset + 1].z = top;
			verts[offset + 2].x = pv2->x;
			verts[offset + 2].y = pv2->y;
			verts[offset + 2].z = top;
			verts[offset + 3].x = pv2->x;
			verts[offset + 3].y = pv2->y;
			verts[offset + 3].z = bottom;
			if (pinSide)
			{
				verts[offset].tu = rgtexcoord[i];
				verts[offset].tv = 1.0f;

				verts[offset + 1].tu = rgtexcoord[i];
				verts[offset + 1].tv = 0;

				verts[offset + 2].tu = rgtexcoord[c];
				verts[offset + 2].tv = 0;

				verts[offset + 3].tu = rgtexcoord[c];
				verts[offset + 3].tv = 1.0f;
			}

			verts[offset].nx = vnormal[0].x;
			verts[offset].ny = -vnormal[0].y;
			verts[offset].nz = 0;

			verts[offset + 1].nx = vnormal[0].x;
			verts[offset + 1].ny = -vnormal[0].y;
			verts[offset + 1].nz = 0;

			verts[offset + 2].nx = vnormal[1].x;
			verts[offset + 2].ny = -vnormal[1].y;
			verts[offset + 2].nz = 0;

			verts[offset + 3].nx = vnormal[1].x;
			verts[offset + 3].ny = -vnormal[1].y;
			verts[offset + 3].nz = 0;
		}
	}
	delete[] rgnormal;

	{
		sideIndices.resize(m_numVertices * 6);
		int offset2 = 0;
		for (unsigned int i = 0; i < m_numVertices; i++, offset2 += 4)
		{
			sideIndices[i * 6] = offset2;
			sideIndices[i * 6 + 1] = offset2 + 1;
			sideIndices[i * 6 + 2] = offset2 + 2;
			sideIndices[i * 6 + 3] = offset2;
			sideIndices[i * 6 + 4] = offset2 + 2;
			sideIndices[i * 6 + 5] = offset2 + 3;
		}
	}

	if (rgtexcoord)
	{
		delete[] rgtexcoord;
		rgtexcoord = NULL;
	}

	{
		topBottomIndices.clear();

		{
			std::vector<unsigned int> vpoly(m_numVertices);
			for (unsigned int i = 0; i < m_numVertices; i++)
			{
				vpoly[i] = i;
			}

			PolygonToTriangles(vvertex, vpoly, topBottomIndices, false);
		}

		m_numPolys = (unsigned int)(topBottomIndices.size() / 3);
		if (m_numPolys == 0)
		{
			return;
		}

		const float heightNotDropped = m_d.m_heighttop;
		const float heightDropped = m_d.m_heightbottom + 0.1f;

		const float inv_tablewidth = 1.0f / (m_ptable->m_right - m_ptable->m_left);
		const float inv_tableheight = 1.0f / (m_ptable->m_bottom - m_ptable->m_top);

		topBuf.resize(m_numVertices * 3);
		Vertex3D_NoTex2* const vertsTop[3] = {&topBuf[0], &topBuf[m_numVertices], &topBuf[m_numVertices * 2]};

		for (unsigned int i = 0; i < m_numVertices; i++)
		{
			const RenderVertex* const pv0 = &vvertex[i];

			vertsTop[0][i].x = pv0->x;
			vertsTop[0][i].y = pv0->y;
			vertsTop[0][i].z = heightNotDropped + m_ptable->m_tableheight;
			vertsTop[0][i].tu = pv0->x * inv_tablewidth;
			vertsTop[0][i].tv = pv0->y * inv_tableheight;
			vertsTop[0][i].nx = 0;
			vertsTop[0][i].ny = 0;
			vertsTop[0][i].nz = 1.0f;

			vertsTop[1][i].x = pv0->x;
			vertsTop[1][i].y = pv0->y;
			vertsTop[1][i].z = heightDropped;
			vertsTop[1][i].tu = pv0->x * inv_tablewidth;
			vertsTop[1][i].tv = pv0->y * inv_tableheight;
			vertsTop[1][i].nx = 0;
			vertsTop[1][i].ny = 0;
			vertsTop[1][i].nz = 1.0f;

			vertsTop[2][i].x = pv0->x;
			vertsTop[2][i].y = pv0->y;
			vertsTop[2][i].z = m_d.m_heightbottom;
			vertsTop[2][i].tu = pv0->x * inv_tablewidth;
			vertsTop[2][i].tv = pv0->y * inv_tableheight;
			vertsTop[2][i].nx = 0;
			vertsTop[2][i].ny = 0;
			vertsTop[2][i].nz = -1.0f;
		}
	}
}

ItemTypeEnum Surface::HitableGetItemType() const
{
	return eItemSurface;
}

void Surface::GetBoundingVertices(std::vector<Vertex3Ds>& pvvertex3D)
{
	for (int i = 0; i < 8; i++)
	{
		const Vertex3Ds pv(
			(i & 1) ? m_ptable->m_right : m_ptable->m_left,
			(i & 2) ? m_ptable->m_bottom : m_ptable->m_top,
			(i & 4) ? m_d.m_heighttop : m_d.m_heightbottom);

		pvvertex3D.push_back(pv);
	}
}

//
// end of license:GPLv3+, back to 'old MAME'-like
//

void Surface::PrepareWallsAtHeight()
{
	if (m_IBuffer)
	{
		m_IBuffer->release();
	}
	if (m_VBuffer)
	{
		m_VBuffer->release();
	}

	std::vector<Vertex3D_NoTex2> topBottomBuf;
	std::vector<Vertex3D_NoTex2> sideBuf;
	std::vector<WORD> topBottomIndices;
	std::vector<WORD> sideIndices;
	GenerateMesh(topBottomBuf, sideBuf, topBottomIndices, sideIndices);

	RenderDevice* const pd3dDevice = g_pplayer->m_pin3d.m_pd3dPrimaryDevice;

	pd3dDevice->CreateVertexBuffer(m_numVertices * 4 + (!topBottomBuf.empty() ? m_numVertices * 3 : 0), 0, MY_D3DFVF_NOTEX2_VERTEX, &m_VBuffer);

	// Vertex3D_NoTex2* verts;
	// m_VBuffer->lock(0, 0, (void**)&verts, VertexBuffer::WRITEONLY);
	// memcpy(verts, sideBuf.data(), sizeof(Vertex3D_NoTex2) * m_numVertices * 4);

	m_VBuffer->CopyMemory(sideBuf.data(), 0, sizeof(Vertex3D_NoTex2) * m_numVertices * 4);

	if (!topBottomBuf.empty())
	{
		// memcpy(verts + m_numVertices * 4, topBottomBuf.data(), sizeof(Vertex3D_NoTex2) * m_numVertices * 3);

		m_VBuffer->CopyMemory(topBottomBuf.data(), (m_numVertices * 4), (m_numVertices * 3) * sizeof(Vertex3D_NoTex2));
	}

	// m_VBuffer->unlock();

	pd3dDevice->CreateIndexBuffer((unsigned int)topBottomIndices.size() + (unsigned int)sideIndices.size(), 0, IndexBuffer::FMT_INDEX16, &m_IBuffer);

	// WORD* buf;
	// m_IBuffer->lock(0, 0, (void**)&buf, 0);
	// memcpy(buf, sideIndices.data(), sideIndices.size() * sizeof(WORD));

	m_IBuffer->CopyMemory(sideIndices.data(), 0, sideIndices.size() * sizeof(WORD));

	if (!topBottomIndices.empty())
	{
		// memcpy(buf + sideIndices.size(), topBottomIndices.data(), topBottomIndices.size() * sizeof(WORD));

		m_IBuffer->CopyMemory(topBottomIndices.data(), sideIndices.size(), topBottomIndices.size() * sizeof(WORD));
	}

	// m_IBuffer->unlock();
}

void Surface::PrepareSlingshots()
{
	const float slingbottom = (m_d.m_heighttop - m_d.m_heightbottom) * 0.2f + m_d.m_heightbottom;
	const float slingtop = (m_d.m_heighttop - m_d.m_heightbottom) * 0.8f + m_d.m_heightbottom;

	Vertex3D_NoTex2* const rgv3D = new Vertex3D_NoTex2[m_vlinesling.size() * 9];

	unsigned int offset = 0;
	for (size_t i = 0; i < m_vlinesling.size(); i++, offset += 9)
	{
		LineSegSlingshot* const plinesling = m_vlinesling[i];
		plinesling->m_slingshotanim.m_animations = (m_d.m_slingshotAnimation != 0);

		rgv3D[offset].x = plinesling->v1.x;
		rgv3D[offset].y = plinesling->v1.y;
		rgv3D[offset].z = slingbottom + m_ptable->m_tableheight;

		rgv3D[offset + 1].x = (plinesling->v1.x + plinesling->v2.x) * 0.5f + plinesling->normal.x * (m_d.m_slingshotforce * 0.25f);
		rgv3D[offset + 1].y = (plinesling->v1.y + plinesling->v2.y) * 0.5f + plinesling->normal.y * (m_d.m_slingshotforce * 0.25f);
		rgv3D[offset + 1].z = slingbottom + m_ptable->m_tableheight;

		rgv3D[offset + 2].x = plinesling->v2.x;
		rgv3D[offset + 2].y = plinesling->v2.y;
		rgv3D[offset + 2].z = slingbottom + m_ptable->m_tableheight;

		for (unsigned int l = 0; l < 3; l++)
		{
			rgv3D[l + offset + 3].x = rgv3D[l + offset].x;
			rgv3D[l + offset + 3].y = rgv3D[l + offset].y;
			rgv3D[l + offset + 3].z = slingtop + m_ptable->m_tableheight;
		}

		for (unsigned int l = 0; l < 3; l++)
		{
			rgv3D[l + offset + 6].x = rgv3D[l + offset].x - plinesling->normal.x * 5.0f;
			rgv3D[l + offset + 6].y = rgv3D[l + offset].y - plinesling->normal.y * 5.0f;
			rgv3D[l + offset + 6].z = slingtop + m_ptable->m_tableheight;
		}

		ComputeNormals(rgv3D + offset, 9, rgiSlingshot, 24);
	}

	// TODO: if (m_slingshotVBuffer)
	// {
	// 		m_slingshotVBuffer->release();
	// }

	RenderDevice* const pd3dDevice = g_pplayer->m_pin3d.m_pd3dPrimaryDevice;

	pd3dDevice->CreateVertexBuffer((unsigned int)m_vlinesling.size() * 9, 0, MY_D3DFVF_NOTEX2_VERTEX, &m_slingshotVBuffer);

	// Vertex3D_NoTex2* buf;
	// m_slingshotVBuffer->lock(0, 0, (void**)&buf, VertexBuffer::WRITEONLY);
	// memcpy(buf, rgv3D, m_vlinesling.size() * 9 * sizeof(Vertex3D_NoTex2));

	m_slingshotVBuffer->CopyMemory(rgv3D, 0, m_vlinesling.size() * 9 * sizeof(Vertex3D_NoTex2));

	// m_slingshotVBuffer->unlock();

	delete[] rgv3D;

	if (!slingIBuffer)
	{
		slingIBuffer = pd3dDevice->CreateAndFillIndexBuffer(24, rgiSlingshot);
	}
}

void Surface::RenderSetup()
{
	const float oldBottomHeight = m_d.m_heightbottom;
	const float oldTopHeight = m_d.m_heighttop;

	m_d.m_heightbottom *= m_ptable->m_BG_scalez[m_ptable->m_BG_current_set];
	m_d.m_heighttop *= m_ptable->m_BG_scalez[m_ptable->m_BG_current_set];

	if (!m_vlinesling.empty())
	{
		PrepareSlingshots();
	}

	m_isDynamic = false;
	if (m_d.m_sideVisible)
	{
		if (m_ptable->GetMaterial(m_d.m_szSideMaterial)->m_bOpacityActive)
		{
			m_isDynamic = true;
		}
	}
	if (m_d.m_topBottomVisible)
	{
		if (m_ptable->GetMaterial(m_d.m_szTopMaterial)->m_bOpacityActive)
		{
			m_isDynamic = true;
		}
	}

	PrepareWallsAtHeight();
	m_d.m_heightbottom = oldBottomHeight;
	m_d.m_heighttop = oldTopHeight;
}

void Surface::RenderStatic()
{
	if (m_ptable->m_reflectionEnabled && !m_d.m_reflectionEnabled)
	{
		return;
	}

	RenderSlingshots();

	if (!m_d.m_droppable && !m_isDynamic)
	{
		RenderWallsAtHeight(false);
	}
}

void Surface::RenderSlingshots()
{
	if (!m_d.m_sideVisible || m_vlinesling.empty())
	{
		return;
	}

	bool nothing_to_draw = true;

	for (size_t i = 0; i < m_vlinesling.size(); i++)
	{
		LineSegSlingshot* const plinesling = m_vlinesling[i];
		if (plinesling->m_slingshotanim.m_iframe || plinesling->m_doHitEvent)
		{
			nothing_to_draw = false;
			break;
		}
	}

	if (nothing_to_draw)
	{
		return;
	}

	RenderDevice* const pd3dDevice = g_pplayer->m_pin3d.m_pd3dPrimaryDevice;

	const Material* const mat = m_ptable->GetMaterial(m_d.m_szSlingShotMaterial);
	pd3dDevice->basicShader->SetTechnique(mat->m_bIsMetal ? "basic_without_texture_isMetal" : "basic_without_texture_isNotMetal");
	pd3dDevice->basicShader->SetMaterial(mat);

	pd3dDevice->SetRenderState(RenderDevice::DEPTHBIAS, 0);
	pd3dDevice->SetRenderState(RenderDevice::ZWRITEENABLE, RenderDevice::RS_TRUE);
	pd3dDevice->SetRenderState(RenderDevice::CULLMODE, RenderDevice::CULL_NONE);

	for (size_t i = 0; i < m_vlinesling.size(); i++)
	{
		LineSegSlingshot* const plinesling = m_vlinesling[i];
		if (!plinesling->m_slingshotanim.m_iframe && !plinesling->m_doHitEvent)
		{
			continue;
		}
		else if (plinesling->m_doHitEvent)
		{
			if (plinesling->m_EventTimeReset == 0)
			{
				plinesling->m_EventTimeReset = g_pplayer->m_time_msec + 100;
			}
			else if (plinesling->m_EventTimeReset < g_pplayer->m_time_msec)
			{
				plinesling->m_doHitEvent = false;
				plinesling->m_EventTimeReset = 0;
			}
		}

		pd3dDevice->basicShader->Begin(0);

		pd3dDevice->DrawIndexedPrimitiveVB(RenderDevice::TRIANGLELIST,
										   MY_D3DFVF_NOTEX2_VERTEX,
										   m_slingshotVBuffer, (DWORD)i * 9, 9, slingIBuffer, 0, 24);

		pd3dDevice->basicShader->End();
	}
}

void Surface::RenderWallsAtHeight(const bool drop)
{
	if (m_ptable->m_reflectionEnabled && (m_d.m_heighttop < 0.0f))
	{
		return;
	}

	RenderDevice* const pd3dDevice = g_pplayer->m_pin3d.m_pd3dPrimaryDevice;

	if ((m_d.m_disableLightingTop != 0.f || m_d.m_disableLightingBelow != 0.f) && (m_d.m_sideVisible || m_d.m_topBottomVisible))
	{
		const vec4 tmp(m_d.m_disableLightingTop, m_d.m_disableLightingBelow, 0.f, 0.f);
		pd3dDevice->basicShader->SetDisableLighting(tmp);
	}

	if (m_d.m_sideVisible && !drop && (m_numVertices > 0))
	{
		const Material* const mat = m_ptable->GetMaterial(m_d.m_szSideMaterial);
		pd3dDevice->basicShader->SetMaterial(mat);

		pd3dDevice->SetRenderState(RenderDevice::DEPTHBIAS, 0);
		pd3dDevice->SetRenderState(RenderDevice::ZWRITEENABLE, RenderDevice::RS_TRUE);

		if (mat->m_bOpacityActive || !m_isDynamic)
		{
			pd3dDevice->SetRenderState(RenderDevice::CULLMODE, RenderDevice::CULL_NONE);
		}
		else
		{
			if (m_d.m_topBottomVisible && m_isDynamic)
			{
				pd3dDevice->SetRenderState(RenderDevice::CULLMODE, RenderDevice::CULL_NONE);
			}
			else
			{
				pd3dDevice->SetRenderState(RenderDevice::CULLMODE, RenderDevice::CULL_CCW);
			}
		}
		Texture* const pinSide = m_ptable->GetImage(m_d.m_szSideImage);
		if (pinSide)
		{
			pd3dDevice->basicShader->SetTechnique(mat->m_bIsMetal ? "basic_with_texture_isMetal" : "basic_with_texture_isNotMetal");
			pd3dDevice->basicShader->SetTexture("Texture0", pinSide, false);
			pd3dDevice->basicShader->SetAlphaTestValue(pinSide->m_alphaTestValue * (float)(1.0 / 255.0));
		}
		else
		{
			pd3dDevice->basicShader->SetTechnique(mat->m_bIsMetal ? "basic_without_texture_isMetal" : "basic_without_texture_isNotMetal");
		}

		pd3dDevice->basicShader->Begin(0);
		pd3dDevice->DrawIndexedPrimitiveVB(RenderDevice::TRIANGLELIST, MY_D3DFVF_NOTEX2_VERTEX, m_VBuffer, 0, m_numVertices * 4, m_IBuffer, 0, m_numVertices * 6);
		pd3dDevice->basicShader->End();
	}

	if (m_d.m_topBottomVisible && (m_numPolys > 0))
	{
		const Material* const mat = m_ptable->GetMaterial(m_d.m_szTopMaterial);
		pd3dDevice->basicShader->SetMaterial(mat);

		pd3dDevice->SetRenderState(RenderDevice::DEPTHBIAS, 0);
		pd3dDevice->SetRenderState(RenderDevice::ZWRITEENABLE, RenderDevice::RS_TRUE);

		if (mat->m_bOpacityActive || !m_isDynamic)
		{
			pd3dDevice->SetRenderState(RenderDevice::CULLMODE, RenderDevice::CULL_NONE);
		}
		else
		{
			pd3dDevice->SetRenderState(RenderDevice::CULLMODE, RenderDevice::CULL_CCW);
		}

		Texture* const pin = m_ptable->GetImage(m_d.m_szImage);
		if (pin)
		{
			pd3dDevice->basicShader->SetTechnique(mat->m_bIsMetal ? "basic_with_texture_isMetal" : "basic_with_texture_isNotMetal");
			pd3dDevice->basicShader->SetTexture("Texture0", pin, false);
			pd3dDevice->basicShader->SetAlphaTestValue(pin->m_alphaTestValue * (float)(1.0 / 255.0));
		}
		else
		{
			pd3dDevice->basicShader->SetTechnique(mat->m_bIsMetal ? "basic_without_texture_isMetal" : "basic_without_texture_isNotMetal");
		}

		pd3dDevice->basicShader->Begin(0);
		pd3dDevice->DrawIndexedPrimitiveVB(RenderDevice::TRIANGLELIST, MY_D3DFVF_NOTEX2_VERTEX, m_VBuffer, m_numVertices * 4 + (!drop ? 0 : m_numVertices), m_numVertices, m_IBuffer, m_numVertices * 6, m_numPolys * 3);
		pd3dDevice->basicShader->End();

		if (m_ptable->m_reflectionEnabled)
		{
			if (mat->m_bOpacityActive || !m_isDynamic)
			{
				pd3dDevice->SetRenderState(RenderDevice::CULLMODE, RenderDevice::CULL_NONE);
			}
			else
			{
				pd3dDevice->SetRenderState(RenderDevice::CULLMODE, RenderDevice::CULL_CW);
			}

			pd3dDevice->basicShader->Begin(0);
			pd3dDevice->DrawIndexedPrimitiveVB(RenderDevice::TRIANGLELIST, MY_D3DFVF_NOTEX2_VERTEX, m_VBuffer, m_numVertices * 4 + m_numVertices * 2, m_numVertices, m_IBuffer, m_numVertices * 6, m_numPolys * 3);
			pd3dDevice->basicShader->End();
		}
	}

	if ((m_d.m_disableLightingTop != 0.f || m_d.m_disableLightingBelow != 0.f) && (m_d.m_sideVisible || m_d.m_topBottomVisible))
	{
		const vec4 tmp(0.f, 0.f, 0.f, 0.f);
		pd3dDevice->basicShader->SetDisableLighting(tmp);
	}
}

void Surface::RenderDynamic()
{
	if (m_ptable->m_reflectionEnabled && !m_d.m_reflectionEnabled)
	{
		return;
	}

	RenderSlingshots();

	if (m_d.m_droppable || m_isDynamic)
	{
		if (!m_isDropped)
		{
			RenderWallsAtHeight(false);
		}
		else
		{
			if (!m_d.m_flipbook)
			{
				RenderWallsAtHeight(true);
			}
		}
	}
}
