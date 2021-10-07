

#include <stdio.h>

#include "RH_common.h"
#include "RH_config.h"
#include "RH_color.h"

#include "BLK_dsp.h"
#include "BLK_math.h"



static float hw04( int n ){
    int k = 4+1;
    float res = 0.0;
    while(k--){
        res += powf( (5.0/(float)n), k)/BLK_Math_factorial(k);
    }
    res *= expf(-5.0/(float)n);
    
    
    return res;
}

static float poisson( int xs, int xe, int lamda ){
    float res = 0.0;
    for( int i=xs; i<=xe; i++ ){
        res += powf(lamda,i)/(float)BLK_Math_factorial(i);
    }
    res *= exp(-lamda);
    
    return res;
}

int main(int argc, char const *argv[]){
	/* code */
    
    printf("%f\n",poisson( 0,5, 4));
    
	return 0;
}
