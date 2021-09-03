#include "Rubber.h"
#include "RegUtil.h"

const ItemTypeEnum Rubber::ItemType = eItemRubber;
const int Rubber::TypeNameID = 147;
const int Rubber::ToolID = 32990;
const int Rubber::CursorID = 602;
const unsigned Rubber::AllowedViews = 1;

Rubber* Rubber::COMCreate()
{
	/*CComObject<Rubber>* obj = 0;
	if ((((HRESULT)(CComObject<Rubber>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Rubber();
}

IEditable* Rubber::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Rubber::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Rubber* obj = Rubber::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Rubber::Rubber()
{
	// TODO: m_menuid = IDR_SURFACEMENU;
	m_d.m_collidable = true;
	m_d.m_visible = true;
	m_d.m_hitEvent = false;
	m_dynamicVertexBuffer = 0;
	m_dynamicIndexBuffer = 0;
	m_dynamicVertexBufferRegenerate = true;
	// TODO: m_propPhysics = NULL;
	// TODO: m_propPosition = NULL;
	// TODO: m_propVisual = NULL;
	m_d.m_overwritePhysics = true;
	m_ptable = NULL;
	m_d.m_tdr.m_TimerEnabled = false;
	m_d.m_tdr.m_TimerInterval = 0;
}

Rubber::~Rubber()
{
}

HRESULT Rubber::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;
	m_d.m_visible = true;

	for (int i = 8; i > 0; i--)
	{
		const float angle = (float)(M_PI * 2.0 / 8.0) * (float)i;
		const float xx = x + sinf(angle) * 50.0f;
		const float yy = y - cosf(angle) * 50.0f;
		// 	CComObject<DragPoint>* pdp;
		// 	CComObject<DragPoint>::CreateInstance(&pdp);
		// 	if (pdp)
		// 	{
		// 		pdp->AddRef();
		// 		pdp->Init(this, xx, yy, 0.f, true);
		// 		m_vdpoint.push_back(pdp);
		// 	}
	}

	SetDefaults(fromMouseClick);

	InitVBA(true, 0, NULL);

	return S_OK;
}

HRESULT Rubber::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			GetPTable()->GetUniqueName(eItemRubber, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t*)m_wzName,
			//             sizeof(m_wzName) / sizeof(m_wzName[0]));
		}
	}
	InitScript();
	return ((HRESULT)0L);
}

PinTable* Rubber::GetPTable()
{
	return m_ptable;
}

HRESULT Rubber::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	SetDefaults(false);

	m_d.m_hitHeight = -1.0f;

	m_ptable = pTable;

	BiffReader biffReader(pStream, this, pId, version);
	biffReader.Load();

	return S_OK;
}

void Rubber::SetDefaults(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	static const char strKeyName[] = "DefaultProps\\Rubber";

	m_d.m_height = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "Height", 25.0f) : 25.0f;
	m_d.m_thickness = fromMouseClick ? pRegUtil->LoadValueIntWithDefault(strKeyName, "Thickness", 8) : 8;

	m_d.m_tdr.m_TimerEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "TimerEnabled", false) : false;
	m_d.m_tdr.m_TimerInterval = fromMouseClick ? pRegUtil->LoadValueIntWithDefault(strKeyName, "TimerInterval", 100) : 100;

	const HRESULT hr = pRegUtil->LoadValue(strKeyName, "Image", m_d.m_szImage);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_d.m_szImage.clear();
	}

	m_d.m_hitEvent = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "HitEvent", false) : false;

	SetDefaultPhysics(fromMouseClick);

	m_d.m_visible = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "Visible", true) : true;
	m_d.m_collidable = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "Collidable", true) : true;

	m_d.m_staticRendering = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "EnableStaticRendering", true) : true;
	m_d.m_showInEditor = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "EnableShowInEditor", false) : false;

	m_d.m_rotX = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "RotX", 0.0f) : 0.0f;
	m_d.m_rotY = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "RotY", 0.0f) : 0.0f;
	m_d.m_rotZ = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "RotZ", 0.0f) : 0.0f;

	m_d.m_reflectionEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "ReflectionEnabled", true) : true;
}

void Rubber::SetDefaultPhysics(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	static const char strKeyName[] = "DefaultProps\\Rubber";

	m_d.m_elasticity = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "Elasticity", 0.8f) : 0.8f;
	m_d.m_elasticityFalloff = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "ElasticityFalloff", 0.3f) : 0.3f;
	m_d.m_friction = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "Friction", 0.6f) : 0.6f;
	m_d.m_scatter = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "Scatter", 5) : 5;
	m_d.m_hitHeight = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "HitHeight", 25.0f) : 25.0f;
}

bool Rubber::LoadToken(const int id, BiffReader* const pBiffReader)
{
	switch (id)
	{
	case FID(PIID):
		pBiffReader->GetInt((int*)pBiffReader->m_pData);
		break;
	case FID(HTTP):
		pBiffReader->GetFloat(m_d.m_height);
		break;
	case FID(HTHI):
		pBiffReader->GetFloat(m_d.m_hitHeight);
		break;
	case FID(WDTP):
		pBiffReader->GetInt(m_d.m_thickness);
		break;
	case FID(HTEV):
		pBiffReader->GetBool(m_d.m_hitEvent);
		break;
	case FID(MATR):
		pBiffReader->GetString(m_d.m_szMaterial);
		break;
	case FID(TMON):
		pBiffReader->GetBool(m_d.m_tdr.m_TimerEnabled);
		break;
	case FID(TMIN):
		pBiffReader->GetInt(m_d.m_tdr.m_TimerInterval);
		break;
	case FID(IMAG):
		pBiffReader->GetString(m_d.m_szImage);
		break;
	case FID(NAME):
		pBiffReader->GetWideString(m_wzName, sizeof(m_wzName) / sizeof(wchar_t));
		break;
	case FID(ELAS):
		pBiffReader->GetFloat(m_d.m_elasticity);
		break;
	case FID(ELFO):
		pBiffReader->GetFloat(m_d.m_elasticityFalloff);
		break;
	case FID(RFCT):
		pBiffReader->GetFloat(m_d.m_friction);
		break;
	case FID(RSCT):
		pBiffReader->GetFloat(m_d.m_scatter);
		break;
	case FID(CLDR):
		pBiffReader->GetBool(m_d.m_collidable);
		break;
	case FID(RVIS):
		pBiffReader->GetBool(m_d.m_visible);
		break;
	case FID(REEN):
		pBiffReader->GetBool(m_d.m_reflectionEnabled);
		break;
	case FID(ESTR):
		pBiffReader->GetBool(m_d.m_staticRendering);
		break;
	case FID(ESIE):
		pBiffReader->GetBool(m_d.m_showInEditor);
		break;
	case FID(ROTX):
		pBiffReader->GetFloat(m_d.m_rotX);
		break;
	case FID(ROTY):
		pBiffReader->GetFloat(m_d.m_rotY);
		break;
	case FID(ROTZ):
		pBiffReader->GetFloat(m_d.m_rotZ);
		break;
	case FID(MAPH):
		pBiffReader->GetString(m_d.m_szPhysicsMaterial);
		break;
	case FID(OVPH):
		pBiffReader->GetBool(m_d.m_overwritePhysics);
		break;
	default:
	{
		// TODO: LoadPointToken(id, pBiffReader, pBiffReader->m_version);
		ISelect::LoadToken(id, pBiffReader);
		break;
	}
	}
	return true;
}

void Rubber::WriteRegDefaults()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	static const char strKeyName[] = "DefaultProps\\Rubber";

	pRegUtil->SaveValueFloat(strKeyName, "Height", m_d.m_height);
	pRegUtil->SaveValueFloat(strKeyName, "HitHeight", m_d.m_hitHeight);
	pRegUtil->SaveValueInt(strKeyName, "Thickness", m_d.m_thickness);
	pRegUtil->SaveValueBool(strKeyName, "HitEvent", m_d.m_hitEvent);
	pRegUtil->SaveValueBool(strKeyName, "TimerEnabled", m_d.m_tdr.m_TimerEnabled);
	pRegUtil->SaveValueInt(strKeyName, "TimerInterval", m_d.m_tdr.m_TimerInterval);
	pRegUtil->SaveValue(strKeyName, "Image", m_d.m_szImage);
	pRegUtil->SaveValueFloat(strKeyName, "Elasticity", m_d.m_elasticity);
	pRegUtil->SaveValueFloat(strKeyName, "ElasticityFalloff", m_d.m_elasticityFalloff);
	pRegUtil->SaveValueFloat(strKeyName, "Friction", m_d.m_friction);
	pRegUtil->SaveValueFloat(strKeyName, "Scatter", m_d.m_scatter);
	pRegUtil->SaveValueBool(strKeyName, "Collidable", m_d.m_collidable);
	pRegUtil->SaveValueBool(strKeyName, "Visible", m_d.m_visible);
	pRegUtil->SaveValueBool(strKeyName, "EnableStaticRendering", m_d.m_staticRendering);
	pRegUtil->SaveValueBool(strKeyName, "EnableShowInEditor", m_d.m_showInEditor);
	pRegUtil->SaveValueFloat(strKeyName, "RotX", m_d.m_rotX);
	pRegUtil->SaveValueFloat(strKeyName, "RotY", m_d.m_rotY);
	pRegUtil->SaveValueFloat(strKeyName, "RotZ", m_d.m_rotZ);
	pRegUtil->SaveValueBool(strKeyName, "ReflectionEnabled", m_d.m_reflectionEnabled);
}