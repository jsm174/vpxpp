#include "IEditable.h"
#include "IScriptable.h"
#include "PinTable.h"

#include <wchar.h>

IEditable::IEditable()
{
}

IEditable::~IEditable()
{
}

void IEditable::InitScript()
{
	if (!GetScriptable())
	{
		return;
	}

	if (GetScriptable()->m_wzName[0] == '\0')
	{
		swprintf(GetScriptable()->m_wzName, MAXNAMEBUFFER, L"%p", (void*)this);
	}

	GetPTable()->m_pCodeViewer->AddItem(GetScriptable(), false);
}

IHitable* IEditable::GetIHitable()
{
	return NULL;
};

const IHitable* IEditable::GetIHitable() const
{
	return NULL;
};

void IEditable::GetBoundingVertices(std::vector<Vertex3Ds>& pvvertex3D)
{
}
