#include "Kicker.h"

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
}

Kicker::~Kicker()
{
}

HRESULT Kicker::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	m_d.m_vCenter.x = x;
	m_d.m_vCenter.y = y;

	//TODO: SetDefaults(fromMouseClick);

	return InitVBA(true, 0, NULL);
}

HRESULT Kicker::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			GetPTable()->GetUniqueName(eItemKicker, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t*)m_wzName,
			//             sizeof(m_wzName) / sizeof(m_wzName[0]));
		}
	}
	InitScript();
	return ((HRESULT)0L);
}

PinTable* Kicker::GetPTable()
{
	return m_ptable;
}

HRESULT Kicker::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	return S_OK;
}

bool Kicker::LoadToken(const int id, BiffReader* const pbr)
{
	return true;
}