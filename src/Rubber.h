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

#include "RenderVertex.h"

class RubberData : public BaseProperty
{
public:
	TimerDataRoot m_tdr;
	float m_height;
	float m_hitHeight;
	int m_thickness;
	float m_elasticityFalloff;
	float m_rotX;
	float m_rotY;
	float m_rotZ;
	bool m_staticRendering;
	bool m_showInEditor;
};

class Rubber : public ISelect,
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

	static Rubber* COMCreate();
	static IEditable* COMCreateEditable();
	static IEditable* COMCreateAndInit(PinTable* ptable, float x, float y);

	Rubber();
	~Rubber();

	HRESULT Init(PinTable* ptable, float x, float y, bool fromMouseClick);

	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
	virtual PinTable* GetPTable();
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual HRESULT InitPostLoad();
	virtual void SetDefaults(bool fromMouseClick);
	virtual void SetDefaultPhysics(bool fromMouseClick);
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);
	virtual IEditable* GetIEditable();
	virtual IHitable* GetIHitable();
	virtual void WriteRegDefaults();
	virtual void RenderStatic();
	virtual void RenderDynamic();
	virtual void RenderSetup();

	virtual void GetBoundingVertices(std::vector<Vertex3Ds>& pvvertex3D);

	RubberData m_d;

private:
	Vertex2D* GetSplineVertex(int& pcvertex, bool** const ppfCross, Vertex2D** const pMiddlePoints, const float _accuracy = -1.f);
	void GetCentralCurve(std::vector<RenderVertex>& vv, const float _accuracy) const;

	PinTable* m_ptable;

	VertexBuffer* m_dynamicVertexBuffer;
	IndexBuffer* m_dynamicIndexBuffer;
	bool m_dynamicVertexBufferRegenerate;

	// TODO: PropertyPane* m_propVisual;
	// TODO: PropertyPane* m_propPhysics;
};
