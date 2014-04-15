/*
 * zx_fft.h
 *
 *  Created on: 2013-8-5
 *      Author: monkeyzx
 */

#ifndef _ZX_FFT_H
#define _ZX_FFT_H

#include "Config.h"

#define TYPE_FFT_E     float    /* Type is the same with COMPLEX member */     

#ifndef PI
#define PI             (3.14159265f)
#endif

typedef COMPLEX TYPE_FFT;  /* Define COMPLEX in Config.h */

extern int fft(TYPE_FFT *x, uint32_t N);
extern int fft_real(TYPE_FFT *x, uint32_t N);
extern int ifft(TYPE_FFT *x, uint32_t N);
extern int ifft_real(TYPE_FFT *x, uint32_t N);

#endif /* ZX_FFT_H_ */

