#pragma once

#include "IEditable.h"
#include "IScriptable.h"
#include "ISelect.h"
#include "vector.h"

#include "BiffReader.h"
#include "PinTable.h"
#include "Timer.h"

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
	virtual void SetDefaults(bool fromMouseClick);
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);

	virtual void WriteRegDefaults();

	class LightSeqAnimObject // TODO: : public AnimObject
	{
	public:
		virtual void Animate()
		{
			// TODO: m_pLightSeq->Animate();
		} 
		LightSeq* m_pLightSeq;
	} m_lightseqanim;

	LightSeqData m_d;

private:
	PinTable* m_ptable;
};