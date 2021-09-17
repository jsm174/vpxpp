#pragma once

#include "BaseProperty.h"
#include "IEditable.h"
#include "IScriptable.h"
#include "ISelect.h"
#include "Vertex2D.h"

#include "BiffReader.h"
#include "PinTable.h"

class DecalData : public BaseProperty
{
public:
	Vertex2D m_vCenter;
	float m_width;
	float m_height;
	float m_rotation;
	std::string m_szSurface;
	DecalType m_decaltype;
	std::string m_sztext;
	SizingType m_sizingtype;
	COLORREF m_color;
	bool m_verticalText;
};

class Decal : public ISelect,
			  public IEditable,
			  public IScriptable
{
public:
	static const ItemTypeEnum ItemType;
	static const int TypeNameID;
	static const int ToolID;
	static const int CursorID;
	static const unsigned AllowedViews;

	static Decal* COMCreate();
	static IEditable* COMCreateEditable();
	static IEditable* COMCreateAndInit(PinTable* ptable, float x, float y);

	Decal();
	~Decal();

	HRESULT Init(PinTable* ptable, float x, float y, bool fromMouseClick);

	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
	virtual PinTable* GetPTable();
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual HRESULT InitPostLoad();
	virtual void SetDefaults(bool fromMouseClick);
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);

	virtual void WriteRegDefaults();

	DecalData m_d;

private:
	PinTable* m_ptable;
};