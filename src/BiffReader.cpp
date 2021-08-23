#include "BiffReader.h"

BiffReader::BiffReader(POLE::Stream* pStream, ILoadable* pLoadable, void* pData, const int version)
{
    m_pStream = pStream;
    m_pLoadable = pLoadable;
    m_pData = pData;
    m_version = version;

    m_bytesInRecordRemaining = 0;
}

BiffReader::~BiffReader()
{
}

HRESULT BiffReader::ReadBytes(void* pValue, const unsigned long count)
{
   return m_pStream->read((unsigned char*)pValue, count) != 0 ? S_OK : S_FALSE;
}

HRESULT BiffReader::GetIntNoHash(int& value)
{
    m_bytesInRecordRemaining -= sizeof(int);

    return m_pStream->read((unsigned char*)&value, sizeof(int)) != 0 ? S_OK : S_FALSE;
}

HRESULT BiffReader::GetInt(void* pValue)
{
   m_bytesInRecordRemaining -= sizeof(int);

   return ReadBytes(pValue, sizeof(int));
}

HRESULT BiffReader::GetInt(int& value)
{
    m_bytesInRecordRemaining -= sizeof(int);

    return ReadBytes((void*)&value, sizeof(int));
}

HRESULT BiffReader::GetString(std::string &szvalue)
{
    HRESULT hr;
    int len;

    hr = ReadBytes(&len, sizeof(int));

    if (hr != S_OK)
    {
        szvalue.clear();
        return hr;
    }

    m_bytesInRecordRemaining -= len + (int)sizeof(int);

    char* pTmp = new char[len+1];
    hr = ReadBytes(pTmp, len);
    pTmp[len] = 0;
    szvalue = pTmp;
    delete[] pTmp;

    return hr;
}

HRESULT BiffReader::GetWideString(wchar_t* wzvalue, int maxlen)
{
   HRESULT hr;
   int len;

   hr = ReadBytes(&len, sizeof(int));

   if (hr != S_OK)
   {
      wzvalue[0] = 0;
      return hr;
   }

   m_bytesInRecordRemaining -= len + (int)sizeof(int);

   len /= 2;

   wchar_t* pTmp = new wchar_t[len + 1];
   memset(pTmp, 0, sizeof(wchar_t) * (len + 1));

   char* pPtr = (char*)pTmp;

   for (int index = 0; index < len; index++) {
        hr = ReadBytes(pPtr, 2);
        pPtr += sizeof(wchar_t);
   }

   wcsncpy(wzvalue, pTmp, maxlen);  
   delete[] pTmp;

   return hr;
}

HRESULT BiffReader::GetWideString(std::wstring& wzvalue)
{
   HRESULT hr;
   int len;

   hr = ReadBytes(&len, sizeof(int));

   if (hr != S_OK)
   {
      wzvalue.clear();
      return hr;
   }

   m_bytesInRecordRemaining -= len + (int)sizeof(int);

   len /= 2;

   wchar_t* pTmp = new wchar_t[len + 1];
   memset(pTmp, 0, sizeof(wchar_t) * (len + 1));

   char* pPtr = (char*)pTmp;

   for (int index = 0; index < len; index++) {
        hr = ReadBytes(pPtr, 2);
        pPtr += sizeof(wchar_t);
   }

   wzvalue = pTmp;
   delete[] pTmp;
   
   return hr;
}

HRESULT BiffReader::GetFloat(float& value)
{
   m_bytesInRecordRemaining -= sizeof(float);

   return ReadBytes(&value, sizeof(float));
}

HRESULT BiffReader::GetBool(bool& value)
{
   m_bytesInRecordRemaining -= sizeof(bool);

   return ReadBytes(&value, sizeof(bool));
}

HRESULT BiffReader::GetStruct(void* pValue, const int size)
{
   m_bytesInRecordRemaining -= size;

   return ReadBytes(pValue, size);
}

HRESULT BiffReader::Load()
{
    int tag = 0;
    while (tag != FID(ENDB))
    {
        if (m_version > 30)
        {
            /*const HRESULT hr =*/ GetIntNoHash(m_bytesInRecordRemaining);
        }

        const HRESULT hr = GetInt(tag);

        bool cont = false;
        
        if (hr == S_OK)
        {
            cont = m_pLoadable->LoadToken(tag, this);
        }

        if (!cont)
        {
            return E_FAIL;
        }
        
        if (m_version > 30)
        {
            if (m_bytesInRecordRemaining > 0)
            {
                unsigned char* pStruct = new unsigned char[m_bytesInRecordRemaining];
                GetStruct(pStruct, m_bytesInRecordRemaining);
                delete[] pStruct;
            }
        }
    }

    return S_OK;
}