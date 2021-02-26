#ifndef _RH_GRAPHIC_H
#define _RH_GRAPHIC_H

#include "RH_common.h"
#include "RH_config.h"
#include "RH_color.h"
#include "RH_image.h"

#ifdef __cplusplus
extern "C" {
#endif

#if   ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_BIN    )
typedef __ImageBIN_t           __GraphInfo_t;
typedef bool                   __GraphPixel_t;
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB565 )
typedef __ImageRGB565_t        __GraphInfo_t;
typedef uint16_t               __GraphPixel_t;
#elif ( GRAPHIC_COLOR_TYPE == GRAPHIC_COLOR_RGB888 )
typedef __ImageRGB888_t        __GraphInfo_t;
typedef uint32_t               __GraphPixel_t;
#else
  #error "[RH_graphic]: Unknown color type."
#endif

typedef enum{
    kApplyPixel_mark    ,
    kApplyPixel_unmark  ,
    kApplyPixel_fill    ,
    kApplyPixel_light   ,
    kApplyPixel_depix   ,
    kApplyPixel_reverse ,
    kApplyPixel_pick    ,
    kApplyPixel_blur
}E_ApplyPixel_t;

E_Status_t MAKE_FUNC( Graph , init         ) (void                    );
E_Status_t MAKE_FUNC( Graph , set_penSize  ) (size_t         penSize  );
E_Status_t MAKE_FUNC( Graph , set_penColor ) (__GraphPixel_t penColor );


E_Status_t MAKE_FUNC( Graph , circle_raw   ) (int x ,int y ,int d , __GraphInfo_t* pInfo, E_ApplyPixel_t method);
E_Status_t MAKE_FUNC( Graph , circle_edged ) (int x ,int y ,int d , __GraphInfo_t* pInfo, E_ApplyPixel_t method);
E_Status_t MAKE_FUNC( Graph , circle_fill  ) (int x ,int y ,int d , __GraphInfo_t* pInfo, E_ApplyPixel_t method);


#ifdef __cplusplus
}
#endif

#endif
