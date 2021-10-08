#include "Collection.h"
#include "PinTable.h"
#include "RegUtil.h"

Collection::Collection()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_fireEvents = false;
	m_stopSingleEvents = false;

	m_groupElements = pRegUtil->LoadValueBoolWithDefault("Editor", "GroupElementsInCollection", true);
}

HRESULT Collection::LoadData(POLE::Stream* pStream, PinTable* pPinTable, int version)
{
	BiffReader biffReader(pStream, this, pPinTable, version);
	biffReader.Load();

	return S_OK;
}

bool Collection::LoadToken(const int id, BiffReader* const pBiffReader)
{
	switch (id)
	{
	case FID(NAME):
	{
		wchar_t tmp[MAXNAMEBUFFER + 1];
		pBiffReader->GetWideString(tmp, MAXNAMEBUFFER + 1);
		memcpy(m_wzName, tmp, (MAXNAMEBUFFER - 1) * sizeof(wchar_t));
		m_wzName[MAXNAMEBUFFER - 1] = 0;
		break;
	}
	case FID(EVNT):
		pBiffReader->GetBool(m_fireEvents);
		break;
	case FID(SSNG):
		pBiffReader->GetBool(m_stopSingleEvents);
		break;
	case FID(GREL):
		pBiffReader->GetBool(m_groupElements);
		break;
	case FID(ITEM):
	{
		PinTable* const pPinTable = (PinTable*)pBiffReader->m_pData;

		wchar_t wzT[MAXNAMEBUFFER * 2];
		pBiffReader->GetWideString(wzT, MAXNAMEBUFFER * 2);

		for (size_t i = 0; i < pPinTable->m_vedit.size(); ++i)
		{
			IScriptable* const pIScriptable = pPinTable->m_vedit[i]->GetScriptable();
			if (pIScriptable)
			{
				if (!wcscmp(pIScriptable->m_wzName, wzT))
				{
					pIScriptable->GetISelect()->GetIEditable()->m_vCollection.push_back(this);
					// TODO: pIScriptable->GetISelect()->GetIEditable()->m_viCollection.push_back(m_visel.size());
					// TODO: m_visel.push_back(pIScriptable->GetISelect());
					return true;
				}
			}
		}
		break;
	}
	}
	return true;
}

ISelect* Collection::GetISelect()
{
	return NULL;
}

const ISelect* Collection::GetISelect() const
{
	return NULL;
}

wchar_t* Collection::get_Name()
{
	return m_wzName;
}
