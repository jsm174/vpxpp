#include "ISelect.h"

ISelect::ISelect()
{
}

void ISelect::GetTypeNameForType(const ItemTypeEnum type, wchar_t* const buf) const
{
	//TODO: UINT strID;

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

bool ISelect::LoadToken(const int id, BiffReader* const pbr)
{
	switch (id)
	{
	case FID(LOCK):
		pbr->GetBool(m_locked);
		break;
	case FID(LAYR):
	{
		int tmp;
		pbr->GetInt(tmp);
		m_oldLayerIndex = (char)tmp;
		break;
	}
	case FID(LANR):
	{
		pbr->GetString(m_layerName);
		break;
	}
	case FID(LVIS):
	{
		pbr->GetBool(m_isVisible);
		break;
	}
	}
	return true;
}