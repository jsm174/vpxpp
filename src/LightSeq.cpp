#include "LightSeq.h"

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
}

LightSeq::~LightSeq()
{
}

HRESULT LightSeq::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	m_d.m_vCenter.x = x;
	m_d.m_vCenter.y = y;

	//TODO: SetDefaults(fromMouseClick);

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
	return S_OK;
}

bool LightSeq::LoadToken(const int id, BiffReader* const pbr)
{
	return true;
}