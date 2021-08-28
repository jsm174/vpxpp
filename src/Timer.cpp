#include "Timer.h"

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

	//TODO: SetDefaults(fromMouseClick);

	return InitVBA(true, 0, NULL);
}

HRESULT Timer::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			GetPTable()->GetUniqueName(eItemTimer, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t*)m_wzName,
			//             sizeof(m_wzName) / sizeof(m_wzName[0]));
		}
	}
	InitScript();
	return ((HRESULT)0L);
}

PinTable* Timer::GetPTable()
{
	return m_ptable;
}

HRESULT Timer::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	return S_OK;
}

bool Timer::LoadToken(const int id, BiffReader* const pbr)
{
	return true;
}