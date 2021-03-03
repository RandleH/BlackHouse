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
    
    __Graph_set_penColor(M_COLOR_SILVER);
    __Graph_rect_fill(0, 0, 799, 479, &BufferInfo, kApplyPixel_fill);
    __Graph_set_penColor(M_COLOR_PINK);
    
    __Graph_set_penSize(18);
    __Graph_rect_edged(370, 0, 430, 479, &BufferInfo, kApplyPixel_blur);
    
    
    __Graph_circle_edged(400, 240, 101, &BufferInfo, kApplyPixel_blur);
    
    __Graph_line_edged(40,40, 340, 90, &BufferInfo, kApplyPixel_fill);
    
    __Graph_line_edged(300,40, 500, 40, &BufferInfo, kApplyPixel_fill);
    __Graph_line_edged(40,300, 40, 400, &BufferInfo, kApplyPixel_fill);
//    __Graph_quad_fill(90, 90, 80, 300, 300, 70, 500, 400, &BufferInfo, kApplyPixel_fill);

    __ImgRGB888_out_bmp("/Users/randle_h/desktop/screen.bmp",&BufferInfo);
    
    return 0;
}
