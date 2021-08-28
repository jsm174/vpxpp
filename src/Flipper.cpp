#include "Flipper.h"

const ItemTypeEnum Flipper::ItemType = eItemFlipper;
const int Flipper::TypeNameID = 105;
const int Flipper::ToolID = 111;
const int Flipper::CursorID = 139;
const unsigned Flipper::AllowedViews = 1;

Flipper* Flipper::COMCreate()
{
	/*CComObject<Flipper>* obj = 0;
	if ((((HRESULT)(CComObject<Flipper>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Flipper();
}

IEditable* Flipper::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Flipper::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Flipper* obj = Flipper::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Flipper::Flipper()
{
}

Flipper::~Flipper()
{
}

HRESULT Flipper::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	m_d.m_Center.x = x;
	m_d.m_Center.y = y;

	//TODO: SetDefaults(fromMouseClick);

	//TODO: m_phitflipper = NULL;

	return InitVBA(true, 0, NULL);
}

HRESULT Flipper::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			//GetPTable()->GetUniqueName(eItemFlipper, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t *)m_wzName, sizeof(m_wzName)/sizeof(m_wzName[0]));/*lstrcpyW((WCHAR *)m_wzName, wzUniqueName);*/
		}
	}
	//InitScript();
	return S_OK;
}

PinTable* Flipper::GetPTable()
{
	return m_ptable;
}

HRESULT Flipper::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	return S_OK;
}

bool Flipper::LoadToken(const int id, BiffReader* const pbr)
{
	return true;
}