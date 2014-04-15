/*
 * zx_fft.c
 *
 * Implementation of Fast Fourier Transform(FFT)
 * and reversal Fast Fourier Transform(IFFT)
 * 
 *  Created on: 2013-8-5
 *      Author: monkeyzx
 *
 * TEST OK 2014.01.14
 * == 2014.01.14
 *   Replace @BitReverse(x,x,N,M) by refrence to 
 *   <The Scientist and Engineer's Guide to Digital Signal Processing>
 */

#include "zx_fft.h"

const float sin_tb[] = {  // 精度(PI PI/2 PI/4 PI/8 PI/16 ... PI/(2^k))
0.000000, 1.000000, 0.707107, 0.382683, 0.195090, 0.098017, 
0.049068, 0.024541, 0.012272, 0.006136, 0.003068, 0.001534, 
0.000767, 0.000383, 0.000192, 0.000096, 0.000048, 0.000024, 
0.000012, 0.000006, 0.000003 
};


const float cos_tb[] = {  // 精度(PI PI/2 PI/4 PI/8 PI/16 ... PI/(2^k))
-1.000000, 0.000000, 0.707107, 0.923880, 0.980785, 0.995185, 
0.998795, 0.999699, 0.999925, 0.999981, 0.999995, 0.999999, 
1.000000, 1.000000, 1.000000, 1.000000 , 1.000000, 1.000000, 
1.000000, 1.000000, 1.000000
};

/*
 * FFT Algorithm
 * === Inputs ===
 * x : complex numbers
 * N : nodes of FFT. @N should be power of 2, that is 2^(*)
 * === Output ===
 * the @x contains the result of FFT algorithm, so the original data
 * in @x is destroyed, please store them before using FFT.
 */
int fft(TYPE_FFT *x, uint32_t N)
{
	int i,j,l,k,ip;
	static uint32_t M = 0;
	static int le,le2;
	static TYPE_FFT_E sR,sI,tR,tI,uR,uI;

	M = floor_log2_32(N);

	/*
	 * bit reversal sorting
	 */
	l = N >> 1;
	j = l;
	//BitReverse(x,x,N,M);
    for (i=1; i<=N-2; i++) {
        if (i < j) {
            tR = x[j].real;
			tI = x[j].imag;
            x[j].real = x[i].real;
			x[j].imag = x[i].imag;
            x[i].real = tR;
			x[i].imag = tI;
		}
		k = l;
		while (k <= j) {
            j = j - k;
			k = k >> 1;
		}
		j = j + k;
	}

	/*
	 * For Loops
	 */
	for (l=1; l<=M; l++) {   /* loop for ceil{log2(N)} */
		//le = (int)pow(2,l);
		le  = (int)(1 << l);
		le2 = (int)(le >> 1);
		uR = 1;
		uI = 0;

        k = floor_log2_32(le2);
        sR = cos_tb[k]; //cos(PI / le2);
        sI = -sin_tb[k];  // -sin(PI / le2)
		for (j=1; j<=le2; j++) {   /* loop for each sub DFT */
			//jm1 = j - 1;
			for (i=j-1; i<=N-1; i+=le) {  /* loop for each butterfly */
				ip = i + le2;
				tR = x[ip].real * uR - x[ip].imag * uI;
				tI = x[ip].real * uI + x[ip].imag * uR;
				x[ip].real = x[i].real - tR;
				x[ip].imag = x[i].imag - tI;
				x[i].real += tR;
				x[i].imag += tI;
			}  /* Next i */
			tR = uR;
			uR = tR * sR - uI * sI;
			uI = tR * sI + uI *sR;
		} /* Next j */
	} /* Next l */

	return 0;
}

/*
 * FFT Algorithm with inputs are real
 * === Inputs ===
 * x : complex numbers
 * N : nodes of FFT. @N should be power of 2, that is 2^(*)
 * === Output ===
 * the @x contains the result of FFT algorithm, so the original data
 * in @x is destroyed, please store them before using FFT.
 */
int fft_real(TYPE_FFT *x, uint32_t N)
{
	int i,j,l,k;
	static uint32_t M = 0;
    static uint32_t ND4 = 0;
	static TYPE_FFT_E sR,sI,tR,tI,uR,uI;


    /* Separate even and odd */
    M = N >> 1;
    for (i=0; i<M; i++) {
        x[i].real = x[i<<1].real;
        x[i].imag = x[(i<<1)+1].real;
    }

    /* N/2 points FFT */
    fft(x, M); 

    /* Even/Odd frequency domain decomposition */
    ND4 = N >> 2;
    for (i=1; i<ND4; i++) {
        j = M - i;
        k = i + M;
        l = j + M;
        x[k].real = (x[i].imag + x[j].imag) / 2;
        x[l].real = x[k].real;
        x[k].imag = -(x[i].real - x[j].real) / 2; 
        x[l].imag = -x[k].imag;
        x[i].real = (x[i].real + x[j].real) / 2;
        x[j].real = x[i].real;
        x[i].imag = (x[i].imag - x[j].imag) / 2;
        x[j].imag = -x[i].imag;
    }
    x[N-ND4].real = x[ND4].imag;
    x[M].real = x[0].imag;
    x[N-ND4].imag = 0;
    x[M].imag = 0;
    x[ND4].imag = 0;
    x[0].imag = 0;

    /* Complete last stage FFT */
    uR = 1;
    uI = 0;
    k = floor_log2_32(M);
    sR = cos_tb[k]; //cos(PI / M);
    sI = -sin_tb[k];  // -sin(PI / M)
    //sR = cos(PI / M);
    //sI = -sin(PI / M);

    for (i=0; i<M; i++) {   /* loop for each sub DFT */
        k = i + M;
        tR = x[k].real * uR - x[k].imag * uI;
        tI = x[k].real * uI + x[k].imag * uR;
        x[k].real = x[i].real - tR;
        x[k].imag = x[i].imag - tI;
        x[i].real += tR;
        x[i].imag += tI;

        tR = uR;
        uR = tR * sR - uI * sI;
        uI = tR * sI + uI *sR;
    } /* Next i */

	return 0;
}

/*
 * Inverse FFT Algorithm
 * === Inputs ===
 * x : complex numbers
 * N : nodes of FFT. @N should be power of 2, that is 2^(*)
 * === Output ===
 * the @x contains the result of FFT algorithm, so the original data
 * in @x is destroyed, please store them before using FFT.
 */
int ifft(TYPE_FFT *x, uint32_t N)
{
	int k = 0;

	for (k=0; k<=N-1; k++) {
		x[k].imag = -x[k].imag;
	}

	fft(x, N);    /* using FFT */

	for (k=0; k<=N-1; k++) {
		x[k].real = x[k].real / N;
		x[k].imag = -x[k].imag / N;
	}

	return 0;
}

/*
 * Inverse FFT Algorithm with real in Time Domain
 * === Inputs ===
 * x : complex numbers
 * N : nodes of FFT. @N should be power of 2, that is 2^(*)
 * === Output ===
 * the @x contains the result of FFT algorithm, so the original data
 * in @x is destroyed, please store them before using FFT.
 */
int ifft_real(TYPE_FFT *x, uint32_t N)
{
	int k = 0;

    /* Make frequency domain symmetrical */
	for (k=(N>>1)+1; k<N; k++) {
        x[k].real = x[N-k].real;
		x[k].imag = -x[N-k].imag;
	}

    /* Add real and imag parts together */
    for (k=0; k<N; k++) {
        x[k].real += x[k].imag;
    }

	fft_real(x, N);    /* using FFT */

	for (k=0; k<N; k++) {
		x[k].real = (x[k].real + x[k].imag) / N;
		x[k].imag = 0;
	}

	return 0;
}
