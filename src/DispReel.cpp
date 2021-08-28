#include "DispReel.h"

const ItemTypeEnum DispReel::ItemType = eItemDispReel;
const int DispReel::TypeNameID = 123;
const int DispReel::ToolID = 168;
const int DispReel::CursorID = 171;
const unsigned DispReel::AllowedViews = 2;

DispReel* DispReel::COMCreate()
{
	/*CComObject<DispReel>* obj = 0;
	if ((((HRESULT)(CComObject<DispReel>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new DispReel();
}

IEditable* DispReel::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* DispReel::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	DispReel* obj = DispReel::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

DispReel::DispReel()
{
}

DispReel::~DispReel()
{
}

HRESULT DispReel::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	//TODO: SetDefaults(fromMouseClick);

	m_d.m_v1.x = x;
	m_d.m_v1.y = y;
	m_d.m_v2.x = x + getBoxWidth();
	m_d.m_v2.y = y + getBoxHeight();

	return InitVBA(true, 0, NULL);
}

HRESULT DispReel::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			//GetPTable()->GetUniqueName(eItemDispReel, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t *)m_wzName, sizeof(m_wzName)/sizeof(m_wzName[0]));/*lstrcpyW((WCHAR *)m_wzName, wzUniqueName);*/
		}
	}
	//InitScript();
	return S_OK;
}

PinTable* DispReel::GetPTable()
{
	return m_ptable;
}

HRESULT DispReel::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	return S_OK;
}

bool DispReel::LoadToken(const int id, BiffReader* const pbr)
{
	return true;
}

float DispReel::getBoxWidth() const
{
	const float width = (float)m_d.m_reelcount * m_d.m_width + (float)m_d.m_reelcount * m_d.m_reelspacing + m_d.m_reelspacing; // spacing also includes edges
	return width;
}

float DispReel::getBoxHeight() const
{
	const float height = m_d.m_height + m_d.m_reelspacing + m_d.m_reelspacing; // spacing also includes edges

	return height;
}