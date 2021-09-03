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
	virtual PinTable* GetPTable();
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual void SetDefaults(bool fromMouseClick);
	virtual void SetDefaultPhysics(bool fromMouseClick);
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);

	virtual void WriteRegDefaults();

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