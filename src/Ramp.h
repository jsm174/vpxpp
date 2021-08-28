#pragma once

#include "BaseProperty.h"
#include "IEditable.h"
#include "ISelect.h"
#include "vector.h"

#include "PinTable.h"
#include "Timer.h"
#include "RenderDevice.h"

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

class Ramp : public IEditable
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
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);

	RampData m_d;

private:
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

	// TODO: bool isHabitrail() const;
};