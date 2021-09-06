#include "Decal.h"
#include "RegUtil.h"

const ItemTypeEnum Decal::ItemType = eItemDecal;
const int Decal::TypeNameID = 135;
const int Decal::ToolID = 136;
const int Decal::CursorID = 148;
const unsigned Decal::AllowedViews = 1 | 2;

Decal* Decal::COMCreate()
{
	/*CComObject<Decal>* obj = 0;
	if ((((HRESULT)(CComObject<Decal>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Decal();
}

IEditable* Decal::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Decal::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Decal* obj = Decal::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Decal::Decal()
{
}

Decal::~Decal()
{
}

HRESULT Decal::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	SetDefaults(fromMouseClick);

	m_d.m_vCenter.x = x;
	m_d.m_vCenter.y = y;

	InitVBA(true, 0, NULL);

	// TODO: EnsureSize();

	return S_OK;
}

HRESULT Decal::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			//GetPTable()->GetUniqueName(eItemDecal, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t *)m_wzName, sizeof(m_wzName)/sizeof(m_wzName[0]));/*lstrcpyW((WCHAR *)m_wzName, wzUniqueName);*/
		}
	}
	//InitScript();
	return S_OK;
}

PinTable* Decal::GetPTable()
{
	return m_ptable;
}

HRESULT Decal::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	SetDefaults(false);

	m_ptable = pTable;

	BiffReader biffReader(pStream, this, pId, version);
	biffReader.Load();

	return S_OK;
}

HRESULT Decal::InitPostLoad()
{
   // TODO: EnsureSize();

   return S_OK;
}

void Decal::SetDefaults(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_d.m_width = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Decal", "Width", 100.0f) : 100.0f;
	m_d.m_height = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Decal", "Height", 100.0f) : 100.0f;
	m_d.m_rotation = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Decal", "Rotation", 0.f) : 0.f;

	HRESULT hr = pRegUtil->LoadValue("DefaultProps\\Decal", "Image", m_d.m_szImage);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_d.m_szImage.clear();
	}

	hr = pRegUtil->LoadValue("DefaultProps\\Decal", "Surface", m_d.m_szSurface);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_d.m_szSurface.clear();
	}

	m_d.m_decaltype = fromMouseClick ? (DecalType)pRegUtil->LoadValueIntWithDefault("DefaultProps\\Decal", "DecalType", (int)DecalImage) : DecalImage;

	hr = pRegUtil->LoadValue("DefaultProps\\Decal", "Text", m_d.m_sztext);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_d.m_sztext.clear();
	}

	m_d.m_sizingtype = fromMouseClick ? (SizingType)pRegUtil->LoadValueIntWithDefault("DefaultProps\\Decal", "Sizing", (int)ManualSize) : ManualSize;
	m_d.m_color = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\Decal", "Color", RGB(0, 0, 0)) : RGB(0, 0, 0);
	m_d.m_verticalText = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Decal", "VerticalText", false) : false;

	// TODO: if (!m_pIFont)
	// {
	// 	FONTDESC fd;
	// 	fd.cbSizeofstruct = sizeof(FONTDESC);

	// 	float fTmp;
	// 	hr = LoadValue("DefaultProps\\Decal", "FontSize", fTmp);
	// 	fd.cySize.int64 = (hr == S_OK) && fromMouseClick ? (LONGLONG)(fTmp * 10000.0) : 142500;

	// 	char tmp[MAXSTRING];
	// 	hr = LoadValue("DefaultProps\\Decal", "FontName", tmp, MAXSTRING);
	// 	if ((hr != S_OK) || !fromMouseClick)
	// 		fd.lpstrName = L"Arial Black";
	// 	else
	// 	{
	// 		const int len = lstrlen(tmp) + 1;
	// 		fd.lpstrName = (LPOLESTR)malloc(len * sizeof(WCHAR));
	// 		memset(fd.lpstrName, 0, len * sizeof(WCHAR));
	// 		MultiByteToWideCharNull(CP_ACP, 0, tmp, -1, fd.lpstrName, len);
	// 	}

	// 	fd.sWeight = fromMouseClick ? LoadValueIntWithDefault("DefaultProps\\Decal", "FontWeight", FW_NORMAL) : FW_NORMAL;
	// 	fd.sCharset = fromMouseClick ? LoadValueIntWithDefault("DefaultProps\\Decal", "FontCharSet", 0) : 0;
	// 	fd.fItalic = fromMouseClick ? LoadValueBoolWithDefault("DefaultProps\\Decal", "FontItalic", false) : false;
	// 	fd.fUnderline = fromMouseClick ? LoadValueBoolWithDefault("DefaultProps\\Decal", "FontUnderline", false) : false;
	// 	fd.fStrikethrough = fromMouseClick ? LoadValueBoolWithDefault("DefaultProps\\Decal", "FontStrikeThrough", false) : false;

	// 	OleCreateFontIndirect(&fd, IID_IFont, (void**)&m_pIFont);
	// }
}

bool Decal::LoadToken(const int id, BiffReader* const pBiffReader)
{
	switch (id)
	{
	case FID(PIID):
		pBiffReader->GetInt((int*)pBiffReader->m_pData);
		break;
	case FID(VCEN):
		pBiffReader->GetVector2(m_d.m_vCenter);
		break;
	case FID(WDTH):
		pBiffReader->GetFloat(m_d.m_width);
		break;
	case FID(HIGH):
		pBiffReader->GetFloat(m_d.m_height);
		break;
	case FID(ROTA):
		pBiffReader->GetFloat(m_d.m_rotation);
		break;
	case FID(IMAG):
		pBiffReader->GetString(m_d.m_szImage);
		break;
	case FID(SURF):
		pBiffReader->GetString(m_d.m_szSurface);
		break;
	case FID(NAME):
		pBiffReader->GetWideString(m_wzName, sizeof(m_wzName) / sizeof(wchar_t));
		break;
	case FID(TEXT):
		pBiffReader->GetString(m_d.m_sztext);
		break;
	case FID(TYPE):
		pBiffReader->GetInt(&m_d.m_decaltype);
		break;
	case FID(COLR):
		pBiffReader->GetInt(m_d.m_color);
		break;
	case FID(MATR):
		pBiffReader->GetString(m_d.m_szMaterial);
		break;
	case FID(SIZE):
		pBiffReader->GetInt(&m_d.m_sizingtype);
		break;
	case FID(VERT):
		pBiffReader->GetBool(m_d.m_verticalText);
		break;
	case FID(BGLS):
		pBiffReader->GetBool(m_backglass);
		break;
	case FID(FONT):
	{
		// TODO: if (!m_pIFont)
		// {
		// 	FONTDESC fd;
		// 	fd.cbSizeofstruct = sizeof(FONTDESC);
		// 	fd.lpstrName = L"Arial";
		// 	fd.cySize.int64 = 142500;
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

void Decal::WriteRegDefaults()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	pRegUtil->SaveValueFloat("DefaultProps\\Decal", "Width", m_d.m_width);
	pRegUtil->SaveValueFloat("DefaultProps\\Decal", "Height", m_d.m_height);
	pRegUtil->SaveValueFloat("DefaultProps\\Decal", "Rotation", m_d.m_rotation);
	pRegUtil->SaveValue("DefaultProps\\Decal", "Image", m_d.m_szImage);
	pRegUtil->SaveValueInt("DefaultProps\\Decal", "DecalType", m_d.m_decaltype);
	pRegUtil->SaveValue("DefaultProps\\Decal", "Text", m_d.m_sztext);
	pRegUtil->SaveValueInt("DefaultProps\\Decal", "Sizing", m_d.m_sizingtype);
	pRegUtil->SaveValueInt("DefaultProps\\Decal", "Color", m_d.m_color);
	pRegUtil->SaveValueBool("DefaultProps\\Decal", "VerticalText", m_d.m_verticalText);
	pRegUtil->SaveValue("DefaultProps\\Decal", "Surface", m_d.m_szSurface);

	// TODO: if (m_pIFont)
	// {
	// 	FONTDESC fd;
	// 	fd.cbSizeofstruct = sizeof(FONTDESC);
	// 	m_pIFont->get_Size(&fd.cySize);
	// 	m_pIFont->get_Name(&fd.lpstrName); //!! BSTR
	// 	m_pIFont->get_Weight(&fd.sWeight);
	// 	m_pIFont->get_Charset(&fd.sCharset);
	// 	m_pIFont->get_Italic(&fd.fItalic);
	// 	m_pIFont->get_Underline(&fd.fUnderline);
	// 	m_pIFont->get_Strikethrough(&fd.fStrikethrough);

	// 	const float fTmp = (float)(fd.cySize.int64 / 10000.0);
	// 	SaveValueFloat("DefaultProps\\Decal", "FontSize", fTmp);

	// 	const size_t charCnt = wcslen(fd.lpstrName) + 1;
	// 	char* const strTmp = new char[2 * charCnt];
	// 	WideCharToMultiByteNull(CP_ACP, 0, fd.lpstrName, -1, strTmp, (int)(2 * charCnt), NULL, NULL);
	// 	SaveValue("DefaultProps\\Decal", "FontName", strTmp);
	// 	delete[] strTmp;
	// 	const int weight = fd.sWeight;
	// 	const int charset = fd.sCharset;
	// 	SaveValueInt("DefaultProps\\Decal", "FontWeight", weight);
	// 	SaveValueInt("DefaultProps\\Decal", "FontCharSet", charset);
	// 	SaveValueInt("DefaultProps\\Decal", "FontItalic", fd.fItalic);
	// 	SaveValueInt("DefaultProps\\Decal", "FontUnderline", fd.fUnderline);
	// 	SaveValueInt("DefaultProps\\Decal", "FontStrikeThrough", fd.fStrikethrough);
	// }
}