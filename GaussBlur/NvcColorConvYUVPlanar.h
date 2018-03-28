#pragma once

#include <NvcColorConvDef.h>

#ifdef __cplusplus
extern "C" {
#endif

void NVCAPI Nvc_8Bit_YUV420_Planar_To_16Bit_BGRA(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	int nSrcPitch[4],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	unsigned int nAlpha,
	ENvcColorimetry eColorimetry,
	int bIsNonTemporal,
	int bIsTopSingleLine);

void NVCAPI Nvc_8Bit_YUV420_Planar_To_32Bit_BGRA(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	int nSrcPitch[4],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	float fAlpha,
	ENvcColorimetry eColorimetry,
	int bIsNonTemporal,
	int bIsTopSingleLine);

void NVCAPI Nvc_8Bit_YUVA420_Planar_To_8Bit_BGRA(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const void *pSrcA,
	int nSrcPitch[4],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	ENvcColorimetry eColorimetry,
	int bIsNonTemporal,
	int bIsTopSingleLine);

void NVCAPI Nvc_8Bit_YUVA420_Planar_To_16Bit_BGRA(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const void *pSrcA,
	int nSrcPitch[4],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	ENvcColorimetry eColorimetry,
	int bIsNonTemporal,
	int bIsTopSingleLine);

void NVCAPI Nvc_8Bit_YUVA420_Planar_To_32Bit_BGRA(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const void *pSrcA,
	int nSrcPitch[4],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	ENvcColorimetry eColorimetry,
	int bIsNonTemporal,
	int bIsTopSingleLine);

void NVCAPI Nvc_8Bit_BGRA_To_8Bit_YUV420_Planar(const void *pSrc,
	int nSrcPitch,
	void *pDstY,
	void *pDstU,
	void *pDstV,
	int nDstPitch[4],
	unsigned int nWidth,
	unsigned int nHeight,
	int bPremultiply,
	ENvcColorimetry eColorimetry,
	int bIsNonTemporal);

void NVCAPI Nvc_8Bit_BGRA_To_8Bit_YUY2_Planar(
	const void *pSrc,
	int nSrcPitch,
	void *ppDst[4],
	int nDstPitch[4],
	unsigned int nWidth,
	unsigned int nHeight,
	int bPremultiply,
	ENvcColorimetry eColorimetry,
	int bIsNonTemporal);

void NVCAPI Nvc_8Bit_BGR_To_8Bit_YUY422_Planar(
	const void *pSrc,
	int nSrcPitch,
	void *ppDst[3],
	int nDstPitch[3],
	unsigned int nWidth,
	unsigned int nHeight,
	int bPremultiply,
	ENvcColorimetry eColorimetry,
	int bIsNonTemporal);

void NVCAPI Nvc_8Bit_RGB_To_8Bit_YUY422_Planar(
	const void *pSrc,
	int nSrcPitch,
	void *ppDst[3],
	int nDstPitch[3],
	unsigned int nWidth,
	unsigned int nHeight,
	int bPremultiply,
	ENvcColorimetry eColorimetry,
	int bIsNonTemporal);

void NVCAPI Nvc_8Bit_BGRA_To_8Bit_YUYA422_Planar(
	const void *pSrc,
	int nSrcPitch,
	void *ppDst[4],
	int nDstPitch[4],
	unsigned int nWidth,
	unsigned int nHeight,
	int bPremultiply,
	ENvcColorimetry eColorimetry,
	int bIsNonTemporal);

void NVCAPI Nvc_8Bit_YUV422P_To_8Bit_YUY2(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	int bIsNonTemporal);

void NVCAPI Nvc_8Bit_YUVA4224P_To_8Bit_YUY2(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const void *pSrcA,
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	int bIsNonTemporal);

void NVCAPI Nvc_10Bit_YUVA4224P_To_8Bit_YUY2(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const void *pSrcA,
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	int bIsNonTemporal);

void NVCAPI Nvc_8Bit_YUY2_To_8Bit_YUV422P(const void *pSrcYUV,
	const int nSrcPitch,
	void *pDstY,
	void *pDstU,
	void *pDstV,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight,
	int bIsNonTemporal);

void NVCAPI Nvc_8Bit_YUY2_To_10Bit_YUV422P(const void *pSrcYUV,
	const int nSrcPitch,
	void *pDstY,
	void *pDstU,
	void *pDstV,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight,
	int bIsNonTemporal);

void NVCAPI Nvc_8Bit_YUV422P_To_8Bit_BGR(void *pSrc[],
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	ENvcColorimetry eColorimetry);

void NVCAPI Nvc_8Bit_YUV422P_To_8Bit_RGB(const void *pSrc[],
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	ENvcColorimetry eColorimetry);

void NVCAPI Nvc_8Bit_YUVA4224P_To_8Bit_RGB(void *pSrc[],
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	ENvcColorimetry eColorimetry);

void NVCAPI Nvc_10Bit_YUVA4224P_To_8Bit_RGB(void *pSrc[],
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	ENvcColorimetry eColorimetry);

void NVCAPI Nvc_8Bit_YUVA4224P_To_8Bit_YUAYVA4224(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const void *pSrcA,
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	int bIsNonTemporal);

void Nvc_8Bit_YUVA4224P_To_8Bit_UYAVYA4224(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const void *pSrcA,
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	int bIsNonTemporal);


void NVCAPI Nvc_8Bit_YUAYVA4224_To_8Bit_YUVA4224P(const void *pSrc,
	const int nSrcPitch,
	void *pDstY,
	void *pDstU,
	void *pDstV,
	void *pDstA,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight,
	int bIsNonTemporal);

void NVCAPI Nvc_8Bit_YUAYVA4224_To_10Bit_YUVA4224P(const void *pSrc,
	const int nSrcPitch,
	void *pDstY,
	void *pDstU,
	void *pDstV,
	void *pDstA,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight,
	int bIsNonTemporal);

void NVCAPI Nvc_8Bit_YUV420P_To_8Bit_YUV422P(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const int nSrcPitch[],
	void *pDstY,
	void *pDstU,
	void *pDstV,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight);

void NVCAPI Nvc_8Bit_YUV420P_To_10Bit_YUV422P(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const int nSrcPitch[],
	void *pDstY,
	void *pDstU,
	void *pDstV,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight);

void NVCAPI Nvc_8Bit_YUV444P_To_8Bit_YUV422P(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const int nSrcPitch[],
	void *pDstY,
	void *pDstU,
	void *pDstV,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight);

void NVCAPI Nvc_8Bit_YUV444P_To_10Bit_YUV422P(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const int nSrcPitch[],
	void *pDstY,
	void *pDstU,
	void *pDstV,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight);

void NVCAPI Nvc_8Bit_YUVA4204P_To_8Bit_YUVA4224P(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const void *pSrcA,
	const int nSrcPitch[],
	void *pDstY,
	void *pDstU,
	void *pDstV,
	void *pDstA,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight);

void NVCAPI Nvc_8Bit_YUVA4204P_To_10Bit_YUVA4224P(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const void *pSrcA,
	const int nSrcPitch[],
	void *pDstY,
	void *pDstU,
	void *pDstV,
	void *pDstA,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight);

void NVCAPI Nvc_8Bit_YUV422P_To_16Bit_BGR(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	ENvcColorimetry eColorimetry);

void NVCAPI Nvc_8Bit_YUV422P_To_16Bit_RGB(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	ENvcColorimetry eColorimetry);

void NVCAPI Nvc_8Bit_YUVA4224P_To_8Bit_BGR(void *pSrc[],
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	ENvcColorimetry eColorimetry);

void NVCAPI Nvc_8Bit_YUVA4224P_To_16Bit_BGR(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const void *pSrcA,
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	ENvcColorimetry eColorimetry);

void NVCAPI Nvc_8Bit_YUV422P_To_10Bit_YUY2(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight);

void NVCAPI Nvc_10Bit_YUV422P_To_10Bit_YUY2(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight);

void NVCAPI Nvc_10Bit_YUV422P_To_8Bit_YUY2(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight);

void NVCAPI Nvc_10Bit_YUVA4224P_To_10Bit_YUY2(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const void *pSrcA,
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight);

void NVCAPI Nvc_8Bit_YUVA4224P_To_10Bit_YUY2(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const void *pSrcA,
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight);

void NVCAPI Nvc_10Bit_YUY2_To_8Bit_YUV422P(const void *pSrcYUV,
	const int nSrcPitch,
	void *pDstY,
	void *pDstU,
	void *pDstV,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight,
	int bIsNonTemporal);

void NVCAPI Nvc_10Bit_YUY2_To_10Bit_YUV422P(const void *pSrcYUV,
	const int nSrcPitch,
	void *pDstY,
	void *pDstU,
	void *pDstV,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight,
	int bIsNonTemporal);

void NVCAPI Nvc_10Bit_YUV422P_To_16Bit_BGR(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	ENvcColorimetry eColorimetry);

void NVCAPI Nvc_10Bit_YUV422P_To_16Bit_RGB(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	ENvcColorimetry eColorimetry);

void NVCAPI Nvc_10Bit_YUVA4224P_To_10Bit_YUAYVA4224(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const void *pSrcA,
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	int bIsNonTemporal);

void NVCAPI Nvc_10Bit_YUVA4224P_To_10Bit_UYAVYA4224(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const void *pSrcA,
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	int bIsNonTemporal);

void NVCAPI Nvc_10Bit_UYAVYA4224_To_10Bit_YUVA4224P(const void *pSrc,
	const int nSrcPitch,
	void *pDstY,
	void *pDstU,
	void *pDstV,
	void *pDstA,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight,
	int bIsNonTemporal);

void NVCAPI Nvc_10Bit_YUAYVA4224_To_8Bit_YUVA4224P(const void *pSrc,
	const int nSrcPitch,
	void *pDstY,
	void *pDstU,
	void *pDstV,
	void *pDstA,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight,
	int bIsNonTemporal);

void NVCAPI Nvc_10Bit_YUAYVA4224_To_10Bit_YUVA4224P(const void *pSrc,
	const int nSrcPitch,
	void *pDstY,
	void *pDstU,
	void *pDstV,
	void *pDstA,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight,
	int bIsNonTemporal);

void NVCAPI Nvc_10Bit_YUV420P_To_8Bit_YUV422P(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const int nSrcPitch[],
	void *pDstY,
	void *pDstU,
	void *pDstV,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight);

void NVCAPI Nvc_10Bit_YUV420P_To_10Bit_YUV422P(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const int nSrcPitch[],
	void *pDstY,
	void *pDstU,
	void *pDstV,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight);

void NVCAPI Nvc_10Bit_YUV444P_To_8Bit_YUV422P(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const int nSrcPitch[],
	void *pDstY,
	void *pDstU,
	void *pDstV,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight);

void NVCAPI Nvc_10Bit_YUV444P_To_10Bit_YUV422P(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const int nSrcPitch[],
	void *pDstY,
	void *pDstU,
	void *pDstV,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight);

void NVCAPI Nvc_10Bit_YUVA4204P_To_8Bit_YUVA4224P(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const void *pSrcA,
	const int nSrcPitch[],
	void *pDstY,
	void *pDstU,
	void *pDstV,
	void *pDstA,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight);

void NVCAPI Nvc_10Bit_YUVA4204P_To_10Bit_YUVA4224P(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const void *pSrcA,
	const int nSrcPitch[],
	void *pDstY,
	void *pDstU,
	void *pDstV,
	void *pDstA,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight);

void NVCAPI Nvc_10Bit_YUV422P_To_8Bit_BGR(void *pSrc[],
	const int nSrcPitch[3],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	ENvcColorimetry eColorimetry);

void NVCAPI Nvc_10Bit_YUV422P_To_8Bit_RGB(const void *pSrc[],
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	ENvcColorimetry eColorimetry);

void NVCAPI Nvc_10Bit_YUVA4224P_To_16Bit_BGR(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const void *pSrcA,
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	ENvcColorimetry eColorimetry);

void NVCAPI Nvc_10Bit_YUVA4224P_To_8Bit_BGR(void *pSrc[],
	const int nSrcPitch[],
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	ENvcColorimetry eColorimetry);

void NVCAPI Nvc_YUV422P_10B_To_8B(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const int nSrcPitch[],
	void *pDstY,
	void *pDstU,
	void *pDstV,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight,
	ENvcColorimetry eColorimetry,
	int bIsNonTemporal);

void NVCAPI Nvc_YUVA4224P_10B_To_8B(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const void *pSrcA,
	const int nSrcPitch[],
	void *pDstY,
	void *pDstU,
	void *pDstV,
	void *pDstA,
	int nDstPitch[],
	unsigned int nWidth,
	unsigned int nHeight,
	ENvcColorimetry eColorimetry,
	int bIsNonTemporal);

void NVCAPI NvcColorSpaceConversion_YUVPlanar(
	ENvcColorSpace			eSrcColorSpace,
	ENvcBitDepth			eSrcBitDepth,
	SNvcBufferPlaneInfo&	sSrcPlaneInfo,
	ENvcColorSpace			eDstColorSpace,
	ENvcBitDepth			eDstBitDepth,
	SNvcBufferPlaneInfo&	sDstPlaneInfo,
	unsigned int			uiWidth,
	unsigned int			uiHeight,
	float					fAlpha,
	bool					bPremultipliedConversion,
	ENvcColorimetry			eColorimetry,
	bool					bIsNonTemporal,
	bool					bIsTopSingleLine);

void NVCAPI Nvc_YUV422P_8Bit_To_BGRA_8Bit_Half_Image_Copy(const void *pSrc[],
	const int nSrcPitch[],
	bool bFieldInAFrame,
	int startLine,
	void *pDst,
	const int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	unsigned int nAlpha,
	ENvcColorimetry eColorimetry,
	int bIsNonTemporal);

void NVCAPI Nvc_YUVA4224P_8Bit_To_BGRA_8Bit_Half_Image_Copy(const void *pSrc[],
	const int nSrcPitch[],
	bool bFieldInAFrame,
	int startLine,
	void *pDst,
	const int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	unsigned int nAlpha,
	ENvcColorimetry eColorimetry,
	int bIsNonTemporal);

void NVCAPI Nvc_YUV422P_10Bit_To_BGRA_8Bit_Half_Image_Copy(const void *pSrc[],
	const int nSrcPitch[],
	bool bFieldInAFrame,
	int startLine,
	void *pDst,
	const int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	unsigned int nAlpha,
	ENvcColorimetry eColorimetry,
	int bIsNonTemporal);

void NVCAPI Nvc_YUVA4224P_10Bit_To_BGRA_8Bit_Half_Image_Copy(const void *pSrc[],
	const int nSrcPitch[],
	bool bFieldInAFrame,
	int startLine,
	void *pDst,
	const int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	unsigned int nAlpha,
	ENvcColorimetry eColorimetry,
	int bIsNonTemporal);

void Nvc_YUV422P_ColorimetryConvert(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const int nSrcPitch[],
	const int nSrcColorimetry,
	ENvcBitDepth eSrcBitDepth,
	void *pDstY,
	void *pDstU,
	void *pDstV,
	const int nDstPitch[],
	const int nDstColorimetry,
	ENvcBitDepth eDstBitDepth,
	unsigned int nWidth,
	unsigned int nHeight,
	int bIsNonTemporal);

void Nvc_YUV420P_ColorimetryConvert(const void *pSrcY,
	const void *pSrcU,
	const void *pSrcV,
	const int nSrcPitch[],
	const int nSrcColorimetry,
	ENvcBitDepth eSrcBitDepth,
	void *pDstY,
	void *pDstU,
	void *pDstV,
	const int nDstPitch[],
	const int nDstColorimetry,
	ENvcBitDepth eDstBitDepth,
	unsigned int nWidth,
	unsigned int nHeight,
	int bIsNonTemporal);

#ifdef __cplusplus
}
#endif