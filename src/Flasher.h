#pragma once

#include "IEditable.h"
#include "IHaveDragPoints.h"
#include "IHitable.h"
#include "IScriptable.h"
#include "ISelect.h"

#include "BiffReader.h"
#include "PinTable.h"
#include "RenderDevice.h"
#include "Timer.h"
#include "Vertex3D_TexelOnly.h"

class FlasherData
{
public:
	Vertex2D m_vCenter;
	float m_height;
	COLORREF m_color;
	TimerDataRoot m_tdr;
	float m_rotX;
	float m_rotY;
	float m_rotZ;
	int m_alpha;
	float m_intensity_scale;
	float m_modulate_vs_add;
	float m_depthBias;
	int m_filterAmount;
	Filters m_filter;
	RampImageAlignment m_imagealignment;
	std::string m_szImageA;
	std::string m_szImageB;
	bool m_displayTexture;
	bool m_isVisible;
	bool m_addBlend;
	bool m_isDMD;
};

class Flasher : public ISelect,
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

	static Flasher* COMCreate();
	static IEditable* COMCreateEditable();
	static IEditable* COMCreateAndInit(PinTable* ptable, float x, float y);

	Flasher();
	~Flasher();

	HRESULT Init(PinTable* ptable, float x, float y, bool fromMouseClick);

	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual HRESULT InitPostLoad();
	virtual void SetDefaults(bool fromMouseClick);
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

	virtual bool IsTransparent() const;

	FlasherData m_d;

	bool m_lockedByLS;
	bool m_inPlayState;

private:
	PinTable* m_ptable;

	unsigned int m_numVertices;
	int m_numPolys;
	float m_minx;
	float m_maxx;
	float m_miny;
	float m_maxy;
	Vertex3D_TexelOnly* m_vertices;

	VertexBuffer* m_dynamicVertexBuffer;
	IndexBuffer* m_dynamicIndexBuffer;

	// TODO: PropertyPane* m_propVisual;

	bool m_dynamicVertexBufferRegenerate;
};