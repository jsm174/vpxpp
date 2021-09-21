#include "IEditable.h"

IEditable::IEditable()
{
}

IEditable::~IEditable()
{
}

void IEditable::InitScript()
{
	/*if (!GetScriptable())
      return;

   if (GetScriptable()->m_wzName[0] == '\0')
      // Just in case something screws up - not good having a NULL script name
      swprintf_s(GetScriptable()->m_wzName, L"%d", (long)this);

   GetPTable()->m_pcv->AddItem(GetScriptable(), false);*/
}

IHitable* IEditable::GetIHitable()
{
	return NULL;
};

void IEditable::GetBoundingVertices(std::vector<Vertex3Ds>& pvvertex3D)
{
}
