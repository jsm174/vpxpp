#pragma once

#include "BaseProperty.h"
#include "IEditable.h"
#include "ISelect.h"
#include "vector.h"

#include "PinTable.h"
#include "Timer.h"

class FlipperData : public BaseProperty
{
public:
	float m_BaseRadius;
	float m_EndRadius;
	float m_FlipperRadiusMin; // the flipper length reduction at maximum difficulty
	float m_FlipperRadiusMax;
	float m_FlipperRadius;
	float m_StartAngle;
	float m_EndAngle;
	float m_height;
	Vertex2D m_Center;
	TimerDataRoot m_tdr;

	std::string m_szSurface;
	COLORREF m_color;

	COLORREF m_rubbercolor;
	std::string m_szRubberMaterial;
	float m_rubberthickness;
	float m_rubberheight;
	float m_rubberwidth;

	float m_mass;
	float m_strength;
	float m_elasticityFalloff;
	float m_return;
	float m_rampUp;
	float m_torqueDamping;
	float m_torqueDampingAngle;

	//float m_angleEOS; // angle at which EOS switch opens, as measured from EOS parked position //!! reenable?

	float m_OverrideMass;
	float m_OverrideStrength;
	float m_OverrideElasticity;
	float m_OverrideElasticityFalloff;
	float m_OverrideFriction;
	float m_OverrideReturnStrength;
	float m_OverrideCoilRampUp;
	float m_OverrideTorqueDamping;
	float m_OverrideTorqueDampingAngle;
	float m_OverrideScatterAngle;
	int m_OverridePhysics;

	bool m_enabled;
};

class Flipper : public ISelect,
                public IEditable
{
public:
	static const ItemTypeEnum ItemType;
	static const int TypeNameID;
	static const int ToolID;
	static const int CursorID;
	static const unsigned AllowedViews;

	static Flipper* COMCreate();
	static IEditable* COMCreateEditable();
	static IEditable* COMCreateAndInit(PinTable* ptable, float x, float y);

	Flipper();
	~Flipper();

	HRESULT Init(PinTable* ptable, float x, float y, bool fromMouseClick);

	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
	virtual PinTable* GetPTable();
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);

	FlipperData m_d;

private:
	PinTable* m_ptable;
};
