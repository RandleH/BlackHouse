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


int main(int argc, const char * argv[]) {
    // insert code here...
    printf( "%lf\n", BLK_Gussian_func_2D_XY(200, 0, 133, 80, 0, 0)  );
    
    
    
#if 1
    S_BLK_Img888_t* IMG = BLK_Img888_create(800, 480);
    BLK_TYPE(Pixel888) colors[4] = {
        M_COLOR_RED,
        M_COLOR_WHITE,
        M_COLOR_DARKGRAY,
        M_COLOR_DARKGRAY
    };
    
    BLK_Img888_draw_img_center(IMG, colors, 4);
    
//    BLK_FUNC(Graph,set_penSize       )( 9 );
//    BLK_FUNC(Graph,set_penColor      )( M_COLOR_YELLOW );
//    BLK_FUNC(Graph,set_color_depth   )( kBLK_ColorDepth_24Bit  );
//    BLK_FUNC(Graph,set_render_method )( kBLK_RenderMethod_fill );
//
//    BLK_Graph_triangle_fill       ( 300, 240, 407, 370, 120, 104, IMG, NULL );
//
    BLK_Img888_out_bmp("/Users/randle_h/Desktop/screen.bmp", IMG);
    BLK_Img888_free(IMG);
#endif
    return 0;
}
