#include "Kicker.h"
#include "RegUtil.h"

const ItemTypeEnum Kicker::ItemType = eItemKicker;
const int Kicker::TypeNameID = 133;
const int Kicker::ToolID = 134;
const int Kicker::CursorID = 146;
const unsigned Kicker::AllowedViews = 1;

Kicker* Kicker::COMCreate()
{
	/*CComObject<Kicker>* obj = 0;
	if ((((HRESULT)(CComObject<Kicker>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Kicker();
}

IEditable* Kicker::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Kicker::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Kicker* obj = Kicker::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Kicker::Kicker()
{
	// TODO: m_phitkickercircle = NULL;
	m_vertexBuffer = NULL;
	m_indexBuffer = NULL;
	m_plateVertexBuffer = NULL;
	m_plateIndexBuffer = NULL;
	m_ptable = NULL;
	m_numVertices = 0;
	m_numIndices = 0;
	m_baseHeight = 0.0f;
}

Kicker::~Kicker()
{
}

HRESULT Kicker::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	m_d.m_vCenter.x = x;
	m_d.m_vCenter.y = y;

	SetDefaults(fromMouseClick);

	return InitVBA(true, 0, NULL);
}

HRESULT Kicker::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		GetPTable()->GetUniqueName(eItemKicker, wzUniqueName, 128);
		wcsncpy(m_wzName, wzUniqueName, MAXNAMEBUFFER);
	}
	InitScript();
	return ((HRESULT)0L);
}

HRESULT Kicker::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	SetDefaults(false);

	m_ptable = pTable;

	BiffReader biffReader(pStream, this, pId, version);
	biffReader.Load();

	return S_OK;
}

HRESULT Kicker::InitPostLoad()
{
	// TODO: m_phitkickercircle = NULL;
	return S_OK;
}

void Kicker::SetDefaults(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_d.m_radius = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Kicker", "Radius", 25.f) : 25.f;
	m_d.m_tdr.m_TimerEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Kicker", "TimerEnabled", false) : false;
	m_d.m_tdr.m_TimerInterval = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\Kicker", "TimerInterval", 100) : 100;
	m_d.m_enabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Kicker", "Enabled", true) : true;
	m_d.m_hitAccuracy = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Kicker", "HitAccuracy", 0.5f) : 0.5f;
	m_d.m_hit_height = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Kicker", "HitHeight", 35.0f) : 35.0f;
	m_d.m_orientation = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Kicker", "Orientation", 0.f) : 0.f;

	SetDefaultPhysics(fromMouseClick);

	const HRESULT hr = pRegUtil->LoadValue("DefaultProps\\Kicker", "Surface", m_d.m_szSurface);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_d.m_szSurface.clear();
	}

	m_d.m_kickertype = fromMouseClick ? (KickerType)pRegUtil->LoadValueIntWithDefault("DefaultProps\\Kicker", "KickerType", KickerHole) : KickerHole;

	if (m_d.m_kickertype > KickerCup2)
	{
		m_d.m_kickertype = KickerInvisible;
	}

	m_d.m_fallThrough = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Kicker", "FallThrough", false) : false;
	m_d.m_legacyMode = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Kicker", "Legacy", true) : true;
}

void Kicker::SetDefaultPhysics(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_d.m_scatter = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Kicker", "Scatter", 0.f) : 0.f;
}

bool Kicker::LoadToken(const int id, BiffReader* const pBiffReader)
{
	switch (id)
	{
	case FID(PIID):
		pBiffReader->GetInt((int*)pBiffReader->m_pData);
		break;
	case FID(VCEN):
		pBiffReader->GetStruct(&m_d.m_vCenter, sizeof(Vertex2D));
		break;
	case FID(RADI):
		pBiffReader->GetFloat(m_d.m_radius);
		break;
	case FID(KSCT):
		pBiffReader->GetFloat(m_d.m_scatter);
		break;
	case FID(KHAC):
		pBiffReader->GetFloat(m_d.m_hitAccuracy);
		break;
	case FID(KHHI):
		pBiffReader->GetFloat(m_d.m_hit_height);
		break;
	case FID(KORI):
		pBiffReader->GetFloat(m_d.m_orientation);
		break;
	case FID(MATR):
		pBiffReader->GetString(m_d.m_szMaterial);
		break;
	case FID(TMON):
		pBiffReader->GetBool(m_d.m_tdr.m_TimerEnabled);
		break;
	case FID(EBLD):
		pBiffReader->GetBool(m_d.m_enabled);
		break;
	case FID(TMIN):
		pBiffReader->GetInt(m_d.m_tdr.m_TimerInterval);
		break;
	case FID(TYPE):
	{
		pBiffReader->GetInt(&m_d.m_kickertype);
		if (m_d.m_kickertype > KickerCup2)
		{
			m_d.m_kickertype = KickerInvisible;
		}
		break;
	}
	case FID(SURF):
		pBiffReader->GetString(m_d.m_szSurface);
		break;
	case FID(NAME):
		pBiffReader->GetWideString(m_wzName, sizeof(m_wzName) / sizeof(wchar_t));
		break;
	case FID(FATH):
		pBiffReader->GetBool(m_d.m_fallThrough);
		break;
	case FID(LEMO):
		pBiffReader->GetBool(m_d.m_legacyMode);
		break;
	default:
		ISelect::LoadToken(id, pBiffReader);
		break;
	}
	return true;
}

PinTable* Kicker::GetPTable()
{
	return m_ptable;
}

const PinTable* Kicker::GetPTable() const
{
	return m_ptable;
}

IEditable* Kicker::GetIEditable()
{
	return static_cast<IEditable*>(this);
}

const IEditable* Kicker::GetIEditable() const
{
	return static_cast<const IEditable*>(this);
}

ISelect* Kicker::GetISelect()
{
	return static_cast<ISelect*>(this);
}

const ISelect* Kicker::GetISelect() const
{
	return static_cast<const ISelect*>(this);
}

IHitable* Kicker::GetIHitable()
{
	return static_cast<IHitable*>(this);
}

const IHitable* Kicker::GetIHitable() const
{
	return static_cast<const IHitable*>(this);
}

ItemTypeEnum Kicker::GetItemType() const
{
	return eItemKicker;
}

void Kicker::WriteRegDefaults()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	pRegUtil->SaveValueBool("DefaultProps\\Kicker", "TimerEnabled", m_d.m_tdr.m_TimerEnabled);
	pRegUtil->SaveValueInt("DefaultProps\\Kicker", "TimerInterval", m_d.m_tdr.m_TimerInterval);
	pRegUtil->SaveValueBool("DefaultProps\\Kicker", "Enabled", m_d.m_enabled);
	pRegUtil->SaveValueFloat("DefaultProps\\Kicker", "HitAccuracy", m_d.m_hitAccuracy);
	pRegUtil->SaveValueFloat("DefaultProps\\Kicker", "HitHeight", m_d.m_hit_height);
	pRegUtil->SaveValueFloat("DefaultProps\\Kicker", "Orientation", m_d.m_orientation);
	pRegUtil->SaveValueFloat("DefaultProps\\Kicker", "Radius", m_d.m_radius);
	pRegUtil->SaveValueFloat("DefaultProps\\Kicker", "Scatter", m_d.m_scatter);
	pRegUtil->SaveValueInt("DefaultProps\\Kicker", "KickerType", m_d.m_kickertype);
	pRegUtil->SaveValue("DefaultProps\\Kicker", "Surface", m_d.m_szSurface);
	pRegUtil->SaveValueBool("DefaultProps\\Kicker", "FallThrough", m_d.m_fallThrough);
	pRegUtil->SaveValueBool("DefaultProps\\Kicker", "Legacy", m_d.m_legacyMode);
}

void Kicker::GetHitShapes(std::vector<HitObject*>& pvho)
{
}

void Kicker::GetHitShapesDebug(std::vector<HitObject*>& pvho)
{
}

void Kicker::PreRenderStatic(RenderDevice* pd3dDevice)
{
}

void Kicker::RenderStatic()
{
}

void Kicker::RenderDynamic()
{
}

void Kicker::RenderSetup()
{
}

ItemTypeEnum Kicker::HitableGetItemType() const
{
	return eItemKicker;
}

IScriptable* Kicker::GetScriptable()
{
	return (IScriptable*)this;
}

wchar_t* Kicker::get_Name()
{
	return m_wzName;
}