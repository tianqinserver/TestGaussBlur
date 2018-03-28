//==========================================================================;
//
// (c) Copyright NewAuto Video Technology CO.,Ltd., 2008. All rights reserved. 
//
// This code and information is provided "as is" without warranty of any kind, 
// either expressed or implied, including but not limited to the implied 
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------;
//   Birth Date:       Oct 27. 2008
//   Author:           NewAuto video team
//----------------------------------------------------------------
//   
//   Beijing China. 
//----------------------------------------------------------------
#pragma once

#include <NvcVisionCoreDef.h>


// W(N)^k = exp(-j*2*pi*k/N)    k = 0, 1, ... N-1
void __NvcFFTGeneratePhaseFactors(unsigned int nOrder,
								  float *pFactorReal,
								  float *pFactorImg);


// forward fast fourier transform routine
// input signal should be stored in natural order
// output signal will be stored in bit-reversed order
// NOTE: this routine will process FFT in-place!
void __NvcFFT(float *pSignalReal,
			  float *pSignalImg,
			  const float *pFactorReal,
			  const float *pFactorImg,
			  unsigned int nOrder);

// forward fast fourier transform routine
// input signal should be stored in natural order
// output signal will be stored in bit-reversed order
// NOTE: this routine will process FFT in-place!
// input signal are all real numbers, values in pSignalImg needn't to be initialized
void __NvcRealFFT(float *pSignalReal,
				  float *pSignalImg,
				  const float *pFactorReal,
				  const float *pFactorImg,
				  unsigned int nOrder);

// forward fast fourier transform routine (four channel)
// input signal should be stored in natural order
// output signal will be stored in bit-reversed order
// NOTE: this routine will process FFT in-place!
// input signal are all real numbers, values in pSignalImg needn't to be initialized
void __NvcRealFFT_4C(float *pSignalReal,
					 float *pSignalImg,
					 const float *pFactorReal,
					 const float *pFactorImg,
					 unsigned int nOrder);

// inverse fast fourier transform routine
// input signal should be stored in bit-reversed order
// output signal will be stored in natural order
// NOTE: this routine will process IFFT in-place!
void __NvcIFFT(float *pSignalReal,
			   float *pSignalImg,
			   const float *pFactorReal,
			   const float *pFactorImg,
			   unsigned int nOrder);
 
// inverse fast fourier transform routine
// input signal should be stored in bit-reversed order
// output signal will be stored in natural order
// NOTE: this routine will process IFFT in-place!
// output signal are all real numbers, values in pSignalImg is undetermined after transformation
void __NvcRealIFFT(float *pSignalReal,
				   float *pSignalImg,
				   const float *pFactorReal,
				   const float *pFactorImg,
				   unsigned int nOrder);

// inverse fast fourier transform routine (four channel)
// input signal should be stored in bit-reversed order
// output signal will be stored in natural order
// NOTE: this routine will process IFFT in-place!
// output signal are all real numbers, values in pSignalImg is undetermined after transformation
void __NvcRealIFFT_4C(float *pSignalReal,
					  float *pSignalImg,
					  const float *pFactorReal,
					  const float *pFactorImg,
					  unsigned int nOrder);

