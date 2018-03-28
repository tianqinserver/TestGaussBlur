//==========================================================================
//
// (c) Copyright NewAuto Video Technology CO.,Ltd., 2008. All rights reserved. 
//
// This code and information is provided "as is" without warranty of any kind, 
// either expressed or implied, including but not limited to the implied 
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------
//   Birth Date:       May 5. 2009
//   Author:           NewAuto video team
//--------------------------------------------------------------------------
#pragma once

#include <NvcVisionCoreDef.h>
#include <NvcColorConv.h>
#include <immintrin.h>

//////////////////////////////////////////////////////////////////////////
// bit depth conversion
//////////////////////////////////////////////////////////////////////////

#define __NVC_12BIT_SCALE_TO_16BIT_CONST	((uint32_t)(((65535.0 / 4095.0) * (1 << 16))))
#define __NVC_16BIT_SCALE_TO_12BIT_CONST	((uint32_t)(((4095.0 / 65535.0) * (1 << 16))))

#define __NVC_12BIT_TO_16BIT(x)    (((uint32_t)(x) * __NVC_12BIT_SCALE_TO_16BIT_CONST + (uint32_t)(1 << 15)) >> 16)
#define __NVC_16BIT_TO_12BIT(x)    (((uint32_t)(x) * __NVC_16BIT_SCALE_TO_12BIT_CONST + (uint32_t)(1 << 15)) >> 16)


//////////////////////////////////////////////////////////////////////////
// some constants
//////////////////////////////////////////////////////////////////////////
#define __COEF_RED_LUMA(colorimetry)	(uint16_t)(__NVC_R_Y(colorimetry) * 65536)
#define __COEF_GREEN_LUMA(colorimetry)	(uint16_t)(__NVC_G_Y(colorimetry) * 65536)
#define __COEF_BLUE_LUMA(colorimetry)	(uint16_t)(__NVC_B_Y(colorimetry) * 65536)

//////////////////////////////////////////////////////////////////////////
// some global variables
//////////////////////////////////////////////////////////////////////////
extern const __m128 g_xmm_0_ps;
extern const __m128 g_xmm_1_ps;
extern const __m128 g_xmm_2_ps;
extern const __m128 g_xmm_6_ps;
extern const __m128 g_xmm_16_ps;
extern const __m128 g_xmm_60_ps;
extern const __m128 g_xmm_128_ps;
extern const __m128 g_xmm_64_ps;
extern const __m128 g_xmm_255_ps;
extern const __m128 g_xmm_512_ps;
extern const __m128 g_xmm_65535_ps;

extern const __m128i g_xmm_32bit_32768;
extern const __m128i g_xmm_16bit_32768;
extern const __m128i g_xmm_16bit_128;
extern const __m128i g_xmm_16bit_64;

extern const __m128i g_xmm_luma_red_coef_601;
extern const __m128i g_xmm_luma_green_coef_601;
extern const __m128i g_xmm_luma_blue_coef_601;
extern const __m128i g_xmm_luma_red_coef_709;
extern const __m128i g_xmm_luma_green_coef_709;
extern const __m128i g_xmm_luma_blue_coef_709;
extern const __m128i g_xmm_luma_red_coef_2020;
extern const __m128i g_xmm_luma_green_coef_2020;
extern const __m128i g_xmm_luma_blue_coef_2020;

extern const __m128 g_xmm_luma_red_coef_601_ps;
extern const __m128 g_xmm_luma_green_coef_601_ps;
extern const __m128 g_xmm_luma_blue_coef_601_ps;
extern const __m128 g_xmm_luma_red_coef_709_ps;
extern const __m128 g_xmm_luma_green_coef_709_ps;
extern const __m128 g_xmm_luma_blue_coef_709_ps;
extern const __m128 g_xmm_luma_red_coef_2020_ps;
extern const __m128 g_xmm_luma_green_coef_2020_ps;
extern const __m128 g_xmm_luma_blue_coef_2020_ps;

extern const __m128 g_xmm_scale_4095_65535;
extern const __m128 g_xmm_scale_65535_4095;

extern const __m128i g_xmm_YUY2_LowByteMatte;
extern const __m128i g_xmm_UYVY422_LowByteMatte;

extern const __m128i g_xmm_unpack_8b_mask;

extern const __m128i g_xmm_unpack_16b_mask;

extern const __m128i g_xmm_unpack_float_8b_mask;

extern const __m128i g_xmm_16bit_rounding_div_256;

extern const __m128i g_xmm_yuva_alpha_mask_32bit;

extern const __m128i g_xmm_mask_8_8bit_0;
extern const __m128i g_xmm_mask_8_8bit_1;
extern const __m128i g_xmm_mask_8_8bit_2;
extern const __m128i g_xmm_mask_8_8bit_3;
extern const __m128i g_xmm_mask_8_8bit_4;
extern const __m128i g_xmm_mask_8_8bit_5;
extern const __m128i g_xmm_mask_8_8bit_6;
extern const __m128i g_xmm_mask_8_8bit_7;
extern const __m128i g_xmm_mask_8_8bit_8;
extern const __m128i g_xmm_mask_8_8bit_9;
extern const __m128i g_xmm_mask_8_8bit_10;
extern const __m128i g_xmm_mask_8_8bit_11;
extern const __m128i g_xmm_mask_8_8bit_12;
extern const __m128i g_xmm_mask_8_8bit_13;
extern const __m128i g_xmm_mask_8_8bit_14;
extern const __m128i g_xmm_mask_8_8bit_15;

extern const __m128i g_xmm_mask_16_16bit_0;
extern const __m128i g_xmm_mask_16_16bit_1;
extern const __m128i g_xmm_mask_16_16bit_2;
extern const __m128i g_xmm_mask_16_16bit_3;
extern const __m128i g_xmm_mask_16_16bit_4;
extern const __m128i g_xmm_mask_16_16bit_5;
extern const __m128i g_xmm_mask_16_16bit_6;
extern const __m128i g_xmm_mask_16_16bit_7;

extern const __m128i g_xmm_mask_32_32bit_0;
extern const __m128i g_xmm_mask_32_32bit_1;
extern const __m128i g_xmm_mask_32_32bit_2;
extern const __m128i g_xmm_mask_32_32bit_3;

// for v210 color space conversion
extern const __m128i g_xmm_mask_10_16bit_0;
extern const __m128i g_xmm_mask_10_16bit_1;
extern const __m128i g_xmm_mask_10_16bit_2;
extern const __m128i g_xmm_mask_10_16bit_3;
extern const __m128i g_xmm_mask_10_16bit_4;
extern const __m128i g_xmm_mask_10_16bit_5;
extern const __m128i g_xmm_mask_10_16bit_6;
extern const __m128i g_xmm_mask_10_16bit_7;

//for uyavya4224 color space conversion
extern const __m128i g_xmm_mask_10_hi_16bit;
//////////////////////////////////////////////////////////////////////////
//
// The algorithm we multiply a normalized byte:
// A * B / 255 = A * B * (256 / 255) / 256 = A * B * (1 + 1 / 255) / 256
//             = (A * B + A * B / 255) / 256
//             approximately equal to (A * B + A * B / 256) / 256
//
//////////////////////////////////////////////////////////////////////////
static inline __m128i __Multiply_8_Elements_8Bit_Int(const __m128i& xmm_element, const __m128i& xmm_mul)
{
    __m128i xmm_result = _mm_mullo_epi16(xmm_element, xmm_mul);
    xmm_result = _mm_add_epi16(xmm_result, _mm_srli_epi16(xmm_result, 8));
    xmm_result = _mm_add_epi16(xmm_result, g_xmm_16bit_rounding_div_256);
    return _mm_srli_epi16(xmm_result, 8);
}


#define	__NVC_MATTE_UNPACK_FLOAT_16P_8BIT(matte, xmm_a0_ps, xmm_a1_ps, xmm_a2_ps, xmm_a3_ps)								\
{																															\
	__m128i __xmm0, __xmm1, __xmm2, __xmm_zero = _mm_setzero_si128();														\
																															\
	__xmm0 = _mm_load_si128((__m128i *)(matte));	/* [a15|a14|a13|a12|a11|a10| a9| a8| a7| a6| a5| a4| a3| a2| a1| a0] */	\
	__xmm1 = _mm_unpacklo_epi8(__xmm0, __xmm_zero);	/* [   a7  |   a6  |   a5  |   a4  |   a3  |   a2  |   a1  |   a0  ] */	\
	__xmm2 = _mm_unpacklo_epi16(__xmm1, __xmm_zero);/* [       a3      |       a2      |       a1      |       a0      ] */	\
	xmm_a0_ps = _mm_cvtepi32_ps(__xmm2);																					\
																															\
	__xmm1 = _mm_unpackhi_epi16(__xmm1, __xmm_zero);/* [       a7      |       a6      |       a5      |       a4      ] */	\
	xmm_a1_ps = _mm_cvtepi32_ps(__xmm1);																					\
																															\
	__xmm1 = _mm_unpackhi_epi8(__xmm0, __xmm_zero);	/* [  a15  |  a14  |  a13  |  a12  |  a11  |  a10  |   a9  |   a8  ] */	\
	__xmm2 = _mm_unpacklo_epi16(__xmm1, __xmm_zero);/* [      a11      |      a10      |       a9      |       a8      ] */	\
	xmm_a2_ps = _mm_cvtepi32_ps(__xmm2);																					\
																															\
	__xmm1 = _mm_unpackhi_epi16(__xmm1, __xmm_zero);/* [      a15      |      a14      |      a13      |      a12      ] */	\
	xmm_a3_ps = _mm_cvtepi32_ps(__xmm1);																					\
}

#define	__NVC_MATTE_UNPACK_FLOAT_16P_8BIT_UNALIGNED(matte, xmm_a0_ps, xmm_a1_ps, xmm_a2_ps, xmm_a3_ps)						\
{																															\
	__m128i __xmm0, __xmm1, __xmm2, __xmm_zero = _mm_setzero_si128();														\
	\
	__xmm0 = _mm_loadu_si128((__m128i *)(matte));	/* [a15|a14|a13|a12|a11|a10| a9| a8| a7| a6| a5| a4| a3| a2| a1| a0] */	\
	__xmm1 = _mm_unpacklo_epi8(__xmm0, __xmm_zero);	/* [   a7  |   a6  |   a5  |   a4  |   a3  |   a2  |   a1  |   a0  ] */	\
	__xmm2 = _mm_unpacklo_epi16(__xmm1, __xmm_zero);/* [       a3      |       a2      |       a1      |       a0      ] */	\
	xmm_a0_ps = _mm_cvtepi32_ps(__xmm2);																					\
	\
	__xmm1 = _mm_unpackhi_epi16(__xmm1, __xmm_zero);/* [       a7      |       a6      |       a5      |       a4      ] */	\
	xmm_a1_ps = _mm_cvtepi32_ps(__xmm1);																					\
	\
	__xmm1 = _mm_unpackhi_epi8(__xmm0, __xmm_zero);	/* [  a15  |  a14  |  a13  |  a12  |  a11  |  a10  |   a9  |   a8  ] */	\
	__xmm2 = _mm_unpacklo_epi16(__xmm1, __xmm_zero);/* [      a11      |      a10      |       a9      |       a8      ] */	\
	xmm_a2_ps = _mm_cvtepi32_ps(__xmm2);																					\
	\
	__xmm1 = _mm_unpackhi_epi16(__xmm1, __xmm_zero);/* [      a15      |      a14      |      a13      |      a12      ] */	\
	xmm_a3_ps = _mm_cvtepi32_ps(__xmm1);																					\
}

#define	__NVC_MATTE_PACK_FLOAT_16P_8BIT(xmm_a0_ps, xmm_a1_ps, xmm_a2_ps, xmm_a3_ps, matte)								\
{																														\
	__m128i __xmm0, __xmm1, __xmm2;																						\
																														\
	__xmm0 = _mm_cvtps_epi32(xmm_a0_ps);		/* [       a3      |       a2      |       a1      |       a0      ] */	\
	__xmm1 = _mm_cvtps_epi32(xmm_a1_ps);		/* [       a7      |       a6      |       a5      |       a4      ] */	\
	__xmm0 = _mm_packs_epi32(__xmm0, __xmm1);	/* [   a7  |   a6  |   a5  |   a4  |   a3  |   a2  |   a1  |   a0  ] */	\
	__xmm1 = _mm_cvtps_epi32(xmm_a2_ps);		/* [      a11      |      a10      |       a9      |       a8      ] */	\
	__xmm2 = _mm_cvtps_epi32(xmm_a3_ps);		/* [      a15      |      a14      |      a13      |      a12      ] */	\
	__xmm1 = _mm_packs_epi32(__xmm1, __xmm2);	/* [  a15  |  a14  |  a13  |  a12  |  a11  |  a10  |   a9  |   a8  ] */	\
	__xmm0 = _mm_packus_epi16(__xmm0, __xmm1);	/* [a15|a14|a13|a12|a11|a10| a9| a8| a7| a6| a5| a4| a3| a2| a1| a0] */	\
	_mm_store_si128((__m128i *)(matte), __xmm0);																		\
}

#define	__NVC_MATTE_PACK_FLOAT_16P_8BIT_UNALIGNED(xmm_a0_ps, xmm_a1_ps, xmm_a2_ps, xmm_a3_ps, matte)					\
{																														\
	__m128i __xmm0, __xmm1, __xmm2;																						\
	\
	__xmm0 = _mm_cvtps_epi32(xmm_a0_ps);		/* [       a3      |       a2      |       a1      |       a0      ] */	\
	__xmm1 = _mm_cvtps_epi32(xmm_a1_ps);		/* [       a7      |       a6      |       a5      |       a4      ] */	\
	__xmm0 = _mm_packs_epi32(__xmm0, __xmm1);	/* [   a7  |   a6  |   a5  |   a4  |   a3  |   a2  |   a1  |   a0  ] */	\
	__xmm1 = _mm_cvtps_epi32(xmm_a2_ps);		/* [      a11      |      a10      |       a9      |       a8      ] */	\
	__xmm2 = _mm_cvtps_epi32(xmm_a3_ps);		/* [      a15      |      a14      |      a13      |      a12      ] */	\
	__xmm1 = _mm_packs_epi32(__xmm1, __xmm2);	/* [  a15  |  a14  |  a13  |  a12  |  a11  |  a10  |   a9  |   a8  ] */	\
	__xmm0 = _mm_packus_epi16(__xmm0, __xmm1);	/* [a15|a14|a13|a12|a11|a10| a9| a8| a7| a6| a5| a4| a3| a2| a1| a0] */	\
	_mm_storeu_si128((__m128i *)(matte), __xmm0);																		\
}

#define	__NVC_MATTE_UNPACK_FLOAT_16P_16BIT(matte, xmm_a0_ps, xmm_a1_ps, xmm_a2_ps, xmm_a3_ps)	\
{																								\
	__m128i __xmm0, __xmm_zero = _mm_setzero_si128();											\
																								\
	__xmm0 = _mm_load_si128((__m128i *)(matte));		/* [   a7  |   a6  |   a5  |   a4  |   a3  |   a2  |   a1  |   a0  ] */	\
	xmm_a0_ps = _mm_cvtepi32_ps(_mm_unpacklo_epi16(__xmm0, __xmm_zero));						\
	xmm_a1_ps = _mm_cvtepi32_ps(_mm_unpackhi_epi16(__xmm0, __xmm_zero));						\
																								\
	__xmm0 = _mm_load_si128((__m128i *)((char *)(matte) + 16));/* [   a15 |   a14 |   a13 |   a12 |   a11 |   a10 |   a9  |   a8  ] */	\
	xmm_a2_ps = _mm_cvtepi32_ps(_mm_unpacklo_epi16(__xmm0, __xmm_zero));						\
	xmm_a3_ps = _mm_cvtepi32_ps(_mm_unpackhi_epi16(__xmm0, __xmm_zero));						\
}

#define	__NVC_MATTE_UNPACK_FLOAT_16P_16BIT_UNALIGNED(matte, xmm_a0_ps, xmm_a1_ps, xmm_a2_ps, xmm_a3_ps)	\
{																										\
	__m128i __xmm0, __xmm_zero = _mm_setzero_si128();													\
	\
	__xmm0 = _mm_loadu_si128((__m128i *)(matte));		/* [   a7  |   a6  |   a5  |   a4  |   a3  |   a2  |   a1  |   a0  ] */	\
	xmm_a0_ps = _mm_cvtepi32_ps(_mm_unpacklo_epi16(__xmm0, __xmm_zero));						\
	xmm_a1_ps = _mm_cvtepi32_ps(_mm_unpackhi_epi16(__xmm0, __xmm_zero));						\
	\
	__xmm0 = _mm_loadu_si128((__m128i *)((char *)(matte) + 16));/* [   a15 |   a14 |   a13 |   a12 |   a11 |   a10 |   a9  |   a8  ] */	\
	xmm_a2_ps = _mm_cvtepi32_ps(_mm_unpacklo_epi16(__xmm0, __xmm_zero));						\
	xmm_a3_ps = _mm_cvtepi32_ps(_mm_unpackhi_epi16(__xmm0, __xmm_zero));						\
}

#define	__NVC_MATTE_PACK_FLOAT_16P_16BIT(xmm_a0_ps, xmm_a1_ps, xmm_a2_ps, xmm_a3_ps, matte)								\
{																														\
	__m128i __xmm0, __xmm1;																								\
																														\
	__xmm0 = _mm_cvtps_epi32(xmm_a0_ps);		/* [       a3      |       a2      |       a1      |       a0      ] */	\
	__xmm0 = _mm_sub_epi32(__xmm0, g_xmm_32bit_32768);																	\
	__xmm1 = _mm_cvtps_epi32(xmm_a1_ps);		/* [       a7      |       a6      |       a5      |       a4      ] */	\
	__xmm1 = _mm_sub_epi32(__xmm1, g_xmm_32bit_32768);																	\
	__xmm0 = _mm_packs_epi32(__xmm0, __xmm1);	/* [   a7  |   a6  |   a5  |   a4  |   a3  |   a2  |   a1  |   a0  ] */	\
	__xmm0 = _mm_add_epi16(__xmm0, g_xmm_16bit_32768);																	\
	_mm_store_si128((__m128i *)(matte), __xmm0);																		\
	__xmm0 = _mm_cvtps_epi32(xmm_a2_ps);		/* [      a11      |      a10      |       a9      |       a8      ] */	\
	__xmm0 = _mm_sub_epi32(__xmm0, g_xmm_32bit_32768);																	\
	__xmm1 = _mm_cvtps_epi32(xmm_a3_ps);		/* [      a15      |      a14      |      a13      |      a12      ] */	\
	__xmm1 = _mm_sub_epi32(__xmm1, g_xmm_32bit_32768);																	\
	__xmm0 = _mm_packs_epi32(__xmm0, __xmm1);	/* [  a15  |  a14  |  a13  |  a12  |  a11  |  a10  |   a9  |   a8  ] */	\
	__xmm0 = _mm_add_epi16(__xmm0, g_xmm_16bit_32768);																	\
	_mm_store_si128((__m128i *)((char *)(matte) + 16), __xmm0);															\
}

#define	__NVC_MATTE_PACK_FLOAT_16P_16BIT_UNALIGNED(xmm_a0_ps, xmm_a1_ps, xmm_a2_ps, xmm_a3_ps, matte)					\
{																														\
	__m128i __xmm0, __xmm1;																								\
	\
	__xmm0 = _mm_cvtps_epi32(xmm_a0_ps);		/* [       a3      |       a2      |       a1      |       a0      ] */	\
	__xmm0 = _mm_sub_epi32(__xmm0, g_xmm_32bit_32768);																	\
	__xmm1 = _mm_cvtps_epi32(xmm_a1_ps);		/* [       a7      |       a6      |       a5      |       a4      ] */	\
	__xmm1 = _mm_sub_epi32(__xmm1, g_xmm_32bit_32768);																	\
	__xmm0 = _mm_packs_epi32(__xmm0, __xmm1);	/* [   a7  |   a6  |   a5  |   a4  |   a3  |   a2  |   a1  |   a0  ] */	\
	__xmm0 = _mm_add_epi16(__xmm0, g_xmm_16bit_32768);																	\
	_mm_storeu_si128((__m128i *)(matte), __xmm0);																		\
	__xmm0 = _mm_cvtps_epi32(xmm_a2_ps);		/* [      a11      |      a10      |       a9      |       a8      ] */	\
	__xmm0 = _mm_sub_epi32(__xmm0, g_xmm_32bit_32768);																	\
	__xmm1 = _mm_cvtps_epi32(xmm_a3_ps);		/* [      a15      |      a14      |      a13      |      a12      ] */	\
	__xmm1 = _mm_sub_epi32(__xmm1, g_xmm_32bit_32768);																	\
	__xmm0 = _mm_packs_epi32(__xmm0, __xmm1);	/* [  a15  |  a14  |  a13  |  a12  |  a11  |  a10  |   a9  |   a8  ] */	\
	__xmm0 = _mm_add_epi16(__xmm0, g_xmm_16bit_32768);																	\
	_mm_storeu_si128((__m128i *)((char *)(matte) + 16), __xmm0);														\
}

#define	__NVC_BGRA_UNPACK_FLOAT_4P_8BIT(bgra, xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps)	\
{														\
	__m128i __xmm0, __xmm1;								\
														\
	__xmm0 = _mm_load_si128((__m128i *)(bgra));	/* [ a3| r3| g3| b3| a2| r2| g2| b2| a1| r1| g1| b1| a0| r0| g0| b0] */		\
	__xmm1 = _mm_and_si128(__xmm0, g_xmm_unpack_float_8b_mask);	/* [ 0 | 0 | 0 | b3| 0 | 0 | 0 | b2| 0 | 0 | 0 | b1| 0 | 0 | 0 | b0] */		\
	xmm_b_ps = _mm_cvtepi32_ps(__xmm1);																						\
																															\
	__xmm0 = _mm_srli_epi32(__xmm0, 8);			/* [ 0 | a3| r3| g3| 0 | a2| r2| g2| 0 | a1| r1| g1| 0 | a0| r0| g0] */		\
	__xmm1 = _mm_and_si128(__xmm0, g_xmm_unpack_float_8b_mask);	/* [ 0 | 0 | 0 | g3| 0 | 0 | 0 | g2| 0 | 0 | 0 | g1| 0 | 0 | 0 | g0] */		\
	xmm_g_ps = _mm_cvtepi32_ps(__xmm1);																						\
																															\
	__xmm0 = _mm_srli_epi32(__xmm0, 8);			/* [ 0 | 0 | a3| r3| 0 | 0 | a2| r2| 0 | 0 | a1| r1| 0 | 0 | a0| r0] */		\
	__xmm1 = _mm_and_si128(__xmm0, g_xmm_unpack_float_8b_mask);	/* [ 0 | 0 | 0 | r3| 0 | 0 | 0 | r2| 0 | 0 | 0 | r1| 0 | 0 | 0 | r0] */		\
	xmm_r_ps = _mm_cvtepi32_ps(__xmm1);																						\
																															\
	__xmm0 = _mm_srli_epi32(__xmm0, 8);			/* [ 0 | 0 | 0 | a3| 0 | 0 | 0 | a2| 0 | 0 | 0 | a1| 0 | 0 | 0 | a0] */		\
	xmm_a_ps = _mm_cvtepi32_ps(__xmm0);																						\
}

#define	__NVC_BGRA_UNPACK_FLOAT_4P_8BIT_UNALIGNED(bgra, xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps)	\
{														\
	__m128i __xmm0, __xmm1;								\
														\
	__xmm0 = _mm_loadu_si128((__m128i *)(bgra));	/* [ a3| r3| g3| b3| a2| r2| g2| b2| a1| r1| g1| b1| a0| r0| g0| b0] */	\
	__xmm1 = _mm_and_si128(__xmm0, g_xmm_unpack_float_8b_mask);	/* [ 0 | 0 | 0 | b3| 0 | 0 | 0 | b2| 0 | 0 | 0 | b1| 0 | 0 | 0 | b0] */		\
	xmm_b_ps = _mm_cvtepi32_ps(__xmm1);																						\
																															\
	__xmm0 = _mm_srli_epi32(__xmm0, 8);			/* [ 0 | a3| r3| g3| 0 | a2| r2| g2| 0 | a1| r1| g1| 0 | a0| r0| g0] */		\
	__xmm1 = _mm_and_si128(__xmm0, g_xmm_unpack_float_8b_mask);	/* [ 0 | 0 | 0 | g3| 0 | 0 | 0 | g2| 0 | 0 | 0 | g1| 0 | 0 | 0 | g0] */		\
	xmm_g_ps = _mm_cvtepi32_ps(__xmm1);																						\
																															\
	__xmm0 = _mm_srli_epi32(__xmm0, 8);			/* [ 0 | 0 | a3| r3| 0 | 0 | a2| r2| 0 | 0 | a1| r1| 0 | 0 | a0| r0] */		\
	__xmm1 = _mm_and_si128(__xmm0, g_xmm_unpack_float_8b_mask);	/* [ 0 | 0 | 0 | r3| 0 | 0 | 0 | r2| 0 | 0 | 0 | r1| 0 | 0 | 0 | r0] */		\
	xmm_r_ps = _mm_cvtepi32_ps(__xmm1);																						\
																															\
	__xmm0 = _mm_srli_epi32(__xmm0, 8);			/* [ 0 | 0 | 0 | a3| 0 | 0 | 0 | a2| 0 | 0 | 0 | a1| 0 | 0 | 0 | a0] */		\
	xmm_a_ps = _mm_cvtepi32_ps(__xmm0);																						\
}

static NVC_FORCE_INLINE void __NVC_BGR_UNPACK_FLOAT_4P_16BIT(const uint16_t* bgr,
															__m128& xmm_b,
															__m128& xmm_g,
															__m128& xmm_r)
{
	uint16_t r[4] = { 0 };
	uint16_t g[4] = { 0 };
	uint16_t b[4] = { 0 };

	for (size_t i = 0; i < 4; i++)
	{
		b[i] = bgr[i * 3];
		g[i] = bgr[i * 3 + 1];
		r[i] = bgr[i * 3 + 2];
	}

	xmm_r = _mm_setr_ps(r[0], r[1], r[2], r[3]);
	xmm_g = _mm_setr_ps(g[0], g[1], g[2], g[3]);
	xmm_b = _mm_setr_ps(b[0], b[1], b[2], b[3]);
}

static NVC_FORCE_INLINE void __NVC_RGB_UNPACK_FLOAT_4P_16BIT(const uint16_t* rgb,
															__m128& xmm_r,
															__m128& xmm_g,
															__m128& xmm_b)
{
	uint16_t r[4] = { 0 };
	uint16_t g[4] = { 0 };
	uint16_t b[4] = { 0 };

	for (size_t i = 0; i < 4; i++)
	{
		r[i] = rgb[i * 3];
		g[i] = rgb[i * 3 + 1];
		b[i] = rgb[i * 3 + 2];
	}

	xmm_r = _mm_setr_ps(r[0], r[1], r[2], r[3]);
	xmm_g = _mm_setr_ps(g[0], g[1], g[2], g[3]);
	xmm_b = _mm_setr_ps(b[0], b[1], b[2], b[3]);
}

#define	__NVC_BGRA_UNPACK_FLOAT_4P_16BIT(bgra, xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps)	\
{														\
	__m128i __xmm0, __xmm1, __xmm2, __xmm_zero;			\
														\
	__xmm0 = _mm_load_si128((__m128i *)(bgra));					/* [   a1  |   r1  |   g1  |   b1  |   a0  |   r0  |   g0  |   b0  ] */	\
	__xmm1 = _mm_load_si128((__m128i *)((char *)(bgra) + 16));	/* [   a3  |   r3  |   g3  |   b3  |   a2  |   r2  |   g2  |   b2  ] */	\
																																		\
	__xmm2 = _mm_unpacklo_epi16(__xmm0, __xmm1);		/* [   a2  |   a0  |   r2  |   r0  |   g2  |   g0  |   b2  |   b0  ] */			\
	__xmm0 = _mm_unpackhi_epi16(__xmm0, __xmm1);		/* [   a3  |   a1  |   r3  |   r1  |   g3  |   g1  |   b3  |   b1  ] */			\
	__xmm1 = _mm_unpacklo_epi16(__xmm2, __xmm0);		/* [   g3  |   g2  |   g1  |   g0  |   b3  |   b2  |   b1  |   b0  ] */			\
	__xmm2 = _mm_unpackhi_epi16(__xmm2, __xmm0);		/* [   a3  |   a2  |   a1  |   a0  |   r3  |   r2  |   r1  |   r0  ] */			\
														\
	__xmm_zero = _mm_setzero_si128();					\
														\
	__xmm0 = _mm_unpacklo_epi16(__xmm1, __xmm_zero);	\
	xmm_b_ps = _mm_cvtepi32_ps(__xmm0);					\
	__xmm1 = _mm_unpackhi_epi16(__xmm1, __xmm_zero);	\
	xmm_g_ps = _mm_cvtepi32_ps(__xmm1);					\
	__xmm0 = _mm_unpacklo_epi16(__xmm2, __xmm_zero);	\
	xmm_r_ps = _mm_cvtepi32_ps(__xmm0);					\
	__xmm2 = _mm_unpackhi_epi16(__xmm2, __xmm_zero);	\
	xmm_a_ps = _mm_cvtepi32_ps(__xmm2);					\
}

#define	__NVC_YUVA_UNPACK_FLOAT_4P_16BIT(yuva, xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps)	\
{														\
	__m128i __xmm0, __xmm1, __xmm2, __xmm_zero;			\
														\
	__xmm0 = _mm_load_si128((__m128i *)(bgra));					/* [   a1  |   r1  |   g1  |   b1  |   a0  |   r0  |   g0  |   b0  ] */	\
	__xmm1 = _mm_load_si128((__m128i *)((char *)(bgra) + 16));	/* [   a3  |   r3  |   g3  |   b3  |   a2  |   r2  |   g2  |   b2  ] */	\
																																		\
	__xmm2 = _mm_unpacklo_epi16(__xmm0, __xmm1);		/* [   a2  |   a0  |   r2  |   r0  |   g2  |   g0  |   b2  |   b0  ] */			\
	__xmm0 = _mm_unpackhi_epi16(__xmm0, __xmm1);		/* [   a3  |   a1  |   r3  |   r1  |   g3  |   g1  |   b3  |   b1  ] */			\
	__xmm1 = _mm_unpacklo_epi16(__xmm2, __xmm0);		/* [   g3  |   g2  |   g1  |   g0  |   b3  |   b2  |   b1  |   b0  ] */			\
	__xmm2 = _mm_unpackhi_epi16(__xmm2, __xmm0);		/* [   a3  |   a2  |   a1  |   a0  |   r3  |   r2  |   r1  |   r0  ] */			\
														\
	__xmm_zero = _mm_setzero_si128();					\
														\
	__xmm0 = _mm_unpacklo_epi16(__xmm1, __xmm_zero);	\
	xmm_b_ps = _mm_cvtepi32_ps(__xmm0);					\
	__xmm1 = _mm_unpackhi_epi16(__xmm1, __xmm_zero);	\
	xmm_g_ps = _mm_cvtepi32_ps(__xmm1);					\
	__xmm0 = _mm_unpacklo_epi16(__xmm2, __xmm_zero);	\
	xmm_r_ps = _mm_cvtepi32_ps(__xmm0);					\
	__xmm2 = _mm_unpackhi_epi16(__xmm2, __xmm_zero);	\
	xmm_a_ps = _mm_cvtepi32_ps(__xmm2);					\
}

#define	__NVC_BGRA_UNPACK_FLOAT_4P_16BIT_UNALIGNED(bgra, xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps)	\
{																									\
	__m128i __xmm0, __xmm1, __xmm2, __xmm_zero;														\
	\
	__xmm0 = _mm_loadu_si128((__m128i *)(bgra));				/* [   a1  |   r1  |   g1  |   b1  |   a0  |   r0  |   g0  |   b0  ] */	\
	__xmm1 = _mm_loadu_si128((__m128i *)((char *)(bgra) + 16));	/* [   a3  |   r3  |   g3  |   b3  |   a2  |   r2  |   g2  |   b2  ] */	\
	\
	__xmm2 = _mm_unpacklo_epi16(__xmm0, __xmm1);		/* [   a2  |   a0  |   r2  |   r0  |   g2  |   g0  |   b2  |   b0  ] */			\
	__xmm0 = _mm_unpackhi_epi16(__xmm0, __xmm1);		/* [   a3  |   a1  |   r3  |   r1  |   g3  |   g1  |   b3  |   b1  ] */			\
	__xmm1 = _mm_unpacklo_epi16(__xmm2, __xmm0);		/* [   g3  |   g2  |   g1  |   g0  |   b3  |   b2  |   b1  |   b0  ] */			\
	__xmm2 = _mm_unpackhi_epi16(__xmm2, __xmm0);		/* [   a3  |   a2  |   a1  |   a0  |   r3  |   r2  |   r1  |   r0  ] */			\
	\
	__xmm_zero = _mm_setzero_si128();					\
	\
	__xmm0 = _mm_unpacklo_epi16(__xmm1, __xmm_zero);	\
	xmm_b_ps = _mm_cvtepi32_ps(__xmm0);					\
	__xmm1 = _mm_unpackhi_epi16(__xmm1, __xmm_zero);	\
	xmm_g_ps = _mm_cvtepi32_ps(__xmm1);					\
	__xmm0 = _mm_unpacklo_epi16(__xmm2, __xmm_zero);	\
	xmm_r_ps = _mm_cvtepi32_ps(__xmm0);					\
	__xmm2 = _mm_unpackhi_epi16(__xmm2, __xmm_zero);	\
	xmm_a_ps = _mm_cvtepi32_ps(__xmm2);					\
}

#define	__NVC_BGRA_PACK_FLOAT_4P_8BIT(xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps, bgra)	\
{														                            \
    __m128i __xmm0, __xmm1, __xmm2, __xmm3;                                         \
                                                                                    \
    __xmm0 = _mm_cvtps_epi32(xmm_b_ps);			/* [       b3      |       b2      |       b1      |       b0      ] */	\
    __xmm1 = _mm_cvtps_epi32(xmm_g_ps);			/* [       g3      |       g2      |       g1      |       g0      ] */	\
    __xmm2 = _mm_cvtps_epi32(xmm_r_ps);			/* [       r3      |       r2      |       r1      |       r0      ] */	\
    __xmm3 = _mm_cvtps_epi32(xmm_a_ps);			/* [       a3      |       a2      |       a1      |       a0      ] */	\
    __xmm0 = _mm_packs_epi32(__xmm0, __xmm2);   /* [   r3  |   r2  |   r1  |   r0  |   b3  |   b2  |   b1  |   b0  ] */	\
    __xmm1 = _mm_packs_epi32(__xmm1, __xmm3);   /* [   a3  |   a2  |   a1  |   a0  |   g3  |   g2  |   g1  |   g0  ] */	\
    __xmm2 = _mm_unpacklo_epi16(__xmm0, __xmm1);/* [   g3  |   b3  |   g2  |   b2  |   g1  |   b1  |   g0  |   b0  ] */	\
    __xmm3 = _mm_unpackhi_epi16(__xmm0, __xmm1);/* [   a3  |   r3  |   a2  |   r2  |   a1  |   r1  |   a0  |   r0  ] */	\
    __xmm2 = _mm_packus_epi16(__xmm2, __xmm2);  /* [ g3| b3| g2| b2| g1| b1| g0| b0| g3| b3| g2| b2| g1| b1| g0| b0] */	\
    __xmm3 = _mm_packus_epi16(__xmm3, __xmm3);  /* [ a3| r3| a2| r2| a1| r1| a0| r0| a3| r3| a2| r2| a1| r1| a0| r0] */	\
    __xmm2 = _mm_unpacklo_epi16(__xmm2, __xmm3);/* [ a3| r3| g3| b3| a2| r2| g2| b2| a1| r1| g1| b1| a0| r0| g0| b0] */ \
    _mm_store_si128((__m128i *)(bgra), __xmm2);                                                                         \
}

#define	__NVC_BGRA_PACK_FLOAT_4P_8BIT_UNALIGNED(xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps, bgra)   \
{                                                                                               \
    __m128i __xmm0, __xmm1, __xmm2, __xmm3;                                                     \
                                                                                                \
    __xmm0 = _mm_cvtps_epi32(xmm_b_ps);			/* [       b3      |       b2      |       b1      |       b0      ] */	\
    __xmm1 = _mm_cvtps_epi32(xmm_g_ps);			/* [       g3      |       g2      |       g1      |       g0      ] */	\
    __xmm2 = _mm_cvtps_epi32(xmm_r_ps);			/* [       r3      |       r2      |       r1      |       r0      ] */	\
    __xmm3 = _mm_cvtps_epi32(xmm_a_ps);			/* [       a3      |       a2      |       a1      |       a0      ] */	\
    __xmm0 = _mm_packs_epi32(__xmm0, __xmm2);   /* [   r3  |   r2  |   r1  |   r0  |   b3  |   b2  |   b1  |   b0  ] */	\
    __xmm1 = _mm_packs_epi32(__xmm1, __xmm3);   /* [   a3  |   a2  |   a1  |   a0  |   g3  |   g2  |   g1  |   g0  ] */	\
    __xmm2 = _mm_unpacklo_epi16(__xmm0, __xmm1);/* [   g3  |   b3  |   g2  |   b2  |   g1  |   b1  |   g0  |   b0  ] */	\
    __xmm3 = _mm_unpackhi_epi16(__xmm0, __xmm1);/* [   a3  |   r3  |   a2  |   r2  |   a1  |   r1  |   a0  |   r0  ] */	\
    __xmm2 = _mm_packus_epi16(__xmm2, __xmm2);  /* [ g3| b3| g2| b2| g1| b1| g0| b0| g3| b3| g2| b2| g1| b1| g0| b0] */	\
    __xmm3 = _mm_packus_epi16(__xmm3, __xmm3);  /* [ a3| r3| a2| r2| a1| r1| a0| r0| a3| r3| a2| r2| a1| r1| a0| r0] */	\
    __xmm2 = _mm_unpacklo_epi16(__xmm2, __xmm3);/* [ a3| r3| g3| b3| a2| r2| g2| b2| a1| r1| g1| b1| a0| r0| g0| b0] */ \
    _mm_storeu_si128((__m128i *)(bgra), __xmm2);                                                                        \
}

#define	__NVC_BGRA_PACK_FLOAT_4P_8BIT_NT(xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps, bgra)  \
{                                                                                       \
    __m128i __xmm0, __xmm1, __xmm2, __xmm3;                                             \
                                                                                        \
    __xmm0 = _mm_cvtps_epi32(xmm_b_ps);			/* [       b3      |       b2      |       b1      |       b0      ] */	\
    __xmm1 = _mm_cvtps_epi32(xmm_g_ps);			/* [       g3      |       g2      |       g1      |       g0      ] */	\
    __xmm2 = _mm_cvtps_epi32(xmm_r_ps);			/* [       r3      |       r2      |       r1      |       r0      ] */	\
    __xmm3 = _mm_cvtps_epi32(xmm_a_ps);			/* [       a3      |       a2      |       a1      |       a0      ] */	\
    __xmm0 = _mm_packs_epi32(__xmm0, __xmm2);   /* [   r3  |   r2  |   r1  |   r0  |   b3  |   b2  |   b1  |   b0  ] */	\
    __xmm1 = _mm_packs_epi32(__xmm1, __xmm3);   /* [   a3  |   a2  |   a1  |   a0  |   g3  |   g2  |   g1  |   g0  ] */	\
    __xmm2 = _mm_unpacklo_epi16(__xmm0, __xmm1);/* [   g3  |   b3  |   g2  |   b2  |   g1  |   b1  |   g0  |   b0  ] */	\
    __xmm3 = _mm_unpackhi_epi16(__xmm0, __xmm1);/* [   a3  |   r3  |   a2  |   r2  |   a1  |   r1  |   a0  |   r0  ] */	\
    __xmm2 = _mm_packus_epi16(__xmm2, __xmm2);  /* [ g3| b3| g2| b2| g1| b1| g0| b0| g3| b3| g2| b2| g1| b1| g0| b0] */	\
    __xmm3 = _mm_packus_epi16(__xmm3, __xmm3);  /* [ a3| r3| a2| r2| a1| r1| a0| r0| a3| r3| a2| r2| a1| r1| a0| r0] */	\
    __xmm2 = _mm_unpacklo_epi16(__xmm2, __xmm3);/* [ a3| r3| g3| b3| a2| r2| g2| b2| a1| r1| g1| b1| a0| r0| g0| b0] */ \
	_mm_stream_si128((__m128i *)(bgra), __xmm2);                                                                        \
}

#define	__NVC_BGRA_PACK_FLOAT_4P_16BIT(xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps, bgra)	\
{														\
	__m128i __xmm0, __xmm1, __xmm2;						\
														\
	__xmm0 = _mm_cvtps_epi32(xmm_b_ps);					/* [   0   |   b3  |   0   |   b2  |   0   |   b1  |   0   |   b0  ] */	\
	__xmm1 = _mm_cvtps_epi32(xmm_g_ps);					/* [   0   |   g3  |   0   |   g2  |   0   |   g1  |   0   |   g0  ] */	\
	__xmm0 = _mm_sub_epi32(__xmm0, g_xmm_32bit_32768);																			\
	__xmm1 = _mm_sub_epi32(__xmm1, g_xmm_32bit_32768);																			\
	__xmm0 = _mm_packs_epi32(__xmm0, __xmm0);			/* [   b3  |   b2  |   b1  |   b0  |   b3  |   b2  |   b1  |   b0  ] */	\
	__xmm1 = _mm_packs_epi32(__xmm1, __xmm1);			/* [   g3  |   g2  |   g1  |   g0  |   g3  |   g2  |   g1  |   g0  ] */	\
	__xmm0 = _mm_unpacklo_epi16(__xmm0, __xmm1);		/* [   g3  |   b3  |   g2  |   b2  |   g1  |   b1  |   g0  |   b0  ] */	\
																																\
	__xmm1 = _mm_cvtps_epi32(xmm_r_ps);					/* [   0   |   r3  |   0   |   r2  |   0   |   r1  |   0   |   r0  ] */	\
	__xmm2 = _mm_cvtps_epi32(xmm_a_ps);					/* [   0   |   a3  |   0   |   a2  |   0   |   a1  |   0   |   a0  ] */	\
	__xmm1 = _mm_sub_epi32(__xmm1, g_xmm_32bit_32768);																			\
	__xmm2 = _mm_sub_epi32(__xmm2, g_xmm_32bit_32768);																			\
	__xmm1 = _mm_packs_epi32(__xmm1, __xmm1);			/* [   r3  |   r2  |   r1  |   r0  |   r3  |   r2  |   r1  |   r0  ] */	\
	__xmm2 = _mm_packs_epi32(__xmm2, __xmm2);			/* [   a3  |   a2  |   a1  |   a0  |   a3  |   a2  |   a1  |   a0  ] */	\
	__xmm1 = _mm_unpacklo_epi16(__xmm1, __xmm2);		/* [   a3  |   r3  |   a2  |   r2  |   a1  |   r1  |   a0  |   r0  ] */	\
																																\
	__xmm2 = _mm_unpacklo_epi32(__xmm0, __xmm1);		/* [   a1  |   r1  |   g1  |   b1  |   a0  |   r0  |   g0  |   b0  ] */	\
	__xmm0 = _mm_unpackhi_epi32(__xmm0, __xmm1);		/* [   a3  |   r3  |   g3  |   b3  |   a2  |   r2  |   g2  |   b2  ] */	\
	__xmm2 = _mm_add_epi16(__xmm2, g_xmm_16bit_32768);																			\
	__xmm0 = _mm_add_epi16(__xmm0, g_xmm_16bit_32768);																			\
	_mm_store_si128((__m128i *)bgra, __xmm2);																					\
	_mm_store_si128((__m128i *)((char *)(bgra) + 16), __xmm0);																	\
}

#define	__NVC_BGRA_PACK_FLOAT_4P_16BIT_UNALIGNED(xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps, bgra)									\
{																																\
	__m128i __xmm0, __xmm1, __xmm2;																								\
																																\
	__xmm0 = _mm_cvtps_epi32(xmm_b_ps);					/* [   0   |   b3  |   0   |   b2  |   0   |   b1  |   0   |   b0  ] */	\
	__xmm1 = _mm_cvtps_epi32(xmm_g_ps);					/* [   0   |   g3  |   0   |   g2  |   0   |   g1  |   0   |   g0  ] */	\
	__xmm0 = _mm_sub_epi32(__xmm0, g_xmm_32bit_32768);																			\
	__xmm1 = _mm_sub_epi32(__xmm1, g_xmm_32bit_32768);																			\
	__xmm0 = _mm_packs_epi32(__xmm0, __xmm0);			/* [   b3  |   b2  |   b1  |   b0  |   b3  |   b2  |   b1  |   b0  ] */	\
	__xmm1 = _mm_packs_epi32(__xmm1, __xmm1);			/* [   g3  |   g2  |   g1  |   g0  |   g3  |   g2  |   g1  |   g0  ] */	\
	__xmm0 = _mm_unpacklo_epi16(__xmm0, __xmm1);		/* [   g3  |   b3  |   g2  |   b2  |   g1  |   b1  |   g0  |   b0  ] */	\
																																\
	__xmm1 = _mm_cvtps_epi32(xmm_r_ps);					/* [   0   |   r3  |   0   |   r2  |   0   |   r1  |   0   |   r0  ] */	\
	__xmm2 = _mm_cvtps_epi32(xmm_a_ps);					/* [   0   |   a3  |   0   |   a2  |   0   |   a1  |   0   |   a0  ] */	\
	__xmm1 = _mm_sub_epi32(__xmm1, g_xmm_32bit_32768);																			\
	__xmm2 = _mm_sub_epi32(__xmm2, g_xmm_32bit_32768);																			\
	__xmm1 = _mm_packs_epi32(__xmm1, __xmm1);			/* [   r3  |   r2  |   r1  |   r0  |   r3  |   r2  |   r1  |   r0  ] */	\
	__xmm2 = _mm_packs_epi32(__xmm2, __xmm2);			/* [   a3  |   a2  |   a1  |   a0  |   a3  |   a2  |   a1  |   a0  ] */	\
	__xmm1 = _mm_unpacklo_epi16(__xmm1, __xmm2);		/* [   a3  |   r3  |   a2  |   r2  |   a1  |   r1  |   a0  |   r0  ] */	\
																																\
	__xmm2 = _mm_unpacklo_epi32(__xmm0, __xmm1);		/* [   a1  |   r1  |   g1  |   b1  |   a0  |   r0  |   g0  |   b0  ] */	\
	__xmm0 = _mm_unpackhi_epi32(__xmm0, __xmm1);		/* [   a3  |   r3  |   g3  |   b3  |   a2  |   r2  |   g2  |   b2  ] */	\
	__xmm2 = _mm_add_epi16(__xmm2, g_xmm_16bit_32768);																			\
	__xmm0 = _mm_add_epi16(__xmm0, g_xmm_16bit_32768);																			\
	_mm_storeu_si128((__m128i *)bgra, __xmm2);																					\
	_mm_storeu_si128((__m128i *)((char *)(bgra) + 16), __xmm0);																	\
}

#define	__NVC_BGRA_PACK_FLOAT_4P_16BIT_NT(xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps, bgra)											\
{																																\
	__m128i __xmm0, __xmm1, __xmm2;																								\
																																\
	__xmm0 = _mm_cvtps_epi32(xmm_b_ps);					/* [   0   |   b3  |   0   |   b2  |   0   |   b1  |   0   |   b0  ] */	\
	__xmm1 = _mm_cvtps_epi32(xmm_g_ps);					/* [   0   |   g3  |   0   |   g2  |   0   |   g1  |   0   |   g0  ] */	\
	__xmm0 = _mm_sub_epi32(__xmm0, g_xmm_32bit_32768);																			\
	__xmm1 = _mm_sub_epi32(__xmm1, g_xmm_32bit_32768);																			\
	__xmm0 = _mm_packs_epi32(__xmm0, __xmm0);			/* [   b3  |   b2  |   b1  |   b0  |   b3  |   b2  |   b1  |   b0  ] */	\
	__xmm1 = _mm_packs_epi32(__xmm1, __xmm1);			/* [   g3  |   g2  |   g1  |   g0  |   g3  |   g2  |   g1  |   g0  ] */	\
	__xmm0 = _mm_unpacklo_epi16(__xmm0, __xmm1);		/* [   g3  |   b3  |   g2  |   b2  |   g1  |   b1  |   g0  |   b0  ] */	\
																																\
	__xmm1 = _mm_cvtps_epi32(xmm_r_ps);					/* [   0   |   r3  |   0   |   r2  |   0   |   r1  |   0   |   r0  ] */	\
	__xmm2 = _mm_cvtps_epi32(xmm_a_ps);					/* [   0   |   a3  |   0   |   a2  |   0   |   a1  |   0   |   a0  ] */	\
	__xmm1 = _mm_sub_epi32(__xmm1, g_xmm_32bit_32768);																			\
	__xmm2 = _mm_sub_epi32(__xmm2, g_xmm_32bit_32768);																			\
	__xmm1 = _mm_packs_epi32(__xmm1, __xmm1);			/* [   r3  |   r2  |   r1  |   r0  |   r3  |   r2  |   r1  |   r0  ] */	\
	__xmm2 = _mm_packs_epi32(__xmm2, __xmm2);			/* [   a3  |   a2  |   a1  |   a0  |   a3  |   a2  |   a1  |   a0  ] */	\
	__xmm1 = _mm_unpacklo_epi16(__xmm1, __xmm2);		/* [   a3  |   r3  |   a2  |   r2  |   a1  |   r1  |   a0  |   r0  ] */	\
																																\
	__xmm2 = _mm_unpacklo_epi32(__xmm0, __xmm1);		/* [   a1  |   r1  |   g1  |   b1  |   a0  |   r0  |   g0  |   b0  ] */	\
	__xmm0 = _mm_unpackhi_epi32(__xmm0, __xmm1);		/* [   a3  |   r3  |   g3  |   b3  |   a2  |   r2  |   g2  |   b2  ] */	\
	__xmm2 = _mm_add_epi16(__xmm2, g_xmm_16bit_32768);																			\
	__xmm0 = _mm_add_epi16(__xmm0, g_xmm_16bit_32768);																			\
	_mm_stream_si128((__m128i *)bgra, __xmm2);																					\
	_mm_stream_si128((__m128i *)((char *)(bgra) + 16), __xmm0);																	\
}

#define	__NVC_BGRA_UNPACK_FLOAT_4P_32BIT(bgra, xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps)                                                  \
{                                                                                                                                       \
    __m128 __xmm_tmp0_ps, __xmm_tmp1_ps, __xmm_tmp2_ps, __xmm_tmp3_ps;                                                                  \
                                                                                                                                        \
    xmm_b_ps = _mm_load_ps((const float *)(bgra));              /* [       a0      |       r0      |       g0      |       b0      ] */ \
    xmm_g_ps = _mm_load_ps((const float *)(bgra) + 4);          /* [       a1      |       r1      |       g1      |       b1      ] */ \
    xmm_r_ps = _mm_load_ps((const float *)(bgra) + 8);          /* [       a2      |       r2      |       g2      |       b2      ] */ \
    xmm_a_ps = _mm_load_ps((const float *)(bgra) + 12);         /* [       a3      |       r3      |       g3      |       b3      ] */ \
                                                                                                                                        \
    __xmm_tmp0_ps = _mm_unpacklo_ps(xmm_b_ps, xmm_r_ps);        /* [       g2      |       g0      |       b2      |       b0      ] */ \
    __xmm_tmp1_ps = _mm_unpackhi_ps(xmm_b_ps, xmm_r_ps);        /* [       a2      |       a0      |       r2      |       r0      ] */ \
    __xmm_tmp2_ps = _mm_unpacklo_ps(xmm_g_ps, xmm_a_ps);        /* [       g3      |       g1      |       b3      |       b1      ] */ \
    __xmm_tmp3_ps = _mm_unpackhi_ps(xmm_g_ps, xmm_a_ps);        /* [       a3      |       a1      |       r3      |       r1      ] */ \
                                                                                                                                        \
    xmm_b_ps = _mm_unpacklo_ps(__xmm_tmp0_ps, __xmm_tmp2_ps);   /* [       b3      |       b2      |       b1      |       b0      ] */ \
    xmm_g_ps = _mm_unpackhi_ps(__xmm_tmp0_ps, __xmm_tmp2_ps);   /* [       g3      |       g2      |       g1      |       g0      ] */ \
    xmm_r_ps = _mm_unpacklo_ps(__xmm_tmp1_ps, __xmm_tmp3_ps);   /* [       r3      |       r2      |       r1      |       r0      ] */ \
    xmm_a_ps = _mm_unpackhi_ps(__xmm_tmp1_ps, __xmm_tmp3_ps);   /* [       a3      |       a2      |       a1      |       a0      ] */ \
}

#define	__NVC_BGRA_UNPACK_FLOAT_4P_32BIT_UNALIGNED(bgra, xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps)                                        \
{                                                                                                                                       \
    __m128 __xmm_tmp0_ps, __xmm_tmp1_ps, __xmm_tmp2_ps, __xmm_tmp3_ps;                                                                  \
                                                                                                                                        \
    xmm_b_ps = _mm_loadu_ps((const float *)(bgra));             /* [       a0      |       r0      |       g0      |       b0      ] */ \
    xmm_g_ps = _mm_loadu_ps((const float *)(bgra) + 4);         /* [       a1      |       r1      |       g1      |       b1      ] */ \
    xmm_r_ps = _mm_loadu_ps((const float *)(bgra) + 8);         /* [       a2      |       r2      |       g2      |       b2      ] */ \
    xmm_a_ps = _mm_loadu_ps((const float *)(bgra) + 12);        /* [       a3      |       r3      |       g3      |       b3      ] */ \
                                                                                                                                        \
    __xmm_tmp0_ps = _mm_unpacklo_ps(xmm_b_ps, xmm_r_ps);        /* [       g2      |       g0      |       b2      |       b0      ] */ \
    __xmm_tmp1_ps = _mm_unpackhi_ps(xmm_b_ps, xmm_r_ps);        /* [       a2      |       a0      |       r2      |       r0      ] */ \
    __xmm_tmp2_ps = _mm_unpacklo_ps(xmm_g_ps, xmm_a_ps);        /* [       g3      |       g1      |       b3      |       b1      ] */ \
    __xmm_tmp3_ps = _mm_unpackhi_ps(xmm_g_ps, xmm_a_ps);        /* [       a3      |       a1      |       r3      |       r1      ] */ \
                                                                                                                                        \
    xmm_b_ps = _mm_unpacklo_ps(__xmm_tmp0_ps, __xmm_tmp2_ps);   /* [       b3      |       b2      |       b1      |       b0      ] */ \
    xmm_g_ps = _mm_unpackhi_ps(__xmm_tmp0_ps, __xmm_tmp2_ps);   /* [       g3      |       g2      |       g1      |       g0      ] */ \
    xmm_r_ps = _mm_unpacklo_ps(__xmm_tmp1_ps, __xmm_tmp3_ps);   /* [       r3      |       r2      |       r1      |       r0      ] */ \
    xmm_a_ps = _mm_unpackhi_ps(__xmm_tmp1_ps, __xmm_tmp3_ps);   /* [       a3      |       a2      |       a1      |       a0      ] */ \
}

#define	__NVC_BGRA_PACK_FLOAT_4P_32BIT(xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps, bgra)                                                    \
{                                                                                                                                       \
    __m128 __xmm_tmp0_ps, __xmm_tmp1_ps, __xmm_tmp2_ps, __xmm_tmp3_ps;                                                                  \
                                                                                                                                        \
    __xmm_tmp0_ps = _mm_unpacklo_ps(xmm_b_ps, xmm_r_ps);        /* [       r1      |       b1      |       r0      |       b0      ] */ \
    __xmm_tmp1_ps = _mm_unpackhi_ps(xmm_b_ps, xmm_r_ps);        /* [       r3      |       b3      |       r2      |       b2      ] */ \
    __xmm_tmp2_ps = _mm_unpacklo_ps(xmm_g_ps, xmm_a_ps);        /* [       a1      |       g1      |       a0      |       g0      ] */ \
    __xmm_tmp3_ps = _mm_unpackhi_ps(xmm_g_ps, xmm_a_ps);        /* [       a3      |       g3      |       a2      |       g2      ] */ \
                                                                                                                                        \
    _mm_store_ps((float *)(bgra), _mm_unpacklo_ps(__xmm_tmp0_ps, __xmm_tmp2_ps));                                                       \
    _mm_store_ps((float *)(bgra) + 4, _mm_unpackhi_ps(__xmm_tmp0_ps, __xmm_tmp2_ps));                                                   \
    _mm_store_ps((float *)(bgra) + 8, _mm_unpacklo_ps(__xmm_tmp1_ps, __xmm_tmp3_ps));                                                   \
    _mm_store_ps((float *)(bgra) + 12, _mm_unpackhi_ps(__xmm_tmp1_ps, __xmm_tmp3_ps));                                                  \
}

#define	__NVC_BGRA_PACK_FLOAT_4P_32BIT_UNALIGNED(xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps, bgra)                                          \
{                                                                                                                                       \
    __m128 __xmm_tmp0_ps, __xmm_tmp1_ps, __xmm_tmp2_ps, __xmm_tmp3_ps;                                                                  \
                                                                                                                                        \
    __xmm_tmp0_ps = _mm_unpacklo_ps(xmm_b_ps, xmm_r_ps);        /* [       r1      |       b1      |       r0      |       b0      ] */ \
    __xmm_tmp1_ps = _mm_unpackhi_ps(xmm_b_ps, xmm_r_ps);        /* [       r3      |       b3      |       r2      |       b2      ] */ \
    __xmm_tmp2_ps = _mm_unpacklo_ps(xmm_g_ps, xmm_a_ps);        /* [       a1      |       g1      |       a0      |       g0      ] */ \
    __xmm_tmp3_ps = _mm_unpackhi_ps(xmm_g_ps, xmm_a_ps);        /* [       a3      |       g3      |       a2      |       g2      ] */ \
                                                                                                                                        \
    _mm_storeu_ps((float *)(bgra), _mm_unpacklo_ps(__xmm_tmp0_ps, __xmm_tmp2_ps));                                                      \
    _mm_storeu_ps((float *)(bgra) + 4, _mm_unpackhi_ps(__xmm_tmp0_ps, __xmm_tmp2_ps));                                                  \
    _mm_storeu_ps((float *)(bgra) + 8, _mm_unpacklo_ps(__xmm_tmp1_ps, __xmm_tmp3_ps));                                                  \
    _mm_storeu_ps((float *)(bgra) + 12, _mm_unpackhi_ps(__xmm_tmp1_ps, __xmm_tmp3_ps));                                                 \
}

#define	__NVC_BGRA_PACK_FLOAT_4P_32BIT_NT(xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps, bgra)                                                 \
{                                                                                                                                       \
    __m128 __xmm_tmp0_ps, __xmm_tmp1_ps, __xmm_tmp2_ps, __xmm_tmp3_ps;                                                                  \
                                                                                                                                        \
    __xmm_tmp0_ps = _mm_unpacklo_ps(xmm_b_ps, xmm_r_ps);        /* [       r1      |       b1      |       r0      |       b0      ] */ \
    __xmm_tmp1_ps = _mm_unpackhi_ps(xmm_b_ps, xmm_r_ps);        /* [       r3      |       b3      |       r2      |       b2      ] */ \
    __xmm_tmp2_ps = _mm_unpacklo_ps(xmm_g_ps, xmm_a_ps);        /* [       a1      |       g1      |       a0      |       g0      ] */ \
    __xmm_tmp3_ps = _mm_unpackhi_ps(xmm_g_ps, xmm_a_ps);        /* [       a3      |       g3      |       a2      |       g2      ] */ \
                                                                                                                                        \
    _mm_stream_ps((float *)(bgra), _mm_unpacklo_ps(__xmm_tmp0_ps, __xmm_tmp2_ps));                                                      \
    _mm_stream_ps((float *)(bgra) + 4, _mm_unpackhi_ps(__xmm_tmp0_ps, __xmm_tmp2_ps));                                                  \
    _mm_stream_ps((float *)(bgra) + 8, _mm_unpacklo_ps(__xmm_tmp1_ps, __xmm_tmp3_ps));                                                  \
    _mm_stream_ps((float *)(bgra) + 12, _mm_unpackhi_ps(__xmm_tmp1_ps, __xmm_tmp3_ps));                                                 \
}


#define	__NVC_BGRA_UNPACK_INT_8P_8BIT(bgra, xmm_r, xmm_g, xmm_b, xmm_a)		\
{																			\
	__m128i __xmm0, __xmm1, __xmm2, __xmm_zero;								\
																			\
	__xmm0 = _mm_load_si128((__m128i *)(bgra));					/* [ a3| r3| g3| b3| a2| r2| g2| b2| a1| r1| g1| b1| a0| r0| g0| b0] */	\
	__xmm1 = _mm_load_si128((__m128i *)((char *)(bgra) + 16));	/* [ a7| r7| g7| b7| a6| r6| g6| b6| a5| r5| g5| b5| a4| r4| g4| b4] */	\
																																		\
	__xmm2 = _mm_unpacklo_epi8(__xmm0, __xmm1);			/* [ a5| a1| r5| r1| g5| g1| b5| b1| a4| a0| r4| r0| g4| g0| b4| b0] */			\
	__xmm0 = _mm_unpackhi_epi8(__xmm0, __xmm1);			/* [ a7| a3| r7| r3| g7| g3| b7| b3| a6| a2| r6| r2| g6| g2| b6| b2] */			\
	__xmm1 = _mm_unpacklo_epi8(__xmm2, __xmm0);			/* [ a6| a4| a2| a0| r6| r4| r2| r0| g6| g4| g2| g0| b6| b4| b2| b0] */			\
	__xmm2 = _mm_unpackhi_epi8(__xmm2, __xmm0);			/* [ a7| a5| a3| a1| r7| r5| r3| r1| g7| g5| g3| g1| b7| b5| b3| b1] */			\
	__xmm0 = _mm_unpacklo_epi8(__xmm1, __xmm2);			/* [ g7| g6| g5| g4| g3| g2| g1| g0| b7| b6| b5| b4| b3| b2| b1| b0] */			\
	__xmm1 = _mm_unpackhi_epi8(__xmm1, __xmm2);			/* [ a7| a6| a5| a4| a3| a2| a1| a0| r7| r6| r5| r4| r3| r2| r1| r0] */			\
																																		\
	__xmm_zero = _mm_setzero_si128();																									\
	xmm_b = _mm_unpacklo_epi8(__xmm0, __xmm_zero);		/* [   b7  |   b6  |   b5  |   b4  |   b3  |   b2  |   b1  |   b0  ] */			\
	xmm_g = _mm_unpackhi_epi8(__xmm0, __xmm_zero);		/* [   g7  |   g6  |   g5  |   g4  |   g3  |   g2  |   g1  |   g0  ] */			\
	xmm_r = _mm_unpacklo_epi8(__xmm1, __xmm_zero);		/* [   r7  |   r6  |   r5  |   r4  |   r3  |   r2  |   r1  |   r0  ] */			\
	xmm_a = _mm_unpackhi_epi8(__xmm1, __xmm_zero);		/* [   a7  |   a6  |   a5  |   a4  |   a3  |   a2  |   a1  |   a0  ] */			\
}

#define __NVC_ARGB_UNPACK_INT_8P_8BIT(argb, xmm_r, xmm_g, xmm_b, xmm_a) __NVC_BGRA_UNPACK_INT_8P_8BIT(argb, xmm_g, xmm_r, xmm_a, xmm_b)
#define __NVC_RGBA_UNPACK_INT_8P_8BIT(rgba, xmm_r, xmm_g, xmm_b, xmm_a) __NVC_BGRA_UNPACK_INT_8P_8BIT(rgba, xmm_b, xmm_g, xmm_r, xmm_a)

static NVC_FORCE_INLINE void __NVC_BGR_UNPACK_INT_8P_8BIT(const uint8_t* bgr,
														  __m128i& xmm_r,
														  __m128i& xmm_g,
														  __m128i& xmm_b)
{
	uint8_t r[8] = { 0 };
	uint8_t g[8] = { 0 };
	uint8_t b[8] = { 0 };

	for (size_t i = 0; i < 8; i++)
	{
		b[i] = bgr[i * 3];											
		g[i] = bgr[i * 3 + 1];										
		r[i] = bgr[i * 3 + 2];
	}

	xmm_r = _mm_setr_epi16(r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7]);
	xmm_g = _mm_setr_epi16(g[0], g[1], g[2], g[3], g[4], g[5], g[6], g[7]);
	xmm_b = _mm_setr_epi16(b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]);
}

static NVC_FORCE_INLINE void __NVC_RGB_UNPACK_INT_8P_8BIT(const uint8_t* rgb,
														  __m128i& xmm_r,
														  __m128i& xmm_g,
														  __m128i& xmm_b)
{
	uint8_t r[8] = { 0 };											
	uint8_t g[8] = { 0 };
	uint8_t b[8] = { 0 };

	for (size_t i = 0; i < 8; i++)
	{
		r[i] = rgb[i * 3];
		g[i] = rgb[i * 3 + 1];
		b[i] = rgb[i * 3 + 2];
	}

	xmm_r = _mm_setr_epi16(r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7]);
	xmm_g = _mm_setr_epi16(g[0], g[1], g[2], g[3], g[4], g[5], g[6], g[7]);
	xmm_b = _mm_setr_epi16(b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]);
}

static NVC_FORCE_INLINE void __NVC_BGR_UNPACK_INT_8P_16BIT(const uint16_t* bgr,
	__m128i& xmm_b,
	__m128i& xmm_g,
	__m128i& xmm_r)
{
	uint16_t r[8] = { 0 };
	uint16_t g[8] = { 0 };
	uint16_t b[8] = { 0 };

	for (size_t i = 0; i < 8; i++)
	{
		b[i] = bgr[i * 3];
		g[i] = bgr[i * 3 + 1];
		r[i] = bgr[i * 3 + 2];
	}

	xmm_r = _mm_set_epi16(r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7]);
	xmm_g = _mm_set_epi16(g[0], g[1], g[2], g[3], g[4], g[5], g[6], g[7]);
	xmm_b = _mm_set_epi16(b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]);
}

static NVC_FORCE_INLINE void __NVC_RGB_UNPACK_INT_8P_16BIT(const uint16_t* rgb,
	__m128i& xmm_r,
	__m128i& xmm_g,
	__m128i& xmm_b)
{
	uint16_t r[8] = { 0 };
	uint16_t g[8] = { 0 };
	uint16_t b[8] = { 0 };

	for (size_t i = 0; i < 8; i++)
	{
		r[i] = rgb[i * 3];
		g[i] = rgb[i * 3 + 1];
		b[i] = rgb[i * 3 + 2];
	}

	xmm_r = _mm_set_epi16(r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7]);
	xmm_g = _mm_set_epi16(g[0], g[1], g[2], g[3], g[4], g[5], g[6], g[7]);
	xmm_b = _mm_set_epi16(b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]);
}

#define	__NVC_BGRA_UNPACK_INT_8P_8BIT_UNALIGNED(bgra, xmm_r, xmm_g, xmm_b, xmm_a)	\
{																					\
	__m128i __xmm0, __xmm1, __xmm2, __xmm_zero;										\
	\
	__xmm0 = _mm_loadu_si128((__m128i *)(bgra));				/* [ a3| r3| g3| b3| a2| r2| g2| b2| a1| r1| g1| b1| a0| r0| g0| b0] */	\
	__xmm1 = _mm_loadu_si128((__m128i *)((char *)(bgra) + 16));	/* [ a7| r7| g7| b7| a6| r6| g6| b6| a5| r5| g5| b5| a4| r4| g4| b4] */	\
	\
	__xmm2 = _mm_unpacklo_epi8(__xmm0, __xmm1);			/* [ a5| a1| r5| r1| g5| g1| b5| b1| a4| a0| r4| r0| g4| g0| b4| b0] */			\
	__xmm0 = _mm_unpackhi_epi8(__xmm0, __xmm1);			/* [ a7| a3| r7| r3| g7| g3| b7| b3| a6| a2| r6| r2| g6| g2| b6| b2] */			\
	__xmm1 = _mm_unpacklo_epi8(__xmm2, __xmm0);			/* [ a6| a4| a2| a0| r6| r4| r2| r0| g6| g4| g2| g0| b6| b4| b2| b0] */			\
	__xmm2 = _mm_unpackhi_epi8(__xmm2, __xmm0);			/* [ a7| a5| a3| a1| r7| r5| r3| r1| g7| g5| g3| g1| b7| b5| b3| b1] */			\
	__xmm0 = _mm_unpacklo_epi8(__xmm1, __xmm2);			/* [ g7| g6| g5| g4| g3| g2| g1| g0| b7| b6| b5| b4| b3| b2| b1| b0] */			\
	__xmm1 = _mm_unpackhi_epi8(__xmm1, __xmm2);			/* [ a7| a6| a5| a4| a3| a2| a1| a0| r7| r6| r5| r4| r3| r2| r1| r0] */			\
	\
	__xmm_zero = _mm_setzero_si128();																									\
	xmm_b = _mm_unpacklo_epi8(__xmm0, __xmm_zero);		/* [   b7  |   b6  |   b5  |   b4  |   b3  |   b2  |   b1  |   b0  ] */			\
	xmm_g = _mm_unpackhi_epi8(__xmm0, __xmm_zero);		/* [   g7  |   g6  |   g5  |   g4  |   g3  |   g2  |   g1  |   g0  ] */			\
	xmm_r = _mm_unpacklo_epi8(__xmm1, __xmm_zero);		/* [   r7  |   r6  |   r5  |   r4  |   r3  |   r2  |   r1  |   r0  ] */			\
	xmm_a = _mm_unpackhi_epi8(__xmm1, __xmm_zero);		/* [   a7  |   a6  |   a5  |   a4  |   a3  |   a2  |   a1  |   a0  ] */			\
}

#define __NVC_ARGB_UNPACK_INT_8P_8BIT_UNALIGNED(argb, xmm_r, xmm_g, xmm_b, xmm_a) __NVC_BGRA_UNPACK_INT_8P_8BIT_UNALIGNED(argb, xmm_g, xmm_r, xmm_a, xmm_b)
#define __NVC_RGBA_UNPACK_INT_8P_8BIT_UNALIGNED(rgba, xmm_r, xmm_g, xmm_b, xmm_a) __NVC_BGRA_UNPACK_INT_8P_8BIT_UNALIGNED(rgba, xmm_b, xmm_g, xmm_r, xmm_a)

#define	__NVC_YUVA4444_UNPACK_INT_8P_8BIT(yuva, xmm_y, xmm_u, xmm_v, xmm_a)	\
{																					\
	__m128i __xmm0, __xmm1, __xmm2, __xmm_zero;										\
	\
	__xmm0 = _mm_load_si128((__m128i *)(yuva));					/* [ a3| v3| u3| y3| a2| v2| u2| y2| a1| v1| u1| y1| a0| v0| u0| y0] */	\
	__xmm1 = _mm_load_si128((__m128i *)((char *)(yuva) + 16));	/* [ a7| v7| u7| y7| a6| v6| u6| y6| a5| v5| u5| y5| a4| v4| u4| y4] */	\
	\
	__xmm2 = _mm_unpacklo_epi8(__xmm0, __xmm1);			/* [ a5| a1| v5| v1| u5| u1| y5| y1| a4| a0| v4| v0| u4| u0| y4| y0] */			\
	__xmm0 = _mm_unpackhi_epi8(__xmm0, __xmm1);			/* [ a7| a3| v7| v3| u7| u3| y7| y3| a6| a2| v6| v2| u6| u2| y6| y2] */			\
	__xmm1 = _mm_unpacklo_epi8(__xmm2, __xmm0);			/* [ a6| a4| a2| a0| v6| v4| v2| v0| u6| u4| u2| u0| y6| y4| y2| y0] */			\
	__xmm2 = _mm_unpackhi_epi8(__xmm2, __xmm0);			/* [ a7| a5| a3| a1| v7| v5| v3| v1| u7| u5| u3| u1| y7| y5| y3| y1] */			\
	__xmm0 = _mm_unpacklo_epi8(__xmm1, __xmm2);			/* [ u7| u6| u5| u4| u3| u2| u1| u0| y7| y6| y5| y4| y3| y2| y1| y0] */			\
	__xmm1 = _mm_unpackhi_epi8(__xmm1, __xmm2);			/* [ a7| a6| a5| a4| a3| a2| a1| a0| v7| v6| v5| v4| v3| v2| v1| v0] */			\
	\
	__xmm_zero = _mm_setzero_si128();																									\
	xmm_y = _mm_unpacklo_epi8(__xmm0, __xmm_zero);		/* [   y7  |   y6  |   y5  |   y4  |   y3  |   y2  |   y1  |   y0  ] */			\
	xmm_u = _mm_unpackhi_epi8(__xmm0, __xmm_zero);		/* [   u7  |   u6  |   u5  |   u4  |   u3  |   u2  |   u1  |   u0  ] */			\
	xmm_v = _mm_unpacklo_epi8(__xmm1, __xmm_zero);		/* [   v7  |   v6  |   v5  |   v4  |   v3  |   v2  |   v1  |   v0  ] */			\
	xmm_a = _mm_unpackhi_epi8(__xmm1, __xmm_zero);		/* [   a7  |   a6  |   a5  |   a4  |   a3  |   a2  |   a1  |   a0  ] */			\
}

#define	__NVC_YUVA4444_UNPACK_INT_8P_8BIT_UNALIGNED(yuva, xmm_y, xmm_u, xmm_v, xmm_a)	\
{																					\
	__m128i __xmm0, __xmm1, __xmm2, __xmm_zero;										\
	\
	__xmm0 = _mm_loadu_si128((__m128i *)(yuva));				/* [ a3| v3| u3| y3| a2| v2| u2| y2| a1| v1| u1| y1| a0| v0| u0| y0] */	\
	__xmm1 = _mm_loadu_si128((__m128i *)((char *)(yuva) + 16));	/* [ a7| v7| u7| y7| a6| v6| u6| y6| a5| v5| u5| y5| a4| v4| u4| y4] */	\
	\
	__xmm2 = _mm_unpacklo_epi8(__xmm0, __xmm1);			/* [ a5| a1| v5| v1| u5| u1| y5| y1| a4| a0| v4| v0| u4| u0| y4| y0] */			\
	__xmm0 = _mm_unpackhi_epi8(__xmm0, __xmm1);			/* [ a7| a3| v7| v3| u7| u3| y7| y3| a6| a2| v6| v2| u6| u2| y6| y2] */			\
	__xmm1 = _mm_unpacklo_epi8(__xmm2, __xmm0);			/* [ a6| a4| a2| a0| v6| v4| v2| v0| u6| u4| u2| u0| y6| y4| y2| y0] */			\
	__xmm2 = _mm_unpackhi_epi8(__xmm2, __xmm0);			/* [ a7| a5| a3| a1| v7| v5| v3| v1| u7| u5| u3| u1| y7| y5| y3| y1] */			\
	__xmm0 = _mm_unpacklo_epi8(__xmm1, __xmm2);			/* [ u7| u6| u5| u4| u3| u2| u1| u0| y7| y6| y5| y4| y3| y2| y1| y0] */			\
	__xmm1 = _mm_unpackhi_epi8(__xmm1, __xmm2);			/* [ a7| a6| a5| a4| a3| a2| a1| a0| v7| v6| v5| v4| v3| v2| v1| v0] */			\
	\
	__xmm_zero = _mm_setzero_si128();																									\
	xmm_y = _mm_unpacklo_epi8(__xmm0, __xmm_zero);		/* [   y7  |   y6  |   y5  |   y4  |   y3  |   y2  |   y1  |   y0  ] */			\
	xmm_u = _mm_unpackhi_epi8(__xmm0, __xmm_zero);		/* [   u7  |   u6  |   u5  |   u4  |   u3  |   u2  |   u1  |   u0  ] */			\
	xmm_v = _mm_unpacklo_epi8(__xmm1, __xmm_zero);		/* [   v7  |   v6  |   v5  |   v4  |   v3  |   v2  |   v1  |   v0  ] */			\
	xmm_a = _mm_unpackhi_epi8(__xmm1, __xmm_zero);		/* [   a7  |   a6  |   a5  |   a4  |   a3  |   a2  |   a1  |   a0  ] */			\
}

#define __NVC_YUVA4224_UNPACK_INT_8P_8BIT(yuva, xmm_a0, xmm_y, xmm_u, xmm_v, xmm_a1)			\
{																			\
	__m128i __xmm0, __xmm1;													\
	\
	__xmm0 = _mm_load_si128((__m128i *)(yuva));							/* [ v2| y3| u2| y2| v0| y1| u0| y0| a7| a6| a5| a4| a3| a2| a1| a0] */		\
	\
	xmm_a0 = _mm_unpacklo_epi8(__xmm0, _mm_setzero_si128());			/* [   a7  |   a6  |   a5  |   a4  |   a3  |   a2  |   a1  |   a0  ] */		\
	\
	__xmm1 = _mm_load_si128((__m128i *)((unsigned char *)(yuva) + 16));	/* [ a7| a6| a5| a4| a3| a2| a1| a0| v6| y7| u6| y6| v4| y5| u4| y4] */		\
	\
	xmm_a1 = _mm_unpackhi_epi8(__xmm1, _mm_setzero_si128());			/* [   a7  |   a6  |   a5  |   a4  |   a3  |   a2  |   a1  |   a0  ] */		\
	\
	__xmm1 = _mm_slli_si128(__xmm1, 8);									/* [ v6| y7| u6| y6| v4| y5| u4| y4|			0				   ] */		\
	__xmm0 = _mm_unpackhi_epi64(__xmm0, __xmm1);						/* [ v6| y7| u6| y6| v4| y5| u4| y4| v2| y3| u2| y2| v0| y1| u0| y0] */		\
	xmm_y = _mm_and_si128(__xmm0, g_xmm_YUY2_LowByteMatte);				/* [   y7  |   y6  |   y5  |   y4  |   y3  |   y2  |   y1  |   y0  ] */		\
	__xmm0 = _mm_srli_epi16(__xmm0, 8);									/* [ 0 | v6| 0 | u6| 0 | v4| 0 | u4| 0 | v2| 0 | u2| 0 | v0| 0 | u0] */		\
	__xmm1 = _mm_shufflelo_epi16(__xmm0, 0xA0);							/* [ 0 | v6| 0 | u6| 0 | v4| 0 | u4|   u2  |   u2  |   u0  |   u0  ] */		\
	xmm_u = _mm_shufflehi_epi16(__xmm1, 0xA0);							/* [   u6  |   u6  |   u4  |   u4  |   u2  |   u2  |   u0  |   u0  ] */		\
	__xmm1 = _mm_shufflelo_epi16(__xmm0, 0xF5);							/* [ 0 | v6| 0 | u6| 0 | v4| 0 | u4|   v2  |   v2  |   v0  |   v0  ] */		\
	xmm_v = _mm_shufflehi_epi16(__xmm1, 0xF5);							/* [   v6  |   v6  |   v4  |   v4  |   v2  |   v2  |   v0  |   v0  ] */		\
}

#define __NVC_YUVA4224_UNPACK_INT_8P_8BIT_UNALIGNED(yuva, xmm_a0, xmm_y, xmm_u, xmm_v, xmm_a1)			\
{																			\
	__m128i __xmm0, __xmm1;													\
	\
	__xmm0 = _mm_loadu_si128((__m128i *)(yuva));							/* [ v2| y3| u2| y2| v0| y1| u0| y0| a7| a6| a5| a4| a3| a2| a1| a0] */		\
	\
	xmm_a0 = _mm_unpacklo_epi8(__xmm0, _mm_setzero_si128());			/* [   a7  |   a6  |   a5  |   a4  |   a3  |   a2  |   a1  |   a0  ] */		\
	\
	__xmm1 = _mm_loadu_si128((__m128i *)((unsigned char *)(yuva) + 16));	/* [ a7| a6| a5| a4| a3| a2| a1| a0| v6| y7| u6| y6| v4| y5| u4| y4] */		\
	\
	xmm_a1 = _mm_unpackhi_epi8(__xmm1, _mm_setzero_si128());			/* [   a7  |   a6  |   a5  |   a4  |   a3  |   a2  |   a1  |   a0  ] */		\
	\
	__xmm1 = _mm_slli_si128(__xmm1, 8);									/* [ v6| y7| u6| y6| v4| y5| u4| y4|			0				   ] */		\
	__xmm0 = _mm_unpackhi_epi64(__xmm0, __xmm1);						/* [ v6| y7| u6| y6| v4| y5| u4| y4| v2| y3| u2| y2| v0| y1| u0| y0] */		\
	xmm_y = _mm_and_si128(__xmm0, g_xmm_YUY2_LowByteMatte);				/* [   y7  |   y6  |   y5  |   y4  |   y3  |   y2  |   y1  |   y0  ] */		\
	__xmm0 = _mm_srli_epi16(__xmm0, 8);									/* [ 0 | v6| 0 | u6| 0 | v4| 0 | u4| 0 | v2| 0 | u2| 0 | v0| 0 | u0] */		\
	__xmm1 = _mm_shufflelo_epi16(__xmm0, 0xA0);							/* [ 0 | v6| 0 | u6| 0 | v4| 0 | u4|   u2  |   u2  |   u0  |   u0  ] */		\
	xmm_u = _mm_shufflehi_epi16(__xmm1, 0xA0);							/* [   u6  |   u6  |   u4  |   u4  |   u2  |   u2  |   u0  |   u0  ] */		\
	__xmm1 = _mm_shufflelo_epi16(__xmm0, 0xF5);							/* [ 0 | v6| 0 | u6| 0 | v4| 0 | u4|   v2  |   v2  |   v0  |   v0  ] */		\
	xmm_v = _mm_shufflehi_epi16(__xmm1, 0xF5);							/* [   v6  |   v6  |   v4  |   v4  |   v2  |   v2  |   v0  |   v0  ] */		\
}

#define __NVC_YUY2_UNPACK_INT_8P_8BIT(yuy2, xmm_y, xmm_u, xmm_v)			\
{																			\
	__m128i __xmm0, __xmm1;													\
	\
	__xmm0 = _mm_load_si128((__m128i *)(yuy2));				/* [ v6| y7| u6| y6| v4| y5| u4| y4| v2| y3| u2| y2| v0| y1| u0| y0] */		\
	\
	xmm_y = _mm_and_si128(__xmm0, g_xmm_YUY2_LowByteMatte);	/* [   y7  |   y6  |   y5  |   y4  |   y3  |   y2  |   y1  |   y0  ] */		\
	__xmm0 = _mm_srli_epi16(__xmm0, 8);						/* [ 0 | v6| 0 | u6| 0 | v4| 0 | u4| 0 | v2| 0 | u2| 0 | v0| 0 | u0] */		\
	__xmm1 = _mm_shufflelo_epi16(__xmm0, 0xA0);				/* [ 0 | v6| 0 | u6| 0 | v4| 0 | u4|   u2  |   u2  |   u0  |   u0  ] */		\
	xmm_u = _mm_shufflehi_epi16(__xmm1, 0xA0);				/* [   u6  |   u6  |   u4  |   u4  |   u2  |   u2  |   u0  |   u0  ] */		\
	__xmm1 = _mm_shufflelo_epi16(__xmm0, 0xF5);				/* [ 0 | v6| 0 | u6| 0 | v4| 0 | u4|   v2  |   v2  |   v0  |   v0  ] */		\
	xmm_v = _mm_shufflehi_epi16(__xmm1, 0xF5);				/* [   v6  |   v6  |   v4  |   v4  |   v2  |   v2  |   v0  |   v0  ] */		\
}

#define __NVC_YUY2_UNPACK_INT_8P_8BIT_UNALIGNED(yuy2, xmm_y, xmm_u, xmm_v)	\
{																			\
	__m128i __xmm0, __xmm1;													\
	\
	__xmm0 = _mm_loadu_si128((__m128i *)(yuy2));			/* [ v6| y7| u6| y6| v4| y5| u4| y4| v2| y3| u2| y2| v0| y1| u0| y0] */		\
	\
	xmm_y = _mm_and_si128(__xmm0, g_xmm_YUY2_LowByteMatte);	/* [   y7  |   y6  |   y5  |   y4  |   y3  |   y2  |   y1  |   y0  ] */		\
	__xmm0 = _mm_srli_epi16(__xmm0, 8);						/* [ 0 | v6| 0 | u6| 0 | v4| 0 | u4| 0 | v2| 0 | u2| 0 | v0| 0 | u0] */		\
	__xmm1 = _mm_shufflelo_epi16(__xmm0, 0xA0);				/* [ 0 | v6| 0 | u6| 0 | v4| 0 | u4|   u2  |   u2  |   u0  |   u0  ] */		\
	xmm_u = _mm_shufflehi_epi16(__xmm1, 0xA0);				/* [   u6  |   u6  |   u4  |   u4  |   u2  |   u2  |   u0  |   u0  ] */		\
	__xmm1 = _mm_shufflelo_epi16(__xmm0, 0xF5);				/* [ 0 | v6| 0 | u6| 0 | v4| 0 | u4|   v2  |   v2  |   v0  |   v0  ] */		\
	xmm_v = _mm_shufflehi_epi16(__xmm1, 0xF5);				/* [   v6  |   v6  |   v4  |   v4  |   v2  |   v2  |   v0  |   v0  ] */		\
}

#define __NVC_UYVY422_UNPACK_INT_8P_8BIT(uyvy, xmm_y, xmm_u, xmm_v)			\
{																			\
	__m128i __xmm0, __xmm1, __xmm2;													\
	\
	__xmm0 = _mm_load_si128((__m128i *)(uyvy));				/* [ y7|v6 | y6|u6 | y5|v4 | y4|u4 | y3|v2 | y2|u2 |  y1|v0 | y0 |u0 ] */		\
	\
	xmm_y = _mm_and_si128(__xmm0, g_xmm_UYVY422_LowByteMatte);	/*[y7   |y6   |y5   |y4   |y3   |y2   | y1   |y0   ] */		\
	__xmm2 = xmm_y;                                                                                                             \
	xmm_y = _mm_srli_epi16( __xmm2,8 );                        /* [   y7  |   y6  |   y5  |   y4  |   y3  |   y2  |   y1  |   y0  ] */		\
   __xmm0 = _mm_and_si128(__xmm0, g_xmm_YUY2_LowByteMatte);	/* [ 0 | v6| 0 | u6| 0 | v4| 0 | u4| 0 | v2| 0 | u2| 0 | v0| 0 | u0] */		\
	__xmm1 = _mm_shufflelo_epi16(__xmm0, 0xA0);				/* [ 0 | v6| 0 | u6| 0 | v4| 0 | u4|   u2  |   u2  |   u0  |   u0  ] */		\
	xmm_u = _mm_shufflehi_epi16(__xmm1, 0xA0);				/* [   u6  |   u6  |   u4  |   u4  |   u2  |   u2  |   u0  |   u0  ] */		\
	__xmm1 = _mm_shufflelo_epi16(__xmm0, 0xF5);				/* [ 0 | v6| 0 | u6| 0 | v4| 0 | u4|   v2  |   v2  |   v0  |   v0  ] */		\
	xmm_v = _mm_shufflehi_epi16(__xmm1, 0xF5);				/* [   v6  |   v6  |   v4  |   v4  |   v2  |   v2  |   v0  |   v0  ] */		\
}

#define __NVC_UYVY422_UNPACK_INT_8P_8BIT_UNALIGNED(uyvy, xmm_y, xmm_u, xmm_v)	    \
{																			        \
	__m128i __xmm0, __xmm1,__xmm2;													\
	\
	__xmm0 = _mm_loadu_si128((__m128i *)(uyvy));			/* [ y7|v6 | y6|u6 | y5|v4 | y4|u4 | y3|v2 | y2|u2 |  y1|v0 | y0 |u0 ] */\
	\
	xmm_y = _mm_and_si128(__xmm0, g_xmm_UYVY422_LowByteMatte);	/*[y7   |y6   |y5   |y4   |y3   |y2   | y1   |y0   ] */		         \
	__xmm2 = xmm_y;                         \
	xmm_y = _mm_srli_epi16( __xmm2,8 );                        /* [   y7  |   y6  |   y5  |   y4  |   y3  |   y2  |   y1  |   y0  ] */\
	\
	__xmm0 = _mm_and_si128(__xmm0, g_xmm_YUY2_LowByteMatte);	/* [ 0 | v6| 0 | u6| 0 | v4| 0 | u4| 0 | v2| 0 | u2| 0 | v0| 0 | u0] */\
	__xmm1 = _mm_shufflelo_epi16(__xmm0, 0xA0);				/* [ 0 | v6| 0 | u6| 0 | v4| 0 | u4|   u2  |   u2  |   u0  |   u0  ] */		\
	xmm_u = _mm_shufflehi_epi16(__xmm1, 0xA0);				/* [   u6  |   u6  |   u4  |   u4  |   u2  |   u2  |   u0  |   u0  ] */		\
	__xmm1 = _mm_shufflelo_epi16(__xmm0, 0xF5);				/* [ 0 | v6| 0 | u6| 0 | v4| 0 | u4|   v2  |   v2  |   v0  |   v0  ] */		\
	xmm_v = _mm_shufflehi_epi16(__xmm1, 0xF5);				/* [   v6  |   v6  |   v4  |   v4  |   v2  |   v2  |   v0  |   v0  ] */		\
}


#define	__NVC_BGRA_PACK_INT_8P_8BIT(xmm_r, xmm_g, xmm_b, xmm_a, bgra)		\
{																			\
	__m128i __xmm0, __xmm1, __xmm2;											\
																			\
	__xmm0 = _mm_packus_epi16(xmm_b, xmm_b);			/* [ b7| b6| b5| b4| b3| b2| b1| b0| b7| b6| b5| b4| b3| b2| b1| b0] */	\
	__xmm1 = _mm_packus_epi16(xmm_g, xmm_g);			/* [ g7| g6| g5| g4| g3| g2| g1| g0| g7| g6| g5| g4| g3| g2| g1| g0] */	\
	__xmm0 = _mm_unpacklo_epi8(__xmm0, __xmm1);			/* [ g7| b7| g6| b6| g5| b5| g4| b4| g3| b3| g2| b2| g1| b1| g0| b0] */	\
																																\
	__xmm1 = _mm_packus_epi16(xmm_r, xmm_r);			/* [ r7| r6| r5| r4| r3| r2| r1| r0| r7| r6| r5| r4| r3| r2| r1| r0] */	\
	__xmm2 = _mm_packus_epi16(xmm_a, xmm_a);			/* [ a7| a6| a5| a4| a3| a2| a1| a0| a7| a6| a5| a4| a3| a2| a1| a0] */	\
	__xmm1 = _mm_unpacklo_epi8(__xmm1, __xmm2);			/* [ a7| r7| a6| r6| a5| r5| a4| r4| a3| r3| a2| r2| a1| r1| a0| r0] */	\
																																\
	__xmm2 = _mm_unpacklo_epi16(__xmm0, __xmm1);		/* [ a3| r3| g3| b3| a2| r2| g2| b2| a1| r1| g1| b1| a0| r0| g0| b0] */	\
	__xmm0 = _mm_unpackhi_epi16(__xmm0, __xmm1);		/* [ a7| r7| g7| b7| a6| r6| g6| b6| a5| r5| g5| b5| a4| r4| g4| b4] */	\
																																\
	_mm_store_si128((__m128i *)(bgra), __xmm2);																					\
	_mm_store_si128((__m128i *)((char *)(bgra) + 16), __xmm0);																	\
}

#define	__NVC_BGRA_PACK_INT_8P_8BIT_UNALIGNED(xmm_r, xmm_g, xmm_b, xmm_a, bgra)		\
{																					\
	__m128i __xmm0, __xmm1, __xmm2;													\
	\
	__xmm0 = _mm_packus_epi16(xmm_b, xmm_b);			/* [ b7| b6| b5| b4| b3| b2| b1| b0| b7| b6| b5| b4| b3| b2| b1| b0] */	\
	__xmm1 = _mm_packus_epi16(xmm_g, xmm_g);			/* [ g7| g6| g5| g4| g3| g2| g1| g0| g7| g6| g5| g4| g3| g2| g1| g0] */	\
	__xmm0 = _mm_unpacklo_epi8(__xmm0, __xmm1);			/* [ g7| b7| g6| b6| g5| b5| g4| b4| g3| b3| g2| b2| g1| b1| g0| b0] */	\
	\
	__xmm1 = _mm_packus_epi16(xmm_r, xmm_r);			/* [ r7| r6| r5| r4| r3| r2| r1| r0| r7| r6| r5| r4| r3| r2| r1| r0] */	\
	__xmm2 = _mm_packus_epi16(xmm_a, xmm_a);			/* [ a7| a6| a5| a4| a3| a2| a1| a0| a7| a6| a5| a4| a3| a2| a1| a0] */	\
	__xmm1 = _mm_unpacklo_epi8(__xmm1, __xmm2);			/* [ a7| r7| a6| r6| a5| r5| a4| r4| a3| r3| a2| r2| a1| r1| a0| r0] */	\
	\
	__xmm2 = _mm_unpacklo_epi16(__xmm0, __xmm1);		/* [ a3| r3| g3| b3| a2| r2| g2| b2| a1| r1| g1| b1| a0| r0| g0| b0] */	\
	__xmm0 = _mm_unpackhi_epi16(__xmm0, __xmm1);		/* [ a7| r7| g7| b7| a6| r6| g6| b6| a5| r5| g5| b5| a4| r4| g4| b4] */	\
	\
	_mm_storeu_si128((__m128i *)(bgra), __xmm2);																					\
	_mm_storeu_si128((__m128i *)((char *)(bgra) + 16), __xmm0);																	\
}

#define	__NVC_BGRA_PACK_INT_8P_8BIT_STREAM(xmm_r, xmm_g, xmm_b, xmm_a, bgra)		\
{																					\
	__m128i __xmm0, __xmm1, __xmm2;													\
																					\
	__xmm0 = _mm_packus_epi16(xmm_b, xmm_b);			/* [ b7| b6| b5| b4| b3| b2| b1| b0| b7| b6| b5| b4| b3| b2| b1| b0] */	\
	__xmm1 = _mm_packus_epi16(xmm_g, xmm_g);			/* [ g7| g6| g5| g4| g3| g2| g1| g0| g7| g6| g5| g4| g3| g2| g1| g0] */	\
	__xmm0 = _mm_unpacklo_epi8(__xmm0, __xmm1);			/* [ g7| b7| g6| b6| g5| b5| g4| b4| g3| b3| g2| b2| g1| b1| g0| b0] */	\
																																\
	__xmm1 = _mm_packus_epi16(xmm_r, xmm_r);			/* [ r7| r6| r5| r4| r3| r2| r1| r0| r7| r6| r5| r4| r3| r2| r1| r0] */	\
	__xmm2 = _mm_packus_epi16(xmm_a, xmm_a);			/* [ a7| a6| a5| a4| a3| a2| a1| a0| a7| a6| a5| a4| a3| a2| a1| a0] */	\
	__xmm1 = _mm_unpacklo_epi8(__xmm1, __xmm2);			/* [ a7| r7| a6| r6| a5| r5| a4| r4| a3| r3| a2| r2| a1| r1| a0| r0] */	\
																																\
	__xmm2 = _mm_unpacklo_epi16(__xmm0, __xmm1);		/* [ a3| r3| g3| b3| a2| r2| g2| b2| a1| r1| g1| b1| a0| r0| g0| b0] */	\
	__xmm0 = _mm_unpackhi_epi16(__xmm0, __xmm1);		/* [ a7| r7| g7| b7| a6| r6| g6| b6| a5| r5| g5| b5| a4| r4| g4| b4] */	\
																																\
	_mm_stream_si128((__m128i *)(bgra), __xmm2);																				\
	_mm_stream_si128((__m128i *)((char *)(bgra) + 16), __xmm0);																	\
}

#define	__NVC_BGRA_PACK_INT_8P_8BIT_NT(xmm_r, xmm_g, xmm_b, xmm_a, bgra)	\
{																			\
	__m128i __xmm0, __xmm1, __xmm2;											\
																			\
	__xmm0 = _mm_packus_epi16(xmm_b, xmm_b);			/* [ b7| b6| b5| b4| b3| b2| b1| b0| b7| b6| b5| b4| b3| b2| b1| b0] */	\
	__xmm1 = _mm_packus_epi16(xmm_g, xmm_g);			/* [ g7| g6| g5| g4| g3| g2| g1| g0| g7| g6| g5| g4| g3| g2| g1| g0] */	\
	__xmm0 = _mm_unpacklo_epi8(__xmm0, __xmm1);			/* [ g7| b7| g6| b6| g5| b5| g4| b4| g3| b3| g2| b2| g1| b1| g0| b0] */	\
																																\
	__xmm1 = _mm_packus_epi16(xmm_r, xmm_r);			/* [ r7| r6| r5| r4| r3| r2| r1| r0| r7| r6| r5| r4| r3| r2| r1| r0] */	\
	__xmm2 = _mm_packus_epi16(xmm_a, xmm_a);			/* [ a7| a6| a5| a4| a3| a2| a1| a0| a7| a6| a5| a4| a3| a2| a1| a0] */	\
	__xmm1 = _mm_unpacklo_epi8(__xmm1, __xmm2);			/* [ a7| r7| a6| r6| a5| r5| a4| r4| a3| r3| a2| r2| a1| r1| a0| r0] */	\
																																\
	__xmm2 = _mm_unpacklo_epi16(__xmm0, __xmm1);		/* [ a3| r3| g3| b3| a2| r2| g2| b2| a1| r1| g1| b1| a0| r0| g0| b0] */	\
	__xmm0 = _mm_unpackhi_epi16(__xmm0, __xmm1);		/* [ a7| r7| g7| b7| a6| r6| g6| b6| a5| r5| g5| b5| a4| r4| g4| b4] */	\
																																\
	_mm_stream_si128((__m128i *)(bgra), __xmm2);																				\
	_mm_stream_si128((__m128i *)((char *)(bgra) + 16), __xmm0);																	\
}


#define	__NVC_BGRA_PACK_INT_16P_8BIT(xmm_r, xmm_g, xmm_b, xmm_a, bgra)	    \
{																			\
       __m128i xmm_bg, xmm_ra, xmm_bgra;                                    \
       xmm_bg = _mm_unpacklo_epi8(xmm_b, xmm_g);                            \
       xmm_ra = _mm_unpacklo_epi8(xmm_r, xmm_a);                            \
       xmm_bgra = _mm_unpacklo_epi16(xmm_bg, xmm_ra);                       \
       _mm_store_si128((__m128i *)(bgra), xmm_bgra);                        \
       xmm_bgra = _mm_unpackhi_epi16(xmm_bg, xmm_ra);                       \
       _mm_store_si128((__m128i *)(bgra + 16), xmm_bgra);                   \
                                                                            \
       xmm_bg = _mm_unpackhi_epi8(xmm_b, xmm_g);                            \
       xmm_ra = _mm_unpackhi_epi8(xmm_r, xmm_a);                            \
       xmm_bgra = _mm_unpacklo_epi16(xmm_bg, xmm_ra);                       \
       _mm_store_si128((__m128i *)(bgra + 32), xmm_bgra);                   \
       xmm_bgra = _mm_unpackhi_epi16(xmm_bg, xmm_ra);                       \
       _mm_store_si128((__m128i *)(bgra + 48), xmm_bgra);                   \
}

#define	__NVC_BGRA_PACK_INT_16P_8BIT_NT(xmm_r, xmm_g, xmm_b, xmm_a, bgra)	\
{																			\
       __m128i xmm_bg, xmm_ra, xmm_bgra;                                    \
       xmm_bg = _mm_unpacklo_epi8(xmm_b, xmm_g);                            \
       xmm_ra = _mm_unpacklo_epi8(xmm_r, xmm_a);                            \
       xmm_bgra = _mm_unpacklo_epi16(xmm_bg, xmm_ra);                       \
       _mm_stream_si128((__m128i *)(bgra), xmm_bgra);                       \
       xmm_bgra = _mm_unpackhi_epi16(xmm_bg, xmm_ra);                       \
       _mm_stream_si128((__m128i *)(bgra + 16), xmm_bgra);                  \
                                                                            \
       xmm_bg = _mm_unpackhi_epi8(xmm_b, xmm_g);                            \
       xmm_ra = _mm_unpackhi_epi8(xmm_r, xmm_a);                            \
       xmm_bgra = _mm_unpacklo_epi16(xmm_bg, xmm_ra);                       \
       _mm_stream_si128((__m128i *)(bgra + 32), xmm_bgra);                  \
       xmm_bgra = _mm_unpackhi_epi16(xmm_bg, xmm_ra);                       \
       _mm_stream_si128((__m128i *)(bgra + 48), xmm_bgra);                  \
}

#define	__NVC_BGRA_PACK_INT_16P_8BIT_UNALIGNED(xmm_r, xmm_g, xmm_b, xmm_a, bgra)	\
{																			\
       __m128i xmm_bg, xmm_ra, xmm_bgra;                                    \
       xmm_bg = _mm_unpacklo_epi8(xmm_b, xmm_g);                            \
       xmm_ra = _mm_unpacklo_epi8(xmm_r, xmm_a);                            \
       xmm_bgra = _mm_unpacklo_epi16(xmm_bg, xmm_ra);                       \
       _mm_storeu_si128((__m128i *)(bgra), xmm_bgra);                       \
       xmm_bgra = _mm_unpackhi_epi16(xmm_bg, xmm_ra);                       \
       _mm_storeu_si128((__m128i *)(bgra + 16), xmm_bgra);                  \
                                                                            \
       xmm_bg = _mm_unpackhi_epi8(xmm_b, xmm_g);                            \
       xmm_ra = _mm_unpackhi_epi8(xmm_r, xmm_a);                            \
       xmm_bgra = _mm_unpacklo_epi16(xmm_bg, xmm_ra);                       \
       _mm_storeu_si128((__m128i *)(bgra + 32), xmm_bgra);                  \
       xmm_bgra = _mm_unpackhi_epi16(xmm_bg, xmm_ra);                       \
       _mm_storeu_si128((__m128i *)(bgra + 48), xmm_bgra);                  \
}

#define	__NVC_BGRA_UNPACK_INT_8P_16BIT(bgra, xmm_r, xmm_g, xmm_b, xmm_a)	\
{																			\
	__m128i __xmm0, __xmm1, __xmm2, __xmm3, __xmm4;							\
																			\
	__xmm0 = _mm_load_si128((__m128i *)(bgra));					/* [   a1  |   r1  |   g1  |   b1  |   a0  |   r0  |   g0  |   b0  ] */	\
	__xmm1 = _mm_load_si128((__m128i *)((char *)(bgra) + 16));	/* [   a3  |   r3  |   g3  |   b3  |   a2  |   r2  |   g2  |   b2  ] */	\
																																		\
	__xmm2 = _mm_unpacklo_epi16(__xmm0, __xmm1);		/* [   a2  |   a0  |   r2  |   r0  |   g2  |   g0  |   b2  |   b0  ] */			\
	__xmm0 = _mm_unpackhi_epi16(__xmm0, __xmm1);		/* [   a3  |   a1  |   r3  |   r1  |   g3  |   g1  |   b3  |   b1  ] */			\
	__xmm1 = _mm_unpacklo_epi16(__xmm2, __xmm0);		/* [   g3  |   g2  |   g1  |   g0  |   b3  |   b2  |   b1  |   b0  ] */			\
	__xmm2 = _mm_unpackhi_epi16(__xmm2, __xmm0);		/* [   a3  |   a2  |   a1  |   a0  |   r3  |   r2  |   r1  |   r0  ] */			\
																																		\
	__xmm3 = _mm_load_si128((__m128i *)((char *)(bgra) + 32));	/* [   a5  |   r5  |   g5  |   b5  |   a4  |   r4  |   g4  |   b4  ] */	\
	__xmm4 = _mm_load_si128((__m128i *)((char *)(bgra) + 48));	/* [   a7  |   r7  |   g7  |   b7  |   a6  |   r6  |   g6  |   b6  ] */	\
																																		\
	__xmm0 = _mm_unpacklo_epi16(__xmm3, __xmm4);		/* [   a6  |   a4  |   r6  |   r4  |   g6  |   g4  |   b6  |   b4  ] */			\
	__xmm3 = _mm_unpackhi_epi16(__xmm3, __xmm4);		/* [   a7  |   a5  |   r7  |   r5  |   g7  |   g5  |   b7  |   b5  ] */			\
	__xmm4 = _mm_unpacklo_epi16(__xmm0, __xmm3);		/* [   g7  |   g6  |   g5  |   g4  |   b7  |   b6  |   b5  |   b4  ] */			\
	__xmm0 = _mm_unpackhi_epi16(__xmm0, __xmm3);		/* [   a7  |   a6  |   a5  |   a4  |   r7  |   r6  |   r5  |   r4  ] */			\
																																		\
	xmm_b = _mm_unpacklo_epi64(__xmm1, __xmm4);			/* [   b7  |   b6  |   b5  |   b4  |   b3  |   b2  |   b1  |   b0  ] */			\
	xmm_g = _mm_unpackhi_epi64(__xmm1, __xmm4);			/* [   g7  |   g6  |   g5  |   g4  |   g3  |   g2  |   g1  |   g0  ] */			\
	xmm_r = _mm_unpacklo_epi64(__xmm2, __xmm0);			/* [   r7  |   r6  |   r5  |   r4  |   r3  |   r2  |   r1  |   r0  ] */			\
	xmm_a = _mm_unpackhi_epi64(__xmm2, __xmm0);			/* [   a7  |   a6  |   a5  |   a4  |   a3  |   a2  |   a1  |   a0  ] */			\
}

#define	__NVC_BGRA_UNPACK_INT_8P_16BIT_UNALIGNED(bgra, xmm_r, xmm_g, xmm_b, xmm_a)	\
{																					\
	__m128i __xmm0, __xmm1, __xmm2, __xmm3, __xmm4;									\
	\
	__xmm0 = _mm_loadu_si128((__m128i *)(bgra));				/* [   a1  |   r1  |   g1  |   b1  |   a0  |   r0  |   g0  |   b0  ] */	\
	__xmm1 = _mm_loadu_si128((__m128i *)((char *)(bgra) + 16));	/* [   a3  |   r3  |   g3  |   b3  |   a2  |   r2  |   g2  |   b2  ] */	\
	\
	__xmm2 = _mm_unpacklo_epi16(__xmm0, __xmm1);		/* [   a2  |   a0  |   r2  |   r0  |   g2  |   g0  |   b2  |   b0  ] */			\
	__xmm0 = _mm_unpackhi_epi16(__xmm0, __xmm1);		/* [   a3  |   a1  |   r3  |   r1  |   g3  |   g1  |   b3  |   b1  ] */			\
	__xmm1 = _mm_unpacklo_epi16(__xmm2, __xmm0);		/* [   g3  |   g2  |   g1  |   g0  |   b3  |   b2  |   b1  |   b0  ] */			\
	__xmm2 = _mm_unpackhi_epi16(__xmm2, __xmm0);		/* [   a3  |   a2  |   a1  |   a0  |   r3  |   r2  |   r1  |   r0  ] */			\
	\
	__xmm3 = _mm_loadu_si128((__m128i *)((char *)(bgra) + 32));	/* [   a5  |   r5  |   g5  |   b5  |   a4  |   r4  |   g4  |   b4  ] */	\
	__xmm4 = _mm_loadu_si128((__m128i *)((char *)(bgra) + 48));	/* [   a7  |   r7  |   g7  |   b7  |   a6  |   r6  |   g6  |   b6  ] */	\
	\
	__xmm0 = _mm_unpacklo_epi16(__xmm3, __xmm4);		/* [   a6  |   a4  |   r6  |   r4  |   g6  |   g4  |   b6  |   b4  ] */			\
	__xmm3 = _mm_unpackhi_epi16(__xmm3, __xmm4);		/* [   a7  |   a5  |   r7  |   r5  |   g7  |   g5  |   b7  |   b5  ] */			\
	__xmm4 = _mm_unpacklo_epi16(__xmm0, __xmm3);		/* [   g7  |   g6  |   g5  |   g4  |   b7  |   b6  |   b5  |   b4  ] */			\
	__xmm0 = _mm_unpackhi_epi16(__xmm0, __xmm3);		/* [   a7  |   a6  |   a5  |   a4  |   r7  |   r6  |   r5  |   r4  ] */			\
	\
	xmm_b = _mm_unpacklo_epi64(__xmm1, __xmm4);			/* [   b7  |   b6  |   b5  |   b4  |   b3  |   b2  |   b1  |   b0  ] */			\
	xmm_g = _mm_unpackhi_epi64(__xmm1, __xmm4);			/* [   g7  |   g6  |   g5  |   g4  |   g3  |   g2  |   g1  |   g0  ] */			\
	xmm_r = _mm_unpacklo_epi64(__xmm2, __xmm0);			/* [   r7  |   r6  |   r5  |   r4  |   r3  |   r2  |   r1  |   r0  ] */			\
	xmm_a = _mm_unpackhi_epi64(__xmm2, __xmm0);			/* [   a7  |   a6  |   a5  |   a4  |   a3  |   a2  |   a1  |   a0  ] */			\
}

#define	__NVC_BGRA_PACK_INT_8P_16BIT(xmm_r, xmm_g, xmm_b, xmm_a, bgra)		\
{																			\
	__m128i __xmm0, __xmm1, __xmm2;											\
																			\
	__xmm0 = _mm_unpacklo_epi16(xmm_b, xmm_g);			/* [   g3  |   b3  |   g2  |   b2  |   g1  |   b1  |   g0  |   b0  ] */	\
	__xmm1 = _mm_unpacklo_epi16(xmm_r, xmm_a);			/* [   a3  |   r3  |   a2  |   r2  |   a1  |   r1  |   a0  |   r0  ] */	\
	__xmm2 = _mm_unpacklo_epi32(__xmm0, __xmm1);		/* [   a1  |   r1  |   g1  |   b1  |   a0  |   r0  |   g0  |   b0  ] */	\
	__xmm0 = _mm_unpackhi_epi32(__xmm0, __xmm1);		/* [   a3  |   r3  |   g3  |   b3  |   a2  |   r2  |   g2  |   b2  ] */	\
																																\
	_mm_store_si128((__m128i *)(bgra), __xmm2);																					\
	_mm_store_si128((__m128i *)((char *)(bgra) + 16), __xmm0);																	\
																																\
	__xmm0 = _mm_unpackhi_epi16(xmm_b, xmm_g);			/* [   g7  |   b7  |   g6  |   b6  |   g5  |   b5  |   g4  |   b4  ] */	\
	__xmm1 = _mm_unpackhi_epi16(xmm_r, xmm_a);			/* [   a7  |   r7  |   a6  |   r6  |   a5  |   r5  |   a4  |   r4  ] */	\
	__xmm2 = _mm_unpacklo_epi32(__xmm0, __xmm1);		/* [   a5  |   r5  |   g5  |   b5  |   a4  |   r4  |   g4  |   b4  ] */	\
	__xmm0 = _mm_unpackhi_epi32(__xmm0, __xmm1);		/* [   a7  |   r7  |   g7  |   b7  |   a6  |   r6  |   g6  |   b6  ] */	\
																																\
	_mm_store_si128((__m128i *)((char *)(bgra) + 32), __xmm2);																	\
	_mm_store_si128((__m128i *)((char *)(bgra) + 48), __xmm0);																	\
}

#define	__NVC_BGRA_PACK_INT_8P_16BIT_NT(xmm_r, xmm_g, xmm_b, xmm_a, bgra)	\
{																			\
	__m128i __xmm0, __xmm1, __xmm2;											\
																			\
	__xmm0 = _mm_unpacklo_epi16(xmm_b, xmm_g);			/* [   g3  |   b3  |   g2  |   b2  |   g1  |   b1  |   g0  |   b0  ] */	\
	__xmm1 = _mm_unpacklo_epi16(xmm_r, xmm_a);			/* [   a3  |   r3  |   a2  |   r2  |   a1  |   r1  |   a0  |   r0  ] */	\
	__xmm2 = _mm_unpacklo_epi32(__xmm0, __xmm1);		/* [   a1  |   r1  |   g1  |   b1  |   a0  |   r0  |   g0  |   b0  ] */	\
	__xmm0 = _mm_unpackhi_epi32(__xmm0, __xmm1);		/* [   a3  |   r3  |   g3  |   b3  |   a2  |   r2  |   g2  |   b2  ] */	\
																																\
	_mm_stream_si128((__m128i *)(bgra), __xmm2);																				\
	_mm_stream_si128((__m128i *)((char *)(bgra) + 16), __xmm0);																	\
																																\
	__xmm0 = _mm_unpackhi_epi16(xmm_b, xmm_g);			/* [   g7  |   b7  |   g6  |   b6  |   g5  |   b5  |   g4  |   b4  ] */	\
	__xmm1 = _mm_unpackhi_epi16(xmm_r, xmm_a);			/* [   a7  |   r7  |   a6  |   r6  |   a5  |   r5  |   a4  |   r4  ] */	\
	__xmm2 = _mm_unpacklo_epi32(__xmm0, __xmm1);		/* [   a5  |   r5  |   g5  |   b5  |   a4  |   r4  |   g4  |   b4  ] */	\
	__xmm0 = _mm_unpackhi_epi32(__xmm0, __xmm1);		/* [   a7  |   r7  |   g7  |   b7  |   a6  |   r6  |   g6  |   b6  ] */	\
																																\
	_mm_stream_si128((__m128i *)((char *)(bgra) + 32), __xmm2);																	\
	_mm_stream_si128((__m128i *)((char *)(bgra) + 48), __xmm0);																	\
}

#define	__NVC_BGRA_PACK_INT_8P_16BIT_UNALIGNED(xmm_r, xmm_g, xmm_b, xmm_a, bgra)		\
{																						\
	__m128i __xmm0, __xmm1, __xmm2;														\
																						\
	__xmm0 = _mm_unpacklo_epi16(xmm_b, xmm_g);			/* [   g3  |   b3  |   g2  |   b2  |   g1  |   b1  |   g0  |   b0  ] */	\
	__xmm1 = _mm_unpacklo_epi16(xmm_r, xmm_a);			/* [   a3  |   r3  |   a2  |   r2  |   a1  |   r1  |   a0  |   r0  ] */	\
	__xmm2 = _mm_unpacklo_epi32(__xmm0, __xmm1);		/* [   a1  |   r1  |   g1  |   b1  |   a0  |   r0  |   g0  |   b0  ] */	\
	__xmm0 = _mm_unpackhi_epi32(__xmm0, __xmm1);		/* [   a3  |   r3  |   g3  |   b3  |   a2  |   r2  |   g2  |   b2  ] */	\
																																\
	_mm_storeu_si128((__m128i *)(bgra), __xmm2);																					\
	_mm_storeu_si128((__m128i *)((char *)(bgra) + 16), __xmm0);																	\
																																\
	__xmm0 = _mm_unpackhi_epi16(xmm_b, xmm_g);			/* [   g7  |   b7  |   g6  |   b6  |   g5  |   b5  |   g4  |   b4  ] */	\
	__xmm1 = _mm_unpackhi_epi16(xmm_r, xmm_a);			/* [   a7  |   r7  |   a6  |   r6  |   a5  |   r5  |   a4  |   r4  ] */	\
	__xmm2 = _mm_unpacklo_epi32(__xmm0, __xmm1);		/* [   a5  |   r5  |   g5  |   b5  |   a4  |   r4  |   g4  |   b4  ] */	\
	__xmm0 = _mm_unpackhi_epi32(__xmm0, __xmm1);		/* [   a7  |   r7  |   g7  |   b7  |   a6  |   r6  |   g6  |   b6  ] */	\
																																\
	_mm_storeu_si128((__m128i *)((char *)(bgra) + 32), __xmm2);																	\
	_mm_storeu_si128((__m128i *)((char *)(bgra) + 48), __xmm0);																	\
}


static NVC_FORCE_INLINE uint8_t __Nvc_Cacl_8Bit_Luma_From_8Bit_RGB(uint8_t r, uint8_t g, uint8_t b, int nColorimetry)
{
	uint32_t luma;

	luma = (((uint32_t)r << 8) * __COEF_RED_LUMA(nColorimetry)) >> 16;
	luma += (((uint32_t)g << 8) * __COEF_GREEN_LUMA(nColorimetry)) >> 16;
	luma += (((uint32_t)b << 8) * __COEF_BLUE_LUMA(nColorimetry)) >> 16;

	return (uint8_t)((luma + 128) >> 8);
}

static NVC_FORCE_INLINE __m128i __NvcCalc_8Bit_Luma_From_8Bit_BGRA_8Pix(const void *pBGRA, int nColorimetry)
{
	__m128i __xmm0, __xmm1, __xmm2, __xmm_zero = _mm_setzero_si128();

	__xmm0 = _mm_load_si128((__m128i *)pBGRA);		/* [ a3| r3| g3| b3| a2| r2| g2| b2| a1| r1| g1| b1| a0| r0| g0| b0] */
	__xmm1 = _mm_load_si128((__m128i *)pBGRA + 1);	/* [ a7| r7| g7| b7| a6| r6| g6| b6| a5| r5| g5| b5| a4| r4| g4| b4] */

	__xmm2 = _mm_unpacklo_epi8(__xmm0, __xmm1);		/* [ a5| a1| r5| r1| g5| g1| b5| b1| a4| a0| r4| r0| g4| g0| b4| b0] */	
	__xmm0 = _mm_unpackhi_epi8(__xmm0, __xmm1);		/* [ a7| a3| r7| r3| g7| g3| b7| b3| a6| a2| r6| r2| g6| g2| b6| b2] */	
	__xmm1 = _mm_unpacklo_epi8(__xmm2, __xmm0);		/* [ a6| a4| a2| a0| r6| r4| r2| r0| g6| g4| g2| g0| b6| b4| b2| b0] */	
	__xmm2 = _mm_unpackhi_epi8(__xmm2, __xmm0);		/* [ a7| a5| a3| a1| r7| r5| r3| r1| g7| g5| g3| g1| b7| b5| b3| b1] */	
	__xmm0 = _mm_unpacklo_epi8(__xmm1, __xmm2);		/* [ g7| g6| g5| g4| g3| g2| g1| g0| b7| b6| b5| b4| b3| b2| b1| b0] */	
	__xmm1 = _mm_unpackhi_epi8(__xmm1, __xmm2);		/* [ a7| a6| a5| a4| a3| a2| a1| a0| r7| r6| r5| r4| r3| r2| r1| r0] */

	__xmm2 = _mm_unpacklo_epi8(__xmm0, __xmm_zero);	/* [   b7  |   b6  |   b5  |   b4  |   b3  |   b2  |   b1  |   b0  ] */
	__xmm2 = _mm_slli_epi16(__xmm2, 8);

	__xmm0 = _mm_unpackhi_epi8(__xmm0, __xmm_zero);	/* [   g7  |   g6  |   g5  |   g4  |   g3  |   g2  |   g1  |   g0  ] */	
	__xmm0 = _mm_slli_epi16(__xmm0, 8);

	__xmm1 = _mm_unpacklo_epi8(__xmm1, __xmm_zero);	/* [   r7  |   r6  |   r5  |   r4  |   r3  |   r2  |   r1  |   r0  ] */
	__xmm1 = _mm_slli_epi16(__xmm1, 8);

	if (nColorimetry == NVC_BT709) {
		__xmm2 = _mm_mulhi_epu16(__xmm2, g_xmm_luma_blue_coef_709);		/* blue * coef_blue_luma */
		__xmm0 = _mm_mulhi_epu16(__xmm0, g_xmm_luma_green_coef_709);	/* green * coef_green_luma */
		__xmm1 = _mm_mulhi_epu16(__xmm1, g_xmm_luma_red_coef_709);		/* red * coef_red_luma */
	} else if (nColorimetry == NVC_BT601) {
		__xmm2 = _mm_mulhi_epu16(__xmm2, g_xmm_luma_blue_coef_601);		/* blue * coef_blue_luma */
		__xmm0 = _mm_mulhi_epu16(__xmm0, g_xmm_luma_green_coef_601);	/* green * coef_green_luma */
		__xmm1 = _mm_mulhi_epu16(__xmm1, g_xmm_luma_red_coef_601);		/* red * coef_red_luma */
	} else if (nColorimetry == NVC_BT2020) 	{
		__xmm2 = _mm_mulhi_epu16(__xmm2, g_xmm_luma_blue_coef_2020);	/* blue * coef_blue_luma */
		__xmm0 = _mm_mulhi_epu16(__xmm0, g_xmm_luma_green_coef_2020);	/* green * coef_green_luma */
		__xmm1 = _mm_mulhi_epu16(__xmm1, g_xmm_luma_red_coef_2020);		/* red * coef_red_luma */
	}

	__xmm0 = _mm_add_epi16(__xmm0, __xmm1);
	__xmm0 = _mm_add_epi16(__xmm0, __xmm2);
	__xmm0 = _mm_add_epi16(__xmm0, g_xmm_16bit_128);	/* add rounding */
	return _mm_srli_epi16(__xmm0, 8);					/* [   l7  |   l6  |   l5  |   l4  |   l3  |   l2  |   l1  |   l0  ] */
}

static NVC_FORCE_INLINE uint16_t __Nvc_Cacl_16Bit_Luma_From_16Bit_RGB(uint16_t r, uint16_t g, uint16_t b, int nColorimetry)
{
	uint32_t luma;

	luma = (uint32_t)r * __COEF_RED_LUMA(nColorimetry);
	luma += (uint32_t)g * __COEF_GREEN_LUMA(nColorimetry);
	luma += (uint32_t)b * __COEF_BLUE_LUMA(nColorimetry);

	return (uint16_t)((luma + 32768) >> 16);
}

static NVC_FORCE_INLINE __m128i __NvcCalc_16Bit_Luma_From_16Bit_BGRA_8Pix(const void *pBGRA, int nColorimetry)
{
	__m128i __xmm0, __xmm1, __xmm2, __xmm3, __xmm4, __xmm_zero = _mm_setzero_si128();
	__m128 __xmm_red_ps, __xmm_green_ps, __xmm_blue_ps, __xmm_luma_ps;

	__xmm0 = _mm_load_si128((__m128i *)pBGRA);		/* [   a1  |   r1  |   g1  |   b1  |   a0  |   r0  |   g0  |   b0  ] */
	__xmm1 = _mm_load_si128((__m128i *)pBGRA + 1);	/* [   a3  |   r3  |   g3  |   b3  |   a2  |   r2  |   g2  |   b2  ] */

	__xmm2 = _mm_unpacklo_epi16(__xmm0, __xmm1);	/* [   a2  |   a0  |   r2  |   r0  |   g2  |   g0  |   b2  |   b0  ] */
	__xmm0 = _mm_unpackhi_epi16(__xmm0, __xmm1);	/* [   a3  |   a1  |   r3  |   r1  |   g3  |   g1  |   b3  |   b1  ] */
	__xmm1 = _mm_unpacklo_epi16(__xmm2, __xmm0);	/* [   g3  |   g2  |   g1  |   g0  |   b3  |   b2  |   b1  |   b0  ] */
	__xmm2 = _mm_unpackhi_epi16(__xmm2, __xmm0);	/* [   a3  |   a2  |   a1  |   a0  |   r3  |   r2  |   r1  |   r0  ] */

	__xmm3 = _mm_load_si128((__m128i *)pBGRA + 2);	/* [   a5  |   r5  |   g5  |   b5  |   a4  |   r4  |   g4  |   b4  ] */
	__xmm4 = _mm_load_si128((__m128i *)pBGRA + 3);	/* [   a7  |   r7  |   g7  |   b7  |   a6  |   r6  |   g6  |   b6  ] */

	__xmm0 = _mm_unpacklo_epi16(__xmm3, __xmm4);	/* [   a6  |   a4  |   r6  |   r4  |   g6  |   g4  |   b6  |   b4  ] */
	__xmm3 = _mm_unpackhi_epi16(__xmm3, __xmm4);	/* [   a7  |   a5  |   r7  |   r5  |   g7  |   g5  |   b7  |   b5  ] */
	__xmm4 = _mm_unpacklo_epi16(__xmm0, __xmm3);	/* [   g7  |   g6  |   g5  |   g4  |   b7  |   b6  |   b5  |   b4  ] */
	__xmm0 = _mm_unpackhi_epi16(__xmm0, __xmm3);	/* [   a7  |   a6  |   a5  |   a4  |   r7  |   r6  |   r5  |   r4  ] */

	__xmm3 = _mm_unpacklo_epi64(__xmm1, __xmm4);	/* [   b7  |   b6  |   b5  |   b4  |   b3  |   b2  |   b1  |   b0  ] */
	__xmm1 = _mm_unpackhi_epi64(__xmm1, __xmm4);	/* [   g7  |   g6  |   g5  |   g4  |   g3  |   g2  |   g1  |   g0  ] */
	__xmm2 = _mm_unpacklo_epi64(__xmm2, __xmm0);	/* [   r7  |   r6  |   r5  |   r4  |   r3  |   r2  |   r1  |   r0  ] */

	__xmm_blue_ps = _mm_cvtepi32_ps(_mm_unpacklo_epi16(__xmm3, __xmm_zero));	/* [       b3      |       b2      |       b1      |       b0      ] */
	__xmm_green_ps = _mm_cvtepi32_ps(_mm_unpacklo_epi16(__xmm1, __xmm_zero));	/* [       g3      |       g2      |       g1      |       g0      ] */
	__xmm_red_ps = _mm_cvtepi32_ps(_mm_unpacklo_epi16(__xmm2, __xmm_zero));		/* [       r3      |       r2      |       r1      |       r0      ] */

	if (nColorimetry == NVC_BT709) {
		__xmm_blue_ps = _mm_mul_ps(__xmm_blue_ps, g_xmm_luma_blue_coef_709_ps);		/* blue * coef_blue_luma */
		__xmm_green_ps = _mm_mul_ps(__xmm_green_ps, g_xmm_luma_green_coef_709_ps);	/* green * coef_green_luma */
		__xmm_red_ps = _mm_mul_ps(__xmm_red_ps, g_xmm_luma_red_coef_709_ps);		/* red * coef_red_luma */
	} else if (nColorimetry == NVC_BT601) {
		__xmm_blue_ps = _mm_mul_ps(__xmm_blue_ps, g_xmm_luma_blue_coef_601_ps);		/* blue * coef_blue_luma */
		__xmm_green_ps = _mm_mul_ps(__xmm_green_ps, g_xmm_luma_green_coef_601_ps);	/* green * coef_green_luma */
		__xmm_red_ps = _mm_mul_ps(__xmm_red_ps, g_xmm_luma_red_coef_601_ps);		/* red * coef_red_luma */
	} else if (nColorimetry == NVC_BT2020) {
		__xmm_blue_ps = _mm_mul_ps(__xmm_blue_ps, g_xmm_luma_blue_coef_2020_ps);	/* blue * coef_blue_luma */
		__xmm_green_ps = _mm_mul_ps(__xmm_green_ps, g_xmm_luma_green_coef_2020_ps);	/* green * coef_green_luma */
		__xmm_red_ps = _mm_mul_ps(__xmm_red_ps, g_xmm_luma_red_coef_2020_ps);		/* red * coef_red_luma */
	}

	__xmm_luma_ps = _mm_add_ps(__xmm_blue_ps, __xmm_green_ps);
	__xmm_luma_ps = _mm_add_ps(__xmm_luma_ps, __xmm_red_ps);
	__xmm0 = _mm_cvtps_epi32(__xmm_luma_ps);

	__xmm_blue_ps = _mm_cvtepi32_ps(_mm_unpackhi_epi16(__xmm3, __xmm_zero));	/* [       b7      |       b6      |       b5      |       b4      ] */
	__xmm_green_ps = _mm_cvtepi32_ps(_mm_unpackhi_epi16(__xmm1, __xmm_zero));	/* [       g7      |       g6      |       g5      |       g4      ] */
	__xmm_red_ps = _mm_cvtepi32_ps(_mm_unpackhi_epi16(__xmm2, __xmm_zero));		/* [       r7      |       r6      |       r5      |       r4      ] */

	if (nColorimetry == NVC_BT709) {
		__xmm_blue_ps = _mm_mul_ps(__xmm_blue_ps, g_xmm_luma_blue_coef_709_ps);		/* blue * coef_blue_luma */
		__xmm_green_ps = _mm_mul_ps(__xmm_green_ps, g_xmm_luma_green_coef_709_ps);	/* green * coef_green_luma */
		__xmm_red_ps = _mm_mul_ps(__xmm_red_ps, g_xmm_luma_red_coef_709_ps);		/* red * coef_red_luma */
	} else if (nColorimetry == NVC_BT601) {
		__xmm_blue_ps = _mm_mul_ps(__xmm_blue_ps, g_xmm_luma_blue_coef_601_ps);		/* blue * coef_blue_luma */
		__xmm_green_ps = _mm_mul_ps(__xmm_green_ps, g_xmm_luma_green_coef_601_ps);	/* green * coef_green_luma */
		__xmm_red_ps = _mm_mul_ps(__xmm_red_ps, g_xmm_luma_red_coef_601_ps);		/* red * coef_red_luma */
	} else if (nColorimetry == NVC_BT2020) {
		__xmm_blue_ps = _mm_mul_ps(__xmm_blue_ps, g_xmm_luma_blue_coef_2020_ps);	/* blue * coef_blue_luma */
		__xmm_green_ps = _mm_mul_ps(__xmm_green_ps, g_xmm_luma_green_coef_2020_ps);	/* green * coef_green_luma */
		__xmm_red_ps = _mm_mul_ps(__xmm_red_ps, g_xmm_luma_red_coef_2020_ps);		/* red * coef_red_luma */
	}

	__xmm_luma_ps = _mm_add_ps(__xmm_blue_ps, __xmm_green_ps);
	__xmm_luma_ps = _mm_add_ps(__xmm_luma_ps, __xmm_red_ps);
	__xmm3 = _mm_cvtps_epi32(__xmm_luma_ps);

	__xmm0 = _mm_sub_epi32(__xmm0, g_xmm_32bit_32768);
	__xmm3 = _mm_sub_epi32(__xmm3, g_xmm_32bit_32768);
	__xmm0 = _mm_packs_epi32(__xmm0, __xmm3);
	return _mm_add_epi16(__xmm0, g_xmm_16bit_32768);
}

static NVC_FORCE_INLINE __m128i __NvcCalc_12Bit_Luma_From_16Bit_BGRA_8Pix(const void *pBGRA, int nColorimetry)
{
	__m128i __xmm0, __xmm1, __xmm2, __xmm3, __xmm4, __xmm_zero = _mm_setzero_si128();
	__m128 __xmm_red_ps, __xmm_green_ps, __xmm_blue_ps, __xmm_luma_ps;

	__xmm0 = _mm_load_si128((__m128i *)pBGRA);		/* [   a1  |   r1  |   g1  |   b1  |   a0  |   r0  |   g0  |   b0  ] */
	__xmm1 = _mm_load_si128((__m128i *)pBGRA + 1);	/* [   a3  |   r3  |   g3  |   b3  |   a2  |   r2  |   g2  |   b2  ] */

	__xmm2 = _mm_unpacklo_epi16(__xmm0, __xmm1);	/* [   a2  |   a0  |   r2  |   r0  |   g2  |   g0  |   b2  |   b0  ] */
	__xmm0 = _mm_unpackhi_epi16(__xmm0, __xmm1);	/* [   a3  |   a1  |   r3  |   r1  |   g3  |   g1  |   b3  |   b1  ] */
	__xmm1 = _mm_unpacklo_epi16(__xmm2, __xmm0);	/* [   g3  |   g2  |   g1  |   g0  |   b3  |   b2  |   b1  |   b0  ] */
	__xmm2 = _mm_unpackhi_epi16(__xmm2, __xmm0);	/* [   a3  |   a2  |   a1  |   a0  |   r3  |   r2  |   r1  |   r0  ] */

	__xmm3 = _mm_load_si128((__m128i *)pBGRA + 2);	/* [   a5  |   r5  |   g5  |   b5  |   a4  |   r4  |   g4  |   b4  ] */
	__xmm4 = _mm_load_si128((__m128i *)pBGRA + 3);	/* [   a7  |   r7  |   g7  |   b7  |   a6  |   r6  |   g6  |   b6  ] */

	__xmm0 = _mm_unpacklo_epi16(__xmm3, __xmm4);	/* [   a6  |   a4  |   r6  |   r4  |   g6  |   g4  |   b6  |   b4  ] */
	__xmm3 = _mm_unpackhi_epi16(__xmm3, __xmm4);	/* [   a7  |   a5  |   r7  |   r5  |   g7  |   g5  |   b7  |   b5  ] */
	__xmm4 = _mm_unpacklo_epi16(__xmm0, __xmm3);	/* [   g7  |   g6  |   g5  |   g4  |   b7  |   b6  |   b5  |   b4  ] */
	__xmm0 = _mm_unpackhi_epi16(__xmm0, __xmm3);	/* [   a7  |   a6  |   a5  |   a4  |   r7  |   r6  |   r5  |   r4  ] */

	__xmm3 = _mm_unpacklo_epi64(__xmm1, __xmm4);	/* [   b7  |   b6  |   b5  |   b4  |   b3  |   b2  |   b1  |   b0  ] */
	__xmm1 = _mm_unpackhi_epi64(__xmm1, __xmm4);	/* [   g7  |   g6  |   g5  |   g4  |   g3  |   g2  |   g1  |   g0  ] */
	__xmm2 = _mm_unpacklo_epi64(__xmm2, __xmm0);	/* [   r7  |   r6  |   r5  |   r4  |   r3  |   r2  |   r1  |   r0  ] */

	__xmm_blue_ps = _mm_cvtepi32_ps(_mm_unpacklo_epi16(__xmm3, __xmm_zero));	/* [       b3      |       b2      |       b1      |       b0      ] */
	__xmm_green_ps = _mm_cvtepi32_ps(_mm_unpacklo_epi16(__xmm1, __xmm_zero));	/* [       g3      |       g2      |       g1      |       g0      ] */
	__xmm_red_ps = _mm_cvtepi32_ps(_mm_unpacklo_epi16(__xmm2, __xmm_zero));		/* [       r3      |       r2      |       r1      |       r0      ] */

	if (nColorimetry == NVC_BT709) {
		__xmm_blue_ps = _mm_mul_ps(__xmm_blue_ps, g_xmm_luma_blue_coef_709_ps);		/* blue * coef_blue_luma */
		__xmm_green_ps = _mm_mul_ps(__xmm_green_ps, g_xmm_luma_green_coef_709_ps);	/* green * coef_green_luma */
		__xmm_red_ps = _mm_mul_ps(__xmm_red_ps, g_xmm_luma_red_coef_709_ps);		/* red * coef_red_luma */
	} else if (nColorimetry == NVC_BT601)
	{
		__xmm_blue_ps = _mm_mul_ps(__xmm_blue_ps, g_xmm_luma_blue_coef_601_ps);		/* blue * coef_blue_luma */
		__xmm_green_ps = _mm_mul_ps(__xmm_green_ps, g_xmm_luma_green_coef_601_ps);	/* green * coef_green_luma */
		__xmm_red_ps = _mm_mul_ps(__xmm_red_ps, g_xmm_luma_red_coef_601_ps);		/* red * coef_red_luma */
	} else if (nColorimetry == NVC_BT2020)
	{
		__xmm_blue_ps = _mm_mul_ps(__xmm_blue_ps, g_xmm_luma_blue_coef_2020_ps);		/* blue * coef_blue_luma */
		__xmm_green_ps = _mm_mul_ps(__xmm_green_ps, g_xmm_luma_green_coef_2020_ps);	/* green * coef_green_luma */
		__xmm_red_ps = _mm_mul_ps(__xmm_red_ps, g_xmm_luma_red_coef_2020_ps);		/* red * coef_red_luma */
	}

	__xmm_luma_ps = _mm_add_ps(__xmm_blue_ps, __xmm_green_ps);
	__xmm_luma_ps = _mm_add_ps(__xmm_luma_ps, __xmm_red_ps);
	__xmm_luma_ps = _mm_mul_ps(__xmm_luma_ps, g_xmm_scale_65535_4095);
	__xmm0 = _mm_cvtps_epi32(__xmm_luma_ps);

	__xmm_blue_ps = _mm_cvtepi32_ps(_mm_unpackhi_epi16(__xmm3, __xmm_zero));	/* [       b7      |       b6      |       b5      |       b4      ] */
	__xmm_green_ps = _mm_cvtepi32_ps(_mm_unpackhi_epi16(__xmm1, __xmm_zero));	/* [       g7      |       g6      |       g5      |       g4      ] */
	__xmm_red_ps = _mm_cvtepi32_ps(_mm_unpackhi_epi16(__xmm2, __xmm_zero));		/* [       r7      |       r6      |       r5      |       r4      ] */

	if (nColorimetry == NVC_BT709) {
		__xmm_blue_ps = _mm_mul_ps(__xmm_blue_ps, g_xmm_luma_blue_coef_709_ps);		/* blue * coef_blue_luma */
		__xmm_green_ps = _mm_mul_ps(__xmm_green_ps, g_xmm_luma_green_coef_709_ps);	/* green * coef_green_luma */
		__xmm_red_ps = _mm_mul_ps(__xmm_red_ps, g_xmm_luma_red_coef_709_ps);		/* red * coef_red_luma */
	} else if (nColorimetry == NVC_BT601) {
		__xmm_blue_ps = _mm_mul_ps(__xmm_blue_ps, g_xmm_luma_blue_coef_601_ps);		/* blue * coef_blue_luma */
		__xmm_green_ps = _mm_mul_ps(__xmm_green_ps, g_xmm_luma_green_coef_601_ps);	/* green * coef_green_luma */
		__xmm_red_ps = _mm_mul_ps(__xmm_red_ps, g_xmm_luma_red_coef_601_ps);		/* red * coef_red_luma */
	} else if (nColorimetry == NVC_BT2020) {
		__xmm_blue_ps = _mm_mul_ps(__xmm_blue_ps, g_xmm_luma_blue_coef_2020_ps);		/* blue * coef_blue_luma */
		__xmm_green_ps = _mm_mul_ps(__xmm_green_ps, g_xmm_luma_green_coef_2020_ps);	/* green * coef_green_luma */
		__xmm_red_ps = _mm_mul_ps(__xmm_red_ps, g_xmm_luma_red_coef_2020_ps);		/* red * coef_red_luma */
	}

	__xmm_luma_ps = _mm_add_ps(__xmm_blue_ps, __xmm_green_ps);
	__xmm_luma_ps = _mm_add_ps(__xmm_luma_ps, __xmm_red_ps);
	__xmm_luma_ps = _mm_mul_ps(__xmm_luma_ps, g_xmm_scale_65535_4095);
	__xmm3 = _mm_cvtps_epi32(__xmm_luma_ps);

	__xmm0 = _mm_sub_epi32(__xmm0, g_xmm_32bit_32768);
	__xmm3 = _mm_sub_epi32(__xmm3, g_xmm_32bit_32768);
	__xmm0 = _mm_packs_epi32(__xmm0, __xmm3);
	return _mm_add_epi16(__xmm0, g_xmm_16bit_32768);
}

static NVC_FORCE_INLINE float __Nvc_Cacl_32Bit_Luma_From_16Bit_RGB(uint16_t r, uint16_t g, uint16_t b, int nColorimetry)
{
	float luma;

	luma = (float)r * (float)__NVC_R_Y(nColorimetry);
	luma += (float)g * (float)__NVC_G_Y(nColorimetry);
	luma += (float)b * (float)__NVC_B_Y(nColorimetry);

	return luma;
}

static NVC_FORCE_INLINE __m128 __NvcCalc_32Bit_Luma_From_16Bit_BGRA_4Pix(const void *pBGRA, int nColorimetry)
{
	__m128 __xmm_red_ps, __xmm_green_ps, __xmm_blue_ps, __xmm_alpha_ps, __xmm_luma_ps;

	__NVC_BGRA_UNPACK_FLOAT_4P_16BIT(pBGRA, __xmm_red_ps, __xmm_green_ps, __xmm_blue_ps, __xmm_alpha_ps);

	if (nColorimetry == NVC_BT709) {
		__xmm_blue_ps = _mm_mul_ps(__xmm_blue_ps, g_xmm_luma_blue_coef_709_ps);		/* blue * coef_blue_luma */
		__xmm_green_ps = _mm_mul_ps(__xmm_green_ps, g_xmm_luma_green_coef_709_ps);	/* green * coef_green_luma */
		__xmm_red_ps = _mm_mul_ps(__xmm_red_ps, g_xmm_luma_red_coef_709_ps);		/* red * coef_red_luma */
	} else if (nColorimetry == NVC_BT601)
	{
		__xmm_blue_ps = _mm_mul_ps(__xmm_blue_ps, g_xmm_luma_blue_coef_601_ps);		/* blue * coef_blue_luma */
		__xmm_green_ps = _mm_mul_ps(__xmm_green_ps, g_xmm_luma_green_coef_601_ps);	/* green * coef_green_luma */
		__xmm_red_ps = _mm_mul_ps(__xmm_red_ps, g_xmm_luma_red_coef_601_ps);		/* red * coef_red_luma */
	} else if (nColorimetry == NVC_BT2020)
	{
		__xmm_blue_ps = _mm_mul_ps(__xmm_blue_ps, g_xmm_luma_blue_coef_2020_ps);		/* blue * coef_blue_luma */
		__xmm_green_ps = _mm_mul_ps(__xmm_green_ps, g_xmm_luma_green_coef_2020_ps);	/* green * coef_green_luma */
		__xmm_red_ps = _mm_mul_ps(__xmm_red_ps, g_xmm_luma_red_coef_2020_ps);		/* red * coef_red_luma */
	}

	__xmm_luma_ps = _mm_add_ps(__xmm_blue_ps, __xmm_green_ps);
	return _mm_add_ps(__xmm_luma_ps, __xmm_red_ps);
}

static NVC_FORCE_INLINE __m128 __NvcCalc_32Bit_Luma_From_32Bit_BGRA_4Pix(const void *pBGRA, int nColorimetry)
{
    __m128 xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps, xmm_luma_ps;
    __NVC_BGRA_UNPACK_FLOAT_4P_32BIT(pBGRA, xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps);

    if (nColorimetry == NVC_BT709) {
        xmm_luma_ps = _mm_mul_ps(xmm_r_ps, g_xmm_luma_blue_coef_709_ps);
        xmm_luma_ps = _mm_add_ps(xmm_luma_ps, _mm_mul_ps(xmm_g_ps, g_xmm_luma_green_coef_709_ps));
        xmm_luma_ps = _mm_add_ps(xmm_luma_ps, _mm_mul_ps(xmm_b_ps, g_xmm_luma_red_coef_709_ps));
    } else if (nColorimetry == NVC_BT601) {
        xmm_luma_ps = _mm_mul_ps(xmm_r_ps, g_xmm_luma_blue_coef_601_ps);
        xmm_luma_ps = _mm_add_ps(xmm_luma_ps, _mm_mul_ps(xmm_g_ps, g_xmm_luma_green_coef_601_ps));
        xmm_luma_ps = _mm_add_ps(xmm_luma_ps, _mm_mul_ps(xmm_b_ps, g_xmm_luma_red_coef_601_ps));
	} else if (nColorimetry == NVC_BT2020) {
		xmm_luma_ps = _mm_mul_ps(xmm_r_ps, g_xmm_luma_blue_coef_2020_ps);
		xmm_luma_ps = _mm_add_ps(xmm_luma_ps, _mm_mul_ps(xmm_g_ps, g_xmm_luma_green_coef_2020_ps));
		xmm_luma_ps = _mm_add_ps(xmm_luma_ps, _mm_mul_ps(xmm_b_ps, g_xmm_luma_red_coef_2020_ps));
	}

    return xmm_luma_ps;
}

static inline float __Nvc_Cacl_32Bit_Luma_From_32Bit_RGB(float r, float g, float b, int nColorimetry)
{
    float luma;

	luma = r * (float)__NVC_R_Y(nColorimetry);
    luma += g * (float)__NVC_G_Y(nColorimetry);
    luma += b * (float)__NVC_B_Y(nColorimetry);

    return luma;
}


#ifdef _MSC_VER
#pragma warning(disable:4127)
#endif


// convert four pixels from BGRA to HSV
template <typename T>
static NVC_FORCE_INLINE void __NvcBGRA2HSV4P(const T *bgra,
											 __m128& xmm_hue_ps,
											 __m128& xmm_sat_ps,
											 __m128& xmm_val_ps)
{
	__m128 xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps;
	__m128 xmm_delta_ps, xmm_mask_ps, xmm_tmp_ps;

	if (sizeof(T) == 1) {
		__NVC_BGRA_UNPACK_FLOAT_4P_8BIT(bgra, xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps);
	} else {
		__NVC_BGRA_UNPACK_FLOAT_4P_16BIT(bgra, xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps);
	}

	xmm_val_ps = _mm_max_ps(xmm_r_ps, xmm_g_ps);
	xmm_val_ps = _mm_max_ps(xmm_val_ps, xmm_b_ps);			// max of r/g/b

	xmm_delta_ps = _mm_min_ps(xmm_r_ps, xmm_g_ps);
	xmm_delta_ps = _mm_min_ps(xmm_delta_ps, xmm_b_ps);		// min of r/g/b
	xmm_delta_ps = _mm_sub_ps(xmm_val_ps, xmm_delta_ps);	// delta = max - min

	xmm_sat_ps = _mm_div_ps(xmm_delta_ps, xmm_val_ps);			// delta / max
	xmm_mask_ps = _mm_cmpneq_ps(xmm_val_ps, _mm_setzero_ps());	// mask(max == 0)
	xmm_sat_ps = _mm_and_ps(xmm_sat_ps, xmm_mask_ps);

	// calculate ((g - b) / delta) MOD 6
	xmm_hue_ps = _mm_sub_ps(xmm_g_ps, xmm_b_ps);
	xmm_hue_ps = _mm_div_ps(xmm_hue_ps, xmm_delta_ps);
	xmm_mask_ps = _mm_cmplt_ps(xmm_hue_ps, _mm_setzero_ps());
	xmm_mask_ps = _mm_and_ps(xmm_mask_ps, g_xmm_6_ps);
	xmm_hue_ps = _mm_add_ps(xmm_hue_ps, xmm_mask_ps);
	xmm_mask_ps = _mm_cmpeq_ps(xmm_val_ps, xmm_r_ps);
	xmm_hue_ps = _mm_and_ps(xmm_hue_ps, xmm_mask_ps);

	// calculate (b - r) / delta + 2
	xmm_tmp_ps = _mm_sub_ps(xmm_b_ps, xmm_r_ps);
	xmm_tmp_ps = _mm_div_ps(xmm_tmp_ps, xmm_delta_ps);
	xmm_tmp_ps = _mm_add_ps(xmm_tmp_ps, g_xmm_2_ps);
	xmm_mask_ps = _mm_cmpeq_ps(xmm_val_ps, xmm_g_ps);
	xmm_tmp_ps = _mm_and_ps(xmm_tmp_ps, xmm_mask_ps);
	xmm_hue_ps = _mm_or_ps(xmm_hue_ps, xmm_tmp_ps);

	// calculate (r - g) / delta + 4
	xmm_tmp_ps = _mm_sub_ps(xmm_r_ps, xmm_g_ps);
	xmm_tmp_ps = _mm_div_ps(xmm_tmp_ps, xmm_delta_ps);
	xmm_tmp_ps = _mm_add_ps(xmm_tmp_ps, g_xmm_2_ps);
	xmm_tmp_ps = _mm_add_ps(xmm_tmp_ps, g_xmm_2_ps);
	xmm_mask_ps = _mm_cmpeq_ps(xmm_val_ps, xmm_b_ps);
	xmm_tmp_ps = _mm_and_ps(xmm_tmp_ps, xmm_mask_ps);
	xmm_hue_ps = _mm_or_ps(xmm_hue_ps, xmm_tmp_ps);

	xmm_mask_ps = _mm_cmpneq_ps(xmm_delta_ps, _mm_setzero_ps()); // mask(delta!=0)
	xmm_hue_ps = _mm_and_ps(xmm_hue_ps, xmm_mask_ps);
	xmm_hue_ps = _mm_mul_ps(xmm_hue_ps, g_xmm_60_ps);
}

// convert from RGB to HSV
static NVC_FORCE_INLINE void __NvcRGB2HSV(const __m128& xmm_r_ps,
										  const __m128& xmm_g_ps,
										  const __m128& xmm_b_ps,
										  __m128& xmm_hue_ps,
										  __m128& xmm_sat_ps,
										  __m128& xmm_val_ps)
{
	__m128 xmm_delta_ps, xmm_mask_ps, xmm_tmp_ps;

	xmm_val_ps = _mm_max_ps(xmm_r_ps, xmm_g_ps);
	xmm_val_ps = _mm_max_ps(xmm_val_ps, xmm_b_ps);				// max of r/g/b

	xmm_delta_ps = _mm_min_ps(xmm_r_ps, xmm_g_ps);
	xmm_delta_ps = _mm_min_ps(xmm_delta_ps, xmm_b_ps);			// min of r/g/b
	xmm_delta_ps = _mm_sub_ps(xmm_val_ps, xmm_delta_ps);		// delta = max - min

	xmm_sat_ps = _mm_div_ps(xmm_delta_ps, xmm_val_ps);			// delta / max
	xmm_mask_ps = _mm_cmpneq_ps(xmm_val_ps, _mm_setzero_ps());	// mask(max == 0)
	xmm_sat_ps = _mm_and_ps(xmm_sat_ps, xmm_mask_ps);

	// calculate ((g - b) / delta) MOD 6
	xmm_hue_ps = _mm_sub_ps(xmm_g_ps, xmm_b_ps);
	xmm_hue_ps = _mm_div_ps(xmm_hue_ps, xmm_delta_ps);
	xmm_mask_ps = _mm_cmplt_ps(xmm_hue_ps, _mm_setzero_ps());
	xmm_mask_ps = _mm_and_ps(xmm_mask_ps, g_xmm_6_ps);
	xmm_hue_ps = _mm_add_ps(xmm_hue_ps, xmm_mask_ps);
	xmm_mask_ps = _mm_cmpeq_ps(xmm_val_ps, xmm_r_ps);
	xmm_hue_ps = _mm_and_ps(xmm_hue_ps, xmm_mask_ps);

	// calculate (b - r) / delta + 2
	xmm_tmp_ps = _mm_sub_ps(xmm_b_ps, xmm_r_ps);
	xmm_tmp_ps = _mm_div_ps(xmm_tmp_ps, xmm_delta_ps);
	xmm_tmp_ps = _mm_add_ps(xmm_tmp_ps, g_xmm_2_ps);
	xmm_mask_ps = _mm_cmpeq_ps(xmm_val_ps, xmm_g_ps);
	xmm_tmp_ps = _mm_and_ps(xmm_tmp_ps, xmm_mask_ps);
	xmm_hue_ps = _mm_or_ps(xmm_hue_ps, xmm_tmp_ps);

	// calculate (r - g) / delta + 4
	xmm_tmp_ps = _mm_sub_ps(xmm_r_ps, xmm_g_ps);
	xmm_tmp_ps = _mm_div_ps(xmm_tmp_ps, xmm_delta_ps);
	xmm_tmp_ps = _mm_add_ps(xmm_tmp_ps, g_xmm_2_ps);
	xmm_tmp_ps = _mm_add_ps(xmm_tmp_ps, g_xmm_2_ps);
	xmm_mask_ps = _mm_cmpeq_ps(xmm_val_ps, xmm_b_ps);
	xmm_tmp_ps = _mm_and_ps(xmm_tmp_ps, xmm_mask_ps);
	xmm_hue_ps = _mm_or_ps(xmm_hue_ps, xmm_tmp_ps);

	xmm_mask_ps = _mm_cmpneq_ps(xmm_delta_ps, _mm_setzero_ps()); // mask(delta!=0)
	xmm_hue_ps = _mm_and_ps(xmm_hue_ps, xmm_mask_ps);
	xmm_hue_ps = _mm_mul_ps(xmm_hue_ps, g_xmm_60_ps);
}


// convert four pixels from HSV to BGRA
template <typename T>
static NVC_FORCE_INLINE void __NvcHSV2BGRA4P(__m128& xmm_hue_ps,
											 __m128& xmm_sat_ps,
											 __m128& xmm_val_ps,
											 T *bgra)
{
	T val[4], p[4], q[4], t[4];
	uint8_t i[4];

	__m128 xmm_f_ps, xmm_tmp_ps;
	__m128i xmm0;

	xmm_hue_ps = _mm_div_ps(xmm_hue_ps, g_xmm_60_ps);	// hue / 60
	xmm0 = _mm_cvttps_epi32(xmm_hue_ps);				// truncation of hue / 60
	xmm_tmp_ps = _mm_cvtepi32_ps(xmm0);					// truncation of hue / 60
	xmm_f_ps = _mm_sub_ps(xmm_hue_ps, xmm_tmp_ps);		// f = hue / 60 - truncation of hue / 60
	xmm0 = _mm_packs_epi32(xmm0, xmm0);
	xmm0 = _mm_packus_epi16(xmm0, xmm0);
	*(int *)i = _mm_cvtsi128_si32(xmm0);

	xmm0 = _mm_cvtps_epi32(xmm_val_ps);
	if (sizeof(T) == 1) {
		xmm0 = _mm_packs_epi32(xmm0, xmm0);
		xmm0 = _mm_packus_epi16(xmm0, xmm0);
		*(int *)val = _mm_cvtsi128_si32(xmm0);
	} else {
		xmm0 = _mm_sub_epi32(xmm0, g_xmm_32bit_32768);
		xmm0 = _mm_packs_epi32(xmm0, xmm0);
		xmm0 = _mm_add_epi16(xmm0, g_xmm_16bit_32768);
		_mm_storel_epi64((__m128i *)val, xmm0);
	}

	// calculate p=v*(1-s)
	xmm_tmp_ps = _mm_sub_ps(g_xmm_1_ps, xmm_sat_ps);
	xmm_tmp_ps = _mm_mul_ps(xmm_tmp_ps, xmm_val_ps);
	xmm0 = _mm_cvtps_epi32(xmm_tmp_ps);
	if (sizeof(T) == 1) {
		xmm0 = _mm_packs_epi32(xmm0, xmm0);
		xmm0 = _mm_packus_epi16(xmm0, xmm0);
		*(int *)p = _mm_cvtsi128_si32(xmm0);
	} else {
		xmm0 = _mm_sub_epi32(xmm0, g_xmm_32bit_32768);
		xmm0 = _mm_packs_epi32(xmm0, xmm0);
		xmm0 = _mm_add_epi16(xmm0, g_xmm_16bit_32768);
		_mm_storel_epi64((__m128i *)p, xmm0);
	}

	// calculate q=v*(1-f*s)
	xmm_tmp_ps = _mm_mul_ps(xmm_f_ps, xmm_sat_ps);
	xmm_tmp_ps = _mm_sub_ps(g_xmm_1_ps, xmm_tmp_ps);
	xmm_tmp_ps = _mm_mul_ps(xmm_tmp_ps, xmm_val_ps);
	xmm0 = _mm_cvtps_epi32(xmm_tmp_ps);
	if (sizeof(T) == 1) {
		xmm0 = _mm_packs_epi32(xmm0, xmm0);
		xmm0 = _mm_packus_epi16(xmm0, xmm0);
		*(int *)q = _mm_cvtsi128_si32(xmm0);
	} else {
		xmm0 = _mm_sub_epi32(xmm0, g_xmm_32bit_32768);
		xmm0 = _mm_packs_epi32(xmm0, xmm0);
		xmm0 = _mm_add_epi16(xmm0, g_xmm_16bit_32768);
		_mm_storel_epi64((__m128i *)q, xmm0);
	}

	// calculate t=v*(1-(1-f)*s)
	xmm_tmp_ps = _mm_sub_ps(g_xmm_1_ps, xmm_f_ps);
	xmm_tmp_ps = _mm_mul_ps(xmm_tmp_ps, xmm_sat_ps);
	xmm_tmp_ps = _mm_sub_ps(g_xmm_1_ps, xmm_tmp_ps);
	xmm_tmp_ps = _mm_mul_ps(xmm_tmp_ps, xmm_val_ps);
	xmm0 = _mm_cvtps_epi32(xmm_tmp_ps);
	if (sizeof(T) == 1) {
		xmm0 = _mm_packs_epi32(xmm0, xmm0);
		xmm0 = _mm_packus_epi16(xmm0, xmm0);
		*(int *)t = _mm_cvtsi128_si32(xmm0);
	} else {
		xmm0 = _mm_sub_epi32(xmm0, g_xmm_32bit_32768);
		xmm0 = _mm_packs_epi32(xmm0, xmm0);
		xmm0 = _mm_add_epi16(xmm0, g_xmm_16bit_32768);
		_mm_storel_epi64((__m128i *)t, xmm0);
	}

	for (int k = 0; k < 4; k++) {
		switch (i[k]) {
		case 0: 
			bgra[k * 4 + 0] = p[k];
			bgra[k * 4 + 1] = t[k];
			bgra[k * 4 + 2] = val[k];
			break;
		case 1:
			bgra[k * 4 + 0] = p[k];
			bgra[k * 4 + 1] = val[k];
			bgra[k * 4 + 2] = q[k];			
			break;
		case 2:
			bgra[k * 4 + 0] = t[k];
			bgra[k * 4 + 1] = val[k];
			bgra[k * 4 + 2] = p[k];
			break;
		case 3:
			bgra[k * 4 + 0] = val[k];
			bgra[k * 4 + 1] = q[k];
			bgra[k * 4 + 2] = p[k];			
			break;
		case 4:
			bgra[k * 4 + 0] = val[k];
			bgra[k * 4 + 1] = p[k];
			bgra[k * 4 + 2] = t[k];
			break;
		default: // case 5:
			bgra[k * 4 + 0] = q[k];
			bgra[k * 4 + 1] = p[k];
			bgra[k * 4 + 2] = val[k];
			break;
		}
	}
}

#ifdef _MSC_VER
#pragma warning(disable: 4701)
#endif

// convert from HSV to RGB
static NVC_FORCE_INLINE void __NvcHSV2RGB(const __m128& xmm_hue_ps,
										  const __m128& xmm_sat_ps,
										  const __m128& xmm_val_ps,
										  __m128& xmm_r_ps,
										  __m128& xmm_g_ps,
										  __m128& xmm_b_ps)
{
	union {
		float f[4];
		__m128 x;
	} p, q, t, val;

	union {
		uint8_t idx[4];
		int i;
	} idx;

	__m128 xmm_h_ps, xmm_f_ps, xmm_tmp_ps;
	__m128i xmm0;

	// NOTE: hue must be in the range of [0, 360) !
	xmm_h_ps = _mm_div_ps(xmm_hue_ps, g_xmm_60_ps);	// hue / 60
	xmm0 = _mm_cvttps_epi32(xmm_h_ps);				// floor(hue / 60)
	xmm_tmp_ps = _mm_cvtepi32_ps(xmm0);				// floor(hue / 60)
	xmm_f_ps = _mm_sub_ps(xmm_h_ps, xmm_tmp_ps);	// f = hue / 60 - floor(hue / 60)
	xmm0 = _mm_packs_epi32(xmm0, xmm0);
	xmm0 = _mm_packus_epi16(xmm0, xmm0);
	idx.i = _mm_cvtsi128_si32(xmm0);				// save hue index

	/* calculate p=v*(1-s) */
	p.x = _mm_sub_ps(g_xmm_1_ps, xmm_sat_ps);
	p.x = _mm_mul_ps(p.x, xmm_val_ps);

	/* calculate q=v*(1-f*s) */
	q.x = _mm_mul_ps(xmm_f_ps, xmm_sat_ps);
	q.x = _mm_sub_ps(g_xmm_1_ps, q.x);
	q.x = _mm_mul_ps(q.x, xmm_val_ps);

	/* calculate t=v*(1-(1-f)*s) */
	t.x = _mm_sub_ps(g_xmm_1_ps, xmm_f_ps);
	t.x = _mm_mul_ps(t.x, xmm_sat_ps);
	t.x = _mm_sub_ps(g_xmm_1_ps, t.x);
	t.x = _mm_mul_ps(t.x, xmm_val_ps);

	val.x = xmm_val_ps;

	union {
		float f[4];
		__m128 x;
	} red, green, blue;

	for (int k = 0; k < 4; k++) {
		switch (idx.idx[k]) {
		case 0:
			red.f[k] = val.f[k];
			green.f[k] = t.f[k];
			blue.f[k] = p.f[k];
			break;
		case 1:
			red.f[k] = q.f[k];
			green.f[k] = val.f[k];
			blue.f[k] = p.f[k];
			break;
		case 2:
			red.f[k] = p.f[k];
			green.f[k] = val.f[k];
			blue.f[k] = t.f[k];
			break;
		case 3:
			red.f[k] = p.f[k];
			green.f[k] = q.f[k];
			blue.f[k] = val.f[k];
			break;
		case 4:
			red.f[k] = t.f[k];
			green.f[k] = p.f[k];
			blue.f[k] = val.f[k];
			break;
		default: // case 5:
			red.f[k] = val.f[k];
			green.f[k] = p.f[k];
			blue.f[k] = q.f[k];
			break;
		}
	}

	xmm_r_ps = red.x;
	xmm_g_ps = green.x;
	xmm_b_ps = blue.x;
}

#ifdef _MSC_VER
#pragma warning(default: 4701)
#endif


template < typename T >
static NVC_FORCE_INLINE void __NvcBGRA2HSL4P(const T *bgra,
											 float *hue,
											 float *sat,
											 float *lightness)
{
	__m128 xmm_zero_ps, xmm_delta_ps, xmm_add_ps, xmm_max_ps, xmm_min_ps, xmm_avg_ps, xmm_sat_ps, xmm_hue_ps;
	__m128 xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps, xmm0_ps;
	__m128 xmm_hue_tmp_ps, xmm_2_ps, xmm_6_ps, xmm_60_ps, xmm_sat_tmp_ps;
	__m128 xmm_half_ps;
	__m128 xmm_max_T_ps;

	if (sizeof(T) == 1) {
		xmm_max_T_ps = _mm_set1_ps(255);
		__NVC_BGRA_UNPACK_FLOAT_4P_8BIT(bgra, xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps);
	} else {
		xmm_max_T_ps = _mm_set1_ps(65535);
		__NVC_BGRA_UNPACK_FLOAT_4P_16BIT(bgra, xmm_r_ps, xmm_g_ps, xmm_b_ps, xmm_a_ps);
	}

	// unitary
	xmm_r_ps = _mm_div_ps(xmm_r_ps, xmm_max_T_ps);
	xmm_g_ps = _mm_div_ps(xmm_g_ps, xmm_max_T_ps);
	xmm_b_ps = _mm_div_ps(xmm_b_ps, xmm_max_T_ps);

	// calculate l:
	// formula : 
	// l = 1/2 * (max + min)
	xmm_half_ps = _mm_set1_ps(0.5f);
	xmm_max_ps = _mm_max_ps(xmm_r_ps, xmm_g_ps);
	xmm_max_ps = _mm_max_ps(xmm_max_ps, xmm_b_ps);			// max of r/g/b
	xmm_min_ps = _mm_min_ps(xmm_r_ps, xmm_g_ps);
	xmm_min_ps = _mm_min_ps(xmm_min_ps, xmm_b_ps);		// min of r/g/b

	xmm_delta_ps	= _mm_sub_ps(xmm_max_ps, xmm_min_ps);	// delta = max - min
	xmm_add_ps		= _mm_add_ps(xmm_max_ps, xmm_min_ps);	// add = max + min

	xmm_avg_ps = _mm_mul_ps(xmm_add_ps, xmm_half_ps);

	_mm_store_ps(lightness, xmm_avg_ps);

	xmm_zero_ps = _mm_setzero_ps();
	xmm_2_ps = _mm_set1_ps(2);
	xmm_6_ps = _mm_set1_ps(6);
	xmm_60_ps = _mm_set1_ps(60);

	// calculate s
	// formula :
	// 0 :			if max = min
	// (max-min)/(max+min) = (max-min)/(2l)	: if l <= 1/2
	// (max-min)/(2-(max+min)) = (max-min)/(2-2l) : if l > 1/2
	xmm_sat_tmp_ps	= _mm_div_ps(xmm_delta_ps, xmm_add_ps);
	xmm0_ps			= _mm_cmple_ps(xmm_avg_ps, xmm_half_ps);
	xmm_sat_ps		= _mm_and_ps(xmm_sat_tmp_ps, xmm0_ps);

	xmm_sat_tmp_ps	= _mm_sub_ps(xmm_2_ps, xmm_add_ps);
	xmm_sat_tmp_ps	= _mm_div_ps(xmm_delta_ps, xmm_sat_tmp_ps);
//	xmm0_ps			= _mm_cmpgt_ps(xmm_avg_ps, xmm_half_ps);
	xmm_sat_tmp_ps	= _mm_andnot_ps(xmm0_ps, xmm_sat_tmp_ps);
	xmm_sat_ps		= _mm_or_ps(xmm_sat_ps, xmm_sat_tmp_ps);
	
	xmm0_ps			= _mm_cmpneq_ps(xmm_max_ps, xmm_min_ps);
	xmm_sat_ps		= _mm_and_ps(xmm_sat_ps, xmm0_ps);

	_mm_store_ps(sat, xmm_sat_ps);

	// calculate ((g - b) / delta) MOD 6
	xmm_hue_tmp_ps = _mm_sub_ps(xmm_g_ps, xmm_b_ps);
	xmm_hue_tmp_ps = _mm_div_ps(xmm_hue_tmp_ps, xmm_delta_ps);
	xmm0_ps = _mm_cmplt_ps(xmm_hue_tmp_ps, xmm_zero_ps);
	xmm0_ps = _mm_and_ps(xmm0_ps, xmm_6_ps);
	xmm_hue_tmp_ps = _mm_add_ps(xmm_hue_tmp_ps, xmm0_ps);
	xmm0_ps = _mm_cmpeq_ps(xmm_max_ps, xmm_r_ps);
	xmm_hue_ps = _mm_and_ps(xmm_hue_tmp_ps, xmm0_ps);

	// calculate (b - r) / delta + 2
	xmm_hue_tmp_ps = _mm_sub_ps(xmm_b_ps, xmm_r_ps);
	xmm_hue_tmp_ps = _mm_div_ps(xmm_hue_tmp_ps, xmm_delta_ps);
	xmm_hue_tmp_ps = _mm_add_ps(xmm_hue_tmp_ps, xmm_2_ps);
	xmm0_ps = _mm_cmpeq_ps(xmm_max_ps, xmm_g_ps);
	xmm_hue_tmp_ps = _mm_and_ps(xmm_hue_tmp_ps, xmm0_ps);
	xmm_hue_ps = _mm_or_ps(xmm_hue_ps, xmm_hue_tmp_ps);

	// calculate (r - g) / delta + 4
	xmm_hue_tmp_ps = _mm_sub_ps(xmm_r_ps, xmm_g_ps);
	xmm_hue_tmp_ps = _mm_div_ps(xmm_hue_tmp_ps, xmm_delta_ps);
	xmm_hue_tmp_ps = _mm_add_ps(xmm_hue_tmp_ps, xmm_2_ps);
	xmm_hue_tmp_ps = _mm_add_ps(xmm_hue_tmp_ps, xmm_2_ps);
	xmm0_ps = _mm_cmpeq_ps(xmm_max_ps, xmm_b_ps);
	xmm_hue_tmp_ps = _mm_and_ps(xmm_hue_tmp_ps, xmm0_ps);
	xmm_hue_ps = _mm_or_ps(xmm_hue_ps, xmm_hue_tmp_ps);

	xmm0_ps = _mm_cmpneq_ps(xmm_delta_ps, xmm_zero_ps);		// mask(delta!=0)
	xmm_hue_ps = _mm_and_ps(xmm_hue_ps, xmm0_ps);

	xmm_hue_ps = _mm_mul_ps(xmm_hue_ps, xmm_60_ps);
	_mm_store_ps(hue, xmm_hue_ps);
}

// convert four pixels from HSL to BGRA
template <typename T>
static NVC_FORCE_INLINE void __NvcHSL2BGRA4P(float *hue,
											 float *sat,
											 float *lightness,
											 T* alpha,
											 T *bgra)
{
	__m128 xmm_l_ps, xmm_l_tmp_ps;
	__m128 xmm_s_ps, xmm_s_tmp_ps, xmm_s_tmp_1_ps;
	__m128 xmm_1_ps, xmm_360_ps, xmm_2_ps, xmm_1_3_ps, xmm_1_2_ps, xmm_1_6_ps, xmm_2_3_ps,xmm_6_ps, xmm_zero;
	__m128 xmm_q_ps, xmm_q_tmp_ps;
	__m128 xmm_p_ps, xmm_p_tmp_ps;
	__m128 xmm_h_ps, xmm_h_tmp_ps;
	__m128 xmm_tr_ps, xmm_r_ps;
	__m128 xmm_tg_ps, xmm_g_ps;
	__m128 xmm_tb_ps, xmm_b_ps;
	__m128 xmm0_ps, xmm_tc_tmp_ps, xmm_t_tmp_ps, xmm_q_sub_p_ps;
	__m128 xmm_max_T_ps;
	__m128i xmm_r, xmm_g, xmm_b, xmm_a, xmm_Zero_tmp;

	xmm_1_ps = _mm_set1_ps(1.0f);
	xmm_1_2_ps = _mm_set1_ps(0.5f);

	// calculate q
	// formula :
	// l*(1+s)	: if l<1/2
	// l+s-(l*s) : if l>=1/2
	xmm_l_ps		= _mm_load_ps(lightness);					// light
	xmm_s_ps		= _mm_load_ps(sat);
	xmm_l_tmp_ps	= _mm_add_ps(xmm_1_ps, xmm_s_ps);
	xmm_l_tmp_ps	= _mm_mul_ps(xmm_l_tmp_ps, xmm_l_ps);
	xmm_s_tmp_ps	= _mm_add_ps(xmm_l_ps, xmm_s_ps);
	xmm_s_tmp_1_ps	= _mm_mul_ps(xmm_l_ps, xmm_s_ps);
	xmm_s_tmp_ps	= _mm_sub_ps(xmm_s_tmp_ps, xmm_s_tmp_1_ps);

	xmm0_ps			= _mm_cmplt_ps(xmm_l_ps, xmm_1_2_ps);
	xmm_q_ps		= _mm_and_ps(xmm_l_tmp_ps, xmm0_ps);

//	xmm0_ps			= _mm_cmpge_ps(xmm_l_ps, xmm_1_2_ps);
	xmm_q_tmp_ps	= _mm_andnot_ps(xmm0_ps, xmm_s_tmp_ps);
	xmm_q_ps		= _mm_or_ps(xmm_q_ps, xmm_q_tmp_ps); 

	// calculate p
	// formula :
	// p = 2* l - q
	xmm_2_ps		= _mm_set1_ps(2.0f);
	xmm_p_tmp_ps	= _mm_mul_ps(xmm_l_ps, xmm_2_ps);
	xmm_p_ps		= _mm_sub_ps(xmm_p_tmp_ps, xmm_q_ps);

	// calculate hk
	// formula : 
	// hk = h / 360
	xmm_360_ps		= _mm_set1_ps(360.f);
	xmm_h_ps		= _mm_load_ps(hue);
	xmm_h_tmp_ps	= _mm_div_ps(xmm_h_ps, xmm_360_ps);

	// calculate tr tg tb
	// formula :
	// tr = hk + 1/3
	// tg = hk
	// tb = hk - 1/3
	// if tc < 0 --> tc = tc + 1.0
	// if tc > 1 --> tc = tc - 1.0
	// colorC  = 
	// p + ((q-p)*6*tc)				if tc <1/6
	// q							if 1/6<=tc<1/2
	// p + ((q-p)*6*(2/3-tc))		if 1/2<=tc<2/3
	// p							otherwise
	xmm_zero		= _mm_set1_ps(0.f);
	xmm_1_3_ps		= _mm_set1_ps(0.333333f);

	// calculate tr
	xmm_t_tmp_ps	= _mm_add_ps(xmm_h_tmp_ps, xmm_1_3_ps);	// hk + 1/3
	xmm0_ps			= _mm_cmplt_ps(xmm_t_tmp_ps, xmm_zero);
	xmm_tc_tmp_ps	= _mm_and_ps(xmm_1_ps, xmm0_ps);
	xmm_tr_ps		= _mm_add_ps(xmm_t_tmp_ps, xmm_tc_tmp_ps);

	xmm0_ps			= _mm_cmpgt_ps(xmm_t_tmp_ps, xmm_1_ps);
	xmm_tc_tmp_ps	= _mm_and_ps(xmm_1_ps, xmm0_ps);
	xmm_tr_ps		= _mm_sub_ps(xmm_tr_ps, xmm_tc_tmp_ps);

	// calculate tg
	xmm0_ps			= _mm_cmplt_ps(xmm_h_tmp_ps, xmm_zero);
	xmm_tc_tmp_ps	= _mm_and_ps(xmm_1_ps, xmm0_ps);
	xmm_tg_ps		= _mm_add_ps(xmm_h_tmp_ps, xmm_tc_tmp_ps);

	xmm0_ps			= _mm_cmpgt_ps(xmm_h_tmp_ps, xmm_1_ps);
	xmm_tc_tmp_ps	= _mm_and_ps(xmm_1_ps, xmm0_ps);
	xmm_tg_ps		= _mm_sub_ps(xmm_tg_ps, xmm_tc_tmp_ps);

	// calculate tb
	xmm_t_tmp_ps	= _mm_sub_ps(xmm_h_tmp_ps, xmm_1_3_ps); // hk - 1/3
	xmm0_ps			= _mm_cmplt_ps(xmm_t_tmp_ps, xmm_zero);
	xmm_tc_tmp_ps	= _mm_and_ps(xmm_1_ps, xmm0_ps);
	xmm_tb_ps		= _mm_add_ps(xmm_t_tmp_ps, xmm_tc_tmp_ps);

	xmm0_ps			= _mm_cmpgt_ps(xmm_t_tmp_ps, xmm_1_ps);
	xmm_tc_tmp_ps	= _mm_and_ps(xmm_1_ps, xmm0_ps);
	xmm_tb_ps		= _mm_sub_ps(xmm_tb_ps, xmm_tc_tmp_ps);

	if (sizeof(T) == 1)
		xmm_max_T_ps = _mm_set1_ps(255.0f);
	else
		xmm_max_T_ps = _mm_set1_ps(65535.0f);

	// calculate R G B
	xmm_1_6_ps		= _mm_set1_ps(0.166667f);
	xmm_2_3_ps		= _mm_set1_ps(0.666667f);
	xmm_6_ps		= _mm_set1_ps(6.0f);
	xmm_q_sub_p_ps	= _mm_sub_ps(xmm_q_ps, xmm_p_ps);
	xmm_q_sub_p_ps	= _mm_mul_ps(xmm_q_sub_p_ps, xmm_6_ps);

	// R
	xmm_t_tmp_ps	= _mm_mul_ps(xmm_q_sub_p_ps, xmm_tr_ps); // p + ((q-p)*6*tc)				if tc <1/6
	xmm_t_tmp_ps	= _mm_add_ps(xmm_t_tmp_ps, xmm_p_ps);
	xmm0_ps			= _mm_cmplt_ps(xmm_tr_ps, xmm_1_6_ps);
	xmm_r_ps		= _mm_and_ps(xmm_t_tmp_ps, xmm0_ps);

	xmm0_ps			= _mm_cmpge_ps(xmm_tr_ps, xmm_1_6_ps); // q							if 1/6<=tc<1/2
	xmm_tc_tmp_ps	= _mm_cmplt_ps(xmm_tr_ps, xmm_1_2_ps);
	xmm0_ps			= _mm_and_ps(xmm0_ps, xmm_tc_tmp_ps);
	xmm_t_tmp_ps	= _mm_and_ps(xmm_q_ps, xmm0_ps);
	xmm_r_ps		= _mm_or_ps(xmm_r_ps, xmm_t_tmp_ps);

	xmm_t_tmp_ps	= _mm_sub_ps(xmm_2_3_ps, xmm_tr_ps); // p + ((q-p)*6*(2/3-tc))		if 1/2<=tc<2/3
	xmm_t_tmp_ps	= _mm_mul_ps(xmm_t_tmp_ps, xmm_q_sub_p_ps);
	xmm_t_tmp_ps	= _mm_add_ps(xmm_t_tmp_ps, xmm_p_ps);
	xmm0_ps			= _mm_cmpge_ps(xmm_tr_ps, xmm_1_2_ps);
	xmm_tc_tmp_ps	= _mm_cmplt_ps(xmm_tr_ps, xmm_2_3_ps);
	xmm0_ps			= _mm_and_ps(xmm0_ps, xmm_tc_tmp_ps);
	xmm_t_tmp_ps	= _mm_and_ps(xmm_t_tmp_ps, xmm0_ps);
	xmm_r_ps		= _mm_or_ps(xmm_r_ps, xmm_t_tmp_ps);
	
	xmm0_ps			= _mm_cmpge_ps(xmm_tr_ps, xmm_2_3_ps);
	xmm_t_tmp_ps	= _mm_and_ps(xmm_p_ps, xmm0_ps);
	xmm_r_ps		= _mm_or_ps(xmm_r_ps, xmm_t_tmp_ps);

	xmm_r_ps		= _mm_mul_ps(xmm_r_ps, xmm_max_T_ps);

	// G
	xmm_t_tmp_ps	= _mm_mul_ps(xmm_q_sub_p_ps, xmm_tg_ps); // p + ((q-p)*6*tc)				if tc <1/6
	xmm_t_tmp_ps	= _mm_add_ps(xmm_t_tmp_ps, xmm_p_ps);
	xmm0_ps			= _mm_cmplt_ps(xmm_tg_ps, xmm_1_6_ps);
	xmm_g_ps		= _mm_and_ps(xmm_t_tmp_ps, xmm0_ps);
	
	xmm0_ps			= _mm_cmpge_ps(xmm_tg_ps, xmm_1_6_ps); // q							if 1/6<=tc<1/2
	xmm_tc_tmp_ps	= _mm_cmplt_ps(xmm_tg_ps, xmm_1_2_ps);
	xmm0_ps			= _mm_and_ps(xmm0_ps, xmm_tc_tmp_ps);
	xmm_t_tmp_ps	= _mm_and_ps(xmm_q_ps, xmm0_ps);
	xmm_g_ps		= _mm_or_ps(xmm_g_ps, xmm_t_tmp_ps);

	xmm_t_tmp_ps	= _mm_sub_ps(xmm_2_3_ps, xmm_tg_ps); // p + ((q-p)*6*(2/3-tc))		if 1/2<=tc<2/3
	xmm_t_tmp_ps	= _mm_mul_ps(xmm_t_tmp_ps, xmm_q_sub_p_ps);
	xmm_t_tmp_ps	= _mm_add_ps(xmm_t_tmp_ps, xmm_p_ps);
	xmm0_ps			= _mm_cmpge_ps(xmm_tg_ps, xmm_1_2_ps);
	xmm_tc_tmp_ps	= _mm_cmplt_ps(xmm_tg_ps, xmm_2_3_ps);
	xmm0_ps			= _mm_and_ps(xmm0_ps, xmm_tc_tmp_ps);
	xmm_t_tmp_ps	= _mm_and_ps(xmm_t_tmp_ps, xmm0_ps);
	xmm_g_ps		= _mm_or_ps(xmm_g_ps, xmm_t_tmp_ps);

	xmm0_ps			= _mm_cmpge_ps(xmm_tg_ps, xmm_2_3_ps);
	xmm_t_tmp_ps	= _mm_and_ps(xmm_p_ps, xmm0_ps);
	xmm_g_ps		= _mm_or_ps(xmm_g_ps, xmm_t_tmp_ps);

	xmm_g_ps		= _mm_mul_ps(xmm_g_ps, xmm_max_T_ps);

	// b
	xmm_t_tmp_ps	= _mm_mul_ps(xmm_q_sub_p_ps, xmm_tb_ps); // p + ((q-p)*6*tc)				if tc <1/6
	xmm_t_tmp_ps	= _mm_add_ps(xmm_t_tmp_ps, xmm_p_ps);
	xmm0_ps			= _mm_cmplt_ps(xmm_tb_ps, xmm_1_6_ps);
	xmm_b_ps		= _mm_and_ps(xmm_t_tmp_ps, xmm0_ps);

	xmm0_ps			= _mm_cmpge_ps(xmm_tb_ps, xmm_1_6_ps); // q							if 1/6<=tc<1/2
	xmm_tc_tmp_ps	= _mm_cmplt_ps(xmm_tb_ps, xmm_1_2_ps);
	xmm0_ps			= _mm_and_ps(xmm0_ps, xmm_tc_tmp_ps);
	xmm_t_tmp_ps	= _mm_and_ps(xmm_q_ps, xmm0_ps);
	xmm_b_ps		= _mm_or_ps(xmm_b_ps, xmm_t_tmp_ps);

	xmm_t_tmp_ps	= _mm_sub_ps(xmm_2_3_ps, xmm_tb_ps); // p + ((q-p)*6*(2/3-tc))		if 1/2<=tc<2/3
	xmm_t_tmp_ps	= _mm_mul_ps(xmm_t_tmp_ps, xmm_q_sub_p_ps); 
	xmm_t_tmp_ps	= _mm_add_ps(xmm_t_tmp_ps, xmm_p_ps);
	xmm0_ps			= _mm_cmpge_ps(xmm_tb_ps, xmm_1_2_ps);
	xmm_tc_tmp_ps	= _mm_cmplt_ps(xmm_tb_ps, xmm_2_3_ps);
	xmm0_ps			= _mm_and_ps(xmm0_ps, xmm_tc_tmp_ps);
	xmm_t_tmp_ps	= _mm_and_ps(xmm_t_tmp_ps, xmm0_ps);
	xmm_b_ps		= _mm_or_ps(xmm_b_ps, xmm_t_tmp_ps);

	xmm0_ps			= _mm_cmpge_ps(xmm_tb_ps, xmm_2_3_ps);
	xmm_t_tmp_ps	= _mm_and_ps(xmm_p_ps, xmm0_ps);
	xmm_b_ps		= _mm_or_ps(xmm_b_ps, xmm_t_tmp_ps);

	xmm_b_ps		= _mm_mul_ps(xmm_b_ps, xmm_max_T_ps);

	//
	xmm_Zero_tmp	= _mm_setzero_si128();

	xmm_r			= _mm_cvtps_epi32(xmm_r_ps);
	xmm_g			= _mm_cvtps_epi32(xmm_g_ps);
	xmm_b			= _mm_cvtps_epi32(xmm_b_ps);
	
	if (sizeof(T) == 1)
	{
		xmm_r		= _mm_packs_epi32(xmm_r, xmm_r);	// r3 r2 r1 r0 r3 r2 r1 r0 [ on 16 bits ]
		xmm_g		= _mm_packs_epi32(xmm_g, xmm_g);	// g3 g2 g1 g0 g3 g2 g1 g0 [ on 16 bits ]
		xmm_b		= _mm_packs_epi32(xmm_b, xmm_b);	// b3 b2 b1 b0 b3 b2 b1 b0 [ on 16 bits ]
		xmm_r		= _mm_packus_epi16(xmm_r, xmm_r);	// r3 r2 r1 r0 r3 r2 r1 r0 r3 r2 r1 r0 r3 r2 r1 r0 [ on 8 bits ]
		xmm_g		= _mm_packus_epi16(xmm_g, xmm_g);	// g3 g2 g1 g0 g3 g2 g1 g0 g3 g2 g1 g0 g3 g2 g1 g0 [ on 8 bits ]
		xmm_b		= _mm_packus_epi16(xmm_b, xmm_b);	// b3 b2 b1 b0 b3 b2 b1 b0 b3 b2 b1 b0 b3 b2 b1 b0 [ on 8 bits ]
		xmm_a		= _mm_cvtsi32_si128(*(int*)alpha);	// 0 0 0 0  0  0  0  0  0  0  0  0  0  a3 a2 a1 a0 [ on 8 bits ]
		xmm_r		= _mm_unpacklo_epi8(xmm_r, xmm_a);	// 0 r3 0 r2 0 r1 0 r0 a3 r3 a2 r2 a1 r1 a0 r0 [ on 8 bits ]
		xmm_b		= _mm_unpacklo_epi8(xmm_b, xmm_g);	// g3 b3 g2 b2 g1 b1 g0 b0 g2 b3 g2 b2 g1 b1 g0 b0 [ on 8 bits ]
		xmm_b		= _mm_unpacklo_epi16(xmm_b, xmm_r); // a3 r3 g3 b3 a2 r2 g2 b2 a1 r1 g1 b1 a0 r0 g0 b0 [ on 8 bits ]

		_mm_store_si128((__m128i*)bgra, xmm_b);
	}
	else
	{
		xmm_r		= _mm_sub_epi32(xmm_r, g_xmm_32bit_32768);
		xmm_r		= _mm_packs_epi32(xmm_r, xmm_r);	// r3 r2 r1 r0 r3 r2 r1 r0 [ on 16 bits ]
		xmm_r		= _mm_add_epi16(xmm_r, g_xmm_16bit_32768);

		xmm_g		= _mm_sub_epi32(xmm_g, g_xmm_32bit_32768);
		xmm_g		= _mm_packs_epi32(xmm_g, xmm_g);	// g3 g2 g1 g0 g3 g2 g1 g0 [ on 16 bits ]
		xmm_g		= _mm_add_epi16(xmm_g, g_xmm_16bit_32768);

		xmm_b		= _mm_sub_epi32(xmm_b, g_xmm_32bit_32768);
		xmm_b		= _mm_packs_epi32(xmm_b, xmm_b);	// b3 b2 b1 b0 b3 b2 b1 b0 [ on 16 bits ]
		xmm_b		= _mm_add_epi16(xmm_b, g_xmm_16bit_32768);

		xmm_a		= _mm_loadl_epi64((__m128i*)alpha);	// 0 0 0 0  a3 a2 a1 a0 [ on 16 bits ]
		xmm_r		= _mm_unpacklo_epi16(xmm_r, xmm_a);	// a3 r3 a2 r2 a1 r1 a0 r0 [ on 16 bits ]
		xmm_b		= _mm_unpacklo_epi16(xmm_b, xmm_g);	// g3 b3 g2 b2 g1 b1 g0 b0 [ on 16 bits ]
		xmm_g		= _mm_load_si128(&xmm_b);			// g3 b3 g2 b2 g1 b1 g0 b0 [ on 16 bits ]
		xmm_b		= _mm_unpacklo_epi32(xmm_b, xmm_r); // a1 r1 g1 b1 a0 r0 g0 b0 [ on 16 bits ]
		xmm_g		= _mm_unpackhi_epi32(xmm_g, xmm_r); // a2 r2 g2 b2 a2 r2 g2 b2 [ on 16 bits ]

		_mm_store_si128((__m128i*)bgra, xmm_b);
		_mm_store_si128((__m128i*)(bgra+8), xmm_g);
	}
}

static NVC_FORCE_INLINE void __NVC_RGB_UNPACK_FLOAT_4P_24BIT(const float *rgb, __m128 &xmm_r_ps, __m128 &xmm_g_ps, __m128 &xmm_b_ps)																		\
{																																				
	const __m128i g_xmm_mask = _mm_set_epi32(0, 0, 0, 0xFFFFFFFF);																				
																																				
    __m128i __xmm_tmp0 = _mm_load_si128((__m128i *)(rgb));			/* [       r1      |       b0      |       g0      |       r0      ] */		
    __m128i __xmm_tmp1 = _mm_load_si128((__m128i *)(rgb + 4));		/* [       g2      |       r2      |       b1      |       g1      ] */
    __m128i __xmm_tmp2 = _mm_load_si128((__m128i *)(rgb + 8));		/* [       r3      |       g3      |       b3      |       b2      ] */
																																				
	__m128i xmm_dstR = _mm_and_si128(__xmm_tmp0, g_xmm_mask);																					
	__m128i xmm_dstG = _mm_and_si128(_mm_srli_si128(__xmm_tmp0, 4), g_xmm_mask);																
	__m128i xmm_dstB = _mm_and_si128(_mm_srli_si128(__xmm_tmp0, 8), g_xmm_mask);																
	__m128i xmm_dTmp = _mm_and_si128(_mm_srli_si128(__xmm_tmp0, 12), g_xmm_mask);																
	xmm_dstR = _mm_or_si128(_mm_slli_si128(xmm_dTmp,4), xmm_dstR);																				
																																				
    xmm_dTmp = _mm_and_si128(__xmm_tmp1, g_xmm_mask);																							
	xmm_dstG = _mm_or_si128(_mm_slli_si128(xmm_dTmp,4), xmm_dstG);																				
    xmm_dTmp = _mm_and_si128(_mm_srli_si128(__xmm_tmp1, 4), g_xmm_mask);																		
	xmm_dstB = _mm_or_si128(_mm_slli_si128(xmm_dTmp,4), xmm_dstB);																				
	xmm_dTmp = _mm_and_si128(_mm_srli_si128(__xmm_tmp1, 8), g_xmm_mask);																		
	xmm_dstR = _mm_or_si128(_mm_slli_si128(xmm_dTmp,8), xmm_dstR);																				
	xmm_dTmp = _mm_and_si128(_mm_srli_si128(__xmm_tmp1, 12), g_xmm_mask);																		
	xmm_dstG = _mm_or_si128(_mm_slli_si128(xmm_dTmp,8), xmm_dstG);																				
																																				
    xmm_dTmp = _mm_and_si128(__xmm_tmp2, g_xmm_mask);																							
	xmm_dstB = _mm_or_si128(_mm_slli_si128(xmm_dTmp,8), xmm_dstB);																			
    xmm_dTmp = _mm_and_si128(_mm_srli_si128(__xmm_tmp2, 4), g_xmm_mask);																		
	xmm_dstR = _mm_or_si128(_mm_slli_si128(xmm_dTmp,12), xmm_dstR);																				
	xmm_dTmp = _mm_and_si128(_mm_srli_si128(__xmm_tmp2, 8), g_xmm_mask);																		
	xmm_dstG = _mm_or_si128(_mm_slli_si128(xmm_dTmp,12), xmm_dstG);																				
	xmm_dTmp = _mm_and_si128(_mm_srli_si128(__xmm_tmp2, 12), g_xmm_mask);																		
	xmm_dstB = _mm_or_si128(_mm_slli_si128(xmm_dTmp,12), xmm_dstB);																				
	
    xmm_r_ps = *(__m128 *)(&xmm_dstR);				/* [       r3      |       r2      |       r1      |       r0      ] */
    xmm_g_ps = *(__m128 *)(&xmm_dstG);				/* [       g3      |       g2      |       g1      |       g0      ] */
    xmm_b_ps = *(__m128 *)(&xmm_dstB);				/* [       b3      |       b2      |       b1      |       b0      ] */
}

static NVC_FORCE_INLINE void __NVC_RGB_UNPACK_FLOAT_4P_24BIT_UNALIGNED(const float *rgb, __m128 &xmm_r_ps, __m128 &xmm_g_ps, __m128 &xmm_b_ps)
{																																				
	const __m128i g_xmm_mask = _mm_set_epi32(0, 0, 0, 0xFFFFFFFF);																				
																																				
    __m128i __xmm_tmp0 = _mm_loadu_si128((__m128i *)(rgb));			/* [       r1      |       b0      |       g0      |       r0      ] */		
    __m128i __xmm_tmp1 = _mm_loadu_si128((__m128i *)(rgb + 4));		/* [       g2      |       r2      |       b1      |       g1      ] */
    __m128i __xmm_tmp2 = _mm_loadu_si128((__m128i *)(rgb + 8));		/* [       r3      |       g3      |       b3      |       b2      ] */
																																				
	__m128i xmm_dstR = _mm_and_si128(__xmm_tmp0, g_xmm_mask);																					
	__m128i xmm_dstG = _mm_and_si128(_mm_srli_si128(__xmm_tmp0, 4), g_xmm_mask);																
	__m128i xmm_dstB = _mm_and_si128(_mm_srli_si128(__xmm_tmp0, 8), g_xmm_mask);																
	__m128i xmm_dTmp = _mm_and_si128(_mm_srli_si128(__xmm_tmp0, 12), g_xmm_mask);																
	xmm_dstR = _mm_or_si128(_mm_slli_si128(xmm_dTmp,4), xmm_dstR);																				
																																				
    xmm_dTmp = _mm_and_si128(__xmm_tmp1, g_xmm_mask);																							
	xmm_dstG = _mm_or_si128(_mm_slli_si128(xmm_dTmp,4), xmm_dstG);																				
    xmm_dTmp = _mm_and_si128(_mm_srli_si128(__xmm_tmp1, 4), g_xmm_mask);																		
	xmm_dstB = _mm_or_si128(_mm_slli_si128(xmm_dTmp,4), xmm_dstB);																				
	xmm_dTmp = _mm_and_si128(_mm_srli_si128(__xmm_tmp1, 8), g_xmm_mask);																		
	xmm_dstR = _mm_or_si128(_mm_slli_si128(xmm_dTmp,8), xmm_dstR);																				
	xmm_dTmp = _mm_and_si128(_mm_srli_si128(__xmm_tmp1, 12), g_xmm_mask);																		
	xmm_dstG = _mm_or_si128(_mm_slli_si128(xmm_dTmp,8), xmm_dstG);																				
																																				
    xmm_dTmp = _mm_and_si128(__xmm_tmp2, g_xmm_mask);																							
	xmm_dstB = _mm_or_si128(_mm_slli_si128(xmm_dTmp,8), xmm_dstB);																				
    xmm_dTmp = _mm_and_si128(_mm_srli_si128(__xmm_tmp2, 4), g_xmm_mask);																		
	xmm_dstR = _mm_or_si128(_mm_slli_si128(xmm_dTmp,12), xmm_dstR);																				
	xmm_dTmp = _mm_and_si128(_mm_srli_si128(__xmm_tmp2, 8), g_xmm_mask);																		
	xmm_dstG = _mm_or_si128(_mm_slli_si128(xmm_dTmp,12), xmm_dstG);																				
	xmm_dTmp = _mm_and_si128(_mm_srli_si128(__xmm_tmp2, 12), g_xmm_mask);																		
	xmm_dstB = _mm_or_si128(_mm_slli_si128(xmm_dTmp,12), xmm_dstB);																				
																																				
	xmm_r_ps = *(__m128 *)(&xmm_dstR);				/* [       r3      |       r2      |       r1      |       r0      ] */
	xmm_g_ps = *(__m128 *)(&xmm_dstG);				/* [       g3      |       g2      |       g1      |       g0      ] */
	xmm_b_ps = *(__m128 *)(&xmm_dstB);				/* [       b3      |       b2      |       b1      |       b0      ] */
}

#ifdef _MSC_VER
#pragma warning(default:4127)
#endif


void __NvcApplyOpacityToMatteLine8Bit(void *matteLine, unsigned int pixelCount, float opacity);
void __NvcApplyOpacityToMatteLine16Bit(void *matteLine, unsigned int pixelCount, float opacity);
void __NvcApplyOpacityToMatteLine32Bit(void *matteLine, unsigned int pixelCount, float opacity);

void __NvcApplyOpacityToBGRALine8Bit(void *bgraLine, unsigned int pixelCount, float opacity);
void __NvcApplyOpacityToBGRALine16Bit(void *bgraLine, unsigned int pixelCount, float opacity);
void __NvcApplyOpacityToBGRALine32Bit(void *bgraLine, unsigned int pixelCount, float opacity);

void __NvcInvertMatteLine8Bit(const void *srcMatteLine, void *dstMatteLine, unsigned int pixelCount, unsigned int invertLimit, bool isNonTemporal);
void __NvcInvertMatteLine16Bit(const void *srcMatteLine, void *dstMatteLine, unsigned int pixelCount, unsigned int invertLimit, bool isNonTemporal);
void __NvcInvertMatteLine32Bit(const void *srcMatteLine, void *dstMatteLine, unsigned int pixelCount, float invertLimit, bool isNonTemporal);

unsigned int __NvcCalcLineSize(unsigned int nWidth,
                               ENvcColorSpace eColorSpace,
                               ENvcBitDepth eBitDepth);

void __NvcClearLineToBlack(void *pLine,
                           unsigned int nWidth,
                           ENvcColorSpace eColorSpace,
                           ENvcBitDepth eBitDepth,
                           bool bIsNonTemporal);


