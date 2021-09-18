#include "ISelect.h"

ISelect::ISelect()
{
}

void ISelect::GetTypeNameForType(const ItemTypeEnum type, wchar_t* const buf) const
{
	// TODO: UINT strID;

	//switch (type)
	//{
	//case eItemTable:        strID = IDS_TABLE; break;
	//case eItemLightCenter:  strID = IDS_TB_LIGHT; break;
	//case eItemDragPoint:    strID = IDS_CONTROLPOINT; break;
	////case eItemLightSeqCenter: strID = IDS_TB_LIGHTSEQ; break;
	//default:
	//   strID = EditableRegistry::GetTypeNameStringID(type); break;
	//}

	///*const int len =*/ LoadStringW(m_vpinball->theInstance, strID, buf, 256);
}

bool ISelect::LoadToken(const int id, BiffReader* const pBiffReader)
{
	switch (id)
	{
	case FID(LOCK):
		pBiffReader->GetBool(m_locked);
		break;
	case FID(LAYR):
	{
		int tmp;
		pBiffReader->GetInt(tmp);
		m_oldLayerIndex = (char)tmp;
		break;
	}
	case FID(LANR):
	{
		pBiffReader->GetString(m_layerName);
		break;
	}
	case FID(LVIS):
	{
		pBiffReader->GetBool(m_isVisible);
		break;
	}
	}
	return true;
}