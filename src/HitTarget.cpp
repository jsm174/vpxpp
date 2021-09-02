#include "HitTarget.h"

const ItemTypeEnum HitTarget::ItemType = eItemHitTarget;
const int HitTarget::TypeNameID = 134;
const int HitTarget::ToolID = 135;
const int HitTarget::CursorID = 147;
const unsigned HitTarget::AllowedViews = 1;

HitTarget* HitTarget::COMCreate()
{
	/*CComObject<HitTarget>* obj = 0;
	if ((((HRESULT)(CComObject<HitTarget>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new HitTarget();
}

IEditable* HitTarget::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* HitTarget::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	HitTarget* obj = HitTarget::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

HitTarget::HitTarget()
{
}

HRESULT HitTarget::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	m_d.m_vPosition.x = x;
	m_d.m_vPosition.y = y;

	// TODO: SetDefaults(false);
	// TODO: m_hitEvent = false;

	InitVBA(true, 0, NULL);

	//UpdateStatusBarInfo();

	return S_OK;
}

HRESULT HitTarget::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			GetPTable()->GetUniqueName(eItemHitTarget, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t*)m_wzName,
			//             sizeof(m_wzName) / sizeof(m_wzName[0]));
		}
	}
	InitScript();
	return ((HRESULT)0L);
}

PinTable* HitTarget::GetPTable()
{
	return m_ptable;
}

HRESULT HitTarget::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	return S_OK;
}

bool HitTarget::LoadToken(const int id, BiffReader* const pbr)
{
	return true;
}