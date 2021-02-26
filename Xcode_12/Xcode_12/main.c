//
//  main.c
//  Xcode_12
//
//  Created by Randle.Helmslay on 2021/2/26.
//

#include <stdio.h>
#include <stdint.h>
#include "RH_image.h"
#include "RH_graphic.h"


__UNION_PixelRGB888_t Screen_buffer[480][800] = {0};

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");

    __GraphInfo_t BufferInfo = {    .pBuffer = Screen_buffer[0],
                                    .height  = 480  ,
                                    .width   = 800 };

    __Graph_circle_fill(200, 200, 101, &BufferInfo, kApplyPixel_fill);

    __OutBMP_ImgRGB888("/Users/randle_h/desktop/lenna.bmp",&BufferInfo);
    
    return 0;
}
