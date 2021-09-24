#pragma once

#include <map>
#include <stdint.h>

#ifndef _MSC_VER
typedef uint32_t DWORD; // DWORD = unsigned 32 bit value
typedef uint16_t WORD;	// WORD = unsigned 16 bit value
typedef uint8_t BYTE;	// BYTE = unsigned 8 bit value
typedef int32_t LONG;
#else
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;

#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

typedef long HRESULT;
typedef int COLORREF;
typedef int64_t LONGLONG;
typedef unsigned int UINT;

typedef enum _D3DTRANSFORMSTATETYPE
{
	D3DTS_VIEW = 2,
	D3DTS_PROJECTION = 3,
	D3DTS_TEXTURE0 = 16,
	D3DTS_TEXTURE1 = 17,
	D3DTS_TEXTURE2 = 18,
	D3DTS_TEXTURE3 = 19,
	D3DTS_TEXTURE4 = 20,
	D3DTS_TEXTURE5 = 21,
	D3DTS_TEXTURE6 = 22,
	D3DTS_TEXTURE7 = 23,

	D3DTS_FORCE_DWORD = 0x7fffffff
} D3DTRANSFORMSTATETYPE;

#define D3DTS_WORLDMATRIX(index) (D3DTRANSFORMSTATETYPE)(index + 256)

#define D3DTS_WORLD D3DTS_WORLDMATRIX(0)

enum TransformStateType
{
	TRANSFORMSTATE_WORLD = D3DTS_WORLD,
	TRANSFORMSTATE_VIEW = D3DTS_VIEW,
	TRANSFORMSTATE_PROJECTION = D3DTS_PROJECTION
};

typedef union _LARGE_INTEGER
{
	struct
	{
		DWORD LowPart;
		LONG HighPart;
	};
	struct
	{
		DWORD LowPart;
		LONG HighPart;
	} u;
	LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;

typedef struct
{
	DWORD X;
	DWORD Y;
	DWORD Width;
	DWORD Height;
	float MinZ;
	float MaxZ;
} D3DVIEWPORT9;

typedef D3DVIEWPORT9 ViewPort;

typedef struct _D3DMATRIX
{
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
} D3DMATRIX;

typedef struct D3DXFLOAT16
{
	WORD Value;
} D3DXFLOAT16, *LPD3DXFLOAT16;

typedef struct D3DXMATRIX : public D3DMATRIX
{
public:
	D3DXMATRIX(){};
	D3DXMATRIX(const float*);
	D3DXMATRIX(const D3DMATRIX&);
	D3DXMATRIX(const D3DXFLOAT16*);
	D3DXMATRIX(float _11, float _12, float _13, float _14,
			   float _21, float _22, float _23, float _24,
			   float _31, float _32, float _33, float _34,
			   float _41, float _42, float _43, float _44);

	// access grants
	float& operator()(UINT Row, UINT Col);
	float operator()(UINT Row, UINT Col) const;

	// casting operators
	operator float*();
	operator const float*() const;

	// assignment operators
	D3DXMATRIX& operator*=(const D3DXMATRIX&);
	D3DXMATRIX& operator+=(const D3DXMATRIX&);
	D3DXMATRIX& operator-=(const D3DXMATRIX&);
	D3DXMATRIX& operator*=(float);
	D3DXMATRIX& operator/=(float);

	// unary operators
	D3DXMATRIX operator+() const;
	D3DXMATRIX operator-() const;

	// binary operators
	D3DXMATRIX operator*(const D3DXMATRIX&) const;
	D3DXMATRIX operator+(const D3DXMATRIX&) const;
	D3DXMATRIX operator-(const D3DXMATRIX&) const;
	D3DXMATRIX operator*(float) const;
	D3DXMATRIX operator/(float) const;

	friend D3DXMATRIX operator*(float, const D3DXMATRIX&);

	bool operator==(const D3DXMATRIX&) const;
	bool operator!=(const D3DXMATRIX&) const;

} D3DXMATRIX, *LPD3DXMATRIX;

#define S_OK ((HRESULT)0L)
#define S_FALSE ((HRESULT)1L)
#define E_FAIL ((HRESULT)0x80004005L)

#define NUM_BG_SETS 3
#define BG_DESKTOP 0
#define BG_FULLSCREEN 1
#define BG_FSS 2

#define MIN_TEXTURE_SIZE 8

#define MAX_LIGHT_SOURCES 2
#define LIGHTSEQQUEUESIZE 100
#define MAXTIPSHAPE 256

#define MAX_OPEN_TABLES 9

#define NEW_SOUND_FORMAT_VERSION 1031

#define FID(A) (int)((unsigned int)(#A[0]) | ((unsigned int)(#A[1]) << 8) | ((unsigned int)(#A[2]) << 16) | ((unsigned int)(#A[3]) << 24))

#define RGB(r, g, b) ((COLORREF)(((DWORD)(r)) | (((DWORD)(g)) << 8) | (((DWORD)(b)) << 16)))

// TODO: #define MAX(a,b)        (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

enum eObjType : unsigned char
{
	eNull,
	ePoint,
	eLineSeg,
	eLineSegSlingshot,
	eJoint,
	eCircle,
	eFlipper,
	ePlunger,
	eSpinner,
	eBall,
	e3DPoly,
	eTriangle,
	ePlane,
	e3DLine,
	eGate,
	eTextbox,
	eDispReel,
	eLightSeq,
	ePrimitive,
	eHitTarget,
	eTrigger,
	eKicker,
};

enum ItemTypeEnum
{
	eItemSurface,
	eItemFlipper,
	eItemTimer,
	eItemPlunger,
	eItemTextbox,
	eItemBumper,
	eItemTrigger,
	eItemLight,
	eItemKicker,
	eItemDecal,
	eItemGate,
	eItemSpinner,
	eItemRamp,
	eItemTable,
	eItemLightCenter,
	eItemDragPoint,
	eItemCollection,
	eItemDispReel,
	eItemLightSeq,
	eItemPrimitive,
	eItemFlasher,
	eItemRubber,
	eItemHitTarget,
	eItemTypeCount,
	eItemInvalid = 0xffffffff
};

static const std::map<ItemTypeEnum, const char*> ITEMTYPEENUM_STRING{
	{eItemSurface, "eItemSurface"},
	{eItemFlipper, "eItemFlipper"},
	{eItemTimer, "eItemTimer"},
	{eItemPlunger, "eItemPlunger"},
	{eItemTextbox, "eItemTextbox"},
	{eItemBumper, "eItemBumper"},
	{eItemTrigger, "eItemTrigger"},
	{eItemLight, "eItemLight"},
	{eItemKicker, "eItemKicker"},
	{eItemDecal, "eItemDecal"},
	{eItemGate, "eItemGate"},
	{eItemSpinner, "eItemSpinner"},
	{eItemRamp, "eItemRamp"},
	{eItemTable, "eItemTable"},
	{eItemLightCenter, "eItemLightCenter"},
	{eItemDragPoint, "eItemDragPoint"},
	{eItemCollection, "eItemCollection"},
	{eItemDispReel, "eItemDispReel"},
	{eItemLightSeq, "eItemLightSeq"},
	{eItemPrimitive, "eItemPrimitive"},
	{eItemFlasher, "eItemFlasher"},
	{eItemRubber, "eItemRubber"},
	{eItemHitTarget, "eItemHitTarget"},
};

typedef enum
{
	DecalText = 0,
	DecalImage = 1
} DecalType;

typedef enum
{
	AutoSize = 0,
	AutoWidth = 1,
	ManualSize = 2
} SizingType;

typedef enum
{
	Filter_None = 0,
	Filter_Additive = 1,
	Filter_Overlay = 2,
	Filter_Multiply = 3,
	Filter_Screen = 4,
} Filters;

typedef enum
{
	ImageModeWorld = 0,
	ImageModeWrap = 1,
} RampImageAlignment;

typedef enum
{
	RampTypeFlat = 0,
	RampType4Wire = 1,
	RampType2Wire = 2,
	RampType3WireLeft = 3,
	RampType3WireRight = 4,
	RampType1Wire = 5,
} RampType;

typedef enum
{
	GateWireW = 1,
	GateWireRectangle = 2,
	GatePlate = 3,
	GateLongPlate = 4,
} GateType;

typedef enum
{
	KickerInvisible = 0,
	KickerHole = 1,
	KickerCup = 2,
	KickerHoleSimple = 3,
	KickerWilliams = 4,
	KickerGottlieb = 5,
	KickerCup2 = 6,
} KickerType;

typedef enum
{
	LightStateOff = 0,
	LightStateOn = 1,
	LightStateBlinking = 2,
} LightState;

typedef enum
{
	SeqUpOn = 1,
	SeqUpOff = 2,
	SeqDownOn = 3,
	SeqDownOff = 4,
	SeqRightOn = 5,
	SeqRightOff = 6,
	SeqLeftOn = 7,
	SeqLeftOff = 8,
	SeqDiagUpRightOn = 9,
	SeqDiagUpRightOff = 10,
	SeqDiagUpLeftOn = 11,
	SeqDiagUpLeftOff = 12,
	SeqDiagDownRightOn = 13,
	SeqDiagDownRightOff = 14,
	SeqDiagDownLeftOn = 15,
	SeqDiagDownLeftOff = 16,
	SeqMiddleOutHorizOn = 17,
	SeqMiddleOutHorizOff = 18,
	SeqMiddleInHorizOn = 19,
	SeqMiddleInHorizOff = 20,
	SeqMiddleOutVertOn = 21,
	SeqMiddleOutVertOff = 22,
	SeqMiddleInVertOn = 23,
	SeqMiddleInVertOff = 24,
	SeqStripe1HorizOn = 25,
	SeqStripe1HorizOff = 26,
	SeqStripe2HorizOn = 27,
	SeqStripe2HorizOff = 28,
	SeqStripe1VertOn = 29,
	SeqStripe1VertOff = 30,
	SeqStripe2VertOn = 31,
	SeqStripe2VertOff = 32,
	SeqHatch1HorizOn = 33,
	SeqHatch1HorizOff = 34,
	SeqHatch2HorizOn = 35,
	SeqHatch2HorizOff = 36,
	SeqHatch1VertOn = 37,
	SeqHatch1VertOff = 38,
	SeqHatch2VertOn = 39,
	SeqHatch2VertOff = 40,
	SeqCircleOutOn = 41,
	SeqCircleOutOff = 42,
	SeqCircleInOn = 43,
	SeqCircleInOff = 44,
	SeqClockRightOn = 45,
	SeqClockRightOff = 46,
	SeqClockLeftOn = 47,
	SeqClockLeftOff = 48,
	SeqRadarRightOn = 49,
	SeqRadarRightOff = 50,
	SeqRadarLeftOn = 51,
	SeqRadarLeftOff = 52,
	SeqWiperRightOn = 53,
	SeqWiperRightOff = 54,
	SeqWiperLeftOn = 55,
	SeqWiperLeftOff = 56,
	SeqFanLeftUpOn = 57,
	SeqFanLeftUpOff = 58,
	SeqFanLeftDownOn = 59,
	SeqFanLeftDownOff = 60,
	SeqFanRightUpOn = 61,
	SeqFanRightUpOff = 62,
	SeqFanRightDownOn = 63,
	SeqFanRightDownOff = 64,
	SeqArcBottomLeftUpOn = 65,
	SeqArcBottomLeftUpOff = 66,
	SeqArcBottomLeftDownOn = 67,
	SeqArcBottomLeftDownOff = 68,
	SeqArcBottomRightUpOn = 69,
	SeqArcBottomRightUpOff = 70,
	SeqArcBottomRightDownOn = 71,
	SeqArcBottomRightDownOff = 72,
	SeqArcTopLeftUpOn = 73,
	SeqArcTopLeftUpOff = 74,
	SeqArcTopLeftDownOn = 75,
	SeqArcTopLeftDownOff = 76,
	SeqArcTopRightUpOn = 77,
	SeqArcTopRightUpOff = 78,
	SeqArcTopRightDownOn = 79,
	SeqArcTopRightDownOff = 80,
	SeqScrewRightOn = 81,
	SeqScrewRightOff = 82,
	SeqScrewLeftOn = 83,
	SeqScrewLeftOff = 84,
	SeqLastDynamic = 85,
	SeqAllOff = 1000,
	SeqAllOn = 1001,
	SeqBlinking = 1002,
	SeqRandom = 1003,
} SequencerState;

typedef enum
{
	PlungerTypeModern = 1,
	PlungerTypeFlat = 2,
	PlungerTypeCustom = 3,
} PlungerType;

typedef enum
{
	TextAlignLeft = 0,
	TextAlignCenter = 1,
	TextAlignRight = 2,
} TextAlignment;

typedef enum
{
	TriggerNone = 0,
	TriggerWireA = 1,
	TriggerStar = 2,
	TriggerWireB = 3,
	TriggerButton = 4,
	TriggerWireC = 5,
	TriggerWireD = 6,
	TriggerInder = 7,
} TriggerShape;

typedef enum
{
	DropTargetBeveled = 1,
	DropTargetSimple = 2,
	HitTargetRound = 3,
	HitTargetRectangle = 4,
	HitFatTargetRectangle = 5,
	HitFatTargetSquare = 6,
	DropTargetFlatSimple = 7,
	HitFatTargetSlim = 8,
	HitTargetSlim = 9,
} TargetType;

typedef enum : char
{
	SNDOUT_TABLE = 0,
	SNDOUT_BACKGLASS = 1
} SoundOutTypes;

typedef enum
{
	RGBA,
	RGB_FP
} TextureFormat;

typedef enum
{
	Defaults = -1,
	Disabled = 0,
	Fast_FXAA = 1,
	Standard_FXAA = 2,
	Quality_FXAA = 3,
	Fast_NFAA = 4,
	Standard_DLAA = 5,
	Quality_SMAA = 6,
} FXAASettings;

#pragma pack(push, 1)
typedef struct tWAVEFORMATEX
{
	WORD wFormatTag;
	WORD nChannels;
	DWORD nSamplesPerSec;
	DWORD nAvgBytesPerSec;
	WORD nBlockAlign;
	WORD wBitsPerSample;
	WORD cbSize;
} WAVEFORMATEX, *PWAVEFORMATEX, *LPWAVEFORMATEX;
typedef const WAVEFORMATEX* LPCWAVEFORMATEX;
#pragma pack(pop)

#define MY_D3DFVF_TEX 0
#define MY_D3DFVF_NOTEX2_VERTEX 1
#define MY_D3DTRANSFORMED_NOTEX2_VERTEX 2 // TODO: DELETE?

#define DISPID_SurfaceEvents_Slingshot 1101