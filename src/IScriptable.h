#pragma once

#include "def.h"

class IScriptable
{
   public:
      IScriptable();

      wchar_t m_wzName[MAXNAMEBUFFER];
};