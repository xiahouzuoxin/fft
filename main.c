/*
 * FileName : main.c
 * Author   : xiahouzuoxin @163.com
 * Version  : v1.0
 * Date     : 2014/4/9 22:27:55
 * Brief    : 
 * 
 * Copyright (C) MICL,USTB
 */
#include "zx_fft.h" 

#define  SAMPLE_NODES              (128)  
COMPLEX x[SAMPLE_NODES];  

static void MakeInput()  
{  
    int i;  
  
    for ( i=0;i<SAMPLE_NODES;i++ )  
    {  
        x[i].real = sin(PI*8*i/SAMPLE_NODES);  
        x[i].imag  = 0.0f;  
    }  
}  
  
int main(void)  
{  
    int i = 0;

    MakeInput();  
  
    fft(x,SAMPLE_NODES);  

    //printf("\n====FFT====\n");
    for (i=0; i<SAMPLE_NODES; i++) {
        printf("%.5f %.5f\n", x[i].real, x[i].imag);
    }

    //printf("\n====IFFT====\n");
  
    ifft(x,SAMPLE_NODES);  

    for (i=0; i<SAMPLE_NODES; i++) {
        printf("%.5f %.5f\n", x[i].real, x[i].imag);
    }

    return 0;
}  

