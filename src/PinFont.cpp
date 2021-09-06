#include "PinFont.h"

void PinFont::Register()
{
	// TODO: HDC hdcScreen = GetDC(NULL);

	// LOGFONT lf;
	// lf.lfCharSet = DEFAULT_CHARSET;
	// lstrcpy(lf.lfFaceName, "");
	// lf.lfPitchAndFamily = 0;

	// EnumFontFamiliesEx(hdcScreen, &lf, (FONTENUMPROC)EnumFontFamExProc, (size_t)this, 0);

	// ReleaseDC(NULL, hdcScreen);

	// char szPath[MAXSTRING];
	// GetModuleFileName(NULL, szPath, MAXSTRING);

	// char* szEnd = szPath + lstrlen(szPath);

	// while (szEnd > szPath)
	// {
	// 	if (*szEnd == '\\')
	// 		break;

	// 	szEnd--;
	// }

	// *(szEnd + 1) = '\0'; // Get rid of exe name

	// static int tempFontNumber = -1;
	// tempFontNumber++;

	// m_szTempFile = szPath + string("VPTemp") + std::to_string(tempFontNumber) + ".ttf";
	// WriteToFile(m_szTempFile);

	// /*const int fonts =*/AddFontResource(m_szTempFile.c_str());
}

void PinFont::UnRegister()
{
	// TODO: /*const BOOL foo =*/RemoveFontResource(m_szTempFile.c_str());

	// DeleteFile(m_szTempFile.c_str());
}
