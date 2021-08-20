#include "PinTable.h"
#include "BiffReader.h"

#include <stdio.h>
#include <iostream>

PinTable::PinTable()
{
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

   BiffReader br(pStream, this, NULL, version);
   br.Load();

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
    //SetLoadDefaults();

    int rgi[6] = { 0, 0, 0, 0, 0, 0 };

    BiffReader br(pStream, this, rgi, version);
    br.Load();

    csubobj = rgi[1];
    csounds = rgi[2];
    ctextures = rgi[3];
    cfonts = rgi[4];
    ccollection = rgi[5];

    return S_OK;
}

bool PinTable::LoadToken(const int id, BiffReader * const pbr)
{
    
    
    switch(id) {
        case FID(CUST):
        {
            std::string tmp;
            pbr->GetString(tmp);
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

    BiffReader br(pStream, NULL, NULL, 0);
 
    char* pPtr = (char*)pWString;

    for (int index = 0; index < len; index++) {
        br.ReadBytes(pPtr, 2);
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
