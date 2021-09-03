#include "LightSeq.h"
#include "RegUtil.h"

const ItemTypeEnum LightSeq::ItemType = eItemLightSeq;
const int LightSeq::TypeNameID = 314;
const int LightSeq::ToolID = 313;
const int LightSeq::CursorID = 315;
const unsigned LightSeq::AllowedViews = 3;

LightSeq* LightSeq::COMCreate()
{
	/*CComObject<LightSeq>* obj = 0;
	if ((((HRESULT)(CComObject<LightSeq>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new LightSeq();
}

IEditable* LightSeq::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* LightSeq::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	LightSeq* obj = LightSeq::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

LightSeq::LightSeq()
{
	m_lightseqanim.m_pLightSeq = this;
}

LightSeq::~LightSeq()
{
}

HRESULT LightSeq::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	m_d.m_vCenter.x = x;
	m_d.m_vCenter.y = y;

	SetDefaults(fromMouseClick);

	return InitVBA(true, 0, NULL);
}

HRESULT LightSeq::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			GetPTable()->GetUniqueName(eItemLightSeq, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t*)m_wzName,
			//             sizeof(m_wzName) / sizeof(m_wzName[0]));
		}
	}
	InitScript();
	return ((HRESULT)0L);
}

PinTable* LightSeq::GetPTable()
{
	return m_ptable;
}

HRESULT LightSeq::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	SetDefaults(false);

	m_ptable = pTable;

	BiffReader biffReader(pStream, this, pId, version);
	biffReader.Load();

	return S_OK;
}

void LightSeq::SetDefaults(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_d.m_updateinterval = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\LightSequence", "UpdateInterval", 25) : 25;

	std::string tmp;
	const HRESULT hr = pRegUtil->LoadValue("DefaultProps\\LightSequence", "Collection", tmp);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_d.m_wzCollection.clear();
	}
	else
	{
		wchar_t wtmp[MAXSTRING];
		// TODO: MultiByteToWideCharNull(CP_ACP, 0, tmp.c_str(), -1, wtmp, MAXSTRING);
		m_d.m_wzCollection = wtmp;
	}

	m_d.m_vCenter.x = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\LightSequence", "CenterX", EDITOR_BG_WIDTH / 2) : (EDITOR_BG_WIDTH / 2);
	m_d.m_vCenter.y = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\LightSequence", "CenterY", (2 * EDITOR_BG_WIDTH) / 2) : ((2 * EDITOR_BG_WIDTH) / 2);
	m_d.m_tdr.m_TimerEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\LightSequence", "TimerEnabled", false) : false;
	m_d.m_tdr.m_TimerInterval = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\LightSequence", "TimerInterval", 100) : 100;
}

bool LightSeq::LoadToken(const int id, BiffReader* const pbr)
{
	switch (id)
	{
	case FID(PIID):
		pbr->GetInt((int*)pbr->m_pData);
		break;
	case FID(VCEN):
		pbr->GetVector2(m_d.m_v);
		break;
	case FID(COLC):
		pbr->GetWideString(m_d.m_wzCollection);
		break;
	case FID(CTRX):
		pbr->GetFloat(m_d.m_vCenter.x);
		break;
	case FID(CTRY):
		pbr->GetFloat(m_d.m_vCenter.y);
		break;
	case FID(UPTM):
		pbr->GetInt(m_d.m_updateinterval);
		break;
	case FID(TMON):
		pbr->GetBool(m_d.m_tdr.m_TimerEnabled);
		break;
	case FID(TMIN):
		pbr->GetInt(m_d.m_tdr.m_TimerInterval);
		break;
	case FID(NAME):
		pbr->GetWideString(m_wzName, sizeof(m_wzName) / sizeof(m_wzName[0]));
		break;
	case FID(BGLS):
		pbr->GetBool(m_backglass);
		break;
	default:
	{
		ISelect::LoadToken(id, pbr);
		break;
	}
	}
	return true;
}

void LightSeq::WriteRegDefaults()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	char strTmp[MAXSTRING];
	// TODO: WideCharToMultiByteNull(CP_ACP, 0, m_d.m_wzCollection.c_str(), -1, strTmp, MAXSTRING, NULL, NULL);
	pRegUtil->SaveValueInt("DefaultProps\\LightSequence", "UpdateInterval", m_d.m_updateinterval);
	pRegUtil->SaveValue("DefaultProps\\LightSequence", "Collection", strTmp);
	pRegUtil->SaveValueFloat("DefaultProps\\LightSequence", "CenterX", m_d.m_vCenter.x);
	pRegUtil->SaveValueFloat("DefaultProps\\LightSequence", "CenterY", m_d.m_vCenter.y);
	pRegUtil->SaveValueBool("DefaultProps\\LightSequence", "TimerEnabled", m_d.m_tdr.m_TimerEnabled);
	pRegUtil->SaveValueInt("DefaultProps\\LightSequence", "TimerInterval", m_d.m_tdr.m_TimerInterval);
}