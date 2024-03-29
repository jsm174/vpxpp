#pragma once

#include "IEditable.h"
#include "ISelect.h"

#include "BiffReader.h"
#include "IHaveDragPoints.h"

class DragPoint : public ISelect
{
public:
	void Init(IHaveDragPoints* pihdp, const float x, const float y, const float z, const bool smooth);

	virtual bool LoadToken(const int id, BiffReader* const pBiffReader);

	virtual PinTable* GetPTable();
	virtual const PinTable* GetPTable() const;
	virtual IEditable* GetIEditable();
	virtual const IEditable* GetIEditable() const;

	HRESULT get_Y(float* pVal);
	HRESULT get_X(float* pVal);

	Vertex3Ds m_v;
	float m_calcHeight;

	float m_texturecoord;
	IHaveDragPoints* m_pihdp;
	bool m_smooth;
	bool m_slingshot;
	bool m_autoTexture;
};