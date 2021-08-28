#pragma once

#include "BaseProperty.h"
#include "IEditable.h"
#include "ISelect.h"
#include "vector.h"

#include "PinTable.h"
#include "Timer.h"

class SpinnerData : public BaseProperty
{
public:
	Vertex2D m_vCenter;
	float m_length;
	float m_rotation;
	TimerDataRoot m_tdr;
	float m_height;
	float m_damping;
	float m_angleMax;
	float m_angleMin;
	std::string m_szSurface;
	bool m_showBracket;
};

class Spinner : public ISelect,
                public IEditable
{
public:
	static const ItemTypeEnum ItemType;
	static const int TypeNameID;
	static const int ToolID;
	static const int CursorID;
	static const unsigned AllowedViews;

	static Spinner* COMCreate();
	static IEditable* COMCreateEditable();
	static IEditable* COMCreateAndInit(PinTable* ptable, float x, float y);

	Spinner();
	~Spinner();

	HRESULT Init(PinTable* ptable, float x, float y, bool fromMouseClick);

	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
	virtual PinTable* GetPTable();
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);

	SpinnerData m_d;

private:
	PinTable* m_ptable;
};