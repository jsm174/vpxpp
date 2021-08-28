#include "Decal.h"

const ItemTypeEnum Decal::ItemType = eItemDecal;
const int Decal::TypeNameID = 135;
const int Decal::ToolID = 136;
const int Decal::CursorID = 148;
const unsigned Decal::AllowedViews = 1 | 2;

Decal* Decal::COMCreate()
{
	/*CComObject<Decal>* obj = 0;
	if ((((HRESULT)(CComObject<Decal>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Decal();
}

IEditable* Decal::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Decal::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Decal* obj = Decal::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Decal::Decal()
{
}

Decal::~Decal()
{
}

HRESULT Decal::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	//TODO: SetDefaults(fromMouseClick);

	m_d.m_vCenter.x = x;
	m_d.m_vCenter.y = y;

	InitVBA(true, 0, NULL);

	//TODO: EnsureSize();

	return S_OK;
}

HRESULT Decal::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			//GetPTable()->GetUniqueName(eItemDecal, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t *)m_wzName, sizeof(m_wzName)/sizeof(m_wzName[0]));/*lstrcpyW((WCHAR *)m_wzName, wzUniqueName);*/
		}
	}
	//InitScript();
	return S_OK;
}

PinTable* Decal::GetPTable()
{
	return m_ptable;
}

HRESULT Decal::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	return S_OK;
}

bool Decal::LoadToken(const int id, BiffReader* const pbr)
{
	return true;
}