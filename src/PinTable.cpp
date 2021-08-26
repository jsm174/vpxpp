#include "PinTable.h"
#include "BiffReader.h"

#include <float.h>
#include <iostream>
#include <stdio.h>

#include "EditableRegistry.h"
#include "ISelect.h"

PinTable::PinTable()
{
	m_renderDecals = true;
	m_renderEMReels = true;

	m_overridePhysics = 0;
	m_overridePhysicsFlipper = false;

	m_defaultScatter = 0.f;
	m_nudgeTime = 5.0f;

	m_plungerNormalize = 100;
	m_plungerFilter = false;
	m_PhysicsMaxLoops = 0xFFFFFFFFu; // TODO: LoadValueIntWithDefault("Player", "PhysicsMaxLoops", 0xFFFFFFFFu);

	m_right = 0.0f;
	m_bottom = 0.0f;

	m_glassheight = 210;
	m_tableheight = 0;

	m_BG_current_set = BG_DESKTOP;
	m_currentBackglassMode = m_BG_current_set;

	m_BG_enable_FSS = false;

	for (int i = 0; i < NUM_BG_SETS; ++i)
	{
		m_BG_inclination[i] = 0;
		m_BG_FOV[i] = FLT_MAX;

		m_BG_rotation[i] = 0;
		m_BG_layback[i] = 0;

		m_BG_scalex[i] = 1.0f;
		m_BG_scaley[i] = 1.0f;

		m_BG_xlatex[i] = 0.0f;
		m_BG_xlatey[i] = 0.0f;

		m_BG_scalez[i] = 1.0f;
		m_BG_xlatez[i] = 0.0f;
	}

	m_pCodeViewer = new CodeViewer();

	m_3DZPD = 0.5f;
	m_3DmaxSeparation = 0.03f;
	m_3DOffset = 0.0f;
	m_overwriteGlobalStereo3D = false;
}

PinTable::~PinTable()
{
}

void PinTable::LoadGameFromFilename(const char* pFilename)
{
	POLE::Storage* pStorage = new POLE::Storage(pFilename);
	pStorage->open();

	if (pStorage->result() != POLE::Storage::Ok)
	{
	}

	LoadGameFromStorage(pStorage);

	delete pStorage;

	return;
}

HRESULT PinTable::LoadGameFromStorage(POLE::Storage* pStorage)
{
	int loadFileVersion = CURRENT_FILE_FORMAT_VERSION;

	POLE::Stream* pGameStream = new POLE::Stream(pStorage, "GameStg/GameData");

	HRESULT hr = S_OK;

	if (!pGameStream->fail())
	{
		POLE::Stream* pstmVersion = new POLE::Stream(pStorage, "GameStg/Version");
		if (!pstmVersion->fail())
		{
			pstmVersion->read((unsigned char*)&loadFileVersion, sizeof(int));

			if (loadFileVersion > CURRENT_FILE_FORMAT_VERSION)
			{
				printf("This table was saved with version %i.%02i and is newer than the supported version %i.%02i! You might get problems loading/playing it!", loadFileVersion / 100, loadFileVersion % 100, CURRENT_FILE_FORMAT_VERSION / 100, CURRENT_FILE_FORMAT_VERSION % 100);
			}
		}

		delete pstmVersion;

		LoadInfo(pStorage, loadFileVersion);
		LoadCustomInfo(pStorage, loadFileVersion);

		int csubobj = 0;
		int csounds = 0;
		int ctextures = 0;
		int cfonts = 0;
		int ccollection = 0;

		if (LoadData(pGameStream, csubobj, csounds, ctextures, cfonts, ccollection, loadFileVersion) == S_OK)
		{
			const int ctotalitems = csubobj + csounds + ctextures + cfonts;

			int cloadeditems = 0;

			for (int i = 0; i < csubobj; i++)
			{
				std::string szStmName = "GameStg/GameItem" + std::to_string(i);

				POLE::Stream* pItemStream = new POLE::Stream(pStorage, szStmName);

				if (!pItemStream->fail())
				{
					ItemTypeEnum type;
					pItemStream->read((unsigned char*)&type, sizeof(int));

					IEditable* pEditable = EditableRegistry::Create(type);

					std::cout << szStmName << " = " << type << "\n";

					int id = 0; // VBA id for this item
					hr = pEditable->InitLoad(pItemStream, this, &id, loadFileVersion);
					pEditable->InitVBA(false, id, NULL);

					delete pItemStream;
					pItemStream = NULL;

					if (hr != S_OK)
					{
						break;
					}

					m_vedit.push_back(pEditable);
				}
				else
				{
					delete pItemStream;
				}
			}
		}
	}

	delete pGameStream;

	return hr;
}

HRESULT PinTable::LoadInfo(POLE::Storage* pStorage, int version)
{
	char* pValue = nullptr;
	ReadInfoValue(pStorage, "TableName", &pValue);
	if (pValue != nullptr)
	{
		m_szTableName = pValue;
		delete[] pValue;
	}

	pValue = nullptr;
	ReadInfoValue(pStorage, "AuthorName", &pValue);
	if (pValue != nullptr)
	{
		m_szAuthor = pValue;
		delete[] pValue;
	}

	pValue = nullptr;
	ReadInfoValue(pStorage, "TableVersion", &pValue);
	if (pValue != nullptr)
	{
		m_szVersion = pValue;
		delete[] pValue;
	}

	pValue = nullptr;
	ReadInfoValue(pStorage, "ReleaseDate", &pValue);
	if (pValue != nullptr)
	{
		m_szReleaseDate = pValue;
		delete[] pValue;
	}

	pValue = nullptr;
	ReadInfoValue(pStorage, "AuthorEmail", &pValue);
	if (pValue != nullptr)
	{
		m_szAuthorEMail = pValue;
		delete[] pValue;
	}

	pValue = nullptr;
	ReadInfoValue(pStorage, "AuthorWebSite", &pValue);
	if (pValue != nullptr)
	{
		m_szWebSite = pValue;
		delete[] pValue;
	}

	pValue = nullptr;
	ReadInfoValue(pStorage, "TableBlurb", &pValue);
	if (pValue != nullptr)
	{
		m_szBlurb = pValue;
		delete[] pValue;
	}

	pValue = nullptr;
	ReadInfoValue(pStorage, "TableDescription", &pValue);
	if (pValue != nullptr)
	{
		m_szDescription = pValue;
		delete[] pValue;
	}

	pValue = nullptr;
	ReadInfoValue(pStorage, "TableRules", &pValue);
	if (pValue != nullptr)
	{
		m_szRules = pValue;
		delete[] pValue;
	}

	pValue = nullptr;
	ReadInfoValue(pStorage, "TableSaveDate", &pValue);
	if (pValue != nullptr)
	{
		m_szDateSaved = pValue;
		delete[] pValue;
	}

	pValue = nullptr;
	ReadInfoValue(pStorage, "TableSaveRev", &pValue);
	if (pValue != nullptr)
	{
		m_numTimesSaved = atoi(pValue);
		delete[] pValue;
	}
	else
	{
		m_numTimesSaved = 0;
	}

	// TODO: LOAD SCREENSHOT

	return S_OK;
}

HRESULT PinTable::LoadCustomInfo(POLE::Storage* pStorage, int version)
{
	POLE::Stream* pStream = new POLE::Stream(pStorage, "GameStg/CustomInfoTags");

	BiffReader biffReader(pStream, this, NULL, version);
	biffReader.Load();

	for (size_t i = 0; i < m_vCustomInfoTag.size(); i++)
	{
		char* pValue;
		ReadInfoValue(pStorage, m_vCustomInfoTag[i].c_str(), &pValue);
		m_vCustomInfoContent.push_back(pValue);
		delete[] pValue;
	}

	delete pStream;

	return S_OK;
}

HRESULT PinTable::LoadData(POLE::Stream* pStream, int& csubobj, int& csounds, int& ctextures, int& cfonts, int& ccollection, int version)
{
	// TODO: SetLoadDefaults();

	int rgi[6] = {0, 0, 0, 0, 0, 0};

	BiffReader biffReader(pStream, this, rgi, version);
	biffReader.Load();

	csubobj = rgi[1];
	csounds = rgi[2];
	ctextures = rgi[3];
	cfonts = rgi[4];
	ccollection = rgi[5];

	return S_OK;
}

bool PinTable::LoadToken(const int id, BiffReader* pBiffReader)
{
	//FILE* log = fopen("fid.txt","a");
	//printf("FID: %c%c%c%c \n", (id & 0xFF), ((id >> 8) & 0xFF), ((id >> 16 & 0xFF)), ((id >> 24 & 0xFF)));
	//fclose(log);

	switch (id)
	{
	case FID(LEFT):
		pBiffReader->GetFloat(m_left);
		break;
	case FID(TOPX):
		pBiffReader->GetFloat(m_top);
		break;
	case FID(RGHT):
		pBiffReader->GetFloat(m_right);
		break;
	case FID(BOTM):
		pBiffReader->GetFloat(m_bottom);
		break;
	case FID(ROTA):
		pBiffReader->GetFloat(m_BG_rotation[BG_DESKTOP]);
		break;
	case FID(LAYB):
		pBiffReader->GetFloat(m_BG_layback[BG_DESKTOP]);
		break;
	case FID(INCL):
		pBiffReader->GetFloat(m_BG_inclination[BG_DESKTOP]);
		break;
	case FID(FOVX):
		pBiffReader->GetFloat(m_BG_FOV[BG_DESKTOP]);
		break;
	case FID(SCLX):
		pBiffReader->GetFloat(m_BG_scalex[BG_DESKTOP]);
		break;
	case FID(SCLY):
		pBiffReader->GetFloat(m_BG_scaley[BG_DESKTOP]);
		break;
	case FID(SCLZ):
		pBiffReader->GetFloat(m_BG_scalez[BG_DESKTOP]);
		break;
	case FID(XLTX):
		pBiffReader->GetFloat(m_BG_xlatex[BG_DESKTOP]);
		break;
	case FID(XLTY):
		pBiffReader->GetFloat(m_BG_xlatey[BG_DESKTOP]);
		break;
	case FID(XLTZ):
		pBiffReader->GetFloat(m_BG_xlatez[BG_DESKTOP]);
		break;
	case FID(ROTF):
		pBiffReader->GetFloat(m_BG_rotation[BG_FULLSCREEN]);
		break;
	case FID(LAYF):
		pBiffReader->GetFloat(m_BG_layback[BG_FULLSCREEN]);
		break;
	case FID(INCF):
		pBiffReader->GetFloat(m_BG_inclination[BG_FULLSCREEN]);
		break;
	case FID(FOVF):
		pBiffReader->GetFloat(m_BG_FOV[BG_FULLSCREEN]);
		break;
	case FID(SCFX):
		pBiffReader->GetFloat(m_BG_scalex[BG_FULLSCREEN]);
		break;
	case FID(SCFY):
		pBiffReader->GetFloat(m_BG_scaley[BG_FULLSCREEN]);
		break;
	case FID(SCFZ):
		pBiffReader->GetFloat(m_BG_scalez[BG_FULLSCREEN]);
		break;
	case FID(XLFX):
		pBiffReader->GetFloat(m_BG_xlatex[BG_FULLSCREEN]);
		break;
	case FID(XLFY):
		pBiffReader->GetFloat(m_BG_xlatey[BG_FULLSCREEN]);
		break;
	case FID(XLFZ):
		pBiffReader->GetFloat(m_BG_xlatez[BG_FULLSCREEN]);
		break;
	case FID(ROFS):
		pBiffReader->GetFloat(m_BG_rotation[BG_FSS]);
		break;
	case FID(LAFS):
		pBiffReader->GetFloat(m_BG_layback[BG_FSS]);
		break;
	case FID(INFS):
		pBiffReader->GetFloat(m_BG_inclination[BG_FSS]);
		break;
	case FID(FOFS):
		pBiffReader->GetFloat(m_BG_FOV[BG_FSS]);
		break;
	case FID(SCXS):
		pBiffReader->GetFloat(m_BG_scalex[BG_FSS]);
		break;
	case FID(SCYS):
		pBiffReader->GetFloat(m_BG_scaley[BG_FSS]);
		break;
	case FID(SCZS):
		pBiffReader->GetFloat(m_BG_scalez[BG_FSS]);
		break;
	case FID(XLXS):
		pBiffReader->GetFloat(m_BG_xlatex[BG_FSS]);
		break;
	case FID(XLYS):
		pBiffReader->GetFloat(m_BG_xlatey[BG_FSS]);
		break;
	case FID(XLZS):
		pBiffReader->GetFloat(m_BG_xlatez[BG_FSS]);
		break;
	case FID(EFSS):
	{
		pBiffReader->GetBool(m_BG_enable_FSS);
		if (m_BG_enable_FSS)
		{
			m_BG_current_set = BG_FSS;
		}
		break;
	}
	case FID(ORRP):
		pBiffReader->GetInt(m_overridePhysics);
		break;
	case FID(ORPF):
		pBiffReader->GetBool(m_overridePhysicsFlipper);
		break;
	case FID(GAVT):
		pBiffReader->GetFloat(m_Gravity);
		break;
	case FID(FRCT):
		pBiffReader->GetFloat(m_friction);
		break;
	case FID(ELAS):
		pBiffReader->GetFloat(m_elasticity);
		break;
	case FID(ELFA):
		pBiffReader->GetFloat(m_elasticityFalloff);
		break;
	case FID(PFSC):
		pBiffReader->GetFloat(m_scatter);
		break;
	case FID(SCAT):
		pBiffReader->GetFloat(m_defaultScatter);
		break;
	case FID(NDGT):
		pBiffReader->GetFloat(m_nudgeTime);
		break;
	case FID(MPGC):
	{
		int tmp;
		pBiffReader->GetInt(tmp);
		// TODO: m_plungerNormalize = LoadValueIntWithDefault("Player", "PlungerNormalize", tmp);
		break;
	}
	case FID(MPDF):
	{
		bool tmp;
		pBiffReader->GetBool(tmp);
		// TODO: m_plungerFilter = LoadValueBoolWithDefault("Player", "PlungerFilter", tmp);
		break;
	}
	case FID(PHML):
	{
		pBiffReader->GetInt((int&)m_PhysicsMaxLoops);
		if (m_PhysicsMaxLoops == 0xFFFFFFFF)
		{
			// TODO: LoadValue("Player", "PhysicsMaxLoops", m_PhysicsMaxLoops);
		}
		break;
	}
	case FID(DECL):
		pBiffReader->GetBool(m_renderDecals);
		break;
	case FID(REEL):
		pBiffReader->GetBool(m_renderEMReels);
		break;
	case FID(OFFX):
		pBiffReader->GetFloat(m_offset.x);
		break;
	case FID(OFFY):
		pBiffReader->GetFloat(m_offset.y);
		break;
	case FID(ZOOM):
		pBiffReader->GetFloat(m_zoom);
		break;
	case FID(MAXSEP):
		pBiffReader->GetFloat(m_3DmaxSeparation);
		break;
	case FID(ZPD):
		pBiffReader->GetFloat(m_3DZPD);
		break;
	case FID(STO):
		pBiffReader->GetFloat(m_3DOffset);
		break;
	case FID(OGST):
		pBiffReader->GetBool(m_overwriteGlobalStereo3D);
		break;
	case FID(SLPX):
		pBiffReader->GetFloat(m_angletiltMax);
		break;
	case FID(SLOP):
		pBiffReader->GetFloat(m_angletiltMin);
		break;
	case FID(GLAS):
		pBiffReader->GetFloat(m_glassheight);
		break;
	case FID(TBLH):
		pBiffReader->GetFloat(m_tableheight);
		break;
	case FID(IMAG):
		pBiffReader->GetString(m_image);
		break;
	case FID(BLIM):
		pBiffReader->GetString(m_ballImage);
		break;
	case FID(BLIF):
		pBiffReader->GetString(m_ballImageDecal);
		break;
	case FID(SSHT):
		pBiffReader->GetString(m_szScreenShot);
		break;
	case FID(FBCK):
		pBiffReader->GetBool(m_backdrop);
		break;
	case FID(SEDT):
		pBiffReader->GetInt(&((int*)pBiffReader->m_pData)[1]);
		break;
	case FID(SSND):
		pBiffReader->GetInt(&((int*)pBiffReader->m_pData)[2]);
		break;
	case FID(SIMG):
		pBiffReader->GetInt(&((int*)pBiffReader->m_pData)[3]);
		break;
	case FID(SFNT):
		pBiffReader->GetInt(&((int*)pBiffReader->m_pData)[4]);
		break;
	case FID(SCOL):
		pBiffReader->GetInt(&((int*)pBiffReader->m_pData)[5]);
		break;
	case FID(NAME):
		pBiffReader->GetWideString(m_wzName, sizeof(m_wzName) / sizeof(wchar_t));
		break;
	case FID(BIMG):
		pBiffReader->GetString(m_BG_image[0]);
		break;
	case FID(BIMF):
		pBiffReader->GetString(m_BG_image[1]);
		break;
	case FID(BIMS):
		pBiffReader->GetString(m_BG_image[2]);
		break;
	case FID(BIMN):
		pBiffReader->GetBool(m_imageBackdropNightDay);
		break;
	case FID(IMCG):
		pBiffReader->GetString(m_imageColorGrade);
		break;
	case FID(EIMG):
		pBiffReader->GetString(m_envImage);
		break;
	case FID(PLMA):
		pBiffReader->GetString(m_playfieldMaterial);
		break;
	case FID(NOTX):
		pBiffReader->GetString(m_notesText);
		break;
	case FID(LZAM):
		pBiffReader->GetInt(m_lightAmbient);
		break;
	case FID(LZDI):
		pBiffReader->GetInt(m_Light[0].emission);
		break;
	case FID(LZHI):
		pBiffReader->GetFloat(m_lightHeight);
		break;
	case FID(LZRA):
		pBiffReader->GetFloat(m_lightRange);
		break;
	case FID(LIES):
		pBiffReader->GetFloat(m_lightEmissionScale);
		break;
	case FID(ENES):
		pBiffReader->GetFloat(m_envEmissionScale);
		break;
	case FID(GLES):
		pBiffReader->GetFloat(m_globalEmissionScale);
		break;
	case FID(AOSC):
		pBiffReader->GetFloat(m_AOScale);
		break;
	case FID(SSSC):
		pBiffReader->GetFloat(m_SSRScale);
		break;
	case FID(BREF):
		pBiffReader->GetInt(m_useReflectionForBalls);
		break;
	case FID(PLST):
	{
		int tmp;
		pBiffReader->GetInt(tmp);
		m_playfieldReflectionStrength = dequantizeUnsigned<8>(tmp);
		break;
	}
	case FID(BTRA):
		pBiffReader->GetInt(m_useTrailForBalls);
		break;
	case FID(BTST):
	{
		int tmp;
		pBiffReader->GetInt(tmp);
		m_ballTrailStrength = dequantizeUnsigned<8>(tmp);
		break;
	}
	case FID(BPRS):
		pBiffReader->GetFloat(m_ballPlayfieldReflectionStrength);
		break;
	case FID(DBIS):
		pBiffReader->GetFloat(m_defaultBulbIntensityScaleOnBall);
		break;
	case FID(UAAL):
		pBiffReader->GetInt(m_useAA);
		break;
	case FID(UAOC):
		pBiffReader->GetInt(m_useAO);
		break;
	case FID(USSR):
		pBiffReader->GetInt(m_useSSR);
		break;
	case FID(UFXA):
		pBiffReader->GetInt(m_useFXAA);
		break;
	case FID(BLST):
		pBiffReader->GetFloat(m_bloom_strength);
		break;
	case FID(BCLR):
		pBiffReader->GetInt(m_colorbackdrop);
		break;
	//TODO: case FID(SECB): pBiffReader->GetStruct(&m_protectionData, sizeof(ProtectionData)); break;
	case FID(CODE):
	{
		// if the script is protected then we pass in the proper cryptokey into the code loadstream
		// TODO: const bool script_protected = (((m_protectionData.flags & DISABLE_EVERYTHING) == DISABLE_EVERYTHING) ||
		//    ((m_protectionData.flags & DISABLE_SCRIPT_EDITING) == DISABLE_SCRIPT_EDITING));

		m_pCodeViewer->LoadFromStream(pBiffReader->m_pStream);
		break;
	}
	case FID(CCUS):
		pBiffReader->GetStruct(m_rgcolorcustom, sizeof(COLORREF) * 16);
		break;
	case FID(TDFT):
	{
		pBiffReader->GetFloat(m_globalDifficulty);
		int tmp;
		//const HRESULT hr = LoadValue("Player", "GlobalDifficulty", tmp);
		//if (hr == S_OK) m_globalDifficulty = dequantizeUnsignedPercent(tmp);
		break;
	}
	case FID(CUST):
	{
		std::string tmp;
		pBiffReader->GetString(tmp);
		m_vCustomInfoTag.push_back(tmp);
		break;
	}
	case FID(SVOL):
		pBiffReader->GetFloat(m_TableSoundVolume);
		break;
	case FID(BDMO):
		pBiffReader->GetBool(m_BallDecalMode);
		break;
	case FID(MVOL):
		pBiffReader->GetFloat(m_TableMusicVolume);
		break;
	case FID(AVSY):
		pBiffReader->GetInt(m_TableAdaptiveVSync);
		break;
	case FID(OGAC):
		pBiffReader->GetBool(m_overwriteGlobalDetailLevel);
		break;
	case FID(OGDN):
		pBiffReader->GetBool(m_overwriteGlobalDayNight);
		break;
	case FID(GDAC):
		pBiffReader->GetBool(m_grid);
		break;
	case FID(REOP):
		pBiffReader->GetBool(m_reflectElementsOnPlayfield);
		break;
	case FID(ARAC):
		pBiffReader->GetInt(m_userDetailLevel);
		break;
	case FID(MASI):
		pBiffReader->GetInt(m_numMaterials);
		break;
	case FID(MATE):
	{
		std::vector<SaveMaterial> mats(m_numMaterials);
		pBiffReader->GetStruct(mats.data(), (int)sizeof(SaveMaterial) * m_numMaterials);

		for (size_t i = 0; i < m_materials.size(); ++i)
		{
			delete m_materials[i];
		}

		m_materials.clear();

		for (int i = 0; i < m_numMaterials; i++)
		{
			Material* pMaterial = new Material();
			pMaterial->m_cBase = mats[i].cBase;
			pMaterial->m_cGlossy = mats[i].cGlossy;
			pMaterial->m_cClearcoat = mats[i].cClearcoat;
			pMaterial->m_fWrapLighting = mats[i].fWrapLighting;
			pMaterial->m_fRoughness = mats[i].fRoughness;
			pMaterial->m_fGlossyImageLerp = 1.0f - dequantizeUnsigned<8>(mats[i].fGlossyImageLerp);                  //!! '1.0f -' to be compatible with previous table versions
			pMaterial->m_fThickness = (mats[i].fThickness == 0) ? 0.05f : dequantizeUnsigned<8>(mats[i].fThickness); //!! 0 -> 0.05f to be compatible with previous table versions
			pMaterial->m_fEdge = mats[i].fEdge;
			pMaterial->m_fOpacity = mats[i].fOpacity;
			pMaterial->m_bIsMetal = mats[i].bIsMetal;
			pMaterial->m_bOpacityActive = !!(mats[i].bOpacityActive_fEdgeAlpha & 1);
			pMaterial->m_fEdgeAlpha = dequantizeUnsigned<7>(mats[i].bOpacityActive_fEdgeAlpha >> 1);
			pMaterial->m_szName = mats[i].szName;

			m_materials.push_back(pMaterial);
		}
		break;
	}
	case FID(PHMA):
	{
		std::vector<SavePhysicsMaterial> mats(m_numMaterials);
		pBiffReader->GetStruct(mats.data(), (int)sizeof(SavePhysicsMaterial) * m_numMaterials);

		for (int i = 0; i < m_numMaterials; i++)
		{
			bool found = true;
			Material* pmat = GetMaterial(mats[i].szName);
			/*TODO: if (pmat == &m_vpinball->m_dummyMaterial)
				{
					assert(!"SaveMaterial not found");
					pmat = new Material();
					pmat->m_szName = mats[i].szName;
					found = false;
				}*/
			pmat->m_fElasticity = mats[i].fElasticity;
			pmat->m_fElasticityFalloff = mats[i].fElasticityFallOff;
			pmat->m_fFriction = mats[i].fFriction;
			pmat->m_fScatterAngle = mats[i].fScatterAngle;
			if (!found)
				m_materials.push_back(pmat);
		}
		break;
	}
	}

	return true;
}

void PinTable::ReadInfoValue(POLE::Storage* pStorage, const char* pName, char** ppValue)
{
	char pPath[255];
	snprintf(pPath, 255, "TableInfo/%s", pName);

	POLE::Stream* pStream = new POLE::Stream(pStorage, pPath);

	int len = pStream->size() / 2;

	wchar_t* pWString = new wchar_t[len + 1];
	memset(pWString, 0, sizeof(wchar_t) * (len + 1));

	BiffReader biffReader(pStream, NULL, NULL, 0);

	char* pPtr = (char*)pWString;

	for (int index = 0; index < len; index++)
	{
		biffReader.ReadBytes(pPtr, 2);
		pPtr += sizeof(wchar_t);
	}

	len = wcstombs(nullptr, pWString, 0);

	*ppValue = (char*)malloc(len + 1);
	memset(*ppValue, 0, len + 1);

	wcstombs(*ppValue, pWString, len);

	std::cout << pPath << " = " << *ppValue << "\n";

	delete pStream;
}

Material* PinTable::GetMaterial(const std::string& szName)
{
	// TODO: if (szName.empty())
	//   return &m_vpinball->m_dummyMaterial;

	// during playback, we use the hashtable for lookup
	if (!m_materialMap.empty())
	{
		std::unordered_map<const char*, Material*, StringHashFunctor, StringComparator>::const_iterator
		    it = m_materialMap.find(szName.c_str());
		if (it != m_materialMap.end())
			return it->second;
		// TODO: else
		//   return &m_vpinball->m_dummyMaterial;
	}

	for (size_t i = 0; i < m_materials.size(); i++)
		if (m_materials[i]->m_szName == szName)
			return m_materials[i];

	// TODO: return &m_vpinball->m_dummyMaterial;
}

void PinTable::visit(int indent, POLE::Storage* storage, std::string path)
{
	std::list<std::string> entries;
	entries = storage->entries(path);

	std::list<std::string>::iterator it;
	for (it = entries.begin(); it != entries.end(); ++it)
	{
		std::string name = *it;
		std::string fullname = path + name;
		for (int j = 0; j < indent; j++)
			std::cout << "    ";
		POLE::Stream* ss = new POLE::Stream(storage, fullname);
		std::cout << name;
		if (ss)
			if (!ss->fail())
				std::cout << "  (" << ss->size() << ")";
		std::cout << std::endl;
		delete ss;

		if (storage->isDirectory(fullname))
			visit(indent + 1, storage, fullname + "/");
	}
}

HRESULT PinTable::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	return S_OK;
}

HRESULT PinTable::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	return S_OK;
}

void PinTable::GetUniqueName(const ItemTypeEnum type, wchar_t* const wzUniqueName, const unsigned int wzUniqueName_maxlength) const
{
	wchar_t wzRoot[256];
	GetTypeNameForType(type, wzRoot);
	GetUniqueName(wzRoot, wzUniqueName, wzUniqueName_maxlength);
}

void PinTable::GetUniqueName(const wchar_t* const wzRoot, wchar_t* const wzUniqueName, const unsigned int wzUniqueName_maxlength) const
{
	/*int suffix = 1;
   bool found = false;
   wchar_t * const wzName = new wchar_t[wzUniqueName_maxlength];
   wchar_t wzSuffix[4];

   while (!found)
   {
      WideStrNCopy(wzRoot, wzName, wzUniqueName_maxlength-3);
      _itow_s(suffix, wzSuffix, sizeof(wzSuffix)/sizeof(wzSuffix[0]), 10);
      if(suffix < 10)
         WideStrCat(L"0", wzName, wzUniqueName_maxlength);
      if(suffix < 100)
         WideStrCat(L"0", wzName, wzUniqueName_maxlength);
      WideStrCat(wzSuffix, wzName, wzUniqueName_maxlength);

      if (IsNameUnique(wzName) || suffix == 999)
         found = true;
      else
         suffix++;
   }

   WideStrNCopy(wzName, wzUniqueName, wzUniqueName_maxlength);
   delete[] wzName;*/
}