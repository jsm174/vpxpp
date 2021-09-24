#pragma once

#include "BaseProperty.h"
#include "IEditable.h"
#include "IHaveDragPoints.h"
#include "IHitable.h"
#include "IScriptable.h"
#include "ISelect.h"
#include "Vertex2D.h"
#include "Vertex3Ds.h"

#include "BiffReader.h"
#include "PinTable.h"
#include "RenderDevice.h"
#include "Timer.h"

#include "LineSegSlingshot.h"

class SurfaceData : public BaseProperty
{
public:
	Vertex2D m_Center;
	TimerDataRoot m_tdr;
	float m_slingshot_threshold;
	std::string m_szSideImage;
	std::string m_szTopMaterial;
	std::string m_szSideMaterial;
	std::string m_szSlingShotMaterial;
	float m_heightbottom;
	float m_heighttop;
	float m_slingshotforce;
	float m_disableLightingTop;
	float m_disableLightingBelow;
	float m_elasticityFalloff;
	bool m_droppable;
	bool m_flipbook;
	bool m_displayTexture;
	bool m_sideVisible;
	bool m_enabled;
	bool m_isBottomSolid;
	bool m_slingshotAnimation;
	bool m_topBottomVisible;
	bool m_inner;
};

class Surface : public ISelect,
				public IEditable,
				public IHitable,
				public IScriptable,
				public IHaveDragPoints
{
public:
	static const ItemTypeEnum ItemType;
	static const int TypeNameID;
	static const int ToolID;
	static const int CursorID;
	static const unsigned AllowedViews;

	static const WORD rgiSlingshot[24];
	static IndexBuffer* slingIBuffer;

	static Surface* COMCreate();
	static IEditable* COMCreateEditable();
	static IEditable* COMCreateAndInit(PinTable* ptable, float x, float y);

	Surface();
	~Surface();

	HRESULT Init(PinTable* ptable, float x, float y, bool fromMouseClick);

	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
	virtual PinTable* GetPTable();
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual HRESULT InitPostLoad();
	virtual void SetDefaults(bool fromMouseClick);
	virtual void SetDefaultPhysics(bool fromMouseClick);
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);
	virtual ItemTypeEnum GetItemType() const;
	virtual IEditable* GetIEditable();
	virtual IHitable* GetIHitable();
	virtual void WriteRegDefaults();
	virtual void GetHitShapes(std::vector<HitObject*>& pvho);
	virtual void GetHitShapesDebug(std::vector<HitObject*>& pvho);
	virtual void RenderStatic();
	virtual void RenderDynamic();
	virtual void RenderSetup();
	virtual ItemTypeEnum HitableGetItemType() const;

	virtual void GetBoundingVertices(std::vector<Vertex3Ds>& pvvertex3D);

	SurfaceData m_d;
	bool m_disabled;

private:
	void PrepareWallsAtHeight();
	void PrepareSlingshots();
	void GenerateMesh(std::vector<Vertex3D_NoTex2>& topBuf, std::vector<Vertex3D_NoTex2>& sideBuf, std::vector<WORD>& topBottomIndices, std::vector<WORD>& sideIndices);

	PinTable* m_ptable;

	std::vector<LineSegSlingshot*> m_vlinesling;

	unsigned int m_numVertices;
	unsigned int m_numPolys;

	VertexBuffer* m_slingshotVBuffer;
	VertexBuffer* m_VBuffer;
	IndexBuffer* m_IBuffer;

	// TODO: PropertyPane* m_propPhysics;

	bool m_isDynamic;

	bool m_isWall;
};