#if !defined(_ILOADABLE_H__)
#define _ILOADABLE_H__

class BiffReader;

class ILoadable
{
public:
   virtual bool LoadToken(const int id, BiffReader * const pbr) = 0;
};

#endif