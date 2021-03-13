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
    [ kApplyPixel_pick    ] = NULL                   ,
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


     
E_Status_t __Graph_init                (void){
    GCFG.blur_br_100 = 100;
    GCFG.blur_size   = 44100;
    GCFG.penColor    = M_COLOR_WHITE;
    GCFG.penSize     = 3;
    
    GCFG.blur_tmp.pBuffer = NULL;
    GCFG.blur_tmp.height  = 0;
    GCFG.blur_tmp.width   = 0;
    
    return kStatus_Success;
}
 
void __Graph_set_penSize         (size_t         penSize      ){
    if(penSize >0xff){
        penSize = 0xff;
    }
    GCFG.penSize = penSize;
}
void __Graph_set_penColor        (__GraphPixel_t penColor     ){
    GCFG.penColor = penColor;
}
void __Graph_set_blurSize        (size_t         size_0_65535 ){
    GCFG.blur_size = __limit((unsigned int)(size_0_65535), 0U, 65535U);
}
void __Graph_set_blurBrightness  (size_t         br_100       ){
    GCFG.blur_br_100 = __limit((unsigned int)(br_100), 0, 200);
}

__GraphPixel_t __Graph_get_penColor       (void){
    return GCFG.penColor;
}
size_t         __Graph_get_penSize        (void){
    return GCFG.penSize;
}
size_t         __Graph_get_blurSize       (void){
    return GCFG.blur_size;
}
size_t         __Graph_get_blurBrightness (void){
    return GCFG.blur_br_100;
}
    
    
    
/*===========================================
 > 插入一个空心圆,线宽为1
============================================*/
E_Status_t __Graph_circle_raw     (int x ,int y ,int d ,        __GraphInfo_t* pInfo, E_ApplyPixel_t method){
    int r    = d>>1;
    int p    = 3-2*r;
    bool eps = (d%2==0);
    
    bool blurCmd = ( method == kApplyPixel_blur && GCFG.blur_tmp.pBuffer==NULL );
    
    if( blurCmd ){
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
        __ImgRGB888_blur_average(pInfo, &GCFG.blur_tmp, &GCFG.blur_area, GCFG.blur_size, GCFG.blur_br_100);
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
    
    if( blurCmd ){
        __free(GCFG.blur_tmp.pBuffer);
        memset(&GCFG.blur_area , 0, sizeof(GCFG.blur_area));
        memset(&GCFG.blur_tmp  , 0, sizeof(GCFG.blur_tmp ));
    }
    
    return kStatus_Success;
}

/*====================================
 > 插入一个填充圆
=====================================*/
E_Status_t __Graph_circle_fill    (int x ,int y ,int d ,        __GraphInfo_t* pInfo, E_ApplyPixel_t method){
    int r = d>>1;
    int p = 3-(r<<1);
    int x_tmp = 0,y_tmp = r;
    bool eps  = (d%2==0);
    
    bool blurCmd = ( method == kApplyPixel_blur && GCFG.blur_tmp.pBuffer==NULL );
    if( blurCmd ){
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
        __ImgRGB888_blur_average(pInfo, &GCFG.blur_tmp, &GCFG.blur_area, GCFG.blur_size, GCFG.blur_br_100);
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
    
    if( blurCmd ){
        __free(GCFG.blur_tmp.pBuffer);
        memset(&GCFG.blur_area , 0, sizeof(GCFG.blur_area));
        memset(&GCFG.blur_tmp  , 0, sizeof(GCFG.blur_tmp ));
    }
    return kStatus_Success;
}

/*====================================
 > 插入一个空心圆,线宽随设定
=====================================*/
E_Status_t __Graph_circle_edged   (int x ,int y ,int d ,        __GraphInfo_t* pInfo, E_ApplyPixel_t method){
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
    
    bool blurCmd = ( method == kApplyPixel_blur && GCFG.blur_tmp.pBuffer==NULL );
    if( blurCmd ){
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
        __ImgRGB888_blur_average(pInfo, &GCFG.blur_tmp, &GCFG.blur_area, GCFG.blur_size, GCFG.blur_br_100);
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
    
    if( blurCmd ){
        __free(GCFG.blur_tmp.pBuffer);
        memset(&GCFG.blur_area , 0, sizeof(GCFG.blur_area));
        memset(&GCFG.blur_tmp  , 0, sizeof(GCFG.blur_tmp ));
    }
    return kStatus_Success;
}

/*====================================
 > 插入一个1/4圆
=====================================*/
E_Status_t __Graph_circle_qrt1    (int x ,int y ,int r ,        __GraphInfo_t* pInfo, E_ApplyPixel_t method){
    int p = 3-((r-1)<<1);
    int x_tmp = 0,y_tmp = (r-1);
    
    bool blurCmd = ( method == kApplyPixel_blur && GCFG.blur_tmp.pBuffer==NULL );
    if( blurCmd ){
        GCFG.blur_area.xs                            = __limit( (signed)(x-r-1) , 0 , (int)(pInfo->width ));
        GCFG.blur_area.ys                            = __limit( (signed)(y-r-1) , 0 , (int)(pInfo->height));
        GCFG.blur_area.width  = GCFG.blur_tmp.width  = __limit( (signed)(x+r-1) , 0 , (int)(pInfo->width )) - GCFG.blur_area.xs +1;
        GCFG.blur_area.height = GCFG.blur_tmp.height = __limit( (signed)(y+r+1) , 0 , (int)(pInfo->height)) - GCFG.blur_area.ys +1;
        GCFG.blur_tmp.pBuffer = __malloc( GCFG.blur_area.width*GCFG.blur_area.height*sizeof(__GraphPixel_t) );
#if   ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_BIN    )
        while(1);
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB565 )
        while(1);
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB888 )
        __ImgRGB888_blur_average(pInfo, &GCFG.blur_tmp, &GCFG.blur_area, GCFG.blur_size, GCFG.blur_br_100);
#else
  #error "[RH_graphic]: Unknown color type."
#endif
    }

    for(;x_tmp<=y_tmp;x_tmp++){
        int cnt = y_tmp+1;
        while(cnt--){
            ( *applyPixelMethod [method] )(x+x_tmp,y-cnt, GCFG.penColor, pInfo );
        }

        cnt = x_tmp+1;
        while(cnt--){
            ( *applyPixelMethod [method] )(x+y_tmp,y-cnt, GCFG.penColor, pInfo );
        }

        if(p <= 0){
            p += (x_tmp<<2) + 6;
        }else{
            p += ((x_tmp-y_tmp)<<2) + 10;
            y_tmp--;
        }
    }
    
    if( blurCmd ){
        __free(GCFG.blur_tmp.pBuffer);
        memset(&GCFG.blur_area , 0, sizeof(GCFG.blur_area));
        memset(&GCFG.blur_tmp  , 0, sizeof(GCFG.blur_tmp ));
    }
    return kStatus_Success;
}
E_Status_t __Graph_circle_qrt2    (int x ,int y ,int r ,        __GraphInfo_t* pInfo, E_ApplyPixel_t method){
    int p = 3-((r-1)<<1);
    int x_tmp = 0,y_tmp = (r-1);
    
    bool blurCmd = ( method == kApplyPixel_blur && GCFG.blur_tmp.pBuffer==NULL );
    if( blurCmd ){
        GCFG.blur_area.xs                            = __limit( (signed)(x-r-1) , 0 , (int)(pInfo->width ));
        GCFG.blur_area.ys                            = __limit( (signed)(y-r-1) , 0 , (int)(pInfo->height));
        GCFG.blur_area.width  = GCFG.blur_tmp.width  = __limit( (signed)(x+r-1) , 0 , (int)(pInfo->width )) - GCFG.blur_area.xs +1;
        GCFG.blur_area.height = GCFG.blur_tmp.height = __limit( (signed)(y+r+1) , 0 , (int)(pInfo->height)) - GCFG.blur_area.ys +1;
        GCFG.blur_tmp.pBuffer = __malloc( GCFG.blur_area.width*GCFG.blur_area.height*sizeof(__GraphPixel_t) );
#if   ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_BIN    )
        while(1);
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB565 )
        while(1);
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB888 )
        __ImgRGB888_blur_average(pInfo, &GCFG.blur_tmp, &GCFG.blur_area, GCFG.blur_size, GCFG.blur_br_100);
#else
  #error "[RH_graphic]: Unknown color type."
#endif
    }

    for(;x_tmp<=y_tmp;x_tmp++){
        int cnt = y_tmp+1;
        while(cnt--){
            ( *applyPixelMethod [method] )(x-x_tmp,y-cnt, GCFG.penColor, pInfo );
        }

        cnt = x_tmp+1;
        while(cnt--){
            ( *applyPixelMethod [method] )(x-y_tmp,y-cnt, GCFG.penColor, pInfo );
        }

        if(p <= 0){
            p += (x_tmp<<2) + 6;
        }else{
            p += ((x_tmp-y_tmp)<<2) + 10;
            y_tmp--;
        }
    }
    
    if( blurCmd ){
        __free(GCFG.blur_tmp.pBuffer);
        memset(&GCFG.blur_area , 0, sizeof(GCFG.blur_area));
        memset(&GCFG.blur_tmp  , 0, sizeof(GCFG.blur_tmp ));
    }
    return kStatus_Success;
}
E_Status_t __Graph_circle_qrt3    (int x ,int y ,int r ,        __GraphInfo_t* pInfo, E_ApplyPixel_t method){
    int p = 3-((r-1)<<1);
    int x_tmp = 0,y_tmp = (r-1);
    
    bool blurCmd = ( method == kApplyPixel_blur && GCFG.blur_tmp.pBuffer==NULL );
    if( blurCmd ){
        GCFG.blur_area.xs                            = __limit( (signed)(x-r-1) , 0 , (int)(pInfo->width ));
        GCFG.blur_area.ys                            = __limit( (signed)(y-r-1) , 0 , (int)(pInfo->height));
        GCFG.blur_area.width  = GCFG.blur_tmp.width  = __limit( (signed)(x+r-1) , 0 , (int)(pInfo->width )) - GCFG.blur_area.xs +1;
        GCFG.blur_area.height = GCFG.blur_tmp.height = __limit( (signed)(y+r+1) , 0 , (int)(pInfo->height)) - GCFG.blur_area.ys +1;
        GCFG.blur_tmp.pBuffer = __malloc( GCFG.blur_area.width*GCFG.blur_area.height*sizeof(__GraphPixel_t) );
#if   ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_BIN    )
        while(1);
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB565 )
        while(1);
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB888 )
        __ImgRGB888_blur_average(pInfo, &GCFG.blur_tmp, &GCFG.blur_area, GCFG.blur_size, GCFG.blur_br_100);
#else
  #error "[RH_graphic]: Unknown color type."
#endif
    }

    for(;x_tmp<=y_tmp;x_tmp++){
        int cnt = y_tmp+1;
        while(cnt--){
            ( *applyPixelMethod [method] )(x-x_tmp,y+cnt, GCFG.penColor, pInfo );
        }

        cnt = x_tmp+1;
        while(cnt--){
            ( *applyPixelMethod [method] )(x-y_tmp,y+cnt, GCFG.penColor, pInfo );
        }

        if(p <= 0){
            p += (x_tmp<<2) + 6;
        }else{
            p += ((x_tmp-y_tmp)<<2) + 10;
            y_tmp--;
        }
    }
    
    if( blurCmd ){
        __free(GCFG.blur_tmp.pBuffer);
        memset(&GCFG.blur_area , 0, sizeof(GCFG.blur_area));
        memset(&GCFG.blur_tmp  , 0, sizeof(GCFG.blur_tmp ));
    }
    return kStatus_Success;
}
E_Status_t __Graph_circle_qrt4    (int x ,int y ,int r ,        __GraphInfo_t* pInfo, E_ApplyPixel_t method){
    int p = 3-((r-1)<<1);
    int x_tmp = 0,y_tmp = (r-1);
    
    bool blurCmd = ( method == kApplyPixel_blur && GCFG.blur_tmp.pBuffer==NULL );
    if( blurCmd ){
        GCFG.blur_area.xs                            = __limit( (signed)(x-r-1) , 0 , (int)(pInfo->width ));
        GCFG.blur_area.ys                            = __limit( (signed)(y-r-1) , 0 , (int)(pInfo->height));
        GCFG.blur_area.width  = GCFG.blur_tmp.width  = __limit( (signed)(x+r-1) , 0 , (int)(pInfo->width )) - GCFG.blur_area.xs +1;
        GCFG.blur_area.height = GCFG.blur_tmp.height = __limit( (signed)(y+r+1) , 0 , (int)(pInfo->height)) - GCFG.blur_area.ys +1;
        GCFG.blur_tmp.pBuffer = __malloc( GCFG.blur_area.width*GCFG.blur_area.height*sizeof(__GraphPixel_t) );
#if   ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_BIN    )
        while(1);
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB565 )
        while(1);
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB888 )
        __ImgRGB888_blur_average(pInfo, &GCFG.blur_tmp, &GCFG.blur_area, GCFG.blur_size, GCFG.blur_br_100);
#else
  #error "[RH_graphic]: Unknown color type."
#endif
    }

    for(;x_tmp<=y_tmp;x_tmp++){
        int cnt = y_tmp+1;
        while(cnt--){
            ( *applyPixelMethod [method] )(x+x_tmp,y+cnt, GCFG.penColor, pInfo );
        }

        cnt = x_tmp+1;
        while(cnt--){
            ( *applyPixelMethod [method] )(x+y_tmp,y+cnt, GCFG.penColor, pInfo );
        }

        if(p <= 0){
            p += (x_tmp<<2) + 6;
        }else{
            p += ((x_tmp-y_tmp)<<2) + 10;
            y_tmp--;
        }
    }
    
    if( blurCmd ){
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
 > 插入一个空心长方形,线宽为1
=====================================*/
E_Status_t __Graph_rect_raw       (int xs,int ys,int xe,int ye, __GraphInfo_t* pInfo, E_ApplyPixel_t method){
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
 > 插入一个实心长方形
=====================================*/
E_Status_t __Graph_rect_fill      (int xs,int ys,int xe,int ye, __GraphInfo_t* pInfo, E_ApplyPixel_t method){
    
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
            bool blurCmd = ( method == kApplyPixel_blur && GCFG.blur_tmp.pBuffer==NULL );
            if( blurCmd ){
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
                __ImgRGB888_blur_average(pInfo, &GCFG.blur_tmp, &GCFG.blur_area, GCFG.blur_size, GCFG.blur_br_100);
#else
  #error "[RH_graphic]: Unknown color type."
#endif
            }

            for(int y = ys;y <= ye;y++)
                memcpy( pInfo->pBuffer       + y                   *pInfo->width        + xs                     ,\
                        GCFG.blur_tmp.pBuffer+(y-GCFG.blur_area.ys)*GCFG.blur_tmp.width + (xs-GCFG.blur_area.xs) ,\
                        ((xe-xs+1)*sizeof(pInfo->pBuffer[0]))        );
            if( blurCmd ){
                GCFG.blur_area.xs     = 0;
                GCFG.blur_area.ys     = 0;
                GCFG.blur_tmp.height  = GCFG.blur_area.height = 0;
                GCFG.blur_tmp.width   = GCFG.blur_area.width  = 0;
                __free(GCFG.blur_tmp.pBuffer);
                GCFG.blur_tmp.pBuffer = NULL;
            }
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
E_Status_t __Graph_rect_edged     (int xs,int ys,int xe,int ye, __GraphInfo_t* pInfo, E_ApplyPixel_t method){
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
        __ImgRGB888_blur_average(pInfo, &GCFG.blur_tmp, &GCFG.blur_area, GCFG.blur_size, GCFG.blur_br_100);
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
 > 插入圆角长方形
=====================================*/
E_Status_t __Graph_rect_round     (int xs,int ys,int xe,int ye, __GraphInfo_t* pInfo, E_ApplyPixel_t method){
    int r = __limit((signed)GCFG.penSize, 0, (__min((xe-xs), (ye-ys)))/2 );
    
    bool blurCmd = ( method == kApplyPixel_blur && GCFG.blur_tmp.pBuffer==NULL );
    if( blurCmd ){
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
        __ImgRGB888_blur_average(pInfo, &GCFG.blur_tmp, &GCFG.blur_area, GCFG.blur_size, GCFG.blur_br_100);
#else
  #error "[RH_graphic]: Unknown color type."
#endif
    }
    
    __Graph_rect_fill(xs+r+1, ys    , xe-r-1, ye    , pInfo, method);
    __Graph_rect_fill(xs    , ys+r+1, xs+r  , ye-r-1, pInfo, method);
    __Graph_rect_fill(xe-r  , ys+r+1, xe    , ye-r-1, pInfo, method);

    __Graph_circle_qrt1(xe-r, ys+r, r, pInfo, method);
    __Graph_circle_qrt2(xs+r, ys+r, r, pInfo, method);
    __Graph_circle_qrt3(xs+r, ye-r, r, pInfo, method);
    __Graph_circle_qrt4(xe-r, ye-r, r, pInfo, method);
    
    if( blurCmd ){
        __free(GCFG.blur_tmp.pBuffer);
        memset(&GCFG.blur_area , 0, sizeof(GCFG.blur_area));
        memset(&GCFG.blur_tmp  , 0, sizeof(GCFG.blur_tmp ));
    }
    
    return kStatus_Success;
}
    
/*====================================
 > 插入直线，线宽为1
=====================================*/
E_Status_t __Graph_line_raw       (int x1,int y1,int x2,int y2, __GraphInfo_t* pInfo, E_ApplyPixel_t method){
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
   
/*====================================
 > 插入直线，线宽随设定
=====================================*/
E_Status_t __Graph_line_edged     (int x1,int y1,int x2,int y2, __GraphInfo_t* pInfo, E_ApplyPixel_t method){
    int x_offset = 0;
    int y_offset = 0;
    int x11=0,y11=0,x22=0,y22=0,x33=0,y33=0,x44=0,y44=0;
    
    size_t         penSize  = GCFG.penSize;
    
    bool blurCmd = ( method==kApplyPixel_blur && GCFG.blur_tmp.pBuffer==NULL );
    if( blurCmd ){
        while(1);
    }
    
    if( penSize > 1 ){
        switch(__Dir_Line(x1,y1,x2,y2)){
            case  0:
            
                x11 = x1; y11 = (int)(y1-(penSize>>1)+(penSize%2==0));
                x22 = x1; y22 = (int)(y1+(penSize>>1));
                x33 = x2; y33 = (int)(y2-(penSize>>1)+(penSize%2==0));
                x44 = x2; y44 = (int)(y2+(penSize>>1));
                return __Graph_rect_fill(__min(x1,x2), __min(y11,y22), __max(x1,x2), __max(y33,y44), pInfo, method);
            case 65535:
                x11 = (int)(x1-(penSize>>1)                ); y11 = y1;
                x22 = (int)(x1+(penSize>>1)-(penSize%2==0));  y22 = y1;
                x33 = (int)(x2-(penSize>>1)                ); y33 = y2;
                x44 = (int)(x2+(penSize>>1)-(penSize%2==0));  y44 = y2;
                return __Graph_rect_fill(__min(x11,x22), __min(y1,y2), __max(x33,x44), __max(y1,y2), pInfo, method);
            case  1:
                x_offset = (int)lround(sqrt( ((y1-y2)*(y1-y2)*penSize*penSize/((x1-x2)*(x1-x2))) / (1.0*(y1-y2)*(y1-y2)/((x1-x2)*(x1-x2))+1) ));
                y_offset = (int)lround(sqrt( penSize*penSize/((y1-y2)*(y1-y2)/(1.0*(x1-x2)*(x1-x2))+1) ));
        
                x11 = x1+(x_offset>>1)-(x_offset%2==0); y11 = y1-(y_offset>>1);
                x22 = x1-(x_offset>>1)                ; y22 = y1+(y_offset>>1)-(y_offset%2==0);
                x33 = x2-(x_offset>>1)                ; y33 = y2+(y_offset>>1)-(y_offset%2==0);
                x44 = x2+(x_offset>>1)-(x_offset%2==0); y44 = y2-(y_offset>>1);
                break;
            case -1:
                x_offset = (int)lround(sqrt( ((y1-y2)*(y1-y2)*penSize*penSize/((x1-x2)*(x1-x2))) / (1.0*(y1-y2)*(y1-y2)/((x1-x2)*(x1-x2))+1) ));
                y_offset = (int)lround(sqrt( penSize*penSize/((y1-y2)*(y1-y2)/(1.0*(x1-x2)*(x1-x2))+1) ));
        
                x11 = x1+(x_offset>>1)                ; y11 = y1+(y_offset>>1)-(y_offset%2==0);
                x22 = x1-(x_offset>>1)+(x_offset%2==0); y22 = y1-(y_offset>>1)                ;
                x33 = x2-(x_offset>>1)+(x_offset%2==0); y33 = y2-(y_offset>>1)                ;
                x44 = x2+(x_offset>>1)                ; y44 = y2+(y_offset>>1)-(y_offset%2==0);
                break;
        }
    }
    
    ( *applyPixelMethod [method] )(x11,y11,M_COLOR_RED,pInfo);// GCFG.penColor
    ( *applyPixelMethod [method] )(x22,y22,M_COLOR_YELLOW,pInfo);
    ( *applyPixelMethod [method] )(x33,y33,M_COLOR_BLUE,pInfo);
    ( *applyPixelMethod [method] )(x44,y44,M_COLOR_GREEN,pInfo);
    
    __Graph_quad_fill(x11, y11, x22, y22, x33, y33, x44, y44, pInfo, method);
    
    if( blurCmd ){
        while(1);
    }
    return kStatus_Success;
}
    
/*====================================
 > 插入直线，线宽随设定
=====================================*/
E_Status_t __Graph_line_fill      (int x1,int y1,int x2,int y2, __GraphInfo_t* pInfo, E_ApplyPixel_t method){
    return CALL_FUNC( Graph , line_edged   )(x1,y1,x2,y2,pInfo,method);
}
    
/*====================================
 > 画任意四边形,线宽为1
=====================================*/
E_Status_t __Graph_quad_raw       (int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4, __GraphInfo_t* pInfo, E_ApplyPixel_t method){
    int tmp_y[] = {y1,y2,y3,y4};
    int tmp_x[] = {x1,x2,x3,x4};
    struct IntArray_t tmp;
 // 分析四边形正交化后的长与宽
    tmp = __findMin_INT(tmp_y,4);
    int top_y = tmp.value;
    // int top_x = *((&x1)+((tmp.index)<<1));

    tmp = __findMax_INT(tmp_y,4);
    // int bottom_x = *((&x1)+((tmp.index)<<1));

    tmp = __findMin_INT(tmp_x,4);
    // int left_y = *((&y1)+((tmp.index)<<1));
    int left_x = tmp.value;

    tmp = __findMax_INT(tmp_x,4);
    // int right_y = *((&y1)+((tmp.index)<<1));

 // 分析四边形的边框,任意两点组合去除对角线
    int x11 = x1;
    int y11 = y1;
    int x22,y22,x33,y33,x44,y44;
    
    int tmp_P1P2 = __Point_toLine(x1,y1,x2,y2, x3,y3) + __Point_toLine(x1,y1,x2,y2, x4,y4);
    int tmp_P1P3 = __Point_toLine(x1,y1,x3,y3, x2,y2) + __Point_toLine(x1,y1,x3,y3, x4,y4);

    if(tmp_P1P2 == 0){
        x22 = x3; y22 = y3;
        x33 = x2; y33 = y2;
        x44 = x4; y44 = y4;
    }else if(tmp_P1P3 == 0){
        x22 = x2; y22 = y2;
        x33 = x3; y33 = y3;
        x44 = x4; y44 = y4;
    }else{
        x22 = x2; y22 = y2;
        x33 = x4; y33 = y4;
        x44 = x3; y44 = y3;
    }
    __Graph_line_raw( x11-left_x , y11-top_y , x22-left_x , y22-top_y ,pInfo,method);
        
    __Graph_line_raw( x22-left_x , y22-top_y , x33-left_x , y33-top_y ,pInfo,method);

    __Graph_line_raw( x33-left_x , y33-top_y , x44-left_x , y44-top_y ,pInfo,method);
        
    __Graph_line_raw( x44-left_x , y44-top_y , x11-left_x , y11-top_y ,pInfo,method);
    
    return kStatus_Success;
}
    
/*====================================
 > 填充任意四边形
=====================================*/
E_Status_t __Graph_quad_fill      (int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4, __GraphInfo_t* pInfo, E_ApplyPixel_t method){
    int tmp_y[] = {y1,y2,y3,y4};
    int tmp_x[] = {x1,x2,x3,x4};
    struct IntArray_t tmp;
 // 分析四边形正交化后的长与宽
    tmp = __findMin_INT(tmp_y,4);
    int top_y = tmp.value;
    // int top_x = *((&x1)+((tmp.index)<<1));

    tmp = __findMax_INT(tmp_y,4);
    int bottom_y = tmp.value;
    // int bottom_x = *((&x1)+((tmp.index)<<1));

    tmp = __findMin_INT(tmp_x,4);
    // int left_y = *((&y1)+((tmp.index)<<1));
    int left_x = tmp.value;

    tmp = __findMax_INT(tmp_x,4);
    // int right_y = *((&y1)+((tmp.index)<<1));
    int right_x = tmp.value;

    int area_width  = right_x  - left_x + 1;
    int area_height = bottom_y - top_y  + 1;
 // 分析四边形的边框,任意两点组合去除对角线
    int x11 = x1;
    int y11 = y1;
    int x22,y22,x33,y33,x44,y44;
    
    int tmp_P1P2 = __Point_toLine(x1,y1,x2,y2, x3,y3) + __Point_toLine(x1,y1,x2,y2, x4,y4);
    int tmp_P1P3 = __Point_toLine(x1,y1,x3,y3, x2,y2) + __Point_toLine(x1,y1,x3,y3, x4,y4);

    if(tmp_P1P2 == 0){
        x22 = x3; y22 = y3;
        x33 = x2; y33 = y2;
        x44 = x4; y44 = y4;
    }else if(tmp_P1P3 == 0){
        x22 = x2; y22 = y2;
        x33 = x3; y33 = y3;
        x44 = x4; y44 = y4;
    }else{
        x22 = x2; y22 = y2;
        x33 = x4; y33 = y4;
        x44 = x3; y44 = y3;
    }
    // 创建临时空画布，大小取决于上述分析结果
    __GraphPixel_t* pBuffer = (__GraphPixel_t*)calloc((area_height*area_width),sizeof(__GraphPixel_t));

    // 绘制四边形边框，通过画线程序实现
    __GraphInfo_t pTmpInfo = {    .pBuffer = (void*)pBuffer    ,\
                                  .height  = area_height       ,\
                                  .width   = area_width        };
    __Graph_line_raw( x11-left_x , y11-top_y , x22-left_x , y22-top_y ,&pTmpInfo,kApplyPixel_mark);
        
    __Graph_line_raw( x22-left_x , y22-top_y , x33-left_x , y33-top_y ,&pTmpInfo,kApplyPixel_mark);

    __Graph_line_raw( x33-left_x , y33-top_y , x44-left_x , y44-top_y ,&pTmpInfo,kApplyPixel_mark);
        
    __Graph_line_raw( x44-left_x , y44-top_y , x11-left_x , y11-top_y ,&pTmpInfo,kApplyPixel_mark);
    
    // 从顶点开始，向下左右画点并搜寻,直到找到边线为止,随后填充
    for(int j = 0;j < area_height;j++){
        int LF = 0,RH = area_width - 1;
        for(;LF < RH;LF++){
            if( *(pBuffer + (j*area_width) + LF) == 1 )
                break;
        }
 
        for(;RH > LF;RH--){
            if( *(pBuffer + (j*area_width) + RH) == 1 )
                break;
        }
        memset((pBuffer + (j*area_width) + LF) ,0xff ,(RH-LF)*sizeof(__GraphPixel_t) );
    }
    
    // 将画布上的点，存入图像显存，注意偏移量
    for(int j = 0;j < area_height;j++){
        for(int i = 0;i < area_width;i++){
            if( (*(pBuffer + area_width*j + i)) != 0 )
                ( *applyPixelMethod [method] )( i+left_x , j+top_y , GCFG.penColor, pInfo );
        }
    }
    free(pBuffer);
    return kStatus_Success;
}
    
#ifdef __cplusplus
}
#endif

