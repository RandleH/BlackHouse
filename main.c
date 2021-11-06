

#include <stdio.h>

#include "RH_common.h"
#include "RH_config.h"
#include "RH_color.h"

#include "BLK_dsp.h"
#include "BLK_math.h"



int main(int argc, char const *argv[]){
	/* code */
    
    
    
    float PX[] = {0.5,0.25,0.125,0.125};
    float PY[] = {0.25,0.25,0.25,0.25};
    
    float PXY[4][4] ={
        {0.125   ,0.0625 ,0.0625,0.25 },\
        {0.0625  , 0.125 ,0.0625,   0 },\
        {0.03125 ,0.03125,0.0625,   0 },\
        {0.03125 ,0.03125,0.0625,   0}
    };
    
    
    for(int i=0; i< 4;i++){
        for(int j=0;j<4;j++){
            printf("%f\t", PXY[i][j]/(PX[i]*PY[j]));
        }
        printf("\n");
    }
    
    float res = 0.0;
    
    for(int i=0;i<4;i++){
        res+=__BLK_Math_inf_entropy(PXY[i],4 );
    }
    
    printf("%f\n",res );
    
	return 0;
}
