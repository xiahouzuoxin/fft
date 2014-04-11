/*
 * FileName : zx_math.c
 * Author   : xiahouzuoxin
 * Date     : 2013.12.23
 * Version  : v1.0
 * Brief    :  
 */
#include "zx_math.h"
#include <limits.h>

/*
 * DSPF_sp_maxval
 * @brief 
 */
void dsp_max_min_val(const TYPE_MAX* x, int nx, TYPE_MAX *max, TYPE_MAX *min)
{
	int i;

    *max = SHRT_MIN;
    *min = SHRT_MAX;
	for (i = 0; i < nx; i++) {
		if (x[i] > *max) {
			*max = x[i];
		} else if (x[i] < *min) {
            *min = x[i];
        }
	}

    return;
}

/*
 * scale
 * @brief 
 *  if data not in the same scale, use this function to scale 
 *  them to the same scale, for example [-10000 +10000] 
 * @input params
 *  x: input data
 *  xmax : max value from x[]
 *  xmin : min value from x[]
 *  n    : size of x[]
 *  s_low: low boundary of scale
 *  s_high: high boundary of scale 
 * @output params
 *  x: x[] will be changed after scale funtion and return it back
 * @retval
 *  None
 */
void scale(TYPE_SCALE x[], 
           TYPE_SCALE xmax, 
           TYPE_SCALE xmin,
           int32_t n, 
           TYPE_SCALE s_low, 
           TYPE_SCALE s_high)
{
    int32_t i = 0;
	TYPE_SCALE delta_s = s_high - s_low;
	TYPE_SCALE delta_x = xmax - xmin;

    for (i=0; i<n; i++) {
        x[i] = delta_s * (x[i] - xmin) / delta_x + s_low; 
	}
}



/*
 * COMP_abs
 * @brief 
 * @input params
 * @output params
 * @retval
 */
float cabs(COMPLEX x)
{
    float mag = 0.0f;

    mag = x.real*x.real + x.imag*x.imag;
    mag = sqrt(mag);

    return mag;
}


/*
 * @brief   计算二进制表示中1出现的个数的快速算法.
 *          有c个1，则循环c次
 * @inputs  
 * @outputs 
 * @retval  
 */
int ones_32(uint32_t n)
{
    unsigned int c =0 ;
    for (c = 0; n; ++c)
    {
        n &= (n -1) ; // 清除最低位的1
    }
    return c ;
}

/*
 * @brief   
 *   floor{long2(x)}
 *   x must > 0
 * @inputs  
 * @outputs 
 * @retval  
 */
uint32_t floor_log2_32(uint32_t x)
{
    x |= (x>>1);
    x |= (x>>2);
    x |= (x>>4);
    x |= (x>>8);
    x |= (x>>16);

    return (ones_32(x>>1));
}

