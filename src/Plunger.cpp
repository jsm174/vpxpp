#include "Plunger.h"
#include "RegUtil.h"

const ItemTypeEnum Plunger::ItemType = eItemPlunger;
const int Plunger::TypeNameID = 116;
const int Plunger::ToolID = 117;
const int Plunger::CursorID = 143;
const unsigned Plunger::AllowedViews = 1;

Plunger* Plunger::COMCreate()
{
	/*CComObject<Plunger>* obj = 0;
	if ((((HRESULT)(CComObject<Plunger>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Plunger();
}

IEditable* Plunger::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Plunger::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Plunger* obj = Plunger::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Plunger::Plunger()
{
	// TODO: m_phitplunger = NULL;
	m_vertexBuffer = NULL;
	m_indexBuffer = NULL;
}

Plunger::~Plunger()
{
}

HRESULT Plunger::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	m_d.m_v.x = x;
	m_d.m_v.y = y;

	SetDefaults(fromMouseClick);

	return InitVBA(true, 0, NULL);
}

HRESULT Plunger::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			GetPTable()->GetUniqueName(eItemPlunger, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t*)m_wzName,
			//             sizeof(m_wzName) / sizeof(m_wzName[0]));
		}
	}
	InitScript();
	return ((HRESULT)0L);
}

PinTable* Plunger::GetPTable()
{
	return m_ptable;
}

HRESULT Plunger::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	m_d.m_color = RGB(76, 76, 76);
	SetDefaults(false);

	m_ptable = pTable;

	BiffReader biffReader(pStream, this, pId, version);
	biffReader.Load();

	return S_OK;
}

HRESULT Plunger::InitPostLoad()
{
	return S_OK;
}

void Plunger::SetDefaults(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	SetDefaultPhysics(fromMouseClick);

	m_d.m_height = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Plunger", "Height", 20.f) : 20.f;
	m_d.m_width = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Plunger", "Width", 25.f) : 25.f;
	m_d.m_zAdjust = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Plunger", "ZAdjust", 0) : 0;
	m_d.m_stroke = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Plunger", "Stroke", m_d.m_height * 4) : (m_d.m_height * 4);
	m_d.m_speedPull = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Plunger", "PullSpeed", 5.f) : 5.f;
	m_d.m_type = fromMouseClick ? (PlungerType)pRegUtil->LoadValueIntWithDefault("DefaultProps\\Plunger", "PlungerType", PlungerTypeModern) : PlungerTypeModern;
	m_d.m_color = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\Plunger", "Color", RGB(76, 76, 76)) : RGB(76, 76, 76);

	HRESULT hr = pRegUtil->LoadValue("DefaultProps\\Plunger", "Image", m_d.m_szImage);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_d.m_szImage.clear();
	}

	m_d.m_animFrames = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\Plunger", "AnimFrames", 1) : 1;
	m_d.m_tdr.m_TimerEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Plunger", "TimerEnabled", false) : false;
	m_d.m_tdr.m_TimerInterval = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\Plunger", "TimerInterval", 100) : 100;

	hr = pRegUtil->LoadValue("DefaultProps\\Plunger", "Surface", m_d.m_szSurface);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_d.m_szSurface.clear();
	}

	m_d.m_mechPlunger = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Plunger", "MechPlunger", false) : false; // plungers require selection for mechanical input
	m_d.m_autoPlunger = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Plunger", "AutoPlunger", false) : false;
	m_d.m_visible = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Plunger", "Visible", true) : true;

	hr = pRegUtil->LoadValue("DefaultProps\\Plunger", "CustomTipShape", m_d.m_szTipShape, MAXTIPSHAPE);
	if ((hr != S_OK) || !fromMouseClick)
	{
		strncpy(m_d.m_szTipShape,
				"0 .34; 2 .6; 3 .64; 5 .7; 7 .84; 8 .88; 9 .9; 11 .92; 14 .92; 39 .84", sizeof(m_d.m_szTipShape) - 1);
	}

	m_d.m_rodDiam = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Plunger", "CustomRodDiam", 0.60f) : 0.60f;
	m_d.m_ringGap = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Plunger", "CustomRingGap", 2.0f) : 2.0f;
	m_d.m_ringDiam = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Plunger", "CustomRingDiam", 0.94f) : 0.94f;
	m_d.m_ringWidth = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Plunger", "CustomRingWidth", 3.0f) : 3.0f;
	m_d.m_springDiam = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Plunger", "CustomSpringDiam", 0.77f) : 0.77f;
	m_d.m_springGauge = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Plunger", "CustomSpringGauge", 1.38f) : 1.38f;
	m_d.m_springLoops = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Plunger", "CustomSpringLoops", 8.0f) : 8.0f;
	m_d.m_springEndLoops = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Plunger", "CustomSpringEndLoops", 2.5f) : 2.5f;
	m_d.m_reflectionEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Plunger", "ReflectionEnabled", true) : true;
}

void Plunger::SetDefaultPhysics(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_d.m_speedFire = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Plunger", "ReleaseSpeed", 80.f) : 80.f;
	m_d.m_mechStrength = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Plunger", "MechStrength", 85.f) : 85.f;
	m_d.m_parkPosition = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Plunger", "ParkPosition", (float)(0.5 / 3.0)) : (float)(0.5 / 3.0); // typical mechanical plunger has 3 inch stroke and 0.5 inch rest position //!! 0.01f better for some HW-plungers, but this seems to be rather a firmware/config issue
	m_d.m_scatterVelocity = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Plunger", "ScatterVelocity", 0.f) : 0.f;
	m_d.m_momentumXfer = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Plunger", "MomentumXfer", 1.f) : 1.f;
}

bool Plunger::LoadToken(const int id, BiffReader* const pBiffReader)
{
	switch (id)
	{
	case FID(PIID):
		pBiffReader->GetInt((int*)pBiffReader->m_pData);
		break;
	case FID(VCEN):
		pBiffReader->GetVector2(m_d.m_v);
		break;
	case FID(WDTH):
		pBiffReader->GetFloat(m_d.m_width);
		break;
	case FID(ZADJ):
		pBiffReader->GetFloat(m_d.m_zAdjust);
		break;
	case FID(HIGH):
		pBiffReader->GetFloat(m_d.m_height);
		break;
	case FID(HPSL):
		pBiffReader->GetFloat(m_d.m_stroke);
		break;
	case FID(SPDP):
		pBiffReader->GetFloat(m_d.m_speedPull);
		break;
	case FID(SPDF):
		pBiffReader->GetFloat(m_d.m_speedFire);
		break;
	case FID(MEST):
		pBiffReader->GetFloat(m_d.m_mechStrength);
		break;
	case FID(MPRK):
		pBiffReader->GetFloat(m_d.m_parkPosition);
		break;
	case FID(PSCV):
		pBiffReader->GetFloat(m_d.m_scatterVelocity);
		break;
	case FID(MOMX):
		pBiffReader->GetFloat(m_d.m_momentumXfer);
		break;
	case FID(TMON):
		pBiffReader->GetBool(m_d.m_tdr.m_TimerEnabled);
		break;
	case FID(MECH):
		pBiffReader->GetBool(m_d.m_mechPlunger);
		break;
	case FID(APLG):
		pBiffReader->GetBool(m_d.m_autoPlunger);
		break;
	case FID(TMIN):
		pBiffReader->GetInt(m_d.m_tdr.m_TimerInterval);
		break;
	case FID(NAME):
		pBiffReader->GetWideString(m_wzName, sizeof(m_wzName) / sizeof(wchar_t));
		break;
	case FID(TYPE):
		pBiffReader->GetInt(&m_d.m_type);
		break;
	case FID(ANFR):
		pBiffReader->GetInt(m_d.m_animFrames);
		break;
	case FID(MATR):
		pBiffReader->GetString(m_d.m_szMaterial);
		break;
	case FID(IMAG):
		pBiffReader->GetString(m_d.m_szImage);
		break;
	case FID(VSBL):
		pBiffReader->GetBool(m_d.m_visible);
		break;
	case FID(REEN):
		pBiffReader->GetBool(m_d.m_reflectionEnabled);
		break;
	case FID(SURF):
		pBiffReader->GetString(m_d.m_szSurface);
		break;
	case FID(TIPS):
		pBiffReader->GetString(m_d.m_szTipShape, sizeof(m_d.m_szTipShape));
		break;
	case FID(RODD):
		pBiffReader->GetFloat(m_d.m_rodDiam);
		break;
	case FID(RNGG):
		pBiffReader->GetFloat(m_d.m_ringGap);
		break;
	case FID(RNGD):
		pBiffReader->GetFloat(m_d.m_ringDiam);
		break;
	case FID(RNGW):
		pBiffReader->GetFloat(m_d.m_ringWidth);
		break;
	case FID(SPRD):
		pBiffReader->GetFloat(m_d.m_springDiam);
		break;
	case FID(SPRG):
		pBiffReader->GetFloat(m_d.m_springGauge);
		break;
	case FID(SPRL):
		pBiffReader->GetFloat(m_d.m_springLoops);
		break;
	case FID(SPRE):
		pBiffReader->GetFloat(m_d.m_springEndLoops);
		break;
	default:
		ISelect::LoadToken(id, pBiffReader);
		break;
	}
	return true;
}

void Plunger::WriteRegDefaults()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	pRegUtil->SaveValueFloat("DefaultProps\\Plunger", "Height", m_d.m_height);
	pRegUtil->SaveValueFloat("DefaultProps\\Plunger", "Width", m_d.m_width);
	pRegUtil->SaveValueFloat("DefaultProps\\Plunger", "ZAdjust", m_d.m_zAdjust);
	pRegUtil->SaveValueFloat("DefaultProps\\Plunger", "Stroke", m_d.m_stroke);
	pRegUtil->SaveValueFloat("DefaultProps\\Plunger", "PullSpeed", m_d.m_speedPull);
	pRegUtil->SaveValueFloat("DefaultProps\\Plunger", "ReleaseSpeed", m_d.m_speedFire);
	pRegUtil->SaveValueInt("DefaultProps\\Plunger", "PlungerType", m_d.m_type);
	pRegUtil->SaveValueInt("DefaultProps\\Plunger", "AnimFrames", m_d.m_animFrames);
	pRegUtil->SaveValueInt("DefaultProps\\Plunger", "Color", m_d.m_color);
	pRegUtil->SaveValue("DefaultProps\\Plunger", "Image", m_d.m_szImage);
	pRegUtil->SaveValueBool("DefaultProps\\Plunger", "TimerEnabled", m_d.m_tdr.m_TimerEnabled);
	pRegUtil->SaveValueInt("DefaultProps\\Plunger", "TimerInterval", m_d.m_tdr.m_TimerInterval);
	pRegUtil->SaveValue("DefaultProps\\Plunger", "Surface", m_d.m_szSurface);
	pRegUtil->SaveValueBool("DefaultProps\\Plunger", "MechPlunger", m_d.m_mechPlunger);
	pRegUtil->SaveValueBool("DefaultProps\\Plunger", "AutoPlunger", m_d.m_autoPlunger);
	pRegUtil->SaveValueFloat("DefaultProps\\Plunger", "MechStrength", m_d.m_mechStrength);
	pRegUtil->SaveValueFloat("DefaultProps\\Plunger", "ParkPosition", m_d.m_parkPosition);
	pRegUtil->SaveValueBool("DefaultProps\\Plunger", "Visible", m_d.m_visible);
	pRegUtil->SaveValueFloat("DefaultProps\\Plunger", "ScatterVelocity", m_d.m_scatterVelocity);
	pRegUtil->SaveValueFloat("DefaultProps\\Plunger", "MomentumXfer", m_d.m_momentumXfer);
	pRegUtil->SaveValue("DefaultProps\\Plunger", "CustomTipShape", m_d.m_szTipShape);
	pRegUtil->SaveValueFloat("DefaultProps\\Plunger", "CustomRodDiam", m_d.m_rodDiam);
	pRegUtil->SaveValueFloat("DefaultProps\\Plunger", "CustomRingGap", m_d.m_ringGap);
	pRegUtil->SaveValueFloat("DefaultProps\\Plunger", "CustomRingDiam", m_d.m_ringDiam);
	pRegUtil->SaveValueFloat("DefaultProps\\Plunger", "CustomRingWidth", m_d.m_ringWidth);
	pRegUtil->SaveValueFloat("DefaultProps\\Plunger", "CustomSpringDiam", m_d.m_springDiam);
	pRegUtil->SaveValueFloat("DefaultProps\\Plunger", "CustomSpringGauge", m_d.m_springGauge);
	pRegUtil->SaveValueFloat("DefaultProps\\Plunger", "CustomSpringLoops", m_d.m_springLoops);
	pRegUtil->SaveValueFloat("DefaultProps\\Plunger", "CustomSpringEndLoops", m_d.m_springEndLoops);
	pRegUtil->SaveValueBool("DefaultProps\\Plunger", "ReflectionEnabled", m_d.m_reflectionEnabled);
}