/*
 * FileName : xcorr.c
 * Author   : xiahouzuoxin @163.com
 * Version  : v1.0
 * Date     : 2014/4/10 11:11:18
 * Brief    : 
 * 
 * Copyright (C) MICL,USTB
 */

#include "Config.h"
#include "zx_fft.h"
#include "zx_math.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define CH_NUM           3
#define FIFO_SIZE        2048
#define FIFO_SIZE_DIV2   1024

static COMPLEX x[CH_NUM][FIFO_SIZE];


#define CONJ_REAL(ch1, ch2, i)   \
    (x[ch1][i].real*x[ch2][i].real \
     + x[ch1][i].imag*x[ch2][i].imag)

#define CONJ_IMAG(ch1, ch2, i)   \
    (x[ch1][i].real*x[ch2][i].imag \
     - x[ch1][i].imag*x[ch2][i].real)

#define PEAK_CHECK(delay, max, ch, i, temp)  \
do {  \
    max = x[ch][0].real*x[ch][0].real  \
            + x[ch][0].imag*x[ch][0].imag;  \
	delay = 0;  \
    for (i=1; i<50; i++) {  \
        temp = x[ch][i].real*x[ch][i].real  \
            + x[ch][i].imag*x[ch][i].imag;  \
        if (temp > max) {  \
            max   = temp;  \
            delay = i;  \
        }  \
    }  \
    for (i=FIFO_SIZE-1; i>FIFO_SIZE-50; i--) {  \
        temp = x[ch][i].real*x[ch][i].real  \
            + x[ch][i].imag*x[ch][i].imag;  \
        if (temp > max) {  \
            max   = temp;  \
            delay = i;  \
        }  \
    }  \
    if (delay > FIFO_SIZE_DIV2) {  \
        delay = delay - FIFO_SIZE;  \
    }  \
} while (0)


static float t01,t02,t12;


static float guass_rand(void)  
{  
    static float V1, V2, S;  
    static int phase = 0;  
    float X;  
  
    if ( phase == 0 ) {  
        do {  
            float U1 = (float)rand() / RAND_MAX;  
            float U2 = (float)rand() / RAND_MAX;  
  
            V1 = 2 * U1 - 1;  
            V2 = 2 * U2 - 1;  
            S = V1 * V1 + V2 * V2;  
        } while(S >= 1 || S == 0);  
  
        X = V1 * sqrt(-2 * log(S) / S);  
    } else {  
        X = V2 * sqrt(-2 * log(S) / S);  
    }  
  
    phase = 1 - phase;  
  
    return X;  
}  

static void MakeSignal(void)
{
    int i = 0;
#define SHIFT        15

    srand(time(NULL)); 
	for (i=0; i<SHIFT; i++) {
	    x[0][i].real = 5*sin(PI*i/(FIFO_SIZE_DIV2>>5)) + guass_rand();  
	    x[0][i].imag = 0; 
		x[1][i].real = 0 + guass_rand();
		x[1][i].imag = 0; 
		x[2][i].real = 0 + guass_rand();
		x[2][i].imag = 0;  
	}
//    for (i=SHIFT; i<FIFO_SIZE_DIV2; i++) {  
    for (i=SHIFT; i<FIFO_SIZE; i++) {  
        x[0][i].real = 5*sin(PI*i/(FIFO_SIZE_DIV2>>5)) + guass_rand();  
        x[0][i].imag = 0;  
		x[1][i].real = x[0][i-SHIFT].real + guass_rand();
		x[1][i].imag = 0; 
		x[2][i].real = x[1][i].real + guass_rand();
		x[2][i].imag = 0; 
    } 
}


int main(void)
{
	float temp  = 0;
	float max   = 0;
	int   ch    = 0;
	int   i     = 0;
    static COMPLEX rxy_tmp[6];


    MakeSignal();
    /* Cross-correlation Step1: FFT */
//    for (ch=0; ch<CH_NUM; ch++) {
//	    // corr length is 2N, so padding 0
//        for (i=FIFO_SIZE_DIV2; i<FIFO_SIZE; i++) {
//            x[ch][i].real = 0.;
//            x[ch][i].imag = 0.0;
//        }
//    } 
    for (ch=0; ch<3; ch++) {
        for (i=0; i<FIFO_SIZE; i++) {
            printf("%.5f %.5f\n", x[ch][i].real, x[ch][i].imag);
        }
    }

	for (ch=0; ch<CH_NUM; ch++) {
		fft_real(x[ch], FIFO_SIZE);
    }

    printf("\n\n");
    for (ch=0; ch<3; ch++) {
        for (i=0; i<FIFO_SIZE; i++) {
            printf("%.5f %.5f\n", x[ch][i].real, x[ch][i].imag);
        }
    }

    /* Cross-correlation Step2: Z=X.*conj(Y)*/
    for (i=0; i<FIFO_SIZE; i++) {
        // channel 01
        rxy_tmp[0].real = CONJ_REAL(0,1,i);
        rxy_tmp[0].imag = CONJ_IMAG(0,1,i);

        // channel 02
        rxy_tmp[1].real = CONJ_REAL(2,0,i);
        rxy_tmp[1].imag = CONJ_IMAG(2,0,i);
        
        // channel 12
        rxy_tmp[2].real = CONJ_REAL(1,2,i);
        rxy_tmp[2].imag = CONJ_IMAG(1,2,i);

        // Write back
        x[0][i].real = rxy_tmp[0].real;  // channel 01
        x[0][i].imag = rxy_tmp[0].imag;
        x[1][i].real = rxy_tmp[1].real;  // channel 02
        x[1][i].imag = rxy_tmp[1].imag;
        x[2][i].real = rxy_tmp[2].real;  // channel 12
        x[2][i].imag = rxy_tmp[2].imag;
    }


    printf("\n\n");
    for (ch=0; ch<3; ch++) {
        for (i=0; i<FIFO_SIZE; i++) {
            printf("%.5f %.5f\n", x[ch][i].real, x[ch][i].imag);
        }
    }

    /* Cross-correlation Step3: IFFT */
    ifft_real(x[0], FIFO_SIZE);  // 01
    ifft_real(x[1], FIFO_SIZE);  // 02
    ifft_real(x[2], FIFO_SIZE);  // 12

    printf("\n\n");
    for (ch=0; ch<3; ch++) {
        for (i=0; i<FIFO_SIZE; i++) {
            printf("%.5f %.5f\n", x[ch][i].real, x[ch][i].imag);
        }
    }


    /* Get diff time in ms */
    PEAK_CHECK(t01, max, 0, i, temp);
    PEAK_CHECK(t02, max, 1, i, temp);
    PEAK_CHECK(t12, max, 2, i, temp);

    printf("\n\n");
    printf("%.2f 0.00 \n%.2f 0.00 \n%.2f 0.00 \n", t01,t02,t12);
}

