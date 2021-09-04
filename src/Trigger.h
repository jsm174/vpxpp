#pragma once

#include "BaseProperty.h"
#include "IEditable.h"
#include "IScriptable.h"
#include "ISelect.h"
#include "vector.h"

#include "BiffReader.h"
#include "PinTable.h"
#include "RenderDevice.h"
#include "Timer.h"

class TriggerData : public BaseProperty
{
public:
	Vertex2D m_vCenter;
	float m_radius;
	TimerDataRoot m_tdr;
	std::string m_szSurface;
	TriggerShape m_shape;
	float m_rotation;
	float m_scaleX;
	float m_scaleY;
	float m_hit_height;
	uint32_t m_time_msec;
	float m_animSpeed;
	float m_wireThickness;
	bool m_enabled;
};

class Trigger : public ISelect,
                public IEditable,
                public IScriptable
{
public:
	static const ItemTypeEnum ItemType;
	static const int TypeNameID;
	static const int ToolID;
	static const int CursorID;
	static const unsigned AllowedViews;

	static Trigger* COMCreate();
	static IEditable* COMCreateEditable();
	static IEditable* COMCreateAndInit(PinTable* ptable, float x, float y);

	Trigger();
	~Trigger();

	HRESULT Init(PinTable* ptable, float x, float y, bool fromMouseClick);

	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
	virtual PinTable* GetPTable();
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual void SetDefaults(bool fromMouseClick);
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);

	virtual void WriteRegDefaults();

	TriggerData m_d;

	bool m_hitEnabled;

private:
	PinTable* m_ptable;

	// TODO: TriggerHitCircle* m_ptriggerhitcircle;

	VertexBuffer* m_vertexBuffer;
	IndexBuffer* m_triggerIndexBuffer;
	Vertex3D_NoTex2* m_triggerVertices;

	// TODO: PropertyPane* m_propVisual;

	float m_animHeightOffset;
	float m_vertexBuffer_animHeightOffset;
	bool m_hitEvent;
	bool m_unhitEvent;
	bool m_doAnimation;
	bool m_moveDown;
};