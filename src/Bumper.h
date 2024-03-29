#pragma once

#include "BaseProperty.h"
#include "IEditable.h"
#include "IHitable.h"
#include "IScriptable.h"
#include "ISelect.h"
#include "Vertex2D.h"

#include "BiffReader.h"
#include "PinTable.h"
#include "RenderDevice.h"
#include "Timer.h"

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
	uint32_t m_time_msec;
	TimerDataRoot m_tdr;
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

class Bumper : public ISelect,
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

	static Bumper* COMCreate();
	static IEditable* COMCreateEditable();
	static IEditable* COMCreateAndInit(PinTable* ptable, float x, float y);

	Bumper();
	~Bumper();

	HRESULT Init(PinTable* ptable, float x, float y, bool fromMouseClick);

	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual HRESULT InitPostLoad();
	virtual void SetDefaults(bool fromMouseClick);
	virtual void SetDefaultPhysics(bool fromMouseClick);
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);
	virtual PinTable* GetPTable();
	virtual const PinTable* GetPTable() const;
	virtual IEditable* GetIEditable();
	virtual const IEditable* GetIEditable() const;
	virtual ISelect* GetISelect();
	virtual const ISelect* GetISelect() const;
	virtual IHitable* GetIHitable();
	virtual const IHitable* GetIHitable() const;

	virtual ItemTypeEnum GetItemType() const;
	virtual void WriteRegDefaults();
	virtual void GetHitShapes(std::vector<HitObject*>& pvho);
	virtual void GetHitShapesDebug(std::vector<HitObject*>& pvho);
	virtual void RenderStatic();
	virtual void RenderDynamic();
	virtual void RenderSetup();
	virtual ItemTypeEnum HitableGetItemType() const;
	virtual IScriptable* GetScriptable();
	virtual wchar_t* get_Name();

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