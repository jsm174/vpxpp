#include "Ramp.h"
#include "DragPoint.h"
#include "RegUtil.h"

const ItemTypeEnum Ramp::ItemType = eItemRamp;
const int Ramp::TypeNameID = 145;
const int Ramp::ToolID = 146;
const int Ramp::CursorID = 150;
const unsigned Ramp::AllowedViews = 1;

Ramp* Ramp::COMCreate()
{
	/*CComObject<Ramp>* obj = 0;
	if ((((HRESULT)(CComObject<Ramp>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Ramp();
}

IEditable* Ramp::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Ramp::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Ramp* obj = Ramp::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Ramp::Ramp()
{
	// TODO: m_menuid = IDR_SURFACEMENU;
	m_d.m_collidable = true;
	m_d.m_visible = true;
	m_dynamicVertexBuffer = NULL;
	m_dynamicIndexBuffer = NULL;
	m_dynamicVertexBuffer2 = NULL;
	m_dynamicVertexBufferRegenerate = true;
	m_d.m_depthBias = 0.0f;
	m_d.m_wireDiameter = 6.0f;
	m_d.m_wireDistanceX = 38.0f;
	m_d.m_wireDistanceY = 88.0f;
	// TODO: m_propPosition = NULL;
	// TODO: m_propPhysics = NULL;
	m_d.m_hitEvent = false;
	m_d.m_overwritePhysics = true;
	m_rgheightInit = NULL;
}

Ramp::~Ramp()
{
}

HRESULT Ramp::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_ptable = ptable;

	SetDefaults(fromMouseClick);
	m_d.m_visible = true;

	float length = 0.5f * pRegUtil->LoadValueFloatWithDefault("DefaultProps\\Ramp", "Length", 400.0f);

	// CComObject<DragPoint> *pdp;
	// CComObject<DragPoint>::CreateInstance(&pdp);
	// if (pdp)
	// {
	//    pdp->AddRef();
	//    pdp->Init(this, x, y + length, 0.f, true);
	//    pdp->m_calcHeight = m_d.m_heightbottom;
	//    m_vdpoint.push_back(pdp);
	// }

	// CComObject<DragPoint>::CreateInstance(&pdp);
	// if (pdp)
	// {
	//    pdp->AddRef();
	//    pdp->Init(this, x, y - length, 0.f, true);
	//    pdp->m_calcHeight = m_d.m_heighttop;
	//    m_vdpoint.push_back(pdp);
	// }

	InitVBA(true, 0, NULL);

	return S_OK;
}

HRESULT Ramp::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			GetPTable()->GetUniqueName(eItemRamp, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t*)m_wzName,
			//             sizeof(m_wzName) / sizeof(m_wzName[0]));
		}
	}
	InitScript();
	return ((HRESULT)0L);
}

PinTable* Ramp::GetPTable()
{
	return m_ptable;
}

HRESULT Ramp::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	SetDefaults(false);

	m_ptable = pTable;

	BiffReader biffReader(pStream, this, pId, version);
	biffReader.Load();

	return S_OK;
}

HRESULT Ramp::InitPostLoad()
{
	return S_OK;
}

void Ramp::SetDefaults(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	static const char strKeyName[] = "DefaultProps\\Ramp";

	m_d.m_heightbottom = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "HeightBottom", 0.0f) : 0.0f;
	m_d.m_heighttop = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "HeightTop", 50.0f) : 50.0f;
	m_d.m_widthbottom = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "WidthBottom", 75.0f) : 75.0f;
	m_d.m_widthtop = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "WidthTop", 60.0f) : 60.0f;
	m_d.m_type = fromMouseClick ? (RampType)pRegUtil->LoadValueIntWithDefault(strKeyName, "RampType", RampTypeFlat) : RampTypeFlat;

	m_d.m_tdr.m_TimerEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "TimerEnabled", false) : false;
	m_d.m_tdr.m_TimerInterval = fromMouseClick ? pRegUtil->LoadValueIntWithDefault(strKeyName, "TimerInterval", 100) : 100;

	HRESULT hr = pRegUtil->LoadValue(strKeyName, "Image", m_d.m_szImage);
	if ((hr != S_OK) || !fromMouseClick)
	{
		m_d.m_szImage.clear();
	}

	m_d.m_imagealignment = fromMouseClick ? (RampImageAlignment)pRegUtil->LoadValueIntWithDefault(strKeyName, "ImageMode", ImageModeWorld) : ImageModeWorld;
	m_d.m_imageWalls = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "ImageWalls", true) : true;

	m_d.m_leftwallheight = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "LeftWallHeight", 62.0f) : 62.0f;
	m_d.m_rightwallheight = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "RightWallHeight", 62.0f) : 62.0f;
	m_d.m_leftwallheightvisible = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "LeftWallHeightVisible", 30.0f) : 30.0f;
	m_d.m_rightwallheightvisible = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "RightWallHeightVisible", 30.0f) : 30.0f;

	m_d.m_threshold = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "HitThreshold", 2.0f) : 2.0f;

	SetDefaultPhysics(fromMouseClick);

	m_d.m_visible = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "Visible", true) : true;
	m_d.m_collidable = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "Collidable", true) : true;
	m_d.m_reflectionEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "ReflectionEnabled", true) : true;

	m_d.m_wireDiameter = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "WireDiameter", 8.0f) : 8.0f;
	m_d.m_wireDistanceX = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "WireDistanceX", 38.0f) : 38.0f;
	m_d.m_wireDistanceY = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "WireDistanceY", 88.0f) : 88.0f;
}

void Ramp::SetDefaultPhysics(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	static const char strKeyName[] = "DefaultProps\\Ramp";
	m_d.m_elasticity = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "Elasticity", 0.3f) : 0.3f;
	m_d.m_friction = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "Friction", 0.3f) : 0.3f;
	m_d.m_scatter = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "Scatter", 0) : 0;
}

bool Ramp::LoadToken(const int id, BiffReader* const pBiffReader)
{
	switch (id)
	{
	case FID(PIID):
		pBiffReader->GetInt((int*)pBiffReader->m_pData);
		break;
	case FID(HTBT):
		pBiffReader->GetFloat(m_d.m_heightbottom);
		break;
	case FID(HTTP):
		pBiffReader->GetFloat(m_d.m_heighttop);
		break;
	case FID(WDBT):
		pBiffReader->GetFloat(m_d.m_widthbottom);
		break;
	case FID(WDTP):
		pBiffReader->GetFloat(m_d.m_widthtop);
		break;
	case FID(MATR):
		pBiffReader->GetString(m_d.m_szMaterial);
		break;
	case FID(TMON):
		pBiffReader->GetBool(m_d.m_tdr.m_TimerEnabled);
		break;
	case FID(TMIN):
		pBiffReader->GetInt(m_d.m_tdr.m_TimerInterval);
		break;
	case FID(TYPE):
		pBiffReader->GetInt(&m_d.m_type);
		break;
	case FID(IMAG):
		pBiffReader->GetString(m_d.m_szImage);
		break;
	case FID(ALGN):
		pBiffReader->GetInt(&m_d.m_imagealignment);
		break;
	case FID(IMGW):
		pBiffReader->GetBool(m_d.m_imageWalls);
		break;
	case FID(NAME):
		pBiffReader->GetWideString(m_wzName, sizeof(m_wzName) / sizeof(wchar_t));
		break;
	case FID(WLHL):
		pBiffReader->GetFloat(m_d.m_leftwallheight);
		break;
	case FID(WLHR):
		pBiffReader->GetFloat(m_d.m_rightwallheight);
		break;
	case FID(WVHL):
		pBiffReader->GetFloat(m_d.m_leftwallheightvisible);
		break;
	case FID(WVHR):
		pBiffReader->GetFloat(m_d.m_rightwallheightvisible);
		break;
	case FID(HTEV):
		pBiffReader->GetBool(m_d.m_hitEvent);
		break;
	case FID(THRS):
		pBiffReader->GetFloat(m_d.m_threshold);
		break;
	case FID(ELAS):
		pBiffReader->GetFloat(m_d.m_elasticity);
		break;
	case FID(RFCT):
		pBiffReader->GetFloat(m_d.m_friction);
		break;
	case FID(RSCT):
		pBiffReader->GetFloat(m_d.m_scatter);
		break;
	case FID(CLDR):
		pBiffReader->GetBool(m_d.m_collidable);
		break;
	case FID(RVIS):
		pBiffReader->GetBool(m_d.m_visible);
		break;
	case FID(REEN):
		pBiffReader->GetBool(m_d.m_reflectionEnabled);
		break;
	case FID(RADB):
		pBiffReader->GetFloat(m_d.m_depthBias);
		break;
	case FID(RADI):
		pBiffReader->GetFloat(m_d.m_wireDiameter);
		break;
	case FID(RADX):
		pBiffReader->GetFloat(m_d.m_wireDistanceX);
		break;
	case FID(RADY):
		pBiffReader->GetFloat(m_d.m_wireDistanceY);
		break;
	case FID(MAPH):
		pBiffReader->GetString(m_d.m_szPhysicsMaterial);
		break;
	case FID(OVPH):
		pBiffReader->GetBool(m_d.m_overwritePhysics);
		break;
	default:
	{
		LoadPointToken(id, pBiffReader, pBiffReader->m_version);
		ISelect::LoadToken(id, pBiffReader);
		break;
	}
	}
	return true;
}

ItemTypeEnum Ramp::GetItemType() const
{
	return eItemRamp;
}

IEditable* Ramp::GetIEditable()
{
	return static_cast<IEditable*>(this);
}

IHitable* Ramp::GetIHitable()
{
	return static_cast<IHitable*>(this);
}

void Ramp::WriteRegDefaults()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	static const char strKeyName[] = "DefaultProps\\Ramp";

	pRegUtil->SaveValueFloat(strKeyName, "HeightBottom", m_d.m_heightbottom);
	pRegUtil->SaveValueFloat(strKeyName, "HeightTop", m_d.m_heighttop);
	pRegUtil->SaveValueFloat(strKeyName, "WidthBottom", m_d.m_widthbottom);
	pRegUtil->SaveValueFloat(strKeyName, "WidthTop", m_d.m_widthtop);
	pRegUtil->SaveValueInt(strKeyName, "RampType", m_d.m_type);
	pRegUtil->SaveValueBool(strKeyName, "TimerEnabled", m_d.m_tdr.m_TimerEnabled);
	pRegUtil->SaveValueInt(strKeyName, "TimerInterval", m_d.m_tdr.m_TimerInterval);
	pRegUtil->SaveValue(strKeyName, "Image", m_d.m_szImage);
	pRegUtil->SaveValueInt(strKeyName, "ImageMode", m_d.m_imagealignment);
	pRegUtil->SaveValueBool(strKeyName, "ImageWalls", m_d.m_imageWalls);
	pRegUtil->SaveValueFloat(strKeyName, "LeftWallHeight", m_d.m_leftwallheight);
	pRegUtil->SaveValueFloat(strKeyName, "RightWallHeight", m_d.m_rightwallheight);
	pRegUtil->SaveValueFloat(strKeyName, "LeftWallHeightVisible", m_d.m_leftwallheightvisible);
	pRegUtil->SaveValueFloat(strKeyName, "RightWallHeightVisible", m_d.m_rightwallheightvisible);
	pRegUtil->SaveValueBool(strKeyName, "HitEvent", m_d.m_hitEvent);
	pRegUtil->SaveValueFloat(strKeyName, "HitThreshold", m_d.m_threshold);
	pRegUtil->SaveValueFloat(strKeyName, "Elasticity", m_d.m_elasticity);
	pRegUtil->SaveValueFloat(strKeyName, "Friction", m_d.m_friction);
	pRegUtil->SaveValueFloat(strKeyName, "Scatter", m_d.m_scatter);
	pRegUtil->SaveValueBool(strKeyName, "Collidable", m_d.m_collidable);
	pRegUtil->SaveValueBool(strKeyName, "Visible", m_d.m_visible);
	pRegUtil->SaveValueBool(strKeyName, "ReflectionEnabled", m_d.m_reflectionEnabled);
	pRegUtil->SaveValueFloat(strKeyName, "WireDiameter", m_d.m_wireDiameter);
	pRegUtil->SaveValueFloat(strKeyName, "WireDistanceX", m_d.m_wireDistanceX);
	pRegUtil->SaveValueFloat(strKeyName, "WireDistanceY", m_d.m_wireDistanceY);
}

void Ramp::GetHitShapes(std::vector<HitObject*>& pvho)
{
}

void Ramp::GetHitShapesDebug(std::vector<HitObject*>& pvho)
{
}

void Ramp::RenderStatic()
{
}

void Ramp::RenderDynamic()
{
}

void Ramp::RenderSetup()
{
}

ItemTypeEnum Ramp::HitableGetItemType() const
{
	return eItemRamp;
}

void Ramp::GetBoundingVertices(std::vector<Vertex3Ds>& pvvertex3D)
{
	float* rgheight1;
	int cvertex;
	const Vertex2D* const rgvLocal = GetRampVertex(cvertex, &rgheight1, NULL, NULL, NULL, HIT_SHAPE_DETAIL_LEVEL, true);

	Vertex3Ds bbox_min(FLT_MAX, FLT_MAX, FLT_MAX);
	Vertex3Ds bbox_max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (int i = 0; i < cvertex; i++)
	{
		{
			const Vertex3Ds pv(rgvLocal[i].x, rgvLocal[i].y, rgheight1[i] + (float)(2.0 * PHYS_SKIN)); // leave room for ball //!! use ballsize
			bbox_min.x = min(bbox_min.x, pv.x);
			bbox_min.y = min(bbox_min.y, pv.y);
			bbox_min.z = min(bbox_min.z, pv.z);
			bbox_max.x = max(bbox_max.x, pv.x);
			bbox_max.y = max(bbox_max.y, pv.y);
			bbox_max.z = max(bbox_max.z, pv.z);
		}

		const Vertex3Ds pv(rgvLocal[cvertex * 2 - i - 1].x, rgvLocal[cvertex * 2 - i - 1].y, rgheight1[i] + (float)(2.0 * PHYS_SKIN)); // leave room for ball //!! use ballsize
		bbox_min.x = min(bbox_min.x, pv.x);
		bbox_min.y = min(bbox_min.y, pv.y);
		bbox_min.z = min(bbox_min.z, pv.z);
		bbox_max.x = max(bbox_max.x, pv.x);
		bbox_max.y = max(bbox_max.y, pv.y);
		bbox_max.z = max(bbox_max.z, pv.z);
	}

	delete[] rgvLocal;
	delete[] rgheight1;

	for (int i = 0; i < 8; i++)
	{
		const Vertex3Ds pv(
			(i & 1) ? bbox_min.x : bbox_max.x,
			(i & 2) ? bbox_min.y : bbox_max.y,
			(i & 4) ? bbox_min.z : bbox_max.z);

		pvvertex3D.push_back(pv);
	}
}

void Ramp::AssignHeightToControlPoint(const RenderVertex3D& v, const float height)
{
	for (size_t i = 0; i < m_vdpoint.size(); i++)
	{
		if (m_vdpoint[i]->m_v.x == v.x && m_vdpoint[i]->m_v.y == v.y)
		{
			m_vdpoint[i]->m_calcHeight = height;
		}
	}
}

//
// license:GPLv3+
// Ported at: VisualPinball.Engine/VPT/Ramp/RampMeshGenerator.cs
//

Vertex2D* Ramp::GetRampVertex(int& pcvertex, float** const ppheight, bool** const ppfCross, float** const ppratio, Vertex2D** const pMiddlePoints, const float _accuracy, const bool inc_width)
{
	std::vector<RenderVertex3D> vvertex;
	GetCentralCurve(vvertex, _accuracy);

	const int cvertex = (int)vvertex.size();
	pcvertex = cvertex;
	Vertex2D* const rgvLocal = new Vertex2D[cvertex * 2];

	if (ppheight)
	{
		*ppheight = new float[cvertex];
	}

	if (ppfCross)
	{
		*ppfCross = new bool[cvertex];
	}

	if (ppratio)
	{
		*ppratio = new float[cvertex];
	}

	if (pMiddlePoints)
	{
		*pMiddlePoints = new Vertex2D[cvertex];
	}

	float totallength = 0;
	const float bottomHeight = m_d.m_heightbottom + m_ptable->m_tableheight;
	const float topHeight = m_d.m_heighttop + m_ptable->m_tableheight;

	for (int i = 0; i < (cvertex - 1); i++)
	{
		const RenderVertex3D& v1 = vvertex[i];
		const RenderVertex3D& v2 = vvertex[i + 1];

		const float dx = v1.x - v2.x;
		const float dy = v1.y - v2.y;
		const float length = sqrtf(dx * dx + dy * dy);

		totallength += length;
	}

	float currentlength = 0;
	for (int i = 0; i < cvertex; i++)
	{
		const RenderVertex3D& vprev = vvertex[(i > 0) ? i - 1 : i];
		const RenderVertex3D& vnext = vvertex[(i < (cvertex - 1)) ? i + 1 : i];
		const RenderVertex3D& vmiddle = vvertex[i];

		if (ppfCross)
		{
			(*ppfCross)[i] = vmiddle.controlPoint;
		}

		Vertex2D vnormal;
		{
			Vertex2D v1normal(vprev.y - vmiddle.y, vmiddle.x - vprev.x);
			Vertex2D v2normal(vmiddle.y - vnext.y, vnext.x - vmiddle.x);

			if (i == (cvertex - 1))
			{
				v1normal.Normalize();
				vnormal = v1normal;
			}
			else if (i == 0)
			{
				v2normal.Normalize();
				vnormal = v2normal;
			}
			else
			{
				v1normal.Normalize();
				v2normal.Normalize();

				if (fabsf(v1normal.x - v2normal.x) < 0.0001f && fabsf(v1normal.y - v2normal.y) < 0.0001f)
				{
					vnormal = v1normal;
				}
				else
				{
					const float A = vprev.y - vmiddle.y;
					const float B = vmiddle.x - vprev.x;

					const float C = A * (v1normal.x - vprev.x) + B * (v1normal.y - vprev.y);

					const float D = vnext.y - vmiddle.y;
					const float E = vmiddle.x - vnext.x;

					const float F = D * (v2normal.x - vnext.x) + E * (v2normal.y - vnext.y);

					const float det = A * E - B * D;
					const float inv_det = (det != 0.0f) ? 1.0f / det : 0.0f;

					const float intersectx = (B * F - E * C) * inv_det;
					const float intersecty = (C * D - A * F) * inv_det;

					vnormal.x = vmiddle.x - intersectx;
					vnormal.y = vmiddle.y - intersecty;
				}
			}
		}

		{
			const float dx = vprev.x - vmiddle.x;
			const float dy = vprev.y - vmiddle.y;
			const float length = sqrtf(dx * dx + dy * dy);

			currentlength += length;
		}

		const float percentage = currentlength / totallength;
		float widthcur = percentage * (m_d.m_widthtop - m_d.m_widthbottom) + m_d.m_widthbottom;
		if (ppheight)
		{
			(*ppheight)[i] = vmiddle.z + percentage * (topHeight - bottomHeight) + bottomHeight;
		}

		AssignHeightToControlPoint(vvertex[i], vmiddle.z + percentage * (topHeight - bottomHeight) + bottomHeight);
		if (ppratio)
		{
			(*ppratio)[i] = 1.0f - percentage;
		}

		if (isHabitrail() && m_d.m_type != RampType1Wire)
		{
			widthcur = m_d.m_wireDistanceX;
			if (inc_width)
			{
				widthcur += 20.0f;
			}
		}
		else if (m_d.m_type == RampType1Wire)
		{
			widthcur = m_d.m_wireDiameter;
		}

		if (pMiddlePoints)
		{
			(*pMiddlePoints)[i] = Vertex2D(vmiddle.x, vmiddle.y) + vnormal;
		}
		rgvLocal[i] = Vertex2D(vmiddle.x, vmiddle.y) + (widthcur * 0.5f) * vnormal;
		rgvLocal[cvertex * 2 - i - 1] = Vertex2D(vmiddle.x, vmiddle.y) - (widthcur * 0.5f) * vnormal;
	}

	return rgvLocal;
}

template <typename T>
void Ramp::GetCentralCurve(std::vector<T>& vv, const float _accuracy) const
{
	float accuracy;

	if (_accuracy != -1.f)
	{
		accuracy = _accuracy;
	}
	else
	{
		const Material* const mat = m_ptable->GetMaterial(m_d.m_szMaterial);
		if (!mat->m_bOpacityActive)
		{
			accuracy = 10.f;
		}
		else
		{
			accuracy = (float)m_ptable->GetDetailLevel();
		}
	}

	accuracy = 4.0f * powf(10.0f, (10.0f - accuracy) * (float)(1.0 / 1.5));

	IHaveDragPoints::GetRgVertex(vv, false, accuracy);
}

//
// license:GPLv3+
// Ported at: VisualPinball.Engine/VPT/Ramp/RampHitGenerator.cs
//

bool Ramp::isHabitrail() const
{
	return m_d.m_type == RampType4Wire || m_d.m_type == RampType1Wire || m_d.m_type == RampType2Wire || m_d.m_type == RampType3WireLeft || m_d.m_type == RampType3WireRight;
}

//
// end of license:GPLv3+, back to 'old MAME'-like
//