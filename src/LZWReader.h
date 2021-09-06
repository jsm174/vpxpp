#pragma once

#include "misc.h"
#include "pole.h"

#define MAX_CODES 4095
#define FILE_BUF_SIZE 4096

class LZWReader
{
public:
	LZWReader(POLE::Stream* pStream, int* bits, int width, int height, int pitch);
	~LZWReader();

	short Decoder();

private:
	short init_exp(int size);
	int get_next_code();
	int get_byte();
	BYTE* NextLine();

	POLE::Stream* m_pStream;

	BYTE* m_pbBitsOutCur;
	int m_cbStride;

	int curr_size;
	int clear;
	int ending;
	int newcodes;
	int top_slot;
	int slot;

	int navail_bytes;
	int nbits_left;
	BYTE b1;
	BYTE byte_buff[257];
	BYTE* pbytes;

	BYTE stack[MAX_CODES + 1];
	BYTE suffix[MAX_CODES + 1];
	wchar_t prefix[MAX_CODES + 1];

	int m_cfilebuffer;
	BYTE m_pfilebufferbytes[FILE_BUF_SIZE];

	int m_width;
	int m_height;
	int m_linesleft;

	unsigned long m_readahead;
};
