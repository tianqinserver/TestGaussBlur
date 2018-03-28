//==========================================================================;
//
// (c) Copyright NewAuto Video Technology CO.,Ltd., 2008. All rights reserved. 
//
// This code and information is provided "as is" without warranty of any kind, 
// either expressed or implied, including but not limited to the implied 
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------;
//   Birth Date:       Sep 11 2008
//   Author:           NewAuto video team
//----------------------------------------------------------------
//   
//   Beijing China. 
//----------------------------------------------------------------

#ifndef __NVC_VISION_CORE_DEF_H__
#define	__NVC_VISION_CORE_DEF_H__

#include <NvcDef.h>

#ifdef _WIN32
    extern uint32_t kui32MaxImageWidth;
    extern uint32_t kui32MaxImageHeight;
#else
	extern uint32_t kui32MaxImageWidth;
	extern uint32_t kui32MaxImageHeight;
#endif


enum ENvcColorChannel
{
	keNvcColorChannelAlpha = 0,
	keNvcColorChannelRed,
	keNvcColorChannelGreen,
	keNvcColorChannelBlue,
	keNvcColorChannelLuma
};

enum ENvcBitDepth
{
	keNvcBitDepthInt8 = 0,
	keNvcBitDepthInt16,
	keNvcBitDepthFloat32
};

enum ENvcColorSpace
{
    keNvcColorSpace_Invalid = -1,
	keNvcColorSpace_BGRA = 0,
    keNvcColorSpace_ARGB,
    keNvcColorSpace_BGR,
    keNvcColorSpace_RGB,
    keNvcColorSpace_RGBA,
    keNvcColorSpace_YUY2,
    keNvcColorSpace_YUAYVA4224,
	keNvcColorSpace_UYAVYA4224,
    keNvcColorSpace_YUVA4444,
	keNvcColorSpace_Alpha,
    keNvcColorSpace_UYVY,
	keNvcColorSpace_Y,
	keNvcColorSpace_U,
	keNvcColorSpace_V,
	keNvcColorSpace_YUV444Planar,
	keNvcColorSpace_YUV422Planar,
	keNvcColorSpace_YUVA422Planar,
	keNvcColorSpace_YUV420Planar,
	keNvcColorSpace_YUVA420Planar,
	keNvcSurfaceFormatV210
};


inline int NvcGetPlanarCountForImage(ENvcColorSpace in_ecolorspace)
{
	switch (in_ecolorspace) {
	case keNvcColorSpace_YUV422Planar:
	case keNvcColorSpace_YUV420Planar:
		return 3;
	case keNvcColorSpace_YUVA422Planar:
	case keNvcColorSpace_YUVA420Planar:
		return 4;
	}
	return 1;
}

inline int NvcGetActualWidthForImagePlane(ENvcColorSpace eColorSpace, int in_nWidth, int planeIndex)
{
	if (eColorSpace != keNvcColorSpace_YUV422Planar && eColorSpace != keNvcColorSpace_YUVA422Planar &&
		eColorSpace != keNvcColorSpace_YUV420Planar && eColorSpace != keNvcColorSpace_YUVA420Planar)
		return in_nWidth;

	if (planeIndex == 1 || planeIndex == 2) {// is UV plane
		if (in_nWidth >= 0)
			return  (in_nWidth + 1) / 2;
		else
			return  (in_nWidth - 1) / 2;
	}

	return in_nWidth;
}

inline int NvcGetActualHeightForImagePlane(ENvcColorSpace eColorSpace, int in_nHeight, int planeIndex)
{
	if (eColorSpace != keNvcColorSpace_YUV420Planar && eColorSpace != keNvcColorSpace_YUVA420Planar)
		return in_nHeight;

	if (planeIndex == 1 || planeIndex == 2) {// is UV plane
		return  (in_nHeight + 1) / 2;
	}

	return in_nHeight;
}

enum ENvcFieldRenderMode
{
    keNvcFieldRenderMode_Frame = 0,
    keNvcFieldRenderMode_TopField,
    keNvcFieldRenderMode_BottomField
};


// helper function to get bytes per channel
static inline unsigned int NvcGetBytesFromBitDepth(ENvcBitDepth eBitDepth)
{
	switch (eBitDepth) {
	case keNvcBitDepthInt16:
		return 2;
	case keNvcBitDepthFloat32:
		return 4;
	}

	return 1;
}

// matte composition mode
// f  -- front matte value
// f' -- f * front_intensity
// b  -- back matte value
// b' -- b * back_intensity
// op -- front opacity
enum ENvcMatteCompMode {
	keNvcMatteComp_Invalid = -1,
	keNvcMatteComp_Over = 0,	// b'+(1-b')*op*f'
	keNvcMatteComp_Replace,		// (1-op)*f*b'+op*f'
	keNvcMatteComp_Atop,		// b'*(1+op*(f'-f))
	keNvcMatteComp_Inside,		// f'*b'
	keNvcMatteComp_Outside,		// (1-b')*op*f'
	keNvcMatteComp_Cutout,		// (1-f)*b'
	keNvcMatteComp_Xor,			// (1-b')*op*f'+(1-f)*b'
	keNvcMatteComp_Count
};

enum ENvcComposition2DMode
{
	//
	// general modes
	//
	keNvcComp2DMode_Normal,			// Cr = (Cb*Ab*(1 - At) + Ct*At) / Ar
	keNvcComp2DMode_Dissolve,		// this mode is too complex to explained here
	//
	// darken modes
	//
	keNvcComp2DMode_Darken,			// Cr = min(Cb,Ct)
	keNvcComp2DMode_Multiply,		// Cr = Cb*Ct
	keNvcComp2DMode_Burn,			// Cr = 1 - ((1 - Cb)/Ct)
	keNvcComp2DMode_LinearBurn,		// Cr = Cb + Ct - 1
	keNvcComp2DMode_Subtract,		// Cr = Cb - Ct
	//
	// lighten modes
	//
	keNvcComp2DMode_Lighten,		// Cr = max(Cb,Ct)
	keNvcComp2DMode_Screen,			// Cr = 1 - (1 - Cb)*(1 - Ct) = Cb + Ct - Cb*Ct
	keNvcComp2DMode_Dodge,			// Cr = Cb/(1 - Ct)
	keNvcComp2DMode_Add,			// Cr = Cb + Ct		(also known as linear dodge)
	//
	// light modes
	//
	keNvcComp2DMode_Overlay,		// if Cb<=0.5   Cr = 2*Cb*Ct
	// else         Cr = 1 - 2*(1 - Cb)*(1 - Ct)
	keNvcComp2DMode_SoftLight,		// if Ct<=0.5   Cr = (2*Ct - 1)*(Cb - Cb*Cb) + Cb
	// else         Cr = (2*Ct - 1)*(sqrt(Cb) - Cb) + Cb
	keNvcComp2DMode_HardLight,		// Cr = Overlay(Ct,Cb)
	keNvcComp2DMode_VividLight,		// if Ct<=0.5   Cr = 1 - (1 - Cb)/2*Ct
	// else         Cr = Cb/(2*(1-Ct))
	keNvcComp2DMode_LinearLight,	// Cb + 2*Ct - 1
	keNvcComp2DMode_PinLight,		// if       Cb<2*Ct - 1        Cr = 2*Ct - 1
	// else if  2*Ct - 1<Cb<2*Ct   Cr = Cb
	// else                        Cr = 2*Ct
	keNvcComp2DMode_HardMix,		// if Ct<1 - Cb Cr = 0
	// else         Cr = 1
	//
	// invert modes
	//
	keNvcComp2DMode_Difference,		// Cr = abs(Cb - Ct)
	keNvcComp2DMode_Exclusion,		// Cr = Cb + Ct - (2*Cb*Ct)
	//
	// color modes
	//
	keNvcComp2DMode_Hue,			// takes the hue from the top layer, but takes the brightness and saturation from the bottom layer
	keNvcComp2DMode_Saturation,		// takes its saturation from the top layer but brightness and hue from the bottom layer
	keNvcComp2DMode_Color,			// takes its brightness from the bottom layer, but hue and saturation from the top layer
	keNvcComp2DMode_Luminosity,		// takes its brightness from the top layer, but hue and saturation from the bottom layer
	keNvcComp2DMode_Last
};

enum ENvcMatteCorrelation {
	keNvcMatteCorrelation_None = 0,
	keNvcMatteCorrelation_Superposed,
	keNvcMatteCorrelation_Adjacent
};

struct sNvcBGRA_8Bit {
	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t a;
};

struct sNvcBGRA_16Bit {
	uint16_t b;
	uint16_t g;
	uint16_t r;
	uint16_t a;
};

struct sNvcBGRA_32Bit {
	float b;
	float g;
	float r;
	float a;
};

struct sNvcYUV_8Bit {
	uint8_t y;
	uint8_t u;
	uint8_t v;	
};

struct sNvcYUYV_8Bit {
	uint8_t y0;
	uint8_t u;
	uint8_t y1;
	uint8_t v;	
};

struct sNvcUYVY_8Bit {
    uint8_t u;
    uint8_t y0;
    uint8_t v;
    uint8_t y1;
};

struct sNvcYUVA_8Bit {
    uint8_t y;
    uint8_t u;
    uint8_t v;
    uint8_t a;
};

struct sNvcYUVA_16Bit {
	uint16_t y;
	uint16_t u;
	uint16_t v;
	uint16_t a;
};

//////////////////////////////////////////////////////////////////////////
// general masking information
//////////////////////////////////////////////////////////////////////////
enum ENvcMaskType {
	keNvcMaskTypeAlpha,
	keNvcMaskTypeBGRA,
	keNvcMaskTypeYUV422Planar,
	keNvcMaskTypeYUVA422Planar
};

#define NVC_MASK_INTENSITY_DEFAULT		1
#define NVC_MASK_INTENSITY_MIN			0
#define NVC_MASK_INTENSITY_MAX			1

struct SNvcMaskInfo {
	void *pMaskBits;
	int nMaskPitch;
	ENvcMaskType eType;
	ENvcColorChannel eChannel;
	ENvcBitDepth eBitDepth;
	float fIntensity;
	bool bInvert;
	ENvcColorimetry eColorimetry;
	SNvcBufferPlaneInfo sPlaneInfo; // this is for mask type keNvcMaskTypeYUVA422Planar
};


//CC Basic Tone Type
enum ENvcColorToneType
{
	ENvcColorToneTypeMaster			= 0x00000000,
	ENvcColorToneTypeShadows		= 0x00000001,
	ENvcColorToneTypeMidtones		= 0x00000002,   
	ENvcColorToneTypeHighlights		= 0x00000003,
};

//////////////////////////////////////////////////////////////////////////
// cropping information
//////////////////////////////////////////////////////////////////////////
enum ENvcCroppingMode {
	keNvcGMaskCropMode_None,
	keNvcGMaskCropMode_Relative,
	keNvcGMaskCropMode_Absolute
};

struct SNvcCroppingInfo {
	ENvcCroppingMode eCroppingMode;
	int nCropLeft, nCropRight;
	int nCropTop, nCropBottom;
};

struct SNvcColorF {
	float fRed;
	float fGreen;
	float fBlue;
	float fAlpha;
};

//////////////////////////////////////////////////////////////////////////
// two-dimensional double precision bounding box structure
//////////////////////////////////////////////////////////////////////////
struct SNvcBoundingBox2D {
	double dMinX, dMaxX;
	double dMinY, dMaxY;
};

//////////////////////////////////////////////////////////////////////////
// two-dimensional motion parameters structure
//////////////////////////////////////////////////////////////////////////
struct SNvc2DMotionParams {
	double dTranslationX, dTranslationY;

	double dScaleX, dScaleY;
	double dRotationAngle;	// rotation angle in degree

	// anchor point are in the coordinate space before the transformation
	double dAnchorX, dAnchorY;
};

//////////////////////////////////////////////////////////////////////////
// three-dimensional motion parameters structure
//////////////////////////////////////////////////////////////////////////
struct SNvc3DMotionParams {
	double dTranslationX, dTranslationY, dTranslationZ;

	double dScaleX, dScaleY, dScaleZ;

	// rotation angles in degree
	double dRotationAngleX, dRotationAngleY, dRotationAngleZ;

	// anchor point are in the coordinate space before the transformation
	double dAnchorX, dAnchorY, dAnchorZ;
};

//////////////////////////////////////////////////////////////////////////
// pixel sampling mode
//////////////////////////////////////////////////////////////////////////
enum ENvcPixelSamplingMode
{
	keNvcPixelSamplingMode_NN,			// nearest neighbor
	keNvcPixelSamplingMode_Linear,		// (bi)linear interpolation
	keNvcPixelSamplingMode_Cubic,		// (bi)cubic interpolation (Keys)
	keNvcPixelSamplingMode_Mitchell,	// Mitchell filter (The Mitchell-Netravali cubic)
	keNvcPixelSamplingMode_BSpline,		// The B-spline cubic
	keNvcPixelSamplingMode_Gaussian,	// gaussian filter
	keNvcPixelSamplingMode_Lanczos3		// Lanczos3 filter
};

//////////////////////////////////////////////////////////////////////////
// image wrap mode
//////////////////////////////////////////////////////////////////////////
enum ENvcImageWrapMode
{
	keNvcImageWrapMode_Color,
	keNvcImageWrapMode_Edge,
	keNvcImageWrapMode_Repeat,
	keNvcImageWrapMode_Mirror
};

//////////////////////////////////////////////////////////////////////////
// edge mode
//////////////////////////////////////////////////////////////////////////
enum ENvcEdgeMode
{
	keNvcEdgeMode_Center,
	keNvcEdgeMode_Inside,
	keNvcEdgeMode_Outside
};

//////////////////////////////////////////////////////////////////////////
// edge mode
//////////////////////////////////////////////////////////////////////////
enum ENvcPageCurlMode
{
	keNvcEdgeMode_SectionLeft,
	keNvcEdgeMode_CoilingBlock,
};

//////////////////////////////////////////////////////////////////////////
//3d effect mode
/////////////////////////////////////////////////////////////////////////
enum ENvcSwingMode
{
	keNvcSwingMode_In,
	keNvcSwingMode_Out,
};

enum ENvcRotationMode
{
	keNvcRotationMode_In,
	keNvcRotationMode_Out,
};

enum ENvcDoorMode
{
	keNvcDoorMode_open,
	keNvcDoorMode_close,
};

//////////////////////////////////////////////////////////////////////////
// detection type for ImageDetection
//////////////////////////////////////////////////////////////////////////
enum ENvcDetectionType
{
	keNvcDetectionType_None = 0,
	keNvcDetectionType_Static = 1,			// 静帧
	keNvcDetectionType_Black = 2,			// 黑场
	keNvcDetectionType_Red = 4,				// 红场
	keNvcDetectionType_Green = 8,			// 绿场
	keNvcDetectionType_Blue = 16,			// 蓝场
	keNvcDetectionType_Magenta = 32,		// 粉(洋红)场
	keNvcDetectionType_100ColorBar = 64,	// 100%彩条
	keNvcDetectionType_75ColorBar = 128,	// 75%彩条
	keNvcDetectionType_50ColorBar = 256,	// 50%彩条
	keNvcDetectionType_Mottle = 512,		// 花帧
	keNvcDetectionType_YUVExceed = 1024,
	keNvcDetectionType_RGBExceed = 2048,
	keNvcDetectionType_Luma = 4096,
	keNvcDetectionType_AspectRatioLR = 8192,
	keNvcDetectionType_AspectRatioUDLR = 16384,
	//	keNvcDetectionType_Mosaic,				// 马赛克
	//	keNvcDetectionType_Mute,				// 静音
	//	keNvcDetectionType_Boom,				// 爆音
	keNvcDetectionType_Last
};

//////////////////////////////////////////////////////////////////////////
// keyer patch related
//////////////////////////////////////////////////////////////////////////
#define	NVC_KEYER_PATCH_MAX_COUNT			32

enum ENvcKeyerPatchMode {
	keNvcKeyerPatchBlack = 0,
	keNvcKeyerPatchWhite,
};

//
// keyer patch information structure
//
struct SNvcKeyerPatchInfo {
	SNvcMatrix sMatrix;
	ENvcKeyerPatchMode eMode;
	float fRange;
	float fSoftness;
};

//////////////////////////////////////////////////////////////////////////
//tracker public param 
//////////////////////////////////////////////////////////////////////////

//typedef void* (NVCAPI * NVC_PFN_BUFFER_ALLOC_CALLBACK)(size_t size, int align, void* pAllocator);
//typedef void (NVCAPI * NVC_PFN_BUFFER_FREE_CALLBACK)(void *pMembloack, void* pAllocator);
//
//struct SNvcTrackerBufMgr
//{
//	void* m_pAllocator;
//	NVC_PFN_BUFFER_ALLOC_CALLBACK m_pfnAlloc;
//	NVC_PFN_BUFFER_FREE_CALLBACK m_pfnFree;
//};

struct SNvcTrackerParams
{
	int				width;
	int				height;
	const void*		psrc;
	int				pitch;
	ENvcBitDepth	eBitDepth;
	SNvcPointF		ptDestinationPosition;
	ENvcColorimetry	eColorimetry;
};

struct SNvcTrackerObjInitParams
{
	int				featurescount;
	SNvcRect		rcSelectBox;
	SNvcRect		rcSearchBox;

	int				particlecount;
	int				vecx;
	int				vecy;
};

struct SNvcTrackObjOutput
{
	float				left;
	float				top;
	float				right; 
	float				bottom;
};

#endif // __NVC_VISION_CORE_DEF_H__

