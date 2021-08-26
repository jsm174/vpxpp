#include "Bumper.h"

const ItemTypeEnum Bumper::ItemType = eItemBumper;
const int Bumper::TypeNameID = 122;
const int Bumper::ToolID = 123;
const int Bumper::CursorID = 145;
const unsigned Bumper::AllowedViews = 1;

Bumper* Bumper::COMCreate()
{
	/*CComObject<Bumper>* obj = 0;
	if ((((HRESULT)(CComObject<Bumper>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Bumper();
}

IEditable* Bumper::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Bumper* obj = Bumper::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Bumper::Bumper()
{
	// TODO: m_pbumperhitcircle = NULL;
	m_baseVertexBuffer = NULL;
	m_baseIndexBuffer = NULL;
	m_ringVertexBuffer = NULL;
	m_ringIndexBuffer = NULL;
	m_capVertexBuffer = NULL;
	m_capIndexBuffer = NULL;
	m_socketIndexBuffer = NULL;
	m_socketVertexBuffer = NULL;
	m_ringAnimate = false;
	// TODO: m_propVisual = NULL;
	m_d.m_ringDropOffset = 0.0f;
	m_ringDown = false;
	m_updateSkirt = false;
	m_doSkirtAnimation = false;
	m_enableSkirtAnimation = true;
	m_skirtCounter = 0.0f;
}

Bumper::~Bumper()
{
}

PinTable* Bumper::GetPTable()
{
	return m_ptable;
}

HRESULT Bumper::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	//TODO: SetDefaults(fromMouseClick);

	m_d.m_vCenter.x = x;
	m_d.m_vCenter.y = y;

	return InitVBA(true, 0, NULL);
}

HRESULT Bumper::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	return S_OK;
}

HRESULT Bumper::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			GetPTable()->GetUniqueName(eItemBumper, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t*)m_wzName,
			//             sizeof(m_wzName) / sizeof(m_wzName[0]));
		}
	}
	InitScript();
	return ((HRESULT)0L);
}

bool Bumper::LoadToken(const int id, BiffReader* const pbr)
{
}