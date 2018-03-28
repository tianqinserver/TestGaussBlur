#include <NvcVisionUtilsFFT.h>
#include <emmintrin.h>
#include <math.h>


// W(N)^k = exp(-j*2*pi*k/N)    k = 0, 1, ... N-1
void __NvcFFTGeneratePhaseFactors(unsigned int nOrder,
								  float *pFactorReal,
								  float *pFactorImg)
{
	unsigned int i, nCount = 1 << nOrder;
	double dConst = -2.0 * NVC_PI / nCount;

	for (i = 0; i < nCount; i++) {
		double x = dConst * i;
		pFactorReal[i] = (float)cos(x);
		pFactorImg[i] = (float)sin(x);
	}
}


// forward fast fourier transform routine
// input signal should be stored in natural order
// output signal will be stored in bit-reversed order
// NOTE: this routine will process FFT in-place!
void __NvcFFT(float *pSignalReal,
			  float *pSignalImg,
			  const float *pFactorReal,
			  const float *pFactorImg,
			  unsigned int nOrder)
{
	if (!nOrder)
		return;

	unsigned int count = 1 << nOrder, i, j, k;
	unsigned int section_count, section_size;

	section_count = 1;
	section_size = count;

	for (i = 0; i < nOrder; i++) {
		unsigned int idx = 0, half_section_size = section_size >> 1;

		for (j = 0; j < section_count; j++) {
			for (k = 0; k < half_section_size; k++) {
				__m128 xmm_real1_ss, xmm_img1_ss; 
				__m128 xmm_real2_ss, xmm_img2_ss;
				__m128 xmm_w_real_ss, xmm_w_img_ss;
				__m128 xmm_real_tmp_ss, xmm_img_tmp_ss;
				unsigned int idx1, idx2;

				// do basic butterfly computation
				// A = a + b
				// B = (a - b) * W(N)^r

				idx1 = idx + k;
				idx2 = idx + k + half_section_size;

				xmm_real1_ss = _mm_load_ss(&pSignalReal[idx1]);
				xmm_img1_ss = _mm_load_ss(&pSignalImg[idx1]);
				xmm_real2_ss = _mm_load_ss(&pSignalReal[idx2]);
				xmm_img2_ss = _mm_load_ss(&pSignalImg[idx2]);

				unsigned int w_idx = k << i;
				xmm_w_real_ss = _mm_load_ss(&pFactorReal[w_idx]);
				xmm_w_img_ss = _mm_load_ss(&pFactorImg[w_idx]);

				xmm_real_tmp_ss = _mm_sub_ss(xmm_real1_ss, xmm_real2_ss);
				xmm_img_tmp_ss = _mm_sub_ss(xmm_img1_ss, xmm_img2_ss);

				_mm_store_ss(&pSignalReal[idx1], _mm_add_ss(xmm_real1_ss, xmm_real2_ss));
				_mm_store_ss(&pSignalImg[idx1], _mm_add_ss(xmm_img1_ss, xmm_img2_ss));

				__m128 xmm_tmp_ss;

				xmm_tmp_ss = _mm_mul_ss(xmm_real_tmp_ss, xmm_w_real_ss);
				xmm_tmp_ss = _mm_sub_ss(xmm_tmp_ss, _mm_mul_ss(xmm_img_tmp_ss, xmm_w_img_ss));
				_mm_store_ss(&pSignalReal[idx2], xmm_tmp_ss);

				xmm_tmp_ss = _mm_mul_ss(xmm_real_tmp_ss, xmm_w_img_ss);
				xmm_tmp_ss = _mm_add_ss(xmm_tmp_ss, _mm_mul_ss(xmm_img_tmp_ss, xmm_w_real_ss));
				_mm_store_ss(&pSignalImg[idx2], xmm_tmp_ss);
			}

			idx += section_size;
		}

		// next decimation, section count doubled, section size cut half
		section_count <<= 1;
		section_size = half_section_size;
	}
}

// forward fast fourier transform routine
// input signal should be stored in natural order
// output signal will be stored in bit-reversed order
// NOTE: this routine will process FFT in-place!
// input signal are all real numbers, values in pSignalImg needn't to be initialized
void __NvcRealFFT(float *pSignalReal,
				  float *pSignalImg,
				  const float *pFactorReal,
				  const float *pFactorImg,
				  unsigned int nOrder)
{
	if (!nOrder) {
		*pSignalImg = 0;
		return;
	}

	unsigned int section_count, section_size, half_section_size;
	unsigned int count, i, j, k, idx;

	count = 1 << nOrder;
	section_count = 1;
	section_size = count;

	//
	// first decimation
	//
	idx = 0;
	half_section_size = count >> 1;

	for (k = 0; k < half_section_size; k++) {
		__m128 xmm_real1_ss, xmm_real2_ss;
		__m128 xmm_w_real_ss, xmm_w_img_ss;
		__m128 xmm_real_tmp_ss;
		unsigned int idx1, idx2;

		// do basic butterfly computation
		// A = a + b
		// B = (a - b) * W(N)^r

		idx1 = k;
		idx2 = k + half_section_size;

		xmm_real1_ss = _mm_load_ss(&pSignalReal[idx1]);
		xmm_real2_ss = _mm_load_ss(&pSignalReal[idx2]);

		xmm_w_real_ss = _mm_load_ss(&pFactorReal[k]);
		xmm_w_img_ss = _mm_load_ss(&pFactorImg[k]);

		xmm_real_tmp_ss = _mm_sub_ss(xmm_real1_ss, xmm_real2_ss);

		_mm_store_ss(&pSignalReal[idx1], _mm_add_ss(xmm_real1_ss, xmm_real2_ss));
		pSignalImg[idx1] = 0;
		_mm_store_ss(&pSignalReal[idx2], _mm_mul_ss(xmm_real_tmp_ss, xmm_w_real_ss));
		_mm_store_ss(&pSignalImg[idx2], _mm_mul_ss(xmm_real_tmp_ss, xmm_w_img_ss));
	}

	// next decimation, section count doubled, section size cut half
	section_count <<= 1;
	section_size = half_section_size;

	for (i = 1; i < nOrder; i++) {
		idx = 0;
		half_section_size = section_size >> 1;

		for (j = 0; j < section_count; j++) {
			for (k = 0; k < half_section_size; k++) {
				__m128 xmm_real1_ss, xmm_img1_ss; 
				__m128 xmm_real2_ss, xmm_img2_ss;
				__m128 xmm_w_real_ss, xmm_w_img_ss;
				__m128 xmm_real_tmp_ss, xmm_img_tmp_ss;
				unsigned int idx1, idx2;

				// do basic butterfly computation
				// A = a + b
				// B = (a - b) * W(N)^r

				idx1 = idx + k;
				idx2 = idx + k + half_section_size;

				xmm_real1_ss = _mm_load_ss(&pSignalReal[idx1]);
				xmm_img1_ss = _mm_load_ss(&pSignalImg[idx1]);
				xmm_real2_ss = _mm_load_ss(&pSignalReal[idx2]);
				xmm_img2_ss = _mm_load_ss(&pSignalImg[idx2]);

				unsigned int w_idx = k << i;
				xmm_w_real_ss = _mm_load_ss(&pFactorReal[w_idx]);
				xmm_w_img_ss = _mm_load_ss(&pFactorImg[w_idx]);

				xmm_real_tmp_ss = _mm_sub_ss(xmm_real1_ss, xmm_real2_ss);
				xmm_img_tmp_ss = _mm_sub_ss(xmm_img1_ss, xmm_img2_ss);

				_mm_store_ss(&pSignalReal[idx1], _mm_add_ss(xmm_real1_ss, xmm_real2_ss));
				_mm_store_ss(&pSignalImg[idx1], _mm_add_ss(xmm_img1_ss, xmm_img2_ss));

				__m128 xmm_tmp_ss;

				xmm_tmp_ss = _mm_mul_ss(xmm_real_tmp_ss, xmm_w_real_ss);
				xmm_tmp_ss = _mm_sub_ss(xmm_tmp_ss, _mm_mul_ss(xmm_img_tmp_ss, xmm_w_img_ss));
				_mm_store_ss(&pSignalReal[idx2], xmm_tmp_ss);

				xmm_tmp_ss = _mm_mul_ss(xmm_real_tmp_ss, xmm_w_img_ss);
				xmm_tmp_ss = _mm_add_ss(xmm_tmp_ss, _mm_mul_ss(xmm_img_tmp_ss, xmm_w_real_ss));
				_mm_store_ss(&pSignalImg[idx2], xmm_tmp_ss);
			}

			idx += section_size;
		}

		// next decimation, section count doubled, section size cut half
		section_count <<= 1;
		section_size = half_section_size;
	}
}

// forward fast fourier transform routine (four channel)
// input signal should be stored in natural order
// output signal will be stored in bit-reversed order
// NOTE: this routine will process FFT in-place!
// input signal are all real numbers, values in pSignalImg needn't to be initialized
void __NvcRealFFT_4C(float *pSignalReal,
					 float *pSignalImg,
					 const float *pFactorReal,
					 const float *pFactorImg,
					 unsigned int nOrder)
{
	if (!nOrder) {
		*pSignalImg = 0;
		return;
	}

	unsigned int section_count, section_size, half_section_size;
	unsigned int count, i, j, k, idx;

	count = 1 << nOrder;
	section_count = 1;
	section_size = count;

	//
	// first decimation
	//
	idx = 0;
	half_section_size = count >> 1;

	for (k = 0; k < half_section_size; k++) {
		__m128 xmm_real1_ps, xmm_real2_ps;
		__m128 xmm_w_real_ps, xmm_w_img_ps;
		__m128 xmm_real_tmp_ps;
		unsigned int idx1, idx2;

		// do basic butterfly computation
		// A = a + b
		// B = (a - b) * W(N)^r

		idx1 = k;
		idx2 = k + half_section_size;

		xmm_real1_ps = _mm_load_ps(&pSignalReal[idx1 * 4]);
		xmm_real2_ps = _mm_load_ps(&pSignalReal[idx2 * 4]);

		xmm_w_real_ps = _mm_set1_ps(pFactorReal[k]);
		xmm_w_img_ps = _mm_set1_ps(pFactorImg[k]);

		xmm_real_tmp_ps = _mm_sub_ps(xmm_real1_ps, xmm_real2_ps);

		_mm_store_ps(&pSignalReal[idx1 * 4], _mm_add_ps(xmm_real1_ps, xmm_real2_ps));
		_mm_store_ps(&pSignalImg[idx1 * 4], _mm_setzero_ps());
		_mm_store_ps(&pSignalReal[idx2 * 4], _mm_mul_ps(xmm_real_tmp_ps, xmm_w_real_ps));
		_mm_store_ps(&pSignalImg[idx2 * 4], _mm_mul_ps(xmm_real_tmp_ps, xmm_w_img_ps));
	}

	// next decimation, section count doubled, section size cut half
	section_count <<= 1;
	section_size = half_section_size;

	for (i = 1; i < nOrder; i++) {
		idx = 0;
		half_section_size = section_size >> 1;

		for (j = 0; j < section_count; j++) {
			for (k = 0; k < half_section_size; k++) {
				__m128 xmm_real1_ps, xmm_img1_ps;
				__m128 xmm_real2_ps, xmm_img2_ps;
				__m128 xmm_w_real_ps, xmm_w_img_ps;
				__m128 xmm_real_tmp_ps, xmm_img_tmp_ps;
				unsigned int idx1, idx2;

				// do basic butterfly computation
				// A = a + b
				// B = (a - b) * W(N)^r

				idx1 = idx + k;
				idx2 = idx + k + half_section_size;

				xmm_real1_ps = _mm_load_ps(&pSignalReal[idx1 * 4]);
				xmm_img1_ps = _mm_load_ps(&pSignalImg[idx1 * 4]);
				xmm_real2_ps = _mm_load_ps(&pSignalReal[idx2 * 4]);
				xmm_img2_ps = _mm_load_ps(&pSignalImg[idx2 * 4]);

				unsigned int w_idx = k << i;
				xmm_w_real_ps = _mm_set1_ps(pFactorReal[w_idx]);
				xmm_w_img_ps = _mm_set1_ps(pFactorImg[w_idx]);

				xmm_real_tmp_ps = _mm_sub_ps(xmm_real1_ps, xmm_real2_ps);
				xmm_img_tmp_ps = _mm_sub_ps(xmm_img1_ps, xmm_img2_ps);

				_mm_store_ps(&pSignalReal[idx1 * 4], _mm_add_ps(xmm_real1_ps, xmm_real2_ps));
				_mm_store_ps(&pSignalImg[idx1 * 4], _mm_add_ps(xmm_img1_ps, xmm_img2_ps));

				__m128 xmm_tmp1_ps, xmm_tmp2_ps;

				xmm_tmp1_ps = _mm_mul_ps(xmm_real_tmp_ps, xmm_w_real_ps);
				xmm_tmp2_ps = _mm_mul_ps(xmm_img_tmp_ps, xmm_w_img_ps);
				_mm_store_ps(&pSignalReal[idx2 * 4], _mm_sub_ps(xmm_tmp1_ps, xmm_tmp2_ps));

				xmm_tmp1_ps = _mm_mul_ps(xmm_real_tmp_ps, xmm_w_img_ps);
				xmm_tmp2_ps = _mm_mul_ps(xmm_img_tmp_ps, xmm_w_real_ps);
				_mm_store_ps(&pSignalImg[idx2 * 4], _mm_add_ps(xmm_tmp1_ps, xmm_tmp2_ps));
			}

			idx += section_size;
		}

		// next decimation, section count doubled, section size cut half
		section_count <<= 1;
		section_size = half_section_size;
	}
}

// inverse fast fourier transform routine
// input signal should be stored in bit-reversed order
// output signal will be stored in natural order
// NOTE: this routine will process IFFT in-place!
void __NvcIFFT(float *pSignalReal,
			   float *pSignalImg,
			   const float *pFactorReal,
			   const float *pFactorImg,
			   unsigned int nOrder)
{
	if (!nOrder)
		return;

	unsigned int section_count, section_size, half_section_size;
	unsigned int count, i, j, k;
	
	count = 1 << nOrder;
	section_count = count >> 1;
	section_size = 2;
	half_section_size = 1;

	for (i = 0; i < nOrder; i++) {
		unsigned int idx, order_delta;

		idx = 0;
		order_delta = nOrder - i - 1;

		for (j = 0; j < section_count; j++) {
			for (k = 0; k < half_section_size; k++) {
				__m128 xmm_real1_ss, xmm_img1_ss; 
				__m128 xmm_real2_ss, xmm_img2_ss;
				__m128 xmm_w_real_ss, xmm_w_img_ss;
				__m128 xmm_real_tmp_ss, xmm_img_tmp_ss;
				unsigned int idx1, idx2;

				// do basic butterfly computation
				// A = a + b * W(N)^(-r)
				// B = a - b * W(N)^(-r)

				idx1 = idx + k;
				idx2 = idx + k + half_section_size;

				xmm_real1_ss = _mm_load_ss(&pSignalReal[idx1]);
				xmm_img1_ss = _mm_load_ss(&pSignalImg[idx1]);
				xmm_real2_ss = _mm_load_ss(&pSignalReal[idx2]);
				xmm_img2_ss = _mm_load_ss(&pSignalImg[idx2]);

				unsigned int w_idx = k << order_delta;
				xmm_w_real_ss = _mm_load_ss(&pFactorReal[w_idx]);
				xmm_w_img_ss = _mm_set_ss(-pFactorImg[w_idx]);

				xmm_real_tmp_ss = _mm_mul_ss(xmm_w_real_ss, xmm_real2_ss);
				xmm_real_tmp_ss = _mm_sub_ss(xmm_real_tmp_ss, _mm_mul_ss(xmm_w_img_ss, xmm_img2_ss));
				xmm_img_tmp_ss = _mm_mul_ss(xmm_w_real_ss, xmm_img2_ss);
				xmm_img_tmp_ss = _mm_add_ss(xmm_img_tmp_ss, _mm_mul_ss(xmm_w_img_ss, xmm_real2_ss));

				_mm_store_ss(&pSignalReal[idx1], _mm_add_ss(xmm_real1_ss, xmm_real_tmp_ss));
				_mm_store_ss(&pSignalImg[idx1], _mm_add_ss(xmm_img1_ss, xmm_img_tmp_ss));
				_mm_store_ss(&pSignalReal[idx2], _mm_sub_ss(xmm_real1_ss, xmm_real_tmp_ss));
				_mm_store_ss(&pSignalImg[idx2], _mm_sub_ss(xmm_img1_ss, xmm_img_tmp_ss));
			}

			idx += section_size;
		}

		// next decimation, section count cut half, section size doubled
		section_count >>= 1;
		half_section_size = section_size;
		section_size <<= 1;
	}

	__m128 xmm_divisor_ss = _mm_set_ss(1.0f / count);
	for (i = 0; i < count; i++) {
		__m128 xmm_ss;

		xmm_ss = _mm_load_ss(&pSignalReal[i]);
		_mm_store_ss(&pSignalReal[i], _mm_mul_ss(xmm_ss, xmm_divisor_ss));
		xmm_ss = _mm_load_ss(&pSignalImg[i]);
		_mm_store_ss(&pSignalImg[i], _mm_mul_ss(xmm_ss, xmm_divisor_ss));
	}
}


// inverse fast fourier transform routine
// input signal should be stored in bit-reversed order
// output signal will be stored in natural order
// NOTE: this routine will process IFFT in-place!
// output signal are all real numbers, values in pSignalImg is undetermined after transformation
void __NvcRealIFFT(float *pSignalReal,
				   float *pSignalImg,
				   const float *pFactorReal,
				   const float *pFactorImg,
				   unsigned int nOrder)
{
	if (!nOrder)
		return;

	unsigned int section_count, section_size, half_section_size;
	unsigned int count, i, j, k, idx;

	count = 1 << nOrder;
	section_count = count >> 1;
	section_size = 2;
	half_section_size = 1;

	for (i = 0; i < nOrder - 1; i++) {
		unsigned int order_delta;

		idx = 0;
		order_delta = nOrder - i - 1;

		for (j = 0; j < section_count; j++) {
			for (k = 0; k < half_section_size; k++) {
				__m128 xmm_real1_ss, xmm_img1_ss; 
				__m128 xmm_real2_ss, xmm_img2_ss;
				__m128 xmm_w_real_ss, xmm_w_img_ss;
				__m128 xmm_real_tmp_ss, xmm_img_tmp_ss;
				unsigned int idx1, idx2;

				// do basic butterfly computation
				// A = a + b * W(N)^(-r)
				// B = a - b * W(N)^(-r)

				idx1 = idx + k;
				idx2 = idx + k + half_section_size;

				xmm_real1_ss = _mm_load_ss(&pSignalReal[idx1]);
				xmm_img1_ss = _mm_load_ss(&pSignalImg[idx1]);
				xmm_real2_ss = _mm_load_ss(&pSignalReal[idx2]);
				xmm_img2_ss = _mm_load_ss(&pSignalImg[idx2]);

				unsigned int w_idx = k << order_delta;
				xmm_w_real_ss = _mm_load_ss(&pFactorReal[w_idx]);
				xmm_w_img_ss = _mm_set_ss(-pFactorImg[w_idx]);

				xmm_real_tmp_ss = _mm_mul_ss(xmm_w_real_ss, xmm_real2_ss);
				xmm_real_tmp_ss = _mm_sub_ss(xmm_real_tmp_ss, _mm_mul_ss(xmm_w_img_ss, xmm_img2_ss));
				xmm_img_tmp_ss = _mm_mul_ss(xmm_w_real_ss, xmm_img2_ss);
				xmm_img_tmp_ss = _mm_add_ss(xmm_img_tmp_ss, _mm_mul_ss(xmm_w_img_ss, xmm_real2_ss));

				_mm_store_ss(&pSignalReal[idx1], _mm_add_ss(xmm_real1_ss, xmm_real_tmp_ss));
				_mm_store_ss(&pSignalImg[idx1], _mm_add_ss(xmm_img1_ss, xmm_img_tmp_ss));
				_mm_store_ss(&pSignalReal[idx2], _mm_sub_ss(xmm_real1_ss, xmm_real_tmp_ss));
				_mm_store_ss(&pSignalImg[idx2], _mm_sub_ss(xmm_img1_ss, xmm_img_tmp_ss));
			}

			idx += section_size;
		}

		// next decimation, section count cut half, section size doubled
		section_count >>= 1;
		half_section_size = section_size;
		section_size <<= 1;
	}

	//
	// last decimation
	//
	__m128 xmm_divisor_ss = _mm_set_ss(1.0f / count);

	for (k = 0; k < half_section_size; k++) {
		__m128 xmm_real1_ss; 
		__m128 xmm_real2_ss, xmm_img2_ss;
		__m128 xmm_w_real_ss, xmm_w_img_ss;
		__m128 xmm_real_tmp_ss;
		unsigned int idx1, idx2;

		// do basic butterfly computation
		// A = a + b * W(N)^(-r)
		// B = a - b * W(N)^(-r)

		idx1 = k;
		idx2 = k + half_section_size;

		xmm_real1_ss = _mm_load_ss(&pSignalReal[idx1]);
		xmm_real2_ss = _mm_load_ss(&pSignalReal[idx2]);
		xmm_img2_ss = _mm_load_ss(&pSignalImg[idx2]);

		xmm_w_real_ss = _mm_load_ss(&pFactorReal[k]);
		xmm_w_img_ss = _mm_set_ss(-pFactorImg[k]);

		xmm_real_tmp_ss = _mm_mul_ss(xmm_w_real_ss, xmm_real2_ss);
		xmm_real_tmp_ss = _mm_sub_ss(xmm_real_tmp_ss, _mm_mul_ss(xmm_w_img_ss, xmm_img2_ss));

		__m128 xmm_tmp_ss;

		xmm_tmp_ss = _mm_add_ss(xmm_real1_ss, xmm_real_tmp_ss);
		xmm_tmp_ss = _mm_mul_ss(xmm_tmp_ss, xmm_divisor_ss);
		_mm_store_ss(&pSignalReal[idx1], xmm_tmp_ss);

		xmm_tmp_ss = _mm_sub_ss(xmm_real1_ss, xmm_real_tmp_ss);
		xmm_tmp_ss = _mm_mul_ss(xmm_tmp_ss, xmm_divisor_ss);
		_mm_store_ss(&pSignalReal[idx2], xmm_tmp_ss);
	}
}


// inverse fast fourier transform routine (four channel)
// input signal should be stored in bit-reversed order
// output signal will be stored in natural order
// NOTE: this routine will process IFFT in-place!
// output signal are all real numbers, values in pSignalImg is undetermined after transformation
void __NvcRealIFFT_4C(float *pSignalReal,
					  float *pSignalImg,
					  const float *pFactorReal,
					  const float *pFactorImg,
					  unsigned int nOrder)
{
	if (!nOrder)
		return;

	unsigned int section_count, section_size, half_section_size;
	unsigned int count, i, j, k, idx;

	count = 1 << nOrder;
	section_count = count >> 1;
	section_size = 2;
	half_section_size = 1;

	for (i = 0; i < nOrder - 1; i++) {
		unsigned int order_delta;

		idx = 0;
		order_delta = nOrder - i - 1;

		for (j = 0; j < section_count; j++) {
			for (k = 0; k < half_section_size; k++) {
				__m128 xmm_real1_ps, xmm_img1_ps;
				__m128 xmm_real2_ps, xmm_img2_ps;
				__m128 xmm_w_real_ps, xmm_w_img_ps;
				__m128 xmm_real_tmp_ps, xmm_img_tmp_ps;
				unsigned int idx1, idx2;

				// do basic butterfly computation
				// A = a + b * W(N)^(-r)
				// B = a - b * W(N)^(-r)

				idx1 = idx + k;
				idx2 = idx + k + half_section_size;

				xmm_real1_ps = _mm_load_ps(&pSignalReal[idx1 * 4]);
				xmm_img1_ps = _mm_load_ps(&pSignalImg[idx1 * 4]);
				xmm_real2_ps = _mm_load_ps(&pSignalReal[idx2 * 4]);
				xmm_img2_ps = _mm_load_ps(&pSignalImg[idx2 * 4]);

				unsigned int w_idx = k << order_delta;
				xmm_w_real_ps = _mm_set1_ps(pFactorReal[w_idx]);
				xmm_w_img_ps = _mm_set1_ps(-pFactorImg[w_idx]);

				xmm_real_tmp_ps = _mm_mul_ps(xmm_w_real_ps, xmm_real2_ps);
				xmm_real_tmp_ps = _mm_sub_ps(xmm_real_tmp_ps, _mm_mul_ps(xmm_w_img_ps, xmm_img2_ps));
				xmm_img_tmp_ps = _mm_mul_ps(xmm_w_real_ps, xmm_img2_ps);
				xmm_img_tmp_ps = _mm_add_ps(xmm_img_tmp_ps, _mm_mul_ps(xmm_w_img_ps, xmm_real2_ps));

				_mm_store_ps(&pSignalReal[idx1 * 4], _mm_add_ps(xmm_real1_ps, xmm_real_tmp_ps));
				_mm_store_ps(&pSignalImg[idx1 * 4], _mm_add_ps(xmm_img1_ps, xmm_img_tmp_ps));
				_mm_store_ps(&pSignalReal[idx2 * 4], _mm_sub_ps(xmm_real1_ps, xmm_real_tmp_ps));
				_mm_store_ps(&pSignalImg[idx2 * 4], _mm_sub_ps(xmm_img1_ps, xmm_img_tmp_ps));
			}

			idx += section_size;
		}

		// next decimation, section count cut half, section size doubled
		section_count >>= 1;
		half_section_size = section_size;
		section_size <<= 1;
	}

	//
	// last decimation
	//
	__m128 xmm_divisor_ps = _mm_set1_ps(1.0f / count);

	for (k = 0; k < half_section_size; k++) {
		__m128 xmm_real1_ps;
		__m128 xmm_real2_ps, xmm_img2_ps;
		__m128 xmm_w_real_ps, xmm_w_img_ps;
		__m128 xmm_real_tmp_ps;
		unsigned int idx1, idx2;

		// do basic butterfly computation
		// A = a + b * W(N)^(-r)
		// B = a - b * W(N)^(-r)

		idx1 = k;
		idx2 = k + half_section_size;

		xmm_real1_ps = _mm_load_ps(&pSignalReal[idx1 * 4]);
		xmm_real2_ps = _mm_load_ps(&pSignalReal[idx2 * 4]);
		xmm_img2_ps = _mm_load_ps(&pSignalImg[idx2 * 4]);

		xmm_w_real_ps = _mm_set1_ps(pFactorReal[k]);
		xmm_w_img_ps = _mm_set1_ps(-pFactorImg[k]);

		xmm_real_tmp_ps = _mm_mul_ps(xmm_w_real_ps, xmm_real2_ps);
		xmm_real_tmp_ps = _mm_sub_ps(xmm_real_tmp_ps, _mm_mul_ps(xmm_w_img_ps, xmm_img2_ps));

		__m128 xmm_tmp_ps;

		xmm_tmp_ps = _mm_add_ps(xmm_real1_ps, xmm_real_tmp_ps);
		xmm_tmp_ps = _mm_mul_ps(xmm_tmp_ps, xmm_divisor_ps);
		_mm_store_ps(&pSignalReal[idx1 * 4], xmm_tmp_ps);

		xmm_tmp_ps = _mm_sub_ps(xmm_real1_ps, xmm_real_tmp_ps);
		xmm_tmp_ps = _mm_mul_ps(xmm_tmp_ps, xmm_divisor_ps);
		_mm_store_ps(&pSignalReal[idx2 * 4], xmm_tmp_ps);
	}
}


