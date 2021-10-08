#include "DispReel.h"
#include "RegUtil.h"

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
	m_dispreelanim.m_pDispReel = this;
}

DispReel::~DispReel()
{
}

HRESULT DispReel::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	SetDefaults(fromMouseClick);

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
		GetPTable()->GetUniqueName(eItemDispReel, wzUniqueName, 128);
		wcsncpy(m_wzName, wzUniqueName, MAXNAMEBUFFER);
	}
	InitScript();
	return S_OK;
}

HRESULT DispReel::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	SetDefaults(false);

	m_ptable = pTable;

	BiffReader biffReader(pStream, this, pTable, version);
	biffReader.Load();

	return S_OK;
}

HRESULT DispReel::InitPostLoad()
{
	return S_OK;
}

void DispReel::SetDefaults(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	// object is only available on the backglass
	m_backglass = true;

	// set all the Data defaults
	HRESULT hr;
	hr = pRegUtil->LoadValue("DefaultProps\\Ramp", "Image", m_d.m_szImage);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_d.m_szImage.clear();
	}

	hr = pRegUtil->LoadValue("DefaultProps\\Ramp", "Sound", m_d.m_szSound);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_d.m_szSound.clear();
	}

	m_d.m_useImageGrid = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\EMReel", "UseImageGrid", false) : false;
	m_d.m_visible = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\EMReel", "Visible", true) : true;
	m_d.m_imagesPerGridRow = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\EMReel", "ImagesPerRow", 1) : 1;
	m_d.m_transparent = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\EMReel", "Transparent", false) : false;
	m_d.m_reelcount = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\EMReel", "ReelCount", 5) : 5;
	m_d.m_width = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\EMReel", "Width", 30.0f) : 30.0f;
	m_d.m_height = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\EMReel", "Height", 40.0f) : 40.0f;
	m_d.m_reelspacing = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\EMReel", "ReelSpacing", 4.0f) : 4.0f;
	m_d.m_motorsteps = fromMouseClick ? (int)pRegUtil->LoadValueFloatWithDefault("DefaultProps\\EMReel", "MotorSteps", 2.f) : 2;
	m_d.m_digitrange = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\EMReel", "DigitRange", 9) : 9;
	m_d.m_updateinterval = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\EMReel", "UpdateInterval", 50) : 50;
	m_d.m_backcolor = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\EMReel", "BackColor", RGB(64, 64, 64)) : RGB(64, 64, 64);
	m_d.m_tdr.m_TimerEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\EMReel", "TimerEnabled", false) : false;
	m_d.m_tdr.m_TimerInterval = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\EMReel", "TimerInterval", 100) : 100;
}

bool DispReel::LoadToken(const int id, BiffReader* const pBiffReader)
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
	case FID(WDTH):
		pBiffReader->GetFloat(m_d.m_width);
		break;
	case FID(HIGH):
		pBiffReader->GetFloat(m_d.m_height);
		break;
	case FID(CLRB):
		pBiffReader->GetInt(m_d.m_backcolor);
		break;
	case FID(TMON):
		pBiffReader->GetBool(m_d.m_tdr.m_TimerEnabled);
		break;
	case FID(TMIN):
		pBiffReader->GetInt(m_d.m_tdr.m_TimerInterval);
		break;
	case FID(NAME):
		pBiffReader->GetWideString(m_wzName, sizeof(m_wzName) / sizeof(wchar_t));
		break;
	case FID(TRNS):
		pBiffReader->GetBool(m_d.m_transparent);
		break;
	case FID(IMAG):
		pBiffReader->GetString(m_d.m_szImage);
		break;
	case FID(RCNT):
	{
		float reel;
		pBiffReader->GetFloat(reel);
		m_d.m_reelcount = (int)reel;
		break;
	}
	case FID(RSPC):
		pBiffReader->GetFloat(m_d.m_reelspacing);
		break;
	case FID(MSTP):
	{
		float motorsteps;
		pBiffReader->GetFloat(motorsteps);
		m_d.m_motorsteps = (int)motorsteps;
		break;
	}
	case FID(SOUN):
		pBiffReader->GetString(m_d.m_szSound);
		break;
	case FID(UGRD):
		pBiffReader->GetBool(m_d.m_useImageGrid);
		break;
	case FID(VISI):
		pBiffReader->GetBool(m_d.m_visible);
		break;
	case FID(GIPR):
		pBiffReader->GetInt(m_d.m_imagesPerGridRow);
		break;
	case FID(RANG):
	{
		float dig;
		pBiffReader->GetFloat(dig);
		m_d.m_digitrange = (int)dig;
		break;
	}
	case FID(UPTM):
		pBiffReader->GetInt(m_d.m_updateinterval);
		break;
	case FID(FONT):
	{
		// TODO: IFont* pIFont;
		// FONTDESC fd;
		// fd.cbSizeofstruct = sizeof(FONTDESC);
		// fd.lpstrName = L"Times New Roman";
		// fd.cySize.int64 = 260000;
		// //fd.cySize.Lo = 0;
		// fd.sWeight = FW_BOLD;
		// fd.sCharset = 0;
		// fd.fItalic = 0;
		// fd.fUnderline = 0;
		// fd.fStrikethrough = 0;
		// OleCreateFontIndirect(&fd, IID_IFont, (void**)&pIFont);

		// IPersistStream* ips;
		// pIFont->QueryInterface(IID_IPersistStream, (void**)&ips);

		// ips->Load(pBiffReader->m_pistream);

		// pIFont->Release();

		break;
	}
	default:
		ISelect::LoadToken(id, pBiffReader);
		break;
	}
	return true;
}

PinTable* DispReel::GetPTable()
{
	return m_ptable;
}

const PinTable* DispReel::GetPTable() const
{
	return m_ptable;
}

IEditable* DispReel::GetIEditable()
{
	return static_cast<IEditable*>(this);
}

const IEditable* DispReel::GetIEditable() const
{
	return static_cast<const IEditable*>(this);
}

ISelect* DispReel::GetISelect()
{
	return static_cast<ISelect*>(this);
}

const ISelect* DispReel::GetISelect() const
{
	return static_cast<const ISelect*>(this);
}

IHitable* DispReel::GetIHitable()
{
	return static_cast<IHitable*>(this);
}

const IHitable* DispReel::GetIHitable() const
{
	return static_cast<const IHitable*>(this);
}

ItemTypeEnum DispReel::GetItemType() const
{
	return eItemDispReel;
}

void DispReel::WriteRegDefaults()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	pRegUtil->SaveValue("DefaultProps\\EMReel", "Image", m_d.m_szImage);
	pRegUtil->SaveValue("DefaultProps\\EMReel", "Sound", m_d.m_szSound);
	pRegUtil->SaveValueBool("DefaultProps\\Decal", "UseImageGrid", m_d.m_useImageGrid);
	pRegUtil->SaveValueBool("DefaultProps\\Decal", "Visible", m_d.m_visible);
	pRegUtil->SaveValueInt("DefaultProps\\Decal", "ImagesPerRow", m_d.m_imagesPerGridRow);
	pRegUtil->SaveValueBool("DefaultProps\\Decal", "Transparent", m_d.m_transparent);
	pRegUtil->SaveValueInt("DefaultProps\\Decal", "ReelCount", m_d.m_reelcount);
	pRegUtil->SaveValueFloat("DefaultProps\\EMReel", "Width", m_d.m_width);
	pRegUtil->SaveValueFloat("DefaultProps\\EMReel", "Height", m_d.m_height);
	pRegUtil->SaveValueFloat("DefaultProps\\EMReel", "ReelSpacing", m_d.m_reelspacing);
	pRegUtil->SaveValueFloat("DefaultProps\\EMReel", "MotorSteps", (float)m_d.m_motorsteps);
	pRegUtil->SaveValueInt("DefaultProps\\Decal", "DigitRange", m_d.m_digitrange);
	pRegUtil->SaveValueInt("DefaultProps\\Decal", "UpdateInterval", m_d.m_updateinterval);
	pRegUtil->SaveValueInt("DefaultProps\\EMReel", "BackColor", m_d.m_backcolor);
	pRegUtil->SaveValueBool("DefaultProps\\EMReel", "TimerEnabled", m_d.m_tdr.m_TimerEnabled);
	pRegUtil->SaveValueInt("DefaultProps\\EMReel", "TimerInterval", m_d.m_tdr.m_TimerInterval);
}

void DispReel::GetHitShapes(std::vector<HitObject*>& pvho)
{
}

void DispReel::GetHitShapesDebug(std::vector<HitObject*>& pvho)
{
}

void DispReel::RenderStatic()
{
}

void DispReel::RenderDynamic()
{
}

void DispReel::RenderSetup()
{
}

ItemTypeEnum DispReel::HitableGetItemType() const
{
	return eItemDispReel;
}

IScriptable* DispReel::GetScriptable()
{
	return (IScriptable*)this;
}

wchar_t* DispReel::get_Name()
{
	return m_wzName;
}

float DispReel::getBoxWidth() const
{
	const float width = (float)m_d.m_reelcount * m_d.m_width + (float)m_d.m_reelcount * m_d.m_reelspacing + m_d.m_reelspacing;
	return width;
}

float DispReel::getBoxHeight() const
{
	const float height = m_d.m_height + m_d.m_reelspacing + m_d.m_reelspacing;

	return height;
}
