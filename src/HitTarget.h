#pragma once

#include "BaseProperty.h"
#include "IEditable.h"
#include "ISelect.h"
#include "vector.h"

#include "PinTable.h"
#include "Timer.h"

class HitTargetData : public BaseProperty
{
public:
	Vertex3Ds m_vPosition;
	Vertex3Ds m_vSize;
	float m_rotZ;
	TargetType m_targetType;

	TimerDataRoot m_tdr;

	float m_elasticityFalloff;
	float m_dropSpeed;
	uint32_t m_time_msec;
	int m_raiseDelay;

	float m_depthBias;
	float m_disableLightingTop;
	float m_disableLightingBelow;

	bool m_legacy;
	bool m_isDropped;
};

class HitTarget : public ISelect,
               public IEditable
{
public:
	static const ItemTypeEnum ItemType;
	static const int TypeNameID;
	static const int ToolID;
	static const int CursorID;
	static const unsigned AllowedViews;

	static HitTarget* COMCreate();
	static IEditable* COMCreateEditable();
	static IEditable* COMCreateAndInit(PinTable* ptable, float x, float y);

	HitTarget();
	~HitTarget();

	HRESULT Init(PinTable* ptable, float x, float y, bool fromMouseClick);

	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
	virtual PinTable* GetPTable();
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);

	HitTargetData m_d;

private:
	PinTable* m_ptable;
};