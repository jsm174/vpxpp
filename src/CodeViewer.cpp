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

	uint32_t state = UTF8_ACCEPT;
	if (validate_utf8(&state, pText, len) == UTF8_REJECT) {
		char* const utf8Text = iso8859_1_to_utf8(pText, len); // old ANSI characters? -> convert to UTF-8
		delete[] pText;
		pText = utf8Text;
	}

	std::cout << "Code = " << pText;

	delete[] pText;
}