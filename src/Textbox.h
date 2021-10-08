#pragma once

#include "IEditable.h"
#include "IHitable.h"
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
				public IHitable,
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

	TextboxData m_d;

private:
	PinTable* m_ptable;
};