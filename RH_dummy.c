
#if 0==1

__UNION_PixelRGB888_t in_[6][6] = {
    {{.R =  90},{.R = 100},{.R = 100},{.R =  90},{.R = 100},{.R = 100}},
    {{.R = 100},{.R = 120},{.R =  80},{.R = 100},{.R = 120},{.R = 100}},
    {{.R = 100},{.R =  20},{.R = 100},{.R = 100},{.R = 120},{.R = 100}},
    {{.R =  90},{.R = 100},{.R = 100},{.R =  90},{.R = 100},{.R = 100}},
    {{.R = 100},{.R = 120},{.R =  70},{.R = 100},{.R = 120},{.R = 100}},
    {{.R = 100},{.R =  20},{.R = 100},{.R = 100},{.R = 120},{.R = 100}},
};
__UNION_PixelRGB888_t out_[3][3] = {0};

__ImageRGB888_t in = {
    .pBuffer = in_[0],
    .width   = 6,
    .height  = 6
};

__ImageRGB888_t out = {
    .pBuffer = out_[0],
    .width   = 3,
    .height  = 3
};
__Area_t AREA = {
    .xs = 2,
    .ys = 2,
    .width  = 3,
    .height = 3
};


( *applyPixelMethod [method] )(x1,y1,GCFG.penColor,pInfo);

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
    __Graph_quad_fill(90, 90, 80, 300, 300, 70, 500, 400, &BufferInfo, kApplyPixel_fill);

    __ImgRGB888_out_bmp("/Users/randle_h/desktop/screen.bmp",&BufferInfo);
    
    return 0;
}


#endif



