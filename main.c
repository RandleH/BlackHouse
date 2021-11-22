#include <stdio.h>

#include "RH_common.h"
#include "RH_config.h"
#include "RH_color.h"

#include "BLK_dsp.h"
#include "BLK_math.h"

float price( float S, float F, float tau, float rate, float sig ){
    float d1 = (logf(S/F)+( rate+sig*sig/2.0 )*tau) / (sig*sqrtf(tau));
    float d2 = d1-sig*sqrtf(tau);
    
#define N __BLK_Math_std_normal
    
    return S* N(d1) - F*expf(-rate*tau)*N(d2);
    
#undef N
    
}


int main(int argc, char const *argv[]){
	/* code */
    
    
    
    printf("%f\n", price( 47, 45, 183.0/365.0, 0.1, 0.25 ) );
    
    for (float r=0.1; r<1.0; r+= 0.1)
        printf("%f\n", price( 47, 45, 183.0/365.0, r, 0.4 ) );
    
	return 0;
}
