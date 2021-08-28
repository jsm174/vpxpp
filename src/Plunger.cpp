#include "Plunger.h"

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
}
Plunger::~Plunger()
{
}

HRESULT Plunger::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	m_d.m_v.x = x;
	m_d.m_v.y = y;

	//TODO: SetDefaults(fromMouseClick);

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
	return S_OK;
}

bool Plunger::LoadToken(const int id, BiffReader* const pbr)
{
	return true;
}