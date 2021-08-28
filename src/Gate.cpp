#include "Gate.h"

const ItemTypeEnum Gate::ItemType = eItemGate;
const int Gate::TypeNameID = 139;
const int Gate::ToolID = 140;
const int Gate::CursorID = 141;
const unsigned Gate::AllowedViews = 1;

Gate* Gate::COMCreate()
{
	/*CComObject<Gate>* obj = 0;
	if ((((HRESULT)(CComObject<Gate>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Gate();
}

IEditable* Gate::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Gate::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Gate* obj = Gate::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Gate::Gate()
{
}

Gate::~Gate()
{
}

HRESULT Gate::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	m_d.m_vCenter.x = x;
	m_d.m_vCenter.y = y;

	//TODO: SetDefaults(fromMouseClick);

	InitVBA(true, 0, NULL);

	return S_OK;
}

HRESULT Gate::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			//GetPTable()->GetUniqueName(eItemGate, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t *)m_wzName, sizeof(m_wzName)/sizeof(m_wzName[0]));/*lstrcpyW((WCHAR *)m_wzName, wzUniqueName);*/
		}
	}
	//InitScript();
	return S_OK;
}

PinTable* Gate::GetPTable()
{
	return m_ptable;
}

HRESULT Gate::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	return S_OK;
}

bool Gate::LoadToken(const int id, BiffReader* const pbr)
{
	return true;
}