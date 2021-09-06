#pragma once

#include "BiffReader.h"
#include "ILoadable.h"
#include "IScriptable.h"

#include "pole.h"

class PinTable;

class Collection : public IScriptable,
				   public ILoadable
{
public:
	Collection();

	virtual HRESULT LoadData(POLE::Stream* pStream, PinTable* pPinTable, int version);
	virtual bool LoadToken(const int id, BiffReader* const pBiffReader);

	bool m_fireEvents;
	bool m_stopSingleEvents;
	bool m_groupElements;
};