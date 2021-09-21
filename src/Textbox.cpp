#include "Textbox.h"
#include "RegUtil.h"

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
	// TODO: m_pIFont = NULL;
	// TODO: m_texture = NULL;
}

Textbox::~Textbox()
{
}

HRESULT Textbox::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_ptable = ptable;

	const float width = pRegUtil->LoadValueFloatWithDefault("DefaultProps\\TextBox", "Width", 100.0f);
	const float height = pRegUtil->LoadValueFloatWithDefault("DefaultProps\\TextBox", "Height", 50.0f);

	m_d.m_v1.x = x;
	m_d.m_v1.y = y;
	m_d.m_v2.x = x + width;
	m_d.m_v2.y = y + height;

	SetDefaults(fromMouseClick);

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
	SetDefaults(false);

	m_ptable = pTable;

	BiffReader biffReader(pStream, this, pId, version);
	biffReader.Load();

	return S_OK;
}

HRESULT Textbox::InitPostLoad()
{
	// TODO: m_texture = NULL;

	return S_OK;
}

void Textbox::SetDefaults(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_backglass = true;
	m_d.m_visible = true;

	// TODO: FONTDESC fd;
	// TODO: fd.cbSizeofstruct = sizeof(FONTDESC);
	bool free_lpstrName = false;

	if (!fromMouseClick)
	{
		m_d.m_backcolor = RGB(0, 0, 0);
		m_d.m_fontcolor = RGB(255, 255, 255);
		m_d.m_intensity_scale = 1.0f;
		m_d.m_tdr.m_TimerEnabled = false;
		m_d.m_tdr.m_TimerInterval = 100;
		m_d.m_talign = TextAlignRight;
		m_d.m_transparent = false;
		m_d.m_isDMD = false;
		m_d.m_sztext.clear();

		// fd.cySize.int64 = (LONGLONG)(14.25f * 10000.0f);
		// fd.lpstrName = L"Arial";
		// fd.sWeight = FW_NORMAL;
		// fd.sCharset = 0;
		// fd.fItalic = 0;
		// fd.fUnderline = 0;
		// fd.fStrikethrough = 0;
	}
	else
	{
		m_d.m_backcolor = pRegUtil->LoadValueIntWithDefault("DefaultProps\\TextBox", "BackColor", RGB(0, 0, 0));
		m_d.m_fontcolor = pRegUtil->LoadValueIntWithDefault("DefaultProps\\TextBox", "FontColor", RGB(255, 255, 255));
		m_d.m_intensity_scale = pRegUtil->LoadValueFloatWithDefault("DefaultProps\\TextBox", "IntensityScale", 1.0f);
		m_d.m_tdr.m_TimerEnabled = pRegUtil->LoadValueBoolWithDefault("DefaultProps\\TextBox", "TimerEnabled", false) ? true : false;
		m_d.m_tdr.m_TimerInterval = pRegUtil->LoadValueIntWithDefault("DefaultProps\\TextBox", "TimerInterval", 100);
		m_d.m_talign = (TextAlignment)pRegUtil->LoadValueIntWithDefault("DefaultProps\\TextBox", "TextAlignment", TextAlignRight);
		m_d.m_transparent = pRegUtil->LoadValueBoolWithDefault("DefaultProps\\TextBox", "Transparent", false);
		m_d.m_isDMD = pRegUtil->LoadValueBoolWithDefault("DefaultProps\\TextBox", "DMD", false);

		const float fontSize = pRegUtil->LoadValueFloatWithDefault("DefaultProps\\TextBox", "FontSize", 14.25f);
		// TODO: fd.cySize.int64 = (LONGLONG)(fontSize * 10000.0f);

		std::string tmp;
		HRESULT hr;
		hr = pRegUtil->LoadValue("DefaultProps\\TextBox", "FontName", tmp);
		if (hr != S_OK)
		{
			// TODO: fd.lpstrName = L"Arial";
		}
		else
		{
			// TODO: const int len = (int)tmp.length() + 1;
			// fd.lpstrName = (LPOLESTR)malloc(len * sizeof(WCHAR));
			// memset(fd.lpstrName, 0, len * sizeof(WCHAR));
			// MultiByteToWideCharNull(CP_ACP, 0, tmp.c_str(), -1, fd.lpstrName, len);

			// free_lpstrName = true;
		}

		// fd.sWeight = pRegUtil->LoadValueIntWithDefault("DefaultProps\\TextBox", "FontWeight", FW_NORMAL);
		// fd.sCharset = pRegUtil->LoadValueIntWithDefault("DefaultProps\\TextBox", "FontCharSet", 0);
		// fd.fItalic = pRegUtil->LoadValueIntWithDefault("DefaultProps\\TextBox", "FontItalic", 0);
		// fd.fUnderline = pRegUtil->LoadValueIntWithDefault("DefaultProps\\TextBox", "FontUnderline", 0);
		// fd.fStrikethrough = pRegUtil->LoadValueIntWithDefault("DefaultProps\\TextBox", "FontStrikeThrough", 0);

		hr = pRegUtil->LoadValue("DefaultProps\\TextBox", "Text", m_d.m_sztext);
		if (hr != S_OK)
		{
			m_d.m_sztext.clear();
		}
	}

	// TODO: OleCreateFontIndirect(&fd, IID_IFont, (void**)&m_pIFont);
	//if (free_lpstrName)
	//{
	//	free(fd.lpstrName);
	//}
}

bool Textbox::LoadToken(const int id, BiffReader* const pBiffReader)
{
	switch (id)
	{
	case FID(PIID):
		pBiffReader->GetInt((int*)pBiffReader->m_pData);
		break;
	case FID(VER1):
		pBiffReader->GetVector2(m_d.m_v1);
		break;
	case FID(VER2):
		pBiffReader->GetVector2(m_d.m_v2);
		break;
	case FID(CLRB):
		pBiffReader->GetInt(m_d.m_backcolor);
		break;
	case FID(CLRF):
		pBiffReader->GetInt(m_d.m_fontcolor);
		break;
	case FID(INSC):
		pBiffReader->GetFloat(m_d.m_intensity_scale);
		break;
	case FID(TMON):
		pBiffReader->GetBool(m_d.m_tdr.m_TimerEnabled);
		break;
	case FID(TMIN):
		pBiffReader->GetInt(m_d.m_tdr.m_TimerInterval);
		break;
	case FID(TEXT):
		pBiffReader->GetString(m_d.m_sztext);
		break;
	case FID(NAME):
		pBiffReader->GetWideString(m_wzName, sizeof(m_wzName) / sizeof(wchar_t));
		break;
	case FID(ALGN):
		pBiffReader->GetInt(&m_d.m_talign);
		break;
	case FID(TRNS):
		pBiffReader->GetBool(m_d.m_transparent);
		break;
	case FID(IDMD):
		pBiffReader->GetBool(m_d.m_isDMD);
		break;
	case FID(FONT):
	{
		// TODO: if (!m_pIFont)
		// {
		// 	FONTDESC fd;
		// 	fd.cbSizeofstruct = sizeof(FONTDESC);
		// 	fd.lpstrName = L"Arial";
		// 	fd.cySize.int64 = 142500;
		// 	//fd.cySize.Lo = 0;
		// 	fd.sWeight = FW_NORMAL;
		// 	fd.sCharset = 0;
		// 	fd.fItalic = 0;
		// 	fd.fUnderline = 0;
		// 	fd.fStrikethrough = 0;
		// 	OleCreateFontIndirect(&fd, IID_IFont, (void**)&m_pIFont);
		// }
		// IPersistStream* ips;
		// m_pIFont->QueryInterface(IID_IPersistStream, (void**)&ips);

		// ips->Load(pBiffReader->m_pistream);

		break;
	}
	default:
		ISelect::LoadToken(id, pBiffReader);
		break;
	}
	return true;
}

ItemTypeEnum Textbox::GetItemType() const
{
	return eItemTextbox;
}

IEditable* Textbox::GetIEditable()
{
	return static_cast<IEditable*>(this);
}

IHitable* Textbox::GetIHitable()
{
	return static_cast<IHitable*>(this);
}

void Textbox::WriteRegDefaults()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	pRegUtil->SaveValueInt("DefaultProps\\TextBox", "BackColor", m_d.m_backcolor);
	pRegUtil->SaveValueInt("DefaultProps\\TextBox", "FontColor", m_d.m_fontcolor);
	pRegUtil->SaveValueBool("DefaultProps\\TextBox", "TimerEnabled", m_d.m_tdr.m_TimerEnabled);
	pRegUtil->SaveValueInt("DefaultProps\\TextBox", "TimerInterval", m_d.m_tdr.m_TimerInterval);
	pRegUtil->SaveValueBool("DefaultProps\\TextBox", "Transparent", m_d.m_transparent);
	pRegUtil->SaveValueBool("DefaultProps\\TextBox", "DMD", m_d.m_isDMD);

	// TODO: FONTDESC fd;
	// fd.cbSizeofstruct = sizeof(FONTDESC);
	// m_pIFont->get_Size(&fd.cySize);
	// m_pIFont->get_Name(&fd.lpstrName); //!! BSTR
	// m_pIFont->get_Weight(&fd.sWeight);
	// m_pIFont->get_Charset(&fd.sCharset);
	// m_pIFont->get_Italic(&fd.fItalic);
	// m_pIFont->get_Underline(&fd.fUnderline);
	// m_pIFont->get_Strikethrough(&fd.fStrikethrough);

	// const float fTmp = (float)(fd.cySize.int64 / 10000.0);
	// pRegUtil->SaveValueFloat("DefaultProps\\TextBox", "FontSize", fTmp);
	// size_t charCnt = wcslen(fd.lpstrName) + 1;
	// char* const strTmp = new char[2 * charCnt];
	// WideCharToMultiByteNull(CP_ACP, 0, fd.lpstrName, -1, strTmp, (int)(2 * charCnt), NULL, NULL);
	// pRegUtil->SaveValue("DefaultProps\\TextBox", "FontName", strTmp);
	// delete[] strTmp;
	// int weight = fd.sWeight;
	// int charset = fd.sCharset;
	// pRegUtil->SaveValueInt("DefaultProps\\TextBox", "FontWeight", weight);
	// pRegUtil->SaveValueInt("DefaultProps\\TextBox", "FontCharSet", charset);
	// pRegUtil->SaveValueInt("DefaultProps\\TextBox", "FontItalic", fd.fItalic);
	// pRegUtil->SaveValueInt("DefaultProps\\TextBox", "FontUnderline", fd.fUnderline);
	// pRegUtil->SaveValueInt("DefaultProps\\TextBox", "FontStrikeThrough", fd.fStrikethrough);

	pRegUtil->SaveValue("DefaultProps\\TextBox", "Text", m_d.m_sztext);
}

void Textbox::GetHitShapes(std::vector<HitObject*>& pvho)
{
}

void Textbox::GetHitShapesDebug(std::vector<HitObject*>& pvho)
{
}

void Textbox::RenderStatic()
{
}

void Textbox::RenderDynamic()
{
}

void Textbox::RenderSetup()
{
}

ItemTypeEnum Textbox::HitableGetItemType() const
{
	return eItemTextbox;
}
