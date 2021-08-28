#include "Ramp.h"

const ItemTypeEnum Ramp::ItemType = eItemRamp;
const int Ramp::TypeNameID = 145;
const int Ramp::ToolID = 146;
const int Ramp::CursorID = 150;
const unsigned Ramp::AllowedViews = 1;

Ramp* Ramp::COMCreate()
{
	/*CComObject<Ramp>* obj = 0;
	if ((((HRESULT)(CComObject<Ramp>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Ramp();
}

IEditable* Ramp::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Ramp::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Ramp* obj = Ramp::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Ramp::Ramp()
{
	//TODO: m_menuid = IDR_SURFACEMENU;
	m_d.m_collidable = true;
	m_d.m_visible = true;
	m_dynamicVertexBuffer = NULL;
	m_dynamicIndexBuffer = NULL;
	m_dynamicVertexBuffer2 = NULL;
	m_dynamicVertexBufferRegenerate = true;
	m_d.m_depthBias = 0.0f;
	m_d.m_wireDiameter = 6.0f;
	m_d.m_wireDistanceX = 38.0f;
	m_d.m_wireDistanceY = 88.0f;
	// TODO: m_propPosition = NULL;
	// TODO: m_propPhysics = NULL;
	m_d.m_hitEvent = false;
	m_d.m_overwritePhysics = true;
	m_rgheightInit = NULL;
}

Ramp::~Ramp()
{
}

HRESULT Ramp::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	//TODO:   SetDefaults(fromMouseClick);
	// m_d.m_visible = true;

	// float length = 0.5f * LoadValueFloatWithDefault("DefaultProps\\Ramp", "Length", 400.0f);

	// CComObject<DragPoint> *pdp;
	// CComObject<DragPoint>::CreateInstance(&pdp);
	// if (pdp)
	// {
	//    pdp->AddRef();
	//    pdp->Init(this, x, y + length, 0.f, true);
	//    pdp->m_calcHeight = m_d.m_heightbottom;
	//    m_vdpoint.push_back(pdp);
	// }

	// CComObject<DragPoint>::CreateInstance(&pdp);
	// if (pdp)
	// {
	//    pdp->AddRef();
	//    pdp->Init(this, x, y - length, 0.f, true);
	//    pdp->m_calcHeight = m_d.m_heighttop;
	//    m_vdpoint.push_back(pdp);
	// }

	InitVBA(true, 0, NULL);

	return S_OK;
}

HRESULT Ramp::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			GetPTable()->GetUniqueName(eItemRamp, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t*)m_wzName,
			//             sizeof(m_wzName) / sizeof(m_wzName[0]));
		}
	}
	InitScript();
	return ((HRESULT)0L);
}

PinTable* Ramp::GetPTable()
{
	return m_ptable;
}

HRESULT Ramp::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	return S_OK;
}

bool Ramp::LoadToken(const int id, BiffReader* const pbr)
{
	return true;
}