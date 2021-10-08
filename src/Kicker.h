#pragma once

#include "BaseProperty.h"
#include "IEditable.h"
#include "IHitable.h"
#include "IScriptable.h"
#include "ISelect.h"
#include "Vertex2D.h"

#include "BiffReader.h"
#include "PinTable.h"
#include "RenderDevice.h"
#include "Timer.h"

class KickerData : public BaseProperty
{
public:
	Vertex2D m_vCenter;
	float m_radius;
	TimerDataRoot m_tdr;
	std::string m_szSurface;
	KickerType m_kickertype;
	float m_hitAccuracy;
	float m_orientation;
	float m_hit_height;
	bool m_enabled;
	bool m_fallThrough;
	bool m_legacyMode;
};

class Kicker : public ISelect,
			   public IEditable,
			   public IHitable,
			   public IScriptable
{
public:
	static const ItemTypeEnum ItemType;
	static const int TypeNameID;
	static const int ToolID;
	static const int CursorID;
	static const unsigned AllowedViews;

	static Kicker* COMCreate();
	static IEditable* COMCreateEditable();
	static IEditable* COMCreateAndInit(PinTable* ptable, float x, float y);

	Kicker();
	~Kicker();

	HRESULT Init(PinTable* ptable, float x, float y, bool fromMouseClick);

	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual HRESULT InitPostLoad();
	virtual void SetDefaults(bool fromMouseClick);
	virtual void SetDefaultPhysics(bool fromMouseClick);
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);
	virtual PinTable* GetPTable();
	virtual const PinTable* GetPTable() const;
	virtual IEditable* GetIEditable();
	virtual const IEditable* GetIEditable() const;
	virtual ISelect* GetISelect();
	virtual const ISelect* GetISelect() const;
	virtual IHitable* GetIHitable();
	virtual const IHitable* GetIHitable() const;
	virtual ItemTypeEnum GetItemType() const;
	virtual void WriteRegDefaults();
	virtual void GetHitShapes(std::vector<HitObject*>& pvho);
	virtual void GetHitShapesDebug(std::vector<HitObject*>& pvho);
	virtual void PreRenderStatic(RenderDevice* pd3dDevice);
	virtual void RenderStatic();
	virtual void RenderDynamic();
	virtual void RenderSetup();
	virtual ItemTypeEnum HitableGetItemType() const;
	virtual IScriptable* GetScriptable();
	virtual wchar_t* get_Name();

	KickerData m_d;

private:
	PinTable* m_ptable;

	// TODO: KickerHitCircle* m_phitkickercircle;

	VertexBuffer* m_vertexBuffer;
	IndexBuffer* m_indexBuffer;
	unsigned int m_numVertices;
	unsigned int m_numIndices;

	float m_baseHeight;

	VertexBuffer* m_plateVertexBuffer;
	IndexBuffer* m_plateIndexBuffer;
};