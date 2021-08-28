#pragma once

#include "BaseProperty.h"
#include "IEditable.h"
#include "ISelect.h"
#include "vector.h"

#include "PinTable.h"
#include "Timer.h"

class PrimitiveData : public BaseProperty
{
public:
	int m_Sides;
	Vertex3Ds m_vPosition;
	Vertex3Ds m_vSize;
	float m_aRotAndTra[9];
	std::string m_szNormalMap;
	std::string m_meshFileName;

	COLORREF m_SideColor;
	TimerDataRoot m_tdr;

	float m_elasticityFalloff;
	float m_depthBias;

	float m_edgeFactorUI;
	float m_collision_reductionFactor;

	float m_disableLightingTop;
	float m_disableLightingBelow;

	bool m_useAsPlayfield;

	bool m_use3DMesh;
	bool m_drawTexturesInside;
	bool m_staticRendering;

	bool m_toy;
	bool m_skipRendering;
	bool m_groupdRendering;
	bool m_backfacesEnabled;
	bool m_displayTexture;
	bool m_objectSpaceNormalMap;
};

class Primitive : public ISelect,
                  public IEditable
{
public:
	static const ItemTypeEnum ItemType;
	static const int TypeNameID;
	static const int ToolID;
	static const int CursorID;
	static const unsigned AllowedViews;

	static Primitive* COMCreate();
	static IEditable* COMCreateEditable();
	static IEditable* COMCreateAndInit(PinTable* ptable, float x, float y);

	Primitive();
	~Primitive();

	HRESULT Init(PinTable* ptable, float x, float y, bool fromMouseClick);

	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
	virtual PinTable* GetPTable();
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);

	PrimitiveData m_d;

private:
	PinTable* m_ptable;
};