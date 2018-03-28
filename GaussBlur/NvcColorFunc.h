#ifndef __NVC_COLOR_FUNC_H__
#define	__NVC_COLOR_FUNC_H__

#include "NvcColor.h"


#ifdef __cplusplus
extern "C" {
#endif
	
// vertically flip a surface
void NVCAPI NvcVerticalFlip(void *pSrc, void *pDst,
							int nPitch, int nLineCount);

// vertically flip a surface in place
void NVCAPI NvcVerticalFlipInPlace(void *pSurface, int nPitch, int nLineCount);


// perform surface copy
void NVCAPI NvcCopySurface(const void *pSrc,
						   int nSrcPitch,
						   void *pDst,
						   int nDstPitch,
						   unsigned int nLineSize,
						   unsigned int nHeight);

//multiply 8 bit matte by mask in place
void NVCAPI Nvc_Multiply_MatteLine_ByMask_8Bit(uint8_t *pMatteLine,
											   const uint8_t *pMaskLine,
											   unsigned int nCount,
											   float fIntensity,
											   int bInvertMask);

//multiply 16 bit matte by mask in place
void NVCAPI Nvc_Multiply_MatteLine_ByMask_16Bit(uint16_t *pMatteLine,
												const uint16_t *pMaskLine,
												unsigned int nCount,
												float fIntensity,
												int bInvertMask);

// Multiply 32 bit matte by mask in place
void NVCAPI Nvc_Multiply_MatteLine_ByMask_32Bit(float *pMatteLine,
												const float *pMaskLine,
												unsigned int nCount,
												float fIntensity,
												int bInvertMask);

//interpolate between 8Bit source color and destination color using mask as factor
void NVCAPI Nvc_Intp_BGRALine_ByMask_8Bit(uint8_t *pSrcLine,
										  uint8_t *pDstLine,
										  const uint8_t *pMaskLine,
										  unsigned int nCount,
										  float fIntensity,
										  int bInvertMask);

//interpolate between 16Bit source color and destination color using mask as factor
void NVCAPI Nvc_Intp_BGRALine_ByMask_16Bit(uint16_t *pSrcLine,
										   uint16_t *pDstLine,
										   const uint16_t *pMaskLine,
										   unsigned int nCount,
										   float fIntensity,
										   int bInvertMask);

// Interpolate between 32 bit source color and destination color using mask as factor
void NVCAPI Nvc_Intp_BGRALine_ByMask_32Bit(float *pSrcLine,
										   float *pDstLine,
										   const float *pMaskLine,
										   unsigned int nCount,
										   float fIntensity,
										   int bInvertMask);

#ifdef __cplusplus
}
#endif

#endif	// __NVC_COLOR_FUNC_H__

