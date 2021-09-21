#include "Gate.h"
#include "RegUtil.h"

const ItemTypeEnum Gate::ItemType = eItemGate;
const int Gate::TypeNameID = 139;
const int Gate::ToolID = 140;
const int Gate::CursorID = 141;
const unsigned Gate::AllowedViews = 1;

Gate* Gate::COMCreate()
{
	/*CComObject<Gate>* obj = 0;
	if ((((HRESULT)(CComObject<Gate>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Gate();
}

IEditable* Gate::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Gate::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Gate* obj = Gate::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Gate::Gate()
{
	// TODO: m_phitgate = NULL;
	// TODO: m_plineseg = NULL;
	m_bracketIndexBuffer = NULL;
	m_bracketVertexBuffer = NULL;
	m_wireIndexBuffer = NULL;
	m_wireVertexBuffer = NULL;
	m_vertexbuffer_angle = FLT_MAX;
	m_d.m_type = GateWireW;
	m_vertices = 0;
	m_indices = 0;
	m_numIndices = 0;
	m_numVertices = 0;
}

Gate::~Gate()
{
}

HRESULT Gate::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	m_d.m_vCenter.x = x;
	m_d.m_vCenter.y = y;

	SetDefaults(fromMouseClick);

	InitVBA(true, 0, NULL);

	return S_OK;
}

HRESULT Gate::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			//GetPTable()->GetUniqueName(eItemGate, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t *)m_wzName, sizeof(m_wzName)/sizeof(m_wzName[0]));/*lstrcpyW((WCHAR *)m_wzName, wzUniqueName);*/
		}
	}
	//InitScript();
	return S_OK;
}

PinTable* Gate::GetPTable()
{
	return m_ptable;
}

HRESULT Gate::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	SetDefaults(false);

	m_ptable = pTable;

	BiffReader biffReader(pStream, this, pId, version);
	biffReader.Load();

	return S_OK;
}

HRESULT Gate::InitPostLoad()
{
	return S_OK;
}

void Gate::SetDefaults(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_d.m_length = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Gate", "Length", 100.f) : 100.f;
	m_d.m_height = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Gate", "Height", 50.f) : 50.f;
	m_d.m_rotation = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Gate", "Rotation", -90.f) : -90.f;
	m_d.m_showBracket = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Gate", "ShowBracket", true) : true;
	m_d.m_type = fromMouseClick ? (GateType)pRegUtil->LoadValueIntWithDefault("DefaultProps\\Gate", "GateType", GateWireW) : GateWireW;
	m_d.m_collidable = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Gate", "Collidable", true) : true;
	m_d.m_angleMin = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Gate", "AngleMin", 0.f) : 0.f;
	m_d.m_angleMax = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Gate", "AngleMax", (float)(M_PI / 2.0)) : (float)(M_PI / 2.0);
	m_d.m_visible = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Gate", "Visible", true) : true;
	m_d.m_tdr.m_TimerEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Gate", "TimerEnabled", false) : false;
	m_d.m_tdr.m_TimerInterval = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\Gate", "TimerInterval", 100) : 100;

	const HRESULT hr = pRegUtil->LoadValue("DefaultProps\\Gate", "Surface", m_d.m_szSurface);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_d.m_szSurface.clear();
	}

	SetDefaultPhysics(fromMouseClick);

	m_d.m_twoWay = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Gate", "TwoWay", true) : true;
	m_d.m_reflectionEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Gate", "ReflectionEnabled", true) : true;
}

void Gate::SetDefaultPhysics(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_d.m_elasticity = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Gate", "Elasticity", 0.3f) : 0.3f;
	m_d.m_friction = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Gate", "Friction", 0.02f) : 0.02f;
	m_d.m_damping = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Gate", "AntiFriction", 0.985f) : 0.985f;
	m_d.m_scatter = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Gate", "Scatter", 0.f) : 0.f;
	m_d.m_gravityfactor = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Gate", "GravityFactor", 0.25f) : 0.25f;
}

bool Gate::LoadToken(const int id, BiffReader* const pBiffReader)
{
	switch (id)
	{
	case FID(PIID):
		pBiffReader->GetInt((int*)pBiffReader->m_pData);
		break;
	case FID(GATY):
	{
		pBiffReader->GetInt(&m_d.m_type);
		if (m_d.m_type < GateWireW || m_d.m_type > GateLongPlate)
		{
			m_d.m_type = GateWireW;
		}
		break;
	}
	case FID(VCEN):
		pBiffReader->GetVector2(m_d.m_vCenter);
		break;
	case FID(LGTH):
		pBiffReader->GetFloat(m_d.m_length);
		break;
	case FID(HGTH):
		pBiffReader->GetFloat(m_d.m_height);
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
	case FID(GSUP):
		pBiffReader->GetBool(m_d.m_showBracket);
		break;
	case FID(GCOL):
		pBiffReader->GetBool(m_d.m_collidable);
		break;
	case FID(TWWA):
		pBiffReader->GetBool(m_d.m_twoWay);
		break;
	case FID(GVSB):
		pBiffReader->GetBool(m_d.m_visible);
		break;
	case FID(REEN):
		pBiffReader->GetBool(m_d.m_reflectionEnabled);
		break;
	case FID(TMIN):
		pBiffReader->GetInt(m_d.m_tdr.m_TimerInterval);
		break;
	case FID(SURF):
		pBiffReader->GetString(m_d.m_szSurface);
		break;
	case FID(NAME):
		pBiffReader->GetWideString(m_wzName, sizeof(m_wzName) / sizeof(wchar_t));
		break;
	case FID(ELAS):
		pBiffReader->GetFloat(m_d.m_elasticity);
		break;
	case FID(GAMA):
		pBiffReader->GetFloat(m_d.m_angleMax);
		break;
	case FID(GAMI):
		pBiffReader->GetFloat(m_d.m_angleMin);
		break;
	case FID(GFRC):
		pBiffReader->GetFloat(m_d.m_friction);
		break;
	case FID(AFRC):
		pBiffReader->GetFloat(m_d.m_damping);
		break;
	case FID(GGFC):
		pBiffReader->GetFloat(m_d.m_gravityfactor);
		break;
	default:
		ISelect::LoadToken(id, pBiffReader);
		break;
	}
	return true;
}

ItemTypeEnum Gate::GetItemType() const
{
	return eItemGate;
}

IEditable* Gate::GetIEditable()
{
	return static_cast<IEditable*>(this);
}

IHitable* Gate::GetIHitable()
{
	return static_cast<IHitable*>(this);
}

void Gate::WriteRegDefaults()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	pRegUtil->SaveValueFloat("DefaultProps\\Gate", "Length", m_d.m_length);
	pRegUtil->SaveValueFloat("DefaultProps\\Gate", "Height", m_d.m_height);
	pRegUtil->SaveValueFloat("DefaultProps\\Gate", "Rotation", m_d.m_rotation);
	pRegUtil->SaveValueBool("DefaultProps\\Gate", "ShowBracket", m_d.m_showBracket);
	pRegUtil->SaveValueBool("DefaultProps\\Gate", "Collidable", m_d.m_collidable);
	pRegUtil->SaveValueFloat("DefaultProps\\Gate", "AngleMin", m_d.m_angleMin);
	pRegUtil->SaveValueFloat("DefaultProps\\Gate", "AngleMax", m_d.m_angleMax);
	pRegUtil->SaveValueBool("DefaultProps\\Gate", "Visible", m_d.m_visible);
	pRegUtil->SaveValueBool("DefaultProps\\Gate", "TimerEnabled", m_d.m_tdr.m_TimerEnabled);
	pRegUtil->SaveValueInt("DefaultProps\\Gate", "TimerInterval", m_d.m_tdr.m_TimerInterval);
	pRegUtil->SaveValue("DefaultProps\\Gate", "Surface", m_d.m_szSurface);
	pRegUtil->SaveValueFloat("DefaultProps\\Gate", "Elasticity", m_d.m_elasticity);
	pRegUtil->SaveValueFloat("DefaultProps\\Gate", "Friction", m_d.m_friction);
	pRegUtil->SaveValueFloat("DefaultProps\\Gate", "Scatter", m_d.m_scatter);
	pRegUtil->SaveValueFloat("DefaultProps\\Gate", "GravityFactor", m_d.m_gravityfactor);
	pRegUtil->SaveValueBool("DefaultProps\\Gate", "TwoWay", m_d.m_twoWay);
	pRegUtil->SaveValueBool("DefaultProps\\Gate", "ReflectionEnabled", m_d.m_reflectionEnabled);
	pRegUtil->SaveValueInt("DefaultProps\\Gate", "GateType", m_d.m_type);
}

void Gate::GetHitShapes(std::vector<HitObject*>& pvho)
{
}

void Gate::GetHitShapesDebug(std::vector<HitObject*>& pvho)
{
}

void Gate::RenderStatic()
{
}

void Gate::RenderDynamic()
{
}

void Gate::RenderSetup()
{
}

ItemTypeEnum Gate::HitableGetItemType() const
{
	return eItemGate;
}