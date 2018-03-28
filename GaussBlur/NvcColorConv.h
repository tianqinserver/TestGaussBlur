#ifndef __NVC_COLOR_CONV_H__
#define	__NVC_COLOR_CONV_H__

#include <NvcColorConvDef.h>
#include <NvcColorConvYUVPlanar.h>
#include "NvcColorFunc.h"
//#include "NvcMemoryFunc.h"

#ifdef __cplusplus
extern "C" {
#endif


// NOTE: in-place process is possible
void NVCAPI NvcPremultiply_8Bit_BGRA(const void *pSrc,
									 int nSrcPitch,
									 void *pDst,
									 int nDstPitch,
									 unsigned int nWidth,
									 unsigned int nHeight,
									 int bIsNonTemporal);

// NOTE: in-place process is possible
void NVCAPI NvcPremultiply_16Bit_BGRA(const void *pSrc,
									  int nSrcPitch,
									  void *pDst,
									  int nDstPitch,
									  unsigned int nWidth,
									  unsigned int nHeight,
									  int bIsNonTemporal);

// NOTE: in-place process is possible
void NVCAPI NvcPremultiply_32Bit_BGRA(const void *pSrc,
									  int nSrcPitch,
									  void *pDst,
									  int nDstPitch,
									  unsigned int nWidth,
									  unsigned int nHeight,
									  int bIsNonTemporal);

// NOTE: in-place process is possible
void NVCAPI NvcPremultiply_8Bit_YUVA422_Planar(	const void *pSrcYUVA[4],
												int nSrcPitch[4],
												void *pDstYUVA[4],
												int nDstPitch[4],
												unsigned int nWidth,
												unsigned int nHeight,
												int bIsNonTemporal);

// NOTE: in-place process is possible
void NVCAPI NvcPremultiply_16Bit_YUVA422_Planar(const void *pSrcYUVA[4],
												int nSrcPitch[4],
												void *pDstYUVA[4],
												int nDstPitch[4],
												unsigned int nWidth,
												unsigned int nHeight,
												int bIsNonTemporal);

// NOTE: in-place process is possible
void NVCAPI NvcUnpremultiply_8Bit_BGRA(const void *pSrc,
                                       int nSrcPitch,
                                       void *pDst,
                                       int nDstPitch,
                                       unsigned int nWidth,
                                       unsigned int nHeight,
                                       int bIsNonTemporal);

// NOTE: in-place process is possible
void NVCAPI NvcUnpremultiply_16Bit_BGRA(const void *pSrc,
                                        int nSrcPitch,
                                        void *pDst,
                                        int nDstPitch,
                                        unsigned int nWidth,
                                        unsigned int nHeight,
                                        int bIsNonTemporal);

// NOTE: in-place process is possible
void NVCAPI NvcUnpremultiply_32Bit_BGRA(const void *pSrc,
                                        int nSrcPitch,
                                        void *pDst,
                                        int nDstPitch,
                                        unsigned int nWidth,
                                        unsigned int nHeight,
                                        int bIsNonTemporal);


//////////////////////////////////////////////////////////////////////////
// BGRA <--> 8Bit YUV conversion
//////////////////////////////////////////////////////////////////////////
void NVCAPI Nvc_8Bit_BGRA_To_8Bit_YUY2(const void *pSrc,
									   int nSrcPitch,
									   void *pDst,
									   int nDstPitch,
									   unsigned int nWidth,
									   unsigned int nHeight,
									   int bPremultiply,
									   ENvcColorimetry eColorimetry,
									   int bIsNonTemporal);

void NVCAPI Nvc_16Bit_BGRA_To_8Bit_YUY2(const void *pSrc,
										int nSrcPitch,
										void *pDst,
										int nDstPitch,
										unsigned int nWidth,
										unsigned int nHeight,
										int bPremultiply,
										ENvcColorimetry eColorimetry,
										int bIsNonTemporal);

void NVCAPI Nvc_32Bit_BGRA_To_8Bit_YUY2(const void *pSrc,
										int nSrcPitch,
										void *pDst,
										int nDstPitch,
										unsigned int nWidth,
										unsigned int nHeight,
										int bPremultiply,
										ENvcColorimetry eColorimetry,
										int bIsNonTemporal);

void NVCAPI Nvc_8Bit_BGRA_To_8Bit_UYVY(const void *pSrc,
                                          int nSrcPitch,
                                          void *pDst,
                                          int nDstPitch,
                                          unsigned int nWidth,
                                          unsigned int nHeight,
                                          int bPremultiply,
                                          ENvcColorimetry eColorimetry,
                                          int bIsNonTemporal);

void NVCAPI Nvc_16Bit_BGRA_To_8Bit_UYVY(const void *pSrc,
                                           int nSrcPitch,
                                           void *pDst,
                                           int nDstPitch,
                                           unsigned int nWidth,
                                           unsigned int nHeight,
                                           int bPremultiply,
                                           ENvcColorimetry eColorimetry,
                                           int bIsNonTemporal);

void NVCAPI Nvc_32Bit_BGRA_To_8Bit_UYVY(const void *pSrc,
                                           int nSrcPitch,
                                           void *pDst,
                                           int nDstPitch,
                                           unsigned int nWidth,
                                           unsigned int nHeight,
                                           int bPremultiply,
                                           ENvcColorimetry eColorimetry,
                                           int bIsNonTemporal);

void NVCAPI Nvc_32Bit_BGRA_To_8Bit_YUV422P(const void *pSrc,
											int nSrcPitch,
											void *pDstY,
											void *pDstU,
											void *pDstV,
											int nDstPitch[],
											unsigned int nWidth,
											unsigned int nHeight,
											int bPremultiply,
											ENvcColorimetry eColorimetry,
											int bIsNonTemporal);

void NVCAPI Nvc_8Bit_BGRA_To_8Bit_YUAYVA4224(const void *pSrc,
                                           int nSrcPitch,
                                           void *pDst,
                                           int nDstPitch,
                                           unsigned int nWidth,
                                           unsigned int nHeight,
                                           ENvcColorimetry eColorimetry,
                                           int bIsNonTemporal);

void NVCAPI Nvc_16Bit_BGRA_To_8Bit_YUAYVA4224(const void *pSrc,
                                            int nSrcPitch,
                                            void *pDst,
                                            int nDstPitch,
                                            unsigned int nWidth,
                                            unsigned int nHeight,
                                            ENvcColorimetry eColorimetry,
                                            int bIsNonTemporal);

void NVCAPI Nvc_16Bit_BGRA_To_8Bit_YUV422_Plannar(const void *pSrc,
	                                              int nSrcPitch,
	                                              void *ppDst[3],
	                                              int nDstPitch[3],
	                                              unsigned int nWidth,
	                                              unsigned int nHeight,
	                                              int bPremultiply,
	                                              ENvcColorimetry eColorimetry,
	                                              int bIsNonTemporal);
void NVCAPI Nvc_16Bit_BGR_To_8Bit_YUV422_Plannar(const void *pSrc,
												int nSrcPitch,
												void *ppDst[3],
												int nDstPitch[3],
												unsigned int nWidth,
												unsigned int nHeight,
												int bPremultiply,
												ENvcColorimetry eColorimetry,
												int bIsNonTemporal);

void NVCAPI Nvc_16Bit_RGB_To_8Bit_YUV422_Plannar(const void *pSrc,
												int nSrcPitch,
												void *ppDst[3],
												int nDstPitch[3],
												unsigned int nWidth,
												unsigned int nHeight,
												int bPremultiply,
												ENvcColorimetry eColorimetry,
												int bIsNonTemporal);

void NVCAPI Nvc_16Bit_BGRA_To_8Bit_YUVA4224_Plannar(const void *pSrc,
	                                                int nSrcPitch,
	                                                void *ppDst[4],
	                                                int nDstPitch[4],
	                                                unsigned int nWidth,
	                                                unsigned int nHeight,
	                                                int bPremultiply,
	                                                ENvcColorimetry eColorimetry,
	                                                int bIsNonTemporal);

void NVCAPI Nvc_16Bit_ARGB_To_8Bit_YUVA4224_Plannar(const void *pSrc,
													int nSrcPitch,
													void *ppDst[4],
													int nDstPitch[4],
													unsigned int nWidth,
													unsigned int nHeight,
													int bPremultiply,
													ENvcColorimetry eColorimetry,
													int bIsNonTemporal);

void NVCAPI Nvc_16Bit_RGBA_To_8Bit_YUVA4224_Plannar(const void *pSrc,
	                                                int nSrcPitch,
	                                                void *ppDst[4],
	                                                int nDstPitch[4],
	                                                unsigned int nWidth,
	                                                unsigned int nHeight,
	                                                int bPremultiply,
	                                                ENvcColorimetry eColorimetry,
	                                                int bIsNonTemporal);

void NVCAPI Nvc_32Bit_BGRA_To_8Bit_YUAYVA4224(const void *pSrc,
                                            int nSrcPitch,
                                            void *pDst,
                                            int nDstPitch,
                                            unsigned int nWidth,
                                            unsigned int nHeight,
                                            ENvcColorimetry eColorimetry,
                                            int bIsNonTemporal);

void NVCAPI Nvc_8Bit_YUY2_To_8Bit_BGRA(const void *pSrc,
                                       int nSrcPitch,
                                       void *pDst,
                                       int nDstPitch,
                                       unsigned int nWidth,
                                       unsigned int nHeight,
                                       unsigned int nAlpha,
                                       ENvcColorimetry eColorimetry,
                                       int bIsNonTemporal);

void NVCAPI Nvc_8Bit_YUY2_To_16Bit_BGRA(const void *pSrc,
                                        int nSrcPitch,
                                        void *pDst,
                                        int nDstPitch,
                                        unsigned int nWidth,
                                        unsigned int nHeight,
                                        unsigned int nAlpha,
                                        ENvcColorimetry eColorimetry,
                                        int bIsNonTemporal);

void NVCAPI Nvc_8Bit_YUY2_To_32Bit_BGRA(const void *pSrc,
                                        int nSrcPitch,
                                        void *pDst,
                                        int nDstPitch,
                                        unsigned int nWidth,
                                        unsigned int nHeight,
                                        float fAlpha,
                                        ENvcColorimetry eColorimetry,
                                        int bIsNonTemporal);

void NVCAPI Nvc_8Bit_YUV420_Planar_To_8Bit_BGRA(const void *pSrcY,
												const void *pSrcU,
												const void *pSrcV,
												int nSrcPitch[4],
												void *pDst,
												int nDstPitch,
												unsigned int nWidth,
												unsigned int nHeight,
												unsigned int nAlpha,
												ENvcColorimetry eColorimetry,
												int bIsNonTemporal,
												int bIsTopSingleLine);

void NVCAPI Nvc_8Bit_UYVY_To_8Bit_BGRA(const void *pSrc,
                                          int nSrcPitch,
                                          void *pDst,
                                          int nDstPitch,
                                          unsigned int nWidth,
                                          unsigned int nHeight,
                                          unsigned int nAlpha,
                                          ENvcColorimetry eColorimetry,
                                          int bIsNonTemporal);

void NVCAPI Nvc_8Bit_UYVY_To_16Bit_BGRA(const void *pSrc,
                                           int nSrcPitch,
                                           void *pDst,
                                           int nDstPitch,
                                           unsigned int nWidth,
                                           unsigned int nHeight,
                                           unsigned int nAlpha,
                                           ENvcColorimetry eColorimetry,
                                           int bIsNonTemporal);

void NVCAPI Nvc_8Bit_UYVY_To_32Bit_BGRA(const void *pSrc,
                                           int nSrcPitch,
                                           void *pDst,
                                           int nDstPitch,
                                           unsigned int nWidth,
                                           unsigned int nHeight,
                                           float fAlpha,
                                           ENvcColorimetry eColorimetry,
                                           int bIsNonTemporal);

void NVCAPI Nvc_8Bit_YUVA4224_To_8Bit_BGRA(const void *pSrc,
                                           int nSrcPitch,
                                           void *pDst,
                                           int nDstPitch,
                                           unsigned int nWidth,
                                           unsigned int nHeight,
                                           ENvcColorimetry eColorimetry,
                                           int bIsNonTemporal);

void NVCAPI Nvc_8Bit_YUVA4224_To_16Bit_BGRA(const void *pSrc,
                                            int nSrcPitch,
                                            void *pDst,
                                            int nDstPitch,
                                            unsigned int nWidth,
                                            unsigned int nHeight,
                                            ENvcColorimetry eColorimetry,
                                            int bIsNonTemporal);

void NVCAPI Nvc_8Bit_YUVA4224_To_32Bit_BGRA(const void *pSrc,
                                            int nSrcPitch,
                                            void *pDst,
                                            int nDstPitch,
                                            unsigned int nWidth,
                                            unsigned int nHeight,
                                            ENvcColorimetry eColorimetry,
                                            int bIsNonTemporal);

void NVCAPI Nvc_8Bit_YUYA422_Planar_To_8Bit_BGRA(
	                                        const void *pSrc[4],
	                                        int nSrcPitch[4],
	                                        void *pDst,
	                                        int nDstPitch,
	                                        unsigned int nWidth,
	                                        unsigned int nHeight,
	                                        unsigned int nAlpha,
	                                        ENvcColorimetry eColorimetry,
	                                        int bIsNonTemporal);

void NVCAPI Nvc_8Bit_YUV422_Planar_To_8Bit_BGRA(
	                                        const void *pSrc[4],
	                                        int nSrcPitch[4],
	                                        void *pDst,
	                                        int nDstPitch,
	                                        unsigned int nWidth,
	                                        unsigned int nHeight,
	                                        unsigned int nAlpha,
	                                        ENvcColorimetry eColorimetry,
	                                        int bIsNonTemporal);

//////////////////////////////////////////////////////////////////////////
// BGRA <--> 10Bit YUV conversion
//////////////////////////////////////////////////////////////////////////
void NVCAPI Nvc_8Bit_BGRA_To_10Bit_YUY2(const void *pSrc,
										int nSrcPitch, 
										void *pDst,
										int nDstPitch, 
										unsigned int nWidth,
										unsigned int nHeight,
										int bPremultiply,
										ENvcColorimetry eColorimetry, 
										int bIsNonTemporal);

void NVCAPI Nvc_10Bit_YUY2_To_8Bit_BGRA(const void *pSrc,
										int nSrcPitch,
										void *pDst,
										int nDstPitch,
										unsigned int nWidth,
										unsigned int nHeight, 
										unsigned int nAlpha,
										ENvcColorimetry eColorimetry,
										int bIsNonTemporal);

void NVCAPI Nvc_10Bit_YUY2_Planar_To_8Bit_BGRA(const void *pSrcY,
											   const void *pSrcU,
											   const void *pSrcV,
											   int nSrcPitch[4],
											   void *pDst,
											   int nDstPitch,
											   unsigned int nWidth,
											   unsigned int nHeight, 
											   unsigned int nAlpha,
											   ENvcColorimetry eColorimetry,
											   int bIsNonTemporal);


void NVCAPI Nvc_10Bit_YUY2_Planar_To_16Bit_BGRA(const void *pSrcY,
												const void *pSrcU,
												const void *pSrcV,
												int nSrcPitch[4],
												void *pDst,
												int nDstPitch,
												unsigned int nWidth,
												unsigned int nHeight, 
												unsigned int nAlpha,
												ENvcColorimetry eColorimetry,
												int bIsNonTemporal);

void NVCAPI Nvc_10Bit_YUY2_Planar_To_32Bit_BGRA(const void *pSrcY,
												const void *pSrcU,
												const void *pSrcV,
												int nSrcPitch[4],
												void *pDst,
												int nDstPitch,
												unsigned int nWidth,
												unsigned int nHeight, 
												float fAlpha,
												ENvcColorimetry eColorimetry,
												int bIsNonTemporal);

void NVCAPI Nvc_10Bit_YUVA422_Planar_To_8Bit_BGRA(const void *pSrcY,
												  const void *pSrcU,
												  const void *pSrcV,
												  const void *pSrcA,
												  int nSrcPitch[4],
												  void *pDst,
												  int nDstPitch,
												  unsigned int nWidth,
												  unsigned int nHeight, 
												  ENvcColorimetry eColorimetry,
												  int bIsNonTemporal);

void NVCAPI Nvc_10Bit_YUVA422_Planar_To_16Bit_BGRA(const void *pSrcY,
												   const void *pSrcU,
												   const void *pSrcV,
												   const void *pSrcA,
												   int nSrcPitch[4],
												   void *pDst,
												   int nDstPitch,
												   unsigned int nWidth,
												   unsigned int nHeight, 
												   ENvcColorimetry eColorimetry,
												   int bIsNonTemporal);

void NVCAPI Nvc_10Bit_YUVA422_Planar_To_32Bit_BGRA(	const void *pSrcY,
													const void *pSrcU,
													const void *pSrcV,
													const void *pSrcA,
													int nSrcPitch[4],
													void *pDst,
													int nDstPitch,
													unsigned int nWidth,
													unsigned int nHeight, 
													ENvcColorimetry eColorimetry,
													int bIsNonTemporal);


void NVCAPI Nvc_10Bit_YUV444_Planar_To_8Bit_BGRA(const void *pSrcY,
												 const void *pSrcU,
												 const void *pSrcV,
												 int nSrcPitch[4],
												 void *pDst,
												 int nDstPitch,
												 unsigned int nWidth,
												 unsigned int nHeight, 
												 unsigned int nAlpha,
												 ENvcColorimetry eColorimetry,
												 int bIsNonTemporal);

void NVCAPI Nvc_10Bit_YUV444_Planar_To_16Bit_BGRA(const void *pSrcY,
												  const void *pSrcU,
												  const void *pSrcV,
												  int nSrcPitch[4],
												  void *pDst,
												  int nDstPitch,
												  unsigned int nWidth,
												  unsigned int nHeight, 
												  unsigned int nAlpha,
												  ENvcColorimetry eColorimetry,
												  int bIsNonTemporal);

void NVCAPI Nvc_10Bit_YUV444_Planar_To_32Bit_BGRA(const void *pSrcY,
												  const void *pSrcU,
												  const void *pSrcV,
												  int nSrcPitch[4],
												  void *pDst,
												  int nDstPitch,
												  unsigned int nWidth,
												  unsigned int nHeight, 
												  float fAlpha,
												  ENvcColorimetry eColorimetry,
												  int bIsNonTemporal);

void NVCAPI Nvc_10Bit_YUV420_Planar_To_8Bit_BGRA(const void *pSrcY,
												 const void *pSrcU,
												 const void *pSrcV,
												 int nSrcPitch[4],
												 void *pDst,
												 int nDstPitch,
												 unsigned int nWidth,
												 unsigned int nHeight, 
												 unsigned int nAlpha,
												 ENvcColorimetry eColorimetry,
												 int bIsNonTemporal,
												 int bIsTopSingleLine);

void NVCAPI Nvc_10Bit_YUV420_Planar_To_16Bit_BGRA(const void *pSrcY,
													const void *pSrcU,
													const void *pSrcV,
													int nSrcPitch[4],
													void *pDst,
													int nDstPitch,
													unsigned int nWidth,
													unsigned int nHeight,
													unsigned int nAlpha,
													ENvcColorimetry eColorimetry,
													int bIsNonTemporal,
													int bIsTopSingleLine);

void NVCAPI Nvc_10Bit_YUV420_Planar_To_32Bit_BGRA(const void *pSrcY,
												  const void *pSrcU,
												  const void *pSrcV,
												  int nSrcPitch[4],
												  void *pDst,
												  int nDstPitch,
												  unsigned int nWidth,
												  unsigned int nHeight, 
												  float fAlpha,
												  ENvcColorimetry eColorimetry,
												  int bIsNonTemporal);

void NVCAPI Nvc_8Bit_BGR_To_10Bit_YUV422_Planar(const void *pSrc,
													int nSrcPitch,
													void *pDstY,
													void *pDstU,
													void *pDstV,
													int nDstPitch[3],
													unsigned int nWidth,
													unsigned int nHeight,
													int bPremultiply,
													ENvcColorimetry eColorimetry);

void NVCAPI Nvc_8Bit_RGB_To_10Bit_YUV422_Planar(const void *pSrc,
												int nSrcPitch,
												void *pDstY,
												void *pDstU,
												void *pDstV,
												int nDstPitch[3],
												unsigned int nWidth,
												unsigned int nHeight,
												int bPremultiply,
												ENvcColorimetry eColorimetry);

void NVCAPI Nvc_8Bit_BGRA_To_10Bit_YUY2_Planar(const void *pSrc,
											   int nSrcPitch,
											   void *pDstY,
											   void *pDstU,
											   void *pDstV,
											   int nDstPitch[4],
											   unsigned int nWidth,
											   unsigned int nHeight,
											   int bPremultiply,
											   ENvcColorimetry eColorimetry);

void NVCAPI Nvc_8Bit_BGRA_To_10Bit_YUVA4224_Planar(const void *pSrc,
	                                           int nSrcPitch,
	                                           void *pDstY,
	                                           void *pDstU,
	                                           void *pDstV,
	                                           void *pDstA,
	                                           int nDstPitch[4],
	                                           unsigned int nWidth,
	                                           unsigned int nHeight,
	                                           int bPremultiply,
	                                           ENvcColorimetry eColorimetry);

void NVCAPI Nvc_8Bit_ARGB_To_10Bit_YUVA4224_Planar(const void *pSrc,
												int nSrcPitch,
												void *pDstY,
												void *pDstU,
												void *pDstV,
												void *pDstA,
												int nDstPitch[4],
												unsigned int nWidth,
												unsigned int nHeight,
												int bPremultiply,
												ENvcColorimetry eColorimetry);

void NVCAPI Nvc_8Bit_RGBA_To_10Bit_YUVA4224_Planar(const void *pSrc,
	                                          int nSrcPitch,
	                                          void *pDstY,
	                                          void *pDstU,
	                                          void *pDstV,
	                                          void *pDstA,
	                                          int nDstPitch[4],
	                                          unsigned int nWidth,
	                                          unsigned int nHeight,
	                                          int bPremultiply,
	                                          ENvcColorimetry eColorimetry);

void NVCAPI Nvc_16Bit_BGRA_To_10Bit_YUY2_Planar(const void *pSrc,
												int nSrcPitch,
												void *pDstY,
												void *pDstU,
												void *pDstV,
												int nDstPitch[4],
												unsigned int nWidth,
												unsigned int nHeight,
												int bPremultiply,
												ENvcColorimetry eColorimetry);

void NVCAPI Nvc_16Bit_BGR_To_10Bit_YUV422_Planar(const void *pSrc,
												int nSrcPitch,
												void *pDstY,
												void *pDstU,
												void *pDstV,
												int nDstPitch[3],
												unsigned int nWidth,
												unsigned int nHeight,
												int bPremultiply,
												ENvcColorimetry eColorimetry);

void NVCAPI Nvc_16Bit_RGB_To_10Bit_YUV422_Planar(const void *pSrc,
												int nSrcPitch,
												void *pDstY,
												void *pDstU,
												void *pDstV,
												int nDstPitch[3],
												unsigned int nWidth,
												unsigned int nHeight,
												int bPremultiply,
												ENvcColorimetry eColorimetry);

void NVCAPI Nvc_16Bit_BGRA_To_10Bit_YUVA4224_Planar(const void *pSrc,
	                                                int nSrcPitch,
	                                                void *pDstY,
	                                                void *pDstU,
	                                                void *pDstV,
	                                                void *pDstA,
	                                                int nDstPitch[4],
	                                                unsigned int nWidth,
	                                                unsigned int nHeight,
	                                                int bPremultiply,
	                                                ENvcColorimetry eColorimetry);

void NVCAPI Nvc_32Bit_BGRA_To_10Bit_YUVA4224_Planar(const void *pSrc,
	                                               int nSrcPitch,
	                                               void *pDstY,
	                                               void *pDstU,
	                                               void *pDstV,
	                                               void *pDstA,
	                                               int nDstPitch[4],
	                                               unsigned int nWidth,
	                                               unsigned int nHeight,
	                                               int bPremultiply,
	                                               ENvcColorimetry eColorimetry);

void NVCAPI Nvc_16Bit_ARGB_To_10Bit_YUVA4224_Planar(const void *pSrc,
													int nSrcPitch,
													void *pDstY,
													void *pDstU,
													void *pDstV,
													void *pDstA,
													int nDstPitch[4],
													unsigned int nWidth,
													unsigned int nHeight,
													int bPremultiply,
													ENvcColorimetry eColorimetry);

void NVCAPI Nvc_16Bit_RGBA_To_10Bit_YUVA4224_Planar(const void *pSrc,
	                                                int nSrcPitch,
	                                                void *pDstY,
	                                                void *pDstU,
	                                                void *pDstV,
	                                                void *pDstA,
	                                                int nDstPitch[4],
	                                                unsigned int nWidth,
	                                                unsigned int nHeight,
	                                                int bPremultiply,
	                                                ENvcColorimetry eColorimetry);

void NVCAPI Nvc_8Bit_BGRA_To_10Bit_YUV420_Planar(const void *pSrc,
												 int nSrcPitch,
												 void *pDstY,
												 void *pDstU,
												 void *pDstV,
												 int nDstPitch[4],
												 unsigned int nWidth,
												 unsigned int nHeight,
												 int bPremultiply,
												 ENvcColorimetry eColorimetry);

void NVCAPI Nvc_8Bit_BGRA_To_10Bit_YUAYVA4224(const void *pSrc,
											  int nSrcPitch, 
											  void *pDst,
											  int nDstPitch, 
											  unsigned int nWidth,
											  unsigned int nHeight,
											  int bPremultiply,
											  ENvcColorimetry eColorimetry, 
											  int bIsNonTemporal);

void NVCAPI Nvc_10Bit_YUAYVA4224_To_8Bit_BGRA(const void *pSrc,
											  int nSrcPitch,
											  void *pDst,
											  int nDstPitch,
											  unsigned int nWidth,
											  unsigned int nHeight, 
											  ENvcColorimetry eColorimetry,
											  int bIsNonTemporal);

void NVCAPI Nvc_16Bit_BGRA_To_10Bit_YUY2(const void *pSrc,
										 int nSrcPitch, 
										 void *pDst,
										 int nDstPitch,
										 unsigned int nWidth,
										 unsigned int nHeight,
										 int bPremultiply,
										 ENvcColorimetry eColorimetry,
										 int bIsNonTemporal);

void NVCAPI Nvc_10Bit_YUY2_To_16Bit_BGRA(const void *pSrc, 
									     int nSrcPitch,
										 void *pDst, 
										 int nDstPitch, 
										 unsigned int nWidth,
										 unsigned int nHeight, 
										 unsigned int nAlpha, 
										 ENvcColorimetry eColorimetry, 
										 int bIsNonTemporal);

void NVCAPI Nvc_16Bit_BGRA_To_10Bit_YUAYVA4224(const void *pSrc,
											   int nSrcPitch, 
											   void *pDst,
											   int nDstPitch, 
											   unsigned int nWidth,
											   unsigned int nHeight,
											   int bPremultiply,
											   ENvcColorimetry eColorimetry, 
											   int bIsNonTemporal);


void NVCAPI Nvc_10Bit_YUAYVA4224_To_16Bit_BGRA(const void *pSrc,
											   int nSrcPitch,
											   void *pDst,
											   int nDstPitch,
											   unsigned int nWidth,
											   unsigned int nHeight, 
											   ENvcColorimetry eColorimetry,
											   int bIsNonTemporal);

void NVCAPI Nvc_32Bit_BGRA_To_10Bit_YUY2(const void *pSrc,
										int nSrcPitch, 
										void *pDst,
										int nDstPitch,
										unsigned int nWidth,
										unsigned int nHeight,
										int bPremultiply,
										ENvcColorimetry eColorimetry,
										int bIsNonTemporal);

void NVCAPI Nvc_32Bit_BGRA_To_10Bit_YUV422P(const void *pSrc,
	                                      int nSrcPitch,
	                                      void *pDstY,
	                                      void *pDstU,
	                                      void *pDstV,
	                                      int nDstPitch[3],
	                                      unsigned int nWidth,
	                                      unsigned int nHeight,
	                                      int bPremultiply,
	                                      ENvcColorimetry eColorimery,
	                                      int bIsNonTemporal);

void NVCAPI Nvc_10Bit_YUY2_To_32Bit_BGRA(const void *pSrc,
										int nSrcPitch,
										void *pDst,
										int nDstPitch,
										unsigned int nWidth,
										unsigned int nHeight, 
										float nAlpha,
										ENvcColorimetry eColorimetry,
										int bIsNonTemporal);

void NVCAPI Nvc_32Bit_BGRA_To_10Bit_YUVA4224(const void *pSrc,
											int nSrcPitch, 
											void *pDst,
											int nDstPitch, 
											unsigned int nWidth,
											unsigned int nHeight,
											ENvcColorimetry eColorimetry, 
											int bIsNonTemporal);

void NVCAPI Nvc_10Bit_YUVA4224_To_32Bit_BGRA(const void *pSrc,
												int nSrcPitch,
												void *pDst,
												int nDstPitch,
												unsigned int nWidth,
												unsigned int nHeight, 
												ENvcColorimetry eColorimetry,
												int bIsNonTemporal);

void NVCAPI Nvc_8Bit_BGRA_To_V210_C(const void *pSrc,
								    int nSrcPitch, 
									void *pDst,
									int nDstPitch,
									unsigned int nWidth,
									unsigned int nHeight,
									int bPremultiply,
									ENvcColorimetry eColorimetry);

void NVCAPI Nvc_16Bit_BGRA_To_V210_C(const void *pSrc,
								    int nSrcPitch, 
									void *pDst,
									int nDstPitch,
									unsigned int nWidth,
									unsigned int nHeight,
									int bPremultiply,
									ENvcColorimetry eColorimetry);

void NVCAPI Nvc_32Bit_BGRA_To_V210_C(const void *pSrc,
									 int nSrcPitch, 
									 void *pDst,
									 int nDstPitch,
									 unsigned int nWidth,
									 unsigned int nHeight,
									 int bPremultiply,
									 ENvcColorimetry eColorimetry);

void NVCAPI Nvc_8Bit_BGRA_To_V210(const void *pSrc,
								  int nSrcPitch, 
								  void *pDst,
								  int nDstPitch,
								  unsigned int nWidth,
								  unsigned int nHeight,
								  int bPremultiply,
								  ENvcColorimetry eColorimetry,
								  int bIsNonTemporal);

void NVCAPI Nvc_16Bit_BGRA_To_V210(const void *pSrc,
								   int nSrcPitch, 
								   void *pDst,
								   int nDstPitch,
								   unsigned int nWidth,
								   unsigned int nHeight,
								   int bPremultiply,
								   ENvcColorimetry eColorimetry,
								   int bIsNonTemporal);

void NVCAPI Nvc_32Bit_BGRA_To_V210(const void *pSrc,
								   int nSrcPitch, 
								   void *pDst,
								   int nDstPitch,
								   unsigned int nWidth,
								   unsigned int nHeight,
								   int bPremultiply,
								   ENvcColorimetry eColorimetry,
								   int bIsNonTemporal);

void NVCAPI Nvc_8Bit_YUV422_Planar_To_16Bit_BGRA(
	                               const void *pSrc[4],
	                               int nSrcPitch[4],
	                               void *pDst,
	                               int nDstPitch,
	                               unsigned int nWidth,
	                               unsigned int nHeight,
	                               unsigned int nAlpha,
	                               ENvcColorimetry eColorimetry,
	                               int bIsNonTemporal);

void NVCAPI Nvc_8Bit_YUYA422_Planar_To_16Bit_BGRA(
	                               const void *pSrc[4],
	                               int nSrcPitch[4],
	                               void *pDst,
	                               int nDstPitch,
	                               unsigned int nWidth,
	                               unsigned int nHeight,
	                               unsigned int nAlpha,
	                               ENvcColorimetry eColorimetry,
	                               int bIsNonTemporal);


void NVCAPI Nvc_8Bit_YUV422_Planar_To_32Bit_BGRA(
	                               const void *pSrc[4],
	                               int nSrcPitch[4],
	                               void *pDst,
	                               int nDstPitch,
	                               unsigned int nWidth,
	                               unsigned int nHeight,
	                               float fAlpha,
	                               ENvcColorimetry eColorimetry,
	                               int bIsNonTemporal);

void NVCAPI Nvc_8Bit_YUYA422_Planar_To_32Bit_BGRA(
	                               const void *pSrc[4],
	                               int nSrcPitch[4],
	                               void *pDst,
	                               int nDstPitch,
	                               unsigned int nWidth,
	                               unsigned int nHeight,
	                               float fAlpha,
	                               ENvcColorimetry eColorimetry,
	                               int bIsNonTemporal);

void NVCAPI Nvc_8Bit_BGRA_To_10Bit_YUV444_Planar(const void *pSrc,
									int nSrcPitch,
									void *pDstY,
									void *pDstU,
									void *pDstV,
									int nDstPitch[3],
									unsigned int nWidth,
									unsigned int nHeight,
									int bPremultiply,
									ENvcColorimetry eColorimetry);


void NVCAPI Nvc_10Bit_V210_To_32Bit_BGRA(const void *pSrc,
									int nSrcPitch,
									void *pDst,
									int nDstPitch,
									unsigned int nWidth,
									unsigned int nHeight,
									float fAlpha,
									ENvcColorimetry eColorimetry,
									int bIsNonTemporal);

void NVCAPI Nvc_16Bit_BGRA_To_8Bit_YUV420_Planar(const void *pSrc,
												int nSrcPitch,
												void *pDstY,
												void *pDstU,
												void *pDstV,
												int nDstPitch[4],
												unsigned int nWidth,
												unsigned int nHeight,
												int bPremultiply,
												ENvcColorimetry eColorimetry,
												int bIsNonTemporal,
												int bTopSignleLine);

void NVCAPI Nvc_16Bit_BGRA_To_10Bit_YUV420_Planar(const void *pSrc,
												int nSrcPitch,
												void *pDstY,
												void *pDstU,
												void *pDstV,
												int nDstPitch[4],
												unsigned int nWidth,
												unsigned int nHeight,
												int bPremultiply,
												ENvcColorimetry eColorimetry,
												int bIsNonTemporal,
												int bTopSignleLine);

void NVCAPI Nvc_32Bit_BGRA_To_8Bit_YUV420P(const void *pSrc,
										int nSrcPitch,
										void *pDstY,
										void *pDstU,
										void *pDstV,
										int nDstPitch[],
										unsigned int nWidth,
										unsigned int nHeight,
										int bPremultiply,
										ENvcColorimetry eColorimetry,
										int bIsNonTemporal,
										int bTopSignleLine);

void NVCAPI Nvc_32Bit_BGRA_To_10Bit_YUV420P(const void *pSrc,
										int nSrcPitch,
										void *pDstY,
										void *pDstU,
										void *pDstV,
										int nDstPitch[],
										unsigned int nWidth,
										unsigned int nHeight,
										int bPremultiply,
										ENvcColorimetry eColorimetry,
										int bIsNonTemporal,
										int bTopSignleLine);

void NVCAPI Nvc_32Bit_RGB_To_10Bit_YUV422P(const void *pSrc,
										int nSrcPitch,
										void *pDstY,
										void *pDstU,
										void *pDstV,
										int nDstPitch[3],
										unsigned int nWidth,
										unsigned int nHeight,
										int bPremultiply,
										ENvcColorimetry eColorimery,
										int bIsNonTemporal);

void NVCAPI Nvc_32Bit_RGB_To_8Bit_YUV422P(const void *pSrc,
										int nSrcPitch,
										void *pDstY,
										void *pDstU,
										void *pDstV,
										int nDstPitch[],
										unsigned int nWidth,
										unsigned int nHeight,
										int bPremultiply,
										ENvcColorimetry eColorimetry,
										int bIsNonTemporal);

void NVCAPI Nvc_10Bit_UYAVYA4224_To_32Bit_BGRA(const void *pSrc,
	                                    int nSrcPitch,
	                                    void *pDst,
	                                    int nDstPitch,
	                                    unsigned int nWidth,
	                                    unsigned int nHeight,
	                                    ENvcColorimetry eColorimetry,
	                                    int bIsNonTemporal);
//////////////////////////////////////////////////////////////////////////
// Alpha --> YUV(A) conversion 
//////////////////////////////////////////////////////////////////////////

void NVCAPI Nvc_8Bit_Matte_To_8Bit_YUY2(const void *pSrc,
                                        int nSrcPitch,
                                        void *pDst,
                                        int nDstPitch,
                                        unsigned int nWidth,
                                        unsigned int nHeight,
                                        int bIsNonTemporal);

void NVCAPI Nvc_8Bit_Matte_To_10Bit_YUY2(const void *pSrc,
	                                     int nSrcPitch,
	                                     void *pDst,
	                                     int nDstPitch,
	                                     unsigned int nWidth,
	                                     unsigned int nHeight,
	                                     int bIsNonTemporal);

void NVCAPI Nvc_8Bit_A_To_8Bit_YUVA4224(const void *pSrc,
	                                    int nSrcPitch,
	                                    void *pDst,
	                                    int nDstPitch,
	                                    unsigned int nWidth,
	                                    unsigned int nHeight,
	                                    int bIsNonTemporal);

void NVCAPI Nvc_8Bit_A_To_10Bit_YUVA4224(const void *pSrc,
	                                     int nSrcPitch,
	                                     void *pDst,
	                                     int nDstPitch,
	                                     unsigned int nWidth,
	                                     unsigned int nHeight,
	                                     int bIsNonTemporal);

void NVCAPI Nvc_16Bit_Matte_To_8Bit_YUY2(const void *pSrc,
                                         int nSrcPitch,
                                         void *pDst,
                                         int nDstPitch,
                                         unsigned int nWidth,
                                         unsigned int nHeight,
                                         int bIsNonTemporal);

void NVCAPI Nvc_16Bit_Matte_To_10Bit_YUY2(const void *pSrc,
	                                      int nSrcPitch,
	                                      void *pDst,
	                                      int nDstPitch,
	                                      unsigned int nWidth,
	                                      unsigned int nHeight,
	                                      int bIsNonTemporal);

void NVCAPI Nvc_16Bit_A_To_8Bit_YUVA4224(const void *pSrc,
	                                     int nSrcPitch,
	                                     void *pDst,
	                                     int nDstPitch,
	                                     unsigned int nWidth,
	                                     unsigned int nHeight,
	                                     int bIsNonTemporal);

void NVCAPI Nvc_16Bit_A_To_10Bit_YUVA4224(const void *pSrc,
	                                      int nSrcPitch,
	                                      void *pDst,
	                                      int nDstPitch,
	                                      unsigned int nWidth,
	                                      unsigned int nHeight,
	                                      int bIsNonTemporal);

void NVCAPI Nvc_32Bit_A_To_8Bit_YUY2(const void *pSrc,
	                                 int nSrcPitch,
	                                 void *pDst,
	                                 int nDstPitch,
	                                 unsigned int nWidth,
	                                 unsigned int nHeight,
	                                 int bIsNonTemporal);

void NVCAPI Nvc_32Bit_A_To_10Bit_YUY2(const void *pSrc,
	                                  int nSrcPitch,
	                                  void *pDst,
	                                  int nDstPitch,
	                                  unsigned int nWidth,
	                                  unsigned int nHeight,
	                                  int bIsNonTemporal);

void NVCAPI Nvc_32Bit_A_To_8Bit_YUVA4224(const void *pSrc,
	                                     int nSrcPitch,
	                                     void *pDst,
	                                     int nDstPitch,
	                                     unsigned int nWidth,
	                                     unsigned int nHeight,
	                                     int bIsNonTemporal);

void NVCAPI Nvc_32Bit_A_To_10Bit_YUVA4224(const void *pSrc,
	                                     int nSrcPitch,
	                                     void *pDst,
	                                     int nDstPitch,
	                                     unsigned int nWidth,
	                                     unsigned int nHeight,
	                                     int bIsNonTemporal);


//// CMYK related
//////////////////////////////////////////////////////////////////////////

void NVCAPI Nvc_8Bit_CMYK_To_8Bit_BGRA(const void *pSrc,
									   int nSrcPitch,
									   void *pDst,
									   int nDstPitch,
									   unsigned int nWidth,
									   unsigned int nHeight,
									   unsigned int nAlpha,
                                       int bIsInvertedCMYK,
									   int bIsNonTemporal);


// shuffle
//

void NVCAPI Nvc_32Bits_Shuffle_To_8Bits_BGRA( const void *pSrc,
											 int nSrcPitch,
											 void *pDst,
											 int nDstPitch,
											 unsigned int nWidth,
											 unsigned int nHeight,
											 uint32_t ui32RedMask,
											 uint32_t ui32GreenMask,
											 uint32_t ui32BlueMask);

//
// Color space conversion function for any supported color space which contain A, R, G, B channels,
// source and destination color space can be any of keNvcColorSpace_BGRA, keNvcColorSpace_BGR,
// keNvcColorSpace_RGBA, keNvcColorSpace_RGB, keNvcColorSpace_ARGB or keNvcColorSpace_Alpha.
// source and destination bit depth can be keNvcBitDepthInt8, keNvcBitDepthInt16 or keNvcBitDepthFloat32
//
void NVCAPI NvcColorSpaceConversion_ARGB(const void *pSrc,
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
                                         int bIsNonTemporal);

void NVCAPI NvcBitDepthConversion_Matte_10In16Bit(const void *pSrc,
														int nSrcPitch,
														ENvcBitDepth eSrcBitDepth,
														void *pDst,
														int nDstPitch,
														ENvcBitDepth eDstBitDepth,
														unsigned int nWidth,
														unsigned int nHeight,
														int bIsNonTemporal);

void NVCAPI Nvc_BuildLUT_10BitLog_to_32BitLinear(float fLUT[1024]);

void NVCAPI Nvc_10Bit_RGB_ModeB_To_32Bit_RGB(const void *pSrc,
											 void *pDst,
											 int nDstPitch,
											 unsigned int nWidth,
											 unsigned int nHeight,
											 const float fLUT[1024],
											 bool bIsLSB);

#ifdef __cplusplus
}
#endif

#endif	// __NVC_COLOR_CONV_H__

