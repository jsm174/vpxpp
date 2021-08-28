#pragma once

#include "IEditable.h"
#include "ISelect.h"

#include "PinTable.h"
#include "Timer.h"

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
                public IEditable
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
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);

	FlasherData m_d;

private:
	PinTable* m_ptable;
};