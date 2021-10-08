#include "DragPoint.h"

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

PinTable* DragPoint::GetPTable()
{
	return m_pihdp->GetIEditable()->GetPTable();
}

const PinTable* DragPoint::GetPTable() const
{
	return m_pihdp->GetIEditable()->GetPTable();
}

IEditable* DragPoint::GetIEditable()
{
	return m_pihdp->GetIEditable();
}

const IEditable* DragPoint::GetIEditable() const
{
	return m_pihdp->GetIEditable();
}

/* TODO: STDMETHODIMP*/ HRESULT DragPoint::get_Y(float* pVal)
{
	*pVal = m_v.y;

	return S_OK;
}

/* TODO: STDMETHODIMP*/ HRESULT DragPoint::get_X(float* pVal)
{
	*pVal = m_v.x;
	return S_OK;
}
