#include "PinTable.h"
#include "Collection.h"
#include "EditableRegistry.h"
#include "Player.h"
#include "Ramp.h"
#include "RegUtil.h"
#include "Surface.h"
#include "extern.h"

#include "Inlines.h"

#include <assert.h>
#include <iostream>

PinTable::PinTable(VPinball* pVPinball)
{
	m_vpinball = pVPinball;

	m_renderDecals = true;
	m_renderEMReels = true;

	m_reflectionEnabled = false;

	m_overridePhysics = 0;
	m_overridePhysicsFlipper = false;

	m_defaultScatter = 0.f;
	m_nudgeTime = 5.0f;

	m_plungerNormalize = 100;
	m_plungerFilter = false;

	RegUtil* pRegUtil = RegUtil::SharedInstance();
	m_PhysicsMaxLoops = pRegUtil->LoadValueIntWithDefault("Player", "PhysicsMaxLoops", 0xFFFFFFFFu);

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

	m_globalEmissionScale = 1.0f;

	m_pbTempScreenshot = NULL;

	m_tblMirrorEnabled = false;

	m_numMaterials = 0;

	m_3DZPD = 0.5f;
	m_3DmaxSeparation = 0.03f;
	m_3DOffset = 0.0f;
	m_overwriteGlobalStereo3D = false;
}

PinTable::~PinTable()
{
}

HRESULT PinTable::LoadGameFromFilename(const std::string& szFilename)
{
	HRESULT hr = E_FAIL;

	if (szFilename.empty())
	{
		// TODO: ShowError("Empty File Name String!");
	}
	else
	{
		m_szFilename = szFilename;

		POLE::Storage* pStorage = new POLE::Storage(szFilename.c_str());

		if (pStorage->open())
		{
			visit(0, pStorage, "/");

			hr = LoadGameFromStorage(pStorage);
		}

		delete pStorage;
	}

	return hr;
}

HRESULT PinTable::LoadGameFromStorage(POLE::Storage* pStorage)
{
	int loadFileVersion = CURRENT_FILE_FORMAT_VERSION;

	HRESULT hr;

	if (pStorage->exists("GameStg"))
	{
		if (pStorage->exists("GameStg/GameData"))
		{
			POLE::Stream* pGameDataStream = new POLE::Stream(pStorage, "GameStg/GameData");

			if (pStorage->exists("GameStg/Version"))
			{
				POLE::Stream* pVersionStream = new POLE::Stream(pStorage, "GameStg/Version");

				pVersionStream->read((unsigned char*)&loadFileVersion, sizeof(int));

				if (loadFileVersion > CURRENT_FILE_FORMAT_VERSION)
				{
					printf("This table was saved with version %i.%02i and is newer than the supported version %i.%02i! You might get problems loading/playing it!", loadFileVersion / 100, loadFileVersion % 100, CURRENT_FILE_FORMAT_VERSION / 100, CURRENT_FILE_FORMAT_VERSION % 100);
				}

				delete pVersionStream;

				if (pStorage->exists("TableInfo"))
				{
					hr = LoadInfo(pStorage, loadFileVersion);

					if (pStorage->exists("GameStg/CustomInfoTags"))
					{
						hr = LoadCustomInfo(pStorage, loadFileVersion);
					}
					else
					{
						hr = E_FAIL;
					}
				}
				else
				{
					hr = E_FAIL;
				}

				int csubobj = 0;
				int csounds = 0;
				int ctextures = 0;
				int cfonts = 0;
				int ccollection = 0;

				hr = LoadData(pGameDataStream, csubobj, csounds, ctextures, cfonts, ccollection, loadFileVersion);

				if (hr == S_OK)
				{
					const int ctotalitems = csubobj + csounds + ctextures + cfonts;

					int cloadeditems = 0;

					for (int i = 0; i < csubobj; i++)
					{
						std::string szStmName = "GameStg/GameItem" + std::to_string(i);

						if (pStorage->exists(szStmName))
						{
							POLE::Stream* pGameItemStream = new POLE::Stream(pStorage, szStmName);

							ItemTypeEnum type;
							pGameItemStream->read((unsigned char*)&type, sizeof(int));

							IEditable* const pEditable = EditableRegistry::Create(type);

							int id = 0;
							hr = pEditable->InitLoad(pGameItemStream, this, &id, loadFileVersion);
							pEditable->InitVBA(false, id, NULL);

							delete pGameItemStream;
							pGameItemStream = NULL;

							if (hr != S_OK)
							{
								break;
							}

							m_vedit.push_back(pEditable);

							std::cout << szStmName << " = " << type << " " << ITEMTYPEENUM_STRING.at(type) << std::endl;
						}
						else
						{
							hr = E_FAIL;
						}

						cloadeditems++;

						// TODO: ::SendMessage(hwndProgressBar, PBM_SETPOS, cloadeditems, 0);
					}

					for (int i = 0; i < csounds; i++)
					{
						std::string szStmName = "GameStg/Sound" + std::to_string(i);

						if (pStorage->exists(szStmName))
						{
							POLE::Stream* pSoundStream = new POLE::Stream(pStorage, szStmName);

							LoadSoundFromStream(pSoundStream, loadFileVersion);

							delete pSoundStream;
							pSoundStream = NULL;

							std::cout << szStmName << std::endl;
						}
						else
						{
							hr = E_FAIL;
						}

						cloadeditems++;

						// TODO: ::SendMessage(hwndProgressBar, PBM_SETPOS, cloadeditems, 0);
					}

					assert(m_vimage.empty());
					m_vimage.resize(ctextures);

					{
						for (int i = 0; i < ctextures; i++)
						{
							std::string szStmName = "GameStg/Image" + std::to_string(i);

							if (pStorage->exists(szStmName))
							{
								POLE::Stream* pImageStream = new POLE::Stream(pStorage, szStmName);

								hr = LoadImageFromStream(pImageStream, i, loadFileVersion);

								delete pImageStream;
								pImageStream = NULL;

								if (hr != S_OK)
								{
									return hr;
								}

								std::cout << szStmName << std::endl;
							}
							else
							{
								hr = E_FAIL;
							}

							cloadeditems++;
						}
					}

					for (size_t i = 0; i < m_vimage.size(); ++i)
					{
						if (!m_vimage[i] || m_vimage[i]->m_pdsBuffer == NULL)
						{
							m_vimage.erase(m_vimage.begin() + i);
							--i;
						}
					}

					for (size_t i = 0; i < m_vimage.size() - 1; ++i)
					{
						for (size_t i2 = i + 1; i2 < m_vimage.size(); ++i2)
						{
							if (m_vimage[i]->m_szName == m_vimage[i2]->m_szName && m_vimage[i]->m_szPath == m_vimage[i2]->m_szPath)
							{
								m_vimage.erase(m_vimage.begin() + i2);
								--i2;
							}
						}
					}

					// TODO: ::SendMessage(hwndProgressBar, PBM_SETPOS, cloadeditems, 0);

					for (int i = 0; i < cfonts; i++)
					{
						std::string szStmName = "GameStg/Font" + std::to_string(i);

						if (pStorage->exists(szStmName))
						{
							POLE::Stream* pFontStream = new POLE::Stream(pStorage, szStmName);

							PinFont* const pPinFont = new PinFont();
							pPinFont->LoadFromStream(pFontStream, loadFileVersion);
							m_vfont.push_back(pPinFont);
							pPinFont->Register();

							delete pFontStream;
							pFontStream = NULL;

							std::cout << szStmName << std::endl;
						}
						else
						{
							hr = E_FAIL;
						}

						cloadeditems++;

						// TODO: ::SendMessage(hwndProgressBar, PBM_SETPOS, cloadeditems, 0);
					}

					for (int i = 0; i < ccollection; i++)
					{
						std::string szStmName = "GameStg/Collection" + std::to_string(i);

						if (pStorage->exists(szStmName))
						{
							POLE::Stream* pCollectionStream = new POLE::Stream(pStorage, szStmName);

							Collection* pCollection = new Collection();
							pCollection->LoadData(pCollectionStream, this, loadFileVersion);
							m_vcollection.push_back(pCollection);
							// m_pcv->AddItem((IScriptable*)pcol, false);

							delete pCollectionStream;
							pCollectionStream = NULL;

							std::cout << szStmName << std::endl;
						}
						else
						{
							hr = E_FAIL;
						}

						cloadeditems++;
						// TODO: ::SendMessage(hwndProgressBar, PBM_SETPOS, cloadeditems, 0);
					}

					for (size_t i = 0; i < m_vedit.size(); i++)
					{
						IEditable* const piedit = m_vedit[i];
						piedit->InitPostLoad();
					}
				}
			}
			else
			{
				hr = E_FAIL;
			}

			delete pGameDataStream;

			if (loadFileVersion < 1030)
			{
				for (size_t i = 0; i < m_materials.size(); ++i)
				{
					m_materials[i]->m_fGlossyImageLerp = 1.f;
				}
			}

			if (loadFileVersion < 1040)
			{
				for (size_t i = 0; i < m_materials.size(); ++i)
				{
					m_materials[i]->m_fThickness = 0.05f;
				}
			}
		}
		else
		{
			hr = E_FAIL;
		}
	}
	else
	{
		hr = E_FAIL;
	}

	if (m_pbTempScreenshot)
	{
		delete m_pbTempScreenshot;
		m_pbTempScreenshot = NULL;
	}

	return hr;
}

HRESULT PinTable::LoadInfo(POLE::Storage* pStorage, int version)
{
	char* pValue = NULL;
	ReadInfoValue(pStorage, "TableName", &pValue);
	if (pValue != NULL)
	{
		m_szTableName = pValue;
		delete[] pValue;
	}

	pValue = NULL;
	ReadInfoValue(pStorage, "AuthorName", &pValue);
	if (pValue != NULL)
	{
		m_szAuthor = pValue;
		delete[] pValue;
	}

	pValue = NULL;
	ReadInfoValue(pStorage, "TableVersion", &pValue);
	if (pValue != NULL)
	{
		m_szVersion = pValue;
		delete[] pValue;
	}

	pValue = NULL;
	ReadInfoValue(pStorage, "ReleaseDate", &pValue);
	if (pValue != NULL)
	{
		m_szReleaseDate = pValue;
		delete[] pValue;
	}

	pValue = NULL;
	ReadInfoValue(pStorage, "AuthorEmail", &pValue);
	if (pValue != NULL)
	{
		m_szAuthorEMail = pValue;
		delete[] pValue;
	}

	pValue = NULL;
	ReadInfoValue(pStorage, "AuthorWebSite", &pValue);
	if (pValue != NULL)
	{
		m_szWebSite = pValue;
		delete[] pValue;
	}

	pValue = NULL;
	ReadInfoValue(pStorage, "TableBlurb", &pValue);
	if (pValue != NULL)
	{
		m_szBlurb = pValue;
		delete[] pValue;
	}

	pValue = NULL;
	ReadInfoValue(pStorage, "TableDescription", &pValue);
	if (pValue != NULL)
	{
		m_szDescription = pValue;
		delete[] pValue;
	}

	pValue = NULL;
	ReadInfoValue(pStorage, "TableRules", &pValue);
	if (pValue != NULL)
	{
		m_szRules = pValue;
		delete[] pValue;
	}

	pValue = NULL;
	ReadInfoValue(pStorage, "TableSaveDate", &pValue);
	if (pValue != NULL)
	{
		m_szDateSaved = pValue;
		delete[] pValue;
	}

	pValue = NULL;
	ReadInfoValue(pStorage, "TableSaveRev", &pValue);
	if (pValue != NULL)
	{
		m_numTimesSaved = atoi(pValue);
		delete[] pValue;
	}
	else
	{
		m_numTimesSaved = 0;
	}

	RegUtil* pRegUtil = RegUtil::SharedInstance();
	pRegUtil->SaveValue("Version", m_szTableName, m_szVersion);

	if (pStorage->exists("TableInfo/Screenshot"))
	{
		POLE::Stream* pStream = new POLE::Stream(pStorage, "TableInfo/Screenshot");

		m_pbTempScreenshot = new PinBinary();
		m_pbTempScreenshot->m_cdata = pStream->size();
		m_pbTempScreenshot->m_pData = new char[m_pbTempScreenshot->m_cdata];

		BiffReader biffReader(pStream, this, NULL, version);
		biffReader.ReadBytes(m_pbTempScreenshot->m_pData, m_pbTempScreenshot->m_cdata);

		delete pStream;
	}

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

void PinTable::SetLoadDefaults()
{
	for (unsigned int i = 0; i < NUM_BG_SETS; ++i)
	{
		m_BG_image[i].clear();
	}

	m_imageColorGrade.clear();
	m_ballImage.clear();
	m_ballImageDecal.clear();
	m_imageBackdropNightDay = false;
	m_envImage.clear();

	m_szScreenShot.clear();

	m_colorbackdrop = RGB(0x62, 0x6E, 0x8E);

	m_lightAmbient = RGB((int)(0.1 * 255), (int)(0.1 * 255), (int)(0.1 * 255));

	for (unsigned int i = 0; i < MAX_LIGHT_SOURCES; ++i)
	{
		m_Light[i].emission = RGB((int)(0.4 * 255), (int)(0.4 * 255), (int)(0.4 * 255));
		m_Light[i].pos = Vertex3Ds(0.f, 0.f, 400.0f);
	}

	m_lightHeight = 1000.0f;
	m_lightRange = 3000.0f;
	m_lightEmissionScale = 1000000.0f;
	m_globalEmissionScale = 1.0f;
	m_envEmissionScale = 10.0f;
	m_AOScale = 1.75f;
	m_SSRScale = 0.5f;

	m_angletiltMax = 6.0f;
	m_angletiltMin = 4.5f;

	m_useReflectionForBalls = -1;
	m_playfieldReflectionStrength = 0.2f;
	m_reflectElementsOnPlayfield = false;

	m_useTrailForBalls = -1;
	m_ballTrailStrength = 0.4f;
	m_ballPlayfieldReflectionStrength = 1.f;

	m_useAA = -1;
	m_useFXAA = -1;
	m_useAO = -1;
	m_useSSR = -1;

	m_bloom_strength = 1.0f;

	m_TableSoundVolume = 1.0f;
	m_TableMusicVolume = 1.0f;

	m_BallDecalMode = false;

	m_TableAdaptiveVSync = -1;

	m_overridePhysicsFlipper = false;
}

HRESULT PinTable::LoadData(POLE::Stream* pStream, int& csubobj, int& csounds, int& ctextures, int& cfonts, int& ccollection, int version)
{
	SetLoadDefaults();

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

HRESULT PinTable::LoadSoundFromStream(POLE::Stream* pStream, const int loadFileVersion)
{
	int len;

	if (pStream->read((unsigned char*)&len, sizeof(int)) == 0)
	{
		return E_FAIL;
	}

	PinSound* const pPinSound = new PinSound();
	char* pTmp = new char[len + 1];

	if (pStream->read((unsigned char*)pTmp, len) == 0)
	{
		delete pPinSound;
		return E_FAIL;
	}

	pTmp[len] = 0;
	pPinSound->m_szName = pTmp;
	delete[] pTmp;

	if (pStream->read((unsigned char*)&len, sizeof(int)) == 0)
	{
		delete pPinSound;
		return E_FAIL;
	}

	pTmp = new char[len + 1];

	if (pStream->read((unsigned char*)pTmp, len) == 0)
	{
		delete pPinSound;
		return E_FAIL;
	}

	pTmp[len] = 0;
	pPinSound->m_szPath = pTmp;
	delete[] pTmp;

	if (pStream->read((unsigned char*)&len, sizeof(int)) == 0)
	{
		delete pPinSound;
		return E_FAIL;
	}

	pTmp = new char[len + 1];

	if (pStream->read((unsigned char*)pTmp, len) == 0)
	{
		delete pPinSound;
		return E_FAIL;
	}

	delete[] pTmp;

	if (pPinSound->IsWav2())
	{
		if (pStream->read((unsigned char*)(&pPinSound->m_wfx), sizeof(pPinSound->m_wfx)) == 0)
		{
			delete pPinSound;
			return E_FAIL;
		}
	}

	if (pStream->read((unsigned char*)(&pPinSound->m_cdata), sizeof(int)) == 0)
	{
		delete pPinSound;
		return E_FAIL;
	}

	pPinSound->m_pData = new char[pPinSound->m_cdata];

	if (pStream->read((unsigned char*)(pPinSound->m_pData), pPinSound->m_cdata) == 0)
	{
		delete pPinSound;
		return E_FAIL;
	}

	if (loadFileVersion >= NEW_SOUND_FORMAT_VERSION)
	{
		if (pStream->read((unsigned char*)(&pPinSound->m_outputTarget), sizeof(char)) == 0)
		{
			delete pPinSound;
			return E_FAIL;
		}

		if (pStream->read((unsigned char*)(&pPinSound->m_volume), sizeof(int)) == 0)
		{
			delete pPinSound;
			return E_FAIL;
		}

		if (pStream->read((unsigned char*)(&pPinSound->m_balance), sizeof(int)) == 0)
		{
			delete pPinSound;
			return E_FAIL;
		}

		if (pStream->read((unsigned char*)(&pPinSound->m_fade), sizeof(int)) == 0)
		{
			delete pPinSound;
			return E_FAIL;
		}

		if (pStream->read((unsigned char*)(&pPinSound->m_volume), sizeof(int)) == 0)
		{
			delete pPinSound;
			return E_FAIL;
		}
	}
	else
	{
		bool toBackglassOutput = false;

		if (pStream->read((unsigned char*)&toBackglassOutput, sizeof(bool)) == 0)
		{
			delete pPinSound;
			return E_FAIL;
		}

		/* TODO: pPinSound->m_outputTarget = (StrStrI(pPinSound->m_szName.c_str(), "bgout_") != NULL) || (_stricmp(pPinSound->m_szPath.c_str(), "* Backglass Output *") == 0) // legacy behavior, where the BG selection was encoded into the strings directly
		                              || toBackglassOutput
		                          ? SNDOUT_BACKGLASS
		                          : SNDOUT_TABLE;*/
	}

	HRESULT hr = pPinSound->ReInitialize();

	if (hr != S_OK)
	{
		delete pPinSound;
		return hr;
	}

	for (size_t i = 0; i < m_vsound.size(); ++i)
	{
		if (m_vsound[i]->m_szName == pPinSound->m_szName && m_vsound[i]->m_szPath == pPinSound->m_szPath)
		{
			delete pPinSound;
			return E_FAIL;
		}
	}

	m_vsound.push_back(pPinSound);

	return S_OK;
}

HRESULT PinTable::LoadImageFromStream(POLE::Stream* pStream, unsigned int index, int version)
{
	if (version < 100)
	{
		// TODO: ShowError("Tables from Tech Beta 3 and below are not supported in this version.");
		return E_FAIL;
	}
	else
	{
		Texture* const pTexture = new Texture();

		if (pTexture->LoadFromStream(pStream, version, this) == S_OK)
		{
			m_vimage[index] = pTexture;
		}
		else
		{
			delete pTexture;
		}
	}

	return S_OK;
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

	len = wcstombs(NULL, pWString, 0);

	*ppValue = (char*)malloc(len + 1);
	memset(*ppValue, 0, len + 1);

	wcstombs(*ppValue, pWString, len);

	std::cout << pPath << " = " << *ppValue << "\n";

	delete pStream;
}

Material* PinTable::GetMaterial(const std::string& szName) const
{
	if (szName.empty())
	{
		return &m_vpinball->m_dummyMaterial;
	}

	if (!m_materialMap.empty())
	{
		std::unordered_map<const char*, Material*, StringHashFunctor, StringComparator>::const_iterator
			it = m_materialMap.find(szName.c_str());
		if (it != m_materialMap.end())
		{
			return it->second;
		}
		else
		{
			return &m_vpinball->m_dummyMaterial;
		}
	}

	for (size_t i = 0; i < m_materials.size(); i++)
	{
		if (m_materials[i]->m_szName == szName)
		{
			return m_materials[i];
		}
	}

	return &m_vpinball->m_dummyMaterial;
}

Material* PinTable::GetSurfaceMaterial(const std::string& name) const
{
	if (!name.empty())
	{
		for (size_t i = 0; i < m_vedit.size(); i++)
		{
			IEditable* const item = m_vedit[i];
			if (item->GetItemType() == eItemSurface || item->GetItemType() == eItemRamp)
			{

				// TODO: CComBSTR bstr;
				// TODO: item->GetScriptable()->get_Name(&bstr);
				// TODO: if (!WzSzStrCmp(bstr, name.c_str()))
				{
					if (item->GetItemType() == eItemSurface)
					{
						return GetMaterial(((Surface*)item)->m_d.m_szTopMaterial);
					}
					else
					{
						return GetMaterial(((Ramp*)item)->m_d.m_szMaterial);
					}
				}
			}
		}
	}

	return GetMaterial(m_playfieldMaterial);
}

Texture* PinTable::GetImage(const std::string& szName) const
{
	if (szName.empty())
	{
		return NULL;
	}

	if (!m_textureMap.empty())
	{
		std::unordered_map<const char*, Texture*, StringHashFunctor, StringComparator>::const_iterator
			it = m_textureMap.find(szName.c_str());
		if (it != m_textureMap.end())
		{
			return it->second;
		}

		return NULL;
	}

	for (size_t i = 0; i < m_vimage.size(); i++)
	{
		if (!strcasecmp(m_vimage[i]->m_szName.c_str(), szName.c_str()))
		{
			return m_vimage[i];
		}
	}

	return NULL;
}

HRESULT PinTable::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	return S_OK;
}

PinTable* PinTable::GetPTable()
{
	return this;
}

HRESULT PinTable::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	SetDefaults(false);

	int csubobj;
	int csounds;
	int ctextures;
	int cfonts;
	int ccollection;

	LoadData(pStream, csubobj, csounds, ctextures, cfonts, ccollection, version);

	return S_OK;
}

HRESULT PinTable::InitPostLoad()
{
	return S_OK;
}

void PinTable::InitTablePostLoad()
{
}

void PinTable::SetDefaults(bool fromMouseClick)
{
}

void PinTable::SetDefaultPhysics(bool fromMouseClick)
{
	m_Gravity = 0.97f * GRAVITYCONST;

	m_friction = DEFAULT_TABLE_CONTACTFRICTION;
	m_elasticity = DEFAULT_TABLE_ELASTICITY;
	m_elasticityFalloff = DEFAULT_TABLE_ELASTICITY_FALLOFF;
	m_scatter = DEFAULT_TABLE_PFSCATTERANGLE;
}

bool PinTable::LoadToken(const int id, BiffReader* pBiffReader)
{
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
		RegUtil* pRegUtil = RegUtil::SharedInstance();
		m_plungerNormalize = pRegUtil->LoadValueIntWithDefault("Player", "PlungerNormalize", tmp);
		break;
	}
	case FID(MPDF):
	{
		bool tmp;
		pBiffReader->GetBool(tmp);
		RegUtil* pRegUtil = RegUtil::SharedInstance();
		m_plungerFilter = pRegUtil->LoadValueBoolWithDefault("Player", "PlungerFilter", tmp);
		break;
	}
	case FID(PHML):
	{
		pBiffReader->GetInt((int&)m_PhysicsMaxLoops);
		if (m_PhysicsMaxLoops == 0xFFFFFFFF)
		{
			RegUtil* pRegUtil = RegUtil::SharedInstance();
			pRegUtil->LoadValue("Player", "PhysicsMaxLoops", m_PhysicsMaxLoops);
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
	case FID(SECB):
		// TODO: pBiffReader->GetStruct(&m_protectionData, sizeof(ProtectionData));
		break;
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
		RegUtil* pRegUtil = RegUtil::SharedInstance();

		const HRESULT hr = pRegUtil->LoadValue("Player", "GlobalDifficulty", tmp);
		if (hr == S_OK)
		{
			m_globalDifficulty = dequantizeUnsignedPercent(tmp);
		}
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
			pMaterial->m_fGlossyImageLerp = 1.0f - dequantizeUnsigned<8>(mats[i].fGlossyImageLerp);
			pMaterial->m_fThickness = (mats[i].fThickness == 0) ? 0.05f : dequantizeUnsigned<8>(mats[i].fThickness);
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
			{
				m_materials.push_back(pmat);
			}
		}
		break;
	}
	}

	return true;
}

ItemTypeEnum PinTable::GetItemType() const
{
	return eItemTable;
}

IEditable* PinTable::GetIEditable()
{
	return (IEditable*)this;
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

PinBinary* PinTable::GetImageLinkBinary(const int id)
{
	switch (id)
	{
	case 1:
		PinBinary* const pPinBinary = m_pbTempScreenshot;
		m_pbTempScreenshot = NULL;
		return pPinBinary;
		break;
	}

	return NULL;
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
		{
			std::cout << "    ";
		}
		POLE::Stream* ss = new POLE::Stream(storage, fullname);
		std::cout << name;
		if (ss)
		{
			if (!ss->fail())
			{
				std::cout << "  (" << ss->size() << ")";
			}
		}
		std::cout << std::endl;
		delete ss;

		if (storage->isDirectory(fullname))
		{
			visit(indent + 1, storage, fullname + "/");
		}
	}
}

bool PinTable::GetDecalsEnabled() const
{
	return m_renderDecals;
}

bool PinTable::GetEMReelsEnabled() const
{
	return m_renderEMReels;
}

void PinTable::Play(const bool cameraMode)
{
	g_pplayer = new Player(cameraMode, this);
	g_pplayer->Create(1024, 768);
}

int PinTable::GetDetailLevel() const
{
	return m_overwriteGlobalDetailLevel ? m_userDetailLevel : m_globalDetailLevel;
}
