#ifndef __NVC_COLOR_H__
#define	__NVC_COLOR_H__


#include <NvcVisionCoreDef.h>
#include <float.h>

#define NVC_RGB_CHANNEL_VALUE_DEFAULT               0
#define NVC_RGB_CHANNEL_VALUE_MIN                   (-100000)
#define NVC_RGB_CHANNEL_VALUE_MAX                   100000
#define NVC_RGB_CHANNEL_VALUE_CLASSIC_MIN           0
#define NVC_RGB_CHANNEL_VALUE_CLASSIC_MAX           1

#define NVC_ALPHA_CHANNEL_VALUE_DEFAULT             1
#define NVC_ALPHA_CHANNEL_VALUE_MIN                 0
#define NVC_ALPHA_CHANNEL_VALUE_MAX                 1

#define NVC_SATURATION_CHANNEL_VALUE_MIN            (-100000)
#define NVC_SATURATION_CHANNEL_VALUE_MAX            100000
#define NVC_SATURATION_CHANNEL_VALUE_CLASSIC_MIN    0
#define NVC_SATURATION_CHANNEL_VALUE_CLASSIC_MAX    1
#define NVC_VALUE_CHANNEL_VALUE_MIN                 (-100000)
#define NVC_VALUE_CHANNEL_VALUE_MAX                 100000
#define NVC_VALUE_CHANNEL_VALUE_CLASSIC_MIN         0
#define NVC_VALUE_CHANNEL_VALUE_CLASSIC_MAX         1


static inline float NvcLimit32BitColorChannel(float channel)
{
    if (channel != channel)
        return 0; // Limit Nan to zero

    if (channel < 0)
        return 0;
    else if (channel > 1)
        return 1;
    else
        return channel;
}

static inline int NvcIsValid32BitColor(float color)
{
    if (color != color)
        return 0; // NAN is not a valid color value

    if (color < -FLT_MAX || color > FLT_MAX)
        return 0; // INFINITY is not a valid color value

    return 1;
}

static inline float NvcLimit32BitColor(sNvcBGRA_32Bit *pColor32Bit)
{
    pColor32Bit->r = NvcLimit32BitColorChannel(pColor32Bit->r);
    pColor32Bit->g = NvcLimit32BitColorChannel(pColor32Bit->g);
    pColor32Bit->b = NvcLimit32BitColorChannel(pColor32Bit->b);
    pColor32Bit->a = NvcLimit32BitColorChannel(pColor32Bit->a);
}

static inline void Nvc32BitColorTo8BitColor(const sNvcBGRA_32Bit *pColor32Bit, sNvcBGRA_8Bit *pColor8Bit)
{
	pColor8Bit->r = (uint8_t)(NvcLimit32BitColorChannel(pColor32Bit->r) * 255.0 + 0.5);
	pColor8Bit->g = (uint8_t)(NvcLimit32BitColorChannel(pColor32Bit->g) * 255.0 + 0.5);
	pColor8Bit->b = (uint8_t)(NvcLimit32BitColorChannel(pColor32Bit->b) * 255.0 + 0.5);
	pColor8Bit->a = (uint8_t)(NvcLimit32BitColorChannel(pColor32Bit->a) * 255.0 + 0.5);
}

static inline void Nvc32BitColorTo16BitColor(const sNvcBGRA_32Bit *pColor32Bit, sNvcBGRA_16Bit *pColor16Bit)
{
	pColor16Bit->r = (uint16_t)(NvcLimit32BitColorChannel(pColor32Bit->r) * 65535.0 + 0.5);
	pColor16Bit->g = (uint16_t)(NvcLimit32BitColorChannel(pColor32Bit->g) * 65535.0 + 0.5);
	pColor16Bit->b = (uint16_t)(NvcLimit32BitColorChannel(pColor32Bit->b) * 65535.0 + 0.5);
	pColor16Bit->a = (uint16_t)(NvcLimit32BitColorChannel(pColor32Bit->a) * 65535.0 + 0.5);
}

static inline void Nvc8BitColorTo32BitColor(const sNvcBGRA_8Bit *pColor8Bit, sNvcBGRA_32Bit *pColor32Bit)
{
	pColor32Bit->r = (float)(pColor8Bit->r / 255.0);
	pColor32Bit->g = (float)(pColor8Bit->g / 255.0);
	pColor32Bit->b = (float)(pColor8Bit->b / 255.0);
	pColor32Bit->a = (float)(pColor8Bit->a / 255.0);
}

static inline void Nvc16BitColorTo32BitColor(const sNvcBGRA_16Bit *pColor16Bit, sNvcBGRA_32Bit *pColor32Bit)
{
	pColor32Bit->r = (float)(pColor16Bit->r / 65535.0);
	pColor32Bit->g = (float)(pColor16Bit->g / 65535.0);
	pColor32Bit->b = (float)(pColor16Bit->b / 65535.0);
	pColor32Bit->a = (float)(pColor16Bit->a / 65535.0);
}


#endif	// __NVC_COLOR_H__

