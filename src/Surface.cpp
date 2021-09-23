#include "Surface.h"
#include "RegUtil.h"

#include "Inlines.h"

const ItemTypeEnum Surface::ItemType = eItemSurface;
const int Surface::TypeNameID = 104;
const int Surface::ToolID = 110;
const int Surface::CursorID = 138;
const unsigned Surface::AllowedViews = 1;

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

HRESULT Surface::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_ptable = ptable;

	m_isWall = true;

	const float width = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Wall", "Width", 50.f) : 50.f;
	const float length = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Wall", "Length", 50.f) : 50.f;

	// CComObject<DragPoint>* pdp;
	// CComObject<DragPoint>::CreateInstance(&pdp);
	// if (pdp)
	// {
	// 	pdp->AddRef();
	// 	pdp->Init(this, x - width, y - length, 0.f, false);
	// 	m_vdpoint.push_back(pdp);
	// }
	// CComObject<DragPoint>::CreateInstance(&pdp);
	// if (pdp)
	// {
	// 	pdp->AddRef();
	// 	pdp->Init(this, x - width, y + length, 0.f, false);
	// 	m_vdpoint.push_back(pdp);
	// }
	// CComObject<DragPoint>::CreateInstance(&pdp);
	// if (pdp)
	// {
	// 	pdp->AddRef();
	// 	pdp->Init(this, x + width, y + length, 0.f, false);
	// 	m_vdpoint.push_back(pdp);
	// }
	// CComObject<DragPoint>::CreateInstance(&pdp);
	// if (pdp)
	// {
	// 	pdp->AddRef();
	// 	pdp->Init(this, x + width, y - length, 0.f, false);
	// 	m_vdpoint.push_back(pdp);
	// }

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
		// TODO: const size_t cvertex = m_vdpoint.size();

		// float miny = FLT_MAX;
		// size_t minyindex = 0;

		// // Find smallest y point - use it to connect with surrounding border
		// for (size_t i = 0; i < cvertex; i++)
		// {
		// 	float y;
		// 	m_vdpoint[i]->get_Y(&y);
		// 	if (y < miny)
		// 	{
		// 		miny = y;
		// 		minyindex = i;
		// 	}
		// }

		// float tmpx;
		// m_vdpoint[minyindex]->get_X(&tmpx);
		// const float tmpy = miny /*- 1.0f*/; // put tiny gap in to avoid errors

		// // swap list around
		// std::reverse(m_vdpoint.begin(), m_vdpoint.end());

		// CComObject<DragPoint>* pdp;
		// CComObject<DragPoint>::CreateInstance(&pdp);
		// if (pdp)
		// {
		// 	pdp->AddRef();
		// 	pdp->Init(this, m_ptable->m_left, m_ptable->m_top, 0.f, false);
		// 	m_vdpoint.insert(m_vdpoint.begin() + (cvertex - minyindex - 1), pdp);
		// }
		// CComObject<DragPoint>::CreateInstance(&pdp);
		// if (pdp)
		// {
		// 	pdp->AddRef();
		// 	pdp->Init(this, m_ptable->m_right, m_ptable->m_top, 0.f, false);
		// 	m_vdpoint.insert(m_vdpoint.begin() + (cvertex - minyindex - 1), pdp);
		// }
		// CComObject<DragPoint>::CreateInstance(&pdp);
		// if (pdp)
		// {
		// 	pdp->AddRef();
		// 	pdp->Init(this, m_ptable->m_right + 1.0f, m_ptable->m_bottom, 0.f, false); //!!! +1 needed for whatever reason (triangulation screwed up)
		// 	m_vdpoint.insert(m_vdpoint.begin() + (cvertex - minyindex - 1), pdp);
		// }
		// CComObject<DragPoint>::CreateInstance(&pdp);
		// if (pdp)
		// {
		// 	pdp->AddRef();
		// 	pdp->Init(this, m_ptable->m_left, m_ptable->m_bottom, 0.f, false);
		// 	m_vdpoint.insert(m_vdpoint.begin() + (cvertex - minyindex - 1), pdp);
		// }
		// CComObject<DragPoint>::CreateInstance(&pdp);
		// if (pdp)
		// {
		// 	pdp->AddRef();
		// 	pdp->Init(this, m_ptable->m_left - 1.0f, m_ptable->m_top, 0.f, false); //!!! -1 needed for whatever reason (triangulation screwed up)
		// 	m_vdpoint.insert(m_vdpoint.begin() + (cvertex - minyindex - 1), pdp);
		// }
		// CComObject<DragPoint>::CreateInstance(&pdp);
		// if (pdp)
		// {
		// 	pdp->AddRef();
		// 	pdp->Init(this, tmpx, tmpy, 0.f, false);
		// 	m_vdpoint.insert(m_vdpoint.begin() + (cvertex - minyindex - 1), pdp);
		// }

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
	m_d.m_disableLightingTop = dequantizeUnsigned<8>(fromMouseClick ? pRegUtil->LoadValueIntWithDefault(strKeyName, "DisableLighting", 0) : 0); // stored as uchar for backward compatibility
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

void Surface::RenderStatic()
{
}

void Surface::RenderDynamic()
{
}

void Surface::RenderSetup()
{
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
