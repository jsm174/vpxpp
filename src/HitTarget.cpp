#include "HitTarget.h"
#include "RegUtil.h"
#include "cmath.h"

const ItemTypeEnum HitTarget::ItemType = eItemHitTarget;
const int HitTarget::TypeNameID = 134;
const int HitTarget::ToolID = 135;
const int HitTarget::CursorID = 147;
const unsigned HitTarget::AllowedViews = 1;

HitTarget* HitTarget::COMCreate()
{
	/*CComObject<HitTarget>* obj = 0;
	if ((((HRESULT)(CComObject<HitTarget>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new HitTarget();
}

IEditable* HitTarget::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* HitTarget::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	HitTarget* obj = HitTarget::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

HitTarget::HitTarget()
{
	m_vertexBuffer = NULL;
	m_indexBuffer = NULL;
	m_d.m_depthBias = 0.0f;
	m_d.m_reflectionEnabled = true;

	// TODO: m_propPhysics = NULL;
	// TODO: m_propPosition = NULL;
	// TODO: m_propVisual = NULL;
	m_d.m_overwritePhysics = true;
	m_vertices = NULL;
	m_indices = NULL;
	m_numIndices = 0;
	m_numVertices = 0;
	m_moveAnimation = false;
	m_moveDown = true;
	m_moveAnimationOffset = 0.0f;
	m_hitEvent = false;
	m_ptable = NULL;
	m_timeStamp = 0;
}

HitTarget::~HitTarget()
{
}

HRESULT HitTarget::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	m_d.m_vPosition.x = x;
	m_d.m_vPosition.y = y;

	SetDefaults(false);
	m_hitEvent = false;

	InitVBA(true, 0, NULL);

	// TODO: UpdateStatusBarInfo();

	return S_OK;
}

HRESULT HitTarget::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			GetPTable()->GetUniqueName(eItemHitTarget, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t*)m_wzName,
			//             sizeof(m_wzName) / sizeof(m_wzName[0]));
		}
	}
	InitScript();
	return ((HRESULT)0L);
}

PinTable* HitTarget::GetPTable()
{
	return m_ptable;
}

HRESULT HitTarget::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	SetDefaults(false);

	m_ptable = pTable;

	BiffReader biffReader(pStream, this, pId, version);
	biffReader.Load();

	// TODO: UpdateStatusBarInfo();

	return S_OK;
}

HRESULT HitTarget::InitPostLoad()
{
	// TODO: UpdateStatusBarInfo();

	return S_OK;
}

void HitTarget::SetDefaults(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	static const char strKeyName[] = "DefaultProps\\HitTarget";

	m_d.m_legacy = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "LegacyMode", false) : false;
	m_d.m_tdr.m_TimerEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "TimerEnabled", false) : false;
	m_d.m_tdr.m_TimerInterval = fromMouseClick ? pRegUtil->LoadValueIntWithDefault(strKeyName, "TimerInterval", 100) : 100;
	m_d.m_hitEvent = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "HitEvent", true) : true;
	m_d.m_visible = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "Visible", true) : true;
	m_d.m_isDropped = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "IsDropped", false) : false;

	m_d.m_vPosition.z = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "Position_Z", 0.0f) : 0.0f;

	m_d.m_vSize.x = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "ScaleX", 32.0f) : 32.0f;
	m_d.m_vSize.y = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "ScaleY", 32.0f) : 32.0f;
	m_d.m_vSize.z = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "ScaleZ", 32.0f) : 32.0f;

	m_d.m_rotZ = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "Orientation", 0.0f) : 0.0f;

	const HRESULT hr = pRegUtil->LoadValue(strKeyName, "Image", m_d.m_szImage);
	if ((hr != S_OK) && fromMouseClick)
	{
		m_d.m_szImage.clear();
	}

	m_d.m_targetType = fromMouseClick ? (TargetType)pRegUtil->LoadValueIntWithDefault(strKeyName, "TargetType", DropTargetSimple) : DropTargetSimple;
	m_d.m_threshold = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "HitThreshold", 2.0f) : 2.0f;

	if (m_d.m_targetType == DropTargetBeveled || m_d.m_targetType == DropTargetSimple || m_d.m_targetType == DropTargetFlatSimple)
	{
		m_d.m_dropSpeed = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "DropSpeed", 0.5f) : 0.5f;
	}
	else
	{
		m_d.m_dropSpeed = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "DropSpeed", 0.2f) : 0.2f;
	}

	SetDefaultPhysics(fromMouseClick);

	m_d.m_collidable = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "Collidable", true) : true;
	m_d.m_disableLightingTop = dequantizeUnsigned8(fromMouseClick ? pRegUtil->LoadValueIntWithDefault(strKeyName, "DisableLighting", 0) : 0); // stored as uchar for backward compatibility
	m_d.m_disableLightingBelow = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "DisableLightingBelow", 0.f) : 0.f;
	m_d.m_reflectionEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "ReflectionEnabled", true) : true;
	m_d.m_raiseDelay = fromMouseClick ? pRegUtil->LoadValueIntWithDefault(strKeyName, "RaiseDelay", 100) : 100;
}

void HitTarget::SetDefaultPhysics(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	static const char strKeyName[] = "DefaultProps\\HitTarget";

	m_d.m_elasticity = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "Elasticity", 0.35f) : 0.35f;
	m_d.m_elasticityFalloff = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "ElasticityFalloff", 0.5f) : 0.5f;
	m_d.m_friction = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "Friction", 0.2f) : 0.2f;
	m_d.m_scatter = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "Scatter", 5) : 5;
}

bool HitTarget::LoadToken(const int id, BiffReader* const pBiffReader)
{
	switch (id)
	{
	case FID(PIID):
		pBiffReader->GetInt((int*)pBiffReader->m_pData);
		break;
	case FID(VPOS):
		pBiffReader->GetVector3Padded(m_d.m_vPosition);
		break;
	case FID(VSIZ):
		pBiffReader->GetVector3Padded(m_d.m_vSize);
		break;
	case FID(ROTZ):
		pBiffReader->GetFloat(m_d.m_rotZ);
		break;
	case FID(IMAG):
		pBiffReader->GetString(m_d.m_szImage);
		break;
	case FID(TRTY):
		pBiffReader->GetInt(&m_d.m_targetType);
		break;
	case FID(NAME):
		pBiffReader->GetWideString(m_wzName, sizeof(m_wzName) / sizeof(wchar_t));
		break;
	case FID(MATR):
		pBiffReader->GetString(m_d.m_szMaterial);
		break;
	case FID(TVIS):
		pBiffReader->GetBool(m_d.m_visible);
		break;
	case FID(LEMO):
		pBiffReader->GetBool(m_d.m_legacy);
		break;
	case FID(ISDR):
		pBiffReader->GetBool(m_d.m_isDropped);
		break;
	case FID(DRSP):
		pBiffReader->GetFloat(m_d.m_dropSpeed);
		break;
	case FID(REEN):
		pBiffReader->GetBool(m_d.m_reflectionEnabled);
		break;
	case FID(HTEV):
		pBiffReader->GetBool(m_d.m_hitEvent);
		break;
	case FID(THRS):
		pBiffReader->GetFloat(m_d.m_threshold);
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
	case FID(DILI):
	{
		int tmp;
		pBiffReader->GetInt(tmp);
		m_d.m_disableLightingTop = (tmp == 1) ? 1.f : dequantizeUnsigned8(tmp);
		break;
	}
	case FID(DILB):
		pBiffReader->GetFloat(m_d.m_disableLightingBelow);
		break;
	case FID(PIDB):
		pBiffReader->GetFloat(m_d.m_depthBias);
		break;
	case FID(TMON):
		pBiffReader->GetBool(m_d.m_tdr.m_TimerEnabled);
		break;
	case FID(TMIN):
		pBiffReader->GetInt(m_d.m_tdr.m_TimerInterval);
		break;
	case FID(RADE):
		pBiffReader->GetInt(m_d.m_raiseDelay);
		break;
	case FID(MAPH):
		pBiffReader->GetString(m_d.m_szPhysicsMaterial);
		break;
	case FID(OVPH):
		pBiffReader->GetBool(m_d.m_overwritePhysics);
		break;
	default:
		ISelect::LoadToken(id, pBiffReader);
		break;
	}
	return true;
}

void HitTarget::WriteRegDefaults()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	static const char strKeyName[] = "DefaultProps\\HitTarget";

	pRegUtil->SaveValueBool(strKeyName, "LegacyMode", m_d.m_legacy);
	pRegUtil->SaveValueBool(strKeyName, "TimerEnabled", m_d.m_tdr.m_TimerEnabled);
	pRegUtil->SaveValueInt(strKeyName, "TimerInterval", m_d.m_tdr.m_TimerInterval);
	pRegUtil->SaveValueBool(strKeyName, "Visible", m_d.m_visible);
	pRegUtil->SaveValueBool(strKeyName, "IsDropped", m_d.m_isDropped);

	pRegUtil->SaveValueFloat(strKeyName, "Position_Z", m_d.m_vPosition.z);
	pRegUtil->SaveValueFloat(strKeyName, "DropSpeed", m_d.m_dropSpeed);

	pRegUtil->SaveValueFloat(strKeyName, "ScaleX", m_d.m_vSize.x);
	pRegUtil->SaveValueFloat(strKeyName, "ScaleY", m_d.m_vSize.y);
	pRegUtil->SaveValueFloat(strKeyName, "ScaleZ", m_d.m_vSize.z);

	pRegUtil->SaveValueFloat(strKeyName, "Orientation", m_d.m_rotZ);

	pRegUtil->SaveValue(strKeyName, "Image", m_d.m_szImage);
	pRegUtil->SaveValueBool(strKeyName, "HitEvent", m_d.m_hitEvent);
	pRegUtil->SaveValueFloat(strKeyName, "HitThreshold", m_d.m_threshold);
	pRegUtil->SaveValueFloat(strKeyName, "Elasticity", m_d.m_elasticity);
	pRegUtil->SaveValueFloat(strKeyName, "ElasticityFalloff", m_d.m_elasticityFalloff);
	pRegUtil->SaveValueFloat(strKeyName, "Friction", m_d.m_friction);
	pRegUtil->SaveValueFloat(strKeyName, "Scatter", m_d.m_scatter);

	pRegUtil->SaveValueInt(strKeyName, "TargetType", m_d.m_targetType);

	pRegUtil->SaveValueBool(strKeyName, "Collidable", m_d.m_collidable);
	const int tmp = quantizeUnsigned8(clamp(m_d.m_disableLightingTop, 0.f, 1.f));
	pRegUtil->SaveValueInt(strKeyName, "DisableLighting", (tmp == 1) ? 0 : tmp); // backwards compatible saving
	pRegUtil->SaveValueFloat(strKeyName, "DisableLightingBelow", m_d.m_disableLightingBelow);
	pRegUtil->SaveValueBool(strKeyName, "ReflectionEnabled", m_d.m_reflectionEnabled);
	pRegUtil->SaveValueInt(strKeyName, "RaiseDelay", m_d.m_raiseDelay);
}