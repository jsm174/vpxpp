#pragma once

#include "IEditable.h"
#include "ISelect.h"
#include "PinTable.h"

#include "BaseProperty.h"
#include "RenderDevice.h"
#include "vector.h"

typedef enum
{
	RampTypeFlat = 0,
	RampType4Wire = 1,
	RampType2Wire = 2,
	RampType3WireLeft = 3,
	RampType3WireRight = 4,
	RampType1Wire = 5,
} RampType;

typedef enum
{
	ImageModeWorld = 0,
	ImageModeWrap = 1,
} RampImageAlignment;

class RampData : public BaseProperty
{
public:
	//TODO: TimerDataRoot m_tdr;
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
	Ramp();
	~Ramp();

	static const ItemTypeEnum ItemType = eItemRamp;
	static const int TypeNameID = 0;
	static const int ToolID = 0;
	static const int CursorID = 0;
	static const unsigned AllowedViews = 1;

	static Ramp* COMCreate()
	{
		return new Ramp();
	}

	static IEditable* COMCreateEditable()
	{
		return static_cast<IEditable*>(COMCreate());
	}

	RampData m_d;

	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);

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