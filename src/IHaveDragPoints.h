#pragma once

#include "BiffReader.h"
#include "IEditable.h"

#include "RenderVertex.h"
#include "RenderVertex3D.h"

#include <vector>

class DragPoint;

class IHaveDragPoints
{
public:
	IHaveDragPoints();
	virtual ~IHaveDragPoints();

	virtual IEditable* GetIEditable() = 0;
	virtual PinTable* GetPTable() = 0;

	virtual void LoadPointToken(int id, BiffReader* pBiffReader, int version);

	void GetTextureCoords(const std::vector<RenderVertex>& vv, float** ppcoords);

	friend class DragPoint;

protected:
	void GetRgVertex(std::vector<RenderVertex>& vv, const bool loop = true, const float accuracy = 4.f) const;
	void GetRgVertex(std::vector<RenderVertex3D>& vv, const bool loop = true, const float accuracy = 4.f) const;

	std::vector<DragPoint*> m_vdpoint;
};