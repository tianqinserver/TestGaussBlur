//==========================================================================
//
// (c) Copyright NewAuto Video Technology CO.,Ltd., 2010. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind, 
// either expressed or implied, including but not limited to the implied 
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------
//   Birth Date:       Sep 16. 2010
//   Author:           NewAuto video team
//--------------------------------------------------------------------------
#pragma once

#include "NvcVisionUtilsPixel.h"
//////////////////////////////////////////////////////////////////////////
//
// RGB --> YUV conversion fomula (Rec.601)
//
// Y =  (0.25679 * R) + (0.50413 * G) + (0.09791 * B) + 16
// U = -(0.14822 * R) - (0.29099 * G) + (0.43922 * B) + 128
// V =  (0.43922 * R) - (0.36779 * G) - (0.07142 * B) + 128
//
//
// YUV --> RGB conversion fomula (Rec.601)
//
// R = 1.16438(Y - 16)                    + 1.59602(V - 128)
// G = 1.16438(Y - 16) - 0.39176(U - 128) - 0.81297(V - 128) 
// B = 1.16438(Y - 16) + 2.01723(U - 128)
//
//
// RGB --> YUV conversion fomula (Rec.709)
//
// Y =  (0.18259 * R) + (0.61423 * G) + (0.06201 * B) + 16
// U = -(0.10065 * R) - (0.33857 * G) + (0.43922 * B) + 128
// V =  (0.43922 * R) - (0.39894 * G) - (0.04027 * B) + 128
//
//
// YUV --> RGB conversion fomula (Rec.709)
//
// R = 1.16438(Y - 16)                    + 1.79275(V - 128)
// G = 1.16438(Y - 16) - 0.21325(U - 128) - 0.53291(V - 128)
// B = 1.16438(Y - 16) + 2.11240(U - 128)
//
//////////////////////////////////////////////////////////////////////////

struct __Xmm_RGB_YUV_Params {
	__m128i xmm_R_Y, xmm_G_Y, xmm_B_Y;
	__m128i xmm_R_U, xmm_G_U, xmm_B_U;
	__m128i xmm_R_V, xmm_G_V, xmm_B_V;
};

struct __Xmm_PS_RGB_YUV_Params {
	__m128 xmm_R_Y, xmm_G_Y, xmm_B_Y;
	__m128 xmm_R_U, xmm_G_U, xmm_B_U;
	__m128 xmm_R_V, xmm_G_V, xmm_B_V;
};

struct __Xmm_YUV_RGB_Params {
	__m128i xmm_Y;
	__m128i xmm_U_G, xmm_U_B;
	__m128i xmm_V_R, xmm_V_G;
};

struct __Xmm_PS_YUV_RGB_Params {
	__m128 xmm_Y;
	__m128 xmm_U_G, xmm_U_B;
	__m128 xmm_V_R, xmm_V_G;
};

struct __Xmm128_PS_YUV_ColorimetryConvert_Params {
	__m128 xmm_Y_U, xmm_Y_V;
	__m128 xmm_U_U, xmm_U_V;
	__m128 xmm_V_U, xmm_V_V;
};


static inline void __Fill_Xmm_8B_YUV_8B_RGB_Params(int nColorimetry, __Xmm_YUV_RGB_Params *params)
{
	params->xmm_Y = _mm_set1_epi16((short)(NVC_YUY2_Y * 8192));
	params->xmm_U_G = _mm_set1_epi16((short)(NVC_YUY2_U_G(nColorimetry) * 8192));
	params->xmm_U_B = _mm_set1_epi16((short)(NVC_YUY2_U_B(nColorimetry) * 8192));
	params->xmm_V_G = _mm_set1_epi16((short)(NVC_YUY2_V_G(nColorimetry) * 8192));
	params->xmm_V_R = _mm_set1_epi16((short)(NVC_YUY2_V_R(nColorimetry) * 8192));
}

static inline void __Fill_Xmm_8B_YUV_16B_RGB_Params(int nColorimetry, __Xmm_PS_YUV_RGB_Params *params)
{
	params->xmm_Y = _mm_set1_ps((float)(NVC_YUY2_Y * 257));
	params->xmm_U_G = _mm_set1_ps((float)(NVC_YUY2_U_G(nColorimetry) * 257));
	params->xmm_U_B = _mm_set1_ps((float)(NVC_YUY2_U_B(nColorimetry) * 257));
	params->xmm_V_G = _mm_set1_ps((float)(NVC_YUY2_V_G(nColorimetry) * 257));
	params->xmm_V_R = _mm_set1_ps((float)(NVC_YUY2_V_R(nColorimetry) * 257));
}

static inline void __Fill_Xmm_8B_YUV_32B_RGB_Params(int nColorimetry, __Xmm_PS_YUV_RGB_Params *params)
{
	params->xmm_Y = _mm_set1_ps((float)(NVC_YUY2_Y / 255));
	params->xmm_U_G = _mm_set1_ps((float)(NVC_YUY2_U_G(nColorimetry) / 255));
	params->xmm_U_B = _mm_set1_ps((float)(NVC_YUY2_U_B(nColorimetry) / 255));
	params->xmm_V_G = _mm_set1_ps((float)(NVC_YUY2_V_G(nColorimetry) / 255));
	params->xmm_V_R = _mm_set1_ps((float)(NVC_YUY2_V_R(nColorimetry) / 255));
}

static inline void __Fill_Xmm_16B_RGB_8B_YUV_Params(int nColorimetry, __Xmm_PS_RGB_YUV_Params *params)
{
	params->xmm_R_Y = _mm_set1_ps((float)(NVC_YUY2_R_Y(nColorimetry) / 257));
	params->xmm_G_Y = _mm_set1_ps((float)(NVC_YUY2_G_Y(nColorimetry) / 257));
	params->xmm_B_Y = _mm_set1_ps((float)(NVC_YUY2_B_Y(nColorimetry) / 257));
	params->xmm_R_U = _mm_set1_ps((float)(NVC_YUY2_R_U(nColorimetry) / 257));
	params->xmm_G_U = _mm_set1_ps((float)(NVC_YUY2_G_U(nColorimetry) / 257));
	params->xmm_B_U = _mm_set1_ps((float)(NVC_YUY2_B_U(nColorimetry) / 257));
	params->xmm_R_V = _mm_set1_ps((float)(NVC_YUY2_R_V(nColorimetry) / 257));
	params->xmm_G_V = _mm_set1_ps((float)(NVC_YUY2_G_V(nColorimetry) / 257));
	params->xmm_B_V = _mm_set1_ps((float)(NVC_YUY2_B_V(nColorimetry) / 257));
}

static inline void __Fill_Xmm_8B_RGB_8B_YUV_Params(int nColorimetry, __Xmm_RGB_YUV_Params *params)
{
	params->xmm_R_Y = _mm_set1_epi16((short)(NVC_YUY2_R_Y(nColorimetry) * 65536));
	params->xmm_G_Y = _mm_set1_epi16((short)(NVC_YUY2_G_Y(nColorimetry) * 65536));
	params->xmm_B_Y = _mm_set1_epi16((short)(NVC_YUY2_B_Y(nColorimetry) * 65536));
	params->xmm_R_U = _mm_set1_epi16((short)(NVC_YUY2_R_U(nColorimetry) * 65536));
	params->xmm_G_U = _mm_set1_epi16((short)(NVC_YUY2_G_U(nColorimetry) * 65536));
	params->xmm_B_U = _mm_set1_epi16((short)(NVC_YUY2_B_U(nColorimetry) * 65536));
	params->xmm_R_V = _mm_set1_epi16((short)(NVC_YUY2_R_V(nColorimetry) * 65536));
	params->xmm_G_V = _mm_set1_epi16((short)(NVC_YUY2_G_V(nColorimetry) * 65536));
	params->xmm_B_V = _mm_set1_epi16((short)(NVC_YUY2_B_V(nColorimetry) * 65536));
}

static inline void __Fill_Xmm_32B_RGB_8B_YUV_Params(int nColorimetry, __Xmm_PS_RGB_YUV_Params *params)
{
	params->xmm_R_Y = _mm_set1_ps((float)(NVC_YUY2_R_Y(nColorimetry) * 255));
	params->xmm_G_Y = _mm_set1_ps((float)(NVC_YUY2_G_Y(nColorimetry) * 255));
	params->xmm_B_Y = _mm_set1_ps((float)(NVC_YUY2_B_Y(nColorimetry) * 255));
	params->xmm_R_U = _mm_set1_ps((float)(NVC_YUY2_R_U(nColorimetry) * 255));
	params->xmm_G_U = _mm_set1_ps((float)(NVC_YUY2_G_U(nColorimetry) * 255));
	params->xmm_B_U = _mm_set1_ps((float)(NVC_YUY2_B_U(nColorimetry) * 255));
	params->xmm_R_V = _mm_set1_ps((float)(NVC_YUY2_R_V(nColorimetry) * 255));
	params->xmm_G_V = _mm_set1_ps((float)(NVC_YUY2_G_V(nColorimetry) * 255));
	params->xmm_B_V = _mm_set1_ps((float)(NVC_YUY2_B_V(nColorimetry) * 255));
}

static inline void __Fill_xmm_10B_YUV_8B_RGB_Params(int nColorimetry, __Xmm_YUV_RGB_Params *CovParams_xmm)
{
	CovParams_xmm->xmm_Y		= _mm_set1_epi16((short)(NVC_YUY2_Y * 0x2000)); // * (2^13)
	CovParams_xmm->xmm_U_G	= _mm_set1_epi16((short)(NVC_YUY2_U_G(nColorimetry) * 0x2000)); // * (2^13)
	CovParams_xmm->xmm_U_B	= _mm_set1_epi16((short)(NVC_YUY2_U_B(nColorimetry) * 0x2000)); // * (2^13)
	CovParams_xmm->xmm_V_G	= _mm_set1_epi16((short)(NVC_YUY2_V_G(nColorimetry) * 0x2000)); // * (2^13)
	CovParams_xmm->xmm_V_R	= _mm_set1_epi16((short)(NVC_YUY2_V_R(nColorimetry) * 0x2000)); // * (2^13)
}

static inline void __Fill_xmm_10B_YUV_16B_RGB_Params(int nColorimetry, __Xmm_PS_YUV_RGB_Params *CovParams_xmm)
{
	CovParams_xmm->xmm_Y		= _mm_set1_ps((float)(NVC_YUY2_Y)); 
	CovParams_xmm->xmm_U_G	= _mm_set1_ps((float)(NVC_YUY2_U_G(nColorimetry)));
	CovParams_xmm->xmm_U_B	= _mm_set1_ps((float)(NVC_YUY2_U_B(nColorimetry)));
	CovParams_xmm->xmm_V_G	= _mm_set1_ps((float)(NVC_YUY2_V_G(nColorimetry)));
	CovParams_xmm->xmm_V_R	= _mm_set1_ps((float)(NVC_YUY2_V_R(nColorimetry)));
}

static inline void __Fill_xmm_10B_YUV_32B_RGB_Params(int nColorimetry, __Xmm_PS_YUV_RGB_Params *CovParams_xmm)
{
	CovParams_xmm->xmm_Y = _mm_set1_ps((float)NVC_YUY2_Y / 1023);
	CovParams_xmm->xmm_U_G = _mm_set1_ps((float)NVC_YUY2_U_G(nColorimetry) / 1023);
	CovParams_xmm->xmm_U_B = _mm_set1_ps((float)NVC_YUY2_U_B(nColorimetry) / 1023);
	CovParams_xmm->xmm_V_G = _mm_set1_ps((float)NVC_YUY2_V_G(nColorimetry) / 1023);
	CovParams_xmm->xmm_V_R = _mm_set1_ps((float)NVC_YUY2_V_R(nColorimetry) / 1023);
}

static inline void __Fill_Xmm_32B_RGB_10B_YUV_Params(int nColorimetry, __Xmm_PS_RGB_YUV_Params *CovParams_xmm)
{
	CovParams_xmm->xmm_R_Y = _mm_set1_ps((float)NVC_YUY2_R_Y(nColorimetry) * 1023);
	CovParams_xmm->xmm_G_Y = _mm_set1_ps((float)NVC_YUY2_G_Y(nColorimetry) * 1023);
	CovParams_xmm->xmm_B_Y = _mm_set1_ps((float)NVC_YUY2_B_Y(nColorimetry) * 1023);
	CovParams_xmm->xmm_R_U = _mm_set1_ps((float)NVC_YUY2_R_U(nColorimetry) * 1023);
	CovParams_xmm->xmm_G_U = _mm_set1_ps((float)NVC_YUY2_G_U(nColorimetry) * 1023);
	CovParams_xmm->xmm_B_U = _mm_set1_ps((float)NVC_YUY2_B_U(nColorimetry) * 1023);
	CovParams_xmm->xmm_R_V = _mm_set1_ps((float)NVC_YUY2_R_V(nColorimetry) * 1023);
	CovParams_xmm->xmm_G_V = _mm_set1_ps((float)NVC_YUY2_G_V(nColorimetry) * 1023);
	CovParams_xmm->xmm_B_V = _mm_set1_ps((float)NVC_YUY2_B_V(nColorimetry) * 1023);
}

static inline void __Fill_Xmm128_8B_YUV422P_8B_RGB_Params(int nColorimetry, __Xmm_YUV_RGB_Params *params)
{
	params->xmm_Y = _mm_set1_epi16((short)(NVC_YUY2_Y * 8192));
	params->xmm_U_G = _mm_set1_epi16((short)(NVC_YUY2_U_G(nColorimetry) * 8192));
	params->xmm_U_B = _mm_set1_epi16((short)(NVC_YUY2_U_B(nColorimetry) * 8192));
	params->xmm_V_G = _mm_set1_epi16((short)(NVC_YUY2_V_G(nColorimetry) * 8192));
	params->xmm_V_R = _mm_set1_epi16((short)(NVC_YUY2_V_R(nColorimetry) * 8192));
}

static inline void __Fill_Xmm128_YUV_ColorimetryConvert_Params(int nSrcColorimetry, int nDstColorimetry, __Xmm128_PS_YUV_ColorimetryConvert_Params *params)
{
	params->xmm_Y_U = _mm_set1_ps((float)NVC_YUV422_CONV_COLORIMETRY_Y_U(nSrcColorimetry, nDstColorimetry));
	params->xmm_Y_V = _mm_set1_ps((float)NVC_YUV422_CONV_COLORIMETRY_Y_V(nSrcColorimetry, nDstColorimetry));
	params->xmm_U_U = _mm_set1_ps((float)NVC_YUV422_CONV_COLORIMETRY_U_U(nSrcColorimetry, nDstColorimetry));
	params->xmm_U_V = _mm_set1_ps((float)NVC_YUV422_CONV_COLORIMETRY_U_V(nSrcColorimetry, nDstColorimetry));
	params->xmm_V_U = _mm_set1_ps((float)NVC_YUV422_CONV_COLORIMETRY_V_U(nSrcColorimetry, nDstColorimetry));
	params->xmm_V_V = _mm_set1_ps((float)NVC_YUV422_CONV_COLORIMETRY_V_V(nSrcColorimetry, nDstColorimetry));
}

#define	__INT_MUL_8BIT(a, b) (((uint32_t)(a) * (uint32_t)(b) + 127) / (uint32_t)255)
static uint8_t __CompAlphaChannelNormal8Bit_YUV(
	uint8_t cb,	// bottom color
	uint8_t ab,	// bottom alpha
	uint8_t ct,	// top color
	uint8_t at)	// top alpha
{
	uint32_t tmp;
	tmp = __INT_MUL_8BIT(cb, ab);
	tmp = __INT_MUL_8BIT(tmp, 255 - at);
	tmp += __INT_MUL_8BIT(ct, at);
	if (tmp < 16)
		tmp = ct;

	return tmp <= 255 ? (uint8_t)tmp : 255;
}

#define	__INT_MUL_16BIT(a, b)	(((uint32_t)(a) * (uint32_t)(b) + 32767) / (uint32_t)65535)
// Cr = (Cb*Ab*(1 - At) + Ct*At) / Ar
static uint16_t __CompAlphaChannelNormal10Bit_YUV(uint16_t cb,	// bottom color
	uint16_t ab,	// bottom alpha
	uint16_t ct,	// top color
	uint16_t at)	// top alpha
{
	uint32_t tmp;

	tmp = __INT_MUL_16BIT(cb, ab);
	tmp = __INT_MUL_16BIT(tmp, 65535 - at);
	tmp += __INT_MUL_16BIT(ct, at);
	return tmp <= 65535 ? (uint16_t)tmp : 65535;
}

static inline __m128 __CompositelAphaColorChannel_SSE(
	__m128& xmm_back_ps, __m128& xmm_back_alpha_ps,
	__m128& xmm_front_ps, __m128& xmm_front_alpha_ps,
	const __m128& xmm_result_alpha_ps, const __m128& xmm_max_ps)
{
	//
	// composite color channels: Cr = (Cb*Ab*(1 - At) + Ct*At) / Ar
	//
	__m128 xmm_mask_ps = _mm_cmpneq_ps(xmm_result_alpha_ps, _mm_setzero_ps());
	__m128 xmm_front_ps_tmp = xmm_front_ps;

	xmm_back_ps = _mm_mul_ps(xmm_back_ps, xmm_back_alpha_ps);
	xmm_front_ps = _mm_sub_ps(xmm_front_ps, xmm_back_ps);
	xmm_front_ps = _mm_mul_ps(xmm_front_ps, xmm_front_alpha_ps);
	xmm_front_ps = _mm_add_ps(xmm_front_ps, xmm_back_ps);
	xmm_front_ps = _mm_div_ps(xmm_front_ps, xmm_result_alpha_ps);
	xmm_front_ps = _mm_and_ps(xmm_front_ps, xmm_mask_ps);

	__m128 xmm_front_mask_ps = _mm_cmpeq_ps(xmm_front_ps, _mm_setzero_ps());
	xmm_front_ps_tmp = _mm_and_ps(xmm_front_ps_tmp, xmm_front_mask_ps);
	xmm_front_ps = _mm_andnot_ps(xmm_front_mask_ps, xmm_front_ps);

	return _mm_or_ps(xmm_front_ps, xmm_front_ps_tmp);
}

static inline __m128i __PlannarPreMultiplyAlpha_8Bit_SSE(const __m128i& xmm_element, const __m128i& xmm_mul, bool bUVChannel)
{
	if (_mm_movemask_epi8(_mm_cmpeq_epi8(xmm_mul, _mm_setzero_si128())) == 0xFFFF)
		return (bUVChannel ? _mm_set1_epi16(128) : _mm_set1_epi16(16));
	else if (_mm_movemask_epi8(_mm_cmpeq_epi8(xmm_mul, _mm_set1_epi16(255))) == 0xFFFF)
		return xmm_element;

	__m128 xmm_back_ps = bUVChannel ? g_xmm_128_ps : g_xmm_16_ps;
	__m128i xmm_zero = _mm_setzero_si128();
	__m128 xmm_back_ps_tmp = xmm_back_ps;
	__m128  xmm_1_ps = g_xmm_1_ps;

	__m128i xmm_element_tmp1, xmm_element_tmp2, xmm_mul_tmp;
	__m128 xmm_element_ps, xmm_mul_ps;

	xmm_element_tmp1 = _mm_unpacklo_epi16(xmm_element, xmm_zero);
	xmm_mul_tmp = _mm_unpacklo_epi16(xmm_mul, xmm_zero);

	xmm_element_ps = _mm_cvtepi32_ps(xmm_element_tmp1);
	xmm_mul_ps = _mm_cvtepi32_ps(xmm_mul_tmp);
	xmm_mul_ps = _mm_div_ps(xmm_mul_ps, g_xmm_255_ps);

	xmm_element_ps = __CompositelAphaColorChannel_SSE(
		xmm_back_ps_tmp, xmm_1_ps,
		xmm_element_ps, xmm_mul_ps,
		g_xmm_1_ps, g_xmm_255_ps);
	xmm_element_tmp1 = _mm_cvttps_epi32(xmm_element_ps);
	xmm_element_tmp2 = _mm_unpackhi_epi16(xmm_element, xmm_zero);
	xmm_mul_tmp = _mm_unpackhi_epi16(xmm_mul, xmm_zero);
	xmm_element_ps = _mm_cvtepi32_ps(xmm_element_tmp2);

	xmm_back_ps_tmp = xmm_back_ps;
	xmm_element_ps = __CompositelAphaColorChannel_SSE(
		xmm_back_ps_tmp, xmm_1_ps,
		xmm_element_ps, xmm_mul_ps,
		g_xmm_1_ps, g_xmm_255_ps);

	xmm_element_tmp2 = _mm_cvttps_epi32(xmm_element_ps);
	return _mm_packus_epi32(xmm_element_tmp1, xmm_element_tmp2);
}

static inline __m128i __PlannarPreMultiplyAlpha_10Bit_SSE(const __m128i& xmm_element, const __m128i& xmm_mul, bool bUVChannel)
{
	if (_mm_movemask_epi8(_mm_cmpeq_epi8(xmm_mul, _mm_setzero_si128())) == 0xFFFF)
		return (bUVChannel ? _mm_set1_epi16(512) : _mm_set1_epi16(64));
	else if (_mm_movemask_epi8(_mm_cmpeq_epi8(xmm_mul, _mm_set1_epi16((unsigned short)65535))) == 0xFFFF)
		return xmm_element;

	__m128 xmm_back_ps = bUVChannel ? g_xmm_512_ps : g_xmm_64_ps;

	__m128i xmm_zero = _mm_setzero_si128();
	__m128 xmm_back_ps_tmp = xmm_back_ps;
	__m128  xmm_1_ps = g_xmm_1_ps;

	__m128i xmm_element_tmp1, xmm_element_tmp2, xmm_mul_tmp;
	__m128 xmm_element_ps, xmm_mul_ps;

	xmm_element_tmp1 = _mm_unpacklo_epi16(xmm_element, xmm_zero);
	xmm_mul_tmp = _mm_unpacklo_epi16(xmm_mul, xmm_zero);

	xmm_element_ps = _mm_cvtepi32_ps(xmm_element_tmp1);
	xmm_mul_ps = _mm_cvtepi32_ps(xmm_mul_tmp);
	xmm_mul_ps = _mm_div_ps(xmm_mul_ps, g_xmm_65535_ps);

	xmm_element_ps = __CompositelAphaColorChannel_SSE(
		xmm_back_ps_tmp, xmm_1_ps,
		xmm_element_ps, xmm_mul_ps,
		g_xmm_1_ps, g_xmm_65535_ps);
	xmm_element_tmp1 = _mm_cvttps_epi32(xmm_element_ps);
	xmm_element_tmp2 = _mm_unpackhi_epi16(xmm_element, xmm_zero);
	xmm_mul_tmp = _mm_unpackhi_epi16(xmm_mul, xmm_zero);
	xmm_element_ps = _mm_cvtepi32_ps(xmm_element_tmp2);

	xmm_back_ps_tmp = xmm_back_ps;
	xmm_element_ps = __CompositelAphaColorChannel_SSE(
		xmm_back_ps_tmp, xmm_1_ps,
		xmm_element_ps, xmm_mul_ps,
		g_xmm_1_ps, g_xmm_65535_ps);

	xmm_element_tmp2 = _mm_cvttps_epi32(xmm_element_ps);
	return _mm_packus_epi32(xmm_element_tmp1, xmm_element_tmp2);
}

//////////////////////////////////////////////////////////////////////////
// some global variables
//////////////////////////////////////////////////////////////////////////
static const __m128i g_xmm_YUAYVA422410bits_mask = _mm_set1_epi32((int)0x000003FF);
static const __m128i g_xmm_scale_8bit_to_10bit_mask = _mm_set1_epi16((unsigned short)0x8060);  // 1023 / 255 * 2^13 (the mask unit is 2^15)
static const __m128i g_xmm_16bit_rounding_1 = _mm_set1_epi16((short)0x0001);
static const __m128i g_xmm_scale_10bit_to_8bit_mask = _mm_set1_epi16((unsigned short)0x1FE7); // 255 / 1023 * 2^15

static const __m128i g_xmm_scale_10bit_to_16bit_mask = _mm_set1_epi16((unsigned short)0x801F);  // 65535 / 1023 * 2^9 (the mask unit is 2^15)
static const __m128i g_xmm_scale_16bit_to_10bit_mask = _mm_set1_epi16((unsigned short)0x01FF); // 1023 / 65535 * 2^15

static const __m128i g_xmm_yuyv_10bit_black_y = _mm_set1_epi16((short)0x40); // (16 << 2)
static const __m128i g_xmm_yuyv_10bit_black_uv = _mm_set1_epi16((short)0x200); // (128 << 2)
static const __m128i g_xmm_yuyv_10bit_max_value = _mm_set1_epi16((short)0x3FF);

static const __m128i g_xmm_16bit_4 = _mm_set1_epi16(4);
static const __m128i g_xmm_16bit_8 = _mm_set1_epi16(8);
static const __m128i g_xmm_16bit_16 = _mm_set1_epi16(16);
static const __m128i g_xmm_32bit_64 = _mm_set1_epi32(64);
static const __m128i g_xmm_16bit_257 = _mm_set1_epi16(257);
static const __m128i g_xmm_16bit_4096 = _mm_set1_epi16(4096);

static const __m128i g_xmm_16bit_512 = _mm_set1_epi16(512);
static const __m128i g_xmm_32bit_512 = _mm_set1_epi32(512);
static const __m128 g_xmm_ps_64 = _mm_set1_ps(65535/1023.0f);

static const __m128i g_xmm_1s = _mm_set1_epi16((unsigned short)0xFF);
static const __m128i g_xmm_keep_uv422_mask = _mm_set1_epi32((int)0x0000FFFF);
static const __m128i g_xmm_keep_low_2_bit_mask = _mm_set1_epi16((short)0x0003);
static const __m128i g_xmm_keep_byte0_mask = _mm_set1_epi32((int)0x000000FF);
static const __m128i g_xmm_2_bit_mask = _mm_set_epi8((char)0x30, (char)0x30, (char)0x30, (char)0x30, (char)0x30, (char)0x30, (char)0x30, (char)0x30, (char)0x03, (char)0x03, (char)0x03, (char)0x03, (char)0x03, (char)0x03, (char)0x03, (char)0x03);
//static const __m128i g_xmm_even_byte_mask = _mm_set_epi8((char)0x00, (char)0xFF, (char)0x00, (char)0xFF, (char)0x00, (char)0xFF, (char)0x00, (char)0xFF, (char)0x00, (char)0xFF, (char)0x00, (char)0xFF, (char)0x00, (char)0xFF, (char)0x00, (char)0xFF);
static const __m128i g_xmm_even_byte_mask = _mm_set1_epi16((short)0x00FF);

static const __m128i g_xmm_yuva_alpha_mask_16bit = _mm_set_epi16(255, 255, 255, 255, 255, 255, 255, 255);
static const __m128i g_xmm_1_div_255 = _mm_set1_epi16((unsigned short)((1.0 / 255) * 65536));

static const __m128i g_xmm_1 = _mm_set1_epi16(1);

static const __m128i g_xmm_16bit_opaque_alpha_8b = _mm_set1_epi16((unsigned short)0xFF);
static const __m128i g_xmm_16bit_opaque_alpha_16b = _mm_set1_epi16((unsigned short)0xFFFF);
static const __m128i g_xmm_yuyv_black = _mm_set1_epi32((int)0x80108010);
static const __m128i g_xmm_uyvy_black = _mm_set1_epi32((int)0x10801080);

static const __m128 g_xmm_rgb_upper_limit_ps = _mm_set1_ps(10);
static const __m128 g_xmm_rgb_lower_limit_ps = _mm_set1_ps(-10);

static const __m128i g_xmm_y_black = _mm_set1_epi32((int)0x10101010);
static const __m128i g_xmm_uv_black = _mm_set1_epi32((int)0x80808080);

static const __m128i g_xmm_mask_lo = _mm_set_epi32(0, 0, -1, -1);
static const __m128i g_xmm_mask_hi = _mm_set_epi32(-1, -1, 0, 0);

//////////////////////////////////////////////////////////////////////////
// colorimetry convert
//////////////////////////////////////////////////////////////////////////
// calculate y colorimetry convert
template<typename T>
inline T NVC_Y_ColorimetryConvert(T y, T u, T v, int nSrcColorimetry, int nDstColorimetry)
{
	assert(sizeof(T) == 1 || sizeof(T) == 2);
	uint8_t nBitDepth = (sizeof(T) == 1) ? 8 : 10;
	double Cyu = NVC_YUV422_CONV_COLORIMETRY_Y_U(nSrcColorimetry, nDstColorimetry);
	double Cyv = NVC_YUV422_CONV_COLORIMETRY_Y_V(nSrcColorimetry, nDstColorimetry);
	return (T)(y + Cyu * u + Cyv * v - (Cyu + Cyv)*(pow(2, (nBitDepth - 1))));
}

// calculate u colorimetry convert
template<typename T>
inline T NVC_U_ColorimetryConvert(T u, T v, int nSrcColorimetry, int nDstColorimetry)
{
	assert(sizeof(T) == 1 || sizeof(T) == 2);
	uint8_t nBitDepth = (sizeof(T) == 1) ? 8 : 10;
	double Cuu = NVC_YUV422_CONV_COLORIMETRY_U_U(nSrcColorimetry, nDstColorimetry);
	double Cuv = NVC_YUV422_CONV_COLORIMETRY_U_V(nSrcColorimetry, nDstColorimetry);
	return (T)(Cuu * u + Cuv * v - (Cuu + Cuv - 1)*(pow(2, (nBitDepth - 1))));
}

// calculate v colorimetry convert
template<typename T>
inline T NVC_V_ColorimetryConvert(T u, T v, int nSrcColorimetry, int nDstColorimetry)
{
	assert(sizeof(T) == 1 || sizeof(T) == 2);
	uint8_t nBitDepth = (sizeof(T) == 1) ? 8 : 10;
	double Cvu = NVC_YUV422_CONV_COLORIMETRY_V_U(nSrcColorimetry, nDstColorimetry);
	double Cvv = NVC_YUV422_CONV_COLORIMETRY_V_V(nSrcColorimetry, nDstColorimetry);
	return (T)(Cvu * u + Cvv * v - (Cvu + Cvv - 1)*(pow(2, (nBitDepth - 1))));
}