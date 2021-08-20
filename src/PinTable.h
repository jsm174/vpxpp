#if !defined(_PINTABLE_H__)
#define _PINTABLE_H__

#include "pole.h"
#include "ILoadable.h"
#include <vector>
#include "misc.h"

#define CURRENT_FILE_FORMAT_VERSION  1070

class PinTable : public ILoadable {
    public:
        PinTable();
        virtual ~PinTable();

        void LoadGameFromFilename(const char* pFilename);
        HRESULT LoadGameFromStorage(POLE::Storage* pStorage);

        HRESULT LoadInfo(POLE::Storage* pStorage, int version);
        HRESULT LoadCustomInfo(POLE::Storage* pStorage, int version);

        HRESULT LoadData(POLE::Stream* pStream, int& csubobj, int& csounds, int& ctextures, int& cfonts, int& ccollection, int version);

        void ReadInfoValue(POLE::Storage* pstg, const char* pName, char **pszValue);
        virtual bool LoadToken(const int id, BiffReader * const pbr);

        void visit( int indent, POLE::Storage* storage, std::string path );


    public:
        std::string m_szTableName;
        std::string m_szAuthor;
        std::string m_szVersion;
        std::string m_szReleaseDate;
        std::string m_szAuthorEMail;
        std::string m_szWebSite;
        std::string m_szBlurb;
        std::string m_szDescription;
        std::string m_szRules;
        std::string m_szScreenShot;
        std::string m_szDateSaved;
        unsigned int m_numTimesSaved;

        std::vector<std::string> m_vCustomInfoTag;
        std::vector<std::string> m_vCustomInfoContent;
};

#endif