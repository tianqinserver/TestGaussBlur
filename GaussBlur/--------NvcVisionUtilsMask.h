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
#pragma once



#include <NvcVisionCoreDef.h>


//// multiply matte by mask in place
//void __MultiplyMatteLineByMask(void *pMatteLine,
//							   ENvcBitDepth eBitDepth,
//							   const SNvcMaskInfo *pMaskInfo,
//							   int nMaskLineIdx,
//							   int nPixelCount);

// interpolate between source matte and destination matte using mask as factor
// the final result is stored in destination line
void __IntpMatteLineByMask(const void *pSrcLine,
						   void *pDstLine,
						   ENvcBitDepth eBitDepth,
						   const SNvcMaskInfo *pMaskInfo,
						   int nMaskLineIdx,
						   int nPixelCount);

// interpolate between source color and destination color using mask as factor
// all channels (B/G/R/A) are interpolated
// the final result is stored in destination line
void __IntpBGRALineByMask(const void *pSrcLine,
						  void *pDstLine,
						  ENvcBitDepth eBitDepth,
						  const SNvcMaskInfo *pMaskInfo,
						  int nMaskLineIdx,
						  int nPixelCount);

