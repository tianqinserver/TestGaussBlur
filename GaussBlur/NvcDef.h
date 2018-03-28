//==========================================================================;
//
// (c) Copyright NewAuto Video Technology CO.,Ltd., 2008. All rights reserved. 
//
// This code and information is provided "as is" without warranty of any kind, 
// either expressed or implied, including but not limited to the implied 
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------;
//   Birth Date:       Aug 1. 2008
//   Author:           NewAuto video team
//----------------------------------------------------------------
//   
//   Beijing China. 
//----------------------------------------------------------------

#pragma once

#include "NvcTypes.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include "IUnknown.h"

//
// architecture related
//

#if defined(_MSC_VER)
	#ifndef _WIN64
		#define NVC_ARCH_BITS	32
	#else
		#define NVC_ARCH_BITS	64
	#endif
#elif defined(__GNUC__)
	#define NVC_ARCH_BITS	__SIZEOF_SIZE_T__
#else
	#error unsupported compiler!
#endif

//
// calling convention related
//
#define NVC_CDECL					__cdecl
#define NVC_STDCALL					__stdcall
#define NVCAPI						NVC_CDECL

//
// memory align related
//
#define NVC_CACHELINE_SIZE			128

#ifdef _MSC_VER
#define NVC_MEMORY_ALIGN(A)			__declspec(align(A))
#else
#define NVC_MEMORY_ALIGN(A)
#endif

#define NVC_CACHE_ALIGN				NVC_MEMORY_ALIGN(NVC_CACHELINE_SIZE)

#define NVC_UP_ALIGN(N, STEP)		(((N) + (STEP) - 1) / (STEP) * (STEP))

// Warning: you must insure that STEP is power of 2 when using the following macro
#define NVC_UP_ALIGN_PO2(N, STEP)	(((N) + (STEP) - 1) & (~((STEP) - 1)))

// forced inline
#define NVC_FORCE_INLINE			__forceinline


//
// arithmetic related
//
#ifndef NVC_MAX
#define NVC_MAX(a,b)		(((a) > (b)) ? (a) : (b))
#endif

#ifndef NVC_MIN
#define NVC_MIN(a,b)		(((a) < (b)) ? (a) : (b))
#endif

#ifndef NVC_ABS
#define NVC_ABS(x)			((x) >= 0 ? (x) : -(x))
#endif

#define NVC_LIMIT_VALUE(v, min, max)		((v) >= (min) ? ((v) <= (max) ? (v) : (max)) : (min))

#define NVC_PIE			3.14159265358979323846
#define NVC_E			2.71828182845904523536
#define NVC_LOG2E		1.44269504088896340736
#define NVC_LOG10E		0.434294481903251827651
#define NVC_LN2			0.693147180559945309417
#define NVC_LN10		2.30258509299404568402
#define NVC_PI			3.14159265358979323846
#define NVC_2PI			6.28318530717958647692
#define NVC_PI_2		1.57079632679489661923
#define NVC_PI_3		1.04719755119659774615
#define NVC_PI_4		0.785398163397448309616
#define NVC_1_PI		0.318309886183790671538
#define NVC_2_PI		0.636619772367581343076
#define NVC_2_SQRTPI	1.12837916709551257390
#define NVC_SQRT2		1.41421356237309504880
#define NVC_SQRT1_2		0.707106781186547524401


//
// file related definition
//

#define NVC_MAX_PATH			260
#define NVC_MAX_DRV				3
#define NVC_MAX_DIR				256
#define NVC_MAX_FILENAME		256
#define NVC_MAX_EXTNAME			256

//
// misc
//
typedef	void *NVC_POSTION;

#define	NVC_GET_CONTAINER(PTR, TYPE, MEMBER) \
	((TYPE *)((char *)(PTR) - (size_t)(&((TYPE *)0)->MEMBER)))

#define	NVC_GET_OFFSET(TYPE, MEMBER) (size_t)(&((TYPE *)0)->MEMBER)

#define NVC_GUID_STRING_LENGTH	39


#define NVC_LOW_WORD(dw)		((uint16_t)(dw))
#define NVC_HIGH_WORD(dw)		((uint16_t)((uint32_t)(dw) >> 16))
#define NVC_SWAP_WORD(w)		((uint16_t)(((uint16_t)(w) << 8) | ((uint16_t)(w) >> 8)))
#define NVC_SWAP_DWORD(dw)		(((uint32_t)(NVC_SWAP_WORD(NVC_LOW_WORD(dw))) << 16) | (uint32_t)(NVC_SWAP_WORD(NVC_HIGH_WORD(dw))))


#define NVC_PRODUCT_PATH_ENVNAME		"NvProduct"
#define NVC_PRODUCT_APP_DIR_NAME		"System"
#define NVC_PRODUCT_SHADER_DIR_NAME		"System/Shaders"
#define NVC_PRODUCT_CGPLUGIN_DIR_NAME	"System/CGPlugin/bin"
#define NVC_PRODUCT_LUT_DIR_NAME		"System/LUT"

#define  NVC_MAX_IMAGEFILE_INDEX_WIDTH  9

#define  NVC_MAX_BUFFER_PLANE_COUNT  4


enum ENvcImageOrigin
{
	keNvcImageOriginInvalid,
	keNvcImageOriginTopLeft,
	keNvcImageOriginBottomLeft,
	keNvcImageOriginLast
};

enum ENvcColorimetry
{
	keNvcColorimetryInvalid = -1,
	keNvcColorimetryITUR_BT_601,	
	keNvcColorimetryITUR_BT_709,
	keNvcColorimetryITUR_BT_2020,
	keNvcColorimetryLast
};

/** 
* ScanMode
*/
enum ENvcScanMode
{
	keNvcScanModeInvalid,
	keNvcScanModeFirstFieldTop,
	keNvcScanModeSecondFieldTop,
	keNvcScanModeProgressive,
	keNvcScanModeProgressiveSegmented,
	keNvcScanModeLast
};

/**
* AspectRatio
*/
enum ENvcAspectRatio
{
	keNvcAspectRatioInvalid,
	keNvcAspectRatio_4_3,
	keNvcAspectRatio_16_9,
	keNvcAspectRatio_1_1,
	keNvcAspectRatio_Custom,
	keNvcAspectRatioLast
};

//////////////////////////////////////////////////////////////////////////
// aspect ratio
//////////////////////////////////////////////////////////////////////////
struct SNvcAspectRatio 
{
	double x;
	double y;
};



/**
* FrameRate
*/
enum ENvcFrameRate
{
	keNvcFrameRateInvalid,
	keNvcFrameRate24,
	keNvcFrameRate24M,
	keNvcFrameRate25,
	keNvcFrameRate30,
	keNvcFrameRate30M,
	keNvcFrameRate50,
	keNvcFrameRate60,
	keNvcFrameRate60M,
	keNvcFrameRateVariable,
	keNvcFrameRateLast	
};

static inline double NvcGetFrameRateValue(ENvcFrameRate frameRate)
{
    switch (frameRate) {
    case keNvcFrameRate24:
        return 24;
    case keNvcFrameRate24M:
        return 24000.0 / 1001.0;
    case keNvcFrameRate25:
        return 25;
    case keNvcFrameRate30:
        return 30;
    case keNvcFrameRate30M:
        return 30000.0 / 1001.0;
    case keNvcFrameRate50:
        return 50;
    case keNvcFrameRate60:
        return 60;
    case keNvcFrameRate60M:
        return 60000.0 /1001.0;
    default:
        return 25;
    }
}

static inline void NvcGetFrameRateIntValue(ENvcFrameRate frameRate, int* out_pnRate, int* out_pnSacle)
{
	int nRate = 25;
	int nScale = 1;
	switch (frameRate) {
	case keNvcFrameRate24:
		nRate = 24;
		break;
	case keNvcFrameRate24M:
		nRate = 24000;
		nScale = 1001;
		break;
	case keNvcFrameRate25:
		nRate = 25;
		break;
	case keNvcFrameRate30:
		nRate = 30;
		break;
	case keNvcFrameRate30M:
		nRate = 30000;
		nScale = 1001;
		break;
	case keNvcFrameRate50:
		nRate = 50;
		break;
	case keNvcFrameRate60:
		nRate = 60;
		break;
	case keNvcFrameRate60M:
		nRate = 60000;
		nScale = 1001;
		break;
	}

	if(out_pnRate != NULL)
		*out_pnRate = nRate;
	if(out_pnSacle != NULL)
		*out_pnSacle = nScale;
}

static inline ENvcFrameRate NvcGetFrameRateEnum(double dFrameRate)
{
    double dLegalRate[9] = {24000.0/1001.0, 24.0, 25.0, 30000.0/1001.0, 30.0, 50.0, 60000.0/1001.0, 60.0, 120.0};

    ENvcFrameRate eFrameRate[9] = {keNvcFrameRate24M, keNvcFrameRate24, keNvcFrameRate25, keNvcFrameRate30M, keNvcFrameRate30, keNvcFrameRate50, keNvcFrameRate60M, keNvcFrameRate60, keNvcFrameRateLast};

    double dPrevDiff = 9999999999.99;

    double dCurrentdiff = 0;

    bool	bFound = false;

    int iTypeFound = 0;

    for (int iType = 0; iType <= 8; iType++)
    {
        dCurrentdiff = dLegalRate[iType] - dFrameRate;
        if(dCurrentdiff < 0)
            dCurrentdiff = -dCurrentdiff;

        if (dCurrentdiff < 1e-6)
        {
            bFound = true;
            iTypeFound = iType;
            break;
        } 
        else if (dCurrentdiff > dPrevDiff)
        {
            // we found the minimum point. The minmum is the last one.
            bFound = true;
            iTypeFound = iType - 1;
            break;
        }
        else 
        {
            dPrevDiff = dCurrentdiff;
        }
    }

    if(!bFound || iTypeFound < 0)
        return keNvcFrameRateInvalid;

    return eFrameRate[iTypeFound];    
}

//enum ENvcAudioSampleRate 
//{
//    keNvcAudioSampleRate_Invalid,
//    keNvcAudioSampleRate_8000,
//    keNvcAudioSampleRate_11025,
//    keNvcAudioSampleRate_22050,
//    keNvcAudioSampleRate_32000,
//    keNvcAudioSampleRate_44100,
//    keNvcAudioSampleRate_48000,
//    keNvcAudioSampleRate_96000,
//	  keNvcAudioSampleRate_16000,
//	  keNvcAudioSampleRate_24000,
//    keNvcAudioSampleRate_Last
//};
enum ENvcAudioSampleRate
{
	keNvcAudioSampleRate_Invalid,
	keNvcAudioSampleRate_24000,
	keNvcAudioSampleRate_8000,
	keNvcAudioSampleRate_11025,
	keNvcAudioSampleRate_16000,
	keNvcAudioSampleRate_22050,	
	keNvcAudioSampleRate_32000,
	keNvcAudioSampleRate_44100,
	keNvcAudioSampleRate_48000,
	keNvcAudioSampleRate_96000,	
	keNvcAudioSampleRate_Last
};

static inline ENvcAudioSampleRate NvcGetAudioSampleRateEnum(unsigned int in_nSampleRate)
{
    switch (in_nSampleRate) {
    case 48000:
        return keNvcAudioSampleRate_48000;
    case 32000:
        return keNvcAudioSampleRate_32000;
    case 44100:
        return keNvcAudioSampleRate_44100;
    case 22050:
        return keNvcAudioSampleRate_22050;
	case 24000:
		return keNvcAudioSampleRate_24000;
    case 11025:
        return keNvcAudioSampleRate_11025;
    case 8000:
        return keNvcAudioSampleRate_8000;
    case 96000:
        return keNvcAudioSampleRate_96000;
	case 16000:
		return keNvcAudioSampleRate_16000;
    default:
        return keNvcAudioSampleRate_Invalid;
    }
}

//
// get audio sample rate's numerical value from its enumeration constant
//
static inline unsigned int NvcGetAudioSampleRateFromEnum(ENvcAudioSampleRate in_eSampleRate)
{
    switch (in_eSampleRate) {
    case keNvcAudioSampleRate_48000:
        return 48000;
    case keNvcAudioSampleRate_32000:
        return 32000;
    case keNvcAudioSampleRate_44100:
        return 44100;
    case keNvcAudioSampleRate_22050:
        return 22050;
    case keNvcAudioSampleRate_11025:
        return 11025;
    case keNvcAudioSampleRate_8000:
        return 8000;
    case keNvcAudioSampleRate_96000:
        return 96000;
	case keNvcAudioSampleRate_16000:
		return 16000;
    default:
        return 0;
    }
}

enum ENvcChannelType
{
	keNvcChannelTypeInvalid	 = 0,
	keNvcChannelTypeMono     = 1,
	keNvcChannelTypeStereo   = 2,
	keNvcChannelType1Track   = 1,
	keNvcChannelType2Tracks  = 2,
	keNvcChannelType3Tracks  = 3,
	keNvcChannelType4Tracks  = 4,
	keNvcChannelType5Tracks  = 5,
	keNvcChannelType6Tracks  = 6,
	keNvcChannelType7Tracks  = 7,
	keNvcChannelType8Tracks  = 8,
	keNvcChannelType9Tracks  = 9,
	keNvcChannelType10Tracks = 10,
	keNvcChannelType11Tracks = 11,
	keNvcChannelType12Tracks = 12,
	keNvcChannelType13Tracks = 13,
	keNvcChannelType14Tracks = 14,
	keNvcChannelType15Tracks = 15,
	keNvcChannelType16Tracks = 16,
	keNvcChannelTypeLast
};


enum ENvcAudioDataType
{
	keNvcAudioDataTypeInvalid           = 0,
	keNvcAudioDataTypePCM               = 1,
	keNvcAudioDataTypeIEEEFLoat         = 2,
	keNvcAudioDataTypeMatroxMultiFormat = 3,
	keNvcAudioDataTypeCompress			= 4,
	keNvcAudioDataTypeLast
};

enum ENvcAudioBitsPerSample
{
	keNvcAudioBitsPerSampleInvalid   = 0,
	keNvcAudioBitsPerSample8		 = 8,
	keNvcAudioBitsPerSample16        = 16,
	keNvcAudioBitsPerSample20        = 20,
	keNvcAudioBitsPerSample24        = 24,
	keNvcAudioBitsPerSample32        = 32,
    keNvcAudioBitsPerSample24In32    = 124,
    keNvcAudioBitsPerSample32float   = 132,
	keNvcAudioBitsPerSampleLast
};

static inline ENvcAudioBitsPerSample NvcGetAudioBitsPerSampleEnum(unsigned int in_nValidBPS, unsigned int in_nBPS, ENvcAudioDataType eDataType)
{
    if(eDataType == keNvcAudioDataTypeIEEEFLoat)
    {
        if(in_nValidBPS == 32 && in_nBPS == 32)
            return keNvcAudioBitsPerSample32float;
    }
    else
    {
		if(in_nValidBPS == 8 && in_nBPS == 8)
			return keNvcAudioBitsPerSample8;
		else if(in_nValidBPS == 16 && in_nBPS == 16)
            return keNvcAudioBitsPerSample16;
        else if(in_nValidBPS == 20 && in_nBPS == 24)
            return keNvcAudioBitsPerSample20;
        else if(in_nValidBPS == 24 && in_nBPS == 24)
            return keNvcAudioBitsPerSample24;
        else if(in_nValidBPS == 24 && in_nBPS == 32)
            return keNvcAudioBitsPerSample24In32;
        else if(in_nValidBPS == 32 && in_nBPS == 32)
            return keNvcAudioBitsPerSample32;
    }    

    return keNvcAudioBitsPerSampleInvalid;
}

static inline void NvcGetAudioBitsPerSampleFromEnum(ENvcAudioBitsPerSample in_eAudioBitsPerSample,
													unsigned int &out_nValidBPS, 
													unsigned int &out_nBPS)
{
	switch (in_eAudioBitsPerSample)
	{
	case keNvcAudioBitsPerSample8:
		out_nValidBPS = 8;
		out_nBPS = 8;
		break;
	case keNvcAudioBitsPerSample16:
		out_nValidBPS = 16;
		out_nBPS = 16;
		break;
	case keNvcAudioBitsPerSample20:
		out_nValidBPS = 20;
		out_nBPS = 24;
		break;
	case keNvcAudioBitsPerSample24:
		out_nValidBPS = 24;
		out_nBPS = 24;
		break;
	case keNvcAudioBitsPerSample24In32:
		out_nValidBPS = 24;
		out_nBPS = 32;
		break;
	case keNvcAudioBitsPerSample32:
	case keNvcAudioBitsPerSample32float:
		out_nValidBPS = 32;
		out_nBPS = 32;
		break;
	default:
		out_nValidBPS = 16;
		out_nBPS = 16;
	}
}

enum ENvcChromaFormat {
	keNvcChromaFormatInvalid = 0,
	keNvcChromaFormat420,
	keNvcChromaFormat422,
	keNvcChromaFormat411,
	keNvcChromaFormat444,
	keNvcChromaFormatLast
};


enum ENvcBGRAChannel
{
	keNvcChannel_None = 0,
	keNvcChannel_A    = 0x1,
	keNvcChannel_R    = 0x2,
	keNvcChannel_G    = 0x4,
	keNvcChannel_B	  = 0x8
};

enum ENvcView
{
    keNvcView_Invalid = -1,
    keNvcView_Left = 0,
    keNvcView_Right,
    keNvcView_Count
};

//
// Video Proxy Level
//
enum ENvcProxyLevel
{
	keNvcProxyLevel_Invalid = -1, 
	keNvcProxyLevel_Base = 0,   // Base proxy level (full resolution)
	keNvcProxyLevel_1,          // Proxy level1 (1/2 resolution)
	keNvcProxyLevel_2,          // Proxy level1 (1/4 resolution)
	keNvcProxyLevel_3,          // Proxy level1 (1/8 resolution)
	keNvcProxyLevel_Count
};

static inline int NvcBitMaskOfView(ENvcView view)
{
    return (int)(1 << view);
}

static inline int NvcBitMaskOfAllViews()
{
    return (int)((1 << keNvcView_Count) - 1);
}

struct SNvcRect
{
	long lLeft;
	long lTop;
	long lRight;
	long lBottom;
};

struct SNvcRectF
{
	float fLeft;
	float fTop;
	float fRight;
	float fBottom;
};

struct SNvcRectD
{
	double dLeft;
	double dTop;
	double dRight;
	double dBottom;
};

struct SNvcPoint
{
	long x;
	long y;
};

struct SNvcPointF
{
	float fX;
	float fY;
};

struct SNvcPointD
{
	double dX;
	double dY;
};

//
// Vector with 3 components (X,Y,Z)
//
struct SNvcVector3
{
	double x;
	double y;
	double z;
};

//
// Vector with 4 components (X,Y,Z,W)
//
struct SNvcVector4
{
	double x;
	double y;
	double z;
	double w;
};

//
// Matrix 4 x 4, row major
//
struct SNvcMatrix 
{
	double m[4][4];
};


//
// View matrix properties
//
struct SNvcViewDesc
{
	SNvcVector3 posEye;			// Viewer position.
	SNvcVector3 posLookAtPoint;	// Point at which the viewer is looking.
	SNvcVector3 vecUp;			// "Up" direction in the current world.
};

// Orthographic projection matrix properties
struct SNvcOrthographicProjectionDesc
{
	double left;
	double right;
	double bottom;
	double top;
	double farZ;
	double nearZ;
};

//
// Perspective projection matrix properties
//
struct SNvcPerspectiveProjectionDesc
{
	double fovY;        // The field of view angle, in degrees, in the y-direction
	SNvcAspectRatio aspectRatio; // The aspect ratio that determines the field of view in the x-direction. The aspect ratio is the ratio of x (width) to y (height)
	double nearZ;       // The distance from the viewer to the near clipping plane (always positive). 
	double farZ;        // The distance from the viewer to the far clipping plane (always positive). 
};

struct SNvcPerspectiveProjectionDescExt
{
	double left;		// left border of the near clipping plane
	double top;			// top border of the near clipping plane
	double right;		// right border of the near clipping plane
	double bottom;		// bottom border of the near clipping plane
	double nearZ;       // The distance from the viewer to the near clipping plane (always positive). 
	double farZ;        // The distance from the viewer to the far clipping plane (always positive). 
};

//////////////////////////////////////////////////////////////////////////
// double-precision 2 dimensional affine transformation matrix (row major)
//////////////////////////////////////////////////////////////////////////
struct SNvcAffineMatrix2D {
	double m11, m12;
	double m21, m22;
	double m31, m32;
};

//////////////////////////////////////////////////////////////////////////
// structure to describe image position
//////////////////////////////////////////////////////////////////////////
struct SNvcImagePositionDesc {
	unsigned int nImageWidth;
	unsigned int nImageHeight;
	int nCenterPixelCoordX;	// x coordinates of the center pixel of the image
	int nCenterPixelCoordY;	// y coordinates of the center pixel of the image
};

struct SNvcResolutionInfo
{
	uint32_t        	size;
	uint32_t		    ui32Width;
	uint32_t			ui32Height;
	uint32_t			ui32ComponentBitCount;
	SNvcAspectRatio		sPixelAspectRatio;
	ENvcFrameRate		eFrameRate;
	ENvcScanMode		eScanMode;
};


/**
* WaveFormatInfo
*/
struct SNvcWaveFormatInfo
{
	uint32_t				size;
	ENvcChannelType			eChannelType;
	ENvcAudioDataType		eDataType;
	uint32_t				ui32SamplesPerSec;
	uint32_t				ui32BitsPerSample;
	uint32_t				ui32ValidBitsPerSample;
};

//////////////////////////////////////////////////////////////////////////
// structure to describe planar buffer info
//////////////////////////////////////////////////////////////////////////
struct SNvcBufferPlaneInfo
{
	void *	 pDataPtr[NVC_MAX_BUFFER_PLANE_COUNT];
	int		 nPitch[NVC_MAX_BUFFER_PLANE_COUNT];
	int		 nPlanarCount;
};
