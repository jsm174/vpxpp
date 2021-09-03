#include "Primitive.h"
#include "RegUtil.h"

const int Primitive::Max_Primitive_Sides = 100;

const ItemTypeEnum Primitive::ItemType = eItemPrimitive;
const int Primitive::TypeNameID = 186;
const int Primitive::ToolID = 32842;
const int Primitive::CursorID = 385;
const unsigned Primitive::AllowedViews = 1;

Primitive* Primitive::COMCreate()
{
	/*CComObject<Primitive>* obj = 0;
	if ((((HRESULT)(CComObject<Primitive>::CreateInstance(&obj))) < 0))
	{
		//MessageBoxA(0, "Failed to create COM object.", "Visual Pinball",
		//            0x00000030L);
	}
	obj->AddRef();
	return obj;*/
	return new Primitive();
}

IEditable* Primitive::COMCreateEditable()
{
	return static_cast<IEditable*>(COMCreate());
}

IEditable* Primitive::COMCreateAndInit(PinTable* ptable, float x, float y)
{
	Primitive* obj = Primitive::COMCreate();
	obj->Init(ptable, x, y, true);
	return obj;
}

Primitive::Primitive()
{
	m_vertexBuffer = 0;
	m_vertexBufferRegenerate = true;
	m_indexBuffer = 0;
	m_d.m_use3DMesh = false;
	m_d.m_staticRendering = false;
	m_d.m_edgeFactorUI = 0.25f;
	m_d.m_collision_reductionFactor = 0.f;
	m_d.m_depthBias = 0.0f;
	m_d.m_skipRendering = false;
	m_d.m_groupdRendering = false;
	m_d.m_reflectionEnabled = true;
	m_numGroupIndices = 0;
	m_numGroupVertices = 0;
	m_currentFrame = -1.f;
	m_lockedByLS = false;

	m_numIndices = 0;
	m_numVertices = 0;
	// TODO: m_propPhysics = NULL;
	// TODO: m_propPosition = NULL;
	// TODO: m_propVisual = NULL;
	m_d.m_overwritePhysics = true;
	m_d.m_useAsPlayfield = false;
}

Primitive::~Primitive()
{
}

HRESULT Primitive::Init(PinTable* ptable, float x, float y, bool fromMouseClick)
{
	m_ptable = ptable;

	m_d.m_vPosition.x = x;
	m_d.m_vPosition.y = y;

	SetDefaults(fromMouseClick);

	InitVBA(true, 0, NULL);

	// TODO:UpdateStatusBarInfo();

	return S_OK;
}

HRESULT Primitive::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	wchar_t wzUniqueName[128];
	if (fNew && !wzName)
	{
		{
			GetPTable()->GetUniqueName(eItemPrimitive, wzUniqueName, 128);
			//WideStrNCopy(wzUniqueName, (wchar_t*)m_wzName,
			//             sizeof(m_wzName) / sizeof(m_wzName[0]));
		}
	}
	InitScript();
	return ((HRESULT)0L);
}

PinTable* Primitive::GetPTable()
{
	return m_ptable;
}

HRESULT Primitive::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	SetDefaults(false);

	m_ptable = pTable;

	BiffReader biffReader(pStream, this, pId, version);
	biffReader.Load();

	if (version < 1011)
	{
		// TODO: WaitForMeshDecompression();

		// unsigned int* const tmp = reorderForsyth(m_mesh.m_indices, (int)m_mesh.NumVertices());
		// if (tmp != NULL)
		// {
		// 	memcpy(m_mesh.m_indices.data(), tmp, m_mesh.NumIndices() * sizeof(unsigned int));
		// 	delete[] tmp;
		// }
	}

	m_inPlayState = m_d.m_visible;

	return S_OK;
}

void Primitive::SetDefaults(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	static const char strKeyName[] = "DefaultProps\\Primitive";

	m_d.m_useAsPlayfield = false;
	m_d.m_use3DMesh = false;

	m_d.m_meshFileName.clear();

	m_d.m_Sides = fromMouseClick ? pRegUtil->LoadValueIntWithDefault(strKeyName, "Sides", 4) : 4;
	if (m_d.m_Sides > Max_Primitive_Sides)
	{
		m_d.m_Sides = Max_Primitive_Sides;
	}

	m_d.m_SideColor = fromMouseClick ? pRegUtil->LoadValueIntWithDefault(strKeyName, "SideColor", RGB(150, 150, 150)) : RGB(150, 150, 150);

	m_d.m_visible = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "Visible", true) : true;
	m_inPlayState = m_d.m_visible;
	m_d.m_staticRendering = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "StaticRendering", true) : true;
	m_d.m_drawTexturesInside = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "DrawTexturesInside", false) : false;

	m_d.m_vPosition.z = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "Position_Z", 0.0f) : 0.0f;

	m_d.m_vSize.x = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "Size_X", 100.0f) : 100.0f;
	m_d.m_vSize.y = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "Size_Y", 100.0f) : 100.0f;
	m_d.m_vSize.z = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "Size_Z", 100.0f) : 100.0f;

	m_d.m_aRotAndTra[0] = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "RotAndTra0", 0.0f) : 0.0f;
	m_d.m_aRotAndTra[1] = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "RotAndTra1", 0.0f) : 0.0f;
	m_d.m_aRotAndTra[2] = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "RotAndTra2", 0.0f) : 0.0f;
	m_d.m_aRotAndTra[3] = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "RotAndTra3", 0.0f) : 0.0f;
	m_d.m_aRotAndTra[4] = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "RotAndTra4", 0.0f) : 0.0f;
	m_d.m_aRotAndTra[5] = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "RotAndTra5", 0.0f) : 0.0f;
	m_d.m_aRotAndTra[6] = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "RotAndTra6", 0.0f) : 0.0f;
	m_d.m_aRotAndTra[7] = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "RotAndTra7", 0.0f) : 0.0f;
	m_d.m_aRotAndTra[8] = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "RotAndTra8", 0.0f) : 0.0f;

	HRESULT hr = pRegUtil->LoadValue(strKeyName, "Image", m_d.m_szImage);
	if ((hr != S_OK) && fromMouseClick)
	{
		m_d.m_szImage.clear();
	}

	hr = pRegUtil->LoadValue(strKeyName, "NormalMap", m_d.m_szNormalMap);
	if ((hr != S_OK) && fromMouseClick)
	{
		m_d.m_szNormalMap.clear();
	}

	m_d.m_threshold = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "HitThreshold", 2.0f) : 2.0f;

	SetDefaultPhysics(fromMouseClick);

	m_d.m_edgeFactorUI = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "EdgeFactorUI", 0.25f) : 0.25f;
	m_d.m_collision_reductionFactor = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "CollisionReductionFactor", 0.f) : 0.f;

	m_d.m_collidable = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "Collidable", true) : true;
	m_d.m_toy = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "IsToy", false) : false;
	m_d.m_disableLightingTop = dequantizeUnsigned<8>(fromMouseClick ? pRegUtil->LoadValueIntWithDefault(strKeyName, "DisableLighting", 0) : 0); // stored as uchar for backward compatibility
	m_d.m_disableLightingBelow = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "DisableLightingBelow", 0.f) : 0.f;
	m_d.m_reflectionEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "ReflectionEnabled", true) : true;
	m_d.m_backfacesEnabled = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "BackfacesEnabled", false) : false;
	m_d.m_displayTexture = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "DisplayTexture", false) : false;
	m_d.m_objectSpaceNormalMap = fromMouseClick ? pRegUtil->LoadValueBoolWithDefault(strKeyName, "ObjectSpaceNormalMap", false) : false;
}

void Primitive::SetDefaultPhysics(bool fromMouseClick)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	static const char strKeyName[] = "DefaultProps\\Primitive";

	m_d.m_elasticity = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "Elasticity", 0.3f) : 0.3f;
	m_d.m_elasticityFalloff = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "ElasticityFalloff", 0.5f) : 0.5f;
	m_d.m_friction = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "Friction", 0.3f) : 0.3f;
	m_d.m_scatter = fromMouseClick ? pRegUtil->LoadValueFloatWithDefault(strKeyName, "Scatter", 0) : 0;
}

bool Primitive::LoadToken(const int id, BiffReader* const pBiffReader)
{
	switch (id)
	{
	case FID(PIID):
		pBiffReader->GetInt((int*)pBiffReader->m_pData);
		break;
	case FID(VPOS):
		pBiffReader->GetVector3Padded(m_d.m_vPosition);
		break;
	case FID(VSIZ):
		pBiffReader->GetVector3Padded(m_d.m_vSize);
		break;
	case FID(RTV0):
		pBiffReader->GetFloat(m_d.m_aRotAndTra[0]);
		break;
	case FID(RTV1):
		pBiffReader->GetFloat(m_d.m_aRotAndTra[1]);
		break;
	case FID(RTV2):
		pBiffReader->GetFloat(m_d.m_aRotAndTra[2]);
		break;
	case FID(RTV3):
		pBiffReader->GetFloat(m_d.m_aRotAndTra[3]);
		break;
	case FID(RTV4):
		pBiffReader->GetFloat(m_d.m_aRotAndTra[4]);
		break;
	case FID(RTV5):
		pBiffReader->GetFloat(m_d.m_aRotAndTra[5]);
		break;
	case FID(RTV6):
		pBiffReader->GetFloat(m_d.m_aRotAndTra[6]);
		break;
	case FID(RTV7):
		pBiffReader->GetFloat(m_d.m_aRotAndTra[7]);
		break;
	case FID(RTV8):
		pBiffReader->GetFloat(m_d.m_aRotAndTra[8]);
		break;
	case FID(IMAG):
		pBiffReader->GetString(m_d.m_szImage);
		break;
	case FID(NRMA):
		pBiffReader->GetString(m_d.m_szNormalMap);
		break;
	case FID(SIDS):
		pBiffReader->GetInt(m_d.m_Sides);
		break;
	case FID(NAME):
		pBiffReader->GetWideString(m_wzName, sizeof(m_wzName) / sizeof(wchar_t));
		break;
	case FID(MATR):
		pBiffReader->GetString(m_d.m_szMaterial);
		break;
	case FID(SCOL):
		pBiffReader->GetInt(m_d.m_SideColor);
		break;
	case FID(TVIS):
		pBiffReader->GetBool(m_d.m_visible);
		break;
	case FID(REEN):
		pBiffReader->GetBool(m_d.m_reflectionEnabled);
		break;
	case FID(DTXI):
		pBiffReader->GetBool(m_d.m_drawTexturesInside);
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
	case FID(ELFO):
		pBiffReader->GetFloat(m_d.m_elasticityFalloff);
		break;
	case FID(RFCT):
		pBiffReader->GetFloat(m_d.m_friction);
		break;
	case FID(RSCT):
		pBiffReader->GetFloat(m_d.m_scatter);
		break;
	case FID(EFUI):
		pBiffReader->GetFloat(m_d.m_edgeFactorUI);
		break;
	case FID(CORF):
		pBiffReader->GetFloat(m_d.m_collision_reductionFactor);
		break;
	case FID(CLDR):
		pBiffReader->GetBool(m_d.m_collidable);
		break;
	case FID(ISTO):
		pBiffReader->GetBool(m_d.m_toy);
		break;
	case FID(MAPH):
		pBiffReader->GetString(m_d.m_szPhysicsMaterial);
		break;
	case FID(OVPH):
		pBiffReader->GetBool(m_d.m_overwritePhysics);
		break;
	case FID(STRE):
		pBiffReader->GetBool(m_d.m_staticRendering);
		break;
	case FID(DILI):
	{
		int tmp;
		pBiffReader->GetInt(tmp);
		m_d.m_disableLightingTop = (tmp == 1) ? 1.f : dequantizeUnsigned<8>(tmp); // backwards compatible hacky loading!
		break;
	}
	case FID(DILB):
		pBiffReader->GetFloat(m_d.m_disableLightingBelow);
		break;
	case FID(U3DM):
		pBiffReader->GetBool(m_d.m_use3DMesh);
		break;
	case FID(EBFC):
		pBiffReader->GetBool(m_d.m_backfacesEnabled);
		break;
	case FID(DIPT):
		pBiffReader->GetBool(m_d.m_displayTexture);
		break;
	case FID(M3DN):
		pBiffReader->GetString(m_d.m_meshFileName);
		break;
	case FID(M3VN):
	{
		pBiffReader->GetInt(m_numVertices);
		// TODO: if (!m_mesh.m_animationFrames.empty())
		// {
		// 	for (size_t i = 0; i < m_mesh.m_animationFrames.size(); i++)
		// 		m_mesh.m_animationFrames[i].m_frameVerts.clear();
		// 	m_mesh.m_animationFrames.clear();
		// }
		break;
	}
	case FID(M3DX):
	{
		// TODO: m_mesh.m_vertices.clear();
		// m_mesh.m_vertices.resize(m_numVertices);
		// pBiffReader->GetStruct(m_mesh.m_vertices.data(), (int)sizeof(Vertex3D_NoTex2) * m_numVertices);
		break;
	}
#ifdef COMPRESS_MESHES
	case FID(M3AY):
		pBiffReader->GetInt(m_compressedAnimationVertices);
		break;
	case FID(M3AX):
	{
		Mesh::FrameData frameData;
		frameData.m_frameVerts.clear();
		frameData.m_frameVerts.resize(m_numVertices);

		/*LZWReader lzwreader(pBiffReader->m_pistream, (int *)m_mesh.m_vertices.data(), sizeof(Vertex3D_NoTex2)*numVertices, 1, sizeof(Vertex3D_NoTex2)*numVertices);
      lzwreader.Decoder();*/
		mz_ulong uclen = (mz_ulong)(sizeof(Mesh::VertData) * m_mesh.NumVertices());
		mz_uint8* c = (mz_uint8*)malloc(m_compressedAnimationVertices);
		pBiffReader->GetStruct(c, m_compressedAnimationVertices);
		const int error = uncompress((unsigned char*)frameData.m_frameVerts.data(), &uclen, c, m_compressedAnimationVertices);
		if (error != Z_OK)
		{
			char err[128];
			sprintf_s(err, "Could not uncompress primitive animation vertex data, error %d", error);
			ShowError(err);
		}
		free(c);
		m_mesh.m_animationFrames.push_back(frameData);
		break;
	}
	case FID(M3CY):
		pBiffReader->GetInt(m_compressedVertices);
		break;
	case FID(M3CX):
	{
		m_mesh.m_vertices.clear();
		m_mesh.m_vertices.resize(m_numVertices);
		/*LZWReader lzwreader(pBiffReader->m_pistream, (int *)m_mesh.m_vertices.data(), sizeof(Vertex3D_NoTex2)*numVertices, 1, sizeof(Vertex3D_NoTex2)*numVertices);
       lzwreader.Decoder();*/
		mz_ulong uclen = (mz_ulong)(sizeof(Vertex3D_NoTex2) * m_mesh.NumVertices());
		mz_uint8* c = (mz_uint8*)malloc(m_compressedVertices);
		pBiffReader->GetStruct(c, m_compressedVertices);
		if (g_pPrimitiveDecompressThreadPool == NULL)
			g_pPrimitiveDecompressThreadPool = new ThreadPool(g_pvp->m_logicalNumberOfProcessors);

		g_pPrimitiveDecompressThreadPool->enqueue([uclen, c, this]
		                                          {
			                                          mz_ulong uclen2 = uclen;
			                                          const int error = uncompress((unsigned char*)m_mesh.m_vertices.data(), &uclen2, c, m_compressedVertices);
			                                          if (error != Z_OK)
			                                          {
				                                          char err[128];
				                                          sprintf_s(err, "Could not uncompress primitive vertex data, error %d", error);
				                                          ShowError(err);
			                                          }
			                                          free(c);
		                                          });
		break;
	}
#endif
	case FID(M3FN):
		pBiffReader->GetInt(m_numIndices);
		break;
	case FID(M3DI):
	{
		// TODO: m_mesh.m_indices.resize(m_numIndices);
		// if (m_numVertices > 65535)
		// 	pBiffReader->GetStruct(m_mesh.m_indices.data(), (int)sizeof(unsigned int) * m_numIndices);
		// else
		// {
		// 	std::vector<WORD> tmp(m_numIndices);
		// 	pBiffReader->GetStruct(tmp.data(), (int)sizeof(WORD) * m_numIndices);
		// 	for (int i = 0; i < m_numIndices; ++i)
		// 		m_mesh.m_indices[i] = tmp[i];
		// }
		break;
	}
#ifdef COMPRESS_MESHES
	case FID(M3CJ):
		pBiffReader->GetInt(m_compressedIndices);
		break;
	case FID(M3CI):
	{
		m_mesh.m_indices.resize(m_numIndices);
		if (m_numVertices > 65535)
		{
			//LZWReader lzwreader(pBiffReader->m_pistream, (int *)m_mesh.m_indices.data(), sizeof(unsigned int)*numIndices, 1, sizeof(unsigned int)*numIndices);
			//lzwreader.Decoder();
			mz_ulong uclen = (mz_ulong)(sizeof(unsigned int) * m_mesh.NumIndices());
			mz_uint8* c = (mz_uint8*)malloc(m_compressedIndices);
			pBiffReader->GetStruct(c, m_compressedIndices);
			if (g_pPrimitiveDecompressThreadPool == NULL)
				g_pPrimitiveDecompressThreadPool = new ThreadPool(g_pvp->m_logicalNumberOfProcessors);

			g_pPrimitiveDecompressThreadPool->enqueue([uclen, c, this]
			                                          {
				                                          mz_ulong uclen2 = uclen;
				                                          const int error = uncompress((unsigned char*)m_mesh.m_indices.data(), &uclen2, c, m_compressedIndices);
				                                          if (error != Z_OK)
				                                          {
					                                          char err[128];
					                                          sprintf_s(err, "Could not uncompress (large) primitive index data, error %d", error);
					                                          ShowError(err);
				                                          }
				                                          free(c);
			                                          });
		}
		else
		{
			//LZWReader lzwreader(pBiffReader->m_pistream, (int *)tmp.data(), sizeof(WORD)*numIndices, 1, sizeof(WORD)*numIndices);
			//lzwreader.Decoder();
			mz_ulong uclen = (mz_ulong)(sizeof(WORD) * m_mesh.NumIndices());
			mz_uint8* c = (mz_uint8*)malloc(m_compressedIndices);
			pBiffReader->GetStruct(c, m_compressedIndices);
			if (g_pPrimitiveDecompressThreadPool == NULL)
				g_pPrimitiveDecompressThreadPool = new ThreadPool(g_pvp->m_logicalNumberOfProcessors);

			g_pPrimitiveDecompressThreadPool->enqueue([uclen, c, this]
			                                          {
				                                          std::vector<WORD> tmp(m_numIndices);

				                                          mz_ulong uclen2 = uclen;
				                                          const int error = uncompress((unsigned char*)tmp.data(), &uclen2, c, m_compressedIndices);
				                                          if (error != Z_OK)
				                                          {
					                                          char err[128];
					                                          sprintf_s(err, "Could not uncompress (small) primitive index data, error %d", error);
					                                          ShowError(err);
				                                          }
				                                          free(c);
				                                          for (int i = 0; i < m_numIndices; ++i)
					                                          m_mesh.m_indices[i] = tmp[i];
			                                          });
		}
		break;
	}
#endif
	case FID(PIDB):
		pBiffReader->GetFloat(m_d.m_depthBias);
		break;
	case FID(OSNM):
		pBiffReader->GetBool(m_d.m_objectSpaceNormalMap);
		break;
	default:
		ISelect::LoadToken(id, pBiffReader);
		break;
	}
	return true;
}

void Primitive::WriteRegDefaults()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	static const char strKeyName[] = "DefaultProps\\Primitive";

	pRegUtil->SaveValueInt(strKeyName, "SideColor", m_d.m_SideColor);
	pRegUtil->SaveValueBool(strKeyName, "Visible", m_d.m_visible);
	pRegUtil->SaveValueBool(strKeyName, "StaticRendering", m_d.m_staticRendering);
	pRegUtil->SaveValueBool(strKeyName, "DrawTexturesInside", m_d.m_drawTexturesInside);

	pRegUtil->SaveValueFloat(strKeyName, "Position_Z", m_d.m_vPosition.z);

	pRegUtil->SaveValueFloat(strKeyName, "Size_X", m_d.m_vSize.x);
	pRegUtil->SaveValueFloat(strKeyName, "Size_Y", m_d.m_vSize.y);
	pRegUtil->SaveValueFloat(strKeyName, "Size_Z", m_d.m_vSize.z);

	pRegUtil->SaveValueFloat(strKeyName, "RotAndTra0", m_d.m_aRotAndTra[0]);
	pRegUtil->SaveValueFloat(strKeyName, "RotAndTra1", m_d.m_aRotAndTra[1]);
	pRegUtil->SaveValueFloat(strKeyName, "RotAndTra2", m_d.m_aRotAndTra[2]);
	pRegUtil->SaveValueFloat(strKeyName, "RotAndTra3", m_d.m_aRotAndTra[3]);
	pRegUtil->SaveValueFloat(strKeyName, "RotAndTra4", m_d.m_aRotAndTra[4]);
	pRegUtil->SaveValueFloat(strKeyName, "RotAndTra5", m_d.m_aRotAndTra[5]);
	pRegUtil->SaveValueFloat(strKeyName, "RotAndTra6", m_d.m_aRotAndTra[6]);
	pRegUtil->SaveValueFloat(strKeyName, "RotAndTra7", m_d.m_aRotAndTra[7]);
	pRegUtil->SaveValueFloat(strKeyName, "RotAndTra8", m_d.m_aRotAndTra[8]);

	pRegUtil->SaveValue(strKeyName, "Image", m_d.m_szImage);
	pRegUtil->SaveValue(strKeyName, "NormalMap", m_d.m_szNormalMap);
	pRegUtil->SaveValueBool(strKeyName, "HitEvent", m_d.m_hitEvent);
	pRegUtil->SaveValueFloat(strKeyName, "HitThreshold", m_d.m_threshold);
	pRegUtil->SaveValueFloat(strKeyName, "Elasticity", m_d.m_elasticity);
	pRegUtil->SaveValueFloat(strKeyName, "ElasticityFalloff", m_d.m_elasticityFalloff);
	pRegUtil->SaveValueFloat(strKeyName, "Friction", m_d.m_friction);
	pRegUtil->SaveValueFloat(strKeyName, "Scatter", m_d.m_scatter);

	pRegUtil->SaveValueFloat(strKeyName, "EdgeFactorUI", m_d.m_edgeFactorUI);
	pRegUtil->SaveValueFloat(strKeyName, "CollisionReductionFactor", m_d.m_collision_reductionFactor);

	pRegUtil->SaveValueBool(strKeyName, "Collidable", m_d.m_collidable);
	pRegUtil->SaveValueBool(strKeyName, "IsToy", m_d.m_toy);
	const int tmp = quantizeUnsigned<8>(clamp(m_d.m_disableLightingTop, 0.f, 1.f));
	pRegUtil->SaveValueInt(strKeyName, "DisableLighting", (tmp == 1) ? 0 : tmp); // backwards compatible saving
	pRegUtil->SaveValueFloat(strKeyName, "DisableLightingBelow", m_d.m_disableLightingBelow);
	pRegUtil->SaveValueBool(strKeyName, "ReflectionEnabled", m_d.m_reflectionEnabled);
	pRegUtil->SaveValueBool(strKeyName, "BackfacesEnabled", m_d.m_backfacesEnabled);
	pRegUtil->SaveValueBool(strKeyName, "DisplayTexture", m_d.m_displayTexture);
	pRegUtil->SaveValueBool(strKeyName, "ObjectSpaceNormalMap", m_d.m_objectSpaceNormalMap);
}
