//==========================================================================;
//
// (c) Copyright NewAuto Video Technology CO.,Ltd., 2008. All rights reserved. 
//
// This code and information is provided "as is" without warranty of any kind, 
// either expressed or implied, including but not limited to the implied 
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------;
//   Birth Date:       Dec 24. 2008
//   Author:           NewAuto video team
//----------------------------------------------------------------
//   
//   Beijing China. 
//----------------------------------------------------------------

#ifndef NVC_FORCE_INLINE
#define NVC_FORCE_INLINE			__forceinline
#endif

//#include <NvcVisionUtils.h>
//#include <NvcColorConv.h>
//#include <NvcColorConvUtils.h>

#include <malloc.h>
#include <emmintrin.h>


#define __NVC_MASK_SCALE_255_256		((unsigned int)(256.0 / 255.0 * 32768 + 0.5))

static const __m128i g_xmm_zero = _mm_setzero_si128();

static const __m128i g_xmm_scale_255_256 = _mm_set1_epi16((unsigned short)__NVC_MASK_SCALE_255_256);
static const __m128i g_xmm_mask_rounding_8b = _mm_set1_epi16(32);
static const __m128i g_xmm_1_shift_14 = _mm_set1_epi16(1 << 14);




#ifdef _MSC_VER
#pragma warning(disable:4127)
#endif

// NOTE: both xmm_matte and xmm_mask are modified after processing
#define __MULTIPLY_MATTE_BY_MASK_8P_8BIT_INT(xmm_matte, xmm_mask, xmm_intensity, invert)							\
{																													\
	xmm_mask = _mm_slli_epi16(xmm_mask, 8);																			\
	xmm_mask = _mm_mulhi_epu16(xmm_mask, g_xmm_scale_255_256);	/* mask / 255 * 256   [p15b, w16b] */				\
																													\
	xmm_mask = _mm_mulhi_epu16(xmm_mask, xmm_intensity);		/* mask' = mask * intensity  [p14b, w15b] */		\
	if (invert)																										\
		xmm_mask = _mm_sub_epi16(g_xmm_1_shift_14, xmm_mask);	/* mask' = (1 - mask * intensity)  [p14b, w15b] */	\
																													\
	xmm_matte = _mm_slli_epi16(xmm_matte, 8);																		\
	xmm_matte = _mm_mulhi_epu16(xmm_matte, xmm_mask);			/* matte * mask'     [p14b, w14b] */				\
	xmm_matte = _mm_add_epi16(xmm_matte, g_xmm_mask_rounding_8b);													\
	xmm_matte = _mm_srli_epi16(xmm_matte, 6);					/* matte * mask'     [p8b, w8b]   */				\
}

// NOTE: both xmm_matte and xmm_mask are modified after processing
#define __MULTIPLY_MATTE_BY_MASK_4P_FLOAT(xmm_matte_ps, xmm_mask_ps, xmm_intensity_ps, invert)						\
{																													\
	xmm_mask_ps = _mm_mul_ps(xmm_mask_ps, xmm_intensity_ps);														\
	if (bInvertMask)																								\
		xmm_mask_ps = _mm_sub_ps(g_xmm_65535_ps, xmm_mask_ps);														\
	xmm_mask_ps = _mm_div_ps(xmm_mask_ps, g_xmm_65535_ps);															\
	xmm_matte_ps = _mm_mul_ps(xmm_matte_ps, xmm_mask_ps);															\
}

static uint8_t NVC_FORCE_INLINE __Multiply_Matte_By_Mask_8Bit(uint8_t matte,
															  uint8_t mask,
															  uint32_t intensity,	/* p15b w16b */
															  bool invert)
{
	uint32_t m;

	m = (((uint32_t)mask << 8) * __NVC_MASK_SCALE_255_256) >> 16;	/* mask / 255 * 256   [p15b, w16b] */

	m = (m * intensity) >> 16;	/* mask' = mask * intensity  [p14b, w15b] */
	if (invert)
		m = (1 << 14) - m;		/* mask' = (1 - mask * intensity)  [p14b, w15b] */

	return (uint8_t)((((((uint32_t)matte << 8) * m) >> 16) + 32) >> 6);	/* matte * mask'     [p8b, w8b]   */
}

static uint16_t NVC_FORCE_INLINE __Multiply_Matte_By_Mask_16Bit(uint16_t matte,
																float mask,
																float intensity,
																bool invert)
{
	float m = mask * intensity;
	if (invert)
		m = 65535 - m;

	m /= 65535;
	return (uint16_t)((float)matte * m + 0.5f);
}

static uint16_t NVC_FORCE_INLINE __Multiply_Matte_By_Mask_16Bit(uint16_t matte,
																uint16_t mask,
																float intensity,
																bool invert)
{
	return __Multiply_Matte_By_Mask_16Bit(matte, (float)mask, intensity, invert);
}

static inline float __Multiply_Matte_By_Mask_32Bit(float matte,
                                                   float mask,
                                                   float intensity,
                                                   bool invert)
{
    float m = mask * intensity;
    if (invert)
        m = 1 - m;

    return matte * m;
}


// NOTE: both xmm_color_src and xmm_color_dst are modified after processing
#define __INTERPOLATE_COLOR_BY_MASK_8P_CORE_8BIT_INT(xmm_color_src, xmm_color_dst, xmm_factor1, xmm_factor2)	\
{																												\
	xmm_color_src = _mm_slli_epi16(xmm_color_src, 8);															\
	xmm_color_src = _mm_mulhi_epu16(xmm_color_src, xmm_factor1);	/* src_color * factor1     [p14b, w14b] */	\
	xmm_color_dst = _mm_slli_epi16(xmm_color_dst, 8);															\
	xmm_color_dst = _mm_mulhi_epu16(xmm_color_dst, xmm_factor2);	/* dst_color * factor2     [p14b, w14b] */	\
	xmm_color_dst = _mm_add_epi16(xmm_color_dst, xmm_color_src);	/* src_color * factor1 + dst_color * factor2 [p14b, w14b] */	\
	xmm_color_dst = _mm_add_epi16(xmm_color_dst, g_xmm_mask_rounding_8b);										\
	xmm_color_dst = _mm_srli_epi16(xmm_color_dst, 6);				/* [p8b, w8b] */							\
}

// NOTE: xmm_color_src, xmm_color_dst and xmm_mask are modified after processing
#define __INTERPOLATE_COLOR_BY_MASK_8P_8BIT_INT(xmm_color_src, xmm_color_dst, xmm_mask, xmm_intensity, invert)		\
{																													\
	xmm_mask = _mm_slli_epi16(xmm_mask, 8);																			\
	xmm_mask = _mm_mulhi_epu16(xmm_mask, g_xmm_scale_255_256);	/* mask / 255 * 256   [p15b, w16b] */				\
																													\
	xmm_mask = _mm_mulhi_epu16(xmm_mask, xmm_intensity);		/* mask' = mask * intensity  [p14b, w15b] */		\
	if (invert)																										\
		xmm_mask = _mm_sub_epi16(g_xmm_1_shift_14, xmm_mask);	/* mask' = (1 - mask * intensity)  [p14b, w15b] */	\
																													\
	__m128i __xmm_1_m_mask = _mm_sub_epi16(g_xmm_1_shift_14, xmm_mask);	/* factor = 1 - mask' [p14b, w15b] */		\
																													\
	__INTERPOLATE_COLOR_BY_MASK_8P_CORE_8BIT_INT(xmm_color_src, xmm_color_dst, __xmm_1_m_mask, xmm_mask)			\
}

// NOTE: both xmm_color_dst_ps and xmm_mask_ps are modified after processing
#define __INTERPOLATE_COLOR_BY_MASK_4P_FLOAT(xmm_color_src_ps, xmm_color_dst_ps, xmm_mask_ps, xmm_intensity_ps, invert)	\
{																														\
	xmm_mask_ps = _mm_mul_ps(xmm_mask_ps, xmm_intensity_ps);															\
	if (invert)																											\
		xmm_mask_ps = _mm_sub_ps(g_xmm_65535_ps, xmm_mask_ps);															\
	xmm_mask_ps = _mm_div_ps(xmm_mask_ps, g_xmm_65535_ps);																\
	xmm_color_dst_ps = _mm_sub_ps(xmm_color_dst_ps, xmm_color_src_ps);													\
	xmm_color_dst_ps = _mm_mul_ps(xmm_color_dst_ps, xmm_mask_ps);														\
	xmm_color_dst_ps = _mm_add_ps(xmm_color_dst_ps, xmm_color_src_ps);													\
}

static uint8_t NVC_FORCE_INLINE __Intp_Color_By_Mask_8Bit(uint8_t matte_src,
														  uint8_t matte_dst,
														  uint8_t mask,
														  uint32_t intensity,	/* p15b w16b */
														  bool invert)
{
	uint32_t m, f;

	m = (((uint32_t)mask << 8) * __NVC_MASK_SCALE_255_256) >> 16;	/* mask / 255 * 256   [p15b, w16b] */

	m = (m * intensity) >> 16;	/* mask' = mask * intensity  [p14b, w15b] */
	if (invert)
		m = (1 << 14) - m;		/* mask' = (1 - mask * intensity)  [p14b, w15b] */

	f = (1 << 14) - m;			/* factor = 1 - mask' [p14b, w15b] */

	uint32_t tmp0, tmp1;

	tmp0 = ((matte_src << 8) * f) >> 16;	/* p14b w14b */
	tmp1 = ((matte_dst << 8) * m) >> 16;	/* p14b w14b */
	return (uint8_t)((tmp0 + tmp1 + 32) >> 6);
}

static uint16_t NVC_FORCE_INLINE __Intp_Color_By_Mask_16Bit(uint16_t matte_src,
															uint16_t matte_dst,
															float mask,
															float intensity,
															bool invert)
{
	float f = mask * intensity;
	if (invert)
		f = 65535 - f;

	f /= 65535;
	return (uint16_t)((float)((int)matte_dst - (int)matte_src) * f + 0.5f) + matte_src;
}

static uint16_t NVC_FORCE_INLINE __Intp_Color_By_Mask_16Bit(uint16_t matte_src,
															uint16_t matte_dst,
															uint16_t mask,
															float intensity,
															bool invert)
{
	return __Intp_Color_By_Mask_16Bit(matte_src, matte_dst, (float)mask, intensity, invert);
}

static inline float __Intp_Color_By_Mask_32Bit(float src_color,
                                               float dst_color,
                                               float mask,
                                               float intensity,
                                               bool invert)
{
	float m = mask * intensity;
	if (invert)
		m = 1 - m;

	return (dst_color - src_color) * m + src_color;
}

//
// Helper function to extract a channel from BGRA surface
//
template<typename T>
void __ExtractChannelFromBGRALine(const T *pSrcLine, T *pDstLine, int nChannelIndex, unsigned int nPixelCount)
{
    for (unsigned int i = 0; i < nPixelCount; i++)
        pDstLine[i] = pSrcLine[i * 4 + nChannelIndex];
}

//
// Helper function to generate 8 bit luma line
//
static void __GenerateLumaLine8Bit(const uint8_t *pSrc, uint8_t *pLuma, int nColorimetry, unsigned int nPixelCount)
{
    const unsigned int nGroupCount = nPixelCount / 16;
    const unsigned int nTailCount = nPixelCount & 15;
    for (unsigned int i = 0; i < nGroupCount; i++) {
        __m128i xmm_luma = __NvcCalc_8Bit_Luma_From_8Bit_BGRA_8Pix(pSrc, nColorimetry);
        __m128i xmm_luma2 = __NvcCalc_8Bit_Luma_From_8Bit_BGRA_8Pix(pSrc + 32, nColorimetry);
        xmm_luma = _mm_packus_epi16(xmm_luma, xmm_luma2);
        _mm_store_si128((__m128i *)pLuma, xmm_luma);

        pSrc += 64;
        pLuma += 16;
    }

    for (unsigned int i = 0; i < nTailCount; i++)
        pLuma[i] = __Nvc_Cacl_8Bit_Luma_From_8Bit_RGB(pSrc[i * 4 + 2], pSrc[i * 4 + 1], pSrc[i * 4 + 0], nColorimetry);
}

//
// Helper function to generate 16 bit luma line
//
static void __GenerateLumaLine16Bit(const uint16_t *pSrc, uint16_t *pLuma, int nColorimetry, unsigned int nPixelCount)
{
    const unsigned int nGroupCount = nPixelCount / 8;
    const unsigned int nTailCount = nPixelCount & 7;
    for (unsigned int i = 0; i < nGroupCount; i++) {
        const __m128i xmm_luma = __NvcCalc_16Bit_Luma_From_16Bit_BGRA_8Pix(pSrc, nColorimetry);
        _mm_store_si128((__m128i *)pLuma, xmm_luma);

        pSrc += 32;
        pLuma += 8;
    }

    for (unsigned int i = 0; i < nTailCount; i++)
        pLuma[i] = __Nvc_Cacl_16Bit_Luma_From_16Bit_RGB(pSrc[i * 4 + 2], pSrc[i * 4 + 1], pSrc[i * 4 + 0], nColorimetry);
}

//
// Helper function to generate 32 bit luma line
//
static void __GenerateLumaLine32Bit(const float *pSrc, float *pLuma, int nColorimetry, unsigned int nPixelCount)
{
    const unsigned int nGroupCount = nPixelCount / 4;
    const unsigned int nTailCount = nPixelCount & 3;
    for (unsigned int i = 0; i < nGroupCount; i++) {
        const __m128 xmm_luma_ps = __NvcCalc_32Bit_Luma_From_32Bit_BGRA_4Pix(pSrc, nColorimetry);
        _mm_store_ps(pLuma, xmm_luma_ps);

        pSrc += 16;
        pLuma += 4;
    }

    for (unsigned int i = 0; i < nTailCount; i++)
        pLuma[i] = __Nvc_Cacl_32Bit_Luma_From_32Bit_RGB(pSrc[i * 4 + 2], pSrc[i * 4 + 1], pSrc[i * 4 + 0], nColorimetry);
}

//
// Helper function to construct a mask line with the expected bit depth
//
static void __ConstructMaskLine_BGRA(const void *pSrcMaskLine,
                                ENvcBitDepth eSrcMaskDepth,
                                ENvcMaskType eSrcMaskType,
                                ENvcColorChannel eMaskChannel,
                                int nColorimetry,
                                void *pDstMaskLine,
                                ENvcBitDepth eDstMaskDepth,
                                unsigned int nPixelCount)
{
    const unsigned int nSrcChannelBytes = NvcGetBytesFromBitDepth(eSrcMaskDepth);

    if (eSrcMaskType == keNvcMaskTypeAlpha && eSrcMaskDepth == eDstMaskDepth) {
        memcpy(pDstMaskLine, pSrcMaskLine, nPixelCount * nSrcChannelBytes);
        return;
    }

    void *pAlphaMaskLine = (void *)pSrcMaskLine;

    if (eSrcMaskType != keNvcMaskTypeAlpha) {
        // In this case we must convert this BGRA source mask to a alpha only mask
        if (eSrcMaskDepth != eDstMaskDepth) {
            void *pTmpBuffer = _alloca(nPixelCount * nSrcChannelBytes + 127);
            pTmpBuffer = (void *)NVC_UP_ALIGN_PO2((size_t)pTmpBuffer, 128);
            pAlphaMaskLine = pTmpBuffer;
        } else {
            pAlphaMaskLine = pDstMaskLine;
        }

        if (eMaskChannel != keNvcColorChannelLuma) {
            int nChannelIndex = 0;
            switch (eMaskChannel) {
            case keNvcColorChannelBlue:
                nChannelIndex = 0;
                break;
            case keNvcColorChannelGreen:
                nChannelIndex = 1;
                break;
            case keNvcColorChannelRed:
                nChannelIndex = 2;
                break;
            case keNvcColorChannelAlpha:
                nChannelIndex = 3;
                break;
            }

            switch (eSrcMaskDepth) {
            case keNvcBitDepthInt8:
                __ExtractChannelFromBGRALine<uint8_t>((const uint8_t *)pSrcMaskLine, (uint8_t *)pAlphaMaskLine, nChannelIndex, nPixelCount);
                break;

            case keNvcBitDepthInt16:
                __ExtractChannelFromBGRALine<uint16_t>((const uint16_t *)pSrcMaskLine, (uint16_t *)pAlphaMaskLine, nChannelIndex, nPixelCount);
                break;

            case keNvcBitDepthFloat32:
                __ExtractChannelFromBGRALine<float>((const float *)pSrcMaskLine, (float *)pAlphaMaskLine, nChannelIndex, nPixelCount);
                break;
            }
        } else {
            // Extract luma channel from this BGRA source mask line
            switch (eSrcMaskDepth) {
            case keNvcBitDepthInt8:
                __GenerateLumaLine8Bit((const uint8_t *)pSrcMaskLine, (uint8_t *)pAlphaMaskLine, nColorimetry, nPixelCount);
                break;

            case keNvcBitDepthInt16:
                __GenerateLumaLine16Bit((const uint16_t *)pSrcMaskLine, (uint16_t *)pAlphaMaskLine, nColorimetry, nPixelCount);
                break;

            case keNvcBitDepthFloat32:
                __GenerateLumaLine32Bit((const float *)pSrcMaskLine, (float *)pAlphaMaskLine, nColorimetry, nPixelCount);
                break;
            }
        }
    }

    if (eSrcMaskDepth != eDstMaskDepth) {
        NvcColorSpaceConversion_ARGB(
            pAlphaMaskLine,
            0,
            keNvcColorSpace_Alpha,
            eSrcMaskDepth,
            pDstMaskLine,
            0,
            keNvcColorSpace_Alpha,
            eDstMaskDepth,
            nPixelCount,
            1,
            0,
            0,
            0);
    }
}

template<ENvcColorChannel channelType>
static inline void __Xmm_Get_8B_RGBChannel_From_8B_YUV422(
	__m128i xmm_y,
	__m128i xmm_u,
	__m128i xmm_v,
	const __Xmm_YUV_RGB_Params *params,
	__m128i& xmm_channel)
{
	xmm_y = _mm_sub_epi16(xmm_y, g_xmm_16bit_16);
	xmm_u = _mm_sub_epi16(xmm_u, g_xmm_16bit_128);
	xmm_v = _mm_sub_epi16(xmm_v, g_xmm_16bit_128);
	xmm_y = _mm_slli_epi16(xmm_y, 7);
	xmm_u = _mm_slli_epi16(xmm_u, 7);
	xmm_v = _mm_slli_epi16(xmm_v, 7);

	xmm_y = _mm_mulhi_epi16(xmm_y, params->xmm_Y);

	if (channelType == keNvcColorChannelRed) {
		// Calculate red
		xmm_channel = _mm_mulhi_epi16(xmm_v, params->xmm_V_R);
		xmm_channel = _mm_add_epi16(xmm_channel, xmm_y);
		xmm_channel = _mm_add_epi16(xmm_channel, g_xmm_16bit_8);
		xmm_channel = _mm_srai_epi16(xmm_channel, 4);
	}
	else if (channelType == keNvcColorChannelGreen) {
		// Calculate green
		xmm_channel = _mm_mulhi_epi16(xmm_u, params->xmm_U_G);
		xmm_channel = _mm_sub_epi16(xmm_y, xmm_channel);
		xmm_channel = _mm_sub_epi16(xmm_channel, _mm_mulhi_epi16(xmm_v, params->xmm_V_G));
		xmm_channel = _mm_add_epi16(xmm_channel, g_xmm_16bit_8);
		xmm_channel = _mm_srai_epi16(xmm_channel, 4);
	}
	else if (channelType == keNvcColorChannelBlue) {
		// Calculate blue
		xmm_channel = _mm_mulhi_epi16(xmm_u, params->xmm_U_B);
		xmm_channel = _mm_add_epi16(xmm_y, xmm_channel);
		xmm_channel = _mm_add_epi16(xmm_channel, g_xmm_16bit_8);
		xmm_channel = _mm_srai_epi16(xmm_channel, 4);
	}
}

static NVC_FORCE_INLINE __m128i __NvcCalc_8Bit_RGBChannel_From_8Bit_YUV422_16Pix(
	const void *pY, const void *pU, const void *pV, 
	ENvcColorChannel channelType, const __Xmm_YUV_RGB_Params *params)
{
	const bool bIsSrcAligned = ((size_t)pY & 15) ? false : true;

	__m128i xmm_Y, xmm_U, xmm_V;
	if (bIsSrcAligned) {
		xmm_Y = _mm_load_si128((__m128i *)pY);
	}
	else {
		xmm_Y = _mm_loadu_si128((__m128i *)pY);
	}

	xmm_U = _mm_loadl_epi64((__m128i *)pU);
	xmm_V = _mm_loadl_epi64((__m128i *)pV);
	xmm_U = _mm_unpacklo_epi8(xmm_U, _mm_setzero_si128());
	xmm_V = _mm_unpacklo_epi8(xmm_V, _mm_setzero_si128());

	__m128i xmm_tmp_y, xmm_tmp_u, xmm_tmp_v;
	xmm_tmp_y = _mm_unpacklo_epi8(xmm_Y, _mm_setzero_si128());
	xmm_tmp_u = _mm_unpacklo_epi16(xmm_U, xmm_U);
	xmm_tmp_v = _mm_unpacklo_epi16(xmm_V, xmm_V);
	
	__m128i xmm_channel1, xmm_channel2;

	if (channelType == keNvcColorChannelRed)
		__Xmm_Get_8B_RGBChannel_From_8B_YUV422<keNvcColorChannelRed>(xmm_tmp_y, xmm_tmp_u, xmm_tmp_v, params, xmm_channel1);
	else if (channelType == keNvcColorChannelGreen)
		__Xmm_Get_8B_RGBChannel_From_8B_YUV422<keNvcColorChannelGreen>(xmm_tmp_y, xmm_tmp_u, xmm_tmp_v, params, xmm_channel1);
	else if (channelType == keNvcColorChannelBlue)
		__Xmm_Get_8B_RGBChannel_From_8B_YUV422<keNvcColorChannelBlue>(xmm_tmp_y, xmm_tmp_u, xmm_tmp_v, params, xmm_channel1);


	xmm_tmp_y = _mm_unpackhi_epi8(xmm_Y, _mm_setzero_si128());
	xmm_tmp_u = _mm_unpackhi_epi16(xmm_U, xmm_U);
	xmm_tmp_v = _mm_unpackhi_epi16(xmm_V, xmm_V);

	if (channelType == keNvcColorChannelRed)
		__Xmm_Get_8B_RGBChannel_From_8B_YUV422<keNvcColorChannelRed>(xmm_tmp_y, xmm_tmp_u, xmm_tmp_v, params, xmm_channel2);
	else if (channelType == keNvcColorChannelGreen)
		__Xmm_Get_8B_RGBChannel_From_8B_YUV422<keNvcColorChannelGreen>(xmm_tmp_y, xmm_tmp_u, xmm_tmp_v, params, xmm_channel2);
	else if (channelType == keNvcColorChannelBlue)
		__Xmm_Get_8B_RGBChannel_From_8B_YUV422<keNvcColorChannelBlue>(xmm_tmp_y, xmm_tmp_u, xmm_tmp_v, params, xmm_channel2);

	return _mm_packus_epi16(xmm_channel1, xmm_channel2);
}

static void __GenerateRGBChannelLine8Bit(
	const uint8_t *pY, const uint8_t *pU, const uint8_t *pV, uint8_t *pChannel, 
	ENvcColorChannel channelType, int nColorimetry, unsigned int nPixelCount)
{
	const unsigned int nGroupCount = nPixelCount / 16;
	const unsigned int nTailCount = nPixelCount & 15;

	__Xmm_YUV_RGB_Params param;
	__Fill_Xmm_8B_YUV_8B_RGB_Params(nColorimetry, &param);
	for (unsigned int i = 0; i < nGroupCount; i++) {
		__m128i xmm_luma = __NvcCalc_8Bit_RGBChannel_From_8Bit_YUV422_16Pix(pY, pU, pV, channelType, &param);
		_mm_store_si128((__m128i *)pChannel, xmm_luma);

		pY += 16;
		pU += 8;
		pV += 8;
		pChannel += 16;
	}

	uint8_t r, g, b;
	for (unsigned int i = 0; i < nTailCount/2; i++) {
		NVC_8Bit_RGB_FROM_8Bit_YUV(r,g,b,pY[0], pU[0], pV[0], nColorimetry);
		if (channelType == keNvcColorChannelRed)
			pChannel[0] = r;
		else if (channelType == keNvcColorChannelGreen)
			pChannel[0] = g;
		else if (channelType == keNvcColorChannelBlue)
			pChannel[0] = b;

		pY++;
		pChannel++;

		NVC_8Bit_RGB_FROM_8Bit_YUV(r, g, b, pY[0], pU[0], pV[0], nColorimetry);
		if (channelType == keNvcColorChannelRed)
			pChannel[0] = r;
		else if (channelType == keNvcColorChannelGreen)
			pChannel[0] = g;
		else if (channelType == keNvcColorChannelBlue)
			pChannel[0] = b;
		pY++;
		pChannel++;
	}

	if (nTailCount & 1) {
		NVC_8Bit_RGB_FROM_8Bit_YUV(r, g, b, pY[0], pU[0], pV[0], nColorimetry);
		if (channelType == keNvcColorChannelRed)
			pChannel[0] = r;
		else if (channelType == keNvcColorChannelGreen)
			pChannel[0] = g;
		else if (channelType == keNvcColorChannelBlue)
			pChannel[0] = b;
	}
}

template<ENvcColorChannel channelType>
static inline void __Xmm_Get_16B_RGBChannel_From_10B_YUV422(
	__m128i xmm_y,
	__m128i xmm_u,
	__m128i xmm_v,
	const __Xmm_PS_YUV_RGB_Params *params,
	__m128i& xmm_channel)
{
	__m128i xmm_tmp_y, xmm_tmp_u, xmm_tmp_v, xmm_zero = _mm_setzero_si128();
	__m128 xmm_channel_ps,xmm_tmp_ps, xmm_y_ps, xmm_u_ps, xmm_v_ps;

	xmm_tmp_y = _mm_unpacklo_epi16(xmm_y, xmm_zero);
	xmm_tmp_u = _mm_unpacklo_epi16(xmm_u, xmm_zero);
	xmm_tmp_v = _mm_unpacklo_epi16(xmm_v, xmm_zero);
	xmm_tmp_y = _mm_sub_epi32(xmm_tmp_y, g_xmm_32bit_64);
	xmm_tmp_u = _mm_sub_epi32(xmm_tmp_u, g_xmm_32bit_512);
	xmm_tmp_v = _mm_sub_epi32(xmm_tmp_v, g_xmm_32bit_512);
	xmm_y_ps = _mm_cvtepi32_ps(xmm_tmp_y);
	xmm_u_ps = _mm_cvtepi32_ps(xmm_tmp_u);
	xmm_v_ps = _mm_cvtepi32_ps(xmm_tmp_v);

	xmm_y_ps = _mm_mul_ps(xmm_y_ps, params->xmm_Y);
	if (channelType == keNvcColorChannelRed) {
		xmm_channel_ps = _mm_mul_ps(xmm_v_ps, params->xmm_V_R);
		xmm_channel_ps = _mm_add_ps(xmm_channel_ps, xmm_y_ps);
		xmm_channel_ps = _mm_mul_ps(xmm_channel_ps, g_xmm_ps_64);
		xmm_channel = _mm_cvtps_epi32(xmm_channel_ps);
	}
	else if (channelType == keNvcColorChannelGreen) {
		xmm_channel_ps = _mm_mul_ps(xmm_u_ps, params->xmm_U_G);
		xmm_channel_ps = _mm_sub_ps(xmm_y_ps, xmm_channel_ps);
		xmm_tmp_ps = _mm_mul_ps(xmm_v_ps, params->xmm_V_G);
		xmm_channel_ps = _mm_sub_ps(xmm_channel_ps, xmm_tmp_ps);
		xmm_channel_ps = _mm_mul_ps(xmm_channel_ps, g_xmm_ps_64);
		xmm_channel = _mm_cvtps_epi32(xmm_channel_ps);
	}
	else if (channelType == keNvcColorChannelBlue) {
		xmm_channel_ps = _mm_mul_ps(xmm_u_ps, params->xmm_U_B);
		xmm_channel_ps = _mm_add_ps(xmm_channel_ps, xmm_y_ps);
		xmm_channel_ps = _mm_mul_ps(xmm_channel_ps, g_xmm_ps_64);
		xmm_channel = _mm_cvtps_epi32(xmm_channel_ps);
	}
}

static NVC_FORCE_INLINE __m128i __NvcCalc_16Bit_RGBChannel_From_10Bit_YUV422_8Pix(
	const void *pY, const void *pU, const void *pV,
	ENvcColorChannel channelType, const __Xmm_PS_YUV_RGB_Params *params)
{
	const bool bIsSrcAligned = ((size_t)pY & 15) ? false : true;

	__m128i xmm_Y, xmm_U, xmm_V;
	if (bIsSrcAligned) {
		xmm_Y = _mm_load_si128((__m128i *)pY);
	}
	else {
		xmm_Y = _mm_loadu_si128((__m128i *)pY);
	}

	xmm_U = _mm_loadl_epi64((__m128i *)pU);
	xmm_V = _mm_loadl_epi64((__m128i *)pV);
	xmm_U = _mm_unpacklo_epi16(xmm_U, _mm_setzero_si128());
	xmm_V = _mm_unpacklo_epi16(xmm_V, _mm_setzero_si128());

	__m128i xmm_tmp_y, xmm_tmp_u, xmm_tmp_v;
	xmm_tmp_y = _mm_unpacklo_epi16(xmm_Y, _mm_setzero_si128());
	xmm_tmp_u = _mm_unpacklo_epi32(xmm_U, xmm_U);
	xmm_tmp_v = _mm_unpacklo_epi32(xmm_V, xmm_V);

	__m128i xmm_channel1, xmm_channel2;

	if (channelType == keNvcColorChannelRed)
		__Xmm_Get_16B_RGBChannel_From_10B_YUV422<keNvcColorChannelRed>(xmm_tmp_y, xmm_tmp_u, xmm_tmp_v, params, xmm_channel1);
	else if (channelType == keNvcColorChannelGreen)
		__Xmm_Get_16B_RGBChannel_From_10B_YUV422<keNvcColorChannelGreen>(xmm_tmp_y, xmm_tmp_u, xmm_tmp_v, params, xmm_channel1);
	else if (channelType == keNvcColorChannelBlue)
		__Xmm_Get_16B_RGBChannel_From_10B_YUV422<keNvcColorChannelBlue>(xmm_tmp_y, xmm_tmp_u, xmm_tmp_v, params, xmm_channel1);


	xmm_tmp_y = _mm_unpackhi_epi16(xmm_Y, _mm_setzero_si128());
	xmm_tmp_u = _mm_unpackhi_epi32(xmm_U, xmm_U);
	xmm_tmp_v = _mm_unpackhi_epi32(xmm_V, xmm_V);

	if (channelType == keNvcColorChannelRed)
		__Xmm_Get_16B_RGBChannel_From_10B_YUV422<keNvcColorChannelRed>(xmm_tmp_y, xmm_tmp_u, xmm_tmp_v, params, xmm_channel2);
	else if (channelType == keNvcColorChannelGreen)
		__Xmm_Get_16B_RGBChannel_From_10B_YUV422<keNvcColorChannelGreen>(xmm_tmp_y, xmm_tmp_u, xmm_tmp_v, params, xmm_channel2);
	else if (channelType == keNvcColorChannelBlue)
		__Xmm_Get_16B_RGBChannel_From_10B_YUV422<keNvcColorChannelBlue>(xmm_tmp_y, xmm_tmp_u, xmm_tmp_v, params, xmm_channel2);

	xmm_channel1 = _mm_sub_epi32(xmm_channel1, g_xmm_32bit_32768);
	xmm_channel2 = _mm_sub_epi32(xmm_channel2, g_xmm_32bit_32768);
	xmm_channel1 = _mm_packs_epi32(xmm_channel1, xmm_channel2);
	return _mm_add_epi16(xmm_channel1, g_xmm_16bit_32768);
}

//
// Helper function to generate 16 bit luma line
//
static void __GenerateRGBChannelLine16Bit(
	const uint16_t *pY, const uint16_t *pU, const uint16_t *pV, uint16_t *pChannel,
	ENvcColorChannel channelType, int nColorimetry, unsigned int nPixelCount)
{
	const unsigned int nGroupCount = nPixelCount / 8;
	const unsigned int nTailCount = nPixelCount & 7;
	__Xmm_PS_YUV_RGB_Params param;
	__Fill_xmm_10B_YUV_16B_RGB_Params(nColorimetry, &param);
	for (unsigned int i = 0; i < nGroupCount; i++) {
		__m128i xmm_luma = __NvcCalc_16Bit_RGBChannel_From_10Bit_YUV422_8Pix(pY, pU, pV, channelType, &param);
		_mm_store_si128((__m128i *)pChannel, xmm_luma);

		pY += 8;
		pU += 4;
		pV += 4;
		pChannel += 8;
	}

	uint8_t r = 0, g = 0, b = 0;
	for (unsigned int i = 0; i < nTailCount / 2; i++) {
		if (channelType == keNvcColorChannelRed)
			pChannel[0] = NVC_16Bit_R_FROM_10Bit_YUV(pY[0], pU[0], pV[0], nColorimetry);
		else if (channelType == keNvcColorChannelGreen)
			pChannel[0] = NVC_16Bit_G_FROM_10Bit_YUV(pY[0], pU[0], pV[0], nColorimetry);
		else if (channelType == keNvcColorChannelBlue)
			pChannel[0] = NVC_16Bit_B_FROM_10Bit_YUV(pY[0], pU[0], pV[0], nColorimetry);

		pY++;
		pChannel++;

		if (channelType == keNvcColorChannelRed)
			pChannel[0] = NVC_16Bit_R_FROM_10Bit_YUV(pY[0], pU[0], pV[0], nColorimetry);
		else if (channelType == keNvcColorChannelGreen)
			pChannel[0] = NVC_16Bit_G_FROM_10Bit_YUV(pY[0], pU[0], pV[0], nColorimetry);
		else if (channelType == keNvcColorChannelBlue)
			pChannel[0] = NVC_16Bit_B_FROM_10Bit_YUV(pY[0], pU[0], pV[0], nColorimetry);
		pY++;
		pChannel++;
	}

	if (nTailCount & 1) {
		if (channelType == keNvcColorChannelRed)
			pChannel[0] = NVC_16Bit_R_FROM_10Bit_YUV(pY[0], pU[0], pV[0], nColorimetry);
		else if (channelType == keNvcColorChannelGreen)
			pChannel[0] = NVC_16Bit_G_FROM_10Bit_YUV(pY[0], pU[0], pV[0], nColorimetry);
		else if (channelType == keNvcColorChannelBlue)
			pChannel[0] = NVC_16Bit_B_FROM_10Bit_YUV(pY[0], pU[0], pV[0], nColorimetry);
	}
}

static void __ConstructMaskLine_YUVA(const SNvcBufferPlaneInfo *pSrcMaskLine,
								ENvcBitDepth eSrcMaskDepth,
								ENvcMaskType eSrcMaskType,
								ENvcColorChannel eMaskChannel,
								int nColorimetry,
								void *pDstMaskLine,
								ENvcBitDepth eDstMaskDepth,
								unsigned int nPixelCount)
{
	const unsigned int nSrcChannelBytes = NvcGetBytesFromBitDepth(eSrcMaskDepth);

	if (eSrcMaskType == keNvcMaskTypeYUVA422Planar && eSrcMaskDepth == eDstMaskDepth && 
		eMaskChannel == keNvcColorChannelAlpha) {
		if (pSrcMaskLine->nPlanarCount > 3)
			memcpy(pDstMaskLine, pSrcMaskLine->pDataPtr[3], nPixelCount * nSrcChannelBytes);
		else
			memset(pDstMaskLine, 255, nPixelCount * nSrcChannelBytes);
		return;
	}

	void *pAlphaMaskLine = (void *)pSrcMaskLine;
	if (eSrcMaskType != keNvcMaskTypeAlpha) {
		// In this case we must convert this yuva source mask to a alpha only mask
		if (eSrcMaskDepth != eDstMaskDepth) {
			void *pTmpBuffer = _alloca(nPixelCount * nSrcChannelBytes + 127);
			pTmpBuffer = (void *)NVC_UP_ALIGN_PO2((size_t)pTmpBuffer, 128);
			pAlphaMaskLine = pTmpBuffer;
		}
		else {
			pAlphaMaskLine = pDstMaskLine;
		}

		if (eMaskChannel == keNvcColorChannelAlpha ||
			eMaskChannel == keNvcColorChannelLuma) {
			int nChannelIndex = 0;
			switch (eMaskChannel) {
			case keNvcColorChannelAlpha:
				nChannelIndex = 3;
				break;
			}

			if (pSrcMaskLine->nPlanarCount > 3)
				memcpy(pDstMaskLine, pSrcMaskLine->pDataPtr[3], nPixelCount * nSrcChannelBytes);
			else
				memset(pDstMaskLine, 255, nPixelCount * nSrcChannelBytes);
		}
		else {
			// Extract luma channel from this YUV source mask line
			switch (eSrcMaskDepth) {
			case keNvcBitDepthInt8:
				__GenerateRGBChannelLine8Bit(
					(const uint8_t *)pSrcMaskLine->pDataPtr[0], 
					(const uint8_t *)pSrcMaskLine->pDataPtr[1],
					(const uint8_t *)pSrcMaskLine->pDataPtr[2],
					(uint8_t *)pAlphaMaskLine, 
					eMaskChannel,
					nColorimetry, 
					nPixelCount);
				break;

			case keNvcBitDepthInt16:
				__GenerateRGBChannelLine16Bit(
					(const uint16_t *)pSrcMaskLine->pDataPtr[0],
					(const uint16_t *)pSrcMaskLine->pDataPtr[1],
					(const uint16_t *)pSrcMaskLine->pDataPtr[2],
					(uint16_t *)pAlphaMaskLine,
					eMaskChannel,
					nColorimetry,
					nPixelCount);
				break;
			}
		}
	}
}

//
// Helper function to construct a mask line with the expected bit depth
//
static void __ConstructMaskLine(
	const SNvcBufferPlaneInfo *pSrcMaskLine,
	ENvcBitDepth eSrcMaskDepth,
	ENvcMaskType eSrcMaskType,
	ENvcColorChannel eMaskChannel,
	int nColorimetry,
	void *pDstMaskLine,
	ENvcBitDepth eDstMaskDepth,
	unsigned int nPixelCount)
{	
	if (eSrcMaskType == keNvcMaskTypeYUV422Planar || eSrcMaskType == keNvcMaskTypeYUVA422Planar)
		return __ConstructMaskLine_YUVA(pSrcMaskLine, eSrcMaskDepth, eSrcMaskType, eMaskChannel, nColorimetry, pDstMaskLine, eDstMaskDepth, nPixelCount);
	else
		return __ConstructMaskLine_BGRA(pSrcMaskLine->pDataPtr[0], eSrcMaskDepth, eSrcMaskType, eMaskChannel, nColorimetry, pDstMaskLine, eDstMaskDepth, nPixelCount);
}

static const __m128i g_xmm_255_8b = _mm_set1_epi8((unsigned char)255);

void DoMultiplyMatteLineByMask8Bit(uint8_t *pMatteLine,
								   const uint8_t *pMaskLine,
								   unsigned int nCount,
								   float fIntensity,
								   bool bInvertMask)
{
	const unsigned int nIntensity = (unsigned int)(fIntensity * 32768 + 0.5);
	const __m128i xmm_intensity = _mm_set1_epi16((unsigned short)nIntensity);

    const unsigned int nGroupCount = nCount / 16;
	const unsigned int nTailCount = nCount & 15;

	uint8_t *pMatte = pMatteLine;
	const uint8_t *pMask = pMaskLine;

	for (unsigned int i = 0; i < nGroupCount; i++, pMatte += 16, pMask += 16) {
		// process 16 pixels each time
		__m128i xmm_mask, xmm_mask2;

        xmm_mask = _mm_load_si128((__m128i *)pMask);            // [m15|m14|m13|m12|m11|m10| m9| m8| m7| m6| m5| m4| m3| m2| m1| m0]
        if (_mm_movemask_epi8(_mm_cmpeq_epi8(xmm_mask, g_xmm_zero)) == 0xFFFF) {
            if (!bInvertMask)
                _mm_store_si128((__m128i *)pMatte, g_xmm_zero);
            continue;
        } else if (nIntensity == 32768 && _mm_movemask_epi8(_mm_cmpeq_epi8(xmm_mask, g_xmm_255_8b)) == 0xFFFF) {
            if (bInvertMask)
                _mm_store_si128((__m128i *)pMatte, g_xmm_zero);
            continue;
        }

        xmm_mask2 = _mm_unpackhi_epi8(xmm_mask, g_xmm_zero);    // [  m15  |  m14  |  m13  |  m12  |  m11  |  m10  |   m9  |   m8  ]
        xmm_mask = _mm_unpacklo_epi8(xmm_mask, g_xmm_zero);     // [   m7  |   m6  |   m5  |   m4  |   m3  |   m2  |   m1  |   m0  ]

        __m128i xmm_matte, xmm_matte2;
		xmm_matte = _mm_load_si128((__m128i *)pMatte);          // [a15|a14|a13|a12|a11|a10| a9| a8| a7| a6| a5| a4| a3| a2| a1| a0]
        xmm_matte2 = _mm_unpackhi_epi8(xmm_matte, g_xmm_zero);  // [  a15  |  a14  |  a13  |  a12  |  a11  |  a10  |   a9  |   a8  ]
		xmm_matte = _mm_unpacklo_epi8(xmm_matte, g_xmm_zero);	// [   a7  |   a6  |   a5  |   a4  |   a3  |   a2  |   a1  |   a0  ]

		__MULTIPLY_MATTE_BY_MASK_8P_8BIT_INT(xmm_matte, xmm_mask, xmm_intensity, bInvertMask);
        __MULTIPLY_MATTE_BY_MASK_8P_8BIT_INT(xmm_matte2, xmm_mask2, xmm_intensity, bInvertMask);

		xmm_matte = _mm_packus_epi16(xmm_matte, xmm_matte2);
		_mm_store_si128((__m128i *)pMatte, xmm_matte);
	}

	for (unsigned int i = 0; i < nTailCount; i++)
		pMatte[i] = __Multiply_Matte_By_Mask_8Bit(pMatte[i], pMask[i], nIntensity, bInvertMask);
}

static const __m128i g_xmm_65535_16b = _mm_set1_epi16((unsigned short)65535);

void DoMultiplyMatteLineByMask16Bit(uint16_t *pMatteLine,
									const uint16_t *pMaskLine,
									unsigned int nCount,
									float fIntensity,
									bool bInvertMask)
{
	const __m128 xmm_intensity_ps = _mm_set1_ps(fIntensity);

    const unsigned int nGroupCount = nCount / 8;
	const unsigned int nTailCount = nCount & 7;

	uint16_t *pMatte = pMatteLine;
	const uint16_t *pMask = pMaskLine;

	for (unsigned int i = 0; i < nGroupCount; i++, pMatte += 8, pMask += 8) {
		// process 8 pixels each time
		__m128i xmm_matte, xmm_mask, xmm_matte1, xmm_matte2;
		__m128 xmm_matte_ps, xmm_mask_ps;

        xmm_mask = _mm_load_si128((__m128i *)pMask);	// [   m7  |   m6  |   m5  |   m4  |   m3  |   m2  |   m1  |   m0  ]
        if (_mm_movemask_epi8(_mm_cmpeq_epi16(xmm_mask, g_xmm_zero)) == 0xFFFF) {
            if (!bInvertMask)
                _mm_store_si128((__m128i *)pMatte, g_xmm_zero);
            continue;
        } else if (fIntensity == 1 && _mm_movemask_epi8(_mm_cmpeq_epi16(xmm_mask, g_xmm_65535_16b)) == 0xFFFF) {
            if (bInvertMask)
                _mm_store_si128((__m128i *)pMatte, g_xmm_zero);
            continue;
        }

		xmm_matte = _mm_load_si128((__m128i *)pMatte);	// [   a7  |   a6  |   a5  |   a4  |   a3  |   a2  |   a1  |   a0  ]
		xmm_matte_ps = _mm_cvtepi32_ps(_mm_unpacklo_epi16(xmm_matte, g_xmm_zero));	// [       a3      |       a2      |       a1      |       a0      ]
		xmm_mask_ps = _mm_cvtepi32_ps(_mm_unpacklo_epi16(xmm_mask, g_xmm_zero));	// [       m3      |       m2      |       m1      |       m0      ]
		__MULTIPLY_MATTE_BY_MASK_4P_FLOAT(xmm_matte_ps, xmm_mask_ps, xmm_intensity_ps, bInvertMask);
		xmm_matte1 = _mm_cvtps_epi32(xmm_matte_ps);									// [      a'3      |      a'2      |      a'1      |      a'0      ]
		xmm_matte1 = _mm_sub_epi32(xmm_matte1, g_xmm_32bit_32768);

		xmm_matte_ps = _mm_cvtepi32_ps(_mm_unpackhi_epi16(xmm_matte, g_xmm_zero));	// [       a7      |       a6      |       a5      |       a4      ]
		xmm_mask_ps = _mm_cvtepi32_ps(_mm_unpackhi_epi16(xmm_mask, g_xmm_zero));	// [       m7      |       m6      |       m5      |       m4      ]
		__MULTIPLY_MATTE_BY_MASK_4P_FLOAT(xmm_matte_ps, xmm_mask_ps, xmm_intensity_ps, bInvertMask);
		xmm_matte2 = _mm_cvtps_epi32(xmm_matte_ps);									// [      a'7      |      a'6      |      a'5      |      a'4      ]
		xmm_matte2 = _mm_sub_epi32(xmm_matte2, g_xmm_32bit_32768);

		xmm_matte1 = _mm_packs_epi32(xmm_matte1, xmm_matte2);		// [  a'7  |  a'6  |  a'5  |  a'4  |  a'3  |  a'2  |  a'1  |  a'0  ]
		xmm_matte1 = _mm_add_epi16(xmm_matte1, g_xmm_16bit_32768);	// [  a'7  |  a'6  |  a'5  |  a'4  |  a'3  |  a'2  |  a'1  |  a'0  ]
		_mm_store_si128((__m128i *)pMatte, xmm_matte1);
	}

	for (unsigned int i = 0; i < nTailCount; i++)
		pMatte[i] = __Multiply_Matte_By_Mask_16Bit(pMatte[i], pMask[i], fIntensity, bInvertMask);
}

void DoMultiplyMatteLineByMask32Bit(float *pMatteLine,
									const float *pMaskLine,
									unsigned int nCount,
									float fIntensity,
									bool bInvertMask)
{
	const __m128 xmm_intensity_ps = _mm_set1_ps(fIntensity);
    const __m128 xmm_zero_ps = _mm_setzero_ps();

	float *pMatte = pMatteLine;
	const float *pMask = pMaskLine;

    unsigned int nGroupCount = nCount / 4;
    unsigned int nTailCount = nCount & 3;

	for (unsigned int i = 0; i < nGroupCount; i++, pMatte += 4, pMask += 4) {
		// process 4 pixels each time
        union {
            float f[4];
            __m128 ps;
        } uMask;

        uMask.ps = _mm_load_ps(pMask);        // [       m3      |       m2      |       m1      |       m0      ]
        uMask.ps = _mm_mul_ps(uMask.ps, xmm_intensity_ps);
        if (bInvertMask)
            uMask.ps = _mm_sub_ps(g_xmm_1_ps, uMask.ps);

        if (_mm_movemask_ps(_mm_cmpeq_ps(uMask.ps, xmm_zero_ps)) == 0xF) {
            _mm_store_ps(pMatte, xmm_zero_ps);
            continue;
        } else if (_mm_movemask_ps(_mm_cmpeq_ps(uMask.ps, g_xmm_1_ps)) == 0xF) {
            continue;
        }

        // NOTE: we can't use SSE to process matte masking, since INF * 0 = NAN !!!
        for (int k = 0; k < 4; k++) {
            const float fMask = uMask.f[k];
            if (fMask == 0)
                pMatte[k] = 0;
            else
                pMatte[k] *= fMask;
        }
	}

    for (unsigned int i = 0; i < nTailCount; i++)
        pMatte[i] = __Multiply_Matte_By_Mask_32Bit(pMatte[i], pMask[i], fIntensity, bInvertMask);
}

// multiply matte by mask in place
void __MultiplyMatteLineByMask(void *pMatteLine,
							   ENvcBitDepth eBitDepth,
							   const SNvcMaskInfo *pMaskInfo,
							   int nMaskLineIdx,
							   int nPixelCount)
{
	void *pMaskLine = (char *)pMaskInfo->pMaskBits + pMaskInfo->nMaskPitch * nMaskLineIdx;
    if (pMaskInfo->eType != keNvcMaskTypeAlpha || pMaskInfo->eBitDepth != eBitDepth) {
        // Construct alpha only mask line with the specified depth
        void *pTmpBuffer = _alloca(nPixelCount * NvcGetBytesFromBitDepth(eBitDepth) + 127);
        pTmpBuffer = (void *)NVC_UP_ALIGN_PO2((size_t)pTmpBuffer, 128);
        const int nColorimetry =  NvcGetColorimetry(pMaskInfo->eColorimetry);
		SNvcBufferPlaneInfo maskline;
		if (pMaskInfo->eType == keNvcMaskTypeYUV422Planar || pMaskInfo->eType == keNvcMaskTypeYUVA422Planar) {
			maskline.nPlanarCount = pMaskInfo->sPlaneInfo.nPlanarCount;
			for (int i = 0; i < maskline.nPlanarCount; i++) {
				maskline.pDataPtr[i] = (char *)pMaskInfo->sPlaneInfo.pDataPtr[i] + nMaskLineIdx * pMaskInfo->sPlaneInfo.nPitch[i];
				maskline.nPitch[i] = pMaskInfo->sPlaneInfo.nPitch[i];
			}
		}
		else {
			maskline.nPlanarCount = 1;
			maskline.pDataPtr[0] = pMaskLine;
			maskline.nPitch[0] = pMaskInfo->nMaskPitch;
		}
        __ConstructMaskLine(&maskline, pMaskInfo->eBitDepth, pMaskInfo->eType, pMaskInfo->eChannel, nColorimetry, pTmpBuffer, eBitDepth, nPixelCount);
        pMaskLine = pTmpBuffer;
    }

    switch (eBitDepth) {
    case keNvcBitDepthInt8:
        DoMultiplyMatteLineByMask8Bit((uint8_t *)pMatteLine, (uint8_t *)pMaskLine,
            nPixelCount, pMaskInfo->fIntensity, pMaskInfo->bInvert);
        break;

    case keNvcBitDepthInt16:
        DoMultiplyMatteLineByMask16Bit((uint16_t *)pMatteLine, (uint16_t *)pMaskLine,
            nPixelCount, pMaskInfo->fIntensity, pMaskInfo->bInvert);
        break;

    case keNvcBitDepthFloat32:
        DoMultiplyMatteLineByMask32Bit((float *)pMatteLine, (float *)pMaskLine,
            nPixelCount, pMaskInfo->fIntensity, pMaskInfo->bInvert);
        break;
    }
}

void DoIntpMatteLineByMask8Bit(uint8_t *pSrcLine,
                               uint8_t *pDstLine,
                               const uint8_t *pMaskLine,
                               unsigned int nCount,
                               float fIntensity,
                               bool bInvertMask)
{
    const unsigned int nIntensity = (unsigned int)(fIntensity * 32768 + 0.5);
    const __m128i xmm_intensity = _mm_set1_epi16((unsigned short)nIntensity);

    const unsigned int nGroupCount = nCount / 16;
    const unsigned int nTailCount = nCount & 15;

	uint8_t *pSrc = pSrcLine, *pDst = pDstLine;
	const uint8_t *pMask = pMaskLine;

	for (unsigned int i = 0; i < nGroupCount; i++, pSrc += 16, pDst += 16, pMask += 16) {
		// process 16 pixels each time
        __m128i xmm_mask, xmm_mask2;
        xmm_mask = _mm_load_si128((__m128i *)pMask);    // [m15|m14|m13|m12|m11|m10| m9| m8| m7| m6| m5| m4| m3| m2| m1| m0]
        if (_mm_movemask_epi8(_mm_cmpeq_epi8(xmm_mask, g_xmm_zero)) == 0xFFFF) {
            if (!bInvertMask)
                memcpy(pDst, pSrc, 16);
            continue;
        } else if (nIntensity == 32768 && _mm_movemask_epi8(_mm_cmpeq_epi8(xmm_mask, g_xmm_255_8b)) == 0xFFFF) {
            if (bInvertMask)
                memcpy(pDst, pSrc, 16);
            continue;
        }

        xmm_mask2 = _mm_unpackhi_epi8(xmm_mask, g_xmm_zero);
        xmm_mask = _mm_unpacklo_epi8(xmm_mask, g_xmm_zero);

		__m128i xmm_matte_src, xmm_matte_src2, xmm_matte_dst, xmm_matte_dst2;

		xmm_matte_src = _mm_load_si128((__m128i *)pSrc);
        xmm_matte_src2 = _mm_unpackhi_epi8(xmm_matte_src, g_xmm_zero);
        xmm_matte_src = _mm_unpacklo_epi8(xmm_matte_src, g_xmm_zero);

        xmm_matte_dst = _mm_load_si128((__m128i *)pDst);
        xmm_matte_dst2 = _mm_unpackhi_epi8(xmm_matte_dst, g_xmm_zero);
        xmm_matte_dst = _mm_unpacklo_epi8(xmm_matte_dst, g_xmm_zero);

		__INTERPOLATE_COLOR_BY_MASK_8P_8BIT_INT(xmm_matte_src, xmm_matte_dst, xmm_mask, xmm_intensity, bInvertMask);
        __INTERPOLATE_COLOR_BY_MASK_8P_8BIT_INT(xmm_matte_src2, xmm_matte_dst2, xmm_mask2, xmm_intensity, bInvertMask);

		xmm_matte_dst = _mm_packus_epi16(xmm_matte_dst, xmm_matte_dst2);
		_mm_store_si128((__m128i *)pDst, xmm_matte_dst);
	}

	for (unsigned int i = 0; i < nTailCount; i++)
		pDst[i] = __Intp_Color_By_Mask_8Bit(pSrc[i], pDst[i], pMask[i], nIntensity, bInvertMask);
}

void DoIntpMatteLineByMask16Bit(uint16_t *pSrcLine,
                                uint16_t *pDstLine,
                                const uint16_t *pMaskLine,
                                unsigned int nCount,
                                float fIntensity,
                                bool bInvertMask)
{
    const __m128 xmm_intensity_ps = _mm_set1_ps(fIntensity);

    const unsigned int nGroupCount = nCount / 8;
    const unsigned int nTailCount = nCount & 7;

	uint16_t *pSrc = pSrcLine, *pDst = pDstLine;
	const uint16_t *pMask = pMaskLine;

    for (unsigned int i = 0; i < nGroupCount; i++, pSrc += 8, pDst += 8, pMask += 8) {
		// process 8 pixels each time
		__m128i xmm_matte_src, xmm_matte_dst, xmm_mask, xmm_matte1, xmm_matte2;
		__m128 xmm_matte_src_ps, xmm_matte_dst_ps, xmm_mask_ps;

        xmm_mask = _mm_load_si128((__m128i *)pMask);		// [   m7  |   m6  |   m5  |   m4  |   m3  |   m2  |   m1  |   m0  ]
        if (_mm_movemask_epi8(_mm_cmpeq_epi16(xmm_mask, g_xmm_zero)) == 0xFFFF) {
            if (!bInvertMask)
                memcpy(pDst, pSrc, 16);
            continue;
        } else if (fIntensity == 1 && _mm_movemask_epi8(_mm_cmpeq_epi16(xmm_mask, g_xmm_65535_16b)) == 0xFFFF) {
            if (bInvertMask)
                memcpy(pDst, pSrc, 16);
            continue;
        }

		xmm_matte_src = _mm_load_si128((__m128i *)pSrc);	// [  sa7  |  sa6  |  sa5  |  sa4  |  sa3  |  sa2  |  sa1  |  sa0  ]
		xmm_matte_dst = _mm_load_si128((__m128i *)pDst);	// [  da7  |  da6  |  da5  |  da4  |  da3  |  da2  |  da1  |  da0  ]
		
		xmm_matte_src_ps = _mm_cvtepi32_ps(_mm_unpacklo_epi16(xmm_matte_src, g_xmm_zero));	// [      sa3      |      sa2      |      sa1      |      sa0      ]
		xmm_matte_dst_ps = _mm_cvtepi32_ps(_mm_unpacklo_epi16(xmm_matte_dst, g_xmm_zero));	// [      da3      |      da2      |      da1      |      da0      ]
		xmm_mask_ps = _mm_cvtepi32_ps(_mm_unpacklo_epi16(xmm_mask, g_xmm_zero));			// [       m3      |       m2      |       m1      |       m0      ]
		__INTERPOLATE_COLOR_BY_MASK_4P_FLOAT(xmm_matte_src_ps, xmm_matte_dst_ps, xmm_mask_ps, xmm_intensity_ps, bInvertMask);
		xmm_matte1 = _mm_cvtps_epi32(xmm_matte_dst_ps);				// [      a'3      |      a'2      |      a'1      |      a'0      ]
		xmm_matte1 = _mm_sub_epi32(xmm_matte1, g_xmm_32bit_32768);

		xmm_matte_src_ps = _mm_cvtepi32_ps(_mm_unpackhi_epi16(xmm_matte_src, g_xmm_zero));	// [      sa7      |      sa6      |      sa5      |      sa4      ]
		xmm_matte_dst_ps = _mm_cvtepi32_ps(_mm_unpackhi_epi16(xmm_matte_dst, g_xmm_zero));	// [      da7      |      da6      |      da5      |      da4      ]
		xmm_mask_ps = _mm_cvtepi32_ps(_mm_unpackhi_epi16(xmm_mask, g_xmm_zero));			// [       m7      |       m6      |       m5      |       m4      ]
		__INTERPOLATE_COLOR_BY_MASK_4P_FLOAT(xmm_matte_src_ps, xmm_matte_dst_ps, xmm_mask_ps, xmm_intensity_ps, bInvertMask);
		xmm_matte2 = _mm_cvtps_epi32(xmm_matte_dst_ps);				// [      a'7      |      a'6      |      a'5      |      a'4      ]
		xmm_matte2 = _mm_sub_epi32(xmm_matte2, g_xmm_32bit_32768);

		xmm_matte1 = _mm_packs_epi32(xmm_matte1, xmm_matte2);		// [  a'7  |  a'6  |  a'5  |  a'4  |  a'3  |  a'2  |  a'1  |  a'0  ]
		xmm_matte1 = _mm_add_epi16(xmm_matte1, g_xmm_16bit_32768);	// [  a'7  |  a'6  |  a'5  |  a'4  |  a'3  |  a'2  |  a'1  |  a'0  ]
		_mm_store_si128((__m128i *)pDst, xmm_matte1);
	}

	for (unsigned int i = 0; i < nTailCount; i++)
		pDst[i] = __Intp_Color_By_Mask_16Bit(pSrc[i], pDst[i], pMask[i], fIntensity, bInvertMask);
}

void DoIntpMatteLineByMask32Bit(float *pSrcLine,
                                float *pDstLine,
                                const float *pMaskLine,
                                unsigned int nCount,
                                float fIntensity,
                                bool bInvertMask)
{
    const __m128 xmm_intensity_ps = _mm_set1_ps(fIntensity);
    const __m128 xmm_zero_ps = _mm_setzero_ps();

    float *pSrc = pSrcLine, *pDst = pDstLine;
    const float *pMask = pMaskLine;

    unsigned int nGroupCount = nCount / 4;
    unsigned int nTailCount = nCount & 3;

    for (unsigned int i = 0; i < nGroupCount; i++, pSrc += 4, pDst += 4, pMask += 4) {
        // process 4 pixels each time
        union {
            float f[4];
            __m128 ps;
        } uMask;

        uMask.ps = _mm_load_ps(pMask);        // [       m3      |       m2      |       m1      |       m0      ]
        uMask.ps = _mm_mul_ps(uMask.ps, xmm_intensity_ps);
        if (bInvertMask)
            uMask.ps = _mm_sub_ps(g_xmm_1_ps, uMask.ps);

        if (_mm_movemask_ps(_mm_cmpeq_ps(uMask.ps, xmm_zero_ps)) == 0xF) {
            memcpy(pDst, pSrc, 16);
            continue;
        } else if (_mm_movemask_ps(_mm_cmpeq_ps(uMask.ps, g_xmm_1_ps)) == 0xF) {
            continue;
        }

        // NOTE: we can't use SSE to process matte interpolation, since INF * 0 = NAN !!!
        for (int k = 0; k < 4; k++) {
            const float fMask = uMask.f[k];
            if (fMask == 0) {
                pDst[k] = pSrc[k];
            } else if (fMask == 1) {
                continue;
            } else {
                const float fSrcMatte = pSrc[k];
                pDst[k] = (pDst[k] - fSrcMatte) * fMask + fSrcMatte;
            }
        }
    }

    for (unsigned int i = 0; i < nTailCount; i++)
        pDst[i] = __Intp_Color_By_Mask_32Bit(pSrc[i], pDst[i], pMask[i], fIntensity, bInvertMask);
}

// interpolate between source matte and destination matte using mask as factor
// the final result is stored in destination line
void __IntpMatteLineByMask(const void *pSrcLine,
						   void *pDstLine,
						   ENvcBitDepth eBitDepth,
						   const SNvcMaskInfo *pMaskInfo,
						   int nMaskLineIdx,
						   int nPixelCount)
{
	void *pMaskLine = (char *)pMaskInfo->pMaskBits + pMaskInfo->nMaskPitch * nMaskLineIdx;
    if (pMaskInfo->eType != keNvcMaskTypeAlpha || pMaskInfo->eBitDepth != eBitDepth) {
        // Construct alpha only mask line with the specified depth
        void *pTmpBuffer = _alloca(nPixelCount * NvcGetBytesFromBitDepth(eBitDepth) + 127);
        pTmpBuffer = (void *)NVC_UP_ALIGN_PO2((size_t)pTmpBuffer, 128);
		const int nColorimetry = NvcGetColorimetry(pMaskInfo->eColorimetry);
		SNvcBufferPlaneInfo maskline;
		if (pMaskInfo->eType == keNvcMaskTypeYUV422Planar || pMaskInfo->eType == keNvcMaskTypeYUVA422Planar) {
			maskline.nPlanarCount = pMaskInfo->sPlaneInfo.nPlanarCount;
			for (int i = 0; i < maskline.nPlanarCount; i++) {
				maskline.pDataPtr[i] = (char *)pMaskInfo->sPlaneInfo.pDataPtr[i] + nMaskLineIdx * pMaskInfo->sPlaneInfo.nPitch[i];
				maskline.nPitch[i] = pMaskInfo->sPlaneInfo.nPitch[i];
			}
		}
		else {
			maskline.nPlanarCount = 1;
			maskline.pDataPtr[0] = pMaskLine;
			maskline.nPitch[0] = pMaskInfo->nMaskPitch;
		}
        __ConstructMaskLine(&maskline, pMaskInfo->eBitDepth, pMaskInfo->eType, pMaskInfo->eChannel, nColorimetry, pTmpBuffer, eBitDepth, nPixelCount);
        pMaskLine = pTmpBuffer;
    }

    switch (eBitDepth) {
    case keNvcBitDepthInt8:
        DoIntpMatteLineByMask8Bit((uint8_t *)pSrcLine, (uint8_t *)pDstLine, (uint8_t *)pMaskLine,
            nPixelCount, pMaskInfo->fIntensity, pMaskInfo->bInvert);
        break;

    case keNvcBitDepthInt16:
        DoIntpMatteLineByMask16Bit((uint16_t *)pSrcLine, (uint16_t *)pDstLine, (uint16_t *)pMaskLine,
            nPixelCount, pMaskInfo->fIntensity, pMaskInfo->bInvert);
        break;

    case keNvcBitDepthFloat32:
        DoIntpMatteLineByMask32Bit((float *)pSrcLine, (float *)pDstLine, (float *)pMaskLine,
            nPixelCount, pMaskInfo->fIntensity, pMaskInfo->bInvert);
        break;
    }
}


void DoIntpBGRALineByMask8Bit(uint8_t *pSrcLine,
							  uint8_t *pDstLine,
							  const uint8_t *pMaskLine,
							  unsigned int nCount,
							  float fIntensity,
							  bool bInvertMask)
{
	unsigned int i, nTailCount, nIntensity = (unsigned int)(fIntensity * 32768 + 0.5);
	const __m128i xmm_intensity = _mm_set1_epi16((unsigned short)nIntensity);

	nTailCount = nCount & 3;
	nCount /= 4;

	uint8_t *pSrc = pSrcLine, *pDst = pDstLine;
	const uint8_t *pMask = pMaskLine;

	for (i = 0; i < nCount; i++, pSrc += 16, pDst += 16, pMask += 4) {
		// process 4 pixels each time
		__m128i xmm_color_src1, xmm_color_src2;
		__m128i xmm_color_dst1, xmm_color_dst2;
		__m128i xmm_mask1, xmm_mask2;

        const unsigned int nMask = *(unsigned int *)pMask;
        if (nMask == 0) {
            if (!bInvertMask)
                memcpy(pDst, pSrc, 16);
            continue;
        }

        if (nMask == 0xFFFFFFFF && nIntensity == 32768) {
            if (bInvertMask)
                memcpy(pDst, pSrc, 16);
            continue;
        }

        xmm_mask2 = _mm_cvtsi32_si128(nMask);		            // [ 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | m3| m2| m1| m0]
        xmm_mask2 = _mm_unpacklo_epi8(xmm_mask2, g_xmm_zero);	// [   0   |   0   |   0   |   0   |   m3  |   m2  |   m1  |   m0  ]

        xmm_mask2 = _mm_unpacklo_epi16(xmm_mask2, xmm_mask2);	// [   m3  |   m3  |   m2  |   m2  |   m1  |   m1  |   m0  |   m0  ]
        xmm_mask1 = _mm_shuffle_epi32(xmm_mask2, 0x50);			// [   m1  |   m1  |   m1  |   m1  |   m0  |   m0  |   m0  |   m0  ]
        xmm_mask2 = _mm_shuffle_epi32(xmm_mask2, 0xfa);			// [   m3  |   m3  |   m3  |   m3  |   m2  |   m2  |   m2  |   m2  ]

		xmm_color_src2 = _mm_load_si128((__m128i *)pSrc);				// [sa3|sr3|sg3|sb3|sa2|sr2|sg2|sb2|sa1|sr1|sg1|sb1|sa0|sr0|sg0|sb0]
		xmm_color_src1 = _mm_unpacklo_epi8(xmm_color_src2, g_xmm_zero);	// [  sa1  |  sr1  |  sg1  |  sb1  |  sa0  |  sr0  |  sg0  |  sb0  ]
		xmm_color_src2 = _mm_unpackhi_epi8(xmm_color_src2, g_xmm_zero);	// [  sa3  |  sr3  |  sg3  |  sb3  |  sa2  |  sr2  |  sg2  |  sb2  ]
		xmm_color_dst2 = _mm_load_si128((__m128i *)pDst);				// [da3|dr3|dg3|db3|da2|dr2|dg2|db2|da1|dr1|dg1|db1|da0|dr0|dg0|db0]
		xmm_color_dst1 = _mm_unpacklo_epi8(xmm_color_dst2, g_xmm_zero);	// [  da1  |  dr1  |  dg1  |  db1  |  da0  |  dr0  |  dg0  |  db0  ]
		xmm_color_dst2 = _mm_unpackhi_epi8(xmm_color_dst2, g_xmm_zero);	// [  da3  |  dr3  |  dg3  |  db3  |  da2  |  dr2  |  dg2  |  db2  ]

		__INTERPOLATE_COLOR_BY_MASK_8P_8BIT_INT(xmm_color_src1, xmm_color_dst1, xmm_mask1, xmm_intensity, bInvertMask);
		__INTERPOLATE_COLOR_BY_MASK_8P_8BIT_INT(xmm_color_src2, xmm_color_dst2, xmm_mask2, xmm_intensity, bInvertMask);

		xmm_color_dst1 = _mm_packus_epi16(xmm_color_dst1, xmm_color_dst2);	// [a'3|r'3|g'3|b'3|a'2|r'2|g'2|b'2|a'1|r'1|g'1|b'1|a'0|r'0|g'0|b'0]
		_mm_store_si128((__m128i *)pDst, xmm_color_dst1);
	}

	for (i = 0; i < nTailCount; i++) {
		pDst[i * 4 + 0] = __Intp_Color_By_Mask_8Bit(pSrc[i * 4 + 0], pDst[i * 4 + 0], pMask[i], nIntensity, bInvertMask);
		pDst[i * 4 + 1] = __Intp_Color_By_Mask_8Bit(pSrc[i * 4 + 1], pDst[i * 4 + 1], pMask[i], nIntensity, bInvertMask);
		pDst[i * 4 + 2] = __Intp_Color_By_Mask_8Bit(pSrc[i * 4 + 2], pDst[i * 4 + 2], pMask[i], nIntensity, bInvertMask);
		pDst[i * 4 + 3] = __Intp_Color_By_Mask_8Bit(pSrc[i * 4 + 3], pDst[i * 4 + 3], pMask[i], nIntensity, bInvertMask);
	}
}

void DoIntpBGRALineByMask16Bit(uint16_t *pSrcLine,
							   uint16_t *pDstLine,
							   const uint16_t *pMaskLine,
							   unsigned int nCount,
							   float fIntensity,
							   bool bInvertMask)
{
	unsigned int i, nTailCount;
	const __m128 xmm_intensity_ps = _mm_set1_ps(fIntensity);

	nTailCount = nCount & 1;
	nCount /= 2;

	uint16_t *pSrc = pSrcLine, *pDst = pDstLine;
	const uint16_t *pMask = pMaskLine;

	for (i = 0; i < nCount; i++, pSrc += 8, pDst += 8, pMask += 2) {
		// process 2 pixels each time
		__m128i xmm_color_src, xmm_color_dst, xmm_color1, xmm_color2;
		__m128 xmm_color_src_ps, xmm_color_dst_ps, xmm_mask_ps;

        const unsigned int nMask = *(unsigned int *)pMask;
        if (nMask == 0) {
            if (!bInvertMask)
                memcpy(pDst, pSrc, 16);
            continue;
        }

        if (nMask == 0xFFFFFFFF && fIntensity == 1) {
            if (bInvertMask)
                memcpy(pDst, pSrc, 16);
            continue;
        }

		xmm_color_src = _mm_load_si128((__m128i *)pSrc);	// [  sa1  |  sr1  |  sg1  |  sb1  |  sa0  |  sr0  |  sg0  |  sb0  ]
		xmm_color_dst = _mm_load_si128((__m128i *)pDst);	// [  da1  |  dr1  |  dg1  |  db1  |  da0  |  dr0  |  dg0  |  db0  ]

		xmm_color_src_ps = _mm_cvtepi32_ps(_mm_unpacklo_epi16(xmm_color_src, g_xmm_zero));	// [      sa0      |      sr0      |      sg0      |      sb0      ]
		xmm_color_dst_ps = _mm_cvtepi32_ps(_mm_unpacklo_epi16(xmm_color_dst, g_xmm_zero));	// [      da0      |      dr0      |      dg0      |      db0      ]
		xmm_mask_ps = _mm_set1_ps((float)pMask[0]);
		__INTERPOLATE_COLOR_BY_MASK_4P_FLOAT(xmm_color_src_ps, xmm_color_dst_ps, xmm_mask_ps, xmm_intensity_ps, bInvertMask);
		xmm_color1 = _mm_cvtps_epi32(xmm_color_dst_ps);		// [      a'0      |      r'0      |      g'0      |      b'0      ]
		xmm_color1 = _mm_sub_epi32(xmm_color1, g_xmm_32bit_32768);

		xmm_color_src_ps = _mm_cvtepi32_ps(_mm_unpackhi_epi16(xmm_color_src, g_xmm_zero));	// [      sa1      |      sr1      |      sg1      |      sb1      ]
		xmm_color_dst_ps = _mm_cvtepi32_ps(_mm_unpackhi_epi16(xmm_color_dst, g_xmm_zero));	// [      da1      |      dr1      |      dg1      |      db1      ]
		xmm_mask_ps = _mm_set1_ps((float)pMask[1]);
		__INTERPOLATE_COLOR_BY_MASK_4P_FLOAT(xmm_color_src_ps, xmm_color_dst_ps, xmm_mask_ps, xmm_intensity_ps, bInvertMask);
		xmm_color2 = _mm_cvtps_epi32(xmm_color_dst_ps);		// [      a'1      |      r'1      |      g'1      |      b'1      ]
		xmm_color2 = _mm_sub_epi32(xmm_color2, g_xmm_32bit_32768);

		xmm_color1 = _mm_packs_epi32(xmm_color1, xmm_color2);		// [  a'1  |  r'1  |  g'1  |  b'1  |  a'0  |  r'0  |  g'0  |  b'0  ]
		xmm_color1 = _mm_add_epi16(xmm_color1, g_xmm_16bit_32768);	// [  a'1  |  r'1  |  g'1  |  b'1  |  a'0  |  r'0  |  g'0  |  b'0  ]
		_mm_store_si128((__m128i *)pDst, xmm_color1);
	}

	for (i = 0; i < nTailCount; i++) {
		pDst[i * 4 + 0] = __Intp_Color_By_Mask_16Bit(pSrc[i * 4 + 0], pDst[i * 4 + 0], pMask[i], fIntensity, bInvertMask);
		pDst[i * 4 + 1] = __Intp_Color_By_Mask_16Bit(pSrc[i * 4 + 1], pDst[i * 4 + 1], pMask[i], fIntensity, bInvertMask);
		pDst[i * 4 + 2] = __Intp_Color_By_Mask_16Bit(pSrc[i * 4 + 2], pDst[i * 4 + 2], pMask[i], fIntensity, bInvertMask);
		pDst[i * 4 + 3] = __Intp_Color_By_Mask_16Bit(pSrc[i * 4 + 3], pDst[i * 4 + 3], pMask[i], fIntensity, bInvertMask);
	}
}

void DoIntpBGRALineByMask32Bit(float *pSrcLine,
							   float *pDstLine,
							   const float *pMaskLine,
							   unsigned int nCount,
							   float fIntensity,
							   bool bInvertMask)
{
    float *pSrc = pSrcLine, *pDst = pDstLine;
    const float *pMask = pMaskLine;

	for (unsigned int i = 0; i < nCount; i++, pSrc += 4, pDst += 4) {
		// process 1 pixel every time
        // NOTE: we can't use SSE to process interpolation if fMask is 0, since INF * 0 = NAN !!!
        float fMask = pMask[i] * fIntensity;
        if (bInvertMask)
            fMask = 1 - fMask;

        if (fMask == 0) {
            memcpy(pDst, pSrc, sizeof(float) * 4);
            continue;
        }
        
        if (fMask == 1)
            continue;

		__m128 xmm_color_src_ps, xmm_color_dst_ps;
		xmm_color_src_ps = _mm_load_ps(pSrc);                       // [       sa      |       sr      |       sg      |       sb      ]
		xmm_color_dst_ps = _mm_load_ps(pDst);                       // [       da      |       dr      |       dg      |       db      ]

        const __m128 xmm_mask_ps = _mm_set1_ps(fMask);              // [       m       |       m       |       m       |       m       ]

        // interpolate source and destination colors
        xmm_color_dst_ps = _mm_sub_ps(xmm_color_dst_ps, xmm_color_src_ps);
        xmm_color_dst_ps = _mm_mul_ps(xmm_color_dst_ps, xmm_mask_ps);
        xmm_color_dst_ps = _mm_add_ps(xmm_color_dst_ps, xmm_color_src_ps);

		_mm_store_ps(pDst, xmm_color_dst_ps);
	}
}

// interpolate between source color and destination color using mask as factor
// all channels (B/G/R/A) are interpolated
// the final result is stored in destination line
void __IntpBGRALineByMask(const void *pSrcLine,
						  void *pDstLine,
						  ENvcBitDepth eBitDepth,
						  const SNvcMaskInfo *pMaskInfo,
						  int nMaskLineIdx,
						  int nPixelCount)
{
    void *pMaskLine = (char *)pMaskInfo->pMaskBits + pMaskInfo->nMaskPitch * nMaskLineIdx;
    if (pMaskInfo->eType != keNvcMaskTypeAlpha || pMaskInfo->eBitDepth != eBitDepth) {
        // Construct alpha only mask line with the specified depth
        void *pTmpBuffer = _alloca(nPixelCount * NvcGetBytesFromBitDepth(eBitDepth) + 127);
        pTmpBuffer = (void *)NVC_UP_ALIGN_PO2((size_t)pTmpBuffer, 128);
		const int nColorimetry = NvcGetColorimetry(pMaskInfo->eColorimetry);
		SNvcBufferPlaneInfo maskline;
		if (pMaskInfo->eType == keNvcMaskTypeYUV422Planar || pMaskInfo->eType == keNvcMaskTypeYUVA422Planar) {
			maskline.nPlanarCount = pMaskInfo->sPlaneInfo.nPlanarCount;
			for (int i = 0; i < maskline.nPlanarCount; i++) {
				maskline.pDataPtr[i] = (char *)pMaskInfo->sPlaneInfo.pDataPtr[i] + nMaskLineIdx * pMaskInfo->sPlaneInfo.nPitch[i];
				maskline.nPitch[i] = pMaskInfo->sPlaneInfo.nPitch[i];
			}
		}
		else {
			maskline.nPlanarCount = 1;
			maskline.pDataPtr[0] = pMaskLine;
			maskline.nPitch[0] = pMaskInfo->nMaskPitch;
		}
        __ConstructMaskLine(&maskline, pMaskInfo->eBitDepth, pMaskInfo->eType, pMaskInfo->eChannel, nColorimetry, pTmpBuffer, eBitDepth, nPixelCount);
        pMaskLine = pTmpBuffer;
    }

    switch (eBitDepth) {
    case keNvcBitDepthInt8:
        DoIntpBGRALineByMask8Bit((uint8_t *)pSrcLine, (uint8_t *)pDstLine, (uint8_t *)pMaskLine,
            nPixelCount, pMaskInfo->fIntensity, pMaskInfo->bInvert);
        break;

    case keNvcBitDepthInt16:
        DoIntpBGRALineByMask16Bit((uint16_t *)pSrcLine, (uint16_t *)pDstLine, (uint16_t *)pMaskLine,
            nPixelCount, pMaskInfo->fIntensity, pMaskInfo->bInvert);
        break;

    case keNvcBitDepthFloat32:
        DoIntpBGRALineByMask32Bit((float *)pSrcLine, (float *)pDstLine, (float *)pMaskLine,
            nPixelCount, pMaskInfo->fIntensity, pMaskInfo->bInvert);
        break;
    }
}

#ifdef _MSC_VER
#pragma warning(default:4127)
#endif


