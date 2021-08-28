#include "Primitive.h"

const ItemTypeEnum Primitive::ItemType = eItemPrimitive;
const int Primitive::TypeNameID = 186;
const int Primitive::ToolID = 32842;
const int Primitive::CursorID = 385;
const unsigned Primitive::AllowedViews = 1;

Primitive* Primitive::COMCreate()
{
	/*CComObject<Primitive>* obj = 0;
	if ((((HRESULT)(CComObject<Primitive>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Primitive();
}

IEditable* Primitive::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Primitive::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Primitive* obj = Primitive::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Primitive::Primitive()
{
}

Primitive::~Primitive()
{
}

HRESULT Primitive::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	m_d.m_vPosition.x = x;
	m_d.m_vPosition.y = y;

	//TODO: SetDefaults(fromMouseClick);

	InitVBA(true, 0, NULL);

	//TODO:UpdateStatusBarInfo();

	return S_OK;
}

HRESULT Primitive::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			GetPTable()->GetUniqueName(eItemPrimitive, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t*)m_wzName,
			//             sizeof(m_wzName) / sizeof(m_wzName[0]));
		}
	}
	InitScript();
	return ((HRESULT)0L);
}

PinTable* Primitive::GetPTable()
{
	return m_ptable;
}

HRESULT Primitive::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	return S_OK;
}

bool Primitive::LoadToken(const int id, BiffReader* const pbr)
{
	return true;
}