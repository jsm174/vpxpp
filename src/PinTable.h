#if !defined(_PINTABLE_H__)
#define _PINTABLE_H__

#include "pole.h"
#include "ILoadable.h"
#include <vector>
#include "misc.h"
#include "vector.h"

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
        virtual bool LoadToken(const int id, BiffReader* pBiffReader);

        void visit( int indent, POLE::Storage* storage, std::string path );


    public:
        Vertex2D m_offset;
        float m_zoom;

        float m_left;
        float m_top;
        float m_right;
        float m_bottom;

        float m_glassheight;
        float m_tableheight;

        float m_3DmaxSeparation;
        // TODO: float m_global3DMaxSeparation;
        float m_3DZPD;
        // TODO: float m_global3DZPD;
        float m_3DOffset;
        // TODO: float m_global3DOffset;
        // TODO: float m_defaultBulbIntensityScaleOnBall;

        unsigned int m_BG_current_set;
        float m_BG_inclination[NUM_BG_SETS];
        float m_BG_FOV[NUM_BG_SETS];
        float m_BG_layback[NUM_BG_SETS];
        float m_BG_rotation[NUM_BG_SETS];
        float m_BG_scalex[NUM_BG_SETS];
        float m_BG_scaley[NUM_BG_SETS];
        float m_BG_scalez[NUM_BG_SETS];
        float m_BG_xlatex[NUM_BG_SETS];
        float m_BG_xlatey[NUM_BG_SETS];
        float m_BG_xlatez[NUM_BG_SETS];
        std::string m_BG_image[NUM_BG_SETS];

        bool m_BG_enable_FSS;
        int m_currentBackglassMode;

        float m_angletiltMax;
        float m_angletiltMin;

        int m_overridePhysics;

        unsigned int m_PhysicsMaxLoops;

        float m_Gravity;
        
        float m_friction;
        float m_elasticity;
        float m_elasticityFalloff;
        float m_scatter;

        float m_defaultScatter;
        float m_nudgeTime;
        int m_plungerNormalize;
        bool m_plungerFilter;

        bool m_overridePhysicsFlipper;

        std::string m_image;
        std::string m_ballImage;
        std::string m_ballImageDecal;

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

        bool m_renderDecals;
        bool m_renderEMReels;
        bool m_overwriteGlobalStereo3D;
};

#endif