#include "Bumper.h"
#include "RegUtil.h"

const ItemTypeEnum Bumper::ItemType = eItemBumper;
const int Bumper::TypeNameID = 122;
const int Bumper::ToolID = 123;
const int Bumper::CursorID = 145;
const unsigned Bumper::AllowedViews = 1;

Bumper* Bumper::COMCreate()
{
	/*CComObject<Bumper>* obj = 0;
	if ((((HRESULT)(CComObject<Bumper>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Bumper();
}

IEditable* Bumper::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Bumper::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Bumper* obj = Bumper::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Bumper::Bumper()
{
	// TODO: m_pbumperhitcircle = NULL;
	m_baseVertexBuffer = NULL;
	m_baseIndexBuffer = NULL;
	m_ringVertexBuffer = NULL;
	m_ringIndexBuffer = NULL;
	m_capVertexBuffer = NULL;
	m_capIndexBuffer = NULL;
	m_socketIndexBuffer = NULL;
	m_socketVertexBuffer = NULL;
	m_ringAnimate = false;
	// TODO: m_propVisual = NULL;
	m_d.m_ringDropOffset = 0.0f;
	m_ringDown = false;
	m_updateSkirt = false;
	m_doSkirtAnimation = false;
	m_enableSkirtAnimation = true;
	m_skirtCounter = 0.0f;
}

Bumper::~Bumper()
{
}

HRESULT Bumper::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	SetDefaults(fromMouseClick);

	m_d.m_vCenter.x = x;
	m_d.m_vCenter.y = y;

	return InitVBA(true, 0, NULL);
}

HRESULT Bumper::InitPostLoad()
{
   return S_OK;
}

HRESULT Bumper::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			GetPTable()->GetUniqueName(eItemBumper, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t*)m_wzName,
			//             sizeof(m_wzName) / sizeof(m_wzName[0]));
		}
	}
	InitScript();
	return ((HRESULT)0L);
}

PinTable* Bumper::GetPTable()
{
	return m_ptable;
}

HRESULT Bumper::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	SetDefaults(false);

	m_ptable = pTable;

	BiffReader biffReader(pStream, this, pId, version);
	biffReader.Load();

	return S_OK;
}

void Bumper::SetDefaults(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_d.m_radius = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Bumper", "Radius", 45.f) : 45.f;

	SetDefaultPhysics(fromMouseClick);

	m_d.m_heightScale = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Bumper", "HeightScale", 90.0f) : 90.0f;
	m_d.m_ringSpeed = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Bumper", "RingSpeed", 0.5f) : 0.5f;
	m_d.m_orientation = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Bumper", "Orientation", 0.0f) : 0.0f;
	m_d.m_threshold = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Bumper", "Threshold", 1.f) : 1.f;

	const HRESULT hr = pRegUtil->LoadValue("DefaultProps\\Bumper", "Surface", m_d.m_szSurface);
	if (hr != S_OK || !fromMouseClick)
	{
		m_d.m_szSurface.clear();
	}

	m_d.m_tdr.m_TimerEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Bumper", "TimerEnabled", false) : false;
	m_d.m_tdr.m_TimerInterval = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\Bumper", "TimerInterval", 100) : 100;
	m_d.m_capVisible = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Bumper", "CapVisible", true) : true;
	m_d.m_baseVisible = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Bumper", "BaseVisible", true) : true;
	m_d.m_ringVisible = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Bumper", "RingVisible", true) : true;
	m_d.m_skirtVisible = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Bumper", "SkirtVisible", true) : true;
	m_d.m_reflectionEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Bumper", "ReflectionEnabled", true) : true;
	m_d.m_hitEvent = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Bumper", "HasHitEvent", true) : true;
	m_d.m_collidable = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Bumper", "Collidable", true) : true;

	m_ringAnimate = false;
	m_d.m_ringDropOffset = 0.0f;
}

void Bumper::SetDefaultPhysics(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_d.m_force = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Bumper", "Force", 15) : 15;
	m_d.m_scatter = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Bumper", "Scatter", 0) : 0;
}

bool Bumper::LoadToken(const int id, BiffReader* const pBiffReader)
{
	switch (id)
	{
	case FID(PIID):
		pBiffReader->GetInt((int*)pBiffReader->m_pData);
		break;
	case FID(VCEN):
		pBiffReader->GetVector2(m_d.m_vCenter);
		break;
	case FID(RADI):
		pBiffReader->GetFloat(m_d.m_radius);
		break;
	case FID(MATR):
		pBiffReader->GetString(m_d.m_szCapMaterial);
		break;
	case FID(RIMA):
		pBiffReader->GetString(m_d.m_szRingMaterial);
		break;
	case FID(BAMA):
		pBiffReader->GetString(m_d.m_szBaseMaterial);
		break;
	case FID(SKMA):
		pBiffReader->GetString(m_d.m_szSkirtMaterial);
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
	case FID(FORC):
		pBiffReader->GetFloat(m_d.m_force);
		break;
	case FID(BSCT):
		pBiffReader->GetFloat(m_d.m_scatter);
		break;
	case FID(HISC):
		pBiffReader->GetFloat(m_d.m_heightScale);
		break;
	case FID(RISP):
		pBiffReader->GetFloat(m_d.m_ringSpeed);
		break;
	case FID(ORIN):
		pBiffReader->GetFloat(m_d.m_orientation);
		break;
	case FID(RDLI):
		pBiffReader->GetFloat(m_d.m_ringDropOffset);
		break;
	case FID(SURF):
		pBiffReader->GetString(m_d.m_szSurface);
		break;
	case FID(NAME):
		pBiffReader->GetWideString(m_wzName, sizeof(m_wzName) / sizeof(wchar_t));
		break;
	case FID(BVIS):
	{
		// backwards compatibility when loading old VP9 tables
		bool value;
		pBiffReader->GetBool(value);
		m_d.m_capVisible = value;
		m_d.m_baseVisible = value;
		m_d.m_ringVisible = value;
		m_d.m_skirtVisible = value;
		break;
	}
	case FID(CAVI):
		pBiffReader->GetBool(m_d.m_capVisible);
		break;
	case FID(HAHE):
		pBiffReader->GetBool(m_d.m_hitEvent);
		break;
	case FID(COLI):
		pBiffReader->GetBool(m_d.m_collidable);
		break;
	case FID(BSVS):
	{
		pBiffReader->GetBool(m_d.m_baseVisible);

		m_d.m_ringVisible = m_d.m_baseVisible;
		m_d.m_skirtVisible = m_d.m_baseVisible;
		break;
	}
	case FID(RIVS):
		pBiffReader->GetBool(m_d.m_ringVisible);
		break;
	case FID(SKVS):
		pBiffReader->GetBool(m_d.m_skirtVisible);
		break;
	case FID(REEN):
		pBiffReader->GetBool(m_d.m_reflectionEnabled);
		break;
	default:
		ISelect::LoadToken(id, pBiffReader);
		break;
	}
	return true;
}

void Bumper::WriteRegDefaults()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	pRegUtil->SaveValueFloat("DefaultProps\\Bumper", "Radius", m_d.m_radius);
	pRegUtil->SaveValueFloat("DefaultProps\\Bumper", "Force", m_d.m_force);
	pRegUtil->SaveValueFloat("DefaultProps\\Bumper", "Scatter", m_d.m_scatter);
	pRegUtil->SaveValueFloat("DefaultProps\\Bumper", "HeightScale", m_d.m_heightScale);
	pRegUtil->SaveValueFloat("DefaultProps\\Bumper", "RingSpeed", m_d.m_ringSpeed);
	pRegUtil->SaveValueFloat("DefaultProps\\Bumper", "Orientation", m_d.m_orientation);
	pRegUtil->SaveValueFloat("DefaultProps\\Bumper", "Threshold", m_d.m_threshold);
	pRegUtil->SaveValueBool("DefaultProps\\Bumper", "TimerEnabled", m_d.m_tdr.m_TimerEnabled);
	pRegUtil->SaveValueInt("DefaultProps\\Bumper", "TimerInterval", m_d.m_tdr.m_TimerInterval);
	pRegUtil->SaveValueBool("DefaultProps\\Bumper", "CapVisible", m_d.m_capVisible);
	pRegUtil->SaveValueBool("DefaultProps\\Bumper", "BaseVisible", m_d.m_baseVisible);
	pRegUtil->SaveValueBool("DefaultProps\\Bumper", "HasHitEvent", m_d.m_hitEvent);
	pRegUtil->SaveValueBool("DefaultProps\\Bumper", "Collidable", m_d.m_collidable);
	pRegUtil->SaveValueBool("DefaultProps\\Bumper", "ReflectionEnabled", m_d.m_reflectionEnabled);
	pRegUtil->SaveValue("DefaultProps\\Bumper", "Surface", m_d.m_szSurface);
}