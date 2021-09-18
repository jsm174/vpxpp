#pragma once

#include "IEditable.h"
#include "BiffReader.h"

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

	friend class DragPoint;

protected:
	template <typename T>
	void GetRgVertex(std::vector<T>& vv, const bool loop = true, const float accuracy = 4.f) const;

	std::vector<DragPoint*> m_vdpoint;
};