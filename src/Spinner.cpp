#include "Spinner.h"

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
}

Spinner::~Spinner()
{
}

HRESULT Spinner::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	m_d.m_vCenter.x = x;
	m_d.m_vCenter.y = y;

	//TODO: SetDefaults(fromMouseClick);

	InitVBA(true, 0, NULL);

	return S_OK;
}

HRESULT Spinner::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			GetPTable()->GetUniqueName(eItemSpinner, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t*)m_wzName,
			//             sizeof(m_wzName) / sizeof(m_wzName[0]));
		}
	}
	InitScript();
	return ((HRESULT)0L);
}

PinTable* Spinner::GetPTable()
{
	return m_ptable;
}

HRESULT Spinner::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	return S_OK;
}

bool Spinner::LoadToken(const int id, BiffReader* const pbr)
{
	return true;
}