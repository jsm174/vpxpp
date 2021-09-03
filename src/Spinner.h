#pragma once

#include "BaseProperty.h"
#include "IEditable.h"
#include "IScriptable.h"
#include "ISelect.h"
#include "vector.h"

#include "BiffReader.h"
#include "PinTable.h"
#include "RenderDevice.h"
#include "Timer.h"

class SpinnerData : public BaseProperty
{
public:
	Vertex2D m_vCenter;
	float m_length;
	float m_rotation;
	TimerDataRoot m_tdr;
	float m_height;
	float m_damping;
	float m_angleMax;
	float m_angleMin;
	std::string m_szSurface;
	bool m_showBracket;
};

class Spinner : public ISelect,
                public IEditable,
                public IScriptable
{
public:
	static const ItemTypeEnum ItemType;
	static const int TypeNameID;
	static const int ToolID;
	static const int CursorID;
	static const unsigned AllowedViews;

	static Spinner* COMCreate();
	static IEditable* COMCreateEditable();
	static IEditable* COMCreateAndInit(PinTable* ptable, float x, float y);

	Spinner();
	~Spinner();

	HRESULT Init(PinTable* ptable, float x, float y, bool fromMouseClick);

	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
	virtual PinTable* GetPTable();
	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual void SetDefaults(bool fromMouseClick);
	virtual void SetDefaultPhysics(bool fromMouseClick);
	virtual bool LoadToken(const int id, BiffReader* pBiffReader);

	virtual void WriteRegDefaults();

	SpinnerData m_d;

private:
	PinTable* m_ptable;

	VertexBuffer* m_bracketVertexBuffer;
	IndexBuffer* m_bracketIndexBuffer;
	VertexBuffer* m_plateVertexBuffer;
	IndexBuffer* m_plateIndexBuffer;

	float m_vertexBuffer_spinneranimangle;
};