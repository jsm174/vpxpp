#include "Trigger.h"

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
}

Trigger::~Trigger()
{
}

HRESULT Trigger::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	m_d.m_vCenter.x = x;
	m_d.m_vCenter.y = y;

	// TODO: SetDefaults(fromMouseClick);
	// TODO: if (m_vdpoint.empty())
	// TODO: InitShape(x, y);

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
	return S_OK;
}

bool Trigger::LoadToken(const int id, BiffReader* const pbr)
{
	return true;
}