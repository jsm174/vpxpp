#include "Rubber.h"

const ItemTypeEnum Rubber::ItemType = eItemRubber;
const int Rubber::TypeNameID = 147;
const int Rubber::ToolID = 32990;
const int Rubber::CursorID = 602;
const unsigned Rubber::AllowedViews = 1;

Rubber* Rubber::COMCreate()
{
	/*CComObject<Rubber>* obj = 0;
	if ((((HRESULT)(CComObject<Rubber>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Rubber();
}

IEditable* Rubber::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Rubber::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Rubber* obj = Rubber::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Rubber::Rubber()
{
}

Rubber::~Rubber()
{
}

HRESULT Rubber::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;
	// m_d.m_visible = true;

	// for (int i = 8; i > 0; i--)
	// {
	// 	const float angle = (float)(M_PI * 2.0 / 8.0) * (float)i;
	// 	const float xx = x + sinf(angle) * 50.0f;
	// 	const float yy = y - cosf(angle) * 50.0f;
	// 	CComObject<DragPoint>* pdp;
	// 	CComObject<DragPoint>::CreateInstance(&pdp);
	// 	if (pdp)
	// 	{
	// 		pdp->AddRef();
	// 		pdp->Init(this, xx, yy, 0.f, true);
	// 		m_vdpoint.push_back(pdp);
	// 	}
	// }

	// SetDefaults(fromMouseClick);

	InitVBA(true, 0, NULL);

	return S_OK;
}

HRESULT Rubber::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			GetPTable()->GetUniqueName(eItemRubber, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t*)m_wzName,
			//             sizeof(m_wzName) / sizeof(m_wzName[0]));
		}
	}
	InitScript();
	return ((HRESULT)0L);
}

PinTable* Rubber::GetPTable()
{
	return m_ptable;
}

HRESULT Rubber::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	return S_OK;
}

bool Rubber::LoadToken(const int id, BiffReader* const pbr)
{
	return true;
}