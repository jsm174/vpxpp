#pragma once

#include "IEditable.h"
#include "IScriptable.h"
#include "ISelect.h"

#include "BiffReader.h"
#include "PinTable.h"
#include "Timer.h"

class TextboxData
{
public:
	Vertex2D m_v1;
	Vertex2D m_v2;
	float m_fontsize;
	COLORREF m_backcolor;
	COLORREF m_fontcolor;
	float m_intensity_scale;
	std::string m_sztext;
	TimerDataRoot m_tdr;
	TextAlignment m_talign;
	bool m_transparent;
	bool m_visible;
	bool m_isDMD;
};

class Textbox : public ISelect,
                public IEditable,
                public IScriptable
{
public:
	static const ItemTypeEnum ItemType;
	static const int TypeNameID;
	static const int ToolID;
	static const int CursorID;
	static const unsigned AllowedViews;

	static Textbox* COMCreate();
	static IEditable* COMCreateEditable();
	static IEditable* COMCreateAndInit(PinTable* ptable, float x, float y);

	Textbox();
	~Textbox();

	HRESULT Init(PinTable* ptable, float x, float y, bool fromMouseClick);

	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
	virtual PinTable* GetPTable();
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual void SetDefaults(bool fromMouseClick);
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);

	virtual void WriteRegDefaults();

	TextboxData m_d;

private:
	PinTable* m_ptable;
};