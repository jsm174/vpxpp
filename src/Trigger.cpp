#include "Trigger.h"
#include "RegUtil.h"

const ItemTypeEnum Trigger::ItemType = eItemTrigger;
const int Trigger::TypeNameID = 127;
const int Trigger::ToolID = 128;
const int Trigger::CursorID = 140;
const unsigned Trigger::AllowedViews = 1;

Trigger* Trigger::COMCreate()
{
	/*CComObject<Trigger>* obj = 0;
	if ((((HRESULT)(CComObject<Trigger>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Trigger();
}

IEditable* Trigger::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Trigger::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Trigger* obj = Trigger::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Trigger::Trigger()
{
	// TODO: m_ptriggerhitcircle = NULL;

	m_hitEvent = false;
	m_unhitEvent = false;
	m_doAnimation = false;
	m_moveDown = false;
	m_animHeightOffset = 0.0f;
	m_vertexBuffer_animHeightOffset = -FLT_MAX;

	m_hitEnabled = true;
	m_vertexBuffer = NULL;
	m_triggerIndexBuffer = NULL;
	m_triggerVertices = NULL;
	// TODO: m_menuid = IDR_SURFACEMENU;
	// TODO: m_propVisual = NULL;
}

Trigger::~Trigger()
{
}

HRESULT Trigger::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	m_d.m_vCenter.x = x;
	m_d.m_vCenter.y = y;

	SetDefaults(fromMouseClick);

	// TODO: if (m_vdpoint.empty())
	// {
	// 	InitShape(x, y);
	// }

	return InitVBA(true, 0, NULL);
}

HRESULT Trigger::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			GetPTable()->GetUniqueName(eItemTrigger, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t*)m_wzName,
			//             sizeof(m_wzName) / sizeof(m_wzName[0]));
		}
	}
	InitScript();
	return ((HRESULT)0L);
}

PinTable* Trigger::GetPTable()
{
	return m_ptable;
}

HRESULT Trigger::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	SetDefaults(false);

	m_ptable = pTable;

	BiffReader biffReader(pStream, this, pId, version);
	biffReader.Load();

	// TODO: UpdateStatusBarInfo();

	return S_OK;
}

HRESULT Trigger::InitPostLoad()
{
	// TODO: UpdateStatusBarInfo();
	return S_OK;
}

void Trigger::SetDefaults(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_d.m_radius = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Trigger", "Radius", 25.0f) : 25.0f;
	m_d.m_rotation = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Trigger", "Rotation", 0.f) : 0.f;
	m_d.m_wireThickness = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Trigger", "WireThickness", 0.f) : 0.f;
	m_d.m_scaleX = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Trigger", "ScaleX", 1.f) : 1.f;
	m_d.m_scaleY = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Trigger", "ScaleY", 1.f) : 1.f;
	m_d.m_tdr.m_TimerEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Trigger", "TimerEnabled", false) : false;
	m_d.m_tdr.m_TimerInterval = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\Trigger", "TimerInterval", 100) : 100;
	m_d.m_enabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Trigger", "Enabled", true) : true;
	m_d.m_visible = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Trigger", "Visible", true) : true;
	m_d.m_hit_height = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Trigger", "HitHeight", 50.f) : 50.f;
	m_d.m_shape = fromMouseClick ? (TriggerShape)pRegUtil->LoadValueIntWithDefault("DefaultProps\\Trigger", "Shape", TriggerWireA) : TriggerWireA;

	HRESULT hr = pRegUtil->LoadValue("DefaultProps\\Trigger", "Surface", m_d.m_szSurface);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_d.m_szSurface.clear();
	}

	m_d.m_animSpeed = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\trigger", "AnimSpeed", 1.f) : 1.f;
	m_d.m_reflectionEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Trigger", "ReflectionEnabled", true) : true;
}

bool Trigger::LoadToken(const int id, BiffReader* const pBiffReader)
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
	case FID(ROTA):
		pBiffReader->GetFloat(m_d.m_rotation);
		break;
	case FID(WITI):
		pBiffReader->GetFloat(m_d.m_wireThickness);
		break;
	case FID(SCAX):
		pBiffReader->GetFloat(m_d.m_scaleX);
		break;
	case FID(SCAY):
		pBiffReader->GetFloat(m_d.m_scaleY);
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
	case FID(SURF):
		pBiffReader->GetString(m_d.m_szSurface);
		break;
	case FID(EBLD):
		pBiffReader->GetBool(m_d.m_enabled);
		break;
	case FID(THOT):
		pBiffReader->GetFloat(m_d.m_hit_height);
		break;
	case FID(VSBL):
		pBiffReader->GetBool(m_d.m_visible);
		break;
	case FID(REEN):
		pBiffReader->GetBool(m_d.m_reflectionEnabled);
		break;
	case FID(SHAP):
		pBiffReader->GetInt(&m_d.m_shape);
		break;
	case FID(ANSP):
		pBiffReader->GetFloat(m_d.m_animSpeed);
		break;
	case FID(NAME):
		pBiffReader->GetWideString(m_wzName, sizeof(m_wzName) / sizeof(wchar_t));
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

void Trigger::WriteRegDefaults()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	pRegUtil->SaveValueBool("DefaultProps\\Trigger", "TimerEnabled", m_d.m_tdr.m_TimerEnabled);
	pRegUtil->SaveValueInt("DefaultProps\\Trigger", "TimerInterval", m_d.m_tdr.m_TimerInterval);
	pRegUtil->SaveValueBool("DefaultProps\\Trigger", "Enabled", m_d.m_enabled);
	pRegUtil->SaveValueBool("DefaultProps\\Trigger", "Visible", m_d.m_visible);
	pRegUtil->SaveValueFloat("DefaultProps\\Trigger", "HitHeight", m_d.m_hit_height);
	pRegUtil->SaveValueFloat("DefaultProps\\Trigger", "Radius", m_d.m_radius);
	pRegUtil->SaveValueFloat("DefaultProps\\Trigger", "Rotation", m_d.m_rotation);
	pRegUtil->SaveValueFloat("DefaultProps\\Trigger", "WireThickness", m_d.m_wireThickness);
	pRegUtil->SaveValueFloat("DefaultProps\\Trigger", "ScaleX", m_d.m_scaleX);
	pRegUtil->SaveValueFloat("DefaultProps\\Trigger", "ScaleY", m_d.m_scaleY);
	pRegUtil->SaveValueInt("DefaultProps\\Trigger", "Shape", m_d.m_shape);
	pRegUtil->SaveValue("DefaultProps\\Trigger", "Surface", m_d.m_szSurface);
	pRegUtil->SaveValueFloat("DefaultProps\\Trigger", "AnimSpeed", m_d.m_animSpeed);
	pRegUtil->SaveValueBool("DefaultProps\\Trigger", "ReflectionEnabled", m_d.m_reflectionEnabled);
}