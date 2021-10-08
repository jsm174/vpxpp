#include "Spinner.h"
#include "RegUtil.h"

const ItemTypeEnum Spinner::ItemType = eItemSpinner;
const int Spinner::TypeNameID = 142;
const int Spinner::ToolID = 143;
const int Spinner::CursorID = 149;
const unsigned Spinner::AllowedViews = 1;

Spinner* Spinner::COMCreate()
{
	/*CComObject<Spinner>* obj = 0;
	if ((((HRESULT)(CComObject<Spinner>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Spinner();
}

IEditable* Spinner::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Spinner::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Spinner* obj = Spinner::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Spinner::Spinner()
{
	// TODO: m_phitspinner = NULL;
	m_bracketVertexBuffer = NULL;
	m_bracketIndexBuffer = NULL;
	m_plateVertexBuffer = NULL;
	m_plateIndexBuffer = NULL;
	m_vertexBuffer_spinneranimangle = -FLT_MAX;
}

Spinner::~Spinner()
{
}

HRESULT Spinner::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	m_d.m_vCenter.x = x;
	m_d.m_vCenter.y = y;

	SetDefaults(fromMouseClick);

	InitVBA(true, 0, NULL);

	return S_OK;
}

HRESULT Spinner::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		GetPTable()->GetUniqueName(eItemSpinner, wzUniqueName, 128);
		wcsncpy(m_wzName, wzUniqueName, MAXNAMEBUFFER);
	}
	InitScript();
	return ((HRESULT)0L);
}

HRESULT Spinner::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	SetDefaults(false);

	m_ptable = pTable;

	BiffReader biffReader(pStream, this, pId, version);
	biffReader.Load();

	return S_OK;
}

HRESULT Spinner::InitPostLoad()
{
	return S_OK;
}

void Spinner::SetDefaults(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_d.m_length = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Spinner", "Length", 80.f) : 80.f;
	m_d.m_rotation = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Spinner", "Rotation", 0.f) : 0.f;
	m_d.m_showBracket = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Spinner", "ShowBracket", true) : true;
	m_d.m_height = (float)(fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\Spinner", "Height", 60000) : 60000) / 1000.0f;

	SetDefaultPhysics(fromMouseClick);

	m_d.m_angleMax = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Spinner", "AngleMax", 0.f) : 0.f;
	m_d.m_angleMin = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Spinner", "AngleMin", 0.f) : 0.f;
	m_d.m_visible = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Spinner", "Visible", true) : true;
	m_d.m_reflectionEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Spinner", "ReflectionEnabled", true) : true;
	m_d.m_tdr.m_TimerEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Spinner", "TimerEnabled", false) : false;
	m_d.m_tdr.m_TimerInterval = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\Spinner", "TimerInterval", 100) : 100;

	HRESULT hr = pRegUtil->LoadValue("DefaultProps\\Spinner", "Image", m_d.m_szImage);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_d.m_szImage.clear();
	}

	hr = pRegUtil->LoadValue("DefaultProps\\Spinner", "Surface", m_d.m_szSurface);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_d.m_szSurface.clear();
	}
}

void Spinner::SetDefaultPhysics(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_d.m_elasticity = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Spinner", "Elasticity", 0.3f) : 0.3f;
	m_d.m_damping = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Spinner", "AntiFriction", 0.9879f) : 0.9879f;
}

bool Spinner::LoadToken(const int id, BiffReader* const pBiffReader)
{
	switch (id)
	{
	case FID(PIID):
		pBiffReader->GetInt((int*)pBiffReader->m_pData);
		break;
	case FID(VCEN):
		pBiffReader->GetVector2(m_d.m_vCenter);
		break;
	case FID(ROTA):
		pBiffReader->GetFloat(m_d.m_rotation);
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
	case FID(SSUP):
		pBiffReader->GetBool(m_d.m_showBracket);
		break;
	case FID(HIGH):
		pBiffReader->GetFloat(m_d.m_height);
		break;
	case FID(LGTH):
		pBiffReader->GetFloat(m_d.m_length);
		break;
	case FID(AFRC):
		pBiffReader->GetFloat(m_d.m_damping);
		break;
	case FID(SMAX):
		pBiffReader->GetFloat(m_d.m_angleMax);
		break;
	case FID(SMIN):
		pBiffReader->GetFloat(m_d.m_angleMin);
		break;
	case FID(SELA):
		pBiffReader->GetFloat(m_d.m_elasticity);
		break;
	case FID(SVIS):
		pBiffReader->GetBool(m_d.m_visible);
		break;
	case FID(IMGF):
		pBiffReader->GetString(m_d.m_szImage);
		break;
	case FID(SURF):
		pBiffReader->GetString(m_d.m_szSurface);
		break;
	case FID(NAME):
		pBiffReader->GetWideString(m_wzName, sizeof(m_wzName) / sizeof(m_wzName[0]));
		break;
	default:
		ISelect::LoadToken(id, pBiffReader);
		break;
	}
	return true;
}

PinTable* Spinner::GetPTable()
{
	return m_ptable;
}

const PinTable* Spinner::GetPTable() const
{
	return m_ptable;
}

IEditable* Spinner::GetIEditable()
{
	return static_cast<IEditable*>(this);
}

const IEditable* Spinner::GetIEditable() const
{
	return static_cast<const IEditable*>(this);
}

ISelect* Spinner::GetISelect()
{
	return static_cast<ISelect*>(this);
}

const ISelect* Spinner::GetISelect() const
{
	return static_cast<const ISelect*>(this);
}

IHitable* Spinner::GetIHitable()
{
	return static_cast<IHitable*>(this);
}

const IHitable* Spinner::GetIHitable() const
{
	return static_cast<const IHitable*>(this);
}

ItemTypeEnum Spinner::GetItemType() const
{
	return eItemSpinner;
}

void Spinner::WriteRegDefaults()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	pRegUtil->SaveValueFloat("DefaultProps\\Spinner", "Length", m_d.m_length);
	pRegUtil->SaveValueFloat("DefaultProps\\Spinner", "Rotation", m_d.m_rotation);
	pRegUtil->SaveValueBool("DefaultProps\\Spinner", "ShowBracket", m_d.m_showBracket);
	pRegUtil->SaveValueFloat("DefaultProps\\Spinner", "Height", m_d.m_height);
	pRegUtil->SaveValueFloat("DefaultProps\\Spinner", "AngleMax", m_d.m_angleMax);
	pRegUtil->SaveValueFloat("DefaultProps\\Spinner", "AngleMin", m_d.m_angleMin);
	pRegUtil->SaveValueFloat("DefaultProps\\Spinner", "Elasticity", m_d.m_elasticity);
	pRegUtil->SaveValueFloat("DefaultProps\\Spinner", "AntiFriction", m_d.m_damping);
	pRegUtil->SaveValueFloat("DefaultProps\\Spinner", "Scatter", m_d.m_scatter);
	pRegUtil->SaveValueBool("DefaultProps\\Spinner", "Visible", m_d.m_visible);
	pRegUtil->SaveValueBool("DefaultProps\\Spinner", "TimerEnabled", m_d.m_tdr.m_TimerEnabled);
	pRegUtil->SaveValueInt("DefaultProps\\Spinner", "TimerInterval", m_d.m_tdr.m_TimerInterval);
	pRegUtil->SaveValue("DefaultProps\\Spinner", "Image", m_d.m_szImage);
	pRegUtil->SaveValue("DefaultProps\\Spinner", "Surface", m_d.m_szSurface);
	pRegUtil->SaveValueBool("DefaultProps\\Spinner", "ReflectionEnabled", m_d.m_reflectionEnabled);
}

void Spinner::GetHitShapes(std::vector<HitObject*>& pvho)
{
}

void Spinner::GetHitShapesDebug(std::vector<HitObject*>& pvho)
{
}

void Spinner::RenderStatic()
{
}

void Spinner::RenderDynamic()
{
}

void Spinner::RenderSetup()
{
}

ItemTypeEnum Spinner::HitableGetItemType() const
{
	return eItemSpinner;
}

IScriptable* Spinner::GetScriptable()
{
	return (IScriptable*)this;
}

wchar_t* Spinner::get_Name()
{
	return m_wzName;
}