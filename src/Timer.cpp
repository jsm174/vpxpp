#include "Timer.h"
#include "RegUtil.h"

const ItemTypeEnum Timer::ItemType = eItemTimer;
const int Timer::TypeNameID = 101;
const int Timer::ToolID = 114;
const int Timer::CursorID = 142;
const unsigned Timer::AllowedViews = 3;

Timer* Timer::COMCreate()
{
	//CComObject<Timer>* obj = 0;
	//if ((((HRESULT)(CComObject<Timer>::CreateInstance(&obj))) < 0))
	//{
	//	MessageBoxA(0, "Failed to create COM object.", "Visual Pinball", 0x00000030L);
	//}
	//obj->AddRef();
	//return obj;
	return new Timer();
}

IEditable* Timer::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Timer::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Timer* obj = Timer::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Timer::Timer()
{
}

Timer::~Timer()
{
}

HRESULT Timer::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	m_d.m_v.x = x;
	m_d.m_v.y = y;

	// TODO: SetDefaults(fromMouseClick);

	return InitVBA(true, 0, NULL);
}

HRESULT Timer::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		GetPTable()->GetUniqueName(eItemTimer, wzUniqueName, 128);
		wcsncpy(m_wzName, wzUniqueName, MAXNAMEBUFFER);
	}
	InitScript();
	return ((HRESULT)0L);
}

HRESULT Timer::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	SetDefaults(false);

	m_ptable = pTable;

	BiffReader biffReader(pStream, this, pId, version);
	biffReader.Load();

	return S_OK;
}

HRESULT Timer::InitPostLoad()
{
	return S_OK;
}

void Timer::SetDefaults(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_d.m_tdr.m_TimerEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Timer", "TimerEnabled", true) : true;
	m_d.m_tdr.m_TimerInterval = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\Timer", "TimerInterval", 100) : 100;
}

bool Timer::LoadToken(const int id, BiffReader* const pBiffReader)
{
	switch (id)
	{
	case FID(PIID):
		pBiffReader->GetInt((int*)pBiffReader->m_pData);
		break;
	case FID(VCEN):
		pBiffReader->GetVector2(m_d.m_v);
		break;
	case FID(TMON):
		pBiffReader->GetBool(m_d.m_tdr.m_TimerEnabled);
		break;
	case FID(TMIN):
		pBiffReader->GetInt(m_d.m_tdr.m_TimerInterval);
		break;
	case FID(NAME):
		pBiffReader->GetWideString(m_wzName, sizeof(m_wzName) / sizeof(wchar_t));
		break;
	case FID(BGLS):
		pBiffReader->GetBool(m_backglass);
		break;
	default:
		ISelect::LoadToken(id, pBiffReader);
		break;
	}
	return true;
}

PinTable* Timer::GetPTable()
{
	return m_ptable;
}

const PinTable* Timer::GetPTable() const
{
	return m_ptable;
}

IEditable* Timer::GetIEditable()
{
	return static_cast<IEditable*>(this);
}

const IEditable* Timer::GetIEditable() const
{
	return static_cast<const IEditable*>(this);
}

ISelect* Timer::GetISelect()
{
	return static_cast<ISelect*>(this);
}

const ISelect* Timer::GetISelect() const
{
	return static_cast<const ISelect*>(this);
}

IHitable* Timer::GetIHitable()
{
	return static_cast<IHitable*>(this);
}

const IHitable* Timer::GetIHitable() const
{
	return static_cast<const IHitable*>(this);
}

ItemTypeEnum Timer::GetItemType() const
{
	return eItemTimer;
}

void Timer::WriteRegDefaults()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	pRegUtil->SaveValueBool("DefaultProps\\Timer", "TimerEnabled", m_d.m_tdr.m_TimerEnabled);
	pRegUtil->SaveValueInt("DefaultProps\\Timer", "TimerInterval", m_d.m_tdr.m_TimerInterval);
}

void Timer::GetHitShapes(std::vector<HitObject*>& pvho)
{
}

void Timer::GetHitShapesDebug(std::vector<HitObject*>& pvho)
{
}

void Timer::RenderStatic()
{
}

void Timer::RenderDynamic()
{
}

void Timer::RenderSetup()
{
}

ItemTypeEnum Timer::HitableGetItemType() const
{
	return eItemTimer;
}

IScriptable* Timer::GetScriptable()
{
	return (IScriptable*)this;
}

wchar_t* Timer::get_Name()
{
	return m_wzName;
}
