#pragma once

#include "BaseProperty.h"
#include "IEditable.h"
#include "IHitable.h"
#include "IScriptable.h"
#include "ISelect.h"
#include "Vertex2D.h"

#include "PinTable.h"
#include "RenderDevice.h"
#include "Timer.h"

class PlungerData : public BaseProperty
{
public:
	COLORREF m_color;
	Vertex2D m_v;
	float m_width;
	float m_height;
	float m_stroke;
	float m_zAdjust;
	float m_speedPull;
	float m_speedFire;
	float m_mechStrength;
	PlungerType m_type;
	int m_animFrames;
	TimerDataRoot m_tdr;
	float m_parkPosition;
	std::string m_szSurface;
	float m_scatterVelocity;
	float m_momentumXfer;
	char m_szTipShape[MAXTIPSHAPE];
	float m_rodDiam;
	float m_ringGap;
	float m_ringDiam;
	float m_ringWidth;
	float m_springDiam;
	float m_springGauge;
	float m_springLoops;
	float m_springEndLoops;
	bool m_mechPlunger;
	bool m_autoPlunger;
};

class Plunger : public ISelect,
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

	static Plunger* COMCreate();
	static IEditable* COMCreateEditable();
	static IEditable* COMCreateAndInit(PinTable* ptable, float x, float y);

	Plunger();
	~Plunger();

	HRESULT Init(PinTable* ptable, float x, float y, bool fromMouseClick);

	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
	virtual PinTable* GetPTable();
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual HRESULT InitPostLoad();
	virtual void SetDefaults(bool fromMouseClick);
	void SetDefaultPhysics(bool fromMouseClick);
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);
	virtual IEditable* GetIEditable();
	virtual IHitable* GetIHitable();
	virtual void WriteRegDefaults();
	virtual void RenderStatic();
	virtual void RenderDynamic();
	virtual void RenderSetup();

	PlungerData m_d;

private:
	PinTable* m_ptable;

	VertexBuffer* m_vertexBuffer;
	IndexBuffer* m_indexBuffer;

	// TODO: HitPlunger* m_phitplunger;
};