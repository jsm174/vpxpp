#pragma once

class BiffReader;

class ILoadable
{
public:
   virtual bool LoadToken(const int id, BiffReader* pBiffReader) = 0;
};