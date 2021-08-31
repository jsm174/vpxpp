#pragma once

#include "BaseProperty.h"
#include "ISelect.h"
#include "IEditable.h"
#include "vector.h"

#include "PinTable.h"
#include "Timer.h"

class LightData : public BaseProperty
{
public:
   Vertex2D m_vCenter;
   float m_falloff;
   float m_falloff_power;
   float m_intensity;
   float m_intensity_scale;
   float m_fadeSpeedUp;
   float m_fadeSpeedDown;
   uint32_t m_time_msec;
   float m_currentIntensity;
   float m_transmissionScale;
   float m_modulate_vs_add;
   float m_meshRadius;
   LightState m_state;
   COLORREF m_color;
   COLORREF m_color2; 
   TimerDataRoot m_tdr;
   //TODO: Shape m_shape;
   std::string m_szSurface;

   float m_depthBias;
   float m_bulbHaloHeight;

   bool m_imageMode;  
   bool m_BulbLight;
   bool m_showBulbMesh;
   bool m_showReflectionOnBall;
   bool m_staticBulbMesh;
};

class Light : public ISelect,
               public IEditable
{
public:
	static const ItemTypeEnum ItemType;
	static const int TypeNameID;
	static const int ToolID;
	static const int CursorID;
	static const unsigned AllowedViews;

	static Light* COMCreate();
	static IEditable* COMCreateEditable();
	static IEditable* COMCreateAndInit(PinTable* ptable, float x, float y);

	Light();
	~Light();

	HRESULT Init(PinTable* ptable, float x, float y, bool fromMouseClick);

	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
	virtual PinTable* GetPTable();
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);

	LightData m_d;

private:
	PinTable* m_ptable;
};