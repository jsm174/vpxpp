#include "Light.h"

const ItemTypeEnum Light::ItemType = eItemLight;
const int Light::TypeNameID = 130;
const int Light::ToolID = 131;
const int Light::CursorID = 110;
const unsigned Light::AllowedViews = 3;

Light* Light::COMCreate()
{
	/*CComObject<Light>* obj = 0;
	if ((((HRESULT)(CComObject<Light>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Light();
}

IEditable* Light::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Light::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Light* obj = Light::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Light::Light()
{
}

Light::~Light()
{
}

HRESULT Light::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	m_d.m_vCenter.x = x;
	m_d.m_vCenter.y = y;

	// TODO: SetDefaults(fromMouseClick);

	// TODO: InitShape();

	//m_lockedByLS = false;
	//m_inPlayState = m_d.m_state;
	m_d.m_visible = true;

	return InitVBA(true, 0, NULL);
}

HRESULT Light::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			GetPTable()->GetUniqueName(eItemLight, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t*)m_wzName,
			//             sizeof(m_wzName) / sizeof(m_wzName[0]));
		}
	}
	InitScript();
	return ((HRESULT)0L);
}

PinTable* Light::GetPTable()
{
	return m_ptable;
}

HRESULT Light::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	return S_OK;
}

bool Light::LoadToken(const int id, BiffReader* const pbr)
{
	return true;
}