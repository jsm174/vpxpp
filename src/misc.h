#pragma once

typedef long HRESULT;

#define S_OK ((HRESULT)0L)
#define S_FALSE ((HRESULT)1L)
#define E_FAIL ((HRESULT)0x80004005L)

#define NUM_BG_SETS 3 // different backdrop settings: DT,FS,FSS
#define BG_DESKTOP 0
#define BG_FULLSCREEN 1
#define BG_FSS 2

typedef int COLORREF;

#define MAX_LIGHT_SOURCES 2