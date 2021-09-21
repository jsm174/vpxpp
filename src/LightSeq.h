#pragma once

#include "IEditable.h"
#include "IHitable.h"
#include "IScriptable.h"
#include "ISelect.h"
#include "Vertex2D.h"

#include "BiffReader.h"
#include "PinTable.h"
#include "Timer.h"

#include "LightSeqAnimObject.h"

class LightSeqData
{
public:
	Vertex2D m_v;
	Vertex2D m_vCenter;
	std::wstring m_wzCollection;
	int m_updateinterval;
	TimerDataRoot m_tdr;
};

struct LightSeqQueueData
{
	SequencerState Animation;
	long TailLength;
	long Repeat;
	long Pause;
	long UpdateRate;
};

struct LightSeqQueue
{
	int Head;
	int Tail;
	LightSeqQueueData Data[LIGHTSEQQUEUESIZE];
};

enum
{
	eSeqNull,
	eSeqBlink,
	eSeqRandom,
	eSeqLine,
	eSeqCircle,
	eSeqRadar
};

class LightSeq : public ISelect,
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

	static LightSeq* COMCreate();
	static IEditable* COMCreateEditable();
	static IEditable* COMCreateAndInit(PinTable* ptable, float x, float y);

	LightSeq();
	~LightSeq();

	HRESULT Init(PinTable* ptable, float x, float y, bool fromMouseClick);

	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
	virtual PinTable* GetPTable();
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual HRESULT InitPostLoad();
	virtual void SetDefaults(bool fromMouseClick);
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);
	virtual ItemTypeEnum GetItemType() const;
	virtual IEditable* GetIEditable();
	virtual IHitable* GetIHitable();
	virtual void WriteRegDefaults();
	virtual void GetHitShapes(std::vector<HitObject*>& pvho);
	virtual void GetHitShapesDebug(std::vector<HitObject*>& pvho);
	virtual void RenderStatic();
	virtual void RenderDynamic();
	virtual void RenderSetup();
	virtual ItemTypeEnum HitableGetItemType() const;

	LightSeqAnimObject m_lightseqanim;

	LightSeqData m_d;

private:
	PinTable* m_ptable;
};