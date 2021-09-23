#include "IHaveDragPoints.h"
#include "DragPoint.h"

IHaveDragPoints::IHaveDragPoints()
{
	// TODO: m_propVisuals = NULL;
	// TODO: m_propPosition = NULL;
}

IHaveDragPoints::~IHaveDragPoints()
{
	for (size_t i = 0; i < m_vdpoint.size(); i++)
	{
		delete m_vdpoint[i];
	}
}

void IHaveDragPoints::LoadPointToken(int id, BiffReader* pBiffReader, int version)
{
	if (id == FID(DPNT))
	{
		DragPoint* pdp = new DragPoint();

		// TODO: CComObject<DragPoint>::CreateInstance(&pdp);

		pdp->Init(this, 0.f, 0.f, 0.f, false);

		m_vdpoint.push_back(pdp);

		BiffReader biffReader(pBiffReader->m_pStream, pdp, NULL, version);
		biffReader.Load();

		int i = 5;
	}
}

template <typename T>
void IHaveDragPoints::GetRgVertex(std::vector<T>& vv, const bool loop, const float accuracy) const
{
	static const int Dim = T::Dim;

	const int cpoint = (int)m_vdpoint.size();
	const int endpoint = loop ? cpoint : cpoint - 1;

	T rendv2;

	for (int i = 0; i < endpoint; i++)
	{
		const DragPoint* const pdp1 = m_vdpoint[i];
		const DragPoint* const pdp2 = m_vdpoint[(i < cpoint - 1) ? (i + 1) : 0];

		if ((pdp1->m_v.x == pdp2->m_v.x) && (pdp1->m_v.y == pdp2->m_v.y) && (pdp1->m_v.z == pdp2->m_v.z))
		{
			continue;
		}

		int iprev = (pdp1->m_smooth ? i - 1 : i);
		if (iprev < 0)
		{
			iprev = (loop ? cpoint - 1 : 0);
		}

		int inext = (pdp2->m_smooth ? i + 2 : i + 1);
		if (inext >= cpoint)
		{
			inext = (loop ? inext - cpoint : cpoint - 1);
		}

		const DragPoint* const pdp0 = m_vdpoint[iprev];
		const DragPoint* const pdp3 = m_vdpoint[inext];

		// TODO: CatmullCurve<Dim> cc;
		// TODO: cc.SetCurve(pdp0->m_v, pdp1->m_v, pdp2->m_v, pdp3->m_v);

		T rendv1;

		rendv1.set(pdp1->m_v);
		rendv1.smooth = pdp1->m_smooth;
		rendv1.slingshot = pdp1->m_slingshot;
		rendv1.controlPoint = true;

		rendv2.set(pdp2->m_v);

		// TODO: RecurseSmoothLine(cc, 0.f, 1.f, rendv1, rendv2, vv, accuracy);
	}

	if (!loop)
	{
		rendv2.smooth = true;
		rendv2.slingshot = false;
		rendv2.controlPoint = false;
		vv.push_back(rendv2);
	}
}

template void IHaveDragPoints::GetRgVertex<RenderVertex>(std::vector<RenderVertex>& vv, const bool loop, const float accuracy) const;
template void IHaveDragPoints::GetRgVertex<RenderVertex3D>(std::vector<RenderVertex3D>& vv, const bool loop, const float accuracy) const;