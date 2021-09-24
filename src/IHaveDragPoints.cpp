#include "IHaveDragPoints.h"
#include "DragPoint.h"
#include "mesh.h"

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

void IHaveDragPoints::GetRgVertex(std::vector<RenderVertex>& vv, const bool loop, const float accuracy) const
{
	const int cpoint = (int)m_vdpoint.size();
	const int endpoint = loop ? cpoint : cpoint - 1;

	RenderVertex rendv2;

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

		CatmullCurve<2> cc;
		cc.SetCurve(pdp0->m_v, pdp1->m_v, pdp2->m_v, pdp3->m_v);

		RenderVertex rendv1;

		rendv1.set(pdp1->m_v);
		rendv1.smooth = pdp1->m_smooth;
		rendv1.slingshot = pdp1->m_slingshot;
		rendv1.controlPoint = true;

		rendv2.set(pdp2->m_v);

		RecurseSmoothLine(cc, 0.f, 1.f, rendv1, rendv2, vv, accuracy);
	}

	if (!loop)
	{
		rendv2.smooth = true;
		rendv2.slingshot = false;
		rendv2.controlPoint = false;
		vv.push_back(rendv2);
	}
}

void IHaveDragPoints::GetRgVertex(std::vector<RenderVertex3D>& vv, const bool loop, const float accuracy) const
{
	const int cpoint = (int)m_vdpoint.size();
	const int endpoint = loop ? cpoint : cpoint - 1;

	RenderVertex3D rendv2;

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

		CatmullCurve<3> cc;
		cc.SetCurve(pdp0->m_v, pdp1->m_v, pdp2->m_v, pdp3->m_v);

		RenderVertex3D rendv1;

		rendv1.set(pdp1->m_v);
		rendv1.smooth = pdp1->m_smooth;
		rendv1.slingshot = pdp1->m_slingshot;
		rendv1.controlPoint = true;

		rendv2.set(pdp2->m_v);

		RecurseSmoothLine(cc, 0.f, 1.f, rendv1, rendv2, vv, accuracy);
	}

	if (!loop)
	{
		rendv2.smooth = true;
		rendv2.slingshot = false;
		rendv2.controlPoint = false;
		vv.push_back(rendv2);
	}
}

//
// license:GPLv3+
// Ported at: VisualPinball.Engine/Math/DragPoint.cs
//

void IHaveDragPoints::GetTextureCoords(const std::vector<RenderVertex>& vv, float** ppcoords)
{
	std::vector<int> vitexpoints;
	std::vector<int> virenderpoints;
	bool noCoords = false;

	const int cpoints = (int)vv.size();
	int icontrolpoint = 0;

	*ppcoords = new float[cpoints];
	memset(*ppcoords, 0, sizeof(float) * cpoints);

	for (int i = 0; i < cpoints; ++i)
	{
		const RenderVertex* const prv = &vv[i];
		if (prv->controlPoint)
		{
			if (!m_vdpoint[icontrolpoint]->m_autoTexture)
			{
				vitexpoints.push_back(icontrolpoint);
				virenderpoints.push_back(i);
			}
			++icontrolpoint;
		}
	}

	if (vitexpoints.empty())
	{
		vitexpoints.push_back(0);
		virenderpoints.push_back(0);

		noCoords = true;
	}

	vitexpoints.push_back(vitexpoints[0] + (int)m_vdpoint.size());
	virenderpoints.push_back(virenderpoints[0] + cpoints);

	for (int i = 0; i < (int)vitexpoints.size() - 1; ++i)
	{
		const int startrenderpoint = virenderpoints[i] % cpoints;
		int endrenderpoint = virenderpoints[(i < cpoints - 1) ? (i + 1) : 0] % cpoints;

		float starttexcoord;
		float endtexcoord;
		if (noCoords)
		{
			starttexcoord = 0.0f;
			endtexcoord = 1.0f;
		}
		else
		{
			starttexcoord = m_vdpoint[vitexpoints[i] % m_vdpoint.size()]->m_texturecoord;
			endtexcoord = m_vdpoint[vitexpoints[i + 1] % m_vdpoint.size()]->m_texturecoord;
		}

		const float deltacoord = endtexcoord - starttexcoord;

		if (endrenderpoint <= startrenderpoint)
		{
			endrenderpoint += cpoints;
		}

		float totallength = 0.0f;
		for (int l = startrenderpoint; l < endrenderpoint; ++l)
		{
			const Vertex2D* const pv1 = (Vertex2D*)&vv[l % cpoints];
			const Vertex2D* const pv2 = (Vertex2D*)&vv[(l + 1) % cpoints];

			const float dx = pv1->x - pv2->x;
			const float dy = pv1->y - pv2->y;
			const float length = sqrtf(dx * dx + dy * dy);

			totallength += length;
		}

		float partiallength = 0.0f;
		for (int l = startrenderpoint; l < endrenderpoint; ++l)
		{
			const Vertex2D* const pv1 = (Vertex2D*)&vv[l % cpoints];
			const Vertex2D* const pv2 = (Vertex2D*)&vv[(l + 1) % cpoints];

			const float dx = pv1->x - pv2->x;
			const float dy = pv1->y - pv2->y;
			const float length = sqrtf(dx * dx + dy * dy);
			if (totallength == 0.0f)
			{
				totallength = 1.0f;
			}
			const float texcoord = partiallength / totallength;

			(*ppcoords)[l % cpoints] = (texcoord * deltacoord) + starttexcoord;

			partiallength += length;
		}
	}
}

//
// end of license:GPLv3+, back to 'old MAME'-like
//
