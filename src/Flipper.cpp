#include "Flipper.h"
#include "RegUtil.h"

const ItemTypeEnum Flipper::ItemType = eItemFlipper;
const int Flipper::TypeNameID = 105;
const int Flipper::ToolID = 111;
const int Flipper::CursorID = 139;
const unsigned Flipper::AllowedViews = 1;

Flipper* Flipper::COMCreate()
{
	/*CComObject<Flipper>* obj = 0;
	if ((((HRESULT)(CComObject<Flipper>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Flipper();
}

IEditable* Flipper::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Flipper::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Flipper* obj = Flipper::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Flipper::Flipper()
{
	// TODO: m_phitflipper = NULL;
	m_vertexBuffer = NULL;
	m_indexBuffer = NULL;
	m_ptable = NULL;
}

Flipper::~Flipper()
{
}

HRESULT Flipper::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	m_d.m_Center.x = x;
	m_d.m_Center.y = y;

	SetDefaults(fromMouseClick);

	//TODO: m_phitflipper = NULL;

	return InitVBA(true, 0, NULL);
}

HRESULT Flipper::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			//GetPTable()->GetUniqueName(eItemFlipper, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t *)m_wzName, sizeof(m_wzName)/sizeof(m_wzName[0]));/*lstrcpyW((WCHAR *)m_wzName, wzUniqueName);*/
		}
	}
	//InitScript();
	return S_OK;
}

PinTable* Flipper::GetPTable()
{
	return m_ptable;
}

HRESULT Flipper::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	SetDefaults(false);

	m_ptable = pTable;

	BiffReader biffReader(pStream, this, pId, version);
	biffReader.Load();

	return S_OK;
}

void Flipper::SetDefaults(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	static const char regKey[] = "DefaultProps\\Flipper";

	SetDefaultPhysics(fromMouseClick);

	m_d.m_StartAngle = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(regKey, "StartAngle", 121.f) : 121.f;
	m_d.m_EndAngle = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(regKey, "EndAngle", 70.f) : 70.f;
	m_d.m_BaseRadius = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(regKey, "BaseRadius", 21.5f) : 21.5f;   // 15
	m_d.m_EndRadius = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(regKey, "EndRadius", 13.f) : 13.f;       // 6
	m_d.m_FlipperRadiusMax = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(regKey, "Length", 130.f) : 130.f; // 80
	m_d.m_FlipperRadiusMin = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(regKey, "MaxDifLength", 0.f) : 0.f;

	m_d.m_FlipperRadius = m_d.m_FlipperRadiusMax;

	m_d.m_tdr.m_TimerEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(regKey, "TimerEnabled", false) : false;
	m_d.m_tdr.m_TimerInterval = fromMouseClick ? pRegUtil->LoadValueIntWithDefault(regKey, "TimerInterval", 100) : 100;
	m_d.m_color = fromMouseClick ? pRegUtil->LoadValueIntWithDefault(regKey, "Color", RGB(255, 255, 255)) : RGB(255, 255, 255);
	m_d.m_rubbercolor = fromMouseClick ? pRegUtil->LoadValueIntWithDefault(regKey, "RubberColor", RGB(128, 50, 50)) : RGB(128, 50, 50);

	const HRESULT hr = pRegUtil->LoadValue(regKey, "Surface", m_d.m_szSurface);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_d.m_szSurface.clear();
	}

	m_d.m_height = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(regKey, "Height", 50.f) : 50.f;
	m_d.m_rubberthickness = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(regKey, "RubberThickness", 7.f) : 7.f;
	m_d.m_rubberheight = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(regKey, "RubberHeight", 19.f) : 19.f;
	m_d.m_rubberwidth = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(regKey, "RubberWidth", 24.f) : 24.f;
	m_d.m_visible = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(regKey, "Visible", true) : true;
	m_d.m_enabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(regKey, "Enabled", true) : true;
	m_d.m_reflectionEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(regKey, "ReflectionEnabled", true) : true;
}

void Flipper::SetDefaultPhysics(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	static const char regKey[] = "DefaultProps\\Flipper";

	m_d.m_scatter = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(regKey, "Scatter", 0.f) : 0.f;
	m_d.m_strength = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(regKey, "Strength", 2200.f) : 2200.f;
	m_d.m_torqueDamping = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(regKey, "EOSTorque", 0.75f) : 0.75f;
	m_d.m_torqueDampingAngle = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(regKey, "EOSTorqueAngle", 6.f) : 6.f;

	m_d.m_return = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(regKey, "ReturnStrength", 0.058f) : 0.058f;

	float fTmp;
	HRESULT hr = pRegUtil->LoadValue(regKey, "Mass", fTmp);
	if (hr != S_OK)
	{
		hr = pRegUtil->LoadValue(regKey, "Speed", fTmp); // previously Mass was called Speed, deprecated!
	}
	m_d.m_mass = (hr == S_OK) && fromMouseClick ? fTmp : 1.0f;

	m_d.m_elasticity = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(regKey, "Elasticity", 0.8f) : 0.8f;
	m_d.m_elasticityFalloff = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(regKey, "ElasticityFalloff", 0.43f) : 0.43f;
	m_d.m_friction = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(regKey, "Friction", 0.6f) : 0.6f;
	m_d.m_rampUp = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(regKey, "RampUp", 3.0f) : 3.0f;

	m_d.m_OverridePhysics = 0;
}

bool Flipper::LoadToken(const int id, BiffReader* const pBiffReader)
{
	switch (id)
	{
	case FID(PIID):
		pBiffReader->GetInt((int*)pBiffReader->m_pData);
		break;
	case FID(VCEN):
		pBiffReader->GetVector2(m_d.m_Center);
		break;
	case FID(BASR):
		pBiffReader->GetFloat(m_d.m_BaseRadius);
		break;
	case FID(ENDR):
		pBiffReader->GetFloat(m_d.m_EndRadius);
		break;
	case FID(FLPR):
		pBiffReader->GetFloat(m_d.m_FlipperRadiusMax);
		break;
	case FID(FRTN):
		pBiffReader->GetFloat(m_d.m_return);
		break;
	case FID(ANGS):
		pBiffReader->GetFloat(m_d.m_StartAngle);
		break;
	case FID(ANGE):
		pBiffReader->GetFloat(m_d.m_EndAngle);
		break;
	case FID(OVRP):
		pBiffReader->GetInt(m_d.m_OverridePhysics);
		break;
	case FID(FORC):
		pBiffReader->GetFloat(m_d.m_mass);
		break;
	case FID(TMON):
		pBiffReader->GetBool(m_d.m_tdr.m_TimerEnabled);
		break;
	case FID(TMIN):
	{
		pBiffReader->GetInt(m_d.m_tdr.m_TimerInterval);
		if (m_d.m_tdr.m_TimerInterval < 1)
		{
			m_d.m_tdr.m_TimerInterval = 100;
		}
		break;
	}
	case FID(SURF):
		pBiffReader->GetString(m_d.m_szSurface);
		break;
	case FID(MATR):
		pBiffReader->GetString(m_d.m_szMaterial);
		break;
	case FID(RUMA):
		pBiffReader->GetString(m_d.m_szRubberMaterial);
		break;
	case FID(NAME):
		pBiffReader->GetWideString(m_wzName, sizeof(m_wzName) / sizeof(wchar_t));
		break;
	case FID(RTHK): //!! deprecated, remove
	{
		int rt;
		pBiffReader->GetInt(rt);
		m_d.m_rubberthickness = (float)rt;
		break;
	}
	case FID(RTHF):
		pBiffReader->GetFloat(m_d.m_rubberthickness);
		break;
	case FID(RHGT): //!! deprecated, remove
	{
		int rh;
		pBiffReader->GetInt(rh);
		m_d.m_rubberheight = (float)rh;
		break;
	}
	case FID(RHGF):
		pBiffReader->GetFloat(m_d.m_rubberheight);
		break;
	case FID(RWDT): //!! deprecated, remove
	{
		int rw;
		pBiffReader->GetInt(rw);
		m_d.m_rubberwidth = (float)rw;
		break;
	}
	case FID(RWDF):
		pBiffReader->GetFloat(m_d.m_rubberwidth);
		break;
	case FID(FHGT):
		pBiffReader->GetFloat(m_d.m_height);
		break;
	case FID(STRG):
		pBiffReader->GetFloat(m_d.m_strength);
		break;
	case FID(ELAS):
		pBiffReader->GetFloat(m_d.m_elasticity);
		break;
	case FID(ELFO):
		pBiffReader->GetFloat(m_d.m_elasticityFalloff);
		break;
	case FID(FRIC):
		pBiffReader->GetFloat(m_d.m_friction);
		break;
	case FID(RPUP):
		pBiffReader->GetFloat(m_d.m_rampUp);
		break;
	case FID(SCTR):
		pBiffReader->GetFloat(m_d.m_scatter);
		break;
	case FID(TODA):
		pBiffReader->GetFloat(m_d.m_torqueDamping);
		break;
	case FID(TDAA):
		pBiffReader->GetFloat(m_d.m_torqueDampingAngle);
		break;
	case FID(FRMN):
		pBiffReader->GetFloat(m_d.m_FlipperRadiusMin);
		break;
	case FID(VSBL):
		pBiffReader->GetBool(m_d.m_visible);
		break;
	case FID(ENBL):
		pBiffReader->GetBool(m_d.m_enabled);
		break;
	case FID(REEN):
		pBiffReader->GetBool(m_d.m_reflectionEnabled);
		break;
	case FID(IMAG):
		pBiffReader->GetString(m_d.m_szImage);
		break;
	default:
		ISelect::LoadToken(id, pBiffReader);
		break;
	}
	return true;
}

void Flipper::WriteRegDefaults()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	static const char regKey[] = "DefaultProps\\Flipper";

	pRegUtil->SaveValueFloat(regKey, "Scatter", m_d.m_scatter);
	pRegUtil->SaveValueFloat(regKey, "Strength", m_d.m_strength);
	pRegUtil->SaveValueFloat(regKey, "EOSTorque", m_d.m_torqueDamping);
	pRegUtil->SaveValueFloat(regKey, "EOSTorqueAngle", m_d.m_torqueDampingAngle);
	pRegUtil->SaveValueFloat(regKey, "StartAngle", m_d.m_StartAngle);
	pRegUtil->SaveValueFloat(regKey, "EndAngle", m_d.m_EndAngle);
	pRegUtil->SaveValueFloat(regKey, "BaseRadius", m_d.m_BaseRadius);
	pRegUtil->SaveValueFloat(regKey, "EndRadius", m_d.m_EndRadius);
	pRegUtil->SaveValueFloat(regKey, "MaxDifLength", m_d.m_FlipperRadiusMin);
	pRegUtil->SaveValueFloat(regKey, "ReturnStrength", m_d.m_return);
	pRegUtil->SaveValueFloat(regKey, "Length", m_d.m_FlipperRadiusMax);
	pRegUtil->SaveValueFloat(regKey, "Mass", m_d.m_mass);
	pRegUtil->SaveValueFloat(regKey, "Elasticity", m_d.m_elasticity);
	pRegUtil->SaveValueFloat(regKey, "ElasticityFalloff", m_d.m_elasticityFalloff);
	pRegUtil->SaveValueFloat(regKey, "Friction", m_d.m_friction);
	pRegUtil->SaveValueFloat(regKey, "RampUp", m_d.m_rampUp);
	pRegUtil->SaveValueBool(regKey, "TimerEnabled", m_d.m_tdr.m_TimerEnabled);
	pRegUtil->SaveValueInt(regKey, "TimerInterval", m_d.m_tdr.m_TimerInterval);
	pRegUtil->SaveValueInt(regKey, "Color", m_d.m_color);
	pRegUtil->SaveValueInt(regKey, "RubberColor", m_d.m_rubbercolor);
	pRegUtil->SaveValue(regKey, "Surface", m_d.m_szSurface);
	pRegUtil->SaveValueFloat(regKey, "Height", m_d.m_height);
	pRegUtil->SaveValueFloat(regKey, "RubberThickness", m_d.m_rubberthickness);
	pRegUtil->SaveValueFloat(regKey, "RubberHeight", m_d.m_rubberheight);
	pRegUtil->SaveValueFloat(regKey, "RubberWidth", m_d.m_rubberwidth);
	pRegUtil->SaveValueBool(regKey, "Visible", m_d.m_visible);
	pRegUtil->SaveValueBool(regKey, "Enabled", m_d.m_enabled);
	pRegUtil->SaveValueBool(regKey, "ReflectionEnabled", m_d.m_reflectionEnabled);
}
