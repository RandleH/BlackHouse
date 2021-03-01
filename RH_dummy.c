
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

__Blur_Average_ImgRGB888(&in,&out,&AREA,10,100);

#if   ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_BIN    )
            while(1);
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB565 )
            while(1);
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB888 )
            __Blur_Average_ImgRGB888(pInfo, &GCFG.blur_tmp, &GCFG.blur_area, GCFG.blur_size, GCFG.blur_br_100);
#else
  #error "[RH_graphic]: Unknown color type."
#endif

const char* __restrict__ src  = "/Users/randle_h/desktop/lenna.bmp";
const char* __restrict__ des  = "/Users/randle_h/desktop/lenna_.bmp";

__ImageRGB888_t* IMG_IN  = __LoadBMP_ImgRGB888(src);
__Area_t         AREA    = {
    .xs = 1,
    .ys = 1,
    .width  = 300,
    .height = 300
};
__ImageRGB888_t* IMG_OUT = __Create_ImgRGB888(AREA.width, AREA.height);


__Blur_Average_ImgRGB888(IMG_IN,IMG_OUT,&AREA,40000,100);
__OutBMP_ImgRGB888(des, IMG_OUT);



( *applyPixelMethod [method] )(x1,y1,GCFG.penColor,pInfo);

#endif



