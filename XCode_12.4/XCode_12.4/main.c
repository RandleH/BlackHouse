//
//  main.c
//  XCode_12.4
//
//  Created by Randle.Helmslay on 2021/2/18.
//

#include "RH_image.h"
#include <stdio.h>

int main(int argc, const char * argv[]) {
    // insert code here...
    
    const char* __restrict__ src  = "/Users/randle_h/desktop/lenna.bmp";
    const char* __restrict__ des  = "/Users/randle_h/desktop/lenna_.bmp";

    __ImageRGB888_t* IMG_IN  = __LoadBMP_ImgRGB888(src);

    __ImageRGB888_t* IMG_OUT = __Create_ImgRGB888(IMG_IN->width, IMG_IN->height);
    __Blur_Average_ImgRGB888(IMG_IN,IMG_OUT,80000,100);
    __Blur_Average_ImgRGB888(IMG_OUT,IMG_IN,80000,100);
    __OutBMP_ImgRGB888(des, IMG_IN);
    printf("Hello, World!\n");
    return 0;
}
