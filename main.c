#include <stdio.h>

#include "RH_common.h"
#include "RH_config.h"
#include "RH_color.h"

#include "BLK_dsp.h"
#include "BLK_math.h"

#include "BLK_lib.h"



int main(int argc, char const *argv[]){
	/* code */


    float cs, ce;
    __BLK_Math_stat_confi_interval( 58.11, 6, 49, 0.95, &cs, &ce);
    printf( "[%f,%f]\n", cs, ce);
    
//    float_t a = 0.12;
//    for(int i=0; i<10; i++, a+=0.01)
//        printf("%s\n", BLK_Bin_ftoa(a) );
    
    return 0;
}
