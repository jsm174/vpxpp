#include "Flasher.h"

const ItemTypeEnum Flasher::ItemType = eItemFlasher;
const int Flasher::TypeNameID = 140;
const int Flasher::ToolID = 32890;
const int Flasher::CursorID = 399;
const unsigned Flasher::AllowedViews = 1;

Flasher* Flasher::COMCreate()
{
	/*CComObject<Flasher>* obj = 0;
	if ((((HRESULT)(CComObject<Flasher>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Flasher();
}

IEditable* Flasher::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Flasher::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Flasher* obj = Flasher::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Flasher::Flasher()
{
}

Flasher::~Flasher()
{
}

HRESULT Flasher::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	m_d.m_isVisible = true;

	m_d.m_vCenter.x = x;
	m_d.m_vCenter.y = y;

	m_d.m_rotX = 0.0f;
	m_d.m_rotY = 0.0f;
	m_d.m_rotZ = 0.0f;
	//TODO: SetDefaults(fromMouseClick);

	//TODO: InitShape();

	InitVBA(true, 0, NULL);

	return S_OK;
}

HRESULT Flasher::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			//GetPTable()->GetUniqueName(eItemFlasher, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t *)m_wzName, sizeof(m_wzName)/sizeof(m_wzName[0]));/*lstrcpyW((WCHAR *)m_wzName, wzUniqueName);*/
		}
	}
	//InitScript();
	return S_OK;
}

PinTable* Flasher::GetPTable()
{
	return m_ptable;
}

HRESULT Flasher::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	return S_OK;
}

bool Flasher::LoadToken(const int id, BiffReader* const pbr)
{
	return true;
}
