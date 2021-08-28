#pragma once

#include "BaseProperty.h"
#include "IEditable.h"
#include "ISelect.h"
#include "vector.h"

#include "PinTable.h"
#include "Timer.h"

class DispReelData : public BaseProperty
{
public:
	Vertex2D m_v1, m_v2;
	int m_imagesPerGridRow;
	int m_reelcount;
	float m_width, m_height;
	float m_reelspacing;
	int m_motorsteps;
	int m_digitrange;

	std::string m_szSound;
	int m_updateinterval;

	COLORREF m_backcolor;

	TimerDataRoot m_tdr;
	bool m_transparent;
	bool m_useImageGrid;
};

class DispReel : public ISelect,
                 public IEditable
{
public:
	static const ItemTypeEnum ItemType;
	static const int TypeNameID;
	static const int ToolID;
	static const int CursorID;
	static const unsigned AllowedViews;

	static DispReel* COMCreate();
	static IEditable* COMCreateEditable();
	static IEditable* COMCreateAndInit(PinTable* ptable, float x, float y);

	DispReel();
	~DispReel();

	HRESULT Init(PinTable* ptable, float x, float y, bool fromMouseClick);

	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
	virtual PinTable* GetPTable();
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);

	DispReelData m_d;

private:
	float getBoxWidth() const;
	float getBoxHeight() const;

	PinTable* m_ptable;
};