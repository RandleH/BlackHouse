

#include <stdio.h>

#include "RH_common.h"
#include "RH_config.h"
#include "RH_color.h"

#include "BLK_dsp.h"
#include "BLK_math.h"



int main(int argc, char const *argv[]){
	/* code */
    
    
    const float U[][3] = {
        { 1.0,0.0,1.0 }, // C
        { 0.4,0.5,0.6 }, // D
        { 0.3,0.0,0.1 }, // E
        { 0.6,0.0,0.0 }, // F
        { 0.3,0.3,0.3 }, // G
        { 0.5,0.1,0.4 }, // H
        { 0.5,0.9,0.6 }, // I
        { 0.5,0.5,0.5 }  // M
    };
    
    const char C[] = "CDEFGHIM";
    
    float V  [ sizeof(U)/sizeof(*U) ][3] = { 0 };
    float E  [ sizeof(U)/sizeof(*U) ][3] = { 0 };
    float CNT[ sizeof(U)/sizeof(*U) ]    = { 0 };
    float DEG[ sizeof(U)/sizeof(*U) ]    = { 0 };
    
//    for( int i=0; i<8; i++ ){
//        __BLK_Math_fuzzy_deg_not( U[i], V[i], 3);
//        CNT[i] = __BLK_Math_fuzzy_count( U[i], 3);
//        printf("[%c] = %f \t %f\t%f\n", C[i], V[i][0], V[i][1], V[i][2] );
//    }
    
    printf("%f\n", __BLK_Math_fuzzy_equality(U[0], U[1], 3));
    
//    for(size_t i=0; i<8; i++){
//        for( size_t j=0; j<8; j++ ){
//            E[i][j] = __BLK_Math_fuzzy_subsethood(U[i], U[j], 3);
//
//            printf("%f\t", E[i][j]);
//        }
//        printf("\n");
//    }
    
    
    
    for( int i=0; i<8; i++ ){
        DEG[i] = __BLK_Math_fuzzy_degree( U[i], 3);
        printf("[%c] = %f \n", C[i], DEG[i]);
    }

	return 0;
}
