#pragma once

#include "IEditable.h"
#include "ISelect.h"
#include "BaseProperty.h"
#include "vector.h"
#include <string>

#include "PinTable.h"
#include "RenderDevice.h"

class BumperData : public BaseProperty
{
public:
	Vertex2D m_vCenter;
	float m_radius;
	float m_force;
	float m_heightScale;
	float m_orientation;
	float m_ringSpeed;
	float m_ringDropOffset;
	//TODO: U32 m_time_msec;
	//TODO: TimerDataRoot m_tdr;
	std::string m_szCapMaterial;
	std::string m_szBaseMaterial;
	std::string m_szSkirtMaterial;
	std::string m_szRingMaterial;
	std::string m_szSurface;
	bool m_capVisible;
	bool m_baseVisible;
	bool m_ringVisible;
	bool m_skirtVisible;
};

class Bumper :
	public IEditable
{
public:

	Bumper();
	~Bumper();

	static const ItemTypeEnum ItemType = eItemBumper;
	static const int TypeNameID = 0;
	static const int ToolID = 0; 
	static const int CursorID = 0;
	static const unsigned AllowedViews = 1;

	static Bumper* COMCreate()
	{
		return new Bumper();
	} 

	static IEditable* COMCreateEditable()   
	{ 
		return static_cast<IEditable*>(COMCreate()); 
	}

	BumperData m_d;

private:

	PinTable* m_ptable;

	VertexBuffer* m_baseVertexBuffer;
	IndexBuffer* m_baseIndexBuffer;

	VertexBuffer* m_socketVertexBuffer;
	IndexBuffer* m_socketIndexBuffer;

	VertexBuffer* m_ringVertexBuffer;
	IndexBuffer* m_ringIndexBuffer;

	VertexBuffer* m_capVertexBuffer;
	IndexBuffer* m_capIndexBuffer;

	// TODO: Matrix3D m_fullMatrix;
	Vertex3D_NoTex2* m_ringVertices;
	// TODO: Texture m_ringTexture;
	// TODO: Texture m_skirtTexture;
	// TODO: Texture m_baseTexture;
	// TODO: Texture m_capTexture;

	// TODO: PropertyPane* m_propVisual;

	float m_baseHeight;
	float m_skirtCounter;
	bool m_updateSkirt;
	bool m_doSkirtAnimation;
	bool m_enableSkirtAnimation;
	bool m_ringDown;
	bool m_ringAnimate;
};