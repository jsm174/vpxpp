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

class GateData : public BaseProperty
{
public:
	Vertex2D m_vCenter;
	float m_length;
	float m_height;
	float m_rotation;
	TimerDataRoot m_tdr;
	float m_damping;
	float m_gravityfactor;
	std::string m_szSurface;
	float m_angleMin;
	float m_angleMax;
	GateType m_type;
	bool m_showBracket;
	bool m_twoWay;
};

class Gate : public ISelect,
             public IEditable,
             public IScriptable
{
public:
	static const ItemTypeEnum ItemType;
	static const int TypeNameID;
	static const int ToolID;
	static const int CursorID;
	static const unsigned AllowedViews;

	static Gate* COMCreate();
	static IEditable* COMCreateEditable();
	static IEditable* COMCreateAndInit(PinTable* ptable, float x, float y);

	Gate();
	~Gate();

	HRESULT Init(PinTable* ptable, float x, float y, bool fromMouseClick);

	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
	virtual PinTable* GetPTable();
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual void SetDefaults(bool fromMouseClick);
	virtual void SetDefaultPhysics(bool fromMouseClick);
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);

	virtual void WriteRegDefaults();

	GateData m_d;

private:
	PinTable* m_ptable;

	// TODO: LineSeg *m_plineseg;
	// TODO: HitGate *m_phitgate;

	VertexBuffer* m_wireVertexBuffer;
	IndexBuffer* m_wireIndexBuffer;
	float m_vertexbuffer_angle;

	VertexBuffer* m_bracketVertexBuffer;
	IndexBuffer* m_bracketIndexBuffer;

	const Vertex3D_NoTex2* m_vertices;
	const WORD* m_indices;
	unsigned int m_numVertices;
	unsigned int m_numIndices;
};