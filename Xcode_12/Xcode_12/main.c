//
//  main.c
//  Xcode_12
//
//  Created by Randle.Helmslay on 2021/2/26.
//

#include <stdio.h>
#include <stdint.h>
#include "BLK_image.h"
#include "BLK_graphic.h"

BLK_SRCT(Img888)* BLK_FUNC( Img888, draw_img_center1 )
( BLK_SRCT(Img888)* dst, const BLK_TYPE(Pixel888)* colors, size_t size );
int main(int argc, const char * argv[]) {
    // insert code here...
    
    BLK_Graph_set_color_depth(kBLK_ColorDepth_24Bit);
    BLK_Graph_set_render_method(kBLK_RenderMethod_fill);
    BLK_Graph_set_penColor(MAKE_COLOR( 0,255,0 ));
    BLK_Graph_set_penSize(50);
    
#if 1
    S_BLK_Img888_t* IMG = BLK_Img888_create(1920, 1080);
    BLK_TYPE(Pixel888) colors[] = {
        MAKE_COLOR(234,233,3)    ,
        M_COLOR_BLACK  ,
        M_COLOR_YELLOW ,
        M_COLOR_AQUA   ,
        M_COLOR_BEIGE  ,
        M_COLOR_VOILET
        
    };

    RH_RECORD_TIME( BLK_Img888_draw_img_blur(IMG, colors, sizeof(colors)/sizeof(*colors)), printf);
    
    BLK_Graph_set_penColor(MAKE_COLOR( 98,98,0 ));
    BLK_Graph_rect_round_fill(1920/2-350, 1080/2-250, 1920/2+350, 1080/2+250, IMG, NULL);
    
    BLK_Img888_out_bmp("/Users/randle_h/Desktop/screen.bmp", IMG);
    BLK_Img888_free(IMG);
#endif
    return 0;
}
