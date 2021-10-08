#include "Light.h"
#include "RegUtil.h"

const ItemTypeEnum Light::ItemType = eItemLight;
const int Light::TypeNameID = 130;
const int Light::ToolID = 131;
const int Light::CursorID = 110;
const unsigned Light::AllowedViews = 3;

Light* Light::COMCreate()
{
	/*CComObject<Light>* obj = 0;
	if ((((HRESULT)(CComObject<Light>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Light();
}

IEditable* Light::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Light::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Light* obj = Light::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Light::Light()
{
	// TODO: m_menuid = IDR_SURFACEMENU;
	m_customMoverVBuffer = NULL;
	m_customMoverIBuffer = NULL;
	m_bulbLightIndexBuffer = NULL;
	m_bulbLightVBuffer = NULL;
	m_bulbSocketIndexBuffer = NULL;
	m_bulbSocketVBuffer = NULL;
	m_d.m_depthBias = 0.0f;
	// TODO: m_d.m_shape = ShapeCustom;
	m_d.m_visible = true;
	m_roundLight = false;
	// TODO: m_propVisual = NULL;
	m_updateBulbLightHeight = false;
}

Light::~Light()
{
}

bool Light::IsTransparent() const
{
	return m_d.m_BulbLight || (m_surfaceMaterial && m_surfaceMaterial->m_bOpacityActive);
}

HRESULT Light::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	m_d.m_vCenter.x = x;
	m_d.m_vCenter.y = y;

	SetDefaults(fromMouseClick);

	// TODO: InitShape();

	m_lockedByLS = false;
	m_inPlayState = m_d.m_state;
	m_d.m_visible = true;

	return InitVBA(true, 0, NULL);
}

HRESULT Light::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		GetPTable()->GetUniqueName(eItemLight, wzUniqueName, 128);
		wcsncpy(m_wzName, wzUniqueName, MAXNAMEBUFFER);
	}
	InitScript();
	return ((HRESULT)0L);
}

HRESULT Light::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	SetDefaults(false);

	m_d.m_falloff = 50.f;
	m_d.m_falloff_power = 2.0f;
	m_d.m_state = LightStateOff;
	// TODO: m_d.m_shape = ShapeCustom;

	m_d.m_tdr.m_TimerEnabled = false;
	m_d.m_tdr.m_TimerInterval = 100;

	m_d.m_color = RGB(255, 255, 0);
	m_d.m_color2 = RGB(255, 255, 255);

	m_rgblinkpattern = "10";
	m_blinkinterval = 125;

	m_ptable = pTable;

	m_lockedByLS = false;
	m_inPlayState = m_d.m_state;

	BiffReader biffReader(pStream, this, pId, version);
	biffReader.Load();

	return S_OK;
}

HRESULT Light::InitPostLoad()
{
	return S_OK;
}

void Light::SetDefaults(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_duration = 0;
	m_finalState = 0;

	m_d.m_falloff = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Light", "Falloff", 50.f) : 50.f;
	m_d.m_falloff_power = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Light", "FalloffPower", 2.0f) : 2.0f;
	m_d.m_state = fromMouseClick ? (LightState)pRegUtil->LoadValueIntWithDefault("DefaultProps\\Light", "LightState", LightStateOff) : LightStateOff;

	// TODO: m_d.m_shape = ShapeCustom;

	m_d.m_tdr.m_TimerEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Light", "TimerEnabled", false) : false;
	m_d.m_tdr.m_TimerInterval = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\Light", "TimerInterval", 100) : 100;
	m_d.m_color = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\Light", "Color", RGB(255, 255, 0)) : RGB(255, 255, 0);
	m_d.m_color2 = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\Light", "ColorFull", RGB(255, 255, 255)) : RGB(255, 255, 255);

	HRESULT hr = pRegUtil->LoadValue("DefaultProps\\Light", "OffImage", m_d.m_szImage);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_d.m_szImage.clear();
	}

	hr = pRegUtil->LoadValue("DefaultProps\\Light", "BlinkPattern", m_rgblinkpattern);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_rgblinkpattern = "10";
	}

	m_blinkinterval = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\Light", "BlinkInterval", 125) : 125;
	m_d.m_intensity = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Light", "Intensity", 1.0f) : 1.0f;
	m_d.m_transmissionScale = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Light", "TransmissionScale", 0.5f) : 0.f; // difference in defaults is intended

	m_d.m_intensity_scale = 1.0f;

	hr = pRegUtil->LoadValue("DefaultProps\\Light", "Surface", m_d.m_szSurface);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_d.m_szSurface.clear();
	}

	m_d.m_fadeSpeedUp = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Light", "FadeSpeedUp", 0.2f) : 0.2f;
	m_d.m_fadeSpeedDown = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Light", "FadeSpeedDown", 0.2f) : 0.2f;
	m_d.m_BulbLight = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Light", "Bulb", false) : false;
	m_d.m_imageMode = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Light", "ImageMode", false) : false;
	m_d.m_showBulbMesh = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Light", "ShowBulbMesh", false) : false;
	m_d.m_staticBulbMesh = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Light", "StaticBulbMesh", true) : true;
	m_d.m_showReflectionOnBall = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Light", "ShowReflectionOnBall", true) : true;
	m_d.m_meshRadius = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Light", "ScaleBulbMesh", 20.0f) : 20.0f;
	m_d.m_modulate_vs_add = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Light", "BulbModulateVsAdd", 0.9f) : 0.9f;
	m_d.m_bulbHaloHeight = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Light", "BulbHaloHeight", 28.0f) : 28.0f;
}

bool Light::LoadToken(const int id, BiffReader* const pBiffReader)
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
		pBiffReader->GetFloat(m_d.m_falloff);
		break;
	case FID(FAPO):
		pBiffReader->GetFloat(m_d.m_falloff_power);
		break;
	case FID(STAT):
	{
		pBiffReader->GetInt(&m_d.m_state);
		m_inPlayState = m_d.m_state;
		break;
	}
	case FID(COLR):
		pBiffReader->GetInt(m_d.m_color);
		break;
	case FID(COL2):
		pBiffReader->GetInt(m_d.m_color2);
		break;
	case FID(IMG1):
		pBiffReader->GetString(m_d.m_szImage);
		break;
	case FID(TMON):
		pBiffReader->GetBool(m_d.m_tdr.m_TimerEnabled);
		break;
	case FID(TMIN):
		pBiffReader->GetInt(m_d.m_tdr.m_TimerInterval);
		break;
	case FID(SHAP):
		m_roundLight = true;
		break;
	case FID(BPAT):
		pBiffReader->GetString(m_rgblinkpattern);
		break;
	case FID(BINT):
		pBiffReader->GetInt(m_blinkinterval);
		break;
	case FID(BWTH):
		pBiffReader->GetFloat(m_d.m_intensity);
		break;
	case FID(TRMS):
		pBiffReader->GetFloat(m_d.m_transmissionScale);
		break;
	case FID(SURF):
		pBiffReader->GetString(m_d.m_szSurface);
		break;
	case FID(NAME):
		pBiffReader->GetWideString(m_wzName, sizeof(m_wzName) / sizeof(wchar_t));
		break;
	case FID(BGLS):
		pBiffReader->GetBool(m_backglass);
		break;
	case FID(LIDB):
		pBiffReader->GetFloat(m_d.m_depthBias);
		break;
	case FID(FASP):
		pBiffReader->GetFloat(m_d.m_fadeSpeedUp);
		break;
	case FID(FASD):
		pBiffReader->GetFloat(m_d.m_fadeSpeedDown);
		break;
	case FID(BULT):
		pBiffReader->GetBool(m_d.m_BulbLight);
		break;
	case FID(IMMO):
		pBiffReader->GetBool(m_d.m_imageMode);
		break;
	case FID(SHBM):
		pBiffReader->GetBool(m_d.m_showBulbMesh);
		break;
	case FID(STBM):
		pBiffReader->GetBool(m_d.m_staticBulbMesh);
		break;
	case FID(SHRB):
		pBiffReader->GetBool(m_d.m_showReflectionOnBall);
		break;
	case FID(BMSC):
		pBiffReader->GetFloat(m_d.m_meshRadius);
		break;
	case FID(BMVA):
		pBiffReader->GetFloat(m_d.m_modulate_vs_add);
		break;
	case FID(BHHI):
		pBiffReader->GetFloat(m_d.m_bulbHaloHeight);
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

PinTable* Light::GetPTable()
{
	return m_ptable;
}

const PinTable* Light::GetPTable() const
{
	return m_ptable;
}

IEditable* Light::GetIEditable()
{
	return static_cast<IEditable*>(this);
}

const IEditable* Light::GetIEditable() const
{
	return static_cast<const IEditable*>(this);
}

ISelect* Light::GetISelect()
{
	return static_cast<ISelect*>(this);
}

const ISelect* Light::GetISelect() const
{
	return static_cast<const ISelect*>(this);
}

IHitable* Light::GetIHitable()
{
	return static_cast<IHitable*>(this);
}

const IHitable* Light::GetIHitable() const
{
	return static_cast<const IHitable*>(this);
}

ItemTypeEnum Light::GetItemType() const
{
	return eItemLight;
}

void Light::WriteRegDefaults()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	pRegUtil->SaveValueFloat("DefaultProps\\Light", "Falloff", m_d.m_falloff);
	pRegUtil->SaveValueFloat("DefaultProps\\Light", "FalloffPower", m_d.m_falloff_power);
	pRegUtil->SaveValueInt("DefaultProps\\Light", "LightState", m_d.m_state);
	pRegUtil->SaveValueBool("DefaultProps\\Light", "TimerEnabled", m_d.m_tdr.m_TimerEnabled);
	pRegUtil->SaveValueInt("DefaultProps\\Light", "TimerInterval", m_d.m_tdr.m_TimerInterval);
	pRegUtil->SaveValueInt("DefaultProps\\Light", "Color", m_d.m_color);
	pRegUtil->SaveValueInt("DefaultProps\\Light", "ColorFull", m_d.m_color2);
	pRegUtil->SaveValue("DefaultProps\\Light", "OffImage", m_d.m_szImage);
	pRegUtil->SaveValue("DefaultProps\\Light", "BlinkPattern", m_rgblinkpattern);
	pRegUtil->SaveValueInt("DefaultProps\\Light", "BlinkInterval", m_blinkinterval);
	pRegUtil->SaveValue("DefaultProps\\Light", "Surface", m_d.m_szSurface);
	pRegUtil->SaveValueFloat("DefaultProps\\Light", "FadeSpeedUp", m_d.m_fadeSpeedUp);
	pRegUtil->SaveValueFloat("DefaultProps\\Light", "FadeSpeedDown", m_d.m_fadeSpeedDown);
	pRegUtil->SaveValueFloat("DefaultProps\\Light", "Intensity", m_d.m_intensity);
	pRegUtil->SaveValueFloat("DefaultProps\\Light", "TransmissionScale", m_d.m_transmissionScale);
	pRegUtil->SaveValueBool("DefaultProps\\Light", "Bulb", m_d.m_BulbLight);
	pRegUtil->SaveValueBool("DefaultProps\\Light", "ImageMode", m_d.m_imageMode);
	pRegUtil->SaveValueBool("DefaultProps\\Light", "ShowBulbMesh", m_d.m_showBulbMesh);
	pRegUtil->SaveValueBool("DefaultProps\\Light", "StaticBulbMesh", m_d.m_staticBulbMesh);
	pRegUtil->SaveValueBool("DefaultProps\\Light", "ShowReflectionOnBall", m_d.m_showReflectionOnBall);
	pRegUtil->SaveValueFloat("DefaultProps\\Light", "ScaleBulbMesh", m_d.m_meshRadius);
	pRegUtil->SaveValueFloat("DefaultProps\\Light", "BulbModulateVsAdd", m_d.m_modulate_vs_add);
	pRegUtil->SaveValueFloat("DefaultProps\\Light", "BulbHaloHeight", m_d.m_bulbHaloHeight);
}

void Light::GetHitShapes(std::vector<HitObject*>& pvho)
{
}

void Light::GetHitShapesDebug(std::vector<HitObject*>& pvho)
{
}

void Light::RenderStatic()
{
}

void Light::RenderDynamic()
{
}

void Light::RenderSetup()
{
	// TODO: RenderDevice* const pd3dDevice = m_backglass ? g_pplayer->m_pin3d.m_pd3dSecondaryDevice : g_pplayer->m_pin3d.m_pd3dPrimaryDevice;

	// TODO: m_iblinkframe = 0;
	// TODO: m_d.m_time_msec = g_pplayer->m_time_msec;
	// TODO: m_updateBulbLightHeight = false;

	// TODO: m_initSurfaceHeight = m_ptable->GetSurfaceHeight(m_d.m_szSurface, m_d.m_vCenter.x, m_d.m_vCenter.y) * m_ptable->m_BG_scalez[m_ptable->m_BG_current_set];
	m_surfaceMaterial = m_ptable->GetSurfaceMaterial(m_d.m_szSurface);
	// TODO: m_surfaceTexture = m_ptable->GetSurfaceImage(m_d.m_szSurface);
}

ItemTypeEnum Light::HitableGetItemType() const
{
	return eItemLight;
}

IScriptable* Light::GetScriptable()
{
	return (IScriptable*)this;
}

wchar_t* Light::get_Name()
{
	return m_wzName;
}
