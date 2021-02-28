#include "RH_graphic.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef __GraphPixel_t (*func_ApplyPixelMethod)  (int x, int y , __GraphPixel_t param  , __GraphInfo_t* pInfo);
static  __GraphPixel_t __ApplyPixel_mark         (int x, int y , __GraphPixel_t nan    , __GraphInfo_t* pInfo);
static  __GraphPixel_t __ApplyPixel_unmark       (int x, int y , __GraphPixel_t nan    , __GraphInfo_t* pInfo);
static  __GraphPixel_t __ApplyPixel_fill         (int x, int y , __GraphPixel_t color  , __GraphInfo_t* pInfo);
static  __GraphPixel_t __ApplyPixel_lightness    (int x, int y , __GraphPixel_t br_100 , __GraphInfo_t* pInfo);
static  __GraphPixel_t __ApplyPixel_unzero       (int x, int y , __GraphPixel_t color  , __GraphInfo_t* pInfo);
static  __GraphPixel_t __ApplyPixel_cpblur       (int x, int y , __GraphPixel_t nan    , __GraphInfo_t* pInfo);
static  __GraphPixel_t __ApplyPixel_reverse      (int x, int y , __GraphPixel_t color  , __GraphInfo_t* pInfo);

static const func_ApplyPixelMethod applyPixelMethod[] = {
    [ kApplyPixel_mark    ] = __ApplyPixel_mark      ,
    [ kApplyPixel_unmark  ] = __ApplyPixel_unmark    ,
    [ kApplyPixel_fill    ] = __ApplyPixel_fill      ,
    [ kApplyPixel_light   ] = __ApplyPixel_lightness ,
    [ kApplyPixel_depix   ] = __ApplyPixel_unzero    ,
    [ kApplyPixel_reverse ] = __ApplyPixel_reverse   ,
    [ kApplyPixel_pick    ] = NULL      ,
    [ kApplyPixel_blur    ] = __ApplyPixel_cpblur
};

struct __GraphConfig_t{
    __GraphPixel_t  penColor;
    size_t          penSize;
    
    unsigned int    blur_br_100;
    unsigned int    blur_size;
    __GraphInfo_t   blur_tmp;
    __Area_t        blur_area;
};
    
static struct __GraphConfig_t GCFG = {
    .penColor    = M_COLOR_WHITE ,
    .penSize     = 3             ,
    .blur_size   = 44100         ,
    .blur_br_100 = 70
};
    
/*===========================================
 > 在指定缓存区,标记一个点
============================================*/
static __GraphPixel_t __ApplyPixel_mark      (int x,int y,__GraphPixel_t nan   ,__GraphInfo_t* pInfo){
    __GraphPixel_t* p = (__GraphPixel_t*)(pInfo->pBuffer);
    size_t width      = pInfo->width;
    size_t height     = pInfo->height;

    __exitReturn( x>=width || y>=height || x<0 || y<0, 0 );

    *(p+(y*width)+x) = 1;
    return 0;
}

/*===========================================
 > 在指定缓存区,去除标记一个点
============================================*/
static __GraphPixel_t __ApplyPixel_unmark    (int x,int y,__GraphPixel_t nan   ,__GraphInfo_t* pInfo){
    __GraphPixel_t* p = (__GraphPixel_t*)(pInfo->pBuffer);
    size_t width      = pInfo->width;
    size_t height     = pInfo->height;

    __exitReturn( x>=width || y>=height || x<0 || y<0, 1 );

    *(p+(y*width)+x) = 0;
    return 0;
}

/*===========================================
 > 在指定缓存区,填充一个像素点,颜色随设定
============================================*/
static __GraphPixel_t __ApplyPixel_fill      (int x,int y,__GraphPixel_t color ,__GraphInfo_t* pInfo){
    __GraphPixel_t* p = (__GraphPixel_t*)(pInfo->pBuffer);
    size_t width   = pInfo->width;
    size_t height  = pInfo->height;

    __exitReturn( x>=width || y>=height || x<0 || y<0 , 1 );
   *(p+y*width+x) = color;

    return 0;
}

/*===========================================
 > 在指定缓冲区,如果为0,则填充像素
============================================*/
static __GraphPixel_t __ApplyPixel_unzero    (int x,int y,__GraphPixel_t color ,__GraphInfo_t* pInfo){
    __GraphPixel_t* p = (__GraphPixel_t*)(pInfo->pBuffer);
    size_t width   = pInfo->width;
    size_t height  = pInfo->height;

    __exitReturn( x>=width || y>=height || x<0 || y<0 , color );
    
    if( *(p+(y*height)+x) == 0){
        *(p+(y*height)+x) = color;
    }
    return 0;
}

/*===========================================
 > 在主屏显存区调节像素的亮度 「耦合」
============================================*/
static __GraphPixel_t __ApplyPixel_lightness (int x,int y,__GraphPixel_t br_100,__GraphInfo_t* pInfo){
    __GraphPixel_t* p = (__GraphPixel_t*)(pInfo->pBuffer);
    size_t width   = pInfo->width;
    size_t height  = pInfo->height;
    __exitReturn( x>=width || y>=height || x<0 || y<0 , br_100);
    
#if   ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_BIN    )
    #error "Not support"
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB565 )
    __UNION_PixelRGB565_t temp = {
        .data = *(p+y*width+x)
    };
    temp.R = __limit((signed)(temp.R*br_100/100) , 0 , ((1<<5)-1));
    temp.G = __limit((signed)(temp.G*br_100/100) , 0 , ((1<<6)-1));
    temp.B = __limit((signed)(temp.B*br_100/100) , 0 , ((1<<5)-1));
    
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB888 )
    __UNION_PixelRGB888_t temp = {
        .data = *(p+y*width+x)
    };
    temp.R = __limit((signed)(temp.R*br_100/100) , 0 , ((1<<8)-1));
    temp.G = __limit((signed)(temp.G*br_100/100) , 0 , ((1<<8)-1));
    temp.B = __limit((signed)(temp.B*br_100/100) , 0 , ((1<<8)-1));
#endif
    *(p+y*width+x) = temp.data;
    return 0;
}

/*===========================================
 > 在指定缓冲区,取反一个像素颜色
============================================*/
static __GraphPixel_t __ApplyPixel_reverse   (int x,int y,__GraphPixel_t br_100,__GraphInfo_t* pInfo){
    __GraphPixel_t* p = (__GraphPixel_t*)(pInfo->pBuffer);
    size_t width      = pInfo->width;
    size_t height     = pInfo->height;
    __exitReturn( x>=width || y>=height || x<0 || y<0 , 0);
    
#if   ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_BIN    )
    #error "Not support"
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB565 )
    __UNION_PixelRGB565_t temp = {.data = *(p+y*width+x)};
        temp.B = 31 - temp.B;
        temp.G = 63 - temp.G;
        temp.R = 31 - temp.R;
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB888 )
    __UNION_PixelRGB888_t temp = {.data = *(p+y*width+x)};
        temp.B = 0xff - temp.B;
        temp.G = 0xff - temp.G;
        temp.R = 0xff - temp.R;
#endif
    *(p+y*width+x) = temp.data;
    return 0;
}

    
static  __GraphPixel_t __ApplyPixel_cpblur   (int x,int y,__GraphPixel_t nan   ,__GraphInfo_t* pInfo){
    __GraphPixel_t* p = (__GraphPixel_t*)(pInfo->pBuffer);
    size_t width      = pInfo->width;
    size_t height     = pInfo->height;
    __exitReturn( x>=width || y>=height || x<0 || y<0 , 0);
    
    *(p+y*width+x) = GCFG.blur_tmp.pBuffer[ GCFG.blur_tmp.width*(y-GCFG.blur_area.ys)+x-GCFG.blur_area.xs ].data;
    return 0;
}


    
E_Status_t MAKE_FUNC( Graph , init ) (void){
    GCFG.blur_br_100 = 100;
    GCFG.blur_size   = 44100;
    GCFG.penColor    = M_COLOR_WHITE;
    GCFG.penSize     = 3;
    
    GCFG.blur_tmp.pBuffer = NULL;
    GCFG.blur_tmp.height  = 0;
    GCFG.blur_tmp.width   = 0;
    
    return kStatus_Success;
}
 
E_Status_t MAKE_FUNC( Graph , set_penSize ) (size_t penSize){
    E_Status_t state = kStatus_Success;
    if(penSize >0xff){
        penSize = 0xff;
        state = kStatus_Denied;
    }
    GCFG.penSize = penSize;
    
    return state;
}
    
E_Status_t MAKE_FUNC( Graph , set_penColor ) (__GraphPixel_t penColor){
    GCFG.penColor = penColor;
    return kStatus_Success;
}
    
E_Status_t MAKE_FUNC( Graph , set_blurSize ) (size_t size_0_65535){
    GCFG.blur_size = __limit((unsigned int)(size_0_65535), 0U, 65535U);
    return kStatus_Success;
}
    
E_Status_t MAKE_FUNC( Graph , set_blurBrightness ) (size_t br_100){
    GCFG.blur_br_100 = __limit((unsigned int)(br_100), 0, 200);
    return kStatus_Success;
}


/*===========================================
 > 插入一个空心圆,线宽为1
============================================*/
E_Status_t MAKE_FUNC( Graph , circle_raw   )  (int x ,int y ,int d ,        __GraphInfo_t* pInfo, E_ApplyPixel_t method){
    int r    = d>>1;
    int p    = 3-2*r;
    bool eps = (d%2==0);
    
    if( method == kApplyPixel_blur ){
        GCFG.blur_area.xs                            = __limit( (signed)(x-(d>>1)-1) , 0 , (int)(pInfo->width ));
        GCFG.blur_area.ys                            = __limit( (signed)(y-(d>>1)-1) , 0 , (int)(pInfo->height));
        GCFG.blur_area.width  = GCFG.blur_tmp.width  = __limit( (signed)(x+(d>>1)-1) , 0 , (int)(pInfo->width )) - GCFG.blur_area.xs +1;
        GCFG.blur_area.height = GCFG.blur_tmp.height = __limit( (signed)(y+(d>>1)+1) , 0 , (int)(pInfo->height)) - GCFG.blur_area.ys +1;
        GCFG.blur_tmp.pBuffer = __malloc( GCFG.blur_area.width*GCFG.blur_area.height*sizeof(__GraphPixel_t) );
#if   ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_BIN    )
        while(1);
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB565 )
        while(1);
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB888 )
        __Blur_Average_ImgRGB888(pInfo, &GCFG.blur_tmp, &GCFG.blur_area, GCFG.blur_size, GCFG.blur_br_100);
#else
  #error "[RH_graphic]: Unknown color type."
#endif
    }
    
    for(int x_tmp=0,y_tmp = r;x_tmp<=y_tmp;x_tmp++){
            ( *applyPixelMethod [method] )(x     + x_tmp ,y     + y_tmp ,GCFG.penColor,pInfo );
            ( *applyPixelMethod [method] )(x+eps - x_tmp ,y     + y_tmp ,GCFG.penColor,pInfo );
            ( *applyPixelMethod [method] )(x     + x_tmp ,y+eps - y_tmp ,GCFG.penColor,pInfo );
            ( *applyPixelMethod [method] )(x+eps - x_tmp ,y+eps - y_tmp ,GCFG.penColor,pInfo );
            ( *applyPixelMethod [method] )(x     + y_tmp ,y     + x_tmp ,GCFG.penColor,pInfo );
            ( *applyPixelMethod [method] )(x+eps - y_tmp ,y     + x_tmp ,GCFG.penColor,pInfo );
            ( *applyPixelMethod [method] )(x     + y_tmp ,y+eps - x_tmp ,GCFG.penColor,pInfo );
            ( *applyPixelMethod [method] )(x+eps - y_tmp ,y+eps - x_tmp ,GCFG.penColor,pInfo );

        if(p <= 0){
            p += (x_tmp<<2) + 6;
        }else{
            p += ((x_tmp-y_tmp)<<2) + 10;
            y_tmp--;
        }
    }
    
    if( method == kApplyPixel_blur ){
        __free(GCFG.blur_tmp.pBuffer);
        GCFG.blur_area.xs     = 0;
        GCFG.blur_area.ys     = 0;
        GCFG.blur_area.width  = GCFG.blur_tmp.width  = 0;
        GCFG.blur_area.height = GCFG.blur_tmp.height = 0;
        GCFG.blur_tmp.pBuffer = NULL;
    }
    
    return kStatus_Success;
}

/*====================================
 > 插入一个填充圆
=====================================*/
E_Status_t MAKE_FUNC( Graph , circle_fill  )  (int x ,int y ,int d ,        __GraphInfo_t* pInfo, E_ApplyPixel_t method){
    int r = d>>1;
    int p = 3-(r<<1);
    int x_tmp = 0,y_tmp = r;
    bool eps  = (d%2==0);
    
    if( method == kApplyPixel_blur ){
        GCFG.blur_area.xs                            = __limit( (signed)(x-(d>>1)-1) , 0 , (int)(pInfo->width ));
        GCFG.blur_area.ys                            = __limit( (signed)(y-(d>>1)-1) , 0 , (int)(pInfo->height));
        GCFG.blur_area.width  = GCFG.blur_tmp.width  = __limit( (signed)(x+(d>>1)-1) , 0 , (int)(pInfo->width )) - GCFG.blur_area.xs +1;
        GCFG.blur_area.height = GCFG.blur_tmp.height = __limit( (signed)(y+(d>>1)+1) , 0 , (int)(pInfo->height)) - GCFG.blur_area.ys +1;
        GCFG.blur_tmp.pBuffer = __malloc( GCFG.blur_area.width*GCFG.blur_area.height*sizeof(__GraphPixel_t) );
#if   ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_BIN    )
        while(1);
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB565 )
        while(1);
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB888 )
        __Blur_Average_ImgRGB888(pInfo, &GCFG.blur_tmp, &GCFG.blur_area, GCFG.blur_size, GCFG.blur_br_100);
#else
  #error "[RH_graphic]: Unknown color type."
#endif
    }
    
    for(;x_tmp<=y_tmp;x_tmp++){
        int cnt = y_tmp+1;
        while(cnt--){
            ( *applyPixelMethod [method] )(x     + x_tmp ,y     + cnt,GCFG.penColor,pInfo );
            ( *applyPixelMethod [method] )(x+eps - x_tmp ,y     + cnt,GCFG.penColor,pInfo );
            ( *applyPixelMethod [method] )(x     + x_tmp ,y+eps - cnt,GCFG.penColor,pInfo );
            ( *applyPixelMethod [method] )(x+eps - x_tmp ,y+eps - cnt,GCFG.penColor,pInfo );
        }

        cnt = x_tmp+1;
        while(cnt--){
            ( *applyPixelMethod [method] )(x     + y_tmp ,y     + cnt,GCFG.penColor,pInfo );
            ( *applyPixelMethod [method] )(x+eps - y_tmp ,y     + cnt,GCFG.penColor,pInfo );
            ( *applyPixelMethod [method] )(x     + y_tmp ,y+eps - cnt,GCFG.penColor,pInfo );
            ( *applyPixelMethod [method] )(x+eps - y_tmp ,y+eps - cnt,GCFG.penColor,pInfo );
        }

        if(p <= 0){
            p += (x_tmp<<2) + 6;
        }else{
            p += ((x_tmp-y_tmp)<<2) + 10;
            y_tmp--;
        }
    }
    
    if( method == kApplyPixel_blur ){
        __free(GCFG.blur_tmp.pBuffer);
        GCFG.blur_area.xs     = 0;
        GCFG.blur_area.ys     = 0;
        GCFG.blur_area.width  = GCFG.blur_tmp.width  = 0;
        GCFG.blur_area.height = GCFG.blur_tmp.height = 0;
        GCFG.blur_tmp.pBuffer = NULL;
    }
    return kStatus_Success;
}

/*====================================
 > 插入一个空心圆,线宽随设定
=====================================*/
E_Status_t MAKE_FUNC( Graph , circle_edged )  (int x ,int y ,int d ,        __GraphInfo_t* pInfo, E_ApplyPixel_t method){
    int r = d>>1;
    int r_ex  = r;
    int r_in  = (int)(r-GCFG.penSize);
    
    int x_ex_tmp = 0;
    int y_ex_tmp = r_ex;
    int x_in_tmp = 0;
    int y_in_tmp = r_in;
    int p_ex  = 3-2*r_ex;
    int p_in  = 3-2*r_in;
    
    bool eps  = (d%2==0);
    
    if( method == kApplyPixel_blur ){
        GCFG.blur_area.xs                            = __limit( (signed)(x-(d>>1)-1) , 0 , (int)(pInfo->width ));
        GCFG.blur_area.ys                            = __limit( (signed)(y-(d>>1)-1) , 0 , (int)(pInfo->height));
        GCFG.blur_area.width  = GCFG.blur_tmp.width  = __limit( (signed)(x+(d>>1)-1) , 0 , (int)(pInfo->width )) - GCFG.blur_area.xs +1;
        GCFG.blur_area.height = GCFG.blur_tmp.height = __limit( (signed)(y+(d>>1)+1) , 0 , (int)(pInfo->height)) - GCFG.blur_area.ys +1;
        GCFG.blur_tmp.pBuffer = __malloc( GCFG.blur_area.width*GCFG.blur_area.height*sizeof(__GraphPixel_t) );
#if   ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_BIN    )
        while(1);
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB565 )
        while(1);
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB888 )
        __Blur_Average_ImgRGB888(pInfo, &GCFG.blur_tmp, &GCFG.blur_area, GCFG.blur_size, GCFG.blur_br_100);
#else
  #error "[RH_graphic]: Unknown color type."
#endif
    }
    
    for(;x_ex_tmp<y_ex_tmp;x_ex_tmp++,x_in_tmp++){
        for(int Y = y_in_tmp;Y<y_ex_tmp;Y++){
            ( *applyPixelMethod [method] )(x     + x_ex_tmp ,y     + Y        ,GCFG.penColor,pInfo );
            ( *applyPixelMethod [method] )(x+eps - x_ex_tmp ,y     + Y        ,GCFG.penColor,pInfo );
            ( *applyPixelMethod [method] )(x     + x_ex_tmp ,y+eps - Y        ,GCFG.penColor,pInfo );
            ( *applyPixelMethod [method] )(x+eps - x_ex_tmp ,y+eps - Y        ,GCFG.penColor,pInfo );
            ( *applyPixelMethod [method] )(x     + Y        ,y     + x_ex_tmp ,GCFG.penColor,pInfo );
            ( *applyPixelMethod [method] )(x+eps - Y        ,y     + x_ex_tmp ,GCFG.penColor,pInfo );
            ( *applyPixelMethod [method] )(x     + Y        ,y+eps - x_ex_tmp ,GCFG.penColor,pInfo );
            ( *applyPixelMethod [method] )(x+eps - Y        ,y+eps - x_ex_tmp ,GCFG.penColor,pInfo );
        }

        if(p_ex <= 0){
            p_ex += (x_ex_tmp<<2) + 6;
        }else{
            p_ex += ((x_ex_tmp-y_ex_tmp)<<2) + 10;
            y_ex_tmp--;
        }
        if(p_in <= 0){
            p_in += (x_in_tmp<<2) + 6;
        }else{
            p_in += ((x_in_tmp-y_in_tmp)<<2) + 10;
            y_in_tmp--;
        }
    }
    
    if( method == kApplyPixel_blur ){
        __free(GCFG.blur_tmp.pBuffer);
        GCFG.blur_area.xs     = 0;
        GCFG.blur_area.ys     = 0;
        GCFG.blur_area.width  = GCFG.blur_tmp.width  = 0;
        GCFG.blur_area.height = GCFG.blur_tmp.height = 0;
        GCFG.blur_tmp.pBuffer = NULL;
    }
    return kStatus_Success;
}

/*====================================
 > 插入一个空心长发形,线宽为1
=====================================*/
E_Status_t MAKE_FUNC( Graph , rect_raw     )  (int xs,int ys,int xe,int ye, __GraphInfo_t* pInfo, E_ApplyPixel_t method){
    if( method == kApplyPixel_blur ){
        while(1);
    }
    for(int x=xs;x<=xe;x++){
        ( *applyPixelMethod [method] )(x,ys,GCFG.penColor,pInfo);
        ( *applyPixelMethod [method] )(x,ye,GCFG.penColor,pInfo);
    }
    for(int y=ys+1;y<ye;y++){
        ( *applyPixelMethod [method] )(xs,y,GCFG.penColor,pInfo);
        ( *applyPixelMethod [method] )(xe,y,GCFG.penColor,pInfo);
    }
    
    if( method == kApplyPixel_blur ){
        while(1);
    }
    return kStatus_Success;
}

/*====================================
 > 插入一个填充长发形
=====================================*/
E_Status_t MAKE_FUNC( Graph , rect_fill    )  (int xs,int ys,int xe,int ye, __GraphInfo_t* pInfo, E_ApplyPixel_t method){
    
    switch(method){
        case kApplyPixel_fill:
        case kApplyPixel_mark:
        case kApplyPixel_unmark:
            for(int x = xs;x <= xe;x++)
                ( *applyPixelMethod [method] )(x,ys,GCFG.penColor,pInfo);
            for(int y = ys+1;y <= ye;y++)
                memcpy((pInfo->pBuffer + y  * pInfo->width + xs),\
                       (pInfo->pBuffer + ys * pInfo->width + xs),\
                       ((xe-xs+1)*sizeof(pInfo->pBuffer[0])) );
            break;
        case kApplyPixel_blur:
        {
            GCFG.blur_area.xs                            = __limit( (signed)(xs) , 0 , (int)(pInfo->width ));
            GCFG.blur_area.ys                            = __limit( (signed)(ys) , 0 , (int)(pInfo->height));
            GCFG.blur_area.width  = GCFG.blur_tmp.width  = __limit( (signed)(xe) , 0 , (int)(pInfo->width )) - GCFG.blur_area.xs +1;
            GCFG.blur_area.height = GCFG.blur_tmp.height = __limit( (signed)(ye) , 0 , (int)(pInfo->height)) - GCFG.blur_area.ys +1;
            GCFG.blur_tmp.pBuffer = __malloc( GCFG.blur_area.width*GCFG.blur_area.height*sizeof(__GraphPixel_t) );
#if   ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_BIN    )
            while(1);
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB565 )
            while(1);
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB888 )
            __Blur_Average_ImgRGB888(pInfo, &GCFG.blur_tmp, &GCFG.blur_area, GCFG.blur_size, GCFG.blur_br_100);
#else
  #error "[RH_graphic]: Unknown color type."
#endif
            

            for(int y = ys;y <= ye;y++)
                memcpy( pInfo->pBuffer       + y    *pInfo->width  + xs  ,\
                        GCFG.blur_tmp.pBuffer+(y-ys)*GCFG.blur_tmp.width ,\
                        ((xe-xs+1)*sizeof(pInfo->pBuffer[0]))        );
            GCFG.blur_area.xs     = 0;
            GCFG.blur_area.ys     = 0;
            GCFG.blur_tmp.height  = GCFG.blur_area.height = 0;
            GCFG.blur_tmp.width   = GCFG.blur_area.width  = 0;
            __free(GCFG.blur_tmp.pBuffer);
            GCFG.blur_tmp.pBuffer = NULL;
        }
            break;
        default:
            while(1);
            break;
    }
    
    return kStatus_Success;
}

/*====================================
 > 插入一个空心长发形,线宽随设定
=====================================*/
E_Status_t MAKE_FUNC( Graph , rect_edged   )  (int xs,int ys,int xe,int ye, __GraphInfo_t* pInfo, E_ApplyPixel_t method){
    int loop = 0;
    if(method == kApplyPixel_blur){
        GCFG.blur_area.xs                            = __limit( (signed)(xs) , 0 , (int)(pInfo->width ));
        GCFG.blur_area.ys                            = __limit( (signed)(ys) , 0 , (int)(pInfo->height));
        GCFG.blur_area.width  = GCFG.blur_tmp.width  = __limit( (signed)(xe) , 0 , (int)(pInfo->width )) - GCFG.blur_area.xs +1;
        GCFG.blur_area.height = GCFG.blur_tmp.height = __limit( (signed)(ye) , 0 , (int)(pInfo->height)) - GCFG.blur_area.ys +1;
        GCFG.blur_tmp.pBuffer = __malloc( GCFG.blur_area.width*GCFG.blur_area.height*sizeof(__GraphPixel_t) );
#if   ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_BIN    )
            while(1);
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB565 )
            while(1);
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB888 )
            __Blur_Average_ImgRGB888(pInfo, &GCFG.blur_tmp, &GCFG.blur_area, GCFG.blur_size, GCFG.blur_br_100);
#else
  #error "[RH_graphic]: Unknown color type."
#endif
        
    }
    
    while(loop < GCFG.penSize){
        int x = xs+loop, y = ys+loop;

        while(x<=(xe-loop) && y<=(ye-loop)){
            if( x < (xe-loop) ){
                ( *applyPixelMethod [method] )(x,ys + loop, GCFG.penColor ,pInfo );
                ( *applyPixelMethod [method] )(x,ye - loop, GCFG.penColor ,pInfo );
                x++;
            }
            else{
                ( *applyPixelMethod [method] )(xs + loop,y, GCFG.penColor ,pInfo );
                ( *applyPixelMethod [method] )(xe - loop,y, GCFG.penColor ,pInfo );
                y++;
            }
        }
        loop++;
    }
    if(method == kApplyPixel_blur){
        GCFG.blur_area.xs     = 0;
        GCFG.blur_area.ys     = 0;
        GCFG.blur_tmp.height  = GCFG.blur_area.height = 0;
        GCFG.blur_tmp.width   = GCFG.blur_area.width  = 0;
        __free(GCFG.blur_tmp.pBuffer);
        GCFG.blur_tmp.pBuffer = NULL;
    }
    return kStatus_Success;
}
    
/*====================================
 > 插入直线，线宽为1
=====================================*/
E_Status_t MAKE_FUNC( Graph , line_raw     )  (int x1,int y1,int x2,int y2, __GraphInfo_t* pInfo, E_ApplyPixel_t method){
    int x_min = (int)(__min(x1,x2));
    int x_max = (int)(__max(x1,x2));
    int y_min = (int)(__min(y1,y2));
    int y_max = (int)(__max(y1,y2));
    int delta_x    = (int)( x_max - x_min );
    int delta_y    = (int)( y_max - y_min );
    
    int type = (int)(( ((y2-y1)*(x2-x1)<0) << 1 ) | (delta_y > delta_x));

    if(delta_y > delta_x){
        int temp = delta_x;
        delta_x = delta_y;
        delta_y = temp;
    }
    
    int j = 0;
    int e = 0;
    
    if(method == kApplyPixel_blur){
        while(1);
    }
    
    ( *applyPixelMethod [method] )(x1,y1,GCFG.penColor,pInfo);
    ( *applyPixelMethod [method] )(x2,y2,GCFG.penColor,pInfo);
    for(int i = 0;i < delta_x;i++){
        switch(type){
            case 0:( *applyPixelMethod [method] )(x_min+i,y_min+j,GCFG.penColor,pInfo);break;
            case 1:( *applyPixelMethod [method] )(x_min+j,y_min+i,GCFG.penColor,pInfo);break;
            case 2:( *applyPixelMethod [method] )(x_min+i,y_max-j,GCFG.penColor,pInfo);break;
            case 3:( *applyPixelMethod [method] )(x_min+j,y_max-i,GCFG.penColor,pInfo);break;
        }
        e += delta_y;
        if( 2*( e + delta_y ) > delta_x){
            j++;
            e -= delta_x;
        }
    }
    
    if(method == kApplyPixel_blur){
        while(1);
    }
    return kStatus_Success;
}
   
    
E_Status_t MAKE_FUNC( Graph , line_edged   )  (int x1,int y1,int x2,int y2, __GraphInfo_t* pInfo, E_ApplyPixel_t method){
    return kStatus_Denied;
}
    
#ifdef __cplusplus
}
#endif

