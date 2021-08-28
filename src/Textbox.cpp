#include "Textbox.h"

const ItemTypeEnum Textbox::ItemType = eItemTextbox;
const int Textbox::TypeNameID = 119;
const int Textbox::ToolID = 120;
const int Textbox::CursorID = 144;
const unsigned Textbox::AllowedViews = 2;

Textbox* Textbox::COMCreate()
{
	/*CComObject<Textbox>* obj = 0;
	if ((((HRESULT)(CComObject<Textbox>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Textbox();
}

IEditable* Textbox::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Textbox::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Textbox* obj = Textbox::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Textbox::Textbox()
{
}

Textbox::~Textbox()
{
}

HRESULT Textbox::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	// TODO: const float width = LoadValueFloatWithDefault("DefaultProps\\TextBox", "Width", 100.0f);
	// const float height = LoadValueFloatWithDefault("DefaultProps\\TextBox", "Height", 50.0f);

	// m_d.m_v1.x = x;
	// m_d.m_v1.y = y;
	// m_d.m_v2.x = x + width;
	// m_d.m_v2.y = y + height;

	// SetDefaults(fromMouseClick);

	return InitVBA(true, 0, NULL);
}

HRESULT Textbox::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			GetPTable()->GetUniqueName(eItemTextbox, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t*)m_wzName,
			//             sizeof(m_wzName) / sizeof(m_wzName[0]));
		}
	}
	InitScript();
	return ((HRESULT)0L);
}

PinTable* Textbox::GetPTable()
{
	return m_ptable;
}

HRESULT Textbox::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	return S_OK;
}

bool Textbox::LoadToken(const int id, BiffReader* const pbr)
{
	return true;
}