#include "Surface.h"

const ItemTypeEnum Surface::ItemType = eItemSurface;
const int Surface::TypeNameID = 104;
const int Surface::ToolID = 110;
const int Surface::CursorID = 138;
const unsigned Surface::AllowedViews = 1;

Surface* Surface::COMCreate()
{
	/*CComObject<Surface>* obj = 0;
	if ((((HRESULT)(CComObject<Surface>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Surface();
}

IEditable* Surface::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Surface::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Surface* obj = Surface::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Surface::Surface()
{
}

Surface::~Surface()
{
}

HRESULT Surface::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	// TODO: m_isWall = true;

	// const float width = fromMouseClick ? LoadValueFloatWithDefault("DefaultProps\\Wall", "Width", 50.f) : 50.f;
	// const float length = fromMouseClick ? LoadValueFloatWithDefault("DefaultProps\\Wall", "Length", 50.f) : 50.f;

	// CComObject<DragPoint>* pdp;
	// CComObject<DragPoint>::CreateInstance(&pdp);
	// if (pdp)
	// {
	// 	pdp->AddRef();
	// 	pdp->Init(this, x - width, y - length, 0.f, false);
	// 	m_vdpoint.push_back(pdp);
	// }
	// CComObject<DragPoint>::CreateInstance(&pdp);
	// if (pdp)
	// {
	// 	pdp->AddRef();
	// 	pdp->Init(this, x - width, y + length, 0.f, false);
	// 	m_vdpoint.push_back(pdp);
	// }
	// CComObject<DragPoint>::CreateInstance(&pdp);
	// if (pdp)
	// {
	// 	pdp->AddRef();
	// 	pdp->Init(this, x + width, y + length, 0.f, false);
	// 	m_vdpoint.push_back(pdp);
	// }
	// CComObject<DragPoint>::CreateInstance(&pdp);
	// if (pdp)
	// {
	// 	pdp->AddRef();
	// 	pdp->Init(this, x + width, y - length, 0.f, false);
	// 	m_vdpoint.push_back(pdp);
	// }

	// SetDefaults(fromMouseClick);

	return InitVBA(true, 0, NULL);
}

HRESULT Surface::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			GetPTable()->GetUniqueName(eItemSurface, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t*)m_wzName,
			//             sizeof(m_wzName) / sizeof(m_wzName[0]));
		}
	}
	InitScript();
	return ((HRESULT)0L);
}

PinTable* Surface::GetPTable()
{
	return m_ptable;
}

HRESULT Surface::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	return S_OK;
}

bool Surface::LoadToken(const int id, BiffReader* const pbr)
{
	return true;
}