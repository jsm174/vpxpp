#include "Flasher.h"
#include "RegUtil.h"

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
	// TODO: m_menuid = IDR_SURFACEMENU;
	m_d.m_isVisible = true;
	m_d.m_depthBias = 0.0f;
	m_dynamicVertexBuffer = NULL;
	m_dynamicIndexBuffer = NULL;
	m_dynamicVertexBufferRegenerate = true;
	m_vertices = NULL;
	// TODO: m_propVisual = NULL;
	m_ptable = NULL;
	m_numVertices = 0;
	m_numPolys = 0;
	m_minx = FLT_MAX;
	m_maxx = -FLT_MAX;
	m_miny = FLT_MAX;
	m_maxy = -FLT_MAX;
	m_lockedByLS = false;
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

	SetDefaults(fromMouseClick);

	// TODO: InitShape();

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
	SetDefaults(false);

	m_ptable = pTable;

	BiffReader biffReader(pStream, this, pId, version);
	biffReader.Load();

	m_inPlayState = m_d.m_isVisible;

	return S_OK;
}

void Flasher::SetDefaults(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_d.m_height = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Flasher", "Height", 50.f) : 50.f;
	m_d.m_rotX = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Flasher", "RotX", 0.f) : 0.f;
	m_d.m_rotY = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Flasher", "RotY", 0.f) : 0.f;
	m_d.m_rotZ = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Flasher", "RotZ", 0.f) : 0.f;
	m_d.m_color = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\Flasher", "Color", RGB(50, 200, 50)) : RGB(50, 200, 50);
	m_d.m_tdr.m_TimerEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Flasher", "TimerEnabled", false) : false;
	m_d.m_tdr.m_TimerInterval = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\Flasher", "TimerInterval", 100) : 100;

	HRESULT hr = pRegUtil->LoadValue("DefaultProps\\Flasher", "ImageA", m_d.m_szImageA);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_d.m_szImageA.clear();
	}

	hr = pRegUtil->LoadValue("DefaultProps\\Flasher", "ImageB", m_d.m_szImageB);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_d.m_szImageB.clear();
	}

	m_d.m_alpha = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\Flasher", "Opacity", 100) : 100;

	m_d.m_intensity_scale = 1.0f;

	m_d.m_modulate_vs_add = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Flasher", "ModulateVsAdd", 0.9f) : 0.9f;
	m_d.m_filterAmount = fromMouseClick ? pRegUtil->LoadValueIntWithDefault("DefaultProps\\Flasher", "FilterAmount", 100) : 100;
	m_d.m_isVisible = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Flasher", "Visible", true) : true;
	m_inPlayState = m_d.m_isVisible;
	m_d.m_addBlend = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Flasher", "AddBlend", false) : false;
	m_d.m_isDMD = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Flasher", "DMD", false) : false;
	m_d.m_displayTexture = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault("DefaultProps\\Flasher", "DisplayTexture", false) : false;
	m_d.m_imagealignment = fromMouseClick ? (RampImageAlignment)pRegUtil->LoadValueIntWithDefault("DefaultProps\\Flasher", "ImageMode", ImageModeWrap) : ImageModeWrap;
	m_d.m_filter = fromMouseClick ? (Filters)pRegUtil->LoadValueIntWithDefault("DefaultProps\\Flasher", "Filter", Filter_Overlay) : Filter_Overlay;
}

bool Flasher::LoadToken(const int id, BiffReader* const pBiffReader)
{
	switch (id)
	{
	case FID(PIID):
		pBiffReader->GetInt((int*)pBiffReader->m_pData);
		break;
	case FID(FHEI):
		pBiffReader->GetFloat(m_d.m_height);
		break;
	case FID(FLAX):
		pBiffReader->GetFloat(m_d.m_vCenter.x);
		break;
	case FID(FLAY):
		pBiffReader->GetFloat(m_d.m_vCenter.y);
		break;
	case FID(FROX):
		pBiffReader->GetFloat(m_d.m_rotX);
		break;
	case FID(FROY):
		pBiffReader->GetFloat(m_d.m_rotY);
		break;
	case FID(FROZ):
		pBiffReader->GetFloat(m_d.m_rotZ);
		break;
	case FID(COLR):
		pBiffReader->GetInt(m_d.m_color);
		break;
	case FID(TMON):
		pBiffReader->GetBool(m_d.m_tdr.m_TimerEnabled);
		break;
	case FID(TMIN):
		pBiffReader->GetInt(m_d.m_tdr.m_TimerInterval);
		break;
	case FID(IMAG):
		pBiffReader->GetString(m_d.m_szImageA);
		break;
	case FID(IMAB):
		pBiffReader->GetString(m_d.m_szImageB);
		break;
	case FID(FALP):
	{
		int iTmp;
		pBiffReader->GetInt(iTmp);
		if (iTmp < 0) {
			iTmp = 0;
		}
		m_d.m_alpha = iTmp;
		break;
	}
	case FID(MOVA):
		pBiffReader->GetFloat(m_d.m_modulate_vs_add);
		break;
	case FID(NAME):
		pBiffReader->GetWideString(m_wzName, sizeof(m_wzName) / sizeof(wchar_t));
		break;
	case FID(FVIS):
		pBiffReader->GetBool(m_d.m_isVisible);
		break;
	case FID(ADDB):
		pBiffReader->GetBool(m_d.m_addBlend);
		break;
	case FID(IDMD):
		pBiffReader->GetBool(m_d.m_isDMD);
		break;
	case FID(DSPT):
		pBiffReader->GetBool(m_d.m_displayTexture);
		break;
	case FID(FLDB):
		pBiffReader->GetFloat(m_d.m_depthBias);
		break;
	case FID(ALGN):
		pBiffReader->GetInt(&m_d.m_imagealignment);
		break;
	case FID(FILT):
		pBiffReader->GetInt(&m_d.m_filter);
		break;
	case FID(FIAM):
		pBiffReader->GetInt(m_d.m_filterAmount);
		break;
	default:
	{
		// TODO: LoadPointToken(id, pBiffReader, pBiffReader->m_version);
		ISelect::LoadToken(id, pBiffReader);
		break;
	}
	}
	return true;
}

void Flasher::WriteRegDefaults()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	pRegUtil->SaveValueFloat("DefaultProps\\Flasher", "Height", m_d.m_height);
	pRegUtil->SaveValueFloat("DefaultProps\\Flasher", "RotX", m_d.m_rotX);
	pRegUtil->SaveValueFloat("DefaultProps\\Flasher", "RotY", m_d.m_rotY);
	pRegUtil->SaveValueFloat("DefaultProps\\Flasher", "RotZ", m_d.m_rotZ);
	pRegUtil->SaveValueInt("DefaultProps\\Flasher", "Color", m_d.m_color);
	pRegUtil->SaveValueBool("DefaultProps\\Flasher", "TimerEnabled", m_d.m_tdr.m_TimerEnabled);
	pRegUtil->SaveValueInt("DefaultProps\\Flasher", "TimerInterval", m_d.m_tdr.m_TimerInterval);
	pRegUtil->SaveValue("DefaultProps\\Flasher", "ImageA", m_d.m_szImageA);
	pRegUtil->SaveValue("DefaultProps\\Flasher", "ImageB", m_d.m_szImageB);
	pRegUtil->SaveValueInt("DefaultProps\\Flasher", "Alpha", m_d.m_alpha);
	pRegUtil->SaveValueFloat("DefaultProps\\Flasher", "ModulateVsAdd", m_d.m_modulate_vs_add);
	pRegUtil->SaveValueBool("DefaultProps\\Flasher", "Visible", m_d.m_isVisible);
	pRegUtil->SaveValueBool("DefaultProps\\Flasher", "DisplayTexture", m_d.m_displayTexture);
	pRegUtil->SaveValueBool("DefaultProps\\Flasher", "AddBlend", m_d.m_addBlend);
	pRegUtil->SaveValueBool("DefaultProps\\Flasher", "DMD", m_d.m_isDMD);
	pRegUtil->SaveValueInt("DefaultProps\\Flasher", "ImageMode", m_d.m_imagealignment);
	pRegUtil->SaveValueInt("DefaultProps\\Flasher", "Filter", m_d.m_filter);
	pRegUtil->SaveValueInt("DefaultProps\\Flasher", "FilterAmount", m_d.m_filterAmount);
}