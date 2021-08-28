#pragma once

#include "IEditable.h"
#include "ISelect.h"
#include "vector.h"

#include "PinTable.h"

class TimerDataRoot
{
public:
	int m_TimerInterval;
	bool m_TimerEnabled;
};

class TimerData
{
public:
	TimerDataRoot m_tdr;
	Vertex2D m_v;
};

class Timer : public ISelect,
              public IEditable
{
public:
	static const ItemTypeEnum ItemType;
	static const int TypeNameID;
	static const int ToolID;
	static const int CursorID;
	static const unsigned AllowedViews;

	static Timer* COMCreate();
	static IEditable* COMCreateEditable();
	static IEditable* COMCreateAndInit(PinTable* ptable, float x, float y);

	Timer();
	~Timer();

	HRESULT Init(PinTable* ptable, float x, float y, bool fromMouseClick);

	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
	virtual PinTable* GetPTable();
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);

	TimerData m_d;

private:
	PinTable* m_ptable;
};