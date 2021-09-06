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
			// TODO: IScriptable* const piscript = pPinTable->m_vedit[i]->GetScriptable();
			// if (piscript) // skip decals
			// {
			// 	if (!WideStrCmp(piscript->m_wzName, wzT))
			// 	{
			// 		piscript->GetISelect()->GetIEditable()->m_vCollection.push_back(this);
			// 		piscript->GetISelect()->GetIEditable()->m_viCollection.push_back(m_visel.size());
			// 		m_visel.push_back(piscript->GetISelect());
			// 		return true;
			// 	}
			// }
		}
		break;
	}
	}
	return true;
}
