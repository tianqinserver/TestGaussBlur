#include <NvcGaussBlur.h>
//#include <NvcVisionUtils.h>
#include <myNvcVisionUtils.h>
#include <emmintrin.h>
#include <assert.h>
#include <math.h>
#include <new>


#define	__MAX_RADIUS			(NVC_GBLUR_SIGMA_MAX * 3)

#define __KERNEL_SCALE_BITS		15
#define __KERNEL_SCALE			(1 << __KERNEL_SCALE_BITS)

#define __NVC_OVERLAP_ADD_THRESHOLD_MATTE_HOZ		90
#define __NVC_OVERLAP_ADD_THRESHOLD_BGRA_HOZ		45
#define __NVC_OVERLAP_ADD_THRESHOLD_MATTE_VER		(__MAX_RADIUS + 1)
#define __NVC_OVERLAP_ADD_THRESHOLD_BGRA_VER		(__MAX_RADIUS + 1)


//
// class to handle temporary lines(lines which has performed horizontal convolution)
//
class CNvcTmpLinesInfo
{
public:
	CNvcTmpLinesInfo(
		int nImageHeight,
		void *pBuffer,
		int nCapacity,
		int nPitch,
		int nStartLineNo,
		int nEndLineNo/* exclusive */)
	{
		m_nImageHeight = nImageHeight;

		m_pBuffer = pBuffer;
		m_nCapacity = nCapacity;
		m_nPitch = nPitch;

		m_nStartLineIdxInBuffer = 0;

		assert(nEndLineNo > nStartLineNo);
		m_nStartLineNo = nStartLineNo;
		m_nEndLineNo = nEndLineNo;
	}

public:
	inline void * GetLineByNo(int nLineNo)
	{
		if (nLineNo < 0)
			nLineNo = 0;
		else if (nLineNo >= m_nImageHeight)
			nLineNo = m_nImageHeight - 1;

		assert(nLineNo >= m_nStartLineNo && nLineNo < m_nEndLineNo);

		int nIdx = (nLineNo - (m_nStartLineNo >= 0 ? m_nStartLineNo : 0) + m_nStartLineIdxInBuffer) % m_nCapacity;
		return (unsigned char *)m_pBuffer + nIdx * m_nPitch;
	}

	inline void MoveForward()
	{
		if (m_nStartLineNo >= 0)
			m_nStartLineIdxInBuffer = (m_nStartLineIdxInBuffer + 1) % m_nCapacity;

		m_nStartLineNo++;
		m_nEndLineNo++;
	}

	inline int GetNextLineNo() { return m_nEndLineNo; }

protected:
	int m_nImageHeight;	// height of image which this buffer belongs to

	void *m_pBuffer;
	int m_nCapacity;	// total line count can be stored in this buffer
	int m_nPitch;
	int m_nStartLineIdxInBuffer;

	int m_nStartLineNo, m_nEndLineNo/* exclusive */;
};


//////////////////////////////////////////////////////////////////////////
// class to implement gaussian blur
//////////////////////////////////////////////////////////////////////////
class CNvcGaussBlur
{
public:
	CNvcGaussBlur(bool& bSucceeded);
	~CNvcGaussBlur();

public:
	bool Prepare(
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

	bool Process(unsigned int nThreadIndex);

protected:
	bool m_bReadyToProcess;
	bool m_bTwoPhaseProcess;

	//
	// input parameters
	//
	void *m_pSrc, *m_pDst, *m_pAuxBuffer;
	int m_nSrcPitch, m_nDstPitch, m_nAuxPitch;
	unsigned int m_nWidth, m_nHeight;
	ENvcBitDepth m_eBitDepth;
	bool m_bIsDataMatte;	// is single channel

	unsigned int m_nThreadCount;
	//CNvcThreadSyncMgr m_oSyncMgr;

	//
	// internal parameters
	//
	unsigned int m_nBlurWidth;		// horizontal radius of the convolution kernel
	unsigned int m_nBlurHeight;		// vertical radius of the convolution kernel

	bool m_bUseOverlapAddHoz;		// use overlap-add method to process horizontal convolution?
	unsigned int m_nOverlapAddStepSizeHoz;
	unsigned int m_nOverlapAddStepCountHoz;
	unsigned int m_nOverlapAddOrderHoz;

	bool m_bUseOverlapAddVer;		// use overlap-add method to process vertical convolution?
	unsigned int m_nOverlapAddStepSizeVer;
	unsigned int m_nOverlapAddStepCountVer;
	unsigned int m_nOverlapAddOrderVer;

	unsigned int m_nBlurHozLeftUnsafeRegionEnd;		// exclusive
	unsigned int m_nBlurHozRightUnsafeRegionStart;

	// horizontal convolution kernel
	union {
		unsigned int m_tabKernelHoz[__MAX_RADIUS + 1];
		float m_tabKernelHozFloat[__MAX_RADIUS + 1];
	};

	// vertical convolution kernel
	union {
		unsigned int m_tabKernelVer[__MAX_RADIUS + 1];
		float m_tabKernelVerFloat[__MAX_RADIUS + 1];
	};

	//
	// temporary buffers used to perform horizontal convolution using overlap-add method (per thread)
	//
	__CNvcBufferManager m_oHozConvBuffer;
	unsigned int m_nConvLineSizeHoz;		// size in bytes of the buffer to store a temporary line (include padding bytes)

	//
	// temporary buffer used to perform vertical convolution using overlap-add method
	//
	__CNvcBufferManager m_oVerConvBuffer;
	unsigned int m_nConvLineSizeVer;		// size in bytes of the buffer to store a temporary line (include padding bytes)

	//
	// xmm variables used in convolution
	//
    union {
	    __m128i *m_xmm_kernel_table_hoz;
        __m128 *m_xmm_kernel_table_hoz_ps;
    };

    union {
        __m128i *m_xmm_kernel_table_ver;
        __m128 *m_xmm_kernel_table_ver_ps;
    };


	//
	// buffers to hold FFT phase factors
	//
	__CNvcBufferManager m_oHozFFTFactorsBuffer;
	__CNvcBufferManager m_oVerFFTFactorsBuffer;

	//
	// buffers to store FFT coefficients of the padded convolution kernel
	//
	__CNvcBufferManager m_oHozKernelFFTCoefBuffer;
	__CNvcBufferManager m_oVerKernelFFTCoefBuffer;

	//
	// mask information
	//
	SNvcMaskInfo m_sMaskInfo;
	bool m_bIsMaskValid;

protected:
	void ProcessPerThread(
		unsigned int nThreadIndex,
		unsigned int nTotalThreadCount,
		unsigned int nProcessPhase);

protected:
	void InitConvKernel(float fBlurWidth, float fBlurHeight);
	bool ShouldProcessInTwoPhase();
    bool ShouldUseFloatKernelHoz() const;
    bool ShouldUseFloatKernelVer() const;
	void CaclOverlapAddConvParameters(
		unsigned int nSignalLength,
		unsigned int nKernelRadius,
		unsigned int& nConvStepSize,
		unsigned int& nConvStepCount,
		unsigned int& nConvOrder);
	void BuildKernelTable(int nRadius, double dSigma, unsigned int *pTab);
	void BuildKernelTableFloat(int nRadius, double dSigma, float *pTab);
	bool PrepareIntlBuffer();

	void ProcessBlurHorizontalOnly(unsigned int nStartLine, unsigned int nEndLine, unsigned int nThreadIdx);
	void ProcessBlurVeticalNormal(unsigned int nStartLine, unsigned int nEndLine, unsigned int nThreadIdx);
	void ProcessBlurVerticalOverlapAdd(unsigned int nStartLine, unsigned int nEndLine, unsigned int nThreadIdx);

	//
	// horizontal blur methods
	//
	void BlurLineHorizontally(const void *pSrcLine, void *pDstLine, unsigned int nThreadIdx);

	void BlurLineHozMatte8Bit(const uint8_t *pSrcLine, uint8_t *pDstLine,
		unsigned int nStartPixel, unsigned int nEndPixel);

	void BlurLineHozMatte16Bit(const uint16_t *pSrcLine, uint16_t *pDstLine,
		unsigned int nStartPixel, unsigned int nEndPixel);

    void BlurLineHozMatte32Bit(const float *pSrcLine, float *pDstLine,
        unsigned int nStartPixel, unsigned int nEndPixel);

	template<typename T, typename CALC_T>
	void BlurLineHozMatteComplex(
        const T *pSrcLine,
        T *pDstLine,
		unsigned int nStartPixel,
        unsigned int nEndPixel,
        const CALC_T *pKernelTable);

	void BlurLineHozBGRA8Bit(const uint8_t *pSrcLine, uint8_t *pDstLine,
		unsigned int nStartPixel, unsigned int nEndPixel);

	void BlurLineHozBGRA16Bit(const uint16_t *pSrcLine, uint16_t *pDstLine,
		unsigned int nStartPixel, unsigned int nEndPixel);

    void BlurLineHozBGRA32Bit(const float *pSrcLine, float *pDstLine,
        unsigned int nStartPixel, unsigned int nEndPixel);

	template<typename T, typename CALC_T>
	void BlurLineHozBGRAComplex(
        const T *pSrcLine,
        T *pDstLine,
		unsigned int nStartPixel,
        unsigned int nEndPixel,
        const CALC_T *pKernelTable);

	void BlurLineHozWithOverlapAdd(
		const void *pSrcLine, void *pDstLine,
		float *pTmpLine, float *pReal, float *pImg);

	template<typename T>
	void BlurMatteLineHozWithOverlapAdd(
		const T *pSrcLine, T *pDstLine,
		float *pTmpLine, float *pReal, float *pImg);

	template<typename T>
	void FillMatteSignalSequenceHoz(
		const T *pSrcLine, float *pSignal,
		unsigned int nStartIdx, unsigned int nEndIdx);

	template<typename T>
	void BlurBGRALineHozWithOverlapAdd(
		const T *pSrcLine, T *pDstLine,
		float *pTmpLine, float *pReal, float *pImg);

	template<typename T>
	void FillBGRASignalSequenceHoz(
		const T *pSrcLine, float *pSignal,
		unsigned int nStartIdx, unsigned int nEndIdx);

	//
	// vertical blur methods
	//
	inline void BlurLineVerticallyFirstPass(
		const void *pSrcLine1,
        const void *pSrcLine2,
		unsigned int nKernelIndex,
		uint32_t *pDstLine,
        unsigned int nPixelCount);

	inline void BlurLineVerticallyMedianPass(
		const void *pSrcLine1,
        const void *pSrcLine2,
		unsigned int nKernelIndex,
		uint32_t *pDstLine,
        unsigned int nPixelCount);

	inline void BlurLineVerticallyFinalPass(
		const void *pSrcLine1,
        const uint32_t *pSrcLine2,
		unsigned int nKernelIndex,
		void *pDstLine,
        unsigned int nPixelCount);

	void BlurLineVerFirstPass8Bit(
		const uint8_t *pSrcLine1, const uint8_t *pSrcLine2,
		__m128i& xmm_factor, unsigned int nFactor,
		uint32_t *pDstLine, unsigned int nElementCount);

	void BlurLineVerFirstPass16Bit(
		const uint16_t *pSrcLine1, const uint16_t *pSrcLine2,
		__m128i& xmm_factor, unsigned int nFactor,
		uint32_t *pDstLine, unsigned int nElementCount);

    void BlurLineVerFirstPass32Bit(
        const float *pSrcLine1, const float *pSrcLine2,
        const __m128& xmm_factor_ps, float fFactor,
        float *pDstLine, unsigned int nElementCount);

	void BlurLineVerMedianPass8Bit(
		const uint8_t *pSrcLine1, const uint8_t *pSrcLine2,
		__m128i& xmm_factor, unsigned int nFactor,
		uint32_t *pDstLine, unsigned int nElementCount);

	void BlurLineVerMedianPass16Bit(
		const uint16_t *pSrcLine1, const uint16_t *pSrcLine2,
		__m128i& xmm_factor, unsigned int nFactor,
		uint32_t *pDstLine, unsigned int nElementCount);

    void BlurLineVerMedianPass32Bit(
        const float *pSrcLine1, const float *pSrcLine2,
        const __m128& xmm_factor_ps, float fFactor,
        float *pDstLine, unsigned int nElementCount);

	void BlurLineVerFinalPass8Bit(
		const uint8_t *pSrcLine1, const uint32_t *pSrcLine2,
		const __m128i& xmm_factor, unsigned int nFactor,
		uint8_t *pDstLine, unsigned int nElementCount);

    void BlurLineVerFinalPass16Bit(
        const uint16_t *pSrcLine1, const uint32_t *pSrcLine2,
        const __m128i& xmm_factor, unsigned int nFactor,
        uint16_t *pDstLine, unsigned int nElementCount);

    void BlurLineVerFinalPass32Bit(
        const float *pSrcLine1, const float *pSrcLine2,
        const __m128& xmm_factor_ps, float fFactor,
        float *pDstLine, unsigned int nElementCount);

	//
	// circular convolution related method
	//
	template<bool bIsFirstPass>
	void CircularConvolutionMatte(
		float *pSignalReal,
		float *pSignalImg,
		const float *pFFTFactorReal,
		const float *pFFTFactorImg,
		const float *pKernelFFTCoefReal,
		const float *pKernelFFTCoefImg,
		float *pDestReal,
		unsigned int nOrder,
		unsigned int nOverlapCount);

	template<bool bIsFirstPass>
	void CircularConvolutionBGRA(
		float *pSignalReal,
		float *pSignalImg,
		const float *pFFTFactorReal,
		const float *pFFTFactorImg,
		const float *pKernelFFTCoefReal,
		const float *pKernelFFTCoefImg,
		float *pDestReal,
		unsigned int nOrder,
		unsigned int nOverlapCount);
};


CNvcGaussBlur::CNvcGaussBlur(bool& bSucceeded)
: m_oHozConvBuffer(1920 * 4 * sizeof(float) * 3, 512 * 4 * sizeof(float), &bSucceeded)
, m_oVerConvBuffer(0, 512 * 4 * sizeof(float), &bSucceeded)
, m_oHozFFTFactorsBuffer(512 * sizeof(float) * 2, 512 * sizeof(float) * 2, &bSucceeded)
, m_oVerFFTFactorsBuffer(0, 512 * sizeof(float) * 2, &bSucceeded)
, m_oHozKernelFFTCoefBuffer(512 * sizeof(float) * 4 * 2, 512 * sizeof(float) * 2, &bSucceeded)
, m_oVerKernelFFTCoefBuffer(0, 512 * sizeof(float) * 2, &bSucceeded)
{
	m_bReadyToProcess = false;
	m_bTwoPhaseProcess = false;
	m_nThreadCount = 0;

    m_bIsMaskValid = false;

	m_xmm_kernel_table_hoz = 0;
	m_xmm_kernel_table_ver = 0;

	m_xmm_kernel_table_hoz = (__m128i *)_malloc_aligned((__MAX_RADIUS + 1) * sizeof(__m128i), 128);
	if (!m_xmm_kernel_table_hoz) {
		// out of memory
		bSucceeded = false;
		return;
	}

	m_xmm_kernel_table_ver = (__m128i *)_malloc_aligned((__MAX_RADIUS + 1) * sizeof(__m128i), 128);
	if (!m_xmm_kernel_table_ver) {
		// out of memory
		bSucceeded = false;
		return;
	}
}

CNvcGaussBlur::~CNvcGaussBlur()
{
	if (m_xmm_kernel_table_hoz)
		_free_aligned(m_xmm_kernel_table_hoz);

	if (m_xmm_kernel_table_ver)
		_free_aligned(m_xmm_kernel_table_ver);
}

bool CNvcGaussBlur::Prepare(void *pSrc, int nSrcPitch,
							void *pDst, int nDstPitch,
							void *pAuxBuffer, int nAuxPitch,
							unsigned int nWidth,
							unsigned int nHeight,
							ENvcBitDepth eBitDepth,
							int bIsDataMatte,
							float fBlurWidth,
							float fBlurHeight,
							unsigned int nThreadCount,
							const SNvcMaskInfo *pMaskInfo)
{
	m_bReadyToProcess = false;

	if (!pSrc || !pDst || !pAuxBuffer || !nThreadCount) {
		assert(false);
		return false;
	}

	//
	// remember input parameters
	//
	m_pSrc = pSrc;
	m_nSrcPitch = nSrcPitch;
	m_pDst = pDst;
	m_nDstPitch = nDstPitch;
	m_pAuxBuffer = pAuxBuffer;
	m_nAuxPitch = nAuxPitch;
	m_nWidth = nWidth;
	m_nHeight = nHeight;

	m_eBitDepth = eBitDepth;
	m_bIsDataMatte = bIsDataMatte ? true : false;

	m_nThreadCount = nThreadCount;

	// initialize convolution kernel
	InitConvKernel(fBlurWidth, fBlurHeight);

	m_bTwoPhaseProcess = ShouldProcessInTwoPhase();
	if (m_bTwoPhaseProcess && m_nThreadCount > 1) {
		//if (!m_oSyncMgr.Initialize(m_nThreadCount))
			return false;
	}

	if (!m_bUseOverlapAddHoz) {
		//
		// determine safe/unsafe regions for horizontal convolution
		//
		if (m_nWidth > m_nBlurWidth) {
			unsigned int nAlign = 1;    // aligned pixel count
            switch (eBitDepth) {
            case keNvcBitDepthInt8:
                nAlign = m_bIsDataMatte ? 16 : 4;
                break;

            case keNvcBitDepthInt16:
                nAlign = m_bIsDataMatte ? 8 : 2;
                break;

            case keNvcBitDepthFloat32:
                nAlign = m_bIsDataMatte ? 4 : 1;
                break;
            }

			m_nBlurHozLeftUnsafeRegionEnd = NVC_UP_ALIGN_PO2(m_nBlurWidth, nAlign);
			m_nBlurHozRightUnsafeRegionStart = (m_nWidth - m_nBlurWidth) & ~(nAlign - 1);

			if (m_nBlurHozRightUnsafeRegionStart <= m_nBlurHozLeftUnsafeRegionEnd)
				m_nBlurHozLeftUnsafeRegionEnd = m_nBlurHozRightUnsafeRegionStart = m_nWidth;
		} else {
			// all the pixels are unsafe
			m_nBlurHozLeftUnsafeRegionEnd = m_nBlurHozRightUnsafeRegionStart = m_nWidth;
		}
	}

	// prepare internal buffer used to store temporary lines
	if (!PrepareIntlBuffer())
		return false;

	if (pMaskInfo) {
		m_sMaskInfo = *pMaskInfo;
		m_bIsMaskValid = true;
	} else {
		m_bIsMaskValid = false;
	}

	m_bReadyToProcess = true;
	return true;
}

bool CNvcGaussBlur::Process(unsigned int nThreadIndex)
{
	if (!m_bReadyToProcess)
		return false;

	if (nThreadIndex >= m_nThreadCount) {
		assert(false);
		return false;
	}

	// process phase one
	ProcessPerThread(nThreadIndex, m_nThreadCount, 1);

	if (m_bTwoPhaseProcess) {
		//m_oSyncMgr.Sync(nThreadIndex);

		// process phase two
		ProcessPerThread(nThreadIndex, m_nThreadCount, 2);
	}

	return true;
}

void CNvcGaussBlur::ProcessPerThread(unsigned int nThreadIndex,
									 unsigned int nTotalThreadCount,
									 unsigned int nProcessPhase)
{
	//
	// determine the line numbers this thread should process
	//
	unsigned int nStartLine, nEndLine/* exclusive */;

	nStartLine = m_nHeight * nThreadIndex / nTotalThreadCount;
	nEndLine = m_nHeight * (nThreadIndex + 1) / nTotalThreadCount;
	if (nEndLine <= nStartLine)
		return;

	if (!m_bTwoPhaseProcess) {
		if (m_nBlurWidth == 0 && m_nBlurHeight == 0) {
			// no blur is to be done, just copy the source surface to destination surface
			void *pSrc, *pDst;

			pSrc = (unsigned char *)m_pSrc + m_nSrcPitch * nStartLine;
			pDst = (unsigned char *)m_pDst + m_nDstPitch * nStartLine;

			_copy_surface(
				pSrc, m_nSrcPitch,
				pDst, m_nDstPitch,
				m_nWidth * NvcGetBytesFromBitDepth(m_eBitDepth) * (m_bIsDataMatte ? 1 : 4),
				nEndLine - nStartLine);

			return;
		}

		if (m_nBlurHeight == 0) {
			// only process horizontal blur
			ProcessBlurHorizontalOnly(nStartLine, nEndLine, nThreadIndex);
			return;
		}

		if (!m_bUseOverlapAddVer) {
			ProcessBlurVeticalNormal(nStartLine, nEndLine, nThreadIndex);
			return;
		}

		ProcessBlurVerticalOverlapAdd(nStartLine, nEndLine, nThreadIndex);
		return;
	}

	void *pSrcLine, *pDstLine;
	unsigned int i, j, count;

	count = nEndLine - nStartLine;

	if (nProcessPhase == 1) {
		// process phase one, do horizontal blur in this phase
		pSrcLine = (char *)m_pSrc + nStartLine * m_nSrcPitch;
		pDstLine = (char *)m_pAuxBuffer + nStartLine * m_nAuxPitch;

		for (i = 0; i < count; i++) {
			BlurLineHorizontally(pSrcLine, pDstLine, nThreadIndex);
			pSrcLine = (char *)pSrcLine + m_nSrcPitch;
			pDstLine = (char *)pDstLine + m_nAuxPitch;
		}

		return;
	}

	//
	// process phase two, do vertical blur in this phase
	//
	CNvcTmpLinesInfo oTmpLinesInfo(
		m_nHeight,
		m_pAuxBuffer,
		m_nHeight,
		m_nAuxPitch,
		-(int)m_nBlurHeight,
		m_nHeight + m_nBlurHeight/* exclusive */);

	// allocate a temporary buffer to store median result
	void *pTmpBuffer = _alloca(m_nWidth * (m_bIsDataMatte ? 1 : 4) * sizeof(uint32_t) + 127);
	pTmpBuffer = (void *)NVC_UP_ALIGN_PO2((size_t)pTmpBuffer, 128);

	pSrcLine = (char *)m_pSrc + m_nSrcPitch * nStartLine;
	pDstLine = (char *)m_pDst + m_nDstPitch * nStartLine;

	for (i = nStartLine; i < nEndLine; i++) {
		int nLineNum1 = i - m_nBlurHeight, nLineNum2 = i + m_nBlurHeight;

		for (j = 0; j < m_nBlurHeight; j++, nLineNum1++, nLineNum2--) {
			if (j) {
				BlurLineVerticallyMedianPass(
					oTmpLinesInfo.GetLineByNo(nLineNum1),
					oTmpLinesInfo.GetLineByNo(nLineNum2),
					j,
					(uint32_t *)pTmpBuffer,
					m_nWidth);
			} else {
				BlurLineVerticallyFirstPass(
					oTmpLinesInfo.GetLineByNo(nLineNum1),
					oTmpLinesInfo.GetLineByNo(nLineNum2),
					j,
					(uint32_t *)pTmpBuffer,
					m_nWidth);
			}
		}

		BlurLineVerticallyFinalPass(
			oTmpLinesInfo.GetLineByNo(i),
			(uint32_t *)pTmpBuffer,
			j,
			pDstLine,
			m_nWidth);

		if (m_bIsMaskValid) {
			if (m_bIsDataMatte)
				__IntpMatteLineByMask(pSrcLine, pDstLine, m_eBitDepth, &m_sMaskInfo, i, m_nWidth);
			else
				__IntpBGRALineByMask(pSrcLine, pDstLine, m_eBitDepth, &m_sMaskInfo, i, m_nWidth);
		}

		pSrcLine = (char *)pSrcLine + m_nSrcPitch;
		pDstLine = (char *)pDstLine + m_nDstPitch;
	}
}

void CNvcGaussBlur::InitConvKernel(float fBlurWidth,
								   float fBlurHeight)
{
	fBlurWidth = NVC_LIMIT_VALUE(fBlurWidth, NVC_GBLUR_WIDTH_MIN, NVC_GBLUR_WIDTH_MAX);
	fBlurHeight = NVC_LIMIT_VALUE(fBlurHeight, NVC_GBLUR_HEIGHT_MIN, NVC_GBLUR_HEIGHT_MAX);

	//
	// calculate convolution kernel radius, kernel length is radius * 2 + 1
	// we use fBlurWidth/fBlurHeight as sigma of Gaussian function
	//
	m_bUseOverlapAddHoz = false;
	if (fBlurWidth >= 0.01) {
		m_nBlurWidth = (unsigned int)ceil(fBlurWidth * 3);
		if (m_nBlurWidth > __MAX_RADIUS)
			m_nBlurWidth = __MAX_RADIUS;

        if ((m_bIsDataMatte && m_nBlurWidth >= __NVC_OVERLAP_ADD_THRESHOLD_MATTE_HOZ) ||
            (!m_bIsDataMatte && m_nBlurWidth >= __NVC_OVERLAP_ADD_THRESHOLD_BGRA_HOZ))
            m_bUseOverlapAddHoz = true;

        if (ShouldUseFloatKernelHoz())
            BuildKernelTableFloat(m_nBlurWidth, fBlurWidth, m_tabKernelHozFloat);
        else
            BuildKernelTable(m_nBlurWidth, fBlurWidth, m_tabKernelHoz);

        if (m_bUseOverlapAddHoz) {
            CaclOverlapAddConvParameters(
                m_nWidth,
                m_nBlurWidth,
                m_nOverlapAddStepSizeHoz,
                m_nOverlapAddStepCountHoz,
                m_nOverlapAddOrderHoz);
        }
	} else {
		m_nBlurWidth = 0;
	}

	m_bUseOverlapAddVer = false;
	if (fBlurHeight >= 0.01) {
		m_nBlurHeight = (unsigned int)ceil(fBlurHeight * 3);
		if (m_nBlurHeight > __MAX_RADIUS)
			m_nBlurHeight = __MAX_RADIUS;

        if ((m_bIsDataMatte && m_nBlurHeight >= __NVC_OVERLAP_ADD_THRESHOLD_MATTE_VER) ||
            (!m_bIsDataMatte && m_nBlurHeight >= __NVC_OVERLAP_ADD_THRESHOLD_BGRA_VER))
            m_bUseOverlapAddVer = true;

        if (ShouldUseFloatKernelVer())
            BuildKernelTableFloat(m_nBlurHeight, fBlurHeight, m_tabKernelVerFloat);
        else
            BuildKernelTable(m_nBlurHeight, fBlurHeight, m_tabKernelVer);

        if (m_bUseOverlapAddVer) {
            CaclOverlapAddConvParameters(
                m_nHeight,
                m_nBlurHeight,
                m_nOverlapAddStepSizeVer,
                m_nOverlapAddStepCountVer,
                m_nOverlapAddOrderVer);
        }
	} else {
		m_nBlurHeight = 0;
	}
}

bool CNvcGaussBlur::ShouldProcessInTwoPhase()
{
	if (!m_nBlurWidth || !m_nBlurHeight)
		return false;

	unsigned int nTotalTmpLineCount = (m_nBlurHeight * 2 + 1) * m_nThreadCount;
	if (nTotalTmpLineCount >= (unsigned int)(m_nHeight * 0.2))
		return true;

	return false;
}

bool CNvcGaussBlur::ShouldUseFloatKernelHoz() const
{
    return m_bUseOverlapAddHoz || m_eBitDepth == keNvcBitDepthFloat32;
}

bool CNvcGaussBlur::ShouldUseFloatKernelVer() const
{
    return m_bUseOverlapAddVer || m_eBitDepth == keNvcBitDepthFloat32;
}

// calculate the parameters when using overlap-add method to do convolution
void CNvcGaussBlur::CaclOverlapAddConvParameters(unsigned int nSignalLength,
												 unsigned int nKernelRadius,
												 unsigned int& nConvStepSize,
												 unsigned int& nConvStepCount,
												 unsigned int& nConvOrder)
{
	unsigned int m = nKernelRadius * 2 + 1;

	nConvOrder = 0;
	while (((unsigned int)1 << nConvOrder) < (m - 1) * 2)
		nConvOrder++;

	nConvStepSize = (1 << nConvOrder) - (m - 1);
	nConvStepCount = ((nSignalLength + nKernelRadius * 2) + nConvStepSize - 1) / nConvStepSize;
}

//
// gaussian function: gauss(x) = 1/(sqrt(2*pi)*sigma)*exp(-x^2/(2*sigma^2))
//
static inline void __CalcKernel(int radius, double sigma, double *kernel)
{
    double c0 = 1 / (sqrt(2 * NVC_PI) * sigma);
    double c1 = 2 * sigma * sigma;
    double sum = 0;
    int i;

    for (i = 0; i < radius; i++) {
        const double coeff = c0 * exp(-(i - radius) * (i - radius) / c1);
        kernel[i] = coeff;
        sum += coeff;
    }

    sum *= 2;

    kernel[i] = c0;
    sum += c0;

    // normalize kernel
    for (i = 0; i <= radius; i++)
        kernel[i] /= sum;
}

//
// gaussian function: gauss(x) = 1/(sqrt(2*pi)*sigma)*exp(-x^2/(2*sigma^2))
//
void CNvcGaussBlur::BuildKernelTable(int nRadius, double dSigma, unsigned int *pTab)
{
    double kernel[__MAX_RADIUS + 1];
    __CalcKernel(nRadius, dSigma, kernel);

    for (int i = 0; i <= nRadius; i++)
        pTab[i] = (unsigned int)(kernel[i] * __KERNEL_SCALE);
}

//
// gaussian function: gauss(x) = 1/(sqrt(2*pi)*sigma)*exp(-x^2/(2*sigma^2))
//
void CNvcGaussBlur::BuildKernelTableFloat(int nRadius, double dSigma, float *pTab)
{
    double kernel[__MAX_RADIUS + 1];
    __CalcKernel(nRadius, dSigma, kernel);

    for (int i = 0; i <= nRadius; i++)
        pTab[i] = (float)kernel[i];
}

bool CNvcGaussBlur::PrepareIntlBuffer()
{
	//
	// allocate internal auxiliary buffers
	//
	unsigned int i, nConvSize;

	if (m_nBlurWidth) {
        if (!m_bUseOverlapAddHoz) {
            // initialize xmm variables for horizontal convolution kernel
            if (ShouldUseFloatKernelHoz()) {
                for (i = 0; i < m_nBlurWidth + 1; i++)
                    m_xmm_kernel_table_hoz_ps[i] = _mm_set1_ps(m_tabKernelHozFloat[i]);
            } else {
                for (i = 0; i < m_nBlurWidth + 1; i++)
                    m_xmm_kernel_table_hoz[i] = _mm_set1_epi16((unsigned short)m_tabKernelHoz[i]);
            }
        } else {
			//
			// allocate and initialize FFT phase factors
			//
			nConvSize = 1 << m_nOverlapAddOrderHoz;
			if (!m_oHozFFTFactorsBuffer.CommitMemory(nConvSize * 2 * sizeof(float), 1))
				return false;	// out of memory

			__NvcFFTGeneratePhaseFactors(
				m_nOverlapAddOrderHoz,
				(float *)m_oHozFFTFactorsBuffer.GetBufferPointer(0),
				(float *)m_oHozFFTFactorsBuffer.GetBufferPointer(0) + nConvSize);

			//
			// allocate and initialize temporary buffers to do overlap-add convolution
			//
			unsigned int nTmpLineSize, nFFTCoefSize, nBufferSize;

			nTmpLineSize = m_nOverlapAddStepSizeHoz * m_nOverlapAddStepCountHoz + m_nBlurWidth * 2;
			nTmpLineSize *= (m_bIsDataMatte ? 1 : 4) * sizeof(float);
			nTmpLineSize = NVC_UP_ALIGN_PO2(nTmpLineSize, 16);

			assert(!(nConvSize & 3));
			nFFTCoefSize = nConvSize * (m_bIsDataMatte ? 1 : 4) * sizeof(float) * 2;

			nBufferSize = nTmpLineSize + nFFTCoefSize;
			if (!m_oHozConvBuffer.CommitMemory(nBufferSize, m_nThreadCount))
				return false;	// out of memory

			m_nConvLineSizeHoz = nTmpLineSize;

			//
			// allocate and initialize buffer to hold FFT coefficients of the padded convolution kernel
			//
			if (!m_oHozKernelFFTCoefBuffer.CommitMemory(nConvSize * 2 * sizeof(float), 1))
				return false;	// out of memory

			memcpy(m_oHozKernelFFTCoefBuffer.GetBufferPointer(0), m_tabKernelHozFloat, (m_nBlurWidth + 1) * sizeof(float));

			float *pCoef = (float *)m_oHozKernelFFTCoefBuffer.GetBufferPointer(0) + m_nBlurWidth + 1;
			for (i = 0; i < m_nBlurWidth; i++)
				pCoef[i] = m_tabKernelHozFloat[m_nBlurWidth - i - 1];

			memset(&pCoef[i], 0, (nConvSize - m_nBlurWidth * 2 - 1) * sizeof(float));

			// perform forward fast fourier transformation to the padded kernel
			__NvcRealFFT(
				(float *)m_oHozKernelFFTCoefBuffer.GetBufferPointer(0),
				(float *)m_oHozKernelFFTCoefBuffer.GetBufferPointer(0) + nConvSize,
				(float *)m_oHozFFTFactorsBuffer.GetBufferPointer(0),
				(float *)m_oHozFFTFactorsBuffer.GetBufferPointer(0) + nConvSize,
				m_nOverlapAddOrderHoz);
		}
	}

	if (m_nBlurHeight) {
		if (!m_bUseOverlapAddVer) {
			// initialize xmm variables for Vertical convolution kernel
            if (ShouldUseFloatKernelVer()) {
                for (i = 0; i < m_nBlurHeight + 1; i++)
                    m_xmm_kernel_table_ver_ps[i] = _mm_set1_ps(m_tabKernelVerFloat[i]);
            } else {
                for (i = 0; i < m_nBlurHeight + 1; i++)
                    m_xmm_kernel_table_ver[i] = _mm_set1_epi16((unsigned short)m_tabKernelVer[i]);
            }
		} else {
			//
			// allocate and initialize FFT phase factors
			//
			nConvSize = 1 << m_nOverlapAddOrderVer;
			if (!m_oVerFFTFactorsBuffer.CommitMemory(nConvSize * 2 * sizeof(float), 1)) {
				// out of memory
				return false;
			}

			__NvcFFTGeneratePhaseFactors(
				m_nOverlapAddOrderVer,
				(float *)m_oVerFFTFactorsBuffer.GetBufferPointer(0),
				(float *)m_oVerFFTFactorsBuffer.GetBufferPointer(0) + nConvSize);

			//
			// allocate and initialize temporary buffers to do overlap-add convolution
			//
			unsigned int nTmpLineSize, nFFTCoefSize, nBufferSize;

			nTmpLineSize = m_nOverlapAddStepSizeVer * m_nOverlapAddStepCountVer + m_nBlurHeight * 2;
			nTmpLineSize *= (m_bIsDataMatte ? 1 : 4) * sizeof(float);
			nTmpLineSize = NVC_UP_ALIGN_PO2(nTmpLineSize, 16);

			assert(!(nConvSize & 3));
			nFFTCoefSize = nConvSize * (m_bIsDataMatte ? 1 : 4) * sizeof(float) * 2;

			nBufferSize = nTmpLineSize + nFFTCoefSize;
			if (!m_oVerConvBuffer.CommitMemory(nBufferSize, 1))
				return false;	// out of memory

			m_nConvLineSizeVer = nTmpLineSize;

			//
			// allocate and initialize buffer to hold FFT coefficients of the padded convolution kernel
			//
			if (!m_oVerKernelFFTCoefBuffer.CommitMemory(nConvSize * 2 * sizeof(float), 1))
				return false;	// out of memory

			memcpy(m_oVerKernelFFTCoefBuffer.GetBufferPointer(0), m_tabKernelVerFloat, (m_nBlurHeight + 1) * sizeof(float));

			float *pCoef = (float *)m_oVerKernelFFTCoefBuffer.GetBufferPointer(0) + m_nBlurHeight + 1;
			for (i = 0; i < m_nBlurHeight; i++)
				pCoef[i] = m_tabKernelVerFloat[m_nBlurHeight - i - 1];

			memset(&pCoef[i], 0, (nConvSize - m_nBlurHeight * 2 - 1) * sizeof(float));

			// perform forward fast fourier transformation to the padded kernel
			__NvcRealFFT(
				(float *)m_oVerKernelFFTCoefBuffer.GetBufferPointer(0),
				(float *)m_oVerKernelFFTCoefBuffer.GetBufferPointer(0) + nConvSize,
				(float *)m_oVerFFTFactorsBuffer.GetBufferPointer(0),
				(float *)m_oVerFFTFactorsBuffer.GetBufferPointer(0) + nConvSize,
				m_nOverlapAddOrderVer);
		}
	}

	return true;
}

void CNvcGaussBlur::ProcessBlurHorizontalOnly(unsigned int nStartLine,
											  unsigned int nEndLine,
											  unsigned int nThreadIdx)
{
	void *pSrc, *pDst;
	unsigned int i;

	pSrc = (unsigned char *)m_pSrc + m_nSrcPitch * nStartLine;
	pDst = (unsigned char *)m_pDst + m_nDstPitch * nStartLine;

	for (i = nStartLine; i < nEndLine; i++) {
		BlurLineHorizontally(pSrc, pDst, nThreadIdx);

		if (m_bIsMaskValid) {
			if (m_bIsDataMatte)
				__IntpMatteLineByMask(pSrc, pDst, m_eBitDepth, &m_sMaskInfo, i, m_nWidth);
			else
				__IntpBGRALineByMask(pSrc, pDst, m_eBitDepth, &m_sMaskInfo, i, m_nWidth);
		}

		pSrc = (unsigned char *)pSrc + m_nSrcPitch;
		pDst = (unsigned char *)pDst + m_nDstPitch;
	}

}

void CNvcGaussBlur::ProcessBlurVeticalNormal(unsigned int nStartLine,
											 unsigned int nEndLine,
											 unsigned int nThreadIdx)
{
	int capacity, pitch, start_line, end_line/* exclusive */;
	void *pSrc, *pDst, *buffer;
	bool bProcessHoz;	// whether we should process horizontal blur

	pSrc = (unsigned char *)m_pSrc + m_nSrcPitch * nStartLine;
	pDst = (unsigned char *)m_pDst + m_nDstPitch * nStartLine;
	bProcessHoz = m_nBlurWidth != 0;

	if (bProcessHoz) {
		capacity = m_nBlurHeight * 2 + 1;
		assert(capacity * m_nThreadCount <= m_nHeight);
		buffer = (char *)m_pAuxBuffer + capacity * m_nAuxPitch * nThreadIdx;
		pitch = m_nAuxPitch;
		start_line = nStartLine - m_nBlurHeight;
		end_line = nStartLine + m_nBlurHeight + 1;
	} else {
		buffer = m_pSrc;
		capacity = m_nHeight;
		pitch = m_nSrcPitch;
		start_line = -(int)m_nBlurHeight;
		end_line = m_nHeight + m_nBlurHeight;
	}

	CNvcTmpLinesInfo oTmpLinesInfo(m_nHeight, buffer, capacity, pitch, start_line, end_line);
	unsigned int i, j;

	if (bProcessHoz) {
		j = end_line <= (int)m_nHeight ? end_line : m_nHeight;
		for (i = (start_line >= 0 ? start_line : 0); i < j; i++)
			BlurLineHorizontally((unsigned char *)m_pSrc + m_nSrcPitch * i, oTmpLinesInfo.GetLineByNo(i), nThreadIdx);
	}

	// allocate a temporary buffer to store median result
	void *pTmpBuffer = _alloca(m_nWidth * (m_bIsDataMatte ? 1 : 4) * sizeof(uint32_t) + 127);
	pTmpBuffer = (void *)NVC_UP_ALIGN_PO2((size_t)pTmpBuffer, 128);

	for (i = nStartLine; i < nEndLine; i++) {
		int nLineNum1 = i - m_nBlurHeight, nLineNum2 = i + m_nBlurHeight;

		for (j = 0; j < m_nBlurHeight; j++, nLineNum1++, nLineNum2--) {
			if (j) {
				BlurLineVerticallyMedianPass(
					oTmpLinesInfo.GetLineByNo(nLineNum1),
					oTmpLinesInfo.GetLineByNo(nLineNum2),
					j,
					(uint32_t *)pTmpBuffer,
					m_nWidth);
			} else {
				BlurLineVerticallyFirstPass(
					oTmpLinesInfo.GetLineByNo(nLineNum1),
					oTmpLinesInfo.GetLineByNo(nLineNum2),
					j,
					(uint32_t *)pTmpBuffer,
					m_nWidth);
			}
		}

		BlurLineVerticallyFinalPass(
			oTmpLinesInfo.GetLineByNo(i),
			(uint32_t *)pTmpBuffer,
			j,
			pDst,
			m_nWidth);

		if (m_bIsMaskValid) {
			if (m_bIsDataMatte)
				__IntpMatteLineByMask(pSrc, pDst, m_eBitDepth, &m_sMaskInfo, i, m_nWidth);
			else
				__IntpBGRALineByMask(pSrc, pDst, m_eBitDepth, &m_sMaskInfo, i, m_nWidth);
		}

		if (bProcessHoz) {
			int next_no = oTmpLinesInfo.GetNextLineNo();
			oTmpLinesInfo.MoveForward();
			if (next_no < (int)m_nHeight) {
				BlurLineHorizontally(
					(unsigned char *)m_pSrc + m_nSrcPitch * next_no,
					oTmpLinesInfo.GetLineByNo(next_no),
					nThreadIdx);
			}
		}

		pSrc = (unsigned char *)pSrc + m_nSrcPitch;
		pDst = (unsigned char *)pDst + m_nDstPitch;
	}
}

void CNvcGaussBlur::ProcessBlurVerticalOverlapAdd(unsigned int nStartLine,
												  unsigned int nEndLine,
												  unsigned int nThreadIdx)
{
	UNREFERENCED_PARAMETER(nStartLine);
	UNREFERENCED_PARAMETER(nEndLine);
	UNREFERENCED_PARAMETER(nThreadIdx);

	// for now, I don't see the benefit to process vertical convolution using overlap add method
	// because that require only one thread to process it
}



#ifdef _MSC_VER
#pragma warning(disable:4127)
#endif

void CNvcGaussBlur::BlurLineHorizontally(const void *pSrcLine,
										 void *pDstLine,
										 unsigned int nThreadIdx)
{
	if (m_bUseOverlapAddHoz) {
		//
		// use overlap-add method to process horizontal convolution
		//
		float *pTmpLine, *pReal, *pImg;
		void *pConvBuffer;

		pConvBuffer = m_oHozConvBuffer.GetBufferPointer(nThreadIdx);
		pTmpLine = (float *)pConvBuffer;
		pReal = (float *)((char *)pConvBuffer + m_nConvLineSizeHoz);
		pImg = pReal + (1 << m_nOverlapAddOrderHoz) * (m_bIsDataMatte ? 1 : 4);
		BlurLineHozWithOverlapAdd(pSrcLine, pDstLine, pTmpLine, pReal, pImg);
		return;
	}

	//
	// process left unsafe region
	//
	if (m_bIsDataMatte) {
		if (m_eBitDepth == keNvcBitDepthInt8)
			BlurLineHozMatteComplex<uint8_t, unsigned int>((uint8_t *)pSrcLine, (uint8_t *)pDstLine, 0, m_nBlurHozLeftUnsafeRegionEnd, m_tabKernelHoz);
		else if (m_eBitDepth == keNvcBitDepthInt16)
			BlurLineHozMatteComplex<uint16_t, unsigned int>((uint16_t *)pSrcLine, (uint16_t *)pDstLine, 0, m_nBlurHozLeftUnsafeRegionEnd, m_tabKernelHoz);
        else if (m_eBitDepth == keNvcBitDepthFloat32)
            BlurLineHozMatteComplex<float, float>((float *)pSrcLine, (float *)pDstLine, 0, m_nBlurHozLeftUnsafeRegionEnd, m_tabKernelHozFloat);
	} else {
		if (m_eBitDepth == keNvcBitDepthInt8)
			BlurLineHozBGRAComplex<uint8_t, unsigned int>((uint8_t *)pSrcLine, (uint8_t *)pDstLine, 0, m_nBlurHozLeftUnsafeRegionEnd, m_tabKernelHoz);
		else if (m_eBitDepth == keNvcBitDepthInt16)
			BlurLineHozBGRAComplex<uint16_t, unsigned int>((uint16_t *)pSrcLine, (uint16_t *)pDstLine, 0, m_nBlurHozLeftUnsafeRegionEnd, m_tabKernelHoz);
        else if (m_eBitDepth == keNvcBitDepthFloat32)
            BlurLineHozBGRAComplex<float, float>((float *)pSrcLine, (float *)pDstLine, 0, m_nBlurHozLeftUnsafeRegionEnd, m_tabKernelHozFloat);
	}

	//
	// process safe region
	//
	if (m_bIsDataMatte) {
		if (m_eBitDepth == keNvcBitDepthInt8)
			BlurLineHozMatte8Bit((uint8_t *)pSrcLine, (uint8_t *)pDstLine, m_nBlurHozLeftUnsafeRegionEnd, m_nBlurHozRightUnsafeRegionStart);
		else if (m_eBitDepth == keNvcBitDepthInt16)
			BlurLineHozMatte16Bit((uint16_t *)pSrcLine, (uint16_t *)pDstLine, m_nBlurHozLeftUnsafeRegionEnd, m_nBlurHozRightUnsafeRegionStart);
        else if (m_eBitDepth == keNvcBitDepthFloat32)
            BlurLineHozMatte32Bit((float *)pSrcLine, (float *)pDstLine, m_nBlurHozLeftUnsafeRegionEnd, m_nBlurHozRightUnsafeRegionStart);
	} else {
		if (m_eBitDepth == keNvcBitDepthInt8)
			BlurLineHozBGRA8Bit((uint8_t *)pSrcLine, (uint8_t *)pDstLine, m_nBlurHozLeftUnsafeRegionEnd, m_nBlurHozRightUnsafeRegionStart);
		else if (m_eBitDepth == keNvcBitDepthInt16)
			BlurLineHozBGRA16Bit((uint16_t *)pSrcLine, (uint16_t *)pDstLine, m_nBlurHozLeftUnsafeRegionEnd, m_nBlurHozRightUnsafeRegionStart);
        else if (m_eBitDepth == keNvcBitDepthFloat32)
            BlurLineHozBGRA32Bit((float *)pSrcLine, (float *)pDstLine, m_nBlurHozLeftUnsafeRegionEnd, m_nBlurHozRightUnsafeRegionStart);
	}

	//
	// process left unsafe region
	//
	if (m_bIsDataMatte) {
		if (m_eBitDepth == keNvcBitDepthInt8)
			BlurLineHozMatteComplex<uint8_t, unsigned int>((uint8_t *)pSrcLine, (uint8_t *)pDstLine, m_nBlurHozRightUnsafeRegionStart, m_nWidth, m_tabKernelHoz);
		else if (m_eBitDepth == keNvcBitDepthInt16)
			BlurLineHozMatteComplex<uint16_t, unsigned int>((uint16_t *)pSrcLine, (uint16_t *)pDstLine, m_nBlurHozRightUnsafeRegionStart, m_nWidth, m_tabKernelHoz);
        else if (m_eBitDepth == keNvcBitDepthFloat32)
            BlurLineHozMatteComplex<float, float>((float *)pSrcLine, (float *)pDstLine, m_nBlurHozRightUnsafeRegionStart, m_nWidth, m_tabKernelHozFloat);
	} else {
		if (m_eBitDepth == keNvcBitDepthInt8)
			BlurLineHozBGRAComplex<uint8_t, unsigned int>((uint8_t *)pSrcLine, (uint8_t *)pDstLine, m_nBlurHozRightUnsafeRegionStart, m_nWidth, m_tabKernelHoz);
		else if (m_eBitDepth == keNvcBitDepthInt16)
			BlurLineHozBGRAComplex<uint16_t, unsigned int>((uint16_t *)pSrcLine, (uint16_t *)pDstLine, m_nBlurHozRightUnsafeRegionStart, m_nWidth, m_tabKernelHoz);
        else if (m_eBitDepth == keNvcBitDepthFloat32)
            BlurLineHozBGRAComplex<float, float>((float *)pSrcLine, (float *)pDstLine, m_nBlurHozRightUnsafeRegionStart, m_nWidth, m_tabKernelHozFloat);
	}
}

#define	__GBLUR_MULTIPLY_FACTOR(xmm_element, xmm_factor, xmm_mul1, xmm_mul2)	\
{																				\
	__m128i __xmm0;																\
																				\
	xmm_mul1 = _mm_mullo_epi16(xmm_element, xmm_factor);						\
	__xmm0 = _mm_mulhi_epu16(xmm_element, xmm_factor);							\
	xmm_mul2 = _mm_unpackhi_epi16(xmm_mul1, __xmm0);							\
	xmm_mul1 = _mm_unpacklo_epi16(xmm_mul1, __xmm0);							\
}

#define __GBLUR_SHIFT_MATTE_TO_TAIL(xmm_matte1, xmm_matte2, matte)	\
{																	\
	xmm_matte1 = _mm_srli_si128(xmm_matte1, 2);						\
	xmm_matte1 = _mm_insert_epi16(xmm_matte1, _mm_extract_epi16(xmm_matte2, 0), 7);		\
	xmm_matte2 = _mm_srli_si128(xmm_matte2, 2);						\
	xmm_matte2 = _mm_insert_epi16(xmm_matte2, (int)(matte), 7);		\
}

#define __GBLUR_SHIFT_MATTE_TO_HEAD(xmm_matte1, xmm_matte2, matte)	\
{																	\
	xmm_matte2 = _mm_slli_si128(xmm_matte2, 2);						\
	xmm_matte2 = _mm_insert_epi16(xmm_matte2, _mm_extract_epi16(xmm_matte1, 7), 0);		\
	xmm_matte1 = _mm_slli_si128(xmm_matte1, 2);						\
	xmm_matte1 = _mm_insert_epi16(xmm_matte1, (int)matte, 0);		\
}


static const __m128i g_xmm_rounding = _mm_set1_epi32(1 << (__KERNEL_SCALE_BITS - 1));


void CNvcGaussBlur::BlurLineHozMatte8Bit(const uint8_t *pSrcLine,
										 uint8_t *pDstLine,
										 unsigned int nStartPixel,
										 unsigned int nEndPixel)
{
	const __m128i xmm_zero = _mm_setzero_si128();
	unsigned int i, nGroupCount;
	const uint8_t *pSrc;
	uint8_t *pDst;

	nGroupCount = (nEndPixel - nStartPixel) / 16;
	pSrc = pSrcLine + nStartPixel;
	pDst = pDstLine + nStartPixel;

	for (i = 0; i < nGroupCount; i++) {
		// process 16 pixels every time
		__m128i xmm_sum1, xmm_sum2, xmm_sum3, xmm_sum4;
		__m128i xmm_pixels1, xmm_pixels2;

		xmm_pixels1 = _mm_load_si128((__m128i *)pSrc);
		xmm_pixels2 = _mm_unpackhi_epi8(xmm_pixels1, xmm_zero);
		xmm_pixels1 = _mm_unpacklo_epi8(xmm_pixels1, xmm_zero);

		__GBLUR_MULTIPLY_FACTOR(xmm_pixels1, m_xmm_kernel_table_hoz[m_nBlurWidth], xmm_sum1, xmm_sum2);
		__GBLUR_MULTIPLY_FACTOR(xmm_pixels2, m_xmm_kernel_table_hoz[m_nBlurWidth], xmm_sum3, xmm_sum4);

		__m128i xmm_left_pixels1, xmm_left_pixels2, xmm_right_pixels1, xmm_right_pixels2;
		const uint8_t *pLeftPixels, *pRightPixels;
		pLeftPixels = pSrc - 1;
		pRightPixels = pSrc + 16;

		xmm_left_pixels1 = xmm_pixels1;
		xmm_left_pixels2 = xmm_pixels2;
		xmm_right_pixels1 = xmm_pixels1;
		xmm_right_pixels2 = xmm_pixels2;

		for (int j = m_nBlurWidth - 1; j >= 0; j--, pLeftPixels--, pRightPixels++) {
            __GBLUR_SHIFT_MATTE_TO_HEAD(xmm_left_pixels1, xmm_left_pixels2, pLeftPixels[0]);
            __GBLUR_SHIFT_MATTE_TO_TAIL(xmm_right_pixels1, xmm_right_pixels2, pRightPixels[0]);

			xmm_pixels1 = _mm_add_epi16(xmm_left_pixels1, xmm_right_pixels1);
			xmm_pixels2 = _mm_add_epi16(xmm_left_pixels2, xmm_right_pixels2);

			__m128i xmm_mul1, xmm_mul2;

			__GBLUR_MULTIPLY_FACTOR(xmm_pixels1, m_xmm_kernel_table_hoz[j], xmm_mul1, xmm_mul2);
			xmm_sum1 = _mm_add_epi32(xmm_sum1, xmm_mul1);
			xmm_sum2 = _mm_add_epi32(xmm_sum2, xmm_mul2);

			__GBLUR_MULTIPLY_FACTOR(xmm_pixels2, m_xmm_kernel_table_hoz[j], xmm_mul1, xmm_mul2);
			xmm_sum3 = _mm_add_epi32(xmm_sum3, xmm_mul1);
			xmm_sum4 = _mm_add_epi32(xmm_sum4, xmm_mul2);
		}

		xmm_sum1 = _mm_add_epi32(xmm_sum1, g_xmm_rounding);
		xmm_sum1 = _mm_srli_epi32(xmm_sum1, __KERNEL_SCALE_BITS);
		xmm_sum2 = _mm_add_epi32(xmm_sum2, g_xmm_rounding);
		xmm_sum2 = _mm_srli_epi32(xmm_sum2, __KERNEL_SCALE_BITS);
		xmm_sum3 = _mm_add_epi32(xmm_sum3, g_xmm_rounding);
		xmm_sum3 = _mm_srli_epi32(xmm_sum3, __KERNEL_SCALE_BITS);
		xmm_sum4 = _mm_add_epi32(xmm_sum4, g_xmm_rounding);
		xmm_sum4 = _mm_srli_epi32(xmm_sum4, __KERNEL_SCALE_BITS);

		xmm_sum1 = _mm_packs_epi32(xmm_sum1, xmm_sum2);
		xmm_sum3 = _mm_packs_epi32(xmm_sum3, xmm_sum4);
		xmm_sum1 = _mm_packus_epi16(xmm_sum1, xmm_sum3);
		_mm_store_si128((__m128i *)pDst, xmm_sum1);

		pSrc += 16;
		pDst += 16;
	}
}

void CNvcGaussBlur::BlurLineHozMatte16Bit(const uint16_t *pSrcLine,
										  uint16_t *pDstLine,
										  unsigned int nStartPixel,
										  unsigned int nEndPixel)
{
	unsigned int i, nGroupCount;
	const uint16_t *pSrc;
	uint16_t *pDst;

	nGroupCount = (nEndPixel - nStartPixel) / 8;
	pSrc = pSrcLine + nStartPixel;
	pDst = pDstLine + nStartPixel;

	for (i = 0; i < nGroupCount; i++) {
		// process 8 pixels every time
		__m128i xmm_sum1, xmm_sum2;
		__m128i xmm_pixels;

		xmm_pixels = _mm_load_si128((__m128i *)pSrc);

		__GBLUR_MULTIPLY_FACTOR(xmm_pixels, m_xmm_kernel_table_hoz[m_nBlurWidth], xmm_sum1, xmm_sum2);

		__m128i xmm_left_pixels, xmm_right_pixels;
		const uint16_t *pLeftPixels, *pRightPixels;

		pLeftPixels = pSrc - 1;
		pRightPixels = pSrc + 8;

		xmm_left_pixels = xmm_pixels;
		xmm_right_pixels = xmm_pixels;

		for (int j = m_nBlurWidth - 1; j >= 0; j--, pLeftPixels--, pRightPixels++) {
            xmm_left_pixels = _mm_slli_si128(xmm_left_pixels, 2);
            xmm_left_pixels = _mm_insert_epi16(xmm_left_pixels, pLeftPixels[0], 0);

            xmm_right_pixels = _mm_srli_si128(xmm_right_pixels, 2);
            xmm_right_pixels = _mm_insert_epi16(xmm_right_pixels, pRightPixels[0], 7);

			__m128i xmm_mul1, xmm_mul2;

			__GBLUR_MULTIPLY_FACTOR(xmm_left_pixels, m_xmm_kernel_table_hoz[j], xmm_mul1, xmm_mul2);
			xmm_sum1 = _mm_add_epi32(xmm_sum1, xmm_mul1);
			xmm_sum2 = _mm_add_epi32(xmm_sum2, xmm_mul2);

			__GBLUR_MULTIPLY_FACTOR(xmm_right_pixels, m_xmm_kernel_table_hoz[j], xmm_mul1, xmm_mul2);
			xmm_sum1 = _mm_add_epi32(xmm_sum1, xmm_mul1);
			xmm_sum2 = _mm_add_epi32(xmm_sum2, xmm_mul2);
		}

		xmm_sum1 = _mm_add_epi32(xmm_sum1, g_xmm_rounding);
		xmm_sum1 = _mm_srli_epi32(xmm_sum1, __KERNEL_SCALE_BITS);
		xmm_sum2 = _mm_add_epi32(xmm_sum2, g_xmm_rounding);
		xmm_sum2 = _mm_srli_epi32(xmm_sum2, __KERNEL_SCALE_BITS);

		xmm_sum1 = _mm_sub_epi32(xmm_sum1, g_xmm_32bit_32768);
		xmm_sum2 = _mm_sub_epi32(xmm_sum2, g_xmm_32bit_32768);
		xmm_sum1 = _mm_packs_epi32(xmm_sum1, xmm_sum2);
		xmm_sum1 = _mm_add_epi16(xmm_sum1, g_xmm_16bit_32768);
		_mm_store_si128((__m128i *)pDst, xmm_sum1);

		pSrc += 8;
		pDst += 8;
	}
}

void CNvcGaussBlur::BlurLineHozMatte32Bit(const float *pSrcLine,
                                          float *pDstLine,
                                          unsigned int nStartPixel,
                                          unsigned int nEndPixel)
{
    const float *pSrc = pSrcLine + nStartPixel;
    float *pDst = pDstLine + nStartPixel;

    unsigned int nGroupCount = (nEndPixel - nStartPixel) / 4;
    for (unsigned int i = 0; i < nGroupCount; i++) {
        // process 4 pixels every time
        __m128 xmm_pixels_ps = _mm_load_ps(pSrc);
        __m128 xmm_sum_ps = _mm_mul_ps(xmm_pixels_ps, m_xmm_kernel_table_hoz_ps[m_nBlurWidth]);

        const float *pLeftPixels, *pRightPixels;
        pLeftPixels = pSrc - 1;
        pRightPixels = pSrc + 4;

        __m128 xmm_left_pixels_ps, xmm_right_pixels_ps;
        xmm_left_pixels_ps = xmm_pixels_ps;
        xmm_right_pixels_ps = xmm_pixels_ps;

        for (int j = m_nBlurWidth - 1; j >= 0; j--, pLeftPixels--, pRightPixels++) {
            __m128 xmm_tmp_ps = _mm_load_ss(pLeftPixels);
            xmm_left_pixels_ps = _mm_shuffle_ps(xmm_left_pixels_ps, xmm_left_pixels_ps, 0x90);
            xmm_left_pixels_ps = _mm_move_ss(xmm_left_pixels_ps, xmm_tmp_ps);

            xmm_tmp_ps = _mm_load_ss(pRightPixels);
            xmm_right_pixels_ps = _mm_move_ss(xmm_right_pixels_ps, xmm_tmp_ps);
            xmm_right_pixels_ps = _mm_shuffle_ps(xmm_right_pixels_ps, xmm_right_pixels_ps, 0x39);

            xmm_tmp_ps = _mm_add_ps(xmm_left_pixels_ps, xmm_right_pixels_ps);
            xmm_sum_ps = _mm_add_ps(xmm_sum_ps, _mm_mul_ps(xmm_tmp_ps, m_xmm_kernel_table_hoz_ps[j]));
        }

        _mm_store_ps(pDst, xmm_sum_ps);

        pSrc += 4;
        pDst += 4;
    }
}

template<typename T, typename CALC_T>
void CNvcGaussBlur::BlurLineHozMatteComplex(const T *pSrcLine,
                                            T *pDstLine,
                                            unsigned int nStartPixel,
                                            unsigned int nEndPixel,
                                            const CALC_T *pKernelTable)
{
	T nLeftGuard, nRightGuard;
	int i, j;

	nLeftGuard = pSrcLine[0];
	nRightGuard = pSrcLine[m_nWidth - 1];

	for (i = nStartPixel; i < (int)nEndPixel; i++) {
		int k = i - m_nBlurWidth, l = i + m_nBlurWidth;
		CALC_T sum = 0;

		for (j = 0; j < (int)m_nBlurWidth; j++, k++, l--) {
			CALC_T matte;

			if (k >= 0)
				matte = pSrcLine[k];
			else
				matte = nLeftGuard;

			if (l < (int)m_nWidth)
				matte += pSrcLine[l];
			else
				matte += nRightGuard;

			sum += pKernelTable[j] * matte;
		}

		sum += pSrcLine[i] * pKernelTable[j];
        if (sizeof(T) != 4)
		    pDstLine[i] = (T)((sum + (__KERNEL_SCALE / 2)) / __KERNEL_SCALE);
        else
            pDstLine[i] = (T)sum;
	}
}

#define __GBLUR_SHIFT_8BIT_BGRA_TO_TAIL(xmm_bgra1, xmm_bgra2, bgra, xmm_zero)	\
{																				\
	xmm_bgra1 = _mm_srli_si128(xmm_bgra1, 8);									\
	xmm_bgra1 = _mm_unpacklo_epi64(xmm_bgra1, xmm_bgra2);						\
																				\
	__m128i __xmm0;																\
	__xmm0 = _mm_cvtsi32_si128(bgra);											\
	__xmm0 = _mm_unpacklo_epi8(__xmm0, xmm_zero);								\
	xmm_bgra2 = _mm_srli_si128(xmm_bgra2, 8);									\
	xmm_bgra2 = _mm_unpacklo_epi64(xmm_bgra2, __xmm0);							\
}

#define __GBLUR_SHIFT_8BIT_BGRA_TO_HEAD(xmm_bgra1, xmm_bgra2, bgra, xmm_zero)	\
{																				\
	xmm_bgra2 = _mm_slli_si128(xmm_bgra2, 8);									\
	xmm_bgra2 = _mm_unpackhi_epi64(xmm_bgra1, xmm_bgra2);						\
																				\
	__m128i __xmm0;																\
	__xmm0 = _mm_cvtsi32_si128(bgra);											\
	__xmm0 = _mm_unpacklo_epi8(__xmm0, xmm_zero);								\
	xmm_bgra1 = _mm_slli_si128(xmm_bgra1, 8);									\
	xmm_bgra1 = _mm_or_si128(xmm_bgra1, __xmm0);								\
}

void CNvcGaussBlur::BlurLineHozBGRA8Bit(const uint8_t *pSrcLine,
										uint8_t *pDstLine,
										unsigned int nStartPixel,
										unsigned int nEndPixel)
{
	const __m128i xmm_zero = _mm_setzero_si128();
	unsigned int i, nGroupCount;
	const uint8_t *pSrc;
	uint8_t *pDst;

	nGroupCount = (nEndPixel - nStartPixel) / 4;
	pSrc = pSrcLine + nStartPixel * 4;
	pDst = pDstLine + nStartPixel * 4;

	for (i = 0; i < nGroupCount; i++) {
		// process 4 pixels every time
		__m128i xmm_sum1, xmm_sum2, xmm_sum3, xmm_sum4;
		__m128i xmm_pixels1, xmm_pixels2;

		xmm_pixels1 = _mm_load_si128((__m128i *)pSrc);
		xmm_pixels2 = _mm_unpackhi_epi8(xmm_pixels1, xmm_zero);
		xmm_pixels1 = _mm_unpacklo_epi8(xmm_pixels1, xmm_zero);

		__GBLUR_MULTIPLY_FACTOR(xmm_pixels1, m_xmm_kernel_table_hoz[m_nBlurWidth], xmm_sum1, xmm_sum2);
		__GBLUR_MULTIPLY_FACTOR(xmm_pixels2, m_xmm_kernel_table_hoz[m_nBlurWidth], xmm_sum3, xmm_sum4);

		__m128i xmm_left_pixels1, xmm_left_pixels2, xmm_right_pixels1, xmm_right_pixels2;
		const uint8_t *pLeftPixels, *pRightPixels;
		pLeftPixels = pSrc - 4;
		pRightPixels = pSrc + 16;

		xmm_left_pixels1 = xmm_pixels1;
		xmm_left_pixels2 = xmm_pixels2;
		xmm_right_pixels1 = xmm_pixels1;
		xmm_right_pixels2 = xmm_pixels2;

		for (int j = m_nBlurWidth - 1; j >= 0; j--, pLeftPixels -= 4, pRightPixels += 4) {
            __GBLUR_SHIFT_8BIT_BGRA_TO_HEAD(xmm_left_pixels1, xmm_left_pixels2, *(unsigned int *)pLeftPixels, xmm_zero);
            __GBLUR_SHIFT_8BIT_BGRA_TO_TAIL(xmm_right_pixels1, xmm_right_pixels2, *(unsigned int *)pRightPixels, xmm_zero);

			xmm_pixels1 = _mm_add_epi16(xmm_left_pixels1, xmm_right_pixels1);
			xmm_pixels2 = _mm_add_epi16(xmm_left_pixels2, xmm_right_pixels2);

			__m128i xmm_mul1, xmm_mul2;

			__GBLUR_MULTIPLY_FACTOR(xmm_pixels1, m_xmm_kernel_table_hoz[j], xmm_mul1, xmm_mul2);
			xmm_sum1 = _mm_add_epi32(xmm_sum1, xmm_mul1);
			xmm_sum2 = _mm_add_epi32(xmm_sum2, xmm_mul2);

			__GBLUR_MULTIPLY_FACTOR(xmm_pixels2, m_xmm_kernel_table_hoz[j], xmm_mul1, xmm_mul2);
			xmm_sum3 = _mm_add_epi32(xmm_sum3, xmm_mul1);
			xmm_sum4 = _mm_add_epi32(xmm_sum4, xmm_mul2);
		}

		xmm_sum1 = _mm_add_epi32(xmm_sum1, g_xmm_rounding);
		xmm_sum1 = _mm_srli_epi32(xmm_sum1, __KERNEL_SCALE_BITS);
		xmm_sum2 = _mm_add_epi32(xmm_sum2, g_xmm_rounding);
		xmm_sum2 = _mm_srli_epi32(xmm_sum2, __KERNEL_SCALE_BITS);
		xmm_sum3 = _mm_add_epi32(xmm_sum3, g_xmm_rounding);
		xmm_sum3 = _mm_srli_epi32(xmm_sum3, __KERNEL_SCALE_BITS);
		xmm_sum4 = _mm_add_epi32(xmm_sum4, g_xmm_rounding);
		xmm_sum4 = _mm_srli_epi32(xmm_sum4, __KERNEL_SCALE_BITS);

		xmm_sum1 = _mm_packs_epi32(xmm_sum1, xmm_sum2);
		xmm_sum3 = _mm_packs_epi32(xmm_sum3, xmm_sum4);
		xmm_sum1 = _mm_packus_epi16(xmm_sum1, xmm_sum3);
		_mm_store_si128((__m128i *)pDst, xmm_sum1);

		pSrc += 16;
		pDst += 16;
	}
}

#define __GBLUR_SHIFT_16BIT_BGRA_TO_TAIL(xmm_bgra, bgra)	\
{															\
	xmm_bgra = _mm_srli_si128(xmm_bgra, 8);					\
	__m128i __xmm0 = _mm_loadl_epi64((__m128i *)(bgra));	\
	xmm_bgra = _mm_unpacklo_epi64(xmm_bgra, __xmm0);		\
}

#define __GBLUR_SHIFT_16BIT_BGRA_TO_HEAD(xmm_bgra, bgra)	\
{															\
	__m128i __xmm0 = _mm_loadl_epi64((__m128i *)(bgra));	\
	xmm_bgra = _mm_unpacklo_epi64(__xmm0, xmm_bgra);		\
}

void CNvcGaussBlur::BlurLineHozBGRA16Bit(const uint16_t *pSrcLine,
										 uint16_t *pDstLine,
										 unsigned int nStartPixel,
										 unsigned int nEndPixel)
{
	unsigned int i, nGroupCount;
	const uint16_t *pSrc;
	uint16_t *pDst;

	nGroupCount = (nEndPixel - nStartPixel) / 2;
	pSrc = pSrcLine + nStartPixel * 4;
	pDst = pDstLine + nStartPixel * 4;

	for (i = 0; i < nGroupCount; i++) {
		// process 2 pixels every time
		__m128i xmm_sum1, xmm_sum2;
		__m128i xmm_pixels;

		xmm_pixels = _mm_load_si128((__m128i *)pSrc);

		__GBLUR_MULTIPLY_FACTOR(xmm_pixels, m_xmm_kernel_table_hoz[m_nBlurWidth], xmm_sum1, xmm_sum2);

		__m128i xmm_left_pixels, xmm_right_pixels;
		const uint16_t *pLeftPixels, *pRightPixels;
		pLeftPixels = pSrc - 4;
		pRightPixels = pSrc + 8;

		xmm_left_pixels = xmm_pixels;
		xmm_right_pixels = xmm_pixels;

		for (int j = m_nBlurWidth - 1; j >= 0; j--, pLeftPixels -= 4, pRightPixels += 4) {
            __GBLUR_SHIFT_16BIT_BGRA_TO_HEAD(xmm_left_pixels, pLeftPixels);
            __GBLUR_SHIFT_16BIT_BGRA_TO_TAIL(xmm_right_pixels, pRightPixels);

			__m128i xmm_mul1, xmm_mul2;

			__GBLUR_MULTIPLY_FACTOR(xmm_left_pixels, m_xmm_kernel_table_hoz[j], xmm_mul1, xmm_mul2);
			xmm_sum1 = _mm_add_epi32(xmm_sum1, xmm_mul1);
			xmm_sum2 = _mm_add_epi32(xmm_sum2, xmm_mul2);

			__GBLUR_MULTIPLY_FACTOR(xmm_right_pixels, m_xmm_kernel_table_hoz[j], xmm_mul1, xmm_mul2);
			xmm_sum1 = _mm_add_epi32(xmm_sum1, xmm_mul1);
			xmm_sum2 = _mm_add_epi32(xmm_sum2, xmm_mul2);
		}

		xmm_sum1 = _mm_add_epi32(xmm_sum1, g_xmm_rounding);
		xmm_sum1 = _mm_srli_epi32(xmm_sum1, __KERNEL_SCALE_BITS);
		xmm_sum2 = _mm_add_epi32(xmm_sum2, g_xmm_rounding);
		xmm_sum2 = _mm_srli_epi32(xmm_sum2, __KERNEL_SCALE_BITS);

		xmm_sum1 = _mm_sub_epi32(xmm_sum1, g_xmm_32bit_32768);
		xmm_sum2 = _mm_sub_epi32(xmm_sum2, g_xmm_32bit_32768);
		xmm_sum1 = _mm_packs_epi32(xmm_sum1, xmm_sum2);
		xmm_sum1 = _mm_add_epi16(xmm_sum1, g_xmm_16bit_32768);
		_mm_store_si128((__m128i *)pDst, xmm_sum1);

		pSrc += 8;
		pDst += 8;
	}
}

void CNvcGaussBlur::BlurLineHozBGRA32Bit(const float *pSrcLine,
                                         float *pDstLine,
                                         unsigned int nStartPixel,
                                         unsigned int nEndPixel)
{
    const float *pSrc = pSrcLine + nStartPixel * 4;
    float *pDst = pDstLine + nStartPixel * 4;

    const unsigned int nPixelCount = nEndPixel - nStartPixel;
    for (unsigned int i = 0; i < nPixelCount; i++) {
        // process 1 pixel every time
        const __m128 xmm_pixel_ps = _mm_load_ps(pSrc);
        __m128 xmm_sum_ps = _mm_mul_ps(xmm_pixel_ps, m_xmm_kernel_table_hoz_ps[m_nBlurWidth]);

        const float *pLeftPixel, *pRightPixel;
        pLeftPixel = pSrc - 4;
        pRightPixel = pSrc + 4;
        for (int j = m_nBlurWidth - 1; j >= 0; j--, pLeftPixel -= 4, pRightPixel += 4) {
            __m128 xmm_left_pixel = _mm_load_ps(pLeftPixel);
            __m128 xmm_right_pixel = _mm_load_ps(pRightPixel);
            xmm_left_pixel = _mm_add_ps(xmm_left_pixel, xmm_right_pixel);
            xmm_sum_ps = _mm_add_ps(xmm_sum_ps, _mm_mul_ps(xmm_left_pixel, m_xmm_kernel_table_hoz_ps[j]));
        }

        _mm_store_ps(pDst, xmm_sum_ps);

        pSrc += 4;
        pDst += 4;
    }
}

template<typename T, typename CALC_T>
void CNvcGaussBlur::BlurLineHozBGRAComplex(const T *pSrcLine,
                                           T *pDstLine,
                                           unsigned int nStartPixel,
                                           unsigned int nEndPixel,
                                           const CALC_T *pKernelTable)
{
	T nLeftGuardRed, nRightGuardRed;
	T nLeftGuardGreen, nRightGuardGreen;
	T nLeftGuardBlue, nRightGuardBlue;
	T nLeftGuardAlpha, nRightGuardAlpha;
	int i, j;

	nLeftGuardBlue = pSrcLine[0];
	nLeftGuardGreen = pSrcLine[1];
	nLeftGuardRed = pSrcLine[2];
	nLeftGuardAlpha = pSrcLine[3];
	i = m_nWidth - 1;
	nRightGuardBlue = pSrcLine[i * 4 + 0];
	nRightGuardGreen = pSrcLine[i * 4 + 1];
	nRightGuardRed = pSrcLine[i * 4 + 2];
	nRightGuardAlpha = pSrcLine[i * 4 + 3];

	for (i = nStartPixel; i < (int)nEndPixel; i++) {
		CALC_T sum_r = 0, sum_g = 0, sum_b = 0, sum_a = 0, coef;
		int k = i - m_nBlurWidth, l = i + m_nBlurWidth;

		for (j = 0; j < (int)m_nBlurWidth; j++, k++, l--) {
			CALC_T r, g, b, a;

			if (k >= 0) {
				b = pSrcLine[k * 4 + 0];
				g = pSrcLine[k * 4 + 1];
				r = pSrcLine[k * 4 + 2];
				a = pSrcLine[k * 4 + 3];
			} else {
				b = nLeftGuardBlue;
				g = nLeftGuardGreen;
				r = nLeftGuardRed;
				a = nLeftGuardAlpha;
			}

			if (l < (int)m_nWidth) {
				b += pSrcLine[l * 4 + 0];
				g += pSrcLine[l * 4 + 1];
				r += pSrcLine[l * 4 + 2];
				a += pSrcLine[l * 4 + 3];
			} else {
				b += nRightGuardBlue;
				g += nRightGuardGreen;
				r += nRightGuardRed;
				a += nRightGuardAlpha;
			}

			coef = pKernelTable[j];

			sum_b += b * coef;
			sum_g += g * coef;
			sum_r += r * coef;
			sum_a += a * coef;
		}

		coef = pKernelTable[j];
		sum_b += pSrcLine[i * 4 + 0] * coef;
		sum_g += pSrcLine[i * 4 + 1] * coef;
		sum_r += pSrcLine[i * 4 + 2] * coef;
		sum_a += pSrcLine[i * 4 + 3] * coef;

        if (sizeof(T) != 4) {
            pDstLine[i * 4 + 0] = (T)((sum_b + (__KERNEL_SCALE / 2)) / __KERNEL_SCALE);
            pDstLine[i * 4 + 1] = (T)((sum_g + (__KERNEL_SCALE / 2)) / __KERNEL_SCALE);
            pDstLine[i * 4 + 2] = (T)((sum_r + (__KERNEL_SCALE / 2)) / __KERNEL_SCALE);
            pDstLine[i * 4 + 3] = (T)((sum_a + (__KERNEL_SCALE / 2)) / __KERNEL_SCALE);
        } else {
            pDstLine[i * 4 + 0] = (T)sum_b;
            pDstLine[i * 4 + 1] = (T)sum_g;
            pDstLine[i * 4 + 2] = (T)sum_r;
            pDstLine[i * 4 + 3] = (T)sum_a;
        }
	}
}

void CNvcGaussBlur::BlurLineHozWithOverlapAdd(const void *pSrcLine,
											  void *pDstLine,
											  float *pTmpLine,
											  float *pReal,
											  float *pImg)
{
	if (m_bIsDataMatte) {
		if (m_eBitDepth == keNvcBitDepthInt8)
			BlurMatteLineHozWithOverlapAdd<uint8_t>((uint8_t *)pSrcLine, (uint8_t *)pDstLine, pTmpLine, pReal, pImg);
		else if (m_eBitDepth == keNvcBitDepthInt16)
			BlurMatteLineHozWithOverlapAdd<uint16_t>((uint16_t *)pSrcLine, (uint16_t *)pDstLine, pTmpLine, pReal, pImg);
        else if (m_eBitDepth == keNvcBitDepthFloat32)
            BlurMatteLineHozWithOverlapAdd<float>((float *)pSrcLine, (float *)pDstLine, pTmpLine, pReal, pImg);
	} else {
		if (m_eBitDepth == keNvcBitDepthInt8)
			BlurBGRALineHozWithOverlapAdd<uint8_t>((uint8_t *)pSrcLine, (uint8_t *)pDstLine, pTmpLine, pReal, pImg);
		else if (m_eBitDepth == keNvcBitDepthInt16)
			BlurBGRALineHozWithOverlapAdd<uint16_t>((uint16_t *)pSrcLine, (uint16_t *)pDstLine, pTmpLine, pReal, pImg);
        else if (m_eBitDepth == keNvcBitDepthFloat32)
            BlurBGRALineHozWithOverlapAdd<float>((float *)pSrcLine, (float *)pDstLine, pTmpLine, pReal, pImg);
	}
}

template<typename T>
void CNvcGaussBlur::BlurMatteLineHozWithOverlapAdd(const T *pSrcLine,
												   T *pDstLine,
												   float *pTmpLine,
												   float *pReal,
												   float *pImg)
{
	//
	// the signal length in the whole convolution is actually IMAGE_WIDTH + KERNEL_RADIUS * 2
	// the signals in [0, KERNEL_RADIUS) is padded with the first original signal value
	// the signals in [IMAGE_WIDTH + KERNEL_RADIUS, IMAGE_WIDTH + KERNEL_RADIUS * 2) is padded with the last original signal value
	// the kernel length is KERNEL_RADIUS * 2 + 1
	// the final output signal length of the whole convolution is IMAGE_WIDTH + KERNEL_RADIUS * 4
	// but we only care about the values in [KERNEL_RADIUS * 2, KERNEL_RADIUS * 2 + IMAGE_WIDTH)
	//

	unsigned int nConvSize = 1 << m_nOverlapAddOrderHoz, i;
	unsigned int nStartIdx = 0;
	float *pLinePtr = pTmpLine;

	for (i = 0; i < m_nOverlapAddStepCountHoz; i++) {
		FillMatteSignalSequenceHoz<T>(pSrcLine, pReal, nStartIdx, nStartIdx + m_nOverlapAddStepSizeHoz);
		memset(pReal + m_nOverlapAddStepSizeHoz, 0, (nConvSize - m_nOverlapAddStepSizeHoz) * sizeof(float));

		if (i) {
			CircularConvolutionMatte<false>(
				pReal,
				pImg,
				(float *)m_oHozFFTFactorsBuffer.GetBufferPointer(0),
				(float *)m_oHozFFTFactorsBuffer.GetBufferPointer(0) + nConvSize,
				(float *)m_oHozKernelFFTCoefBuffer.GetBufferPointer(0),
				(float *)m_oHozKernelFFTCoefBuffer.GetBufferPointer(0) + nConvSize,
				pLinePtr,
				m_nOverlapAddOrderHoz,
				nConvSize - m_nOverlapAddStepSizeHoz);
		} else {
			CircularConvolutionMatte<true>(
				pReal,
				pImg,
				(float *)m_oHozFFTFactorsBuffer.GetBufferPointer(0),
				(float *)m_oHozFFTFactorsBuffer.GetBufferPointer(0) + nConvSize,
				(float *)m_oHozKernelFFTCoefBuffer.GetBufferPointer(0),
				(float *)m_oHozKernelFFTCoefBuffer.GetBufferPointer(0) + nConvSize,
				pLinePtr,
				m_nOverlapAddOrderHoz,
				nConvSize - m_nOverlapAddStepSizeHoz);
		}

		nStartIdx += m_nOverlapAddStepSizeHoz;
		pLinePtr += m_nOverlapAddStepSizeHoz;
	}

	//
	// copy convolution result to destination buffer
	//
    if (sizeof(T) == 4) {
        memcpy(pDstLine, pTmpLine + m_nBlurWidth * 2, m_nWidth * sizeof(float));
        return;
    }

    unsigned int nGroupCount, nTailCount;
    T *pDst = pDstLine;

    nGroupCount = m_nWidth / 4;
    nTailCount = m_nWidth & 3;
    pLinePtr = pTmpLine + m_nBlurWidth * 2;
    for (i = 0; i < nGroupCount; i++) {
        __m128i xmm_matte;

        xmm_matte = _mm_cvtps_epi32(_mm_loadu_ps(pLinePtr));
        if (sizeof(T) == 1) {
            xmm_matte = _mm_packs_epi32(xmm_matte, xmm_matte);
            xmm_matte = _mm_packus_epi16(xmm_matte, xmm_matte);
            *(int *)pDst = _mm_cvtsi128_si32(xmm_matte);
        } else {
            xmm_matte = _mm_sub_epi32(xmm_matte, g_xmm_32bit_32768);
            xmm_matte = _mm_packs_epi32(xmm_matte, xmm_matte);
            xmm_matte = _mm_add_epi16(xmm_matte, g_xmm_16bit_32768);
            _mm_storel_epi64((__m128i *)pDst, xmm_matte);
        }

        pLinePtr += 4;
        pDst += 4;
    }

    for (i = 0; i < nTailCount; i++) {
        *pDst = (T)_mm_cvtss_si32(_mm_load_ss(pLinePtr));

        pLinePtr++;
        pDst++;
    }
}

template<typename T>
void CNvcGaussBlur::FillMatteSignalSequenceHoz(const T *pSrcLine,
											   float *pSignal,
											   unsigned int nStartIdx,
											   unsigned int nEndIdx)
{
	unsigned int i, nCount;

	while (nStartIdx < nEndIdx) {
		if (nStartIdx < m_nBlurWidth) {
			// fill this region with the leftmost pixel in the current line
			float fLeftPad = (float)pSrcLine[0];

			nCount = NVC_MIN(nEndIdx, m_nBlurWidth) - nStartIdx;
			for (i = 0; i < nCount; i++)
				pSignal[i] = fLeftPad;

		} else if (nStartIdx < m_nWidth + m_nBlurWidth) {
			// fill this region with the pixels in the original line
			const T *pSrc = pSrcLine + nStartIdx - m_nBlurWidth;

			nCount = NVC_MIN(nEndIdx, m_nWidth + m_nBlurWidth) - nStartIdx;
			for (i = 0; i < nCount; i++)
				pSignal[i] = (float)pSrc[i];

		} else if (nStartIdx < m_nWidth + m_nBlurWidth * 2) {
			// fill this region with the rightmost pixel in the current line
			float fRightPad = (float)pSrcLine[m_nWidth - 1];

			nCount = NVC_MIN(nEndIdx, m_nWidth + m_nBlurWidth * 2) - nStartIdx;
			for (i = 0; i < nCount; i++)
				pSignal[i] = fRightPad;

		} else {
			// fill this region with zero
			nCount = nEndIdx - nStartIdx;
			memset(pSignal, 0, nCount * sizeof(float));
		}

		nStartIdx += nCount;
		pSignal += nCount;
	}
}


template<typename T>
void CNvcGaussBlur::BlurBGRALineHozWithOverlapAdd(const T *pSrcLine,
												  T *pDstLine,
												  float *pTmpLine,
												  float *pReal,
												  float *pImg)
{
	//
	// the signal length in the whole convolution is actually IMAGE_WIDTH + KERNEL_RADIUS * 2
	// the signals in [0, KERNEL_RADIUS) is padded with the first original signal value
	// the signals in [IMAGE_WIDTH + KERNEL_RADIUS, IMAGE_WIDTH + KERNEL_RADIUS * 2) is padded with the last original signal value
	// the kernel length is KERNEL_RADIUS * 2 + 1
	// the final output signal length of the whole convolution is IMAGE_WIDTH + KERNEL_RADIUS * 4
	// but we only care about the values in [KERNEL_RADIUS * 2, KERNEL_RADIUS * 2 + IMAGE_WIDTH)
	//

	unsigned int nConvSize = 1 << m_nOverlapAddOrderHoz, i;
	unsigned int nStartIdx = 0;
	float *pLinePtr = pTmpLine;

    for (i = 0; i < m_nOverlapAddStepCountHoz; i++) {
		FillBGRASignalSequenceHoz<T>(pSrcLine, pReal, nStartIdx, nStartIdx + m_nOverlapAddStepSizeHoz);
		memset(pReal + m_nOverlapAddStepSizeHoz * 4, 0, (nConvSize - m_nOverlapAddStepSizeHoz) * 4 * sizeof(float));

		if (i) {
			CircularConvolutionBGRA<false>(
				pReal,
				pImg,
				(float *)m_oHozFFTFactorsBuffer.GetBufferPointer(0),
				(float *)m_oHozFFTFactorsBuffer.GetBufferPointer(0) + nConvSize,
				(float *)m_oHozKernelFFTCoefBuffer.GetBufferPointer(0),
				(float *)m_oHozKernelFFTCoefBuffer.GetBufferPointer(0) + nConvSize,
				pLinePtr,
				m_nOverlapAddOrderHoz,
				nConvSize - m_nOverlapAddStepSizeHoz);
		} else {
			CircularConvolutionBGRA<true>(
				pReal,
				pImg,
				(float *)m_oHozFFTFactorsBuffer.GetBufferPointer(0),
				(float *)m_oHozFFTFactorsBuffer.GetBufferPointer(0) + nConvSize,
				(float *)m_oHozKernelFFTCoefBuffer.GetBufferPointer(0),
				(float *)m_oHozKernelFFTCoefBuffer.GetBufferPointer(0) + nConvSize,
				pLinePtr,
				m_nOverlapAddOrderHoz,
				nConvSize - m_nOverlapAddStepSizeHoz);
		}

		nStartIdx += m_nOverlapAddStepSizeHoz;
		pLinePtr += m_nOverlapAddStepSizeHoz * 4;
	}

	//
	// copy convolution result to destination buffer
	//
    if (sizeof(T) == 4) {
        memcpy(pDstLine, pTmpLine + m_nBlurWidth * 2 * 4, m_nWidth * 4 * sizeof(float));
        return;
    }

	unsigned int nGroupCount, nTailCount;
	T *pDst = pDstLine;

	nGroupCount = m_nWidth / 4;
	nTailCount = m_nWidth & 3;
	pLinePtr = pTmpLine + m_nBlurWidth * 2 * 4;
	for (i = 0; i < nGroupCount; i++) {
		__m128 xmm0_ps, xmm1_ps;
		__m128i xmm0, xmm1, xmm2;

		xmm0_ps = _mm_load_ps(pLinePtr);
		xmm1_ps = _mm_load_ps(pLinePtr + 4);
		xmm0 = _mm_cvtps_epi32(xmm0_ps);
		xmm1 = _mm_cvtps_epi32(xmm1_ps);

		if (sizeof(T) == 1) {
			xmm0 = _mm_packs_epi32(xmm0, xmm1);
		} else {
			xmm0 = _mm_sub_epi32(xmm0, g_xmm_32bit_32768);
			xmm1 = _mm_sub_epi32(xmm1, g_xmm_32bit_32768);
			xmm0 = _mm_packs_epi32(xmm0, xmm1);
			xmm0 = _mm_add_epi16(xmm0, g_xmm_16bit_32768);
			_mm_store_si128((__m128i *)pDst, xmm0);
		}

		xmm0_ps = _mm_load_ps(pLinePtr + 8);
		xmm1_ps = _mm_load_ps(pLinePtr + 12);
		xmm1 = _mm_cvtps_epi32(xmm0_ps);
		xmm2 = _mm_cvtps_epi32(xmm1_ps);

		if (sizeof(T) == 1) {
			xmm1 = _mm_packs_epi32(xmm1, xmm2);
			xmm0 = _mm_packus_epi16(xmm0, xmm1);
			_mm_store_si128((__m128i *)pDst, xmm0);
		} else {
			xmm1 = _mm_sub_epi32(xmm1, g_xmm_32bit_32768);
			xmm2 = _mm_sub_epi32(xmm2, g_xmm_32bit_32768);
			xmm1 = _mm_packs_epi32(xmm1, xmm2);
			xmm1 = _mm_add_epi16(xmm1, g_xmm_16bit_32768);
			_mm_store_si128((__m128i *)(pDst + 8), xmm1);
		}

		pLinePtr += 16;
		pDst += 16;
	}

	for (i = 0; i < nTailCount; i++) {
		__m128i xmm0 = _mm_cvtps_epi32(_mm_load_ps(pLinePtr));

		if (sizeof(T) == 1) {
			xmm0 = _mm_packs_epi32(xmm0, xmm0);
			xmm0 = _mm_packus_epi16(xmm0, xmm0);
			*(int *)pDst = _mm_cvtsi128_si32(xmm0);
		} else {
			xmm0 = _mm_sub_epi32(xmm0, g_xmm_32bit_32768);
			xmm0 = _mm_packs_epi32(xmm0, xmm0);
			xmm0 = _mm_add_epi16(xmm0, g_xmm_16bit_32768);
			_mm_storel_epi64((__m128i *)pDst, xmm0);
		}

		pLinePtr += 4;
		pDst += 4;
	}
}

template<typename T>
void CNvcGaussBlur::FillBGRASignalSequenceHoz(const T *pSrcLine,
											  float *pSignal,
											  unsigned int nStartIdx,
											  unsigned int nEndIdx)
{
	unsigned int i, nCount;

	while (nStartIdx < nEndIdx) {
		if (nStartIdx < m_nBlurWidth) {
			// fill this region with the leftmost pixel in the current line
			float fLeftPadBlue = (float)pSrcLine[0];
			float fLeftPadGreen = (float)pSrcLine[1];
			float fLeftPadRed = (float)pSrcLine[2];
			float fLeftPadAlpha = (float)pSrcLine[3];

			nCount = NVC_MIN(nEndIdx, m_nBlurWidth) - nStartIdx;
			for (i = 0; i < nCount; i++) {
				pSignal[i * 4 + 0] = fLeftPadBlue;
				pSignal[i * 4 + 1] = fLeftPadGreen;
				pSignal[i * 4 + 2] = fLeftPadRed;
				pSignal[i * 4 + 3] = fLeftPadAlpha;
			}

		} else if (nStartIdx < m_nWidth + m_nBlurWidth) {
			// fill this region with the pixels in the original line
			const T *pSrc = pSrcLine + (nStartIdx - m_nBlurWidth) * 4;

			nCount = NVC_MIN(nEndIdx, m_nWidth + m_nBlurWidth) - nStartIdx;
			for (i = 0; i < nCount; i++) {
				pSignal[i * 4 + 0] = (float)pSrc[i * 4 + 0];
				pSignal[i * 4 + 1] = (float)pSrc[i * 4 + 1];
				pSignal[i * 4 + 2] = (float)pSrc[i * 4 + 2];
				pSignal[i * 4 + 3] = (float)pSrc[i * 4 + 3];
			}

		} else if (nStartIdx < m_nWidth + m_nBlurWidth * 2) {
			// fill this region with the rightmost pixel in the current line
			float fRightPadBlue = (float)pSrcLine[(m_nWidth - 1) * 4 + 0];
			float fRightPadGreen = (float)pSrcLine[(m_nWidth - 1) * 4 + 1];
			float fRightPadRed = (float)pSrcLine[(m_nWidth - 1) * 4 + 2];
			float fRightPadAlpha = (float)pSrcLine[(m_nWidth - 1) * 4 + 3];

			nCount = NVC_MIN(nEndIdx, m_nWidth + m_nBlurWidth * 2) - nStartIdx;
			for (i = 0; i < nCount; i++) {
				pSignal[i * 4 + 0] = fRightPadBlue;
				pSignal[i * 4 + 1] = fRightPadGreen;
				pSignal[i * 4 + 2] = fRightPadRed;
				pSignal[i * 4 + 3] = fRightPadAlpha;
			}

		} else {
			// fill this region with zero
			nCount = nEndIdx - nStartIdx;
			memset(pSignal, 0, nCount * 4 * sizeof(float));
		}

		nStartIdx += nCount;
		pSignal += nCount * 4;
	}
}


// dst = (src1 + src2) * factor
void CNvcGaussBlur::BlurLineVerticallyFirstPass(const void *pSrcLine1,
												const void *pSrcLine2,
												unsigned int nKernelIndex,
												uint32_t *pDstLine,
												unsigned int nPixelCount)
{
	if (m_eBitDepth == keNvcBitDepthInt8) {
		BlurLineVerFirstPass8Bit(
			(uint8_t *)pSrcLine1,
			(uint8_t *)pSrcLine2,
			m_xmm_kernel_table_ver[nKernelIndex],
			m_tabKernelVer[nKernelIndex],
			pDstLine,
			nPixelCount * (m_bIsDataMatte ? 1 : 4));
	} else if (m_eBitDepth == keNvcBitDepthInt16) {
		BlurLineVerFirstPass16Bit(
			(uint16_t *)pSrcLine1,
			(uint16_t *)pSrcLine2,
            m_xmm_kernel_table_ver[nKernelIndex],
            m_tabKernelVer[nKernelIndex],
			pDstLine,
			nPixelCount * (m_bIsDataMatte ? 1 : 4));
	} else if (m_eBitDepth == keNvcBitDepthFloat32) {
        BlurLineVerFirstPass32Bit(
            (float *)pSrcLine1,
            (float *)pSrcLine2,
            m_xmm_kernel_table_ver_ps[nKernelIndex],
            m_tabKernelVerFloat[nKernelIndex],
            (float *)pDstLine,
            nPixelCount * (m_bIsDataMatte ? 1 : 4));
    }
}

// dst = dst + (src1 + src2) * factor
void CNvcGaussBlur::BlurLineVerticallyMedianPass(const void *pSrcLine1,
												 const void *pSrcLine2,
												 unsigned int nKernelIndex,
												 uint32_t *pDstLine,
												 unsigned int nPixelCount)
{
	if (m_eBitDepth == keNvcBitDepthInt8) {
		BlurLineVerMedianPass8Bit(
			(uint8_t *)pSrcLine1,
			(uint8_t *)pSrcLine2,
            m_xmm_kernel_table_ver[nKernelIndex],
            m_tabKernelVer[nKernelIndex],
			pDstLine,
			nPixelCount * (m_bIsDataMatte ? 1 : 4));
	} else if (m_eBitDepth == keNvcBitDepthInt16) {
		BlurLineVerMedianPass16Bit(
			(uint16_t *)pSrcLine1,
			(uint16_t *)pSrcLine2,
            m_xmm_kernel_table_ver[nKernelIndex],
            m_tabKernelVer[nKernelIndex],
			pDstLine,
			nPixelCount * (m_bIsDataMatte ? 1 : 4));
	} else if (m_eBitDepth == keNvcBitDepthFloat32) {
        BlurLineVerMedianPass32Bit(
            (float *)pSrcLine1,
            (float *)pSrcLine2,
            m_xmm_kernel_table_ver_ps[nKernelIndex],
            m_tabKernelVerFloat[nKernelIndex],
            (float *)pDstLine,
            nPixelCount * (m_bIsDataMatte ? 1 : 4));
    }
}

// dst = src1 * factor + src2
void CNvcGaussBlur::BlurLineVerticallyFinalPass(const void *pSrcLine1,
												const uint32_t *pSrcLine2,
												unsigned int nKernelIndex,
												void *pDstLine,
												unsigned int nPixelCount)
{
	if (m_eBitDepth == keNvcBitDepthInt8) {
		BlurLineVerFinalPass8Bit(
			(uint8_t *)pSrcLine1,
			pSrcLine2,
            m_xmm_kernel_table_ver[nKernelIndex],
            m_tabKernelVer[nKernelIndex],
			(uint8_t *)pDstLine,
			nPixelCount * (m_bIsDataMatte ? 1 : 4));
	} else if (m_eBitDepth == keNvcBitDepthInt16) {
		BlurLineVerFinalPass16Bit(
			(uint16_t *)pSrcLine1,
			pSrcLine2,
            m_xmm_kernel_table_ver[nKernelIndex],
            m_tabKernelVer[nKernelIndex],
			(uint16_t *)pDstLine,
			nPixelCount * (m_bIsDataMatte ? 1 : 4));
	} else if (m_eBitDepth == keNvcBitDepthFloat32) {
        BlurLineVerFinalPass32Bit(
            (float *)pSrcLine1,
            (float *)pSrcLine2,
            m_xmm_kernel_table_ver_ps[nKernelIndex],
            m_tabKernelVerFloat[nKernelIndex],
            (float *)pDstLine,
            nPixelCount * (m_bIsDataMatte ? 1 : 4));
    }
}

// dst = (src1 + src2) * factor
void CNvcGaussBlur::BlurLineVerFirstPass8Bit(const uint8_t *pSrcLine1,
											 const uint8_t *pSrcLine2,
											 __m128i& xmm_factor,
											 unsigned int nFactor,
											 uint32_t *pDstLine,
											 unsigned int nElementCount)
{
	const __m128i xmm_zero = _mm_setzero_si128();
	unsigned int i, nGroupCount, nTailCount;

	nGroupCount = nElementCount / 16;
	nTailCount = nElementCount & 15;

	for (i = 0; i < nGroupCount; i++) {
		// process 16 element every time
		__m128i xmm_element1, xmm_element2;
		__m128i xmm1, xmm2;

		xmm_element1 = _mm_load_si128((__m128i *)pSrcLine1);
		xmm_element2 = _mm_unpackhi_epi8(xmm_element1, xmm_zero);
		xmm_element1 = _mm_unpacklo_epi8(xmm_element1, xmm_zero);

		xmm1 = _mm_load_si128((__m128i *)pSrcLine2);
		xmm2 = _mm_unpackhi_epi8(xmm1, xmm_zero);
		xmm1 = _mm_unpacklo_epi8(xmm1, xmm_zero);

		xmm_element1 = _mm_add_epi16(xmm_element1, xmm1);
		xmm_element2 = _mm_add_epi16(xmm_element2, xmm2);

		__m128i xmm_mul1, xmm_mul2;

		__GBLUR_MULTIPLY_FACTOR(xmm_element1, xmm_factor, xmm_mul1, xmm_mul2);

		_mm_store_si128((__m128i *)pDstLine, xmm_mul1);
		_mm_store_si128((__m128i *)pDstLine + 1, xmm_mul2);

		__GBLUR_MULTIPLY_FACTOR(xmm_element2, xmm_factor, xmm_mul1, xmm_mul2);

		_mm_store_si128((__m128i *)pDstLine + 2, xmm_mul1);
		_mm_store_si128((__m128i *)pDstLine + 3, xmm_mul2);

		pSrcLine1 += 16;
		pSrcLine2 += 16;
		pDstLine += 16;
	}

	for (i = 0; i < nTailCount; i++)
		pDstLine[i] = ((uint32_t)pSrcLine1[i] + (uint32_t)pSrcLine2[i]) * (uint32_t)nFactor;
}

// dst = (src1 + src2) * factor
void CNvcGaussBlur::BlurLineVerFirstPass16Bit(const uint16_t *pSrcLine1,
											  const uint16_t *pSrcLine2,
											  __m128i& xmm_factor,
											  unsigned int nFactor,
											  uint32_t *pDstLine,
											  unsigned int nElementCount)
{
	unsigned int i, nGroupCount, nTailCount;

	nGroupCount = nElementCount / 8;
	nTailCount = nElementCount & 7;

	for (i = 0; i < nGroupCount; i++) {
		// process 8 element every time
		__m128i xmm_element, xmm_sum1, xmm_sum2, xmm_mul1, xmm_mul2;

		xmm_element = _mm_load_si128((__m128i *)pSrcLine1);
		__GBLUR_MULTIPLY_FACTOR(xmm_element, xmm_factor, xmm_sum1, xmm_sum2);

		xmm_element = _mm_load_si128((__m128i *)pSrcLine2);
		__GBLUR_MULTIPLY_FACTOR(xmm_element, xmm_factor, xmm_mul1, xmm_mul2);

		xmm_sum1 = _mm_add_epi32(xmm_sum1, xmm_mul1);
		xmm_sum2 = _mm_add_epi32(xmm_sum2, xmm_mul2);

		_mm_store_si128((__m128i *)pDstLine, xmm_sum1);
		_mm_store_si128((__m128i *)pDstLine + 1, xmm_sum2);

		pSrcLine1 += 8;
		pSrcLine2 += 8;
		pDstLine += 8;
	}

	for (i = 0; i < nTailCount; i++)
		pDstLine[i] = ((uint32_t)pSrcLine1[i] + (uint32_t)pSrcLine2[i]) * (uint32_t)nFactor;
}

// dst = (src1 + src2) * factor
void CNvcGaussBlur::BlurLineVerFirstPass32Bit(const float *pSrcLine1,
                                              const float *pSrcLine2,
                                              const __m128& xmm_factor_ps,
                                              float fFactor,
                                              float *pDstLine,
                                              unsigned int nElementCount)
{
    const unsigned int nGroupCount = nElementCount / 4;
    const unsigned int nTailCount = nElementCount & 3;

    for (unsigned int i = 0; i < nGroupCount; i++) {
        // process 4 elements every time
        __m128 xmm_element_ps = _mm_load_ps(pSrcLine1);
        xmm_element_ps = _mm_add_ps(xmm_element_ps, _mm_load_ps(pSrcLine2));
        xmm_element_ps = _mm_mul_ps(xmm_element_ps, xmm_factor_ps);
        _mm_store_ps(pDstLine, xmm_element_ps);

        pSrcLine1 += 4;
        pSrcLine2 += 4;
        pDstLine += 4;
    }

    for (unsigned int i = 0; i < nTailCount; i++)
        pDstLine[i] = (pSrcLine1[i] + pSrcLine2[i]) * fFactor;
}

// dst = dst + (src1 + src2) * factor
void CNvcGaussBlur::BlurLineVerMedianPass8Bit(const uint8_t *pSrcLine1,
											  const uint8_t *pSrcLine2,
											  __m128i& xmm_factor,
											  unsigned int nFactor,
											  uint32_t *pDstLine,
											  unsigned int nElementCount)
{
	const __m128i xmm_zero = _mm_setzero_si128();
	unsigned int i, nGroupCount, nTailCount;

	nGroupCount = nElementCount / 16;
	nTailCount = nElementCount & 15;

	for (i = 0; i < nGroupCount; i++) {
		// process 16 element every time
		__m128i xmm_element1, xmm_element2;
		__m128i xmm1, xmm2;

		xmm_element1 = _mm_load_si128((__m128i *)pSrcLine1);
		xmm_element2 = _mm_unpackhi_epi8(xmm_element1, xmm_zero);
		xmm_element1 = _mm_unpacklo_epi8(xmm_element1, xmm_zero);

		xmm1 = _mm_load_si128((__m128i *)pSrcLine2);
		xmm2 = _mm_unpackhi_epi8(xmm1, xmm_zero);
		xmm1 = _mm_unpacklo_epi8(xmm1, xmm_zero);

		xmm_element1 = _mm_add_epi16(xmm_element1, xmm1);
		xmm_element2 = _mm_add_epi16(xmm_element2, xmm2);

		__m128i xmm_mul1, xmm_mul2;

		__GBLUR_MULTIPLY_FACTOR(xmm_element1, xmm_factor, xmm_mul1, xmm_mul2);

		xmm_mul1 = _mm_add_epi32(xmm_mul1, *(__m128i *)pDstLine);
		_mm_store_si128((__m128i *)pDstLine, xmm_mul1);
		xmm_mul2 = _mm_add_epi32(xmm_mul2, *((__m128i *)pDstLine + 1));
		_mm_store_si128((__m128i *)pDstLine + 1, xmm_mul2);

		__GBLUR_MULTIPLY_FACTOR(xmm_element2, xmm_factor, xmm_mul1, xmm_mul2);

		xmm_mul1 = _mm_add_epi32(xmm_mul1, *((__m128i *)pDstLine + 2));
		_mm_store_si128((__m128i *)pDstLine + 2, xmm_mul1);
		xmm_mul2 = _mm_add_epi32(xmm_mul2, *((__m128i *)pDstLine + 3));
		_mm_store_si128((__m128i *)pDstLine + 3, xmm_mul2);

		pSrcLine1 += 16;
		pSrcLine2 += 16;
		pDstLine += 16;
	}

	for (i = 0; i < nTailCount; i++)
		pDstLine[i] += ((uint32_t)pSrcLine1[i] + (uint32_t)pSrcLine2[i]) * (uint32_t)nFactor;
}

// dst = dst + (src1 + src2) * factor
void CNvcGaussBlur::BlurLineVerMedianPass16Bit(const uint16_t *pSrcLine1,
											   const uint16_t *pSrcLine2,
											   __m128i& xmm_factor,
											   unsigned int nFactor,
											   uint32_t *pDstLine,
											   unsigned int nElementCount)
{
	unsigned int i, nGroupCount, nTailCount;

	nGroupCount = nElementCount / 8;
	nTailCount = nElementCount & 7;

	for (i = 0; i < nGroupCount; i++) {
		// process 8 element every time
		__m128i xmm_element, xmm_sum1, xmm_sum2, xmm_mul1, xmm_mul2;

		xmm_element = _mm_load_si128((__m128i *)pSrcLine1);
		__GBLUR_MULTIPLY_FACTOR(xmm_element, xmm_factor, xmm_sum1, xmm_sum2);

		xmm_element = _mm_load_si128((__m128i *)pSrcLine2);
		__GBLUR_MULTIPLY_FACTOR(xmm_element, xmm_factor, xmm_mul1, xmm_mul2);

		xmm_sum1 = _mm_add_epi32(xmm_sum1, xmm_mul1);
		xmm_sum1 = _mm_add_epi32(xmm_sum1, *(__m128i *)pDstLine);
		_mm_store_si128((__m128i *)pDstLine, xmm_sum1);
		xmm_sum2 = _mm_add_epi32(xmm_sum2, xmm_mul2);
		xmm_sum2 = _mm_add_epi32(xmm_sum2, *((__m128i *)pDstLine + 1));
		_mm_store_si128((__m128i *)pDstLine + 1, xmm_sum2);

		pSrcLine1 += 8;
		pSrcLine2 += 8;
		pDstLine += 8;
	}

	for (i = 0; i < nTailCount; i++)
		pDstLine[i] += ((uint32_t)pSrcLine1[i] + (uint32_t)pSrcLine2[i]) * (uint32_t)nFactor;
}

// dst = dst + (src1 + src2) * factor
void CNvcGaussBlur::BlurLineVerMedianPass32Bit(const float *pSrcLine1,
                                               const float *pSrcLine2,
                                               const __m128& xmm_factor_ps,
                                               float fFactor,
                                               float *pDstLine,
                                               unsigned int nElementCount)
{
    const unsigned int nGroupCount = nElementCount / 4;
    const unsigned int nTailCount = nElementCount & 3;

    for (unsigned int i = 0; i < nGroupCount; i++) {
        // process 4 elements every time
        __m128 xmm_element_ps = _mm_load_ps(pSrcLine1);
        xmm_element_ps = _mm_add_ps(xmm_element_ps, _mm_load_ps(pSrcLine2));
        xmm_element_ps = _mm_mul_ps(xmm_element_ps, xmm_factor_ps);
        xmm_element_ps = _mm_add_ps(xmm_element_ps, *(__m128 *)pDstLine);
        _mm_store_ps(pDstLine, xmm_element_ps);

        pSrcLine1 += 4;
        pSrcLine2 += 4;
        pDstLine += 4;
    }

    for (unsigned int i = 0; i < nTailCount; i++)
        pDstLine[i] += (pSrcLine1[i] + pSrcLine2[i]) * fFactor;
}

// dst = src1 * factor + src2
void CNvcGaussBlur::BlurLineVerFinalPass8Bit(const uint8_t *pSrcLine1,
                                             const uint32_t *pSrcLine2,
                                             const __m128i& xmm_factor,
                                             unsigned int nFactor,
                                             uint8_t *pDstLine,
                                             unsigned int nElementCount)
{
    const __m128i xmm_zero = _mm_setzero_si128();

    unsigned int i, nGroupCount, nTailCount;

    nGroupCount = nElementCount / 16;
    nTailCount = nElementCount & 15;

    for (i = 0; i < nGroupCount; i++) {
        // process 16 element every time
        __m128i xmm_element1, xmm_element2;
        xmm_element1 = _mm_load_si128((__m128i *)pSrcLine1);
        xmm_element2 = _mm_unpackhi_epi8(xmm_element1, xmm_zero);
        xmm_element1 = _mm_unpacklo_epi8(xmm_element1, xmm_zero);

        __m128i xmm_mul1, xmm_mul2, xmm_result;

        __GBLUR_MULTIPLY_FACTOR(xmm_element1, xmm_factor, xmm_mul1, xmm_mul2);

        xmm_mul1 = _mm_add_epi32(xmm_mul1, *(__m128i *)pSrcLine2);
        xmm_mul1 = _mm_add_epi32(xmm_mul1, g_xmm_rounding);
        xmm_mul1 = _mm_srli_epi32(xmm_mul1, __KERNEL_SCALE_BITS);
        xmm_mul2 = _mm_add_epi32(xmm_mul2, *((__m128i *)pSrcLine2 + 1));
        xmm_mul2 = _mm_add_epi32(xmm_mul2, g_xmm_rounding);
        xmm_mul2 = _mm_srli_epi32(xmm_mul2, __KERNEL_SCALE_BITS);

        xmm_result = _mm_packs_epi32(xmm_mul1, xmm_mul2);

        __GBLUR_MULTIPLY_FACTOR(xmm_element2, xmm_factor, xmm_mul1, xmm_mul2);

        xmm_mul1 = _mm_add_epi32(xmm_mul1, *((__m128i *)pSrcLine2 + 2));
        xmm_mul1 = _mm_add_epi32(xmm_mul1, g_xmm_rounding);
        xmm_mul1 = _mm_srli_epi32(xmm_mul1, __KERNEL_SCALE_BITS);
        xmm_mul2 = _mm_add_epi32(xmm_mul2, *((__m128i *)pSrcLine2 + 3));
        xmm_mul2 = _mm_add_epi32(xmm_mul2, g_xmm_rounding);
        xmm_mul2 = _mm_srli_epi32(xmm_mul2, __KERNEL_SCALE_BITS);

        xmm_mul1 = _mm_packs_epi32(xmm_mul1, xmm_mul2);
        xmm_result = _mm_packus_epi16(xmm_result, xmm_mul1);
        _mm_store_si128((__m128i *)pDstLine, xmm_result);

        pSrcLine1 += 16;
        pSrcLine2 += 16;
        pDstLine += 16;
    }

    for (i = 0; i < nTailCount; i++) {
        uint32_t sum = ((uint32_t)pSrcLine1[i] * (uint32_t)nFactor) + pSrcLine2[i];
        pDstLine[i] = (uint8_t)((sum + (1 << (__KERNEL_SCALE_BITS - 1))) >> __KERNEL_SCALE_BITS);
    }
}

// dst = src1 * factor + src2
void CNvcGaussBlur::BlurLineVerFinalPass16Bit(const uint16_t *pSrcLine1,
                                              const uint32_t *pSrcLine2,
                                              const __m128i& xmm_factor,
                                              unsigned int nFactor,
                                              uint16_t *pDstLine,
                                              unsigned int nElementCount)
{
    unsigned int i, nGroupCount, nTailCount;

    nGroupCount = nElementCount / 16;
    nTailCount = nElementCount & 15;

    for (i = 0; i < nGroupCount; i++) {
        // process 16 element every time
        __m128i xmm_element1, xmm_element2;
        xmm_element1 = _mm_load_si128((__m128i *)pSrcLine1);
        xmm_element2 = _mm_load_si128((__m128i *)pSrcLine1 + 1);

        __m128i xmm_mul1, xmm_mul2, xmm_result;

        __GBLUR_MULTIPLY_FACTOR(xmm_element1, xmm_factor, xmm_mul1, xmm_mul2);

        xmm_mul1 = _mm_add_epi32(xmm_mul1, *(__m128i *)pSrcLine2);
        xmm_mul1 = _mm_add_epi32(xmm_mul1, g_xmm_rounding);
        xmm_mul1 = _mm_srli_epi32(xmm_mul1, __KERNEL_SCALE_BITS);
        xmm_mul2 = _mm_add_epi32(xmm_mul2, *((__m128i *)pSrcLine2 + 1));
        xmm_mul2 = _mm_add_epi32(xmm_mul2, g_xmm_rounding);
        xmm_mul2 = _mm_srli_epi32(xmm_mul2, __KERNEL_SCALE_BITS);

        xmm_mul1 = _mm_sub_epi32(xmm_mul1, g_xmm_32bit_32768);
        xmm_mul2 = _mm_sub_epi32(xmm_mul2, g_xmm_32bit_32768);
        xmm_result = _mm_packs_epi32(xmm_mul1, xmm_mul2);
        xmm_result = _mm_add_epi16(xmm_result, g_xmm_16bit_32768);
        _mm_store_si128((__m128i *)pDstLine, xmm_result);

        __GBLUR_MULTIPLY_FACTOR(xmm_element2, xmm_factor, xmm_mul1, xmm_mul2);

        xmm_mul1 = _mm_add_epi32(xmm_mul1, *((__m128i *)pSrcLine2 + 2));
        xmm_mul1 = _mm_add_epi32(xmm_mul1, g_xmm_rounding);
        xmm_mul1 = _mm_srli_epi32(xmm_mul1, __KERNEL_SCALE_BITS);
        xmm_mul2 = _mm_add_epi32(xmm_mul2, *((__m128i *)pSrcLine2 + 3));
        xmm_mul2 = _mm_add_epi32(xmm_mul2, g_xmm_rounding);
        xmm_mul2 = _mm_srli_epi32(xmm_mul2, __KERNEL_SCALE_BITS);

        xmm_mul1 = _mm_sub_epi32(xmm_mul1, g_xmm_32bit_32768);
        xmm_mul2 = _mm_sub_epi32(xmm_mul2, g_xmm_32bit_32768);
        xmm_mul1 = _mm_packs_epi32(xmm_mul1, xmm_mul2);
        xmm_mul1 = _mm_add_epi16(xmm_mul1, g_xmm_16bit_32768);
        _mm_store_si128((__m128i *)pDstLine + 1, xmm_mul1);

        pSrcLine1 += 16;
        pSrcLine2 += 16;
        pDstLine += 16;
    }

    for (i = 0; i < nTailCount; i++) {
        uint32_t sum = ((uint32_t)pSrcLine1[i] * (uint32_t)nFactor) + pSrcLine2[i];
        pDstLine[i] = (uint16_t)((sum + (1 << (__KERNEL_SCALE_BITS - 1))) >> __KERNEL_SCALE_BITS);
    }
}

// dst = src1 * factor + src2
void CNvcGaussBlur::BlurLineVerFinalPass32Bit(const float *pSrcLine1,
                                              const float *pSrcLine2,
                                              const __m128& xmm_factor_ps,
                                              float fFactor,
                                              float *pDstLine,
                                              unsigned int nElementCount)
{
    const unsigned int nGroupCount = nElementCount / 4;
    const unsigned int nTailCount = nElementCount & 3;

    for (unsigned int i = 0; i < nGroupCount; i++) {
        // process 4 elements every time
        __m128 xmm_element_ps = _mm_load_ps(pSrcLine1);
        xmm_element_ps = _mm_mul_ps(xmm_element_ps, xmm_factor_ps);
        xmm_element_ps = _mm_add_ps(xmm_element_ps, _mm_load_ps(pSrcLine2));
        _mm_store_ps(pDstLine, xmm_element_ps);

        pSrcLine1 += 4;
        pSrcLine2 += 4;
        pDstLine += 4;
    }

    for (unsigned int i = 0; i < nTailCount; i++)
        pDstLine[i] = pSrcLine1[i] * fFactor + pSrcLine2[i];
}


template<bool bIsFirstPass>
void CNvcGaussBlur::CircularConvolutionMatte(float *pSignalReal,
											 float *pSignalImg,
											 const float *pFFTFactorReal,
											 const float *pFFTFactorImg,
											 const float *pKernelFFTCoefReal,
											 const float *pKernelFFTCoefImg,
											 float *pDestReal,
											 unsigned int nOrder,
											 unsigned int nOverlapCount)
{
	unsigned int nConvSize = 1 << nOrder;

	// perform forward fast fourier transform
	__NvcRealFFT(pSignalReal, pSignalImg, pFFTFactorReal, pFFTFactorImg, nOrder);

	//
	// multiply FFT coefficients of the signal by the kernel FFT coefficients
	//
	float *pSigReal, *pSigImg;
	const float *pKerReal, *pKerImg;
	unsigned int i, nGroupCount;

	assert(!(nConvSize & 3));
	nGroupCount = nConvSize / 4;

	pSigReal = pSignalReal;
	pSigImg = pSignalImg;
	pKerReal = pKernelFFTCoefReal;
	pKerImg = pKernelFFTCoefImg;

	for (i = 0; i < nGroupCount; i++) {
		__m128 xmm_sig_real_ps, xmm_sig_img_ps;
		__m128 xmm_ker_real_ps, xmm_ker_img_ps;
		__m128 xmm_tmp1_ps, xmm_tmp2_ps;

		xmm_sig_real_ps = _mm_load_ps(pSigReal);
		xmm_sig_img_ps = _mm_load_ps(pSigImg);
		xmm_ker_real_ps = _mm_load_ps(pKerReal);
		xmm_ker_img_ps = _mm_load_ps(pKerImg);

		xmm_tmp1_ps = _mm_mul_ps(xmm_sig_real_ps, xmm_ker_real_ps);
		xmm_tmp2_ps = _mm_mul_ps(xmm_sig_img_ps, xmm_ker_img_ps);
		_mm_store_ps(pSigReal, _mm_sub_ps(xmm_tmp1_ps, xmm_tmp2_ps));

		xmm_tmp1_ps = _mm_mul_ps(xmm_sig_real_ps, xmm_ker_img_ps);
		xmm_tmp2_ps = _mm_mul_ps(xmm_sig_img_ps, xmm_ker_real_ps);
		_mm_store_ps(pSigImg, _mm_add_ps(xmm_tmp1_ps, xmm_tmp2_ps));

		pSigReal += 4;
		pSigImg += 4;
		pKerReal += 4;
		pKerImg += 4;
	}

	// perform inverse fast fourier transform
	__NvcRealIFFT(pSignalReal, pSignalImg, pFFTFactorReal, pFFTFactorImg, nOrder);

	//
	// store the convolution result
	//
	__m128 xmm_idx_ps, xmm_idx_step_ps, xmm_threshold_ps;

	xmm_idx_ps = _mm_set_ps(3, 2, 1, 0);
	xmm_idx_step_ps = _mm_set1_ps(4);
	xmm_threshold_ps = _mm_set1_ps((float)nOverlapCount);

	pSigReal = pSignalReal;

	for (i = 0; i < nGroupCount; i++) {
		__m128 xmm_sig_ps;

		xmm_sig_ps = _mm_load_ps(pSigReal);

		if (!bIsFirstPass) {
			__m128 xmm_org_sig_ps, xmm_mask_ps;

			xmm_org_sig_ps = _mm_loadu_ps(pDestReal);
			xmm_mask_ps = _mm_cmplt_ps(xmm_idx_ps, xmm_threshold_ps);
			xmm_mask_ps = _mm_and_ps(xmm_mask_ps, xmm_org_sig_ps);
			xmm_sig_ps = _mm_add_ps(xmm_sig_ps, xmm_mask_ps);
			xmm_idx_ps = _mm_add_ps(xmm_idx_ps, xmm_idx_step_ps);
		}

		_mm_storeu_ps(pDestReal, xmm_sig_ps);

		pSigReal += 4;
		pDestReal += 4;
	}
}

template<bool bIsFirstPass>
void CNvcGaussBlur::CircularConvolutionBGRA(float *pSignalReal,
											float *pSignalImg,
											const float *pFFTFactorReal,
											const float *pFFTFactorImg,
											const float *pKernelFFTCoefReal,
											const float *pKernelFFTCoefImg,
											float *pDestReal,
											unsigned int nOrder,
											unsigned int nOverlapCount)
{
	unsigned int nConvSize = 1 << nOrder;

	// perform forward fast fourier transform
	__NvcRealFFT_4C(pSignalReal, pSignalImg, pFFTFactorReal, pFFTFactorImg, nOrder);

	//
	// multiply FFT coefficients of the signal by the kernel FFT coefficients
	//
	float *pSigReal, *pSigImg;
	const float *pKerReal, *pKerImg;
	unsigned int i;

	pSigReal = pSignalReal;
	pSigImg = pSignalImg;
	pKerReal = pKernelFFTCoefReal;
	pKerImg = pKernelFFTCoefImg;

	for (i = 0; i < nConvSize; i++) {
		__m128 xmm_sig_real_ps, xmm_sig_img_ps;
		__m128 xmm_ker_real_ps, xmm_ker_img_ps;
		__m128 xmm_tmp1_ps, xmm_tmp2_ps;

		xmm_sig_real_ps = _mm_load_ps(pSigReal);
		xmm_sig_img_ps = _mm_load_ps(pSigImg);
		xmm_ker_real_ps = _mm_set1_ps(*pKerReal);
		xmm_ker_img_ps = _mm_set1_ps(*pKerImg);

		xmm_tmp1_ps = _mm_mul_ps(xmm_sig_real_ps, xmm_ker_real_ps);
		xmm_tmp2_ps = _mm_mul_ps(xmm_sig_img_ps, xmm_ker_img_ps);
		_mm_store_ps(pSigReal, _mm_sub_ps(xmm_tmp1_ps, xmm_tmp2_ps));

		xmm_tmp1_ps = _mm_mul_ps(xmm_sig_real_ps, xmm_ker_img_ps);
		xmm_tmp2_ps = _mm_mul_ps(xmm_sig_img_ps, xmm_ker_real_ps);
		_mm_store_ps(pSigImg, _mm_add_ps(xmm_tmp1_ps, xmm_tmp2_ps));

		pSigReal += 4;
		pSigImg += 4;
		pKerReal += 1;
		pKerImg += 1;
	}

	// perform inverse fast fourier transform
	__NvcRealIFFT_4C(pSignalReal, pSignalImg, pFFTFactorReal, pFFTFactorImg, nOrder);

	//
	// store the convolution result
	//
	pSigReal = pSignalReal;

	if (!bIsFirstPass) {
		for (i = 0; i < nOverlapCount; i++) {
			__m128 xmm_sig_ps;

			xmm_sig_ps = _mm_load_ps(pSigReal);
			xmm_sig_ps = _mm_add_ps(xmm_sig_ps, *(__m128 *)pDestReal);
			_mm_store_ps(pDestReal, xmm_sig_ps);

			pSigReal += 4;
			pDestReal += 4;
		}

		memcpy(pDestReal, pSigReal, (nConvSize - nOverlapCount) * 4 * sizeof(float));
	} else {
		memcpy(pDestReal, pSigReal, nConvSize * 4 * sizeof(float));
	}
}

#ifdef _MSC_VER
#pragma warning(default:4127)
#endif


// create a gaussian blur object, return the handle of the object
NVC_HGAUSSBLUR NvcGaussBlurCreate()
{
	CNvcGaussBlur *pGaussBlur;
	bool bSucceeded = true;

	pGaussBlur = new(std::nothrow) CNvcGaussBlur(bSucceeded);
	if (!pGaussBlur)
		return NULL;

	if (!bSucceeded) {
		delete pGaussBlur;
		return NULL;
	}

	return (NVC_HGAUSSBLUR)pGaussBlur;
}

// destroy a gaussian blur object
void NvcGaussBlurDestroy(NVC_HGAUSSBLUR hGaussBlur)
{
	if (hGaussBlur == NULL) {
		assert(false);
		return;
	}

	CNvcGaussBlur *pGaussBlur = (CNvcGaussBlur *)hGaussBlur;
	delete pGaussBlur;
}

// NOTE: pAuxBuffer should be a buffer whose dimension is at least the same as source buffer
// and its pitch must be at least 16 bytes aligned (cache size aligned is recommended)
int NvcGaussBlurPrepare(NVC_HGAUSSBLUR hGaussBlur,
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
						const SNvcMaskInfo *pMaskInfo)
{
	if (!hGaussBlur) {
		assert(false);
		return -1;
	}

	CNvcGaussBlur *pGaussBlur = (CNvcGaussBlur *)hGaussBlur;
	bool bRet;

	bRet = pGaussBlur->Prepare(
		pSrc, nSrcPitch,
		pDst, nDstPitch,
		pAuxBuffer, nAuxPitch,
		nWidth, nHeight,
		eBitDepth,
		bIsDataMatte,
		fBlurWidth,
		fBlurHeight,
		nThreadCount,
		pMaskInfo);

	return bRet ? 0 : -1;
}

int NvcGaussBlurProcess(NVC_HGAUSSBLUR hGaussBlur, unsigned int nThreadIndex)
{
	if (hGaussBlur == NULL) {
		assert(false);
		return -1;
	}

	CNvcGaussBlur *pGaussBlur = (CNvcGaussBlur *)hGaussBlur;
	bool bRet;

	bRet = pGaussBlur->Process(nThreadIndex);
	return bRet ? 0 : -1;
}

