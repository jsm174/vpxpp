#pragma once

#include "IEditable.h"
#include "IScriptable.h"
#include "ISelect.h"

#include "BiffReader.h"
#include "PinTable.h"
#include "RenderDevice.h"
#include "Timer.h"
#include "def.h"

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
                public IScriptable
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
	virtual PinTable* GetPTable();
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual void SetDefaults(bool fromMouseClick);
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);

	virtual void WriteRegDefaults();

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