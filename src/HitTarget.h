#pragma once

#include "BaseProperty.h"
#include "IEditable.h"
#include "IHitable.h"
#include "IScriptable.h"
#include "ISelect.h"
#include "Vertex3Ds.h"

#include "BiffReader.h"
#include "PinTable.h"
#include "RenderDevice.h"
#include "Timer.h"

class HitTargetData : public BaseProperty
{
public:
	Vertex3Ds m_vPosition;
	Vertex3Ds m_vSize;
	float m_rotZ;
	TargetType m_targetType;

	TimerDataRoot m_tdr;

	float m_elasticityFalloff;
	float m_dropSpeed;
	uint32_t m_time_msec;
	int m_raiseDelay;

	float m_depthBias;
	float m_disableLightingTop;
	float m_disableLightingBelow;

	bool m_legacy;
	bool m_isDropped;
};

class HitTarget : public ISelect,
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

	static HitTarget* COMCreate();
	static IEditable* COMCreateEditable();
	static IEditable* COMCreateAndInit(PinTable* ptable, float x, float y);

	HitTarget();
	~HitTarget();

	HRESULT Init(PinTable* ptable, float x, float y, bool fromMouseClick);

	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
	virtual PinTable* GetPTable();
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual HRESULT InitPostLoad();
	virtual void SetDefaults(bool fromMouseClick);
	void SetDefaultPhysics(bool fromMouseClick);
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

	HitTargetData m_d;

	bool m_hitEvent;

private:
	PinTable* m_ptable;

	const Vertex3D_NoTex2* m_vertices;
	const WORD* m_indices;
	unsigned int m_numVertices;
	unsigned int m_numIndices;

	// TODO: PropertyPane* m_propVisual;
	// TODO: PropertyPane* m_propPosition;
	// TODO: PropertyPane* m_propPhysics;

	VertexBuffer* m_vertexBuffer;
	IndexBuffer* m_indexBuffer;

	uint32_t m_timeStamp;
	float m_moveAnimationOffset;
	bool m_moveAnimation;
	bool m_moveDown;
};