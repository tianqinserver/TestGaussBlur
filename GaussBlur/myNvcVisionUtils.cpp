
#include <myNvcVisionUtils.h>
#include <NvcColorConvUtils.h>


static const __m128i g_xmm_zero = _mm_setzero_si128();
static const __m128 g_xmm_zero_ps = _mm_setzero_ps();

static const __m128i g_xmm_255_8b = _mm_set1_epi8((unsigned char)255);

#define __NVC_MASK_SCALE_255_256		((unsigned int)(256.0 / 255.0 * 32768 + 0.5))


static const __m128i g_xmm_scale_255_256 = _mm_set1_epi16((unsigned short)__NVC_MASK_SCALE_255_256);
static const __m128i g_xmm_mask_rounding_8b = _mm_set1_epi16(32);
static const __m128i g_xmm_1_shift_14 = _mm_set1_epi16(1 << 14);

const __m128i g_xmm_32bit_32768 = _mm_set1_epi32(32768);
const __m128i g_xmm_16bit_32768 = _mm_set1_epi16((unsigned short)32768);
const __m128i g_xmm_16bit_128 = _mm_set1_epi16((unsigned short)128);
const __m128i g_xmm_16bit_64 = _mm_set1_epi16(64);

const __m128i g_xmm_luma_red_coef_601 = _mm_set1_epi16(__COEF_RED_LUMA(NVC_BT601));
const __m128i g_xmm_luma_green_coef_601 = _mm_set1_epi16(__COEF_GREEN_LUMA(NVC_BT601));
const __m128i g_xmm_luma_blue_coef_601 = _mm_set1_epi16(__COEF_BLUE_LUMA(NVC_BT601));
const __m128i g_xmm_luma_red_coef_709 = _mm_set1_epi16(__COEF_RED_LUMA(NVC_BT709));
const __m128i g_xmm_luma_green_coef_709 = _mm_set1_epi16(__COEF_GREEN_LUMA(NVC_BT709));
const __m128i g_xmm_luma_blue_coef_709 = _mm_set1_epi16(__COEF_BLUE_LUMA(NVC_BT709));
const __m128i g_xmm_luma_red_coef_2020 = _mm_set1_epi16(__COEF_RED_LUMA(NVC_BT2020));
const __m128i g_xmm_luma_green_coef_2020 = _mm_set1_epi16(__COEF_GREEN_LUMA(NVC_BT2020));
const __m128i g_xmm_luma_blue_coef_2020 = _mm_set1_epi16(__COEF_BLUE_LUMA(NVC_BT2020));

const __m128 g_xmm_luma_red_coef_601_ps = _mm_set1_ps(__NVC_R_Y_601);
const __m128 g_xmm_luma_green_coef_601_ps = _mm_set1_ps(__NVC_G_Y_601);
const __m128 g_xmm_luma_blue_coef_601_ps = _mm_set1_ps(__NVC_B_Y_601);
const __m128 g_xmm_luma_red_coef_709_ps = _mm_set1_ps(__NVC_R_Y_709);
const __m128 g_xmm_luma_green_coef_709_ps = _mm_set1_ps(__NVC_G_Y_709);
const __m128 g_xmm_luma_blue_coef_709_ps = _mm_set1_ps(__NVC_B_Y_709);
const __m128 g_xmm_luma_red_coef_2020_ps = _mm_set1_ps(__NVC_R_Y_2020);
const __m128 g_xmm_luma_green_coef_2020_ps = _mm_set1_ps(__NVC_G_Y_2020);
const __m128 g_xmm_luma_blue_coef_2020_ps = _mm_set1_ps(__NVC_B_Y_2020);

//
// 16 bit to 8 bit conversion helper functions
//
#define __NVC_16BIT_TO_8BIT_RATIO           (1 / 257.0)
#define __NVC_16BIT_TO_8BIT_RATIO_SCALED    ((unsigned int)(__NVC_16BIT_TO_8BIT_RATIO * 65536))

const __m128i g_xmm_8b_to_16b_ratio = _mm_set1_epi16(257);
const __m128i g_xmm_16b_to_8b_ratio = _mm_set1_epi16((unsigned short)__NVC_16BIT_TO_8BIT_RATIO_SCALED);

static const __m128i g_xmm_alpha_mask_16bit = _mm_set_epi16(255, 0, 0, 0, 255, 0, 0, 0);

const __m128i g_xmm_16bit_rounding_div_256 = _mm_set1_epi16(128);

unsigned int __NvcCalcLineSize(unsigned int nWidth,
	ENvcColorSpace eColorSpace,
	ENvcBitDepth eBitDepth)
{
	if ((eColorSpace == keNvcColorSpace_YUY2 ||
		eColorSpace == keNvcColorSpace_UYVY ||
		eColorSpace == keNvcColorSpace_YUAYVA4224 ||
		eColorSpace == keNvcColorSpace_YUVA4444) &&
		eBitDepth != keNvcBitDepthInt8)
	{
		assert(false);
		return 0;
	}

	unsigned int nAlignedPixelCount = nWidth;
	unsigned int nChannelCountPerPixel = 1;

	switch (eColorSpace) {
	case keNvcColorSpace_BGRA:
	case keNvcColorSpace_ARGB:
	case keNvcColorSpace_RGBA:
	case keNvcColorSpace_YUVA4444:
		nChannelCountPerPixel = 4;
		break;

	case keNvcColorSpace_BGR:
	case keNvcColorSpace_RGB:
		nChannelCountPerPixel = 3;
		break;

	case keNvcColorSpace_YUY2:
	case keNvcColorSpace_UYVY:
		nAlignedPixelCount = NVC_UP_ALIGN_PO2(nWidth, 2);
		nChannelCountPerPixel = 2;
		break;

	case keNvcColorSpace_YUAYVA4224:
		nAlignedPixelCount = NVC_UP_ALIGN_PO2(nWidth, 8);
		nChannelCountPerPixel = 3;
		break;

	case keNvcColorSpace_Alpha:
		nChannelCountPerPixel = 1;
		break;

	case keNvcColorSpace_YUV422Planar: 
	case keNvcColorSpace_YUV420Planar:
		{
			int uvacturalWidth = NvcGetActualWidthForImagePlane(keNvcColorSpace_YUV422Planar, nWidth, 1);

			return (nWidth + uvacturalWidth * 2) * NvcGetBytesFromBitDepth(eBitDepth);

		}
	case keNvcColorSpace_YUVA422Planar:
	case keNvcColorSpace_YUVA420Planar:
		{
			int uvacturalWidth = NvcGetActualWidthForImagePlane(keNvcColorSpace_YUV422Planar, nWidth, 1);

			return (nWidth*2 + uvacturalWidth * 2) * NvcGetBytesFromBitDepth(eBitDepth);

		}

	default:
		assert(false);
		break;
	}

	return nAlignedPixelCount * nChannelCountPerPixel * NvcGetBytesFromBitDepth(eBitDepth);
}

////////////////////////88888888888888888888888888__ConstructMaskLine_YUVA,start////////////////////////////////////////////////////////////////////////
template<ENvcColorChannel channelType>
static inline void __Xmm_Get_8B_RGBChannel_From_8B_YUV422(
	__m128i xmm_y,
	__m128i xmm_u,
	__m128i xmm_v,
	const __Xmm_YUV_RGB_Params *params,
	__m128i& xmm_channel)
{
	xmm_y = _mm_sub_epi16(xmm_y, g_xmm_16bit_16);
	xmm_u = _mm_sub_epi16(xmm_u, g_xmm_16bit_128);
	xmm_v = _mm_sub_epi16(xmm_v, g_xmm_16bit_128);
	xmm_y = _mm_slli_epi16(xmm_y, 7);
	xmm_u = _mm_slli_epi16(xmm_u, 7);
	xmm_v = _mm_slli_epi16(xmm_v, 7);

	xmm_y = _mm_mulhi_epi16(xmm_y, params->xmm_Y);

	if (channelType == keNvcColorChannelRed) {
		// Calculate red
		xmm_channel = _mm_mulhi_epi16(xmm_v, params->xmm_V_R);
		xmm_channel = _mm_add_epi16(xmm_channel, xmm_y);
		xmm_channel = _mm_add_epi16(xmm_channel, g_xmm_16bit_8);
		xmm_channel = _mm_srai_epi16(xmm_channel, 4);
	}
	else if (channelType == keNvcColorChannelGreen) {
		// Calculate green
		xmm_channel = _mm_mulhi_epi16(xmm_u, params->xmm_U_G);
		xmm_channel = _mm_sub_epi16(xmm_y, xmm_channel);
		xmm_channel = _mm_sub_epi16(xmm_channel, _mm_mulhi_epi16(xmm_v, params->xmm_V_G));
		xmm_channel = _mm_add_epi16(xmm_channel, g_xmm_16bit_8);
		xmm_channel = _mm_srai_epi16(xmm_channel, 4);
	}
	else if (channelType == keNvcColorChannelBlue) {
		// Calculate blue
		xmm_channel = _mm_mulhi_epi16(xmm_u, params->xmm_U_B);
		xmm_channel = _mm_add_epi16(xmm_y, xmm_channel);
		xmm_channel = _mm_add_epi16(xmm_channel, g_xmm_16bit_8);
		xmm_channel = _mm_srai_epi16(xmm_channel, 4);
	}
}

static NVC_FORCE_INLINE __m128i __NvcCalc_8Bit_RGBChannel_From_8Bit_YUV422_16Pix(
	const void *pY, const void *pU, const void *pV, 
	ENvcColorChannel channelType, const __Xmm_YUV_RGB_Params *params)
{
	const bool bIsSrcAligned = ((size_t)pY & 15) ? false : true;

	__m128i xmm_Y, xmm_U, xmm_V;
	if (bIsSrcAligned) {
		xmm_Y = _mm_load_si128((__m128i *)pY);
	}
	else {
		xmm_Y = _mm_loadu_si128((__m128i *)pY);
	}

	xmm_U = _mm_loadl_epi64((__m128i *)pU);
	xmm_V = _mm_loadl_epi64((__m128i *)pV);
	xmm_U = _mm_unpacklo_epi8(xmm_U, _mm_setzero_si128());
	xmm_V = _mm_unpacklo_epi8(xmm_V, _mm_setzero_si128());

	__m128i xmm_tmp_y, xmm_tmp_u, xmm_tmp_v;
	xmm_tmp_y = _mm_unpacklo_epi8(xmm_Y, _mm_setzero_si128());
	xmm_tmp_u = _mm_unpacklo_epi16(xmm_U, xmm_U);
	xmm_tmp_v = _mm_unpacklo_epi16(xmm_V, xmm_V);

	__m128i xmm_channel1, xmm_channel2;

	if (channelType == keNvcColorChannelRed)
		__Xmm_Get_8B_RGBChannel_From_8B_YUV422<keNvcColorChannelRed>(xmm_tmp_y, xmm_tmp_u, xmm_tmp_v, params, xmm_channel1);
	else if (channelType == keNvcColorChannelGreen)
		__Xmm_Get_8B_RGBChannel_From_8B_YUV422<keNvcColorChannelGreen>(xmm_tmp_y, xmm_tmp_u, xmm_tmp_v, params, xmm_channel1);
	else if (channelType == keNvcColorChannelBlue)
		__Xmm_Get_8B_RGBChannel_From_8B_YUV422<keNvcColorChannelBlue>(xmm_tmp_y, xmm_tmp_u, xmm_tmp_v, params, xmm_channel1);


	xmm_tmp_y = _mm_unpackhi_epi8(xmm_Y, _mm_setzero_si128());
	xmm_tmp_u = _mm_unpackhi_epi16(xmm_U, xmm_U);
	xmm_tmp_v = _mm_unpackhi_epi16(xmm_V, xmm_V);

	if (channelType == keNvcColorChannelRed)
		__Xmm_Get_8B_RGBChannel_From_8B_YUV422<keNvcColorChannelRed>(xmm_tmp_y, xmm_tmp_u, xmm_tmp_v, params, xmm_channel2);
	else if (channelType == keNvcColorChannelGreen)
		__Xmm_Get_8B_RGBChannel_From_8B_YUV422<keNvcColorChannelGreen>(xmm_tmp_y, xmm_tmp_u, xmm_tmp_v, params, xmm_channel2);
	else if (channelType == keNvcColorChannelBlue)
		__Xmm_Get_8B_RGBChannel_From_8B_YUV422<keNvcColorChannelBlue>(xmm_tmp_y, xmm_tmp_u, xmm_tmp_v, params, xmm_channel2);

	return _mm_packus_epi16(xmm_channel1, xmm_channel2);
}

static void __GenerateRGBChannelLine8Bit(
	const uint8_t *pY, const uint8_t *pU, const uint8_t *pV, uint8_t *pChannel, 
	ENvcColorChannel channelType, int nColorimetry, unsigned int nPixelCount)
{
	const unsigned int nGroupCount = nPixelCount / 16;
	const unsigned int nTailCount = nPixelCount & 15;

	__Xmm_YUV_RGB_Params param;
	__Fill_Xmm_8B_YUV_8B_RGB_Params(nColorimetry, &param);
	for (unsigned int i = 0; i < nGroupCount; i++) {
		__m128i xmm_luma = __NvcCalc_8Bit_RGBChannel_From_8Bit_YUV422_16Pix(pY, pU, pV, channelType, &param);
		_mm_store_si128((__m128i *)pChannel, xmm_luma);

		pY += 16;
		pU += 8;
		pV += 8;
		pChannel += 16;
	}

	uint8_t r, g, b;
	for (unsigned int i = 0; i < nTailCount/2; i++) {
		NVC_8Bit_RGB_FROM_8Bit_YUV(r,g,b,pY[0], pU[0], pV[0], nColorimetry);
		if (channelType == keNvcColorChannelRed)
			pChannel[0] = r;
		else if (channelType == keNvcColorChannelGreen)
			pChannel[0] = g;
		else if (channelType == keNvcColorChannelBlue)
			pChannel[0] = b;

		pY++;
		pChannel++;

		NVC_8Bit_RGB_FROM_8Bit_YUV(r, g, b, pY[0], pU[0], pV[0], nColorimetry);
		if (channelType == keNvcColorChannelRed)
			pChannel[0] = r;
		else if (channelType == keNvcColorChannelGreen)
			pChannel[0] = g;
		else if (channelType == keNvcColorChannelBlue)
			pChannel[0] = b;
		pY++;
		pChannel++;
	}

	if (nTailCount & 1) {
		NVC_8Bit_RGB_FROM_8Bit_YUV(r, g, b, pY[0], pU[0], pV[0], nColorimetry);
		if (channelType == keNvcColorChannelRed)
			pChannel[0] = r;
		else if (channelType == keNvcColorChannelGreen)
			pChannel[0] = g;
		else if (channelType == keNvcColorChannelBlue)
			pChannel[0] = b;
	}
}

static void __ConstructMaskLine_YUVA(const SNvcBufferPlaneInfo *pSrcMaskLine,
	ENvcBitDepth eSrcMaskDepth,
	ENvcMaskType eSrcMaskType,
	ENvcColorChannel eMaskChannel,
	int nColorimetry,
	void *pDstMaskLine,
	ENvcBitDepth eDstMaskDepth,
	unsigned int nPixelCount)
{
	const unsigned int nSrcChannelBytes = NvcGetBytesFromBitDepth(eSrcMaskDepth);

	if (eSrcMaskType == keNvcMaskTypeYUVA422Planar && eSrcMaskDepth == eDstMaskDepth && 
		eMaskChannel == keNvcColorChannelAlpha) {
			if (pSrcMaskLine->nPlanarCount > 3)
				memcpy(pDstMaskLine, pSrcMaskLine->pDataPtr[3], nPixelCount * nSrcChannelBytes);
			else
				memset(pDstMaskLine, 255, nPixelCount * nSrcChannelBytes);
			return;
	}

	void *pAlphaMaskLine = (void *)pSrcMaskLine;
	if (eSrcMaskType != keNvcMaskTypeAlpha) {
		// In this case we must convert this yuva source mask to a alpha only mask
		if (eSrcMaskDepth != eDstMaskDepth) {
			void *pTmpBuffer = _alloca(nPixelCount * nSrcChannelBytes + 127);
			pTmpBuffer = (void *)NVC_UP_ALIGN_PO2((size_t)pTmpBuffer, 128);
			pAlphaMaskLine = pTmpBuffer;
		}
		else {
			pAlphaMaskLine = pDstMaskLine;
		}

		if (eMaskChannel == keNvcColorChannelAlpha ||
			eMaskChannel == keNvcColorChannelLuma) {
				int nChannelIndex = 0;
				switch (eMaskChannel) {
				case keNvcColorChannelAlpha:
					nChannelIndex = 3;
					break;
				}

				if (pSrcMaskLine->nPlanarCount > 3)
					memcpy(pDstMaskLine, pSrcMaskLine->pDataPtr[3], nPixelCount * nSrcChannelBytes);
				else
					memset(pDstMaskLine, 255, nPixelCount * nSrcChannelBytes);
		}
		else {
			// Extract luma channel from this YUV source mask line
			switch (eSrcMaskDepth) {
			case keNvcBitDepthInt8:
				__GenerateRGBChannelLine8Bit(
					(const uint8_t *)pSrcMaskLine->pDataPtr[0], 
					(const uint8_t *)pSrcMaskLine->pDataPtr[1],
					(const uint8_t *)pSrcMaskLine->pDataPtr[2],
					(uint8_t *)pAlphaMaskLine, 
					eMaskChannel,
					nColorimetry, 
					nPixelCount);
				break;

			case keNvcBitDepthInt16:
				//__GenerateRGBChannelLine16Bit(
				//	(const uint16_t *)pSrcMaskLine->pDataPtr[0],
				//	(const uint16_t *)pSrcMaskLine->pDataPtr[1],
				//	(const uint16_t *)pSrcMaskLine->pDataPtr[2],
				//	(uint16_t *)pAlphaMaskLine,
				//	eMaskChannel,
				//	nColorimetry,
				//	nPixelCount);
				break;
			}
		}
	}
}

////////////////////////88888888888888888888888888__ConstructMaskLine_YUVA,end/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////88888888888888888888888888,__ConstructMaskLine_BGRA,start/////////////////////////////////////////////////////////////////////////////


//
// Helper function to generate 8 bit luma line
//
static void __GenerateLumaLine8Bit(const uint8_t *pSrc, uint8_t *pLuma, int nColorimetry, unsigned int nPixelCount)
{
	const unsigned int nGroupCount = nPixelCount / 16;
	const unsigned int nTailCount = nPixelCount & 15;
	for (unsigned int i = 0; i < nGroupCount; i++) {
		__m128i xmm_luma = __NvcCalc_8Bit_Luma_From_8Bit_BGRA_8Pix(pSrc, nColorimetry);
		__m128i xmm_luma2 = __NvcCalc_8Bit_Luma_From_8Bit_BGRA_8Pix(pSrc + 32, nColorimetry);
		xmm_luma = _mm_packus_epi16(xmm_luma, xmm_luma2);
		_mm_store_si128((__m128i *)pLuma, xmm_luma);

		pSrc += 64;
		pLuma += 16;
	}

	for (unsigned int i = 0; i < nTailCount; i++)
		pLuma[i] = __Nvc_Cacl_8Bit_Luma_From_8Bit_RGB(pSrc[i * 4 + 2], pSrc[i * 4 + 1], pSrc[i * 4 + 0], nColorimetry);
}

//
// Helper function to extract a channel from BGRA surface
//
template<typename T>
void __ExtractChannelFromBGRALine(const T *pSrcLine, T *pDstLine, int nChannelIndex, unsigned int nPixelCount)
{
	for (unsigned int i = 0; i < nPixelCount; i++)
		pDstLine[i] = pSrcLine[i * 4 + nChannelIndex];
}

static inline unsigned int __NvcComponentCount(ENvcColorSpace eColorSpace)
{
	switch (eColorSpace) {
	case keNvcColorSpace_BGRA:
	case keNvcColorSpace_RGBA:
	case keNvcColorSpace_ARGB:
		return 4;

	case keNvcColorSpace_BGR:
	case keNvcColorSpace_RGB:
		return 3;

	case keNvcColorSpace_Alpha:
		return 1;

	default:
		assert(false);
		return 0;
	}
}

//
// base class for all converter
//
template<bool bIsSrcAligned, bool bIsDstAligned, bool bIsNonTemporalUsed>
class __CNvcBaseConverter
{
public:
	static NVC_FORCE_INLINE __m128i LoadSI128(const void *ptr)
	{
		if (bIsSrcAligned)
			return _mm_load_si128((const __m128i *)ptr);
		else
			return _mm_loadu_si128((const __m128i *)ptr);
	}

	static NVC_FORCE_INLINE __m128 LoadPS(const void *ptr)
	{
		if (bIsSrcAligned)
			return _mm_load_ps((const float *)ptr);
		else
			return _mm_loadu_ps((const float *)ptr);
	}

	static NVC_FORCE_INLINE void StoreSI128(void *ptr, const __m128i& xmm_data)
	{
		if (bIsNonTemporalUsed)
			_mm_stream_si128((__m128i *)ptr, xmm_data);
		else if (bIsDstAligned)
			_mm_store_si128((__m128i *)ptr, xmm_data);
		else
			_mm_storeu_si128((__m128i *)ptr, xmm_data);
	}

	static NVC_FORCE_INLINE void StorePS(void *ptr, const __m128& xmm_data)
	{
		if (bIsNonTemporalUsed)
			_mm_stream_ps((float *)ptr, xmm_data);
		else if (bIsDstAligned)
			_mm_store_ps((float *)ptr, xmm_data);
		else
			_mm_storeu_ps((float *)ptr, xmm_data);
	}

	static NVC_FORCE_INLINE void SFence()
	{
		if (bIsNonTemporalUsed)
			_mm_sfence();
	}
};



static NVC_FORCE_INLINE uint16_t __Conv_8Bit_To_16Bit(uint8_t element)
{
    return (uint16_t)element * 257;
}

static NVC_FORCE_INLINE float __Conv_8Bit_To_32Bit(uint8_t element)
{
    return element / 255.0f;
}
static NVC_FORCE_INLINE uint8_t __Conv_16Bit_To_8Bit(uint16_t element)
{
    return ((unsigned int)element * __NVC_16BIT_TO_8BIT_RATIO_SCALED + 32768) >> 16;
}
static NVC_FORCE_INLINE float __Conv_16Bit_To_32Bit(uint16_t element)
{
    return element / 65535.0f;
}
static NVC_FORCE_INLINE uint8_t __Conv_32Bit_To_8Bit(float element)
{
    return (uint8_t)(NvcLimit32BitColorChannel(element) * 255 + 0.5f);
}
static NVC_FORCE_INLINE uint16_t __Conv_32Bit_To_16Bit(float element)
{
    return (uint16_t)(NvcLimit32BitColorChannel(element) * 65535 + 0.5f);
}

//
// 8 bit to 16 bit conversion helper functions
//


static NVC_FORCE_INLINE void __XmmConv_8Bit_To_16Bit_16Elements(const __m128i& xmm_8b_element, __m128i xmm_16b_element[2])
{
	xmm_16b_element[0] = _mm_mullo_epi16(_mm_unpacklo_epi8(xmm_8b_element, g_xmm_zero), g_xmm_8b_to_16b_ratio);
	xmm_16b_element[1] = _mm_mullo_epi16(_mm_unpackhi_epi8(xmm_8b_element, g_xmm_zero), g_xmm_8b_to_16b_ratio);
}

// class to convert 8 bit pixel element to 16 bit
template<bool bIsSrcAligned, bool bIsDstAligned, bool bIsNonTemporalUsed>
class __CNvcBitDepthConv_8B_16B : public __CNvcBaseConverter<bIsSrcAligned, bIsDstAligned, bIsNonTemporalUsed>
{
public:
	static NVC_FORCE_INLINE unsigned int SrcElementSizeInBytes() { return 1; }
	static NVC_FORCE_INLINE unsigned int DstElementSizeInBytes() { return 2; }
	static NVC_FORCE_INLINE unsigned int ElementCountInGroup() { return 16; }

	NVC_FORCE_INLINE void ConvertGroup(const void *pSrcPtr, void *pDstPtr)
	{
        __m128i xmm_src_element = this->LoadSI128(pSrcPtr);
		__m128i xmm_dst_element[2];
		__XmmConv_8Bit_To_16Bit_16Elements(xmm_src_element, xmm_dst_element);
        this->StoreSI128(pDstPtr, xmm_dst_element[0]);
        this->StoreSI128((char *)pDstPtr + 16, xmm_dst_element[1]);
	}

	NVC_FORCE_INLINE void ConvertElement(const void *pSrcPtr, void *pDstPtr)
	{
		*(uint16_t *)pDstPtr = __Conv_8Bit_To_16Bit(*(uint8_t *)pSrcPtr);
	}
};

template<class Converter>
void __NvcBitDepthConversion(const void *pSrc, int nSrcPitch, void *pDst, int nDstPitch, unsigned int nWidth, unsigned int nHeight)
{
	Converter conv;
	const unsigned int nGroupCount = nWidth / conv.ElementCountInGroup();
	const unsigned int nTailCount = nWidth % conv.ElementCountInGroup();
	const unsigned int nSrcGroupStride = conv.ElementCountInGroup() * conv.SrcElementSizeInBytes();
	const unsigned int nDstGroupStride = conv.ElementCountInGroup() * conv.DstElementSizeInBytes();

	const void *pSrcLine = pSrc;
	void *pDstLine = pDst;

	for (unsigned int row = 0; row < nHeight; row++) {
		const void *pSrcPtr = pSrcLine;
		void *pDstPtr = pDstLine;

		for (unsigned int i = 0; i < nGroupCount; i++) {
			conv.ConvertGroup(pSrcPtr, pDstPtr);
			pSrcPtr = (char *)pSrcPtr + nSrcGroupStride;
			pDstPtr = (char *)pDstPtr + nDstGroupStride;
		}

		for (unsigned int i = 0; i < nTailCount; i++) {
			conv.ConvertElement(pSrcPtr, pDstPtr);
			pSrcPtr = (char *)pSrcPtr + conv.SrcElementSizeInBytes();
			pDstPtr = (char *)pDstPtr + conv.DstElementSizeInBytes();
		}

		pSrcLine = (char *)pSrcLine + nSrcPitch;
		pDstLine = (char *)pDstLine + nDstPitch;
	}

	conv.SFence();
}




static NVC_FORCE_INLINE __m128i __XmmConv_16Bit_To_8Bit_16Elements(const __m128i xmm_element[2])
{
	__m128i xmm_high = _mm_mulhi_epu16(xmm_element[0], g_xmm_16b_to_8b_ratio);
	__m128i xmm_low = _mm_mullo_epi16(xmm_element[0], g_xmm_16b_to_8b_ratio);
	xmm_low = _mm_srli_epi16(xmm_low, 15); // For rounding
	const __m128i xmm_tmp_result1 = _mm_add_epi16(xmm_high, xmm_low);

	xmm_high = _mm_mulhi_epu16(xmm_element[1], g_xmm_16b_to_8b_ratio);
	xmm_low = _mm_mullo_epi16(xmm_element[1], g_xmm_16b_to_8b_ratio);
	xmm_low = _mm_srli_epi16(xmm_low, 15); // For rounding
	const __m128i xmm_tmp_result2 = _mm_add_epi16(xmm_high, xmm_low);

	return _mm_packus_epi16(xmm_tmp_result1, xmm_tmp_result2);
}

// class to convert 16 bit pixel element to 8 bit
template<bool bIsSrcAligned, bool bIsDstAligned, bool bIsNonTemporalUsed>
class __CNvcBitDepthConv_16B_8B : public __CNvcBaseConverter<bIsSrcAligned, bIsDstAligned, bIsNonTemporalUsed>
{
public:
	static NVC_FORCE_INLINE unsigned int SrcElementSizeInBytes() { return 2; }
	static NVC_FORCE_INLINE unsigned int DstElementSizeInBytes() { return 1; }
	static NVC_FORCE_INLINE unsigned int ElementCountInGroup() { return 16; }

	NVC_FORCE_INLINE void ConvertGroup(const void *pSrcPtr, void *pDstPtr)
	{
		__m128i xmm_src_element[2];
        xmm_src_element[0] = this->LoadSI128(pSrcPtr);
        xmm_src_element[1] = this->LoadSI128((char *)pSrcPtr + 16);
		__m128i xmm_dst_element = __XmmConv_16Bit_To_8Bit_16Elements(xmm_src_element);
        this->StoreSI128(pDstPtr, xmm_dst_element);
	}

	NVC_FORCE_INLINE void ConvertElement(const void *pSrcPtr, void *pDstPtr)
	{
		*(uint8_t *)pDstPtr = __Conv_16Bit_To_8Bit(*(uint16_t *)pSrcPtr);
	}
};

template<bool bIsSrcAligned, bool bIsDstAligned, bool bIsNonTemporalUsed>
void __NvcDoBitDepthConversion_ARGB(const void *pSrc,
	int nSrcPitch,
	ENvcBitDepth eSrcBitDepth,
	void *pDst,
	int nDstPitch,
	ENvcBitDepth eDstBitDepth,
	ENvcColorSpace eColorSpace,
	unsigned int nWidth,
	unsigned int nHeight)
{
	unsigned int nElementCount = __NvcComponentCount(eColorSpace) * nWidth;

	switch (eSrcBitDepth) {
	case keNvcBitDepthInt8:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt16:
			__NvcBitDepthConversion<__CNvcBitDepthConv_8B_16B<bIsSrcAligned, bIsDstAligned, bIsNonTemporalUsed> >(pSrc, nSrcPitch, pDst, nDstPitch, nElementCount, nHeight);
			break;

		case keNvcBitDepthFloat32:
			//__NvcBitDepthConversion<__CNvcBitDepthConv_8B_32B<bIsSrcAligned, bIsDstAligned, bIsNonTemporalUsed> >(pSrc, nSrcPitch, pDst, nDstPitch, nElementCount, nHeight);
			break;
		}
		break;

	case keNvcBitDepthInt16:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			__NvcBitDepthConversion<__CNvcBitDepthConv_16B_8B<bIsSrcAligned, bIsDstAligned, bIsNonTemporalUsed> >(pSrc, nSrcPitch, pDst, nDstPitch, nElementCount, nHeight);
			break;

		case keNvcBitDepthFloat32:
			//__NvcBitDepthConversion<__CNvcBitDepthConv_16B_32B<bIsSrcAligned, bIsDstAligned, bIsNonTemporalUsed> >(pSrc, nSrcPitch, pDst, nDstPitch, nElementCount, nHeight);
			break;
		}
		break;

	case keNvcBitDepthFloat32:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			//__NvcBitDepthConversion<__CNvcBitDepthConv_32B_8B<bIsSrcAligned, bIsDstAligned, bIsNonTemporalUsed> >(pSrc, nSrcPitch, pDst, nDstPitch, nElementCount, nHeight);
			break;

		case keNvcBitDepthInt16:
			//__NvcBitDepthConversion<__CNvcBitDepthConv_32B_16B<bIsSrcAligned, bIsDstAligned, bIsNonTemporalUsed> >(pSrc, nSrcPitch, pDst, nDstPitch, nElementCount, nHeight);
			break;
		}
		break;
	}
}

// NOTE: in-place process is possible
void NvcPremultiply_32Bit_BGRA(const void *pSrc,
	int nSrcPitch,
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	int bIsNonTemporal)
{
	UNREFERENCED_PARAMETER(bIsNonTemporal);

	const float *pSrcLine = (float *)pSrc;
	float *pDstLine = (float *)pDst;

	for (unsigned int i = 0; i < nHeight; i++) {
		const float *pSrcPtr = pSrcLine;
		float *pDstPtr = pDstLine;

		for (unsigned int j = 0; j < nWidth; j++) {
			const float alpha = pSrcPtr[3];
			pDstPtr[0] = pSrcPtr[0] * alpha;
			pDstPtr[1] = pSrcPtr[1] * alpha;
			pDstPtr[2] = pSrcPtr[2] * alpha;
			pDstPtr[3] = alpha;
			pSrcPtr += 4;
			pDstPtr += 4;
		}

		pSrcLine = (const float *)((char *)pSrcLine + nSrcPitch);
		pDstLine = (float *)((char *)pDstLine + nDstPitch);
	}
}

// General bit depth conversion
void __NvcBitDepthConversion_ARGB(const void *pSrc,
	int nSrcPitch,
	ENvcBitDepth eSrcBitDepth,
	void *pDst,
	int nDstPitch,
	ENvcBitDepth eDstBitDepth,
	ENvcColorSpace eColorSpace,
	unsigned int nWidth,
	unsigned int nHeight,
	int bComposite,
	int bIsNonTemporal)
{
	const bool bIsSrcAligned = ((size_t)pSrc & 15 || nSrcPitch & 15) ? false : true;
	const bool bIsDstAligned = ((size_t)pDst & 15 || nDstPitch & 15) ? false : true;
	bool bIsNonTemporalUsed = bIsNonTemporal && bIsDstAligned;
	if (bComposite && (eColorSpace == keNvcColorSpace_BGRA || eColorSpace == keNvcColorSpace_RGBA))
		bIsNonTemporalUsed = false;

	if (bIsSrcAligned) {
		if (bIsDstAligned) {
			if (bIsNonTemporalUsed)
				__NvcDoBitDepthConversion_ARGB<true, true, true>(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, eColorSpace, nWidth, nHeight);
			else
				__NvcDoBitDepthConversion_ARGB<true, true, false>(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, eColorSpace, nWidth, nHeight);
		} else {
			__NvcDoBitDepthConversion_ARGB<true, false, false>(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, eColorSpace, nWidth, nHeight);
		}
	} else {
		if (bIsDstAligned) {
			if (bIsNonTemporalUsed)
				__NvcDoBitDepthConversion_ARGB<false, true, true>(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, eColorSpace, nWidth, nHeight);
			else
				__NvcDoBitDepthConversion_ARGB<false, true, false>(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, eColorSpace, nWidth, nHeight);
		} else {
			__NvcDoBitDepthConversion_ARGB<false, false, false>(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, eColorSpace, nWidth, nHeight);
		}
	}

	if (bComposite && (eColorSpace == keNvcColorSpace_BGRA || eColorSpace == keNvcColorSpace_RGBA)) {
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			NvcPremultiply_8Bit_BGRA(pDst, nDstPitch, pDst, nDstPitch, nWidth, nHeight, 0);
			break;
		case keNvcBitDepthInt16:
			//NvcPremultiply_16Bit_BGRA(pDst, nDstPitch, pDst, nDstPitch, nWidth, nHeight, 0);
			break;
		case keNvcBitDepthFloat32:
			//NvcPremultiply_32Bit_BGRA(pDst, nDstPitch, pDst, nDstPitch, nWidth, nHeight, 0);
			break;
		}
	}
}

template<ENvcColorSpace eSrcColorSpace, ENvcColorSpace eDstColorSpace>
static NVC_FORCE_INLINE __m128i __XmmShuffle_ARGB_Packed_16Bit(const __m128i& xmm_pixel)
{
	__m128i xmm_result;

	switch (eSrcColorSpace) {
	case keNvcColorSpace_BGRA:
		switch (eDstColorSpace) {
		case keNvcColorSpace_RGBA:
			xmm_result = _mm_shufflelo_epi16(xmm_pixel, 0xC6);
			xmm_result = _mm_shufflehi_epi16(xmm_result, 0xC6);
			return xmm_result;

		case keNvcColorSpace_ARGB:
			xmm_result = _mm_shufflelo_epi16(xmm_pixel, 0x1B);
			xmm_result = _mm_shufflehi_epi16(xmm_result, 0x1B);
			return xmm_result;
		}
		break;

	case keNvcColorSpace_RGBA:
		switch (eDstColorSpace) {
		case keNvcColorSpace_BGRA:
			xmm_result = _mm_shufflelo_epi16(xmm_pixel, 0xC6);
			xmm_result = _mm_shufflehi_epi16(xmm_result, 0xC6);
			return xmm_result;

		case keNvcColorSpace_ARGB:
			xmm_result = _mm_shufflelo_epi16(xmm_pixel, 0x93);
			xmm_result = _mm_shufflehi_epi16(xmm_result, 0x93);
			return xmm_result;
		}
		break;

	case keNvcColorSpace_ARGB:
		switch (eDstColorSpace) {
		case keNvcColorSpace_BGRA:
			xmm_result = _mm_shufflelo_epi16(xmm_pixel, 0x1B);
			xmm_result = _mm_shufflehi_epi16(xmm_result, 0x1B);
			return xmm_result;

		case keNvcColorSpace_RGBA:
			xmm_result = _mm_shufflelo_epi16(xmm_pixel, 0x39);
			xmm_result = _mm_shufflehi_epi16(xmm_result, 0x39);
			return xmm_result;
		}
		break;
	}

	return xmm_pixel;
}


template<ENvcColorSpace eSrcColorSpace, ENvcColorSpace eDstColorSpace, typename T>
static NVC_FORCE_INLINE void __Shuffle_ARGB(const T *pSrcPixel, T *pDstPixel)
{
	switch (eSrcColorSpace) {
	case keNvcColorSpace_BGRA:
		switch (eDstColorSpace) {
		case keNvcColorSpace_RGBA:
			pDstPixel[0] = pSrcPixel[2];
			pDstPixel[1] = pSrcPixel[1];
			pDstPixel[2] = pSrcPixel[0];
			pDstPixel[3] = pSrcPixel[3];
			return;

		case keNvcColorSpace_ARGB:
			pDstPixel[0] = pSrcPixel[3];
			pDstPixel[1] = pSrcPixel[2];
			pDstPixel[2] = pSrcPixel[1];
			pDstPixel[3] = pSrcPixel[0];
			return;
		}
		break;

	case keNvcColorSpace_RGBA:
		switch (eDstColorSpace) {
		case keNvcColorSpace_BGRA:
			pDstPixel[0] = pSrcPixel[2];
			pDstPixel[1] = pSrcPixel[1];
			pDstPixel[2] = pSrcPixel[0];
			pDstPixel[3] = pSrcPixel[3];
			return;

		case keNvcColorSpace_ARGB:
			pDstPixel[0] = pSrcPixel[3];
			pDstPixel[1] = pSrcPixel[0];
			pDstPixel[2] = pSrcPixel[1];
			pDstPixel[3] = pSrcPixel[2];
			return;
		}
		break;

	case keNvcColorSpace_ARGB:
		switch (eDstColorSpace) {
		case keNvcColorSpace_BGRA:
			pDstPixel[0] = pSrcPixel[3];
			pDstPixel[1] = pSrcPixel[2];
			pDstPixel[2] = pSrcPixel[1];
			pDstPixel[3] = pSrcPixel[0];
			return;

		case keNvcColorSpace_RGBA:
			pDstPixel[0] = pSrcPixel[1];
			pDstPixel[1] = pSrcPixel[2];
			pDstPixel[2] = pSrcPixel[3];
			pDstPixel[3] = pSrcPixel[0];
			return;
		}
		break;
	}

	pDstPixel[0] = pSrcPixel[0];
	pDstPixel[1] = pSrcPixel[1];
	pDstPixel[2] = pSrcPixel[2];
	pDstPixel[3] = pSrcPixel[3];
}

//////////////////////////////////////////////////////////////////////////
// Conversion from ARGB to ARGB
//////////////////////////////////////////////////////////////////////////

template<ENvcColorSpace eSrcColorSpace, ENvcColorSpace eDstColorSpace, bool bIsSrcAligned, bool bIsDstAligned, bool bIsNonTemporalUsed>
class __CNvcColorSpaceConv_ARGB_8B_8B : public __CNvcBaseConverter<bIsSrcAligned, bIsDstAligned, bIsNonTemporalUsed>
{
public:
	static NVC_FORCE_INLINE unsigned int SrcPixelSizeInBytes() { return 4; }
	static NVC_FORCE_INLINE unsigned int DstPixelSizeInBytes() { return 4; }
	static NVC_FORCE_INLINE unsigned int PixelCountInGroup() { return 4; }

	NVC_FORCE_INLINE void ConvertGroup(const void *pSrcPtr, void *pDstPtr)
	{
        __m128i xmm_src_pixel = this->LoadSI128(pSrcPtr);
		__m128i xmm_src_pixel_low = _mm_unpacklo_epi8(xmm_src_pixel, g_xmm_zero);
		__m128i xmm_src_pixel_high = _mm_unpackhi_epi8(xmm_src_pixel, g_xmm_zero);
		xmm_src_pixel_low = __XmmShuffle_ARGB_Packed_16Bit<eSrcColorSpace, eDstColorSpace>(xmm_src_pixel_low);
		xmm_src_pixel_high = __XmmShuffle_ARGB_Packed_16Bit<eSrcColorSpace, eDstColorSpace>(xmm_src_pixel_high);
		__m128i xmm_dst_pixel = _mm_packus_epi16(xmm_src_pixel_low, xmm_src_pixel_high);
        this->StoreSI128(pDstPtr, xmm_dst_pixel);
	}

	NVC_FORCE_INLINE void ConvertPixel(const void *pSrcPtr, void *pDstPtr)
	{
		__Shuffle_ARGB<eSrcColorSpace, eDstColorSpace, uint8_t>((const uint8_t *)pSrcPtr, (uint8_t *)pDstPtr);
	}
};


template<class Converter>
void __NvcDoColorSpaceConversion_ARGB_Impl(const void *pSrc,
	int nSrcPitch,
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight)
{
	Converter conv;
	const unsigned int nGroupCount = nWidth / conv.PixelCountInGroup();
	const unsigned int nTailCount = nWidth % conv.PixelCountInGroup();
	const unsigned int nSrcGroupStride = conv.PixelCountInGroup() * conv.SrcPixelSizeInBytes();
	const unsigned int nDstGroupStride = conv.PixelCountInGroup() * conv.DstPixelSizeInBytes();

	const void *pSrcLine = pSrc;
	void *pDstLine = pDst;

	for (unsigned int row = 0; row < nHeight; row++) {
		const void *pSrcPtr = pSrcLine;
		void *pDstPtr = pDstLine;

		for (unsigned int i = 0; i < nGroupCount; i++) {
			conv.ConvertGroup(pSrcPtr, pDstPtr);
			pSrcPtr = (char *)pSrcPtr + nSrcGroupStride;
			pDstPtr = (char *)pDstPtr + nDstGroupStride;
		}

		for (unsigned int i = 0; i < nTailCount; i++) {
			conv.ConvertPixel(pSrcPtr, pDstPtr);
			pSrcPtr = (char *)pSrcPtr + conv.SrcPixelSizeInBytes();
			pDstPtr = (char *)pDstPtr + conv.DstPixelSizeInBytes();
		}

		pSrcLine = (char *)pSrcLine + nSrcPitch;
		pDstLine = (char *)pDstLine + nDstPitch;
	}

	conv.SFence();
}

template<ENvcColorSpace eSrcColorSpace, ENvcColorSpace eDstColorSpace, bool bIsSrcAligned, bool bIsDstAligned, bool bIsNonTemporalUsed>
void __NvcDoColorSpaceConversion_ARGB_Ext(const void *pSrc,
	int nSrcPitch,
	ENvcBitDepth eSrcBitDepth,
	void *pDst,
	int nDstPitch,
	ENvcBitDepth eDstBitDepth,
	unsigned int nWidth,
	unsigned int nHeight)
{
	switch (eSrcBitDepth) {
	case keNvcBitDepthInt8:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			__NvcDoColorSpaceConversion_ARGB_Impl<__CNvcColorSpaceConv_ARGB_8B_8B<eSrcColorSpace, eDstColorSpace, bIsSrcAligned, bIsDstAligned, bIsNonTemporalUsed> >(pSrc, nSrcPitch, pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthInt16:
			//__NvcDoColorSpaceConversion_ARGB_Impl<__CNvcColorSpaceConv_ARGB_8B_16B<eSrcColorSpace, eDstColorSpace, bIsSrcAligned, bIsDstAligned, bIsNonTemporalUsed> >(pSrc, nSrcPitch, pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthFloat32:
			//__NvcDoColorSpaceConversion_ARGB_Impl<__CNvcColorSpaceConv_ARGB_8B_32B<eSrcColorSpace, eDstColorSpace, bIsSrcAligned, bIsDstAligned, bIsNonTemporalUsed> >(pSrc, nSrcPitch, pDst, nDstPitch, nWidth, nHeight);
			break;
		}
		break;

	case keNvcBitDepthInt16:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			//__NvcDoColorSpaceConversion_ARGB_Impl<__CNvcColorSpaceConv_ARGB_16B_8B<eSrcColorSpace, eDstColorSpace, bIsSrcAligned, bIsDstAligned, bIsNonTemporalUsed> >(pSrc, nSrcPitch, pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthInt16:
			//__NvcDoColorSpaceConversion_ARGB_Impl<__CNvcColorSpaceConv_ARGB_16B_16B<eSrcColorSpace, eDstColorSpace, bIsSrcAligned, bIsDstAligned, bIsNonTemporalUsed> >(pSrc, nSrcPitch, pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthFloat32:
			//__NvcDoColorSpaceConversion_ARGB_Impl<__CNvcColorSpaceConv_ARGB_16B_32B<eSrcColorSpace, eDstColorSpace, bIsSrcAligned, bIsDstAligned, bIsNonTemporalUsed> >(pSrc, nSrcPitch, pDst, nDstPitch, nWidth, nHeight);
			break;
		}
		break;

	case keNvcBitDepthFloat32:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			//__NvcDoColorSpaceConversion_ARGB_Impl<__CNvcColorSpaceConv_ARGB_32B_8B<eSrcColorSpace, eDstColorSpace, bIsSrcAligned, bIsDstAligned, bIsNonTemporalUsed> >(pSrc, nSrcPitch, pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthInt16:
			//__NvcDoColorSpaceConversion_ARGB_Impl<__CNvcColorSpaceConv_ARGB_32B_16B<eSrcColorSpace, eDstColorSpace, bIsSrcAligned, bIsDstAligned, bIsNonTemporalUsed> >(pSrc, nSrcPitch, pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthFloat32:
			//__NvcDoColorSpaceConversion_ARGB_Impl<__CNvcColorSpaceConv_ARGB_32B_32B<eSrcColorSpace, eDstColorSpace, bIsSrcAligned, bIsDstAligned, bIsNonTemporalUsed> >(pSrc, nSrcPitch, pDst, nDstPitch, nWidth, nHeight);
			break;
		}
		break;
	}
}

template<ENvcColorSpace eSrcColorSpace, ENvcColorSpace eDstColorSpace>
void __NvcDoColorSpaceConversion_ARGB(const void *pSrc,
	int nSrcPitch,
	ENvcBitDepth eSrcBitDepth,
	void *pDst,
	int nDstPitch,
	ENvcBitDepth eDstBitDepth,
	unsigned int nWidth,
	unsigned int nHeight,
	int bIsNonTemporal)
{
	const bool bIsSrcAligned = ((size_t)pSrc & 15 || nSrcPitch & 15) ? false : true;
	const bool bIsDstAligned = ((size_t)pDst & 15 || nDstPitch & 15) ? false : true;
	const bool bIsNonTemporalUsed = bIsNonTemporal && bIsDstAligned;

	if (bIsSrcAligned) {
		if (bIsDstAligned) {
			if (bIsNonTemporalUsed)
				__NvcDoColorSpaceConversion_ARGB_Ext<eSrcColorSpace, eDstColorSpace, true, true, true>(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, nWidth, nHeight);
			else
				__NvcDoColorSpaceConversion_ARGB_Ext<eSrcColorSpace, eDstColorSpace, true, true, false>(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, nWidth, nHeight);
		} else {
			__NvcDoColorSpaceConversion_ARGB_Ext<eSrcColorSpace, eDstColorSpace, true, false, false>(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, nWidth, nHeight);
		}
	} else {
		if (bIsDstAligned) {
			if (bIsNonTemporalUsed)
				__NvcDoColorSpaceConversion_ARGB_Ext<eSrcColorSpace, eDstColorSpace, false, true, true>(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, nWidth, nHeight);
			else
				__NvcDoColorSpaceConversion_ARGB_Ext<eSrcColorSpace, eDstColorSpace, false, true, false>(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, nWidth, nHeight);
		} else {
			__NvcDoColorSpaceConversion_ARGB_Ext<eSrcColorSpace, eDstColorSpace, false, false, false>(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, nWidth, nHeight);
		}
	}
}
void __NvcColorSpaceConversion_ARGB(const void *pSrc,
	int nSrcPitch,
	ENvcColorSpace eSrcColorSpace,
	ENvcBitDepth eSrcBitDepth,
	void *pDst,
	int nDstPitch,
	ENvcColorSpace eDstColorSpace,
	ENvcBitDepth eDstBitDepth,
	unsigned int nWidth,
	unsigned int nHeight,
	int bIsNonTemporal)
{
	switch (eSrcColorSpace) {
	case keNvcColorSpace_BGRA:
		switch (eDstColorSpace) {
		case keNvcColorSpace_RGBA:
			__NvcDoColorSpaceConversion_ARGB<keNvcColorSpace_BGRA, keNvcColorSpace_RGBA>(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, nWidth, nHeight, bIsNonTemporal);
			break;

		case keNvcColorSpace_ARGB:
			__NvcDoColorSpaceConversion_ARGB<keNvcColorSpace_BGRA, keNvcColorSpace_ARGB>(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, nWidth, nHeight, bIsNonTemporal);
			break;
		}
		break;

	case keNvcColorSpace_RGBA:
		switch (eDstColorSpace) {
		case keNvcColorSpace_BGRA:
			__NvcDoColorSpaceConversion_ARGB<keNvcColorSpace_RGBA, keNvcColorSpace_BGRA>(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, nWidth, nHeight, bIsNonTemporal);
			break;

		case keNvcColorSpace_ARGB:
			__NvcDoColorSpaceConversion_ARGB<keNvcColorSpace_RGBA, keNvcColorSpace_ARGB>(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, nWidth, nHeight, bIsNonTemporal);
			break;
		}
		break;

	case keNvcColorSpace_ARGB:
		switch (eDstColorSpace) {
		case keNvcColorSpace_BGRA:
			__NvcDoColorSpaceConversion_ARGB<keNvcColorSpace_ARGB, keNvcColorSpace_BGRA>(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, nWidth, nHeight, bIsNonTemporal);
			break;

		case keNvcColorSpace_RGBA:
			__NvcDoColorSpaceConversion_ARGB<keNvcColorSpace_ARGB, keNvcColorSpace_RGBA>(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, nWidth, nHeight, bIsNonTemporal);
			break;
		}
		break;
	}
}

template<ENvcColorSpace eColorSpace, typename T>
static NVC_FORCE_INLINE void __Extract_ARGB_Channel(const T *pixel, T& red, T& green, T& blue, T& alpha)
{
	switch (eColorSpace) {
	case keNvcColorSpace_BGRA:
		red = pixel[2];
		green = pixel[1];
		blue = pixel[0];
		alpha = pixel[3];
		break;

	case keNvcColorSpace_RGBA:
		red = pixel[0];
		green = pixel[1];
		blue = pixel[2];
		alpha = pixel[3];
		break;

	case keNvcColorSpace_ARGB:
		red = pixel[1];
		green = pixel[2];
		blue = pixel[3];
		alpha = pixel[0];
		break;
	}
}

template<typename T>
static NVC_FORCE_INLINE void __Composite_ARGB(T& red, T& green, T& blue, const T alpha)
{
	if (sizeof(T) == 1) {
		red = (T)(((unsigned int)red * (unsigned int)alpha + 127) / 255);
		green = (T)(((unsigned int)green * (unsigned int)alpha + 127) / 255);
		blue = (T)(((unsigned int)blue * (unsigned int)alpha + 127) / 255);
	} else if (sizeof(T) == 2) {
		red = (T)(((unsigned int)red * (unsigned int)alpha + 32767) / 65535);
		green = (T)(((unsigned int)green * (unsigned int)alpha + 32767) / 65535);
		blue = (T)(((unsigned int)blue * (unsigned int)alpha + 32767) / 65535);
	} else if (sizeof(T) == 4) {
		red *= alpha;
		green *= alpha;
		blue *= alpha;
	}
}


template<typename SrcType, typename DstType>
static NVC_FORCE_INLINE DstType __ConvertElementBitDepth(SrcType element)
{
    switch (sizeof(SrcType)) {
    case 1:
        switch (sizeof(DstType)) {
        case 1:
            return (DstType)element;

        case 2:
            return (DstType)__Conv_8Bit_To_16Bit((uint8_t)element);

        case 4:
            return (DstType)__Conv_8Bit_To_32Bit((uint8_t)element);
        }
        break;

    case 2:
        switch (sizeof(DstType)) {
        case 1:
            return (DstType)__Conv_16Bit_To_8Bit((uint16_t)element);

        case 2:
            return (DstType)element;

        case 4:
            return (DstType)__Conv_16Bit_To_32Bit((uint16_t)element);
        }
        break;

    case 4:
        switch (sizeof(DstType)) {
        case 1:
            return (DstType)__Conv_32Bit_To_8Bit((float)element);

        case 2:
            return (DstType)__Conv_32Bit_To_16Bit((float)element);

        case 4:
            return (DstType)element;
        }
        break;
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////
// Conversion from ARGB to RGB
//////////////////////////////////////////////////////////////////////////

template<ENvcColorSpace eSrcColorSpace, typename SrcType, typename DstType>
void __NvcDoColorSpaceConversion_ARGB_To_RGB_Impl(const SrcType *pSrc,
	int nSrcPitch,
	DstType *pDst,
	int nDstPitch,
	bool bIsDstBGR,
	unsigned int nWidth,
	unsigned int nHeight,
	bool bComposite)
{
	const SrcType *pSrcLine = pSrc;
	DstType *pDstLine = pDst;

	for (unsigned int row = 0; row < nHeight; row++) {
		const SrcType *pSrcPtr = pSrcLine;
		DstType *pDstPtr = pDstLine;

		if (bIsDstBGR) {
			for (unsigned int i = 0; i < nWidth; i++) {
				SrcType red, green, blue, alpha;
				__Extract_ARGB_Channel<eSrcColorSpace, SrcType>(pSrcPtr, red, green, blue, alpha);
				if (bComposite)
					__Composite_ARGB<SrcType>(red, green, blue, alpha);

				pDstPtr[0] = __ConvertElementBitDepth<SrcType, DstType>(blue);
				pDstPtr[1] = __ConvertElementBitDepth<SrcType, DstType>(green);
				pDstPtr[2] = __ConvertElementBitDepth<SrcType, DstType>(red);

				pSrcPtr += 4;
				pDstPtr += 3;
			}
		} else {
			for (unsigned int i = 0; i < nWidth; i++) {
				SrcType red, green, blue, alpha;
				__Extract_ARGB_Channel<eSrcColorSpace, SrcType>(pSrcPtr, red, green, blue, alpha);
				if (bComposite)
					__Composite_ARGB<SrcType>(red, green, blue, alpha);

				pDstPtr[0] = __ConvertElementBitDepth<SrcType, DstType>(red);
				pDstPtr[1] = __ConvertElementBitDepth<SrcType, DstType>(green);
				pDstPtr[2] = __ConvertElementBitDepth<SrcType, DstType>(blue);

				pSrcPtr += 4;
				pDstPtr += 3;
			}
		}

		pSrcLine = (const SrcType *)((char *)pSrcLine + nSrcPitch);
		pDstLine = (DstType *)((char *)pDstLine + nDstPitch);
	}
}
template<ENvcColorSpace eSrcColorSpace>
void __NvcDoColorSpaceConversion_ARGB_To_RGB(const void *pSrc,
	int nSrcPitch,
	ENvcBitDepth eSrcBitDepth,
	void *pDst,
	int nDstPitch,
	ENvcColorSpace eDstColorSpace,
	ENvcBitDepth eDstBitDepth,
	unsigned int nWidth,
	unsigned int nHeight,
	bool bComposite)
{
	const bool bIsDstBGR = eDstColorSpace == keNvcColorSpace_BGR;

	switch (eSrcBitDepth) {
	case keNvcBitDepthInt8:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			__NvcDoColorSpaceConversion_ARGB_To_RGB_Impl<eSrcColorSpace, uint8_t, uint8_t>((uint8_t *)pSrc, nSrcPitch, (uint8_t *)pDst, nDstPitch, bIsDstBGR, nWidth, nHeight, bComposite);
			break;

		case keNvcBitDepthInt16:
			__NvcDoColorSpaceConversion_ARGB_To_RGB_Impl<eSrcColorSpace, uint8_t, uint16_t>((uint8_t *)pSrc, nSrcPitch, (uint16_t *)pDst, nDstPitch, bIsDstBGR, nWidth, nHeight, bComposite);
			break;

		case keNvcBitDepthFloat32:
			__NvcDoColorSpaceConversion_ARGB_To_RGB_Impl<eSrcColorSpace, uint8_t, float>((uint8_t *)pSrc, nSrcPitch, (float *)pDst, nDstPitch, bIsDstBGR, nWidth, nHeight, bComposite);
			break;
		}
		break;

	case keNvcBitDepthInt16:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			__NvcDoColorSpaceConversion_ARGB_To_RGB_Impl<eSrcColorSpace, uint16_t, uint8_t>((uint16_t *)pSrc, nSrcPitch, (uint8_t *)pDst, nDstPitch, bIsDstBGR, nWidth, nHeight, bComposite);
			break;

		case keNvcBitDepthInt16:
			__NvcDoColorSpaceConversion_ARGB_To_RGB_Impl<eSrcColorSpace, uint16_t, uint16_t>((uint16_t *)pSrc, nSrcPitch, (uint16_t *)pDst, nDstPitch, bIsDstBGR, nWidth, nHeight, bComposite);
			break;

		case keNvcBitDepthFloat32:
			__NvcDoColorSpaceConversion_ARGB_To_RGB_Impl<eSrcColorSpace, uint16_t, float>((uint16_t *)pSrc, nSrcPitch, (float *)pDst, nDstPitch, bIsDstBGR, nWidth, nHeight, bComposite);
			break;
		}
		break;

	case keNvcBitDepthFloat32:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			__NvcDoColorSpaceConversion_ARGB_To_RGB_Impl<eSrcColorSpace, float, uint8_t>((float *)pSrc, nSrcPitch, (uint8_t *)pDst, nDstPitch, bIsDstBGR, nWidth, nHeight, bComposite);
			break;

		case keNvcBitDepthInt16:
			__NvcDoColorSpaceConversion_ARGB_To_RGB_Impl<eSrcColorSpace, float, uint16_t>((float *)pSrc, nSrcPitch, (uint16_t *)pDst, nDstPitch, bIsDstBGR, nWidth, nHeight, bComposite);
			break;

		case keNvcBitDepthFloat32:
			__NvcDoColorSpaceConversion_ARGB_To_RGB_Impl<eSrcColorSpace, float, float>((float *)pSrc, nSrcPitch, (float *)pDst, nDstPitch, bIsDstBGR, nWidth, nHeight, bComposite);
			break;
		}
		break;
	}
}

void __NvcColorSpaceConversion_ARGB_To_RGB(const void *pSrc,
	int nSrcPitch,
	ENvcColorSpace eSrcColorSpace,
	ENvcBitDepth eSrcBitDepth,
	void *pDst,
	int nDstPitch,
	ENvcColorSpace eDstColorSpace,
	ENvcBitDepth eDstBitDepth,
	unsigned int nWidth,
	unsigned int nHeight,
	bool bComposite)
{
	switch (eSrcColorSpace) {
	case keNvcColorSpace_BGRA:
		__NvcDoColorSpaceConversion_ARGB_To_RGB<keNvcColorSpace_BGRA>(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstColorSpace, eDstBitDepth, nWidth, nHeight, bComposite);
		break;

	case keNvcColorSpace_RGBA:
		__NvcDoColorSpaceConversion_ARGB_To_RGB<keNvcColorSpace_RGBA>(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstColorSpace, eDstBitDepth, nWidth, nHeight, bComposite);
		break;

	case keNvcColorSpace_ARGB:
		__NvcDoColorSpaceConversion_ARGB_To_RGB<keNvcColorSpace_ARGB>(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstColorSpace, eDstBitDepth, nWidth, nHeight, bComposite);
		break;
	}
}




template<typename SrcType, typename DstType>
void __NvcDoColorSpaceConversion_ARGB_To_A(const SrcType *pSrc,
	int nSrcPitch,
	int nAlphaIndex,
	DstType *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight)
{
	const SrcType *pSrcLine = pSrc;
	DstType *pDstLine = pDst;

	for (unsigned int row = 0; row < nHeight; row++) {
		const SrcType *pSrcPtr = pSrcLine;
		DstType *pDstPtr = pDstLine;

		for (unsigned int i = 0; i < nWidth; i++) {
			*pDstPtr = __ConvertElementBitDepth<SrcType, DstType>(pSrcPtr[nAlphaIndex]);
			pSrcPtr += 4;
			++pDstPtr;
		}

		pSrcLine = (const SrcType *)((char *)pSrcLine + nSrcPitch);
		pDstLine = (DstType *)((char *)pDstLine + nDstPitch);
	}
}

void __NvcColorSpaceConversion_ARGB_To_A(const void *pSrc,
	int nSrcPitch,
	ENvcColorSpace eSrcColorSpace,
	ENvcBitDepth eSrcBitDepth,
	void *pDst,
	int nDstPitch,
	ENvcBitDepth eDstBitDepth,
	unsigned int nWidth,
	unsigned int nHeight)
{
	int nAlphaIndex = 0;
	switch (eSrcColorSpace) {
	case keNvcColorSpace_BGRA:
	case keNvcColorSpace_RGBA:
		nAlphaIndex = 3;
		break;
	}

	switch (eSrcBitDepth) {
	case keNvcBitDepthInt8:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			__NvcDoColorSpaceConversion_ARGB_To_A<uint8_t, uint8_t>((uint8_t *)pSrc, nSrcPitch, nAlphaIndex, (uint8_t *)pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthInt16:
			__NvcDoColorSpaceConversion_ARGB_To_A<uint8_t, uint16_t>((uint8_t *)pSrc, nSrcPitch, nAlphaIndex, (uint16_t *)pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthFloat32:
			__NvcDoColorSpaceConversion_ARGB_To_A<uint8_t, float>((uint8_t *)pSrc, nSrcPitch, nAlphaIndex, (float *)pDst, nDstPitch, nWidth, nHeight);
			break;
		}
		break;

	case keNvcBitDepthInt16:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			__NvcDoColorSpaceConversion_ARGB_To_A<uint16_t, uint8_t>((uint16_t *)pSrc, nSrcPitch, nAlphaIndex, (uint8_t *)pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthInt16:
			__NvcDoColorSpaceConversion_ARGB_To_A<uint16_t, uint16_t>((uint16_t *)pSrc, nSrcPitch, nAlphaIndex, (uint16_t *)pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthFloat32:
			__NvcDoColorSpaceConversion_ARGB_To_A<uint16_t, float>((uint16_t *)pSrc, nSrcPitch, nAlphaIndex, (float *)pDst, nDstPitch, nWidth, nHeight);
			break;
		}
		break;

	case keNvcBitDepthFloat32:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			__NvcDoColorSpaceConversion_ARGB_To_A<float, uint8_t>((float *)pSrc, nSrcPitch, nAlphaIndex, (uint8_t *)pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthInt16:
			__NvcDoColorSpaceConversion_ARGB_To_A<float, uint16_t>((float *)pSrc, nSrcPitch, nAlphaIndex, (uint16_t *)pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthFloat32:
			__NvcDoColorSpaceConversion_ARGB_To_A<float, float>((float *)pSrc, nSrcPitch, nAlphaIndex, (float *)pDst, nDstPitch, nWidth, nHeight);
			break;
		}
		break;
	}
}

template<ENvcColorSpace eColorSpace, typename T>
static NVC_FORCE_INLINE void __Fill_ARGB(const T red, const T green, const T blue, const T alpha, T *pixel)
{
	switch (eColorSpace) {
	case keNvcColorSpace_BGRA:
		pixel[0] = blue;
		pixel[1] = green;
		pixel[2] = red;
		pixel[3] = alpha;
		break;

	case keNvcColorSpace_RGBA:
		pixel[0] = red;
		pixel[1] = green;
		pixel[2] = blue;
		pixel[3] = alpha;
		break;

	case keNvcColorSpace_ARGB:
		pixel[0] = alpha;
		pixel[1] = red;
		pixel[2] = green;
		pixel[3] = blue;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// Conversion from RGB to ARGB
//////////////////////////////////////////////////////////////////////////

template<ENvcColorSpace eDstColorSpace, typename SrcType, typename DstType>
void __NvcDoColorSpaceConversion_RGB_To_ARGB_Impl(const SrcType *pSrc,
	int nSrcPitch,
	bool bIsSrcBGR,
	DstType *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	float fPaddingAlpha)
{
	DstType dstAlpha = 0;
	if (sizeof(DstType) == 1)
		dstAlpha = (DstType)(fPaddingAlpha * 255 + 0.5f);
	else if (sizeof(DstType) == 2)
		dstAlpha = (DstType)(fPaddingAlpha * 65535 + 0.5f);
	else if (sizeof(DstType) == 4)
		dstAlpha = (DstType)fPaddingAlpha;

	const SrcType *pSrcLine = pSrc;
	DstType *pDstLine = pDst;

	for (unsigned int row = 0; row < nHeight; row++) {
		const SrcType *pSrcPtr = pSrcLine;
		DstType *pDstPtr = pDstLine;

		if (bIsSrcBGR) {
			for (unsigned int i = 0; i < nWidth; i++) {
				DstType red = __ConvertElementBitDepth<SrcType, DstType>(pSrcPtr[2]);
				DstType green = __ConvertElementBitDepth<SrcType, DstType>(pSrcPtr[1]);
				DstType blue = __ConvertElementBitDepth<SrcType, DstType>(pSrcPtr[0]);
				__Fill_ARGB<eDstColorSpace, DstType>(red, green, blue, dstAlpha, pDstPtr);

				pSrcPtr += 3;
				pDstPtr += 4;
			}
		} else {
			for (unsigned int i = 0; i < nWidth; i++) {
				DstType red = __ConvertElementBitDepth<SrcType, DstType>(pSrcPtr[0]);
				DstType green = __ConvertElementBitDepth<SrcType, DstType>(pSrcPtr[1]);
				DstType blue = __ConvertElementBitDepth<SrcType, DstType>(pSrcPtr[2]);
				__Fill_ARGB<eDstColorSpace, DstType>(red, green, blue, dstAlpha, pDstPtr);

				pSrcPtr += 3;
				pDstPtr += 4;
			}
		}

		pSrcLine = (const SrcType *)((char *)pSrcLine + nSrcPitch);
		pDstLine = (DstType *)((char *)pDstLine + nDstPitch);
	}
}

template<ENvcColorSpace eDstColorSpace>
void __NvcDoColorSpaceConversion_RGB_To_ARGB(const void *pSrc,
	int nSrcPitch,
	ENvcColorSpace eSrcColorSpace,
	ENvcBitDepth eSrcBitDepth,
	void *pDst,
	int nDstPitch,
	ENvcBitDepth eDstBitDepth,
	unsigned int nWidth,
	unsigned int nHeight,
	float fPaddingAlpha)
{
	const bool bIsSrcBGR = eSrcColorSpace == keNvcColorSpace_BGR;

	switch (eSrcBitDepth) {
	case keNvcBitDepthInt8:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			__NvcDoColorSpaceConversion_RGB_To_ARGB_Impl<eDstColorSpace, uint8_t, uint8_t>((uint8_t *)pSrc, nSrcPitch, bIsSrcBGR, (uint8_t *)pDst, nDstPitch, nWidth, nHeight, fPaddingAlpha);
			break;

		case keNvcBitDepthInt16:
			//__NvcDoColorSpaceConversion_RGB_To_ARGB_Impl<eDstColorSpace, uint8_t, uint16_t>((uint8_t *)pSrc, nSrcPitch, bIsSrcBGR, (uint16_t *)pDst, nDstPitch, nWidth, nHeight, fPaddingAlpha);
			break;

		case keNvcBitDepthFloat32:
			//__NvcDoColorSpaceConversion_RGB_To_ARGB_Impl<eDstColorSpace, uint8_t, float>((uint8_t *)pSrc, nSrcPitch, bIsSrcBGR, (float *)pDst, nDstPitch, nWidth, nHeight, fPaddingAlpha);
			break;
		}
		break;

	case keNvcBitDepthInt16:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			__NvcDoColorSpaceConversion_RGB_To_ARGB_Impl<eDstColorSpace, uint16_t, uint8_t>((uint16_t *)pSrc, nSrcPitch, bIsSrcBGR, (uint8_t *)pDst, nDstPitch, nWidth, nHeight, fPaddingAlpha);
			break;

		case keNvcBitDepthInt16:
			__NvcDoColorSpaceConversion_RGB_To_ARGB_Impl<eDstColorSpace, uint16_t, uint16_t>((uint16_t *)pSrc, nSrcPitch, bIsSrcBGR, (uint16_t *)pDst, nDstPitch, nWidth, nHeight, fPaddingAlpha);
			break;

		case keNvcBitDepthFloat32:
			__NvcDoColorSpaceConversion_RGB_To_ARGB_Impl<eDstColorSpace, uint16_t, float>((uint16_t *)pSrc, nSrcPitch, bIsSrcBGR, (float *)pDst, nDstPitch, nWidth, nHeight, fPaddingAlpha);
			break;
		}
		break;

	case keNvcBitDepthFloat32:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			__NvcDoColorSpaceConversion_RGB_To_ARGB_Impl<eDstColorSpace, float, uint8_t>((float *)pSrc, nSrcPitch, bIsSrcBGR, (uint8_t *)pDst, nDstPitch, nWidth, nHeight, fPaddingAlpha);
			break;

		case keNvcBitDepthInt16:
			__NvcDoColorSpaceConversion_RGB_To_ARGB_Impl<eDstColorSpace, float, uint16_t>((float *)pSrc, nSrcPitch, bIsSrcBGR, (uint16_t *)pDst, nDstPitch, nWidth, nHeight, fPaddingAlpha);
			break;

		case keNvcBitDepthFloat32:
			__NvcDoColorSpaceConversion_RGB_To_ARGB_Impl<eDstColorSpace, float, float>((float *)pSrc, nSrcPitch, bIsSrcBGR, (float *)pDst, nDstPitch, nWidth, nHeight, fPaddingAlpha);
			break;
		}
		break;
	}
}

void __NvcColorSpaceConversion_RGB_To_ARGB(const void *pSrc,
	int nSrcPitch,
	ENvcColorSpace eSrcColorSpace,
	ENvcBitDepth eSrcBitDepth,
	void *pDst,
	int nDstPitch,
	ENvcColorSpace eDstColorSpace,
	ENvcBitDepth eDstBitDepth,
	unsigned int nWidth,
	unsigned int nHeight,
	float fPaddingAlpha)
{
	switch (eDstColorSpace) {
	case keNvcColorSpace_BGRA:
		__NvcDoColorSpaceConversion_RGB_To_ARGB<keNvcColorSpace_BGRA>(pSrc, nSrcPitch, eSrcColorSpace, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, nWidth, nHeight, fPaddingAlpha);
		break;

	case keNvcColorSpace_RGBA:
		__NvcDoColorSpaceConversion_RGB_To_ARGB<keNvcColorSpace_RGBA>(pSrc, nSrcPitch, eSrcColorSpace, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, nWidth, nHeight, fPaddingAlpha);
		break;

	case keNvcColorSpace_ARGB:
		__NvcDoColorSpaceConversion_RGB_To_ARGB<keNvcColorSpace_ARGB>(pSrc, nSrcPitch, eSrcColorSpace, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, nWidth, nHeight, fPaddingAlpha);
		break;
	}
}

template<typename SrcType, typename DstType>
void __NvcDoColorSpaceConversion_RGB(const SrcType *pSrc,
	int nSrcPitch,
	DstType *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight)
{
	const SrcType *pSrcLine = pSrc;
	DstType *pDstLine = pDst;

	for (unsigned int row = 0; row < nHeight; row++) {
		const SrcType *pSrcPtr = pSrcLine;
		DstType *pDstPtr = pDstLine;

		for (unsigned int i = 0; i < nWidth; i++) {
			pDstPtr[0] = __ConvertElementBitDepth<SrcType, DstType>(pSrcPtr[2]);
			pDstPtr[1] = __ConvertElementBitDepth<SrcType, DstType>(pSrcPtr[1]);
			pDstPtr[2] = __ConvertElementBitDepth<SrcType, DstType>(pSrcPtr[0]);
			pSrcPtr += 3;
			pDstPtr += 3;
		}

		pSrcLine = (const SrcType *)((char *)pSrcLine + nSrcPitch);
		pDstLine = (DstType *)((char *)pDstLine + nDstPitch);
	}
}

void __NvcColorSpaceConversion_RGB(const void *pSrc,
	int nSrcPitch,
	ENvcBitDepth eSrcBitDepth,
	void *pDst,
	int nDstPitch,
	ENvcBitDepth eDstBitDepth,
	unsigned int nWidth,
	unsigned int nHeight)
{
	switch (eSrcBitDepth) {
	case keNvcBitDepthInt8:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			__NvcDoColorSpaceConversion_RGB<uint8_t, uint8_t>((uint8_t *)pSrc, nSrcPitch, (uint8_t *)pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthInt16:
			__NvcDoColorSpaceConversion_RGB<uint8_t, uint16_t>((uint8_t *)pSrc, nSrcPitch, (uint16_t *)pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthFloat32:
			__NvcDoColorSpaceConversion_RGB<uint8_t, float>((uint8_t *)pSrc, nSrcPitch, (float *)pDst, nDstPitch, nWidth, nHeight);
			break;
		}
		break;

	case keNvcBitDepthInt16:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			__NvcDoColorSpaceConversion_RGB<uint16_t, uint8_t>((uint16_t *)pSrc, nSrcPitch, (uint8_t *)pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthInt16:
			__NvcDoColorSpaceConversion_RGB<uint16_t, uint16_t>((uint16_t *)pSrc, nSrcPitch, (uint16_t *)pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthFloat32:
			__NvcDoColorSpaceConversion_RGB<uint16_t, float>((uint16_t *)pSrc, nSrcPitch, (float *)pDst, nDstPitch, nWidth, nHeight);
			break;
		}
		break;

	case keNvcBitDepthFloat32:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			__NvcDoColorSpaceConversion_RGB<float, uint8_t>((float *)pSrc, nSrcPitch, (uint8_t *)pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthInt16:
			__NvcDoColorSpaceConversion_RGB<float, uint16_t>((float *)pSrc, nSrcPitch, (uint16_t *)pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthFloat32:
			__NvcDoColorSpaceConversion_RGB<float, float>((float *)pSrc, nSrcPitch, (float *)pDst, nDstPitch, nWidth, nHeight);
			break;
		}
		break;
	}
}

// Clear a surface of Matte of which channel is 8 bits. 
// Color used to be set is supplied by variable nValue.

template <bool bNonTemporalUsed, bool bAligned >
static void _NvcClear_8Bit_Matte(void *pSurface,
	int nPitch,
	unsigned int nWidth, 
	unsigned int nHeight,
	unsigned int nValue)
{
	uint8_t *pSurLine = (uint8_t*)pSurface;

	__m128i xmm_value = _mm_set1_epi8((char)nValue);

	unsigned int nGroupCount = nWidth >> 4;
	unsigned int nTailCount = nWidth & 15;

	for (unsigned int i=0; i<nHeight; i++)
	{
		uint8_t *pSurTmp = pSurLine;
		for (unsigned int j=0; j<nGroupCount; j++)
		{
			if ( bNonTemporalUsed )
				_mm_stream_si128( (__m128i*)pSurTmp, xmm_value );
			else if ( bAligned )
				_mm_store_si128( (__m128i*)pSurTmp, xmm_value );
			else
				_mm_storeu_si128( (__m128i*)pSurTmp, xmm_value );

			pSurTmp += 16;
		}

		if (nTailCount)
			memset(pSurTmp,(int)nValue,nTailCount);

		pSurLine += nPitch;
	}

	if (bNonTemporalUsed)
		_mm_sfence();
}

void NvcClear_8Bit_Matte(void *pSurface,
	int nPitch,
	unsigned int nWidth, 
	unsigned int nHeight,
	unsigned int nValue,
	int bIsNonTemporal)
{
	bool bAligned = ( (size_t)pSurface & 15 || nPitch & 15 ) ? false : true;
	bool bNonTemporalUsed = (bAligned && bIsNonTemporal) ? true : false;

	if ( bNonTemporalUsed )
		_NvcClear_8Bit_Matte<true,true>(	pSurface,
		nPitch,
		nWidth,
		nHeight,
		nValue);	
	else if(bAligned)
		_NvcClear_8Bit_Matte<false,true>(	pSurface,
		nPitch,
		nWidth,
		nHeight,
		nValue);
	else
		_NvcClear_8Bit_Matte<false,false>(	pSurface,
		nPitch,
		nWidth,
		nHeight,
		nValue);
}

template<typename SrcType, typename DstType>
void __NvcDoColorSpaceConversion_A_To_ARGB(const SrcType *pSrc,
	int nSrcPitch,
	DstType *pDst,
	int nDstPitch,
	bool bIsAlphaAtEnd,
	unsigned int nWidth,
	unsigned int nHeight)
{
	const SrcType *pSrcLine = pSrc;
	DstType *pDstLine = pDst;

	DstType dstRGB = 0;
	if (sizeof(DstType) == 1)
		dstRGB = (DstType)255;
	else if (sizeof(DstType) == 2)
		dstRGB = (DstType)65535;
	else if (sizeof(DstType) == 4)
		dstRGB = 1;

	for (unsigned int row = 0; row < nHeight; row++) {
		const SrcType *pSrcPtr = pSrcLine;
		DstType *pDstPtr = pDstLine;

		if (bIsAlphaAtEnd) {
			for (unsigned int i = 0; i < nWidth; i++) {
				pDstPtr[0] = dstRGB;
				pDstPtr[1] = dstRGB;
				pDstPtr[2] = dstRGB;
				pDstPtr[3] = __ConvertElementBitDepth<SrcType, DstType>(*pSrcPtr);
				++pSrcPtr;
				pDstPtr += 4;
			}
		} else {
			for (unsigned int i = 0; i < nWidth; i++) {
				pDstPtr[0] = __ConvertElementBitDepth<SrcType, DstType>(*pSrcPtr);
				pDstPtr[1] = dstRGB;
				pDstPtr[2] = dstRGB;
				pDstPtr[3] = dstRGB;
				++pSrcPtr;
				pDstPtr += 4;
			}
		}

		pSrcLine = (const SrcType *)((char *)pSrcLine + nSrcPitch);
		pDstLine = (DstType *)((char *)pDstLine + nDstPitch);
	}
}

void __NvcColorSpaceConversion_A_To_ARGB(const void *pSrc,
	int nSrcPitch,
	ENvcBitDepth eSrcBitDepth,
	void *pDst,
	int nDstPitch,
	ENvcColorSpace eDstColorSpace,
	ENvcBitDepth eDstBitDepth,
	unsigned int nWidth,
	unsigned int nHeight)
{
	const bool bIsAlphaAtEnd = eDstColorSpace != keNvcColorSpace_ARGB;

	switch (eSrcBitDepth) {
	case keNvcBitDepthInt8:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			__NvcDoColorSpaceConversion_A_To_ARGB<uint8_t, uint8_t>((uint8_t *)pSrc, nSrcPitch, (uint8_t *)pDst, nDstPitch, bIsAlphaAtEnd, nWidth, nHeight);
			break;

		case keNvcBitDepthInt16:
			__NvcDoColorSpaceConversion_A_To_ARGB<uint8_t, uint16_t>((uint8_t *)pSrc, nSrcPitch, (uint16_t *)pDst, nDstPitch, bIsAlphaAtEnd, nWidth, nHeight);
			break;

		case keNvcBitDepthFloat32:
			__NvcDoColorSpaceConversion_A_To_ARGB<uint8_t, float>((uint8_t *)pSrc, nSrcPitch, (float *)pDst, nDstPitch, bIsAlphaAtEnd, nWidth, nHeight);
			break;
		}
		break;

	case keNvcBitDepthInt16:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			__NvcDoColorSpaceConversion_A_To_ARGB<uint16_t, uint8_t>((uint16_t *)pSrc, nSrcPitch, (uint8_t *)pDst, nDstPitch, bIsAlphaAtEnd, nWidth, nHeight);
			break;

		case keNvcBitDepthInt16:
			__NvcDoColorSpaceConversion_A_To_ARGB<uint16_t, uint16_t>((uint16_t *)pSrc, nSrcPitch, (uint16_t *)pDst, nDstPitch, bIsAlphaAtEnd, nWidth, nHeight);
			break;

		case keNvcBitDepthFloat32:
			__NvcDoColorSpaceConversion_A_To_ARGB<uint16_t, float>((uint16_t *)pSrc, nSrcPitch, (float *)pDst, nDstPitch, bIsAlphaAtEnd, nWidth, nHeight);
			break;
		}
		break;

	case keNvcBitDepthFloat32:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			__NvcDoColorSpaceConversion_A_To_ARGB<float, uint8_t>((float *)pSrc, nSrcPitch, (uint8_t *)pDst, nDstPitch, bIsAlphaAtEnd, nWidth, nHeight);
			break;

		case keNvcBitDepthInt16:
			__NvcDoColorSpaceConversion_A_To_ARGB<float, uint16_t>((float *)pSrc, nSrcPitch, (uint16_t *)pDst, nDstPitch, bIsAlphaAtEnd, nWidth, nHeight);
			break;

		case keNvcBitDepthFloat32:
			__NvcDoColorSpaceConversion_A_To_ARGB<float, float>((float *)pSrc, nSrcPitch, (float *)pDst, nDstPitch, bIsAlphaAtEnd, nWidth, nHeight);
			break;
		}
		break;
	}
}

template<typename SrcType, typename DstType>
void __NvcDoColorSpaceConversion_A_To_RGB(const SrcType *pSrc,
	int nSrcPitch,
	DstType *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight)
{
	const SrcType *pSrcLine = pSrc;
	DstType *pDstLine = pDst;

	for (unsigned int row = 0; row < nHeight; row++) {
		const SrcType *pSrcPtr = pSrcLine;
		DstType *pDstPtr = pDstLine;

		for (unsigned int i = 0; i < nWidth; i++) {
			DstType alpha = __ConvertElementBitDepth<SrcType, DstType>(*pSrcPtr);
			pDstPtr[0] = alpha;
			pDstPtr[1] = alpha;
			pDstPtr[2] = alpha;

			++pSrcPtr;
			pDstPtr += 3;
		}

		pSrcLine = (const SrcType *)((char *)pSrcLine + nSrcPitch);
		pDstLine = (DstType *)((char *)pDstLine + nDstPitch);
	}
}
void __NvcColorSpaceConversion_A_To_RGB(const void *pSrc,
	int nSrcPitch,
	ENvcBitDepth eSrcBitDepth,
	void *pDst,
	int nDstPitch,
	ENvcBitDepth eDstBitDepth,
	unsigned int nWidth,
	unsigned int nHeight)
{
	switch (eSrcBitDepth) {
	case keNvcBitDepthInt8:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			__NvcDoColorSpaceConversion_A_To_RGB<uint8_t, uint8_t>((uint8_t *)pSrc, nSrcPitch, (uint8_t *)pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthInt16:
			__NvcDoColorSpaceConversion_A_To_RGB<uint8_t, uint16_t>((uint8_t *)pSrc, nSrcPitch, (uint16_t *)pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthFloat32:
			__NvcDoColorSpaceConversion_A_To_RGB<uint8_t, float>((uint8_t *)pSrc, nSrcPitch, (float *)pDst, nDstPitch, nWidth, nHeight);
			break;
		}
		break;

	case keNvcBitDepthInt16:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			__NvcDoColorSpaceConversion_A_To_RGB<uint16_t, uint8_t>((uint16_t *)pSrc, nSrcPitch, (uint8_t *)pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthInt16:
			__NvcDoColorSpaceConversion_A_To_RGB<uint16_t, uint16_t>((uint16_t *)pSrc, nSrcPitch, (uint16_t *)pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthFloat32:
			__NvcDoColorSpaceConversion_A_To_RGB<uint16_t, float>((uint16_t *)pSrc, nSrcPitch, (float *)pDst, nDstPitch, nWidth, nHeight);
			break;
		}
		break;

	case keNvcBitDepthFloat32:
		switch (eDstBitDepth) {
		case keNvcBitDepthInt8:
			__NvcDoColorSpaceConversion_A_To_RGB<float, uint8_t>((float *)pSrc, nSrcPitch, (uint8_t *)pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthInt16:
			__NvcDoColorSpaceConversion_A_To_RGB<float, uint16_t>((float *)pSrc, nSrcPitch, (uint16_t *)pDst, nDstPitch, nWidth, nHeight);
			break;

		case keNvcBitDepthFloat32:
			__NvcDoColorSpaceConversion_A_To_RGB<float, float>((float *)pSrc, nSrcPitch, (float *)pDst, nDstPitch, nWidth, nHeight);
			break;
		}
		break;
	}
}



template<bool bIsSrcAligned, bool bIsDstAligned, bool bIsNonTemporalUsed>
static inline void _Premultiply_8Bit_BGRA(const void *pSrc,
	int nSrcPitch,
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight)
{
	const uint8_t *pSrcLine = (uint8_t *)pSrc;
	uint8_t *pDstLine = (uint8_t *)pDst;

	unsigned int nGroupCount = nWidth / 4;
	unsigned int nTailCount = nWidth & 3;

	const __m128i xmm_zero = _mm_setzero_si128();

	for (unsigned int i = 0; i < nHeight; i++) {
		const uint8_t *pSrcPtr = pSrcLine;
		uint8_t *pDstPtr = pDstLine;

		for (unsigned int j = 0; j < nGroupCount; j++) {
			__m128i xmm_src, xmm_elements1, xmm_elements2, xmm_alpha1, xmm_alpha2;

			if (bIsSrcAligned)
				xmm_src = _mm_load_si128((__m128i *)pSrcPtr);		// [ a3| r3| g3| b3| a2| r2| g2| b2| a1| r1| g1| b1| a0| r0| g0| b0]
			else
				xmm_src = _mm_loadu_si128((__m128i*)pSrcPtr);		// [ a3| r3| g3| b3| a2| r2| g2| b2| a1| r1| g1| b1| a0| r0| g0| b0]

			xmm_elements1 = _mm_unpacklo_epi8(xmm_src, xmm_zero);	// [   a1  |   r1  |   g1  |   b1  |   a0  |   r0  |   g0  |   b0  ]
			xmm_alpha1 = _mm_shufflelo_epi16(xmm_elements1, 0xff);	// [   a1  |   r1  |   g1  |   b1  |   a0  |   a0  |   a0  |   a0  ]
			xmm_alpha1 = _mm_shufflehi_epi16(xmm_alpha1, 0xff);		// [   a1  |   a1  |   a1  |   a1  |   a0  |   a0  |   a0  |   a0  ]
			xmm_alpha1 = _mm_or_si128(xmm_alpha1, g_xmm_alpha_mask_16bit);	// [  255  |   a1  |   a1  |   a1  |  255  |   a0  |   a0  |   a0  ]

			xmm_elements1 = __Multiply_8_Elements_8Bit_Int(xmm_elements1, xmm_alpha1);

			xmm_elements2 = _mm_unpackhi_epi8(xmm_src, xmm_zero);	// [   a3  |   r3  |   g3  |   b3  |   a2  |   r2  |   g2  |   b2  ]
			xmm_alpha2 = _mm_shufflelo_epi16(xmm_elements2, 0xff);	// [   a3  |   r3  |   g3  |   b3  |   a2  |   a2  |   a2  |   a2  ]
			xmm_alpha2 = _mm_shufflehi_epi16(xmm_alpha2, 0xff);		// [   a3  |   a3  |   a3  |   a3  |   a2  |   a2  |   a2  |   a2  ]
			xmm_alpha2 = _mm_or_si128(xmm_alpha2, g_xmm_alpha_mask_16bit);	// [  255  |   a3  |   a3  |   a3  |  255  |   a2  |   a2  |   a2  ]

			xmm_elements2 = __Multiply_8_Elements_8Bit_Int(xmm_elements2, xmm_alpha2);

			xmm_elements1 = _mm_packus_epi16(xmm_elements1, xmm_elements2);

			if (bIsDstAligned) {
				if (bIsNonTemporalUsed)
					_mm_stream_si128((__m128i *)pDstPtr, xmm_elements1);
				else
					_mm_store_si128((__m128i *)pDstPtr, xmm_elements1);
			} else {
				_mm_storeu_si128((__m128i *)pDstPtr, xmm_elements1);
			}

			pSrcPtr += 16;
			pDstPtr += 16;
		}

		for (unsigned int k = 0; k < nTailCount; k++) {
			unsigned int alpha = pSrcPtr[k * 4 + 3];
			pDstPtr[k * 4 + 0] = (uint8_t)((pSrcPtr[k * 4 + 0] * alpha + 127) / 255);
			pDstPtr[k * 4 + 1] = (uint8_t)((pSrcPtr[k * 4 + 1] * alpha + 127) / 255);
			pDstPtr[k * 4 + 2] = (uint8_t)((pSrcPtr[k * 4 + 2] * alpha + 127) / 255);
			pDstPtr[k * 4 + 3] = (uint8_t)alpha;
		}

		pSrcLine = pSrcLine + nSrcPitch;
		pDstLine = pDstLine + nDstPitch;
	}

	if (bIsNonTemporalUsed)
		_mm_sfence();
}

// NOTE: in-place process is possible
void NvcPremultiply_8Bit_BGRA(const void *pSrc,
	int nSrcPitch,
	void *pDst,
	int nDstPitch,
	unsigned int nWidth,
	unsigned int nHeight,
	int bIsNonTemporal)
{
	const bool bIsSrcAligned = ((size_t)pSrc & 15 || nSrcPitch & 15) ? false : true;
	const bool bIsDstAligned = ((size_t)pDst & 15 || nDstPitch & 15) ? false : true;
	const bool bIsNonTemporalUsed = (bIsNonTemporal && bIsDstAligned) ? true : false;

	if (bIsSrcAligned) {
		if (bIsDstAligned) {
			if (bIsNonTemporalUsed)
				_Premultiply_8Bit_BGRA<true, true, true>(pSrc, nSrcPitch, pDst, nDstPitch, nWidth, nHeight);
			else
				_Premultiply_8Bit_BGRA<true, true, false>(pSrc, nSrcPitch, pDst, nDstPitch, nWidth, nHeight);
		} else {
			_Premultiply_8Bit_BGRA<true, false, false>(pSrc, nSrcPitch, pDst, nDstPitch, nWidth, nHeight);
		}
	} else {
		if (bIsDstAligned) {
			if (bIsNonTemporalUsed)
				_Premultiply_8Bit_BGRA<false, true, true>(pSrc, nSrcPitch, pDst, nDstPitch, nWidth, nHeight);
			else
				_Premultiply_8Bit_BGRA<false, true, false>(pSrc, nSrcPitch, pDst, nDstPitch, nWidth, nHeight);
		} else {
			_Premultiply_8Bit_BGRA<false, false, false>(pSrc, nSrcPitch, pDst, nDstPitch, nWidth, nHeight);
		}
	}
}

// perform surface copy
void NvcCopySurface(const void *pSrc,
	int nSrcPitch,
	void *pDst,
	int nDstPitch,
	unsigned int nLineSize,
	unsigned int nHeight)
{
	_copy_surface(pSrc, nSrcPitch, pDst, nDstPitch, nLineSize, nHeight);
}

//
// Color space conversion function for any supported color space which contain A, R, G, B channels,
// source and destination color space can be any of keNvcColorSpace_BGRA, keNvcColorSpace_BGR,
// keNvcColorSpace_RGBA, keNvcColorSpace_RGB, keNvcColorSpace_ARGB or keNvcColorSpace_Alpha.
// source and destination bit depth can be keNvcBitDepthInt8, keNvcBitDepthInt16 or keNvcBitDepthFloat32
//
void NvcColorSpaceConversion_ARGB(const void *pSrc,
	int nSrcPitch,
	ENvcColorSpace eSrcColorSpace,
	ENvcBitDepth eSrcBitDepth,
	void *pDst,
	int nDstPitch,
	ENvcColorSpace eDstColorSpace,
	ENvcBitDepth eDstBitDepth,
	unsigned int nWidth,
	unsigned int nHeight,
	float fPaddingAlpha,
	int bComposite,
	int bIsNonTemporal)
{
	//
	// Sanity check
	//
	if (!nWidth || !nHeight)
		return; // Nothing to do

	assert(eSrcColorSpace == keNvcColorSpace_BGRA || eSrcColorSpace == keNvcColorSpace_BGR || eSrcColorSpace == keNvcColorSpace_RGBA ||
		eSrcColorSpace == keNvcColorSpace_RGB || eSrcColorSpace == keNvcColorSpace_ARGB || eSrcColorSpace == keNvcColorSpace_Alpha);

	assert(eSrcBitDepth == keNvcBitDepthInt8 || eSrcBitDepth == keNvcBitDepthInt16 || eSrcBitDepth == keNvcBitDepthFloat32);

	assert(eDstColorSpace == keNvcColorSpace_BGRA || eDstColorSpace == keNvcColorSpace_BGR || eDstColorSpace == keNvcColorSpace_RGBA ||
		eDstColorSpace == keNvcColorSpace_RGB || eDstColorSpace == keNvcColorSpace_ARGB || eDstColorSpace == keNvcColorSpace_Alpha);

	assert(eDstBitDepth == keNvcBitDepthInt8 || eDstBitDepth == keNvcBitDepthInt16 || eDstBitDepth == keNvcBitDepthFloat32);

	if (eSrcColorSpace == eDstColorSpace) {
		if (eSrcBitDepth == eDstBitDepth) {
			if (bComposite && (eSrcColorSpace == keNvcColorSpace_BGRA || eSrcColorSpace == keNvcColorSpace_RGBA)) {
				switch (eSrcBitDepth) {
				case keNvcBitDepthInt8:
					NvcPremultiply_8Bit_BGRA(pSrc, nSrcPitch, pDst, nDstPitch, nWidth, nHeight, bIsNonTemporal);
					break;
				case keNvcBitDepthInt16:
					//NvcPremultiply_16Bit_BGRA(pSrc, nSrcPitch, pDst, nDstPitch, nWidth, nHeight, bIsNonTemporal);
					break;
				case keNvcBitDepthFloat32:
					//NvcPremultiply_32Bit_BGRA(pSrc, nSrcPitch, pDst, nDstPitch, nWidth, nHeight, bIsNonTemporal);
					break;
				}
			} else {
				// Identical color space and bit depth for source and destination, this is the simplest case, just perform a surface copy
				NvcCopySurface(pSrc, nSrcPitch, pDst, nDstPitch, __NvcCalcLineSize(nWidth, eSrcColorSpace, eSrcBitDepth), nHeight);
			}
		} else {
			// Identical color space but different bit depth, perform bit depth conversion
			__NvcBitDepthConversion_ARGB(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, eSrcColorSpace, nWidth, nHeight, bComposite, bIsNonTemporal);
		}

		return;
	}

	switch (eSrcColorSpace) {
	case keNvcColorSpace_BGRA:
	case keNvcColorSpace_RGBA:
	case keNvcColorSpace_ARGB:
		switch (eDstColorSpace) {
		case keNvcColorSpace_BGRA:
		case keNvcColorSpace_RGBA:
		case keNvcColorSpace_ARGB:
			__NvcColorSpaceConversion_ARGB(pSrc, nSrcPitch, eSrcColorSpace, eSrcBitDepth, pDst, nDstPitch, eDstColorSpace, eDstBitDepth, nWidth, nHeight, bIsNonTemporal);
			break;

		case keNvcColorSpace_BGR:
		case keNvcColorSpace_RGB:
			__NvcColorSpaceConversion_ARGB_To_RGB(pSrc, nSrcPitch, eSrcColorSpace, eSrcBitDepth, pDst, nDstPitch, eDstColorSpace, eDstBitDepth, nWidth, nHeight, bComposite ? true : false);
			break;

		case keNvcColorSpace_Alpha:
			__NvcColorSpaceConversion_ARGB_To_A(pSrc, nSrcPitch, eSrcColorSpace, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, nWidth, nHeight);
			break;
		}
		break;

	case keNvcColorSpace_BGR:
	case keNvcColorSpace_RGB:
		switch (eDstColorSpace) {
		case keNvcColorSpace_BGRA:
		case keNvcColorSpace_RGBA:
		case keNvcColorSpace_ARGB:
			__NvcColorSpaceConversion_RGB_To_ARGB(pSrc, nSrcPitch, eSrcColorSpace, eSrcBitDepth, pDst, nDstPitch, eDstColorSpace, eDstBitDepth, nWidth, nHeight, fPaddingAlpha);
			break;

		case keNvcColorSpace_BGR:
		case keNvcColorSpace_RGB:
			__NvcColorSpaceConversion_RGB(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, nWidth, nHeight);
			break;

		case keNvcColorSpace_Alpha:
			{
				// In this case, just clear destination to white
				switch (eDstBitDepth) {
				case keNvcBitDepthInt8:
					NvcClear_8Bit_Matte(pDst, nDstPitch, nWidth, nHeight, 255, bIsNonTemporal);
					break;

				case keNvcBitDepthInt16:
					//NvcClear_16Bit_Matte(pDst, nDstPitch, nWidth, nHeight, 65535, bIsNonTemporal);
					break;

				case keNvcBitDepthFloat32:
					//NvcClear_32Bit_Matte(pDst, nDstPitch, nWidth, nHeight, 1, bIsNonTemporal);
					break;
				}
			}
			break;
		}
		break;

	case keNvcColorSpace_Alpha:
		switch (eDstColorSpace) {
		case keNvcColorSpace_BGRA:
		case keNvcColorSpace_RGBA:
		case keNvcColorSpace_ARGB:
			__NvcColorSpaceConversion_A_To_ARGB(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstColorSpace, eDstBitDepth, nWidth, nHeight);
			break;

		case keNvcColorSpace_BGR:
		case keNvcColorSpace_RGB:
			__NvcColorSpaceConversion_A_To_RGB(pSrc, nSrcPitch, eSrcBitDepth, pDst, nDstPitch, eDstBitDepth, nWidth, nHeight);
			break;
		}
		break;
	}
}
//
// Helper function to construct a mask line with the expected bit depth
//
static void __ConstructMaskLine_BGRA(const void *pSrcMaskLine,
	ENvcBitDepth eSrcMaskDepth,
	ENvcMaskType eSrcMaskType,
	ENvcColorChannel eMaskChannel,
	int nColorimetry,
	void *pDstMaskLine,
	ENvcBitDepth eDstMaskDepth,
	unsigned int nPixelCount)
{
	const unsigned int nSrcChannelBytes = NvcGetBytesFromBitDepth(eSrcMaskDepth);

	if (eSrcMaskType == keNvcMaskTypeAlpha && eSrcMaskDepth == eDstMaskDepth) {
		memcpy(pDstMaskLine, pSrcMaskLine, nPixelCount * nSrcChannelBytes);
		return;
	}

	void *pAlphaMaskLine = (void *)pSrcMaskLine;

	if (eSrcMaskType != keNvcMaskTypeAlpha) {
		// In this case we must convert this BGRA source mask to a alpha only mask
		if (eSrcMaskDepth != eDstMaskDepth) {
			void *pTmpBuffer = _alloca(nPixelCount * nSrcChannelBytes + 127);
			pTmpBuffer = (void *)NVC_UP_ALIGN_PO2((size_t)pTmpBuffer, 128);
			pAlphaMaskLine = pTmpBuffer;
		} else {
			pAlphaMaskLine = pDstMaskLine;
		}

		if (eMaskChannel != keNvcColorChannelLuma) {
			int nChannelIndex = 0;
			switch (eMaskChannel) {
			case keNvcColorChannelBlue:
				nChannelIndex = 0;
				break;
			case keNvcColorChannelGreen:
				nChannelIndex = 1;
				break;
			case keNvcColorChannelRed:
				nChannelIndex = 2;
				break;
			case keNvcColorChannelAlpha:
				nChannelIndex = 3;
				break;
			}

			switch (eSrcMaskDepth) {
			case keNvcBitDepthInt8:
				__ExtractChannelFromBGRALine<uint8_t>((const uint8_t *)pSrcMaskLine, (uint8_t *)pAlphaMaskLine, nChannelIndex, nPixelCount);
				break;

			case keNvcBitDepthInt16:
				//__ExtractChannelFromBGRALine<uint16_t>((const uint16_t *)pSrcMaskLine, (uint16_t *)pAlphaMaskLine, nChannelIndex, nPixelCount);
				break;

			case keNvcBitDepthFloat32:
				//__ExtractChannelFromBGRALine<float>((const float *)pSrcMaskLine, (float *)pAlphaMaskLine, nChannelIndex, nPixelCount);
				break;
			}
		} else {
			// Extract luma channel from this BGRA source mask line
			switch (eSrcMaskDepth) {
			case keNvcBitDepthInt8:
				__GenerateLumaLine8Bit((const uint8_t *)pSrcMaskLine, (uint8_t *)pAlphaMaskLine, nColorimetry, nPixelCount);
				break;

			case keNvcBitDepthInt16:
				//__GenerateLumaLine16Bit((const uint16_t *)pSrcMaskLine, (uint16_t *)pAlphaMaskLine, nColorimetry, nPixelCount);
				break;

			case keNvcBitDepthFloat32:
				//__GenerateLumaLine32Bit((const float *)pSrcMaskLine, (float *)pAlphaMaskLine, nColorimetry, nPixelCount);
				break;
			}
		}
	}

	if (eSrcMaskDepth != eDstMaskDepth) {
		NvcColorSpaceConversion_ARGB(
			pAlphaMaskLine,
			0,
			keNvcColorSpace_Alpha,
			eSrcMaskDepth,
			pDstMaskLine,
			0,
			keNvcColorSpace_Alpha,
			eDstMaskDepth,
			nPixelCount,
			1,
			0,
			0,
			0);
	}
}
////////////////////////88888888888888888888888888,__ConstructMaskLine_BGRA,end//////////////////////////////////////////////////////////////////////

static void __ConstructMaskLine(
	const SNvcBufferPlaneInfo *pSrcMaskLine,
	ENvcBitDepth eSrcMaskDepth,
	ENvcMaskType eSrcMaskType,
	ENvcColorChannel eMaskChannel,
	int nColorimetry,
	void *pDstMaskLine,
	ENvcBitDepth eDstMaskDepth,
	unsigned int nPixelCount)
{	
	if (eSrcMaskType == keNvcMaskTypeYUV422Planar || eSrcMaskType == keNvcMaskTypeYUVA422Planar)
		return __ConstructMaskLine_YUVA(pSrcMaskLine, eSrcMaskDepth, eSrcMaskType, eMaskChannel, nColorimetry, pDstMaskLine, eDstMaskDepth, nPixelCount);
	else
		return __ConstructMaskLine_BGRA(pSrcMaskLine->pDataPtr[0], eSrcMaskDepth, eSrcMaskType, eMaskChannel, nColorimetry, pDstMaskLine, eDstMaskDepth, nPixelCount);
}

// NOTE: both xmm_color_src and xmm_color_dst are modified after processing
#define __INTERPOLATE_COLOR_BY_MASK_8P_CORE_8BIT_INT(xmm_color_src, xmm_color_dst, xmm_factor1, xmm_factor2)	\
{																												\
	xmm_color_src = _mm_slli_epi16(xmm_color_src, 8);															\
	xmm_color_src = _mm_mulhi_epu16(xmm_color_src, xmm_factor1);	/* src_color * factor1     [p14b, w14b] */	\
	xmm_color_dst = _mm_slli_epi16(xmm_color_dst, 8);															\
	xmm_color_dst = _mm_mulhi_epu16(xmm_color_dst, xmm_factor2);	/* dst_color * factor2     [p14b, w14b] */	\
	xmm_color_dst = _mm_add_epi16(xmm_color_dst, xmm_color_src);	/* src_color * factor1 + dst_color * factor2 [p14b, w14b] */	\
	xmm_color_dst = _mm_add_epi16(xmm_color_dst, g_xmm_mask_rounding_8b);										\
	xmm_color_dst = _mm_srli_epi16(xmm_color_dst, 6);				/* [p8b, w8b] */							\
}

// NOTE: xmm_color_src, xmm_color_dst and xmm_mask are modified after processing
#define __INTERPOLATE_COLOR_BY_MASK_8P_8BIT_INT(xmm_color_src, xmm_color_dst, xmm_mask, xmm_intensity, invert)		\
{																													\
	xmm_mask = _mm_slli_epi16(xmm_mask, 8);																			\
	xmm_mask = _mm_mulhi_epu16(xmm_mask, g_xmm_scale_255_256);	/* mask / 255 * 256   [p15b, w16b] */				\
	\
	xmm_mask = _mm_mulhi_epu16(xmm_mask, xmm_intensity);		/* mask' = mask * intensity  [p14b, w15b] */		\
	if (invert)																										\
	xmm_mask = _mm_sub_epi16(g_xmm_1_shift_14, xmm_mask);	/* mask' = (1 - mask * intensity)  [p14b, w15b] */	\
	\
	__m128i __xmm_1_m_mask = _mm_sub_epi16(g_xmm_1_shift_14, xmm_mask);	/* factor = 1 - mask' [p14b, w15b] */		\
	\
	__INTERPOLATE_COLOR_BY_MASK_8P_CORE_8BIT_INT(xmm_color_src, xmm_color_dst, __xmm_1_m_mask, xmm_mask)			\
}

static uint8_t NVC_FORCE_INLINE __Intp_Color_By_Mask_8Bit(uint8_t matte_src,
	uint8_t matte_dst,
	uint8_t mask,
	uint32_t intensity,	/* p15b w16b */
	bool invert)
{
	uint32_t m, f;

	m = (((uint32_t)mask << 8) * __NVC_MASK_SCALE_255_256) >> 16;	/* mask / 255 * 256   [p15b, w16b] */

	m = (m * intensity) >> 16;	/* mask' = mask * intensity  [p14b, w15b] */
	if (invert)
		m = (1 << 14) - m;		/* mask' = (1 - mask * intensity)  [p14b, w15b] */

	f = (1 << 14) - m;			/* factor = 1 - mask' [p14b, w15b] */

	uint32_t tmp0, tmp1;

	tmp0 = ((matte_src << 8) * f) >> 16;	/* p14b w14b */
	tmp1 = ((matte_dst << 8) * m) >> 16;	/* p14b w14b */
	return (uint8_t)((tmp0 + tmp1 + 32) >> 6);
}

////////////////////////////////////////////////////////////////////////////////
void DoIntpMatteLineByMask8Bit(uint8_t *pSrcLine,
	uint8_t *pDstLine,
	const uint8_t *pMaskLine,
	unsigned int nCount,
	float fIntensity,
	bool bInvertMask)
{
	const unsigned int nIntensity = (unsigned int)(fIntensity * 32768 + 0.5);
	const __m128i xmm_intensity = _mm_set1_epi16((unsigned short)nIntensity);

	const unsigned int nGroupCount = nCount / 16;
	const unsigned int nTailCount = nCount & 15;

	uint8_t *pSrc = pSrcLine, *pDst = pDstLine;
	const uint8_t *pMask = pMaskLine;

	for (unsigned int i = 0; i < nGroupCount; i++, pSrc += 16, pDst += 16, pMask += 16) {
		// process 16 pixels each time
		__m128i xmm_mask, xmm_mask2;
		xmm_mask = _mm_load_si128((__m128i *)pMask);    // [m15|m14|m13|m12|m11|m10| m9| m8| m7| m6| m5| m4| m3| m2| m1| m0]
		if (_mm_movemask_epi8(_mm_cmpeq_epi8(xmm_mask, g_xmm_zero)) == 0xFFFF) {
			if (!bInvertMask)
				memcpy(pDst, pSrc, 16);
			continue;
		} else if (nIntensity == 32768 && _mm_movemask_epi8(_mm_cmpeq_epi8(xmm_mask, g_xmm_255_8b)) == 0xFFFF) {
			if (bInvertMask)
				memcpy(pDst, pSrc, 16);
			continue;
		}

		xmm_mask2 = _mm_unpackhi_epi8(xmm_mask, g_xmm_zero);
		xmm_mask = _mm_unpacklo_epi8(xmm_mask, g_xmm_zero);

		__m128i xmm_matte_src, xmm_matte_src2, xmm_matte_dst, xmm_matte_dst2;

		xmm_matte_src = _mm_load_si128((__m128i *)pSrc);
		xmm_matte_src2 = _mm_unpackhi_epi8(xmm_matte_src, g_xmm_zero);
		xmm_matte_src = _mm_unpacklo_epi8(xmm_matte_src, g_xmm_zero);

		xmm_matte_dst = _mm_load_si128((__m128i *)pDst);
		xmm_matte_dst2 = _mm_unpackhi_epi8(xmm_matte_dst, g_xmm_zero);
		xmm_matte_dst = _mm_unpacklo_epi8(xmm_matte_dst, g_xmm_zero);

		__INTERPOLATE_COLOR_BY_MASK_8P_8BIT_INT(xmm_matte_src, xmm_matte_dst, xmm_mask, xmm_intensity, bInvertMask);
		__INTERPOLATE_COLOR_BY_MASK_8P_8BIT_INT(xmm_matte_src2, xmm_matte_dst2, xmm_mask2, xmm_intensity, bInvertMask);

		xmm_matte_dst = _mm_packus_epi16(xmm_matte_dst, xmm_matte_dst2);
		_mm_store_si128((__m128i *)pDst, xmm_matte_dst);
	}

	for (unsigned int i = 0; i < nTailCount; i++)
		pDst[i] = __Intp_Color_By_Mask_8Bit(pSrc[i], pDst[i], pMask[i], nIntensity, bInvertMask);
}

// interpolate between source matte and destination matte using mask as factor
// the final result is stored in destination line
void __IntpMatteLineByMask(const void *pSrcLine,
	void *pDstLine,
	ENvcBitDepth eBitDepth,
	const SNvcMaskInfo *pMaskInfo,
	int nMaskLineIdx,
	int nPixelCount)
{
	void *pMaskLine = (char *)pMaskInfo->pMaskBits + pMaskInfo->nMaskPitch * nMaskLineIdx;
	if (pMaskInfo->eType != keNvcMaskTypeAlpha || pMaskInfo->eBitDepth != eBitDepth) {
		// Construct alpha only mask line with the specified depth
		void *pTmpBuffer = _alloca(nPixelCount * NvcGetBytesFromBitDepth(eBitDepth) + 127);
		pTmpBuffer = (void *)NVC_UP_ALIGN_PO2((size_t)pTmpBuffer, 128);
		const int nColorimetry = NvcGetColorimetry(pMaskInfo->eColorimetry);
		SNvcBufferPlaneInfo maskline;
		if (pMaskInfo->eType == keNvcMaskTypeYUV422Planar || pMaskInfo->eType == keNvcMaskTypeYUVA422Planar) {
			maskline.nPlanarCount = pMaskInfo->sPlaneInfo.nPlanarCount;
			for (int i = 0; i < maskline.nPlanarCount; i++) {
				maskline.pDataPtr[i] = (char *)pMaskInfo->sPlaneInfo.pDataPtr[i] + nMaskLineIdx * pMaskInfo->sPlaneInfo.nPitch[i];
				maskline.nPitch[i] = pMaskInfo->sPlaneInfo.nPitch[i];
			}
		}
		else {
			maskline.nPlanarCount = 1;
			maskline.pDataPtr[0] = pMaskLine;
			maskline.nPitch[0] = pMaskInfo->nMaskPitch;
		}
		__ConstructMaskLine(&maskline, pMaskInfo->eBitDepth, pMaskInfo->eType, pMaskInfo->eChannel, nColorimetry, pTmpBuffer, eBitDepth, nPixelCount);
		pMaskLine = pTmpBuffer;
	}

	switch (eBitDepth) {
	case keNvcBitDepthInt8:
		DoIntpMatteLineByMask8Bit((uint8_t *)pSrcLine, (uint8_t *)pDstLine, (uint8_t *)pMaskLine,
			nPixelCount, pMaskInfo->fIntensity, pMaskInfo->bInvert);
		break;

	case keNvcBitDepthInt16:
		//DoIntpMatteLineByMask16Bit((uint16_t *)pSrcLine, (uint16_t *)pDstLine, (uint16_t *)pMaskLine,
		//	nPixelCount, pMaskInfo->fIntensity, pMaskInfo->bInvert);
		break;

	case keNvcBitDepthFloat32:
		//DoIntpMatteLineByMask32Bit((float *)pSrcLine, (float *)pDstLine, (float *)pMaskLine,
		//	nPixelCount, pMaskInfo->fIntensity, pMaskInfo->bInvert);
		break;
	}
}


void DoIntpBGRALineByMask8Bit(uint8_t *pSrcLine,
	uint8_t *pDstLine,
	const uint8_t *pMaskLine,
	unsigned int nCount,
	float fIntensity,
	bool bInvertMask)
{
	unsigned int i, nTailCount, nIntensity = (unsigned int)(fIntensity * 32768 + 0.5);
	const __m128i xmm_intensity = _mm_set1_epi16((unsigned short)nIntensity);

	nTailCount = nCount & 3;
	nCount /= 4;

	uint8_t *pSrc = pSrcLine, *pDst = pDstLine;
	const uint8_t *pMask = pMaskLine;

	for (i = 0; i < nCount; i++, pSrc += 16, pDst += 16, pMask += 4) {
		// process 4 pixels each time
		__m128i xmm_color_src1, xmm_color_src2;
		__m128i xmm_color_dst1, xmm_color_dst2;
		__m128i xmm_mask1, xmm_mask2;

		const unsigned int nMask = *(unsigned int *)pMask;
		if (nMask == 0) {
			if (!bInvertMask)
				memcpy(pDst, pSrc, 16);
			continue;
		}

		if (nMask == 0xFFFFFFFF && nIntensity == 32768) {
			if (bInvertMask)
				memcpy(pDst, pSrc, 16);
			continue;
		}

		xmm_mask2 = _mm_cvtsi32_si128(nMask);		            // [ 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | m3| m2| m1| m0]
		xmm_mask2 = _mm_unpacklo_epi8(xmm_mask2, g_xmm_zero);	// [   0   |   0   |   0   |   0   |   m3  |   m2  |   m1  |   m0  ]

		xmm_mask2 = _mm_unpacklo_epi16(xmm_mask2, xmm_mask2);	// [   m3  |   m3  |   m2  |   m2  |   m1  |   m1  |   m0  |   m0  ]
		xmm_mask1 = _mm_shuffle_epi32(xmm_mask2, 0x50);			// [   m1  |   m1  |   m1  |   m1  |   m0  |   m0  |   m0  |   m0  ]
		xmm_mask2 = _mm_shuffle_epi32(xmm_mask2, 0xfa);			// [   m3  |   m3  |   m3  |   m3  |   m2  |   m2  |   m2  |   m2  ]

		xmm_color_src2 = _mm_load_si128((__m128i *)pSrc);				// [sa3|sr3|sg3|sb3|sa2|sr2|sg2|sb2|sa1|sr1|sg1|sb1|sa0|sr0|sg0|sb0]
		xmm_color_src1 = _mm_unpacklo_epi8(xmm_color_src2, g_xmm_zero);	// [  sa1  |  sr1  |  sg1  |  sb1  |  sa0  |  sr0  |  sg0  |  sb0  ]
		xmm_color_src2 = _mm_unpackhi_epi8(xmm_color_src2, g_xmm_zero);	// [  sa3  |  sr3  |  sg3  |  sb3  |  sa2  |  sr2  |  sg2  |  sb2  ]
		xmm_color_dst2 = _mm_load_si128((__m128i *)pDst);				// [da3|dr3|dg3|db3|da2|dr2|dg2|db2|da1|dr1|dg1|db1|da0|dr0|dg0|db0]
		xmm_color_dst1 = _mm_unpacklo_epi8(xmm_color_dst2, g_xmm_zero);	// [  da1  |  dr1  |  dg1  |  db1  |  da0  |  dr0  |  dg0  |  db0  ]
		xmm_color_dst2 = _mm_unpackhi_epi8(xmm_color_dst2, g_xmm_zero);	// [  da3  |  dr3  |  dg3  |  db3  |  da2  |  dr2  |  dg2  |  db2  ]

		__INTERPOLATE_COLOR_BY_MASK_8P_8BIT_INT(xmm_color_src1, xmm_color_dst1, xmm_mask1, xmm_intensity, bInvertMask);
		__INTERPOLATE_COLOR_BY_MASK_8P_8BIT_INT(xmm_color_src2, xmm_color_dst2, xmm_mask2, xmm_intensity, bInvertMask);

		xmm_color_dst1 = _mm_packus_epi16(xmm_color_dst1, xmm_color_dst2);	// [a'3|r'3|g'3|b'3|a'2|r'2|g'2|b'2|a'1|r'1|g'1|b'1|a'0|r'0|g'0|b'0]
		_mm_store_si128((__m128i *)pDst, xmm_color_dst1);
	}

	for (i = 0; i < nTailCount; i++) {
		pDst[i * 4 + 0] = __Intp_Color_By_Mask_8Bit(pSrc[i * 4 + 0], pDst[i * 4 + 0], pMask[i], nIntensity, bInvertMask);
		pDst[i * 4 + 1] = __Intp_Color_By_Mask_8Bit(pSrc[i * 4 + 1], pDst[i * 4 + 1], pMask[i], nIntensity, bInvertMask);
		pDst[i * 4 + 2] = __Intp_Color_By_Mask_8Bit(pSrc[i * 4 + 2], pDst[i * 4 + 2], pMask[i], nIntensity, bInvertMask);
		pDst[i * 4 + 3] = __Intp_Color_By_Mask_8Bit(pSrc[i * 4 + 3], pDst[i * 4 + 3], pMask[i], nIntensity, bInvertMask);
	}
}

// interpolate between source color and destination color using mask as factor
// all channels (B/G/R/A) are interpolated
// the final result is stored in destination line
void __IntpBGRALineByMask(const void *pSrcLine,
	void *pDstLine,
	ENvcBitDepth eBitDepth,
	const SNvcMaskInfo *pMaskInfo,
	int nMaskLineIdx,
	int nPixelCount)
{
	void *pMaskLine = (char *)pMaskInfo->pMaskBits + pMaskInfo->nMaskPitch * nMaskLineIdx;
	if (pMaskInfo->eType != keNvcMaskTypeAlpha || pMaskInfo->eBitDepth != eBitDepth) {
		// Construct alpha only mask line with the specified depth
		void *pTmpBuffer = _alloca(nPixelCount * NvcGetBytesFromBitDepth(eBitDepth) + 127);
		pTmpBuffer = (void *)NVC_UP_ALIGN_PO2((size_t)pTmpBuffer, 128);
		const int nColorimetry = NvcGetColorimetry(pMaskInfo->eColorimetry);
		SNvcBufferPlaneInfo maskline;
		if (pMaskInfo->eType == keNvcMaskTypeYUV422Planar || pMaskInfo->eType == keNvcMaskTypeYUVA422Planar) {
			maskline.nPlanarCount = pMaskInfo->sPlaneInfo.nPlanarCount;
			for (int i = 0; i < maskline.nPlanarCount; i++) {
				maskline.pDataPtr[i] = (char *)pMaskInfo->sPlaneInfo.pDataPtr[i] + nMaskLineIdx * pMaskInfo->sPlaneInfo.nPitch[i];
				maskline.nPitch[i] = pMaskInfo->sPlaneInfo.nPitch[i];
			}
		}
		else {
			maskline.nPlanarCount = 1;
			maskline.pDataPtr[0] = pMaskLine;
			maskline.nPitch[0] = pMaskInfo->nMaskPitch;
		}
		__ConstructMaskLine(&maskline, pMaskInfo->eBitDepth, pMaskInfo->eType, pMaskInfo->eChannel, nColorimetry, pTmpBuffer, eBitDepth, nPixelCount);
		pMaskLine = pTmpBuffer;
	}

	switch (eBitDepth) {
	case keNvcBitDepthInt8:
		DoIntpBGRALineByMask8Bit((uint8_t *)pSrcLine, (uint8_t *)pDstLine, (uint8_t *)pMaskLine,
			nPixelCount, pMaskInfo->fIntensity, pMaskInfo->bInvert);
		break;

	case keNvcBitDepthInt16:
		//DoIntpBGRALineByMask16Bit((uint16_t *)pSrcLine, (uint16_t *)pDstLine, (uint16_t *)pMaskLine,
		//	nPixelCount, pMaskInfo->fIntensity, pMaskInfo->bInvert);
		break;

	case keNvcBitDepthFloat32:
		//DoIntpBGRALineByMask32Bit((float *)pSrcLine, (float *)pDstLine, (float *)pMaskLine,
		//	nPixelCount, pMaskInfo->fIntensity, pMaskInfo->bInvert);
		break;
	}
}



void * _fast_memcpy(void *pDst, const void *pSrc, size_t nCount)
{
	const char *pSrcBuf;
	char *pDstBuf;
	size_t startpad, endpad;

	pSrcBuf = (const char *)pSrc;
	pDstBuf = (char *)pDst;

	startpad = (size_t)pDstBuf & 63;
	if (startpad) {
		startpad = 64 - startpad;

		if (nCount > startpad) {
			memcpy(pDstBuf, pSrcBuf, startpad);
			pSrcBuf += startpad;
			pDstBuf += startpad;
			nCount -= startpad;
		} else {
			memcpy(pDstBuf, pSrcBuf, nCount);
			return pDst;
		}
	}

	endpad = nCount & 63;
	nCount -= endpad;

	// now pDstBuf and nCount are 64 bytes aligned
	if (nCount) {
		size_t nGroupCount = nCount / 64;

		// prefetch some bytes
		_mm_prefetch(pSrcBuf, _MM_HINT_NTA);
		_mm_prefetch(pSrcBuf + 64, _MM_HINT_NTA);
		_mm_prefetch(pSrcBuf + 128, _MM_HINT_NTA);
		_mm_prefetch(pSrcBuf + 192, _MM_HINT_NTA);
		_mm_prefetch(pSrcBuf + 256, _MM_HINT_NTA);

		if (!((size_t)pSrcBuf & 15)) {
			// pSrcBuf is 16 bytes aligned
/*			__asm {
				mov			eax, [pSrcBuf]
				mov			edx, [pDstBuf]
				mov			ecx, [nGroupCount]

				// prefetch some bytes into the L1 cache
				// we assume the L1 cache is 64 bytes long
				prefetchnta [eax]
				prefetchnta [eax + 64]
				prefetchnta [eax + 128]
				prefetchnta [eax + 192]
				prefetchnta [eax + 256]

memory_copy_aligned:
				prefetchnta	[eax + 320]

				movaps		xmm0, [eax]
				movaps		xmm1, [eax + 16]
				movaps		xmm2, [eax + 32]
				movaps		xmm3, [eax + 48]

				movntps		[edx], xmm0
				movntps		[edx + 16], xmm1
				movntps		[edx + 32], xmm2
				movntps		[edx + 48], xmm3

				add			eax, 64
				add			edx, 64
				sub			ecx, 1
				jnz			memory_copy_aligned

				// since movntps is weakly-ordered, a "sfence" is needed to become ordered again.
				sfence

				mov			[pSrcBuf], eax
				mov			[pDstBuf], edx
			}
*/
			for (size_t i = 0; i < nGroupCount; i++) {
				_mm_prefetch(pSrcBuf + 320, _MM_HINT_NTA);

				__m128i xmm0, xmm1, xmm2, xmm3;
				xmm0 = _mm_load_si128((__m128i *)pSrcBuf);
				xmm1 = _mm_load_si128((__m128i *)pSrcBuf + 1);
				xmm2 = _mm_load_si128((__m128i *)pSrcBuf + 2);
				xmm3 = _mm_load_si128((__m128i *)pSrcBuf + 3);

				_mm_stream_si128((__m128i *)pDstBuf, xmm0);
				_mm_stream_si128((__m128i *)pDstBuf + 1, xmm1);
				_mm_stream_si128((__m128i *)pDstBuf + 2, xmm2);
				_mm_stream_si128((__m128i *)pDstBuf + 3, xmm3);

				pSrcBuf += 64;
				pDstBuf += 64;
			}
		} else {
			// pSrcBuf is NOT 16 bytes aligned
/*			__asm {
				mov			eax, [pSrcBuf]
				mov			edx, [pDstBuf]
				mov			ecx, [nGroupCount]

				// prefetch some bytes into the L1 cache
				// we assume the L1 cache is 64 bytes long
				prefetchnta [eax]
				prefetchnta [eax + 64]
				prefetchnta [eax + 128]
				prefetchnta [eax + 192]
				prefetchnta [eax + 256]

memory_copy:
				prefetchnta	[eax + 320]

				movups		xmm0, [eax]
				movups		xmm1, [eax + 16]
				movups		xmm2, [eax + 32]
				movups		xmm3, [eax + 48]

				movntps		[edx], xmm0
				movntps		[edx + 16], xmm1
				movntps		[edx + 32], xmm2
				movntps		[edx + 48], xmm3

				add			eax, 64
				add			edx, 64
				sub			ecx, 1
				jnz			memory_copy

				// since movntps is weakly-ordered, a "sfence" is needed to become ordered again.
				sfence

				mov			[pSrcBuf], eax
				mov			[pDstBuf], edx
			}
*/
			for (size_t i = 0; i < nGroupCount; i++) {
				_mm_prefetch(pSrcBuf + 320, _MM_HINT_NTA);

				__m128i xmm0, xmm1, xmm2, xmm3;
				xmm0 = _mm_loadu_si128((__m128i *)pSrcBuf);
				xmm1 = _mm_loadu_si128((__m128i *)pSrcBuf + 1);
				xmm2 = _mm_loadu_si128((__m128i *)pSrcBuf + 2);
				xmm3 = _mm_loadu_si128((__m128i *)pSrcBuf + 3);

				_mm_stream_si128((__m128i *)pDstBuf, xmm0);
				_mm_stream_si128((__m128i *)pDstBuf + 1, xmm1);
				_mm_stream_si128((__m128i *)pDstBuf + 2, xmm2);
				_mm_stream_si128((__m128i *)pDstBuf + 3, xmm3);

				pSrcBuf += 64;
				pDstBuf += 64;
			}
		}

		_mm_sfence();
	}

	if (endpad)
		memcpy(pDstBuf, pSrcBuf, endpad);

	return pDst;
}


// copy one surface to another surface using non-temporal hint
void _copy_surface(const void *pSrc,
	int nSrcPitch,
	void *pDst,
	int nDstPitch,
	unsigned int nLineSize,
	unsigned int nHeight)
{
	if (nSrcPitch == nDstPitch && (int)nLineSize == nSrcPitch) {
		_fast_memcpy(pDst, pSrc, nSrcPitch * nHeight);
		return;
	}

	const void *pSrcLine = pSrc;
	void *pDstLine = pDst;
	unsigned int row;

	for (row = 0; row < nHeight; row++) {
		_fast_memcpy(pDstLine, pSrcLine, nLineSize);
		pSrcLine = (char *)pSrcLine + nSrcPitch;
		pDstLine = (char *)pDstLine + nDstPitch;
	}
}


//// allocate memory block with the start address aligned with nAlign
//// you should make sure nAlign be power of 2!
//static inline void * _malloc_aligned(size_t nSize, int nAlign)
//{
//#ifdef _MSC_VER
//	return _aligned_malloc(nSize, nAlign);
//#else
//	return NULL;
//#endif
//}
//
//// free memory block allocated by malloc_aligned
//static inline void _free_aligned(void *pMem)
//{
//#ifdef _MSC_VER
//	_aligned_free(pMem);
//#else
//
//#endif
//}

void * _malloc_aligned(size_t size, size_t alignment)
{
#ifdef Q_OS_WIN
	return _aligned_malloc(size, alignment);
#elif defined(Q_OS_ANDROID)
	// We don't use posix_memalign() since it is missing in android NDK
	return memalign(alignment, size);
#else
	void *buf = 0;
	posix_memalign(&buf, alignment, size);
	return buf;
#endif
}

void _free_aligned(void *ptr)
{
#ifdef Q_OS_WIN
	_aligned_free(ptr);
#else
	free(ptr);
#endif
}

//void __NvcFFTGeneratePhaseFactors(
//	unsigned int,
//	float *,
//	float *)
//{
//
//}
//
//void __NvcRealFFT(
//	float *,
//	float *,
//	const float *,
//	const float *,
//	unsigned int)
//{
//
//}
//
//void __NvcRealIFFT(
//	float *,
//	float *,
//	const float *,
//	const float *,
//	unsigned int)
//{
//
//}
//
//void __NvcRealFFT_4C(
//	float *,
//	float *,
//	const float *,
//	const float *,
//	unsigned int)
//{
//
//}
//
//void __NvcRealIFFT_4C(
//	float *,
//	float *,
//	const float *,
//	const float *,
//	unsigned int)
//{
//
//}

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
// implementation of class __CNvcBufferManager
//////////////////////////////////////////////////////////////////////////
#define __NVC_CACHELINE_SIZE	128

// @nInitSize: initial size of this buffer
// @nIncreaseSize: the granularity we increase buffer size
__CNvcBufferManager::__CNvcBufferManager(size_t nInitSize,
	size_t nIncreaseSize,
	bool *pbSucceeded)
{
	m_pBuffer = NULL;
	m_nTotalBufferSize = 0;
	m_nIncreaseSize = 0;
	m_nCommittedBufferSizePerThread = 0;
	m_nCommittedThreadCount = 0;

	assert(pbSucceeded);
	if (!(*pbSucceeded))
		return;

	m_nIncreaseSize = nIncreaseSize;
	if (!m_nIncreaseSize) {
		assert(false);
		m_nIncreaseSize = 32 * 1024;
	}

	const size_t nAllocationSize = nInitSize;
	if (nAllocationSize) {
		m_pBuffer = _malloc_aligned(nAllocationSize, __NVC_CACHELINE_SIZE);
		if (!m_pBuffer) {
			assert(false);
			*pbSucceeded = false;
			return;
		}

		m_nTotalBufferSize = nAllocationSize;
	}
}

__CNvcBufferManager::~__CNvcBufferManager()
{
	if (m_pBuffer)
		_free_aligned(m_pBuffer);
}

// commit memory for buffer (in byte unit)
bool __CNvcBufferManager::CommitMemory(size_t nBufferSizePerThread,
	unsigned int nThreadCount)
{
	if (!nThreadCount) {
		assert(false);
		return false;
	}

	//
	// calculate how many bytes we should commit
	//
	size_t nActualSizePerThread, nTotalSize;

	nActualSizePerThread = NVC_UP_ALIGN_PO2(nBufferSizePerThread, __NVC_CACHELINE_SIZE);
	nTotalSize = nActualSizePerThread * nThreadCount;

	if (nTotalSize <= m_nTotalBufferSize) {
		m_nCommittedBufferSizePerThread = nActualSizePerThread;
		m_nCommittedThreadCount = nThreadCount;
		return true;
	}

	size_t nIncreaseSize = m_nIncreaseSize/* * nThreadCount*/;
	size_t nNewTotalBufferSize = NVC_UP_ALIGN(nTotalSize, nIncreaseSize);

	// release the old buffer if necessary
	if (m_pBuffer) {
		_free_aligned(m_pBuffer);
		m_pBuffer = NULL;
		m_nTotalBufferSize = 0;
		m_nCommittedBufferSizePerThread = 0;
		m_nCommittedThreadCount = 0;
	}

	m_pBuffer = _malloc_aligned(nNewTotalBufferSize, __NVC_CACHELINE_SIZE);
	if (!m_pBuffer) {
		assert(false);
		return false;
	}

	m_nTotalBufferSize = nNewTotalBufferSize;
	m_nCommittedBufferSizePerThread = nActualSizePerThread;
	m_nCommittedThreadCount = nThreadCount;
	return true;
}

