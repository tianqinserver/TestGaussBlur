#include <NvcGaussBlur.h>
//#include <NvcVisionUtils.h>
#include <NvcColorConvDef.h>
#include "NvcVisionUtilsFFT.h"

#include <emmintrin.h>
#include <assert.h>
#include <math.h>
#include <new>


struct SNvcMaskInfo;

extern const __m128i g_xmm_32bit_32768;
extern const __m128i g_xmm_16bit_32768;


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

// void __IntpMatteLineByMask(void *, void*, int, SNvcMaskInfo*, int, int);
//
//void __IntpBGRALineByMask(void *, void*, int, SNvcMaskInfo*, int, int);


void _copy_surface(const void *pSrc,
	int nSrcPitch,
	void *pDst,
	int nDstPitch,
	unsigned int nLineSize,
	unsigned int nHeight);


void * _malloc_aligned(size_t size, size_t alignment);

void _free_aligned(void *ptr);

//void __NvcFFTGeneratePhaseFactors(
//	unsigned int,
//	float *,
//	float *);
//
//void __NvcRealFFT(
//	float *,
//	float *,
//	const float *,
//	const float *,
//	unsigned int);
//
//void __NvcRealIFFT(
//	float *,
//	float *,
//	const float *,
//	const float *,
//	unsigned int);
//
//void __NvcRealFFT_4C(
//	float *,
//	float *,
//	const float *,
//	const float *,
//	unsigned int);
//
//void __NvcRealIFFT_4C(
//	float *,
//	float *,
//	const float *,
//	const float *,
//	unsigned int);

//struct __CNvcBufferManager
//{
//	__CNvcBufferManager(float,float,bool)
//	{
//
//	}
//
//	bool CommitMemory(int, int)
//	{
//		return true;
//	}
//
//	void* GetBufferPointer(int)
//	{
//		return NULL;
//	}
//};


//////////////////////////////////////////////////////////////////////////
// class to manage the allocation of video buffer
//////////////////////////////////////////////////////////////////////////
class __CNvcBufferManager
{
public:
	// @nInitSize: initial size of this buffer
	// @nIncreaseSize: the granularity we increase buffer size
	__CNvcBufferManager(size_t nInitSize, size_t nIncreaseSize, bool *pbSucceeded);
	~__CNvcBufferManager();

public:
	// commit memory for buffer (in byte unit)
	bool CommitMemory(size_t nBufferSizePerThread, unsigned int nThreadCount);

	inline void * GetBufferPointer(unsigned int nThreadIdx) const
	{
		if (nThreadIdx >= m_nCommittedThreadCount) {
			assert(false);
			return NULL;
		}

		return (char *)m_pBuffer + m_nCommittedBufferSizePerThread * nThreadIdx;
	}

protected:
	void *m_pBuffer;			// buffer pointer

	size_t m_nTotalBufferSize;	// total buffer size (in byte unit)
	size_t m_nIncreaseSize;

	size_t m_nCommittedBufferSizePerThread;
	unsigned int m_nCommittedThreadCount;
};
