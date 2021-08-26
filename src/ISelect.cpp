#include "ISelect.h"

ISelect::ISelect()
{
}

void ISelect::GetTypeNameForType(const ItemTypeEnum type, wchar_t* const buf) const
{
	//UINT strID;

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