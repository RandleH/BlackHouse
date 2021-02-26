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
static  __GraphPixel_t __ApplyPixel_moveBlur     (int x, int y , __GraphPixel_t nan    , __GraphInfo_t* pInfo);
static  __GraphPixel_t __ApplyPixel_reverse      (int x, int y , __GraphPixel_t color  , __GraphInfo_t* pInfo);

static const func_ApplyPixelMethod applyPixelMethod[] = {
    [ kApplyPixel_mark    ] = __ApplyPixel_mark      ,
    [ kApplyPixel_unmark  ] = __ApplyPixel_unmark    ,
    [ kApplyPixel_fill    ] = __ApplyPixel_fill      ,
    [ kApplyPixel_light   ] = __ApplyPixel_lightness ,
    [ kApplyPixel_depix   ] = __ApplyPixel_unzero    ,
    [ kApplyPixel_reverse ] = __ApplyPixel_reverse   ,
    [ kApplyPixel_pick    ] = NULL      ,
    [ kApplyPixel_blur    ] = NULL
};

/*===========================================
 > 在指定缓存区,标记一个点
============================================*/
static __GraphPixel_t __ApplyPixel_mark(int x,int y,__GraphPixel_t nan,__GraphInfo_t* pInfo){
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
static __GraphPixel_t __ApplyPixel_unmark(int x,int y,__GraphPixel_t nan,__GraphInfo_t* pInfo){
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
static __GraphPixel_t __ApplyPixel_fill(int x,int y,__GraphPixel_t color,__GraphInfo_t* pInfo){
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
static __GraphPixel_t __ApplyPixel_unzero(int x,int y,__GraphPixel_t color,__GraphInfo_t* pInfo){
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
static __GraphPixel_t __ApplyPixel_lightness(int x,int y,__GraphPixel_t br_100,__GraphInfo_t* pInfo){
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
static __GraphPixel_t __ApplyPixel_reverse(int x,int y,__GraphPixel_t br_100,__GraphInfo_t* pInfo){
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
    
    

struct __GraphConfig_t{
    __GraphPixel_t  penColor;
    size_t          penSize;
    
    unsigned int    blur_br_100;
    unsigned int    blur_size;
    
};
    
static struct __GraphConfig_t GCFG = {
    .penColor    = M_COLOR_WHITE ,
    .penSize     = 3             ,
    .blur_size   = 44100         ,
    .blur_br_100 = 100
};
    
E_Status_t MAKE_FUNC( Graph , init ) (void){
    GCFG.blur_br_100 = 100;
    GCFG.blur_size   = 44100;
    GCFG.penColor    = M_COLOR_WHITE;
    GCFG.penSize     = 3;
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

/*===========================================
 > 插入一个空心圆,线宽为1
============================================*/
E_Status_t MAKE_FUNC( Graph , circle_raw )  (int x ,int y ,int d , __GraphInfo_t* pInfo, E_ApplyPixel_t method){
    int r    = d>>1;
    int p    = 3-2*r;
    bool eps = (d%2==0);
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
    return kStatus_Success;
}

/*====================================
 > 插入一个填充圆
=====================================*/
E_Status_t MAKE_FUNC( Graph , circle_fill )  (int x ,int y ,int d , __GraphInfo_t* pInfo, E_ApplyPixel_t method){
    int r = d>>1;
    int p = 3-(r<<1);
    int x_tmp = 0,y_tmp = r;
    bool eps  = (d%2==0);

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
    return kStatus_Success;
}

/*====================================
 > 插入一个空心圆,线宽随设定
=====================================*/
E_Status_t MAKE_FUNC( Graph , circle_edged ) (int x ,int y ,int d , __GraphInfo_t* pInfo, E_ApplyPixel_t method){
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
    return kStatus_Success;
}

    

    
    
#ifdef __cplusplus
}
#endif

