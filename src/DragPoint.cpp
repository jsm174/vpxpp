#include "DragPoint.h"
#include "IHaveDragPoints.h"

void DragPoint::Init(IHaveDragPoints* pihdp, const float x, const float y, const float z, const bool smooth)
{
	m_pihdp = pihdp;
	m_smooth = smooth;

	m_slingshot = false;
	m_v.x = x;
	m_v.y = y;
	m_v.z = z;
	m_calcHeight = 0.0f;
	m_autoTexture = true;
	m_texturecoord = 0.0f;

	// TODO: m_menuid = (pihdp->GetIEditable()->GetItemType() == eItemRubber) ? IDR_POINTMENU_SMOOTH : IDR_POINTMENU;
}

PinTable* DragPoint::GetPTable()
{
	return m_pihdp->GetIEditable()->GetPTable();
}

bool DragPoint::LoadToken(const int id, BiffReader* const pBiffReader)
{
	switch (id)
	{
	case FID(VCEN):
		pBiffReader->GetStruct(&m_v, sizeof(Vertex2D));
		break;
	case FID(POSZ):
		pBiffReader->GetFloat(m_v.z);
		break;
	case FID(SMTH):
		pBiffReader->GetBool(m_smooth);
		break;
	case FID(SLNG):
		pBiffReader->GetBool(m_slingshot);
		break;
	case FID(ATEX):
		pBiffReader->GetBool(m_autoTexture);
		break;
	case FID(TEXC):
		pBiffReader->GetFloat(m_texturecoord);
		break;
	default:
		ISelect::LoadToken(id, pBiffReader);
		break;
	}
	return true;
}

IEditable* DragPoint::GetIEditable()
{
	return m_pihdp->GetIEditable();
}
