

#include <stdio.h>

#include "RH_common.h"
#include "RH_config.h"
#include "RH_color.h"

#include "BLK_image.h"
#include "BLK_math.h"
#include "BLK_data.h"
#include "BLK_lib.h"

int main(int argc, char const *argv[]){
	/* code */
   
	printf( "%ld\n", BLK_Math_sqrt(10));

    printf( "%ld\n", BLK_Math_gcd(108, 98));

	return 0;
}