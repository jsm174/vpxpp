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

#include "RenderVertex3D.h"

class RampData : public BaseProperty
{
public:
	TimerDataRoot m_tdr;
	float m_heightbottom;
	float m_heighttop;
	float m_widthbottom;
	float m_widthtop;
	RampType m_type;
	RampImageAlignment m_imagealignment;
	float m_leftwallheight;
	float m_rightwallheight;
	float m_leftwallheightvisible;
	float m_rightwallheightvisible;

	float m_wireDiameter;
	float m_wireDistanceX;
	float m_wireDistanceY;

	float m_depthBias;

	bool m_imageWalls;
};

class Ramp : public ISelect,
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

	static Ramp* COMCreate();
	static IEditable* COMCreateEditable();
	static IEditable* COMCreateAndInit(PinTable* ptable, float x, float y);

	Ramp();
	~Ramp();

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

	void AssignHeightToControlPoint(const RenderVertex3D& v, const float height);

	RampData m_d;

private:
	Vertex2D* GetRampVertex(int& pcvertex, float** const ppheight, bool** const ppfCross, float** const ppratio, Vertex2D** const pMiddlePoints, const float _accuracy, const bool inc_width);

	template <typename T>
	void GetCentralCurve(std::vector<T>& vv, const float _accuracy = -1.f) const;

	PinTable* m_ptable;

	int m_rampVertex;
	Vertex2D* m_rgvInit;
	float* m_rgheightInit;

	int m_numVertices;
	int m_numIndices;
	Vertex3D_NoTex2* m_vertBuffer;
	Vertex3D_NoTex2* m_vertBuffer2;
	std::vector<short int> m_meshIndices;

	// TODO: std::vector<HitObject*> m_vhoCollidable;

	VertexBuffer* m_dynamicVertexBuffer;
	IndexBuffer* m_dynamicIndexBuffer;
	VertexBuffer* m_dynamicVertexBuffer2;
	bool m_dynamicVertexBufferRegenerate;

	// TODO: PropertyPane *m_propPhysics;

	bool isHabitrail() const;
};