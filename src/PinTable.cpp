#include "PinTable.h"
#include "BiffReader.h"

#include <stdio.h>
#include <float.h>
#include <iostream>

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

    m_3DZPD = 0.5f;
    m_3DmaxSeparation = 0.03f;
    m_3DOffset = 0.0f;
    m_overwriteGlobalStereo3D = false;

}

PinTable::~PinTable()
{
}

void PinTable::LoadGameFromFilename(const char* pFilename) {
    POLE::Storage* pStorage = new POLE::Storage( pFilename );
    pStorage->open();
    
    if ( pStorage->result() != POLE::Storage::Ok )
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

        LoadData(pGameStream, csubobj, csounds, ctextures, cfonts, ccollection, loadFileVersion);

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
    if (pValue != nullptr) {
        m_numTimesSaved = atoi(pValue);
        delete[] pValue;
    }
    else {
        m_numTimesSaved = 0;
    }

    // TODO: LOAD SCREENSHOT

    return S_OK;
}

HRESULT PinTable::LoadCustomInfo(POLE::Storage* pStorage, int version) {
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

    int rgi[6] = { 0, 0, 0, 0, 0, 0 };

    BiffReader biffReader(pStream, this, rgi, version);
    biffReader.Load();

    csubobj = rgi[1];
    csounds = rgi[2];
    ctextures = rgi[3];
    cfonts = rgi[4];
    ccollection = rgi[5];

    return S_OK;
}

bool PinTable::LoadToken(const int id, BiffReader * const pBiffReader)
{
    switch(id) {       
        case FID(LEFT): pBiffReader->GetFloat(m_left); break;
        case FID(TOPX): pBiffReader->GetFloat(m_top); break;
        case FID(RGHT): pBiffReader->GetFloat(m_right); break;
        case FID(BOTM): pBiffReader->GetFloat(m_bottom); break;
        case FID(ROTA): pBiffReader->GetFloat(m_BG_rotation[BG_DESKTOP]); break;
        case FID(LAYB): pBiffReader->GetFloat(m_BG_layback[BG_DESKTOP]); break;
        case FID(INCL): pBiffReader->GetFloat(m_BG_inclination[BG_DESKTOP]); break;
        case FID(FOVX): pBiffReader->GetFloat(m_BG_FOV[BG_DESKTOP]); break;
        case FID(SCLX): pBiffReader->GetFloat(m_BG_scalex[BG_DESKTOP]); break;
        case FID(SCLY): pBiffReader->GetFloat(m_BG_scaley[BG_DESKTOP]); break;
        case FID(SCLZ): pBiffReader->GetFloat(m_BG_scalez[BG_DESKTOP]); break;
        case FID(XLTX): pBiffReader->GetFloat(m_BG_xlatex[BG_DESKTOP]); break;
        case FID(XLTY): pBiffReader->GetFloat(m_BG_xlatey[BG_DESKTOP]); break;
        case FID(XLTZ): pBiffReader->GetFloat(m_BG_xlatez[BG_DESKTOP]); break;
        case FID(ROTF): pBiffReader->GetFloat(m_BG_rotation[BG_FULLSCREEN]); break;
        case FID(LAYF): pBiffReader->GetFloat(m_BG_layback[BG_FULLSCREEN]); break;
        case FID(INCF): pBiffReader->GetFloat(m_BG_inclination[BG_FULLSCREEN]); break;
        case FID(FOVF): pBiffReader->GetFloat(m_BG_FOV[BG_FULLSCREEN]); break;
        case FID(SCFX): pBiffReader->GetFloat(m_BG_scalex[BG_FULLSCREEN]); break;
        case FID(SCFY): pBiffReader->GetFloat(m_BG_scaley[BG_FULLSCREEN]); break;
        case FID(SCFZ): pBiffReader->GetFloat(m_BG_scalez[BG_FULLSCREEN]); break;
        case FID(XLFX): pBiffReader->GetFloat(m_BG_xlatex[BG_FULLSCREEN]); break;
        case FID(XLFY): pBiffReader->GetFloat(m_BG_xlatey[BG_FULLSCREEN]); break;
        case FID(XLFZ): pBiffReader->GetFloat(m_BG_xlatez[BG_FULLSCREEN]); break;
        case FID(ROFS): pBiffReader->GetFloat(m_BG_rotation[BG_FSS]); break;
        case FID(LAFS): pBiffReader->GetFloat(m_BG_layback[BG_FSS]); break;
        case FID(INFS): pBiffReader->GetFloat(m_BG_inclination[BG_FSS]); break;
        case FID(FOFS): pBiffReader->GetFloat(m_BG_FOV[BG_FSS]); break;
        case FID(SCXS): pBiffReader->GetFloat(m_BG_scalex[BG_FSS]); break;
        case FID(SCYS): pBiffReader->GetFloat(m_BG_scaley[BG_FSS]); break;
        case FID(SCZS): pBiffReader->GetFloat(m_BG_scalez[BG_FSS]); break;
        case FID(XLXS): pBiffReader->GetFloat(m_BG_xlatex[BG_FSS]); break;
        case FID(XLYS): pBiffReader->GetFloat(m_BG_xlatey[BG_FSS]); break;
        case FID(XLZS): pBiffReader->GetFloat(m_BG_xlatez[BG_FSS]); break;
        case FID(EFSS):
        {
            pBiffReader->GetBool(m_BG_enable_FSS);
            if (m_BG_enable_FSS)
            {
                m_BG_current_set = BG_FSS;
            }
            break;
        }
        case FID(ORRP): pBiffReader->GetInt(m_overridePhysics); break;
        case FID(ORPF): pBiffReader->GetBool(m_overridePhysicsFlipper); break;
        case FID(GAVT): pBiffReader->GetFloat(m_Gravity); break;
        case FID(FRCT): pBiffReader->GetFloat(m_friction); break;
        case FID(ELAS): pBiffReader->GetFloat(m_elasticity); break;
        case FID(ELFA): pBiffReader->GetFloat(m_elasticityFalloff); break;
        case FID(PFSC): pBiffReader->GetFloat(m_scatter); break;
        case FID(SCAT): pBiffReader->GetFloat(m_defaultScatter); break;
        case FID(NDGT): pBiffReader->GetFloat(m_nudgeTime); break;
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
        case FID(DECL): pBiffReader->GetBool(m_renderDecals); break;
        case FID(REEL): pBiffReader->GetBool(m_renderEMReels); break;
        case FID(OFFX): pBiffReader->GetFloat(m_offset.x); break;
        case FID(OFFY): pBiffReader->GetFloat(m_offset.y); break;
        case FID(ZOOM): pBiffReader->GetFloat(m_zoom); break;
        case FID(MAXSEP): pBiffReader->GetFloat(m_3DmaxSeparation); break;
        case FID(ZPD): pBiffReader->GetFloat(m_3DZPD); break;
        case FID(STO): pBiffReader->GetFloat(m_3DOffset); break;
        case FID(OGST): pBiffReader->GetBool(m_overwriteGlobalStereo3D); break;
        case FID(SLPX): pBiffReader->GetFloat(m_angletiltMax); break;
        case FID(SLOP): pBiffReader->GetFloat(m_angletiltMin); break;
        case FID(GLAS): pBiffReader->GetFloat(m_glassheight); break;
        case FID(TBLH): pBiffReader->GetFloat(m_tableheight); break;
        case FID(IMAG): pBiffReader->GetString(m_image); break;
        case FID(BLIM): pBiffReader->GetString(m_ballImage); break;
        case FID(BLIF): pBiffReader->GetString(m_ballImageDecal); break;
        case FID(SSHT): pBiffReader->GetString(m_szScreenShot); break;
        /* case FID(FBCK): pBiffReader->GetBool(m_backdrop); break;
        case FID(SEDT): pBiffReader->GetInt(&((int *)pBiffReader->m_pdata)[1]); break;
        case FID(SSND): pBiffReader->GetInt(&((int *)pBiffReader->m_pdata)[2]); break;
        case FID(SIMG): pBiffReader->GetInt(&((int *)pBiffReader->m_pdata)[3]); break;
        case FID(SFNT): pBiffReader->GetInt(&((int *)pBiffReader->m_pdata)[4]); break;
        case FID(SCOL): pBiffReader->GetInt(&((int *)pBiffReader->m_pdata)[5]); break;
        case FID(NAME): pBiffReader->GetWideString(m_wzName,sizeof(m_wzName)/sizeof(m_wzName[0])); break;
        case FID(BIMG): pBiffReader->GetString(m_BG_image[0]); break;
        case FID(BIMF): pBiffReader->GetString(m_BG_image[1]); break;
        case FID(BIMS): pBiffReader->GetString(m_BG_image[2]); break;
        case FID(BIMN): pBiffReader->GetBool(m_ImageBackdropNightDay); break;
        case FID(IMCG): pBiffReader->GetString(m_imageColorGrade); break;
        case FID(EIMG): pBiffReader->GetString(m_envImage); break;
        case FID(PLMA): pBiffReader->GetString(m_playfieldMaterial); break;
        case FID(NOTX): {std::string txt;  pBiffReader->GetString(txt); m_notesText = CString(txt.c_str()); break; }
        case FID(LZAM): pBiffReader->GetInt(m_lightAmbient); break;
        case FID(LZDI): pBiffReader->GetInt(m_Light[0].emission); break;
        case FID(LZHI): pBiffReader->GetFloat(m_lightHeight); break;
        case FID(LZRA): pBiffReader->GetFloat(m_lightRange); break;
        case FID(LIES): pBiffReader->GetFloat(m_lightEmissionScale); break;
        case FID(ENES): pBiffReader->GetFloat(m_envEmissionScale); break;
        case FID(GLES): pBiffReader->GetFloat(m_globalEmissionScale); break;
        case FID(AOSC): pBiffReader->GetFloat(m_AOScale); break;
        case FID(SSSC): pBiffReader->GetFloat(m_SSRScale); break;
        case FID(BREF): pBiffReader->GetInt(m_useReflectionForBalls); break;*/
        case FID(CUST):
        {
            std::string tmp;
            pBiffReader->GetString(tmp);
            m_vCustomInfoTag.push_back(tmp);
            break;
        }
        case FID(ENDB):
            break;
        default:
            printf("FID: %c%c%c%c NOT IMPLEMENTED!\n", (id & 0xFF), ((id >> 8) & 0xFF), ((id >> 16 & 0xFF)), ((id >> 24 & 0xFF)));
            break;
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

    for (int index = 0; index < len; index++) {
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

void PinTable::visit( int indent, POLE::Storage* storage, std::string path )
{
    std::list<std::string> entries;
    entries = storage->entries( path );

    std::list<std::string>::iterator it;
    for( it = entries.begin(); it != entries.end(); ++it )
    {
        std::string name = *it;
        std::string fullname = path + name;
        for( int j = 0; j < indent; j++ ) std::cout << "    ";
        POLE::Stream* ss = new POLE::Stream( storage, fullname );
        std::cout << name;
        if( ss ) if( !ss->fail() )std::cout << "  (" << ss->size() << ")";
        std::cout << std::endl;
        delete ss;

        if( storage->isDirectory( fullname ) )
            visit( indent+1, storage, fullname + "/" );
    }
}
