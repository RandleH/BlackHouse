//
//  main.c
//  Xcode_11
//
//  Created by Randle Helmslay on 2021/2/27.
//  Copyright © 2021 Randle Helmslay. All rights reserved.
//

#include <stdio.h>
#include "BLK_image.h"
#include "BLK_graphic.h"
#include "RH_common.h"


int main(int argc, const char * argv[]) {
    // insert code here...
    
    S_BLK_Img888_t* IMG = BLK_Img888_create(800, 480);
    
    BLK_Graph_set_color_depth   ( kBLK_ColorDepth_24Bit  );
    BLK_Graph_set_render_method ( kBLK_RenderMethod_fill );
    
    BLK_Graph_set_penColor( M_COLOR_RED );
    BLK_Graph_circle_fill     ( 400, 240,  39,      IMG, NULL );
    
    BLK_Graph_set_penColor( M_COLOR_BLUE );
    BLK_Graph_set_penSize (20);
    BLK_Graph_rect_edged      (  30,  50, 130, 112, IMG, NULL );
    
    BLK_Graph_set_penColor(M_COLOR_AQUA);
    BLK_Graph_set_penSize (10);
    BLK_Graph_rect_round_raw  ( 104, 178, 200, 234, IMG, NULL );
    
    BLK_Graph_set_penColor(M_COLOR_RED);
    BLK_Graph_rect_raw        ( 300, 240, 307, 270, IMG, NULL );
    BLK_Graph_set_penColor(M_COLOR_AQUA);
    BLK_Graph_capsule_fill    ( 300, 240, 307, 270, IMG, NULL );
    
    
    BLK_Graph_set_penColor(M_COLOR_GREEN);
    BLK_Graph_rect_raw        ( 300, 240, 407, 370, IMG, NULL );
    BLK_Graph_rect_edged      ( 300, 240, 407, 370, IMG, NULL );
    BLK_Graph_rect_fill       ( 300, 240, 407, 370, IMG, NULL );
    BLK_Graph_rect_round_fill ( 300, 240, 407, 370, IMG, NULL );
    BLK_Graph_rect_round_raw  ( 300, 240, 407, 370, IMG, NULL );
    
    BLK_Graph_line_raw        ( 300, 240, 407, 370, IMG, NULL );
    BLK_Graph_line_edged      ( 300, 240, 407, 370, IMG, NULL );
    BLK_Graph_line_fill       ( 300, 240, 407, 370, IMG, NULL );
    BLK_Graph_line_sausage    ( 300, 240, 407, 370, IMG, NULL );

    BLK_Img888_out_bmp("/Users/randle_h/Desktop/screen.bmp", IMG);
    BLK_Img888_free(IMG);
    
    return 0;
}
