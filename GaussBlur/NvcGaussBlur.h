//==========================================================================;
//
// (c) Copyright NewAuto Video Technology CO.,Ltd., 2008. All rights reserved. 
//
// This code and information is provided "as is" without warranty of any kind, 
// either expressed or implied, including but not limited to the implied 
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------;
//   Birth Date:       Oct 17. 2008
//   Author:           NewAuto video team
//----------------------------------------------------------------
//   
//   Beijing China. 
//----------------------------------------------------------------

#pragma once

#include <NvcVisionCoreDef.h>


//////////////////////////////////////////////////////////////////////////
// gaussian blur parameter's default and limitation values
//////////////////////////////////////////////////////////////////////////
#define NVC_GBLUR_SIGMA_MAX				100

#define NVC_GBLUR_WIDTH_DEFAULT			1
#define NVC_GBLUR_WIDTH_MIN				0
#define NVC_GBLUR_WIDTH_MAX				NVC_GBLUR_SIGMA_MAX

#define NVC_GBLUR_HEIGHT_DEFAULT		1
#define NVC_GBLUR_HEIGHT_MIN			0
#define NVC_GBLUR_HEIGHT_MAX			NVC_GBLUR_SIGMA_MAX


// dilate object handle
typedef void *	NVC_HGAUSSBLUR;


#ifdef __cplusplus
extern "C" {
#endif

// create a gaussian blur object, return the handle of the object
NVC_HGAUSSBLUR NVCAPI NvcGaussBlurCreate();

// destroy a gaussian blur object
void NVCAPI NvcGaussBlurDestroy(NVC_HGAUSSBLUR hGaussBlur);

// NOTE: pAuxBuffer should be a buffer whose dimension is at least the same as source buffer
// and its pitch must be at least 16 bytes aligned (cache size aligned is recommended)
int NVCAPI NvcGaussBlurPrepare(NVC_HGAUSSBLUR hGaussBlur,
							   void *pSrc, int nSrcPitch,
							   void *pDst, int nDstPitch,
							   void *pAuxBuffer, int nAuxPitch,
							   unsigned int nWidth,
							   unsigned int nHeight,
							   ENvcBitDepth eBitDepth,
							   int bIsDataMatte,
							   float fBlurWidth,
							   float fBlurHeight,
							   unsigned int nThreadCount,
							   const SNvcMaskInfo *pMaskInfo);

int NVCAPI NvcGaussBlurProcess(NVC_HGAUSSBLUR hGaussBlur, unsigned int nThreadIndex);

#ifdef __cplusplus
}
#endif

