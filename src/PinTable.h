#pragma once

#include "IEditable.h"
#include "IScriptable.h"
#include "ISelect.h"
#include "pole.h"

#include "BiffReader.h"
#include "CodeViewer.h"
#include "Material.h"
#include "PinSound.h"
#include "PinBinary.h"
#include "Texture.h"

#include "cmath.h"
#include "hash.h"
#include "misc.h"
#include "physconst.h"
#include "vector.h"

#include <unordered_map>
#include <vector>

#define CURRENT_FILE_FORMAT_VERSION 1070

struct LightSource
{
	COLORREF emission;
	Vertex3Ds pos;
};

/* TODO: 
struct ProtectionData {
	long fileversion;
	long size;
	unsigned char paraphrase[16 + 8];
	unsigned long flags;
	int keyversion;
	int spare1;
	int spare2;
};
*/

class PinTable : public ISelect,
                 public IScriptable,
                 public IEditable
{
public:
	PinTable();
	virtual ~PinTable();

	void LoadGameFromFilename(const char* pFilename);
	HRESULT LoadGameFromStorage(POLE::Storage* pStorage);

	HRESULT LoadInfo(POLE::Storage* pStorage, int version);
	HRESULT LoadCustomInfo(POLE::Storage* pStorage, int version);
	HRESULT LoadData(POLE::Stream* pStream, int& csubobj, int& csounds, int& ctextures, int& cfonts, int& ccollection, int version);
	HRESULT LoadSoundFromStream(POLE::Stream* pStream, const int version);
	HRESULT LoadImageFromStream(POLE::Stream* pStream, unsigned int index, int version);
	void ReadInfoValue(POLE::Storage* pstg, const char* pName, char** pszValue);

	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
	virtual PinTable* GetPTable();
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual void SetDefaults(bool fromMouseClick);
	virtual void SetDefaultPhysics(bool fromMouseClick);
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);

	Material* GetMaterial(const std::string& szName);

	void GetUniqueName(const ItemTypeEnum type, wchar_t* const wzUniqueName, const unsigned int wzUniqueName_maxlength) const;
	void GetUniqueName(const wchar_t* const prefix, wchar_t* const wzUniqueName, const unsigned int wzUniqueName_maxlength) const;

	void SetLoadDefaults();

	PinBinary* GetImageLinkBinary(const int id);

	void visit(int indent, POLE::Storage* storage, std::string path);

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
	float m_defaultBulbIntensityScaleOnBall;

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

	float m_globalDifficulty;

	// TODO: ProtectionData m_protectionData;

	std::string m_image;
	std::string m_playfieldMaterial;
	COLORREF m_colorbackdrop;
	bool m_imageBackdropNightDay;

	std::string m_imageColorGrade;

	std::string m_ballImage;
	std::string m_ballImageDecal;
	bool m_BallDecalMode;

	std::string m_envImage;

	std::vector<IEditable*> m_vedit;

	std::vector<Texture*> m_vimage;

	int m_numMaterials;
	std::vector<Material*> m_materials;

	std::vector<PinSound*> m_vsound;

	COLORREF m_rgcolorcustom[16];

	float m_TableSoundVolume;
	float m_TableMusicVolume;

	int m_TableAdaptiveVSync;

	CodeViewer* m_pCodeViewer;

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

	PinBinary* m_pbTempScreenshot;

	std::vector<std::string> m_vCustomInfoTag;
	std::vector<std::string> m_vCustomInfoContent;

	int m_userDetailLevel;
	bool m_overwriteGlobalDetailLevel;

	bool m_overwriteGlobalDayNight;

	LightSource m_Light[MAX_LIGHT_SOURCES];
	COLORREF m_lightAmbient;
	float m_lightHeight;
	float m_lightRange;
	float m_lightEmissionScale;
	float m_envEmissionScale;
	float m_globalEmissionScale;
	float m_AOScale;
	float m_SSRScale;

	int m_useReflectionForBalls;
	float m_playfieldReflectionStrength;
	int m_useTrailForBalls;
	float m_ballTrailStrength;
	float m_ballPlayfieldReflectionStrength;
	int m_useAA;
	int m_useFXAA;
	int m_useAO;
	int m_useSSR;
	float m_bloom_strength;

	bool m_grid;
	bool m_backdrop;
	bool m_renderDecals;
	bool m_renderEMReels;
	bool m_overwriteGlobalStereo3D;
	bool m_reflectElementsOnPlayfield;

private:
	std::string m_notesText;

	std::unordered_map<const char*, Texture*, StringHashFunctor, StringComparator> m_textureMap;
	std::unordered_map<const char*, Material*, StringHashFunctor, StringComparator> m_materialMap;
};