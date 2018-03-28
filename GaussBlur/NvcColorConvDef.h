#pragma once

#include <NvcVisionCoreDef.h>

#include <math.h>


/*
* color space format specification
*
* 1) BGRA
* b0 | g0 | r0 | a0 | b1 | g1 | r1 | a1 ...
*
* 2) ARGB
* a0 | r0 | g0 | b0 | a1 | r1 | g1 | b1 ...
*
* 3) BGR
* b0 | g0 | r0 | b1 | g1 | r1 ...
*
* 4) RGB
* r0 | g0 | b0 | r1 | g1 | b1 ...
*
* 5) YUAYVA4224(8 bit)
* y0 | u0 | y1 | v0 | y2 | u2 | y3 | v2 | y4 | u4 | y5 | v4 | y6 | u6 | y7 | v6 | a0 | a1 | a2 | a3 | a4 | a5 | a6 | a7 | ...
* |_____________________________________|
*                 4 pairs(8 pixels)
*
* 6) YUV422(8 bit)
* y0 | u0 | y1 | v0 | y2 | u2 | y3 | v2 ...
*
* 7) YUAYVA4224(10 bit)
* xx | a0(10 bit) | u0(10 bit) | y0(10 bit)
* |_______________________________________|
* MSB            32 bits                LSB
*
* xx | a1(10 bit) | v0(10 bit) | y1(10 bit)
* |_______________________________________|
* MSB            32 bits                LSB
*
* 8) YUV422(10 bit)
* It's too complex to be explained here.
*
* 9) UYVY
* u0 | y0 | v0 | y1 | u2 | y2 | v2 | y3 ...
*
*/

#define NVC_BT601               0
#define NVC_BT709               1
#define NVC_BT2020              2

static inline int NvcGetColorimetry(ENvcColorimetry eColorimetry)
{
	int nColorimetry = NVC_BT601;
	switch (eColorimetry)
	{
	case keNvcColorimetryITUR_BT_601:
		nColorimetry = NVC_BT601;
		break;
	case keNvcColorimetryITUR_BT_709:
		nColorimetry = NVC_BT709;
		break;
	case keNvcColorimetryITUR_BT_2020:
		nColorimetry = NVC_BT2020;
		break;
	default:
		break;
	}
	return nColorimetry;
}

static inline ENvcColorimetry NvcGetColorimetry(int nColorimetry)
{
	ENvcColorimetry eColorimetry = keNvcColorimetryInvalid;
	switch (nColorimetry)
	{
	case NVC_BT601:
		eColorimetry = keNvcColorimetryITUR_BT_601;
		break;
	case NVC_BT709:
		eColorimetry = keNvcColorimetryITUR_BT_709;
		break;
	case NVC_BT2020:
		eColorimetry = keNvcColorimetryITUR_BT_2020;
		break;
	default:
		break;
	}
	return eColorimetry;
}

//
// The following coefficients is for 8 Bit RGB <--> YUV conversion
//
#define NVC_YUY2_R_Y_601		0.25679
#define	NVC_YUY2_G_Y_601		0.50413
#define	NVC_YUY2_B_Y_601		0.09791

#define NVC_YUY2_R_U_601		0.14822
#define	NVC_YUY2_G_U_601		0.29099
#define	NVC_YUY2_B_U_601		0.43922

#define NVC_YUY2_R_V_601		0.43922
#define	NVC_YUY2_G_V_601		0.36779
#define	NVC_YUY2_B_V_601		0.07142

#define NVC_YUY2_R_Y_709		0.18259
#define	NVC_YUY2_G_Y_709		0.61423
#define	NVC_YUY2_B_Y_709		0.06201

#define NVC_YUY2_R_U_709		0.10065
#define	NVC_YUY2_G_U_709		0.33857
#define	NVC_YUY2_B_U_709		0.43922

#define NVC_YUY2_R_V_709		0.43922
#define	NVC_YUY2_G_V_709		0.39894
#define	NVC_YUY2_B_V_709		0.04027

#define NVC_YUY2_R_Y_2020		0.22561
#define NVC_YUY2_G_Y_2020		0.58228
#define NVC_YUY2_B_Y_2020		0.05093

#define NVC_YUY2_R_U_2020		0.12266
#define NVC_YUY2_G_U_2020		0.31656
#define NVC_YUY2_B_U_2020		0.43922

#define NVC_YUY2_R_V_2020		0.43922
#define NVC_YUY2_G_V_2020		0.40389
#define NVC_YUY2_B_V_2020		0.03532

#define NVC_YUY2_R_Y(colorimetry)	((colorimetry) == NVC_BT2020 ? (NVC_YUY2_R_Y_2020) : ((colorimetry) == NVC_BT709 ? (NVC_YUY2_R_Y_709) : (NVC_YUY2_R_Y_601)))
#define NVC_YUY2_G_Y(colorimetry)	((colorimetry) == NVC_BT2020 ? (NVC_YUY2_G_Y_2020) : ((colorimetry) == NVC_BT709 ? (NVC_YUY2_G_Y_709) : (NVC_YUY2_G_Y_601)))
#define NVC_YUY2_B_Y(colorimetry)	((colorimetry) == NVC_BT2020 ? (NVC_YUY2_B_Y_2020) : ((colorimetry) == NVC_BT709 ? (NVC_YUY2_B_Y_709) : (NVC_YUY2_B_Y_601)))

#define NVC_YUY2_R_U(colorimetry)	((colorimetry) == NVC_BT2020 ? (NVC_YUY2_R_U_2020) : ((colorimetry) == NVC_BT709 ? (NVC_YUY2_R_U_709) : (NVC_YUY2_R_U_601)))
#define NVC_YUY2_G_U(colorimetry)	((colorimetry) == NVC_BT2020 ? (NVC_YUY2_G_U_2020) : ((colorimetry) == NVC_BT709 ? (NVC_YUY2_G_U_709) : (NVC_YUY2_G_U_601)))
#define NVC_YUY2_B_U(colorimetry)	((colorimetry) == NVC_BT2020 ? (NVC_YUY2_B_U_2020) : ((colorimetry) == NVC_BT709 ? (NVC_YUY2_B_U_709) : (NVC_YUY2_B_U_601)))

#define NVC_YUY2_R_V(colorimetry)	((colorimetry) == NVC_BT2020 ? (NVC_YUY2_R_V_2020) : ((colorimetry) == NVC_BT709 ? (NVC_YUY2_R_V_709) : (NVC_YUY2_R_V_601)))
#define NVC_YUY2_G_V(colorimetry)	((colorimetry) == NVC_BT2020 ? (NVC_YUY2_G_V_2020) : ((colorimetry) == NVC_BT709 ? (NVC_YUY2_G_V_709) : (NVC_YUY2_G_V_601)))
#define NVC_YUY2_B_V(colorimetry)	((colorimetry) == NVC_BT2020 ? (NVC_YUY2_B_V_2020) : ((colorimetry) == NVC_BT709 ? (NVC_YUY2_B_V_709) : (NVC_YUY2_B_V_601)))


#define NVC_YUY2_Y				1.16438 // Both the coefficients of Y to R,G,B in 601, 709 and 2020 standards are the same

#define NVC_YUY2_U_G_601		0.39176
#define NVC_YUY2_U_B_601		2.01723
#define NVC_YUY2_V_R_601		1.59602
#define NVC_YUY2_V_G_601		0.81297

#define NVC_YUY2_U_G_709		0.21325
#define NVC_YUY2_U_B_709		2.11240
#define NVC_YUY2_V_R_709		1.79275
#define NVC_YUY2_V_G_709		0.53291

#define NVC_YUY2_U_G_2020		0.18733
#define NVC_YUY2_U_B_2020		2.14177
#define NVC_YUY2_V_R_2020		1.67867
#define NVC_YUY2_V_G_2020		0.65042

#define NVC_YUY2_U_G(colorimetry)	((colorimetry) == NVC_BT2020 ? (NVC_YUY2_U_G_2020) : ((colorimetry) == NVC_BT709 ? (NVC_YUY2_U_G_709) : (NVC_YUY2_U_G_601)))
#define NVC_YUY2_U_B(colorimetry)	((colorimetry) == NVC_BT2020 ? (NVC_YUY2_U_B_2020) : ((colorimetry) == NVC_BT709 ? (NVC_YUY2_U_B_709) : (NVC_YUY2_U_B_601)))
#define NVC_YUY2_V_R(colorimetry)	((colorimetry) == NVC_BT2020 ? (NVC_YUY2_V_R_2020) : ((colorimetry) == NVC_BT709 ? (NVC_YUY2_V_R_709) : (NVC_YUY2_V_R_601)))
#define NVC_YUY2_V_G(colorimetry)	((colorimetry) == NVC_BT2020 ? (NVC_YUY2_V_G_2020) : ((colorimetry) == NVC_BT709 ? (NVC_YUY2_V_G_709) : (NVC_YUY2_V_G_601)))


#define NVC_LOGTOLINEAR_BLACKREF	95
#define NVC_LOGTOLINEAR_WHITEREF	685


//conver yuv 601 to 709
#define NVC_YUV422_601_TO_709_Y_U		-0.11554
#define NVC_YUV422_601_TO_709_Y_V		-0.20793
#define NVC_YUV422_601_TO_709_U_U		1.01865
#define NVC_YUV422_601_TO_709_U_V		0.11461
#define NVC_YUV422_601_TO_709_V_U		0.07506
#define NVC_YUV422_601_TO_709_V_V		1.02533

//conver yuv 709 to 601
#define NVC_YUV422_709_TO_601_Y_U		0.09932
#define NVC_YUV422_709_TO_601_Y_V		0.19170
#define NVC_YUV422_709_TO_601_U_U		0.98986
#define NVC_YUV422_709_TO_601_U_V		-0.11065
#define NVC_YUV422_709_TO_601_V_U		-0.07244
#define NVC_YUV422_709_TO_601_V_V		0.98341

//convert yuv 601 to 2020
#define NVC_YUV422_601_TO_2020_Y_U		-0.12538
#define NVC_YUV422_601_TO_2020_Y_V		-0.11329
#define NVC_YUV422_601_TO_2020_U_U		1.01002
#define NVC_YUV422_601_TO_2020_U_V		0.06159
#define NVC_YUV422_601_TO_2020_V_U		0.08697
#define NVC_YUV422_601_TO_2020_V_V		1.02935

//conver yuv 2020 to 601
#define NVC_YUV422_2020_TO_601_Y_U		0.11526
#define NVC_YUV422_2020_TO_601_Y_V		0.10316
#define NVC_YUV422_2020_TO_601_U_U		0.99521
#define NVC_YUV422_2020_TO_601_U_V		-0.05955
#define NVC_YUV422_2020_TO_601_V_U		-0.08408
#define NVC_YUV422_2020_TO_601_V_V		0.97652

//convert yuv 709 to 2020
#define NVC_YUV422_709_TO_2020_Y_U		-0.01659
#define NVC_YUV422_709_TO_2020_Y_V		0.09416
#define NVC_YUV422_709_TO_2020_U_U		0.99531
#define NVC_YUV422_709_TO_2020_U_V		-0.05119
#define NVC_YUV422_709_TO_2020_V_U		0.01151
#define NVC_YUV422_709_TO_2020_V_V		1.00264

//conver yuv 2020 to 709
#define NVC_YUV422_2020_TO_709_Y_U		0.01774
#define NVC_YUV422_2020_TO_709_Y_V		-0.09300
#define NVC_YUV422_2020_TO_709_U_U		1.00412
#define NVC_YUV422_2020_TO_709_U_V		0.05127
#define NVC_YUV422_2020_TO_709_V_U		-0.01152
#define NVC_YUV422_2020_TO_709_V_V		0.99678


#define NVC_YUV422_CONV_COLORIMETRY_Y_U(from, to)	(((from) == NVC_BT2020) ? \
														((to) == NVC_BT709 ? (NVC_YUV422_2020_TO_709_Y_U) : (NVC_YUV422_2020_TO_601_Y_U)) \
													: (((from) == NVC_BT709) ? \
														((to) == NVC_BT2020 ? (NVC_YUV422_709_TO_2020_Y_U) : (NVC_YUV422_709_TO_601_Y_U)) \
													 :	((to) == NVC_BT2020 ? (NVC_YUV422_601_TO_2020_Y_U) : (NVC_YUV422_601_TO_709_Y_U))))

#define NVC_YUV422_CONV_COLORIMETRY_Y_V(from, to)	(((from) == NVC_BT2020) ? \
														((to) == NVC_BT709 ? (NVC_YUV422_2020_TO_709_Y_V) : (NVC_YUV422_2020_TO_601_Y_V)) \
													: (((from) == NVC_BT709) ? \
														((to) == NVC_BT2020 ? (NVC_YUV422_709_TO_2020_Y_V) : (NVC_YUV422_709_TO_601_Y_V)) \
													 :	((to) == NVC_BT2020 ? (NVC_YUV422_601_TO_2020_Y_V) : (NVC_YUV422_601_TO_709_Y_V))))

#define NVC_YUV422_CONV_COLORIMETRY_U_U(from, to)	(((from) == NVC_BT2020) ? \
														((to) == NVC_BT709 ? (NVC_YUV422_2020_TO_709_U_U) : (NVC_YUV422_2020_TO_601_U_U)) \
													: (((from) == NVC_BT709) ? \
														((to) == NVC_BT2020 ? (NVC_YUV422_709_TO_2020_U_U) : (NVC_YUV422_709_TO_601_U_U)) \
													 :	((to) == NVC_BT2020 ? (NVC_YUV422_601_TO_2020_U_U) : (NVC_YUV422_601_TO_709_U_U))))

#define NVC_YUV422_CONV_COLORIMETRY_U_V(from, to)	(((from) == NVC_BT2020) ? \
														((to) == NVC_BT709 ? (NVC_YUV422_2020_TO_709_U_V) : (NVC_YUV422_2020_TO_601_U_V)) \
													: (((from) == NVC_BT709) ? \
														((to) == NVC_BT2020 ? (NVC_YUV422_709_TO_2020_U_V) : (NVC_YUV422_709_TO_601_U_V)) \
													 :	((to) == NVC_BT2020 ? (NVC_YUV422_601_TO_2020_U_V) : (NVC_YUV422_601_TO_709_U_V))))

#define NVC_YUV422_CONV_COLORIMETRY_V_U(from, to)	(((from) == NVC_BT2020) ? \
														((to) == NVC_BT709 ? (NVC_YUV422_2020_TO_709_V_U) : (NVC_YUV422_2020_TO_601_V_U)) \
													: (((from) == NVC_BT709) ? \
														((to) == NVC_BT2020 ? (NVC_YUV422_709_TO_2020_V_U) : (NVC_YUV422_709_TO_601_V_U)) \
													 :	((to) == NVC_BT2020 ? (NVC_YUV422_601_TO_2020_V_U) : (NVC_YUV422_601_TO_709_V_U))))

#define NVC_YUV422_CONV_COLORIMETRY_V_V(from, to)	(((from) == NVC_BT2020) ? \
														((to) == NVC_BT709 ? (NVC_YUV422_2020_TO_709_V_V) : (NVC_YUV422_2020_TO_601_V_V)) \
													: (((from) == NVC_BT709) ? \
														((to) == NVC_BT2020 ? (NVC_YUV422_709_TO_2020_V_V) : (NVC_YUV422_709_TO_601_V_V)) \
													 :	((to) == NVC_BT2020 ? (NVC_YUV422_601_TO_2020_V_V) : (NVC_YUV422_601_TO_709_V_V))))


// calculate y from 8bit r, g, b
inline uint8_t NVC_Y_FROM_8Bit_RGB(uint8_t r, uint8_t g, uint8_t b, int nColorimetry)
{
	unsigned int y;

	y = r * (unsigned int)(NVC_YUY2_R_Y(nColorimetry) * 65536);
	y += g * (unsigned int)(NVC_YUY2_G_Y(nColorimetry) * 65536);
	y += b * (unsigned int)(NVC_YUY2_B_Y(nColorimetry) * 65536);

	y = (y + 32768) >> 16;
	return (uint8_t)(y + 16);
}

// calculate u from 8bit r, g, b
inline uint8_t NVC_U_FROM_8Bit_RGB(uint8_t r, uint8_t g, uint8_t b, int nColorimetry)
{
	int u;

	u = (int)b * (int)(NVC_YUY2_B_U(nColorimetry) * 65536);
	u -= (int)r * (int)(NVC_YUY2_R_U(nColorimetry) * 65536);
	u -= (int)g * (int)(NVC_YUY2_G_U(nColorimetry) * 65536);

	u = (u + 32768) >> 16;
	return (uint8_t)(u + 128);
}

// calculate v from 8bit r, g, b
inline uint8_t NVC_V_FROM_8Bit_RGB(uint8_t r, uint8_t g, uint8_t b, int nColorimetry)
{
	int v;

	v = (int)r * (int)(NVC_YUY2_R_V(nColorimetry) * 65536);
	v -= (int)g * (int)(NVC_YUY2_G_V(nColorimetry) * 65536);
	v -= (int)b * (int)(NVC_YUY2_B_V(nColorimetry) * 65536);

	v = (v + 32768) >> 16;
	return (uint8_t)(v + 128);
}

// calculate y from 16bit r, g, b
inline uint8_t NVC_Y_FROM_16Bit_RGB(uint16_t r, uint16_t g, uint16_t b, int nColorimetry)
{
	unsigned int red, green, blue, y;

	red = ((unsigned int)r + 128) / (unsigned int)257;
	green = ((unsigned int)g + 128) / (unsigned int)257;
	blue = ((unsigned int)b + 128) / (unsigned int)257;

	y = red * (unsigned int)(NVC_YUY2_R_Y(nColorimetry) * 65536);
	y += green * (unsigned int)(NVC_YUY2_G_Y(nColorimetry) * 65536);
	y += blue * (unsigned int)(NVC_YUY2_B_Y(nColorimetry) * 65536);

	y = (y + 32768) >> 16;
	return (uint8_t)(y + 16);
}

// calculate u from 16bit r, g, b
inline uint8_t NVC_U_FROM_16Bit_RGB(uint16_t r, uint16_t g, uint16_t b, int nColorimetry)
{
	int red, green, blue, u;

	red = ((unsigned int)r + 128) / (unsigned int)257;
	green = ((unsigned int)g + 128) / (unsigned int)257;
	blue = ((unsigned int)b + 128) / (unsigned int)257;

	u = blue * (int)(NVC_YUY2_B_U(nColorimetry) * 65536);
	u -= red * (int)(NVC_YUY2_R_U(nColorimetry) * 65536);
	u -= green * (int)(NVC_YUY2_G_U(nColorimetry) * 65536);

	u = (u + 32768) >> 16;
	return (uint8_t)(u + 128);
}

// calculate v from 16bit r, g, b
inline uint8_t NVC_V_FROM_16Bit_RGB(uint16_t r, uint16_t g, uint16_t b, int nColorimetry)
{
	int red, green, blue, v;

	red = ((unsigned int)r + 128) / (unsigned int)257;
	green = ((unsigned int)g + 128) / (unsigned int)257;
	blue = ((unsigned int)b + 128) / (unsigned int)257;

	v = red * (int)(NVC_YUY2_R_V(nColorimetry) * 65536);
	v -= green * (int)(NVC_YUY2_G_V(nColorimetry) * 65536);
	v -= blue * (int)(NVC_YUY2_B_V(nColorimetry) * 65536);

	v = (v + 32768) >> 16;
	return (uint8_t)(v + 128);
}

// calculate y from 32bit r, g, b
// NOTE: before using this function, R, G, B should be limited to a reasonable range (e.g. [-10, 10])
static inline uint8_t NVC_Y_FROM_32Bit_RGB(float r, float g, float b, int nColorimetry)
{
	float y;
	y = r * (float)(NVC_YUY2_R_Y(nColorimetry) * 255);
	y += g * (float)(NVC_YUY2_G_Y(nColorimetry) * 255);
	y += b * (float)(NVC_YUY2_B_Y(nColorimetry) * 255);

	y += 16;
	y = NVC_MIN(y, 255);
	y = NVC_MAX(y, 0);
	return (uint8_t)(y + 0.5f);
}

// calculate u from 32bit r, g, b
// NOTE: before using this function, R, G, B should be limited to a reasonable range (e.g. [-10, 10])
static inline uint8_t NVC_U_FROM_32Bit_RGB(float r, float g, float b, int nColorimetry)
{
	float u;
	
	u = b * (float)(NVC_YUY2_B_U(nColorimetry) * 255);
	u -= r * (float)(NVC_YUY2_R_U(nColorimetry) * 255);
	u -= g * (float)(NVC_YUY2_G_U(nColorimetry) * 255);

	u += 128;
	u = NVC_MIN(u, 255);
	u = NVC_MAX(u, 0);
	return (uint8_t)(u + 0.5f);
}

// calculate v from 32bit r, g, b
// NOTE: before using this function, R, G, B should be limited to a reasonable range (e.g. [-10, 10])
static inline uint8_t NVC_V_FROM_32Bit_RGB(float r, float g, float b, int nColorimetry)
{
	float v;
	
	v = r * (float)(NVC_YUY2_R_V(nColorimetry) * 255);
	v -= g * (float)(NVC_YUY2_G_V(nColorimetry) * 255);
	v -= b * (float)(NVC_YUY2_B_V(nColorimetry) * 255);

	v += 128;
	v = NVC_MIN(v, 255);
	v = NVC_MAX(v, 0);
	return (uint8_t)(v + 0.5f);
}


// calculate 10bit y from 16bit r, g, b
inline uint16_t NVC_10Bit_Y_FROM_16Bit_RGB(uint16_t r, uint16_t g, uint16_t b, int nColorimetry)
{
	double y;

	y = ((double)r / 64.) * NVC_YUY2_R_Y(nColorimetry);
	y += ((double)g / 64.) * NVC_YUY2_G_Y(nColorimetry);
	y += ((double)b / 64.) * NVC_YUY2_B_Y(nColorimetry);

	return (uint16_t)((int)(y + 0.5) + (int)((int)16 << 2));
}

// calculate 10bit u from 16bit r, g, b
inline uint16_t NVC_10Bit_U_FROM_16Bit_RGB(uint16_t r, uint16_t g, uint16_t b, int nColorimetry)
{
	double u;

	u = ((double)b / 64.) * NVC_YUY2_B_U(nColorimetry);
	u -= ((double)r / 64.) * NVC_YUY2_R_U(nColorimetry);
	u -= ((double)g / 64.) * NVC_YUY2_G_U(nColorimetry);
	
	return (uint16_t)((int)(u + 0.5 + 512));
}

// calculate 10bit v from 16bit r, g, b
inline uint16_t NVC_10Bit_V_FROM_16Bit_RGB(uint16_t r, uint16_t g, uint16_t b, int nColorimetry)
{
	double v;

	v = ((double)r / 64.) * NVC_YUY2_R_V(nColorimetry);
	v -= ((double)g / 64.) * NVC_YUY2_G_V(nColorimetry);
	v -= ((double)b / 64.) * NVC_YUY2_B_V(nColorimetry);

	return (uint16_t)((int)(v + 0.5 + 512));
}

// calculate 10bit y from 32bit r, g, b
inline uint16_t NVC_10Bit_Y_FROM_32Bit_RGB(float r, float g, float b, int nColorimetry)
{
	double y;

	y = r * NVC_YUY2_R_Y(nColorimetry);
	y += g * NVC_YUY2_G_Y(nColorimetry);
	y += b * NVC_YUY2_B_Y(nColorimetry);
	
	return (uint16_t)((int)(y * 1023 + 0.5) + (int)((int)16 << 2));
}
// calculate 10bit u from 32bit r, g, b
inline uint16_t NVC_10Bit_U_FROM_32Bit_RGB(float r, float g, float b, int nColorimetry)
{
	double u;

	u = b * NVC_YUY2_B_U(nColorimetry);
	u -= r * NVC_YUY2_R_U(nColorimetry);
	u -= g * NVC_YUY2_G_U(nColorimetry);

	return (uint16_t)((int)(u * 1023 + 0.5 + 512));
}
// calculate 10bit v from 32bit r, g, b
inline uint16_t NVC_10Bit_V_FROM_32Bit_RGB(float r, float g, float b, int nColorimetry)
{
	double v;

	v = r * NVC_YUY2_R_V(nColorimetry);
	v -= g * NVC_YUY2_G_V(nColorimetry);
	v -= b * NVC_YUY2_B_V(nColorimetry);

	return (uint16_t)((int)(v * 1023 + 0.5 + 512));
}

// calculate 16 bit r from 10bit y, u, v
inline uint16_t NVC_16Bit_R_FROM_10Bit_YUV(uint16_t y, uint16_t u, uint16_t v, int nColorimetry)
{
	UNREFERENCED_PARAMETER(u);

	double r;
	int red;

	r = ((double)(y - 64) * 64.) * NVC_YUY2_Y;
	r += ((double)(v - 512) * 64.) * NVC_YUY2_V_R(nColorimetry);

	red = (int)(r + 0.5);

	if (red<0)
		red = 0;
	else if (red>65535)
		red = 65535;


	return (uint16_t)(red);
}

// calculate 16 bit g from 10bit y, u, v
inline uint16_t NVC_16Bit_G_FROM_10Bit_YUV(uint16_t y, uint16_t u, uint16_t v, int nColorimetry)
{
	double g;
	int green;

	g = ((double)(y - 64) * 64.) * NVC_YUY2_Y;
	g -= ((double)(u - 512) * 64.) * NVC_YUY2_U_G(nColorimetry);
	g -= ((double)(v - 512) * 64.) * NVC_YUY2_V_G(nColorimetry);

	green = (int)(g + 0.5);

	if (green<0)
		green = 0;
	else if (green>65535)
		green = 65535;


	return (uint16_t)(green);
}

// calculate 16 bit b from 10bit y, u, v
inline uint16_t NVC_16Bit_B_FROM_10Bit_YUV(uint16_t y, uint16_t u, uint16_t v, int nColorimetry)
{
	UNREFERENCED_PARAMETER(v);

	double b;
	int blue;

	b = ((double)(y - 64) * 64.) * NVC_YUY2_Y;
	b += ((double)(u - 512) * 64.) * NVC_YUY2_U_B(nColorimetry);

	blue = (int)(b + 0.5);

	if (blue<0)
		blue = 0;
	else if (blue>65535)
		blue = 65535;


	return (uint16_t)(blue);
}

// calculate 10 bit y from 8bit r, g, b
inline uint16_t NVC_10Bit_Y_FROM_8Bit_RGB(uint8_t r, uint8_t g, uint8_t b, int nColorimetry)
{
	double y;

	y = ((double)r * 4.) * NVC_YUY2_R_Y(nColorimetry);
	y += ((double)g * 4.) * NVC_YUY2_G_Y(nColorimetry);
	y += ((double)b * 4.) * NVC_YUY2_B_Y(nColorimetry);

	return (uint16_t)((int)(y + 0.5) + (int)((int)16 << 2));
}

// calculate 10 bit u from 8bit r, g, b
inline uint16_t NVC_10Bit_U_FROM_8Bit_RGB(uint8_t r, uint8_t g, uint8_t b, int nColorimetry)
{
	double u;

	u = ((double)b * 4.) * NVC_YUY2_B_U(nColorimetry);
	u -= ((double)r * 4.) * NVC_YUY2_R_U(nColorimetry);
	u -= ((double)g * 4.) * NVC_YUY2_G_U(nColorimetry);

	return (uint16_t)((int)(u + 0.5 + 512));
}

// calculate 10 bit v from 8bit r, g, b
inline uint16_t NVC_10Bit_V_FROM_8Bit_RGB(uint8_t r, uint8_t g, uint8_t b, int nColorimetry)
{
	double v;

	v = ((double)r * 4.) * NVC_YUY2_R_V(nColorimetry);
	v -= ((double)g * 4.) * NVC_YUY2_G_V(nColorimetry);
	v -= ((double)b * 4.) * NVC_YUY2_B_V(nColorimetry);

	return (uint16_t)((int)(v + 0.5 + 512));
}

// calculate 8 bit r from 10bit y, u, v
inline uint8_t NVC_8Bit_R_FROM_10Bit_YUV(uint16_t y, uint16_t u, uint16_t v, int nColorimetry)
{
	UNREFERENCED_PARAMETER(u);

	double r;

	r = ((double)(y - 64) / 4.) * NVC_YUY2_Y;
	r += ((double)(v - 512) / 4.) * NVC_YUY2_V_R(nColorimetry);

	return (uint8_t)((int)(r + 0.5));
}

// calculate 8 bit g from 10bit y, u, v
inline uint8_t NVC_8Bit_G_FROM_10Bit_YUV(uint16_t y, uint16_t u, uint16_t v, int nColorimetry)
{
	double g;

	g = ((double)(y - 64) / 4.) * NVC_YUY2_Y;
	g -= ((double)(u - 512) / 4.) * NVC_YUY2_U_G(nColorimetry);
	g -= ((double)(v - 512) / 4.) * NVC_YUY2_V_G(nColorimetry);

	return (uint8_t)((int)(g + 0.5));
}

// calculate 8 bit b from 10bit y, u, v
inline uint8_t NVC_8Bit_B_FROM_10Bit_YUV(uint16_t y, uint16_t u, uint16_t v, int nColorimetry)
{
	UNREFERENCED_PARAMETER(v);

	double b;

	b = ((double)(y - 64) / 4.) * NVC_YUY2_Y;
	b += ((double)(u - 512) / 4.) * NVC_YUY2_U_B(nColorimetry);

	return (uint8_t)((int)(b + 0.5));
}

// calculate 8bit r, g, b from 8bit y, u, v
inline void NVC_8Bit_RGB_FROM_8Bit_YUV(uint8_t &R, uint8_t &G, uint8_t &B, uint8_t y, uint8_t u, uint8_t v, int nColorimetry)
{
	int r, g, b, Y, U, V, tmp;

	Y = (int)y - 16;
	Y = Y << 7;
	U = (int)u - 128;
	U = U << 7;
	V = (int)v - 128;
	V = V << 7;

	Y = Y * (int)(NVC_YUY2_Y * 8192);
	Y = Y >> 16;

	// R Value
	r = V * (int)(NVC_YUY2_V_R(nColorimetry) * 8192);
	r = r >> 16;
	r += Y;
	r += 8;
	r = r >> 4;
	// G Value
	g = U * (int)(NVC_YUY2_U_G(nColorimetry) * 8192);
	g = g >> 16;
	g = Y - g;
	tmp = V * (int)(NVC_YUY2_V_G(nColorimetry) * 8192);
	tmp = tmp >> 16;
	g -= tmp;
	g += 8;
	g = g >> 4;
	// B Value
	b = U * (int)(NVC_YUY2_U_B(nColorimetry) * 8192);
	b = b >> 16;
	b += Y;
	b += 8;
	b = b >> 4;

	R = (uint8_t)NVC_MIN(255, NVC_MAX(r, 0));
	G = (uint8_t)NVC_MIN(255, NVC_MAX(g, 0));
	B = (uint8_t)NVC_MIN(255, NVC_MAX(b, 0));
}

// calculate 16bit r, g, b from 8bit y, u, v
inline void NVC_16Bit_RGB_FROM_8Bit_YUV(uint16_t &R, uint16_t &G, uint16_t &B, uint8_t y, uint8_t u, uint8_t v, int nColorimetry)
{
	double r, g, b, Y, U, V, tmp;

	Y = (int)y - 16;
	U = (int)u - 128;
	V = (int)v - 128;

	Y *= NVC_YUY2_Y;

	// R Value
	r = V * NVC_YUY2_V_R(nColorimetry);
	r += Y;
	// G Value
	g = U * NVC_YUY2_U_G(nColorimetry);
	g = Y - g;
	tmp = V * NVC_YUY2_V_G(nColorimetry);
	g -= tmp;
	// B Value
	b = U * NVC_YUY2_U_B(nColorimetry);
	b += Y;

	R = (uint16_t)(NVC_MIN(257 * NVC_MAX(r, 0), 65535) + 0.5);
	G = (uint16_t)(NVC_MIN(257 * NVC_MAX(g, 0), 65535) + 0.5);
	B = (uint16_t)(NVC_MIN(257 * NVC_MAX(b, 0), 65535) + 0.5);
}

// calculate 32bit r, g, b from 8bit y, u, v
static inline void NVC_32Bit_RGB_FROM_8Bit_YUV(float& R, float& G, float& B, uint8_t y, uint8_t u, uint8_t v, int nColorimetry)
{
	float Y, U, V;

	Y = (float)((int)y - 16);
	U = (float)((int)u - 128);
	V = (float)((int)v - 128);

	Y *= (float)(NVC_YUY2_Y / 255);

	// R Value
	R = Y + V * (float)(NVC_YUY2_V_R(nColorimetry) / 255);
	// G Value
	G = Y - U * (float)(NVC_YUY2_U_G(nColorimetry) / 255);
	G -= V * (float)(NVC_YUY2_V_G(nColorimetry) / 255);
	// B Value
	B = Y + U * (float)(NVC_YUY2_U_B_709 / 255);
}

// calculate 32bit r, g, b from 10bit y, u, v
static inline void NVC_32Bit_RGB_FROM_10Bit_YUV(float& R, float& G, float& B, uint16_t y, uint16_t u, uint16_t v, int nColorimetry)
{
	float Y, U, V;

	Y = (float)((int)y - 64);
	U = (float)((int)u - 512);
	V = (float)((int)v - 512);

	Y *= (float)(NVC_YUY2_Y / 1023);

	// R Value
	R = Y + V * (float)(NVC_YUY2_V_R(nColorimetry) / 1023);
	// G Value
	G = Y - U * (float)(NVC_YUY2_U_G(nColorimetry) / 1023);
	G -= V * (float)(NVC_YUY2_V_G(nColorimetry) / 1023);
	// B Value
	B = Y + U * (float)(NVC_YUY2_U_B(nColorimetry) / 1023);
}

inline void Nvc_sNvcBGRA_8Bit_To_sNvcYUV_8Bit(sNvcBGRA_8Bit *psNvcBGRA, sNvcYUV_8Bit *pYUV, int nColorimetry)
{
	uint8_t b, g, r;

	b = psNvcBGRA->b;
	g = psNvcBGRA->g;
	r = psNvcBGRA->r;

	pYUV->y = NVC_Y_FROM_8Bit_RGB(r, g, b, nColorimetry);
	pYUV->u = NVC_U_FROM_8Bit_RGB(r, g, b, nColorimetry);
	pYUV->v = NVC_V_FROM_8Bit_RGB(r, g, b, nColorimetry);
}

inline void Nvc_sNvcBGRA_16Bit_To_sNvcYUV_8Bit(sNvcBGRA_16Bit *psNvcBGRA, sNvcYUV_8Bit *pYUV, int nColorimetry)
{
	uint16_t b, g, r;

	b = psNvcBGRA->b;
	g = psNvcBGRA->g;
	r = psNvcBGRA->r;

	pYUV->y = NVC_Y_FROM_16Bit_RGB(r, g, b, nColorimetry);
	pYUV->u = NVC_U_FROM_16Bit_RGB(r, g, b, nColorimetry);
	pYUV->v = NVC_V_FROM_16Bit_RGB(r, g, b, nColorimetry);
}

inline void Nvc_sNvcYUV_8Bit_To_sNvcBGRA_8Bit(sNvcYUV_8Bit *pYUV, sNvcBGRA_8Bit *psNvcBGRA, int nColorimetry)
{
	uint8_t y, u, v;

	u = pYUV->u;
	v = pYUV->v;
	y = pYUV->y;

	NVC_8Bit_RGB_FROM_8Bit_YUV(psNvcBGRA->r, psNvcBGRA->g, psNvcBGRA->b, y, u, v, nColorimetry);
	psNvcBGRA->a = (uint8_t)255;
}

inline void Nvc_sNvcYUV_8Bit_To_sNvcBGRA_16Bit(sNvcYUV_8Bit *pYUV, sNvcBGRA_16Bit *psNvcBGRA, int nColorimetry)
{
	uint8_t y, u, v;

	u = pYUV->u;
	v = pYUV->v;
	y = pYUV->y;

	NVC_16Bit_RGB_FROM_8Bit_YUV(psNvcBGRA->r, psNvcBGRA->g, psNvcBGRA->b, y, u, v, nColorimetry);
	psNvcBGRA->a = (uint16_t)65535;
}


//////////////////////////////////////////////////////////////////////////
// some constants used in rgb<-->yuv color-space conversion
//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/* RGB-->YUV                                                            */
/*                                                                      */
/* Y = Wr * R + Wg * G + Wb * B                                         */
/* U = 0.436 * (B - Y) / (1 - Wb)                                       */
/* V = 0.615 * (R - Y) / (1 - Wr)                                       */
/*                                                                      */
/* YUV-->RGB                                                            */
/*                                                                      */
/* R = Y + (1 - Wr) / 0.615 * V                                         */
/* G = Y - Wb * (1 - Wb) / (Wg * 0.436) * U - Wr * (1 - Wr) / (Wg * 0.615) * V    */
/* B = Y + (1 - Wb) / 0.436 * U                                         */
/*                                                                      */
/* For rec 601: Wr = 0.299, Wg = 0.587, Wb = 0.114                      */
/* For rec 709: Wr = 0.2125, Wg = 0.7154, Wb = 0.0721                   */
/*                                                                      */
/*                                                                      */
/* RGB-->YUV conversion(rec 601)                                        */
/*                                                                      */
/* Y = 0.299R + 0.587G + 0.114B                                         */
/* U = 0.492 (B-Y) = -0.147R - 0.289G + 0.436B                          */
/* V = 0.877 (R-Y) = 0.615R - 0.515G - 0.100B                           */
/*                                                                      */
/* YUV-->RGB conversion(rec 601)                                        */
/*                                                                      */
/* R = Y + 1.140V                                                       */
/* G = Y - 0.395U - 0.581V                                              */
/* B = Y + 2.032U                                                       */
/*                                                                      */
/* RGB-->YUV conversion(rec 709)                                        */
/*                                                                      */
/* Y = 0.2125R + 0.7154G + 0.0721B                                      */
/* U = 0.470 (B-Y) = -0.100R - 0.336G + 0.436B                          */
/* V = 0.781 (R-Y) = 0.615R - 0.559G - 0.056B                           */
/*                                                                      */
/* YUV-->RGB conversion(rec 709)                                        */
/*                                                                      */
/* R = Y + 1.280V                                                       */
/* G = Y - 0.214U - 0.380V                                              */
/* B = Y + 2.128U                                                       */
/*                                                                      */
/* RGB-->YUV conversion(rec 2020)                                       */
/*                                                                      */
/* Y = 0.2627R + 0.6780G + 0.0593B                                      */
/* U = 0.463 (B-Y) = -0.122R - 0.314G + 0.436B                          */
/* V = 0.834 (R-Y) = 0.615R - 0.565G - 0.049B                           */
/*                                                                      */
/* YUV-->RGB conversion(rec 2020)                                       */
/*                                                                      */
/* R = Y + 1.199V                                                       */
/* G = Y - 0.189U - 0.464V                                              */
/* B = Y + 2.158U                                                       */
/*                                                                      */
/* RGB-->YIQ conversion(rec 601)                                        */
/*                                                                      */
/* Y = 0.2989R + 0.5866G + 0.1144B                                      */
/* I = (0.877*(R-Y))*cos33 - (0.492*(B-Y))*sin33 = 0.5957R - 0.2745G - 0.3213B	*/
/* Q = (0.877*(R-Y))*sin33 + (0.492*(B-Y))*cos33 = 0.2115R - 0.5226G - 0.3111B  */
/*                                                                      */
/* YUV-->RGB conversion(rec 601)                                        */
/*                                                                      */
/* R = Y + 0.9563I + 0.6210Q                                            */
/* G = Y - 0.2721I - 0.6474Q                                            */
/* B = Y - 1.1070I + 1.7046Q                                            */
/************************************************************************/


#define	__NVC_R_Y_601		0.299f
#define	__NVC_G_Y_601		0.587f
#define	__NVC_B_Y_601		0.114f

#define	__NVC_BMY_U_601		0.49210f
#define	__NVC_RMY_V_601		0.87732f

#define	__NVC_V_R_601		1.13983f
#define	__NVC_U_G_601		(-0.39465f)
#define	__NVC_V_G_601		(-0.58060f)
#define	__NVC_U_B_601		2.03211f

#define	__NVC_R_Y_709		0.2126f
#define	__NVC_G_Y_709		0.7152f
#define	__NVC_B_Y_709		0.0722f

#define	__NVC_BMY_U_709		0.46993f
#define	__NVC_RMY_V_709		0.78105f

#define	__NVC_V_R_709		1.280f
#define	__NVC_U_G_709		(-0.214f)
#define	__NVC_V_G_709		(-0.380f)
#define	__NVC_U_B_709		2.128f

#define	__NVC_R_Y_2020		0.2627f
#define	__NVC_G_Y_2020		0.6780f
#define	__NVC_B_Y_2020		0.0593f

#define	__NVC_BMY_U_2020	0.46348f
#define	__NVC_RMY_V_2020	0.83413f

#define	__NVC_V_R_2020		1.199f
#define	__NVC_U_G_2020		(-0.189f)
#define	__NVC_V_G_2020		(-0.464f)
#define	__NVC_U_B_2020		2.158f

#define	__NVC_R_Y(colorimetry)	((colorimetry) == NVC_BT2020 ? (__NVC_R_Y_2020) : ((colorimetry) == NVC_BT709 ? (__NVC_R_Y_709) : (__NVC_R_Y_601)))
#define	__NVC_G_Y(colorimetry)	((colorimetry) == NVC_BT2020 ? (__NVC_G_Y_2020) : ((colorimetry) == NVC_BT709 ? (__NVC_G_Y_709) : (__NVC_G_Y_601)))
#define	__NVC_B_Y(colorimetry)	((colorimetry) == NVC_BT2020 ? (__NVC_B_Y_2020) : ((colorimetry) == NVC_BT709 ? (__NVC_B_Y_709) : (__NVC_B_Y_601)))

#define	__NVC_BMY_U(colorimetry)	((colorimetry) == NVC_BT2020 ? (__NVC_BMY_U_2020) : ((colorimetry) == NVC_BT709 ? (__NVC_BMY_U_709) : (__NVC_BMY_U_601)))
#define	__NVC_RMY_V(colorimetry)	((colorimetry) == NVC_BT2020 ? (__NVC_RMY_V_2020) : ((colorimetry) == NVC_BT709 ? (__NVC_RMY_V_709) : (__NVC_RMY_V_601)))

#define	__NVC_V_R(colorimetry)	((colorimetry) == NVC_BT2020 ? (__NVC_V_R_2020) : ((colorimetry) == NVC_BT709 ? (__NVC_V_R_709) : (__NVC_V_R_601)))
#define	__NVC_U_G(colorimetry)	((colorimetry) == NVC_BT2020 ? (__NVC_U_G_2020) : ((colorimetry) == NVC_BT709 ? (__NVC_U_G_709) : (__NVC_U_G_601)))
#define	__NVC_V_G(colorimetry)	((colorimetry) == NVC_BT2020 ? (__NVC_V_G_2020) : ((colorimetry) == NVC_BT709 ? (__NVC_V_G_709) : (__NVC_V_G_601)))
#define	__NVC_U_B(colorimetry)	((colorimetry) == NVC_BT2020 ? (__NVC_U_B_2020) : ((colorimetry) == NVC_BT709 ? (__NVC_U_B_709) : (__NVC_U_B_601)))



#define	__NVC_U_MAX			0.436f
#define	__NVC_V_MAX			0.615f

#define	__NVC_R_U_601		(-0.14713f)
#define	__NVC_G_U_601		(-0.28886f)
#define	__NVC_B_U_601		0.436f

#define	__NVC_R_V_601		0.615f
#define	__NVC_G_V_601		(-0.51499f)
#define	__NVC_B_V_601		(-0.10001f)

#define	__NVC_R_U_709		(-0.09991f)
#define	__NVC_G_U_709		(-0.33609f)
#define	__NVC_B_U_709		0.436f

#define	__NVC_R_V_709		0.615f
#define	__NVC_G_V_709		(-0.55861f)
#define	__NVC_B_V_709		(-0.05639f)

#define	__NVC_R_U_2020		(-0.12176f)
#define	__NVC_G_U_2020		(-0.31424f)
#define	__NVC_B_U_2020		0.43600f

#define	__NVC_R_V_2020		0.61500f
#define	__NVC_G_V_2020		(-0.56554f)
#define	__NVC_B_V_2020		(-0.04946f)


#define	__NVC_R_U(colorimetry)	((colorimetry) == NVC_BT2020 ? (__NVC_R_U_2020) : ((colorimetry) == NVC_BT709 ? (__NVC_R_U_709) : (__NVC_R_U_601)))
#define	__NVC_G_U(colorimetry)	((colorimetry) == NVC_BT2020 ? (__NVC_G_U_2020) : ((colorimetry) == NVC_BT709 ? (__NVC_G_U_709) : (__NVC_G_U_601)))
#define	__NVC_B_U(colorimetry)	((colorimetry) == NVC_BT2020 ? (__NVC_B_U_2020) : ((colorimetry) == NVC_BT709 ? (__NVC_B_U_709) : (__NVC_B_U_601)))

#define	__NVC_R_V(colorimetry)	((colorimetry) == NVC_BT2020 ? (__NVC_R_V_2020) : ((colorimetry) == NVC_BT709 ? (__NVC_R_V_709) : (__NVC_R_V_601)))
#define	__NVC_G_V(colorimetry)	((colorimetry) == NVC_BT2020 ? (__NVC_G_V_2020) : ((colorimetry) == NVC_BT709 ? (__NVC_G_V_709) : (__NVC_G_V_601)))
#define	__NVC_B_V(colorimetry)	((colorimetry) == NVC_BT2020 ? (__NVC_B_V_2020) : ((colorimetry) == NVC_BT709 ? (__NVC_B_V_709) : (__NVC_B_V_601)))


#define	__NVC_R_I_601		0.5957f
#define	__NVC_G_I_601		(-0.2745f)
#define	__NVC_B_I_601		(-0.3213f)

#define	__NVC_R_Q_601		0.2115f
#define	__NVC_G_Q_601		(-0.5226f)
#define	__NVC_B_Q_601		(-0.3111f)

#define	__NVC_I_R_601		0.9563f
#define	__NVC_Q_R_601		0.6210f
#define	__NVC_I_G_601		(-0.2721f)
#define	__NVC_Q_G_601		(-0.6474f)
#define	__NVC_I_B_601		(-1.1070f)
#define	__NVC_Q_B_601		(1.7046f)


// convert from RGB to YUV
static inline void NvcRGBToYUV(float r, float g, float b,
	float& y, float& u, float& v, int nColorimetry)
{
	y = __NVC_R_Y(nColorimetry) * r + __NVC_G_Y(nColorimetry) * g + __NVC_B_Y(nColorimetry) * b;
	u = __NVC_BMY_U(nColorimetry) * (b - y);
	v = __NVC_RMY_V(nColorimetry) * (r - y);
}

// convert from YUV to RGB 
static inline void NvcYUVToRGB(float y, float u, float v,
	float& r, float& g, float& b, int nColorimetry)
{
	r = y + __NVC_V_R(nColorimetry) * v;
	if (r < 0)
		r = 0;
	else if (r > 1)
		r = 1;

	g = y + __NVC_U_G(nColorimetry) * u + __NVC_V_G(nColorimetry) * v;
	if (g < 0)
		g = 0;
	else if (g > 1)
		g = 1;

	b = y + __NVC_U_B(nColorimetry) * u;
	if (b < 0)
		b = 0;
	else if (b > 1)
		b = 1;
}

////
//// convert from RGB to YUV (rec 601)
////
//static inline void NvcRGBToYUV601(float r, float g, float b,
//	float& y, float& u, float& v)
//{
//	y = __NVC_R_Y_601 * r + __NVC_G_Y_601 * g + __NVC_B_Y_601 * b;
//	u = __NVC_BMY_U_601 * (b - y);
//	v = __NVC_RMY_V_601 * (r - y);
//}
//
////
//// convert from RGB to YUV (rec 709)
////
//static inline void NvcRGBToYUV709(float r, float g, float b,
//	float& y, float& u, float& v)
//{
//	y = __NVC_R_Y_709 * r + __NVC_G_Y_709 * g + __NVC_B_Y_709 * b;
//	u = __NVC_BMY_U_709 * (b - y);
//	v = __NVC_RMY_V_709 * (r - y);
//}

////
//// convert from YUV to RGB (rec 601)
////
//static inline void NvcYUVToRGB601(float y, float u, float v,
//	float& r, float& g, float& b)
//{
//	r = y + __NVC_V_R_601 * v;
//	if (r < 0)
//		r = 0;
//	else if (r > 1)
//		r = 1;
//
//	g = y + __NVC_U_G_601 * u + __NVC_V_G_601 * v;
//	if (g < 0)
//		g = 0;
//	else if (g > 1)
//		g = 1;
//
//	b = y + __NVC_U_B_601 * u;
//	if (b < 0)
//		b = 0;
//	else if (b > 1)
//		b = 1;
//}
//
////
//// convert from YUV to RGB (rec 709)
////
//static inline void NvcYUVToRGB709(float y, float u, float v,
//	float& r, float& g, float& b)
//{
//	r = y + __NVC_V_R_709 * v;
//	if (r < 0)
//		r = 0;
//	else if (r > 1)
//		r = 1;
//
//	g = y + __NVC_U_G_709 * u + __NVC_V_G_709 * v;
//	if (g < 0)
//		g = 0;
//	else if (g > 1)
//		g = 1;
//
//	b = y + __NVC_U_B_709 * u;
//	if (b < 0)
//		b = 0;
//	else if (b > 1)
//		b = 1;
//}


//
// conversion from RGB to HSV
//
static inline void NvcRGBToHSV(float r, float g, float b,
	float& hue, float& sat, float& value)
{
	// r, g, b values are from 0 to 1
	// h = [0,360), s = [0,1], v = [0,1]

	float min, max, delta, tmp;
	tmp = NVC_MIN(r, g);
	min = NVC_MIN(tmp, b);
	tmp = NVC_MAX(r, g);
	max = NVC_MAX(tmp, b);
	delta = max - min;

	value = max;	// value is the max of r,g,b
	if (max != 0)
		sat = delta / max;	// saturation = delta / max
	else
		sat = 0;

	if (min != max) {
		if (max == r) {
			hue = (g - b) / delta;
			if (hue < 0)
				hue += 6;
		}
		else if (max == g) {
			hue = (b - r) / delta + 2;
		}
		else if (max == b) {
			hue = (r - g) / delta + 4;
		}
	}
	else {
		hue = 0;
	}

	hue *= 60;
}

//
// conversion from HSV to RGB
//
static inline void NvcHSVToRGB(float hue, float sat, float value,
	float& r, float& g, float& b)
{
	// r, g, b values are from 0 to 1
	// h = [0, infinite), s = [0,1], v = [0,1]
	float f, p, q, t;
	int i;

	if (sat == 0) {
		// achromatic (gray)
		r = g = b = value;
		return;
	}

	hue = fmodf(hue, 360);
	if (hue < 0)
		hue += 360;

	hue /= 60;
	i = (int)hue;	// sector 0 to 5
	f = hue - i;	// factorial part of h
	p = value * (1 - sat);
	q = value * (1 - sat * f);
	t = value * (1 - sat * (1 - f));

	switch (i) {
	case 0:
		r = value;
		g = t;
		b = p;
		break;
	case 1:
		r = q;
		g = value;
		b = p;
		break;
	case 2:
		r = p;
		g = value;
		b = t;
		break;
	case 3:
		r = p;
		g = q;
		b = value;
		break;
	case 4:
		r = t;
		g = p;
		b = value;
		break;
	default: // case 5:
		r = value;
		g = p;
		b = q;
		break;
	}
}

