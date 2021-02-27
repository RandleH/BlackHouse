//
//  main.c
//  Xcode_11
//
//  Created by Randle Helmslay on 2021/2/27.
//  Copyright © 2021 Randle Helmslay. All rights reserved.
//

#include <stdio.h>
#include "RH_image.h"
#include "RH_graphic.h"
__UNION_PixelRGB888_t Screen_buffer[480][800] = {0};
int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    __GraphInfo_t BufferInfo = {    .pBuffer = Screen_buffer[0],
                                    .height  = 480  ,
                                    .width   = 800 };

    __Graph_set_penColor(M_COLOR_SILVER);
    __Graph_set_penSize(7);
    
    __Graph_rect_fill(0  ,   0, 800-1, 480-1, &BufferInfo, kApplyPixel_fill);
    
    __Graph_set_penColor(M_COLOR_TAN);
    __Graph_rect_fill(300,   0, 400  , 480-1, &BufferInfo, kApplyPixel_fill);
    __Graph_circle_fill(350, 250,150 , &BufferInfo, kApplyPixel_blur);
    __OutBMP_ImgRGB888("/Users/randle_h/desktop/lenna.bmp",&BufferInfo);
    
    return 0;
}
