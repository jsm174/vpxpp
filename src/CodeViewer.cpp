#include "CodeViewer.h"

#include <iostream>

CodeViewer::CodeViewer()
{
}

CodeViewer::~CodeViewer()
{
}

void CodeViewer::LoadFromStream(POLE::Stream* pStream)
{
	int len;
	pStream->read((unsigned char*)&len, sizeof(int));

	char* pText = new char[len + 1];
	memset(pText, 0, len + 1);

	pStream->read((unsigned char*)pText, len * (int)sizeof(char));

	original_table_script.resize(len);
	memcpy(original_table_script.data(), pText, len);

	uint32_t state = CODEVIEWER_UTF8_ACCEPT;
	if (validate_utf8(&state, pText, len) == CODEVIEWER_UTF8_REJECT)
	{
		char* const utf8Text = iso8859_1_to_utf8(pText, len); // old ANSI characters? -> convert to UTF-8
		delete[] pText;
		pText = utf8Text;
	}

	std::cout << "Code = " << pText;

	delete[] pText;
}

HRESULT CodeViewer::AddItem(IScriptable* const piscript, const bool global)
{
	// TODO: CodeViewDispatch* const pcvd = new CodeViewDispatch();

	// CComBSTR bstr;
	// piscript->get_Name(&bstr);

	// pcvd->m_wName = bstr;
	// pcvd->m_pdisp = piscript->GetDispatch();
	// pcvd->m_pdisp->QueryInterface(IID_IUnknown, (void**)&pcvd->m_punk);
	// pcvd->m_punk->Release();
	// pcvd->m_piscript = piscript;
	// pcvd->m_global = global;

	// if (m_vcvd.GetSortedIndex(pcvd) != -1)
	// {
	// 	delete pcvd;
	// 	return E_FAIL;
	// }

	// m_vcvd.AddSortedString(pcvd);

	// char szT[MAXNAMEBUFFER * 2];
	// WideCharToMultiByteNull(CP_ACP, 0, pcvd->m_wName.c_str(), -1, szT, sizeof(szT), NULL, NULL);
	// const size_t index = SendMessage(m_hwndItemList, CB_ADDSTRING, 0, (size_t)szT);
	// SendMessage(m_hwndItemList, CB_SETITEMDATA, index, (size_t)piscript);

	return S_OK;
}