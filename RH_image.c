

#include "RH_image.h"

#ifdef __cplusplus
extern "C" {
#endif


/*===========================================================================================================================
 > Image Processing Reference
============================================================================================================================*/
#if defined    (_WIN32)
#include <windows.h>
#elif defined  (__APPLE__)
typedef uint8_t   BYTE;
typedef uint16_t  WORD;
typedef uint32_t  DWORD;
typedef int32_t   LONG;
typedef float     FLOAT;
typedef FLOAT     *PFLOAT;
typedef BYTE      *PBYTE;
typedef BYTE      *LPBYTE;
typedef int       *PINT;
typedef int       *LPINT;
typedef WORD      *PWORD;
typedef WORD      *LPWORD;
typedef int32_t   *LPLONG;
typedef DWORD     *PDWORD;
typedef DWORD     *LPDWORD;
typedef void      *LPVOID;
#pragma pack(1)
typedef struct tagBITMAPFILEHEADER {
    WORD   bfType;
    DWORD  bfSize;
    WORD   bfReserved1;
    WORD   bfReserved2;
    DWORD  bfOffBits;
} BITMAPFILEHEADER;

#pragma pack(1)
typedef struct tagBITMAPINFOHEADER {
    DWORD  biSize;
    LONG   biWidth;
    LONG   biHeight;
    WORD   biPlanes;
    WORD   biBitCount;
    DWORD  biCompression;
    DWORD  biSizeImage;
    LONG   biXPelsPerMeter;
    LONG   biYPelsPerMeter;
    DWORD  biClrUsed;
    DWORD  biClrImportant;
} BITMAPINFOHEADER;

#endif
__ImageRGB888_t* __LoadBMP_ImgRGB888(const char* __restrict__ path){
    FILE* bmp;
    BITMAPFILEHEADER fileHead;
    BITMAPINFOHEADER infoHead;

    __ImageRGB888_t* pIMG = __malloc(sizeof(__ImageRGB888_t));
    pIMG->height  = 0;
    pIMG->width   = 0;
    pIMG->pBuffer = NULL;
    
    bmp = fopen(path, "r");
    if (bmp == NULL) {
        printf("open error\n");
        return pIMG;
    }
    fseek(bmp, 0L, SEEK_SET);
    fread(&fileHead, sizeof(BITMAPFILEHEADER), 1, bmp);
    fread(&infoHead, sizeof(BITMAPINFOHEADER), 1, bmp);

    if (fileHead.bfType != 0x4D42) {
        printf("This not a *.bmp file\n");
        return pIMG;
    }

    fseek(bmp, fileHead.bfOffBits, SEEK_SET);

    pIMG->pBuffer = (__UNION_PixelRGB888_t*)__malloc(infoHead.biWidth * infoHead.biHeight * sizeof(__UNION_PixelRGB888_t));
    
    for (int row = 0; row < infoHead.biHeight; row++) {
        for (int col = 0; col < infoHead.biWidth; col++) {
            fread(&(pIMG->pBuffer[(infoHead.biHeight - row - 1)*infoHead.biWidth + col].data), sizeof(__PixelRGB888_t), 1, bmp);

 //           printf("[%d]: ",(infoHead.biHeight - row - 1)*infoHead.biWidth + col);
 //           printf("R=%d G=%d B=%d\n",\
 //                  pIMG->pBuffer[(infoHead.biHeight - row - 1)*infoHead.biWidth + col].R,\
 //                  pIMG->pBuffer[(infoHead.biHeight - row - 1)*infoHead.biWidth + col].G,\
 //                  pIMG->pBuffer[(infoHead.biHeight - row - 1)*infoHead.biWidth + col].B);
        }
        int eps = (4-(infoHead.biWidth*sizeof(__PixelRGB888_t))%4)%4;
        uint8_t dummyByte;
        while(eps--){
            fread(&dummyByte,sizeof(char) ,1 , bmp);
        }
    }
    fclose(bmp);

    pIMG->width   = infoHead.biWidth;
    pIMG->height  = infoHead.biHeight;

    return pIMG;
}

__ImageRGB888_t* __CopyBMP_ImgRGB888(const __ImageRGB888_t* src,__ImageRGB888_t* dst){
    memcpy(dst->pBuffer, src->pBuffer, (src->height)*(src->width)*sizeof(__UNION_PixelRGB888_t));
    dst->height = src->height;
    dst->width  = src->width;
    return dst;
}

__ImageRGB888_t* __Create_ImgRGB888(size_t width,size_t height){
    __ImageRGB888_t* pIMG = __malloc(sizeof(__ImageRGB888_t));
    if(pIMG == NULL)
        return NULL;
    pIMG->height          = height;
    pIMG->width           = width;
    pIMG->pBuffer         = __malloc((pIMG->height)*(pIMG->width)*sizeof(pIMG->pBuffer[0]));
    if(pIMG->pBuffer == NULL){
        __free(pIMG);
        return NULL;
    }
    memset(pIMG->pBuffer, 0, (pIMG->height)*(pIMG->width)*sizeof(pIMG->pBuffer[0]));
    return pIMG;
}

__ImageRGB888_t* __OutBMP_ImgRGB888(const char* __restrict__ path,__ImageRGB888_t* p){
    FILE* bmp;
    if(p == NULL && p->pBuffer == NULL)
        return NULL;
    int eps = (4-(p->width*sizeof(__PixelRGB888_t))%4)%4;
    BITMAPFILEHEADER fileHead = {
        .bfOffBits      = 40 + 14   ,
        .bfReserved1    = 0         ,
        .bfReserved2    = 0         ,
        .bfSize         = (uint32_t)(p->height * p->width * sizeof(__PixelRGB888_t) + 54),
        .bfType         = 0x4D42    ,
    };
    BITMAPINFOHEADER infoHead = {
        .biSize          = 40        ,
        .biWidth         = (int)(p->width)  ,
        .biHeight        = (int)(p->height) ,
        .biPlanes        = 1         ,
        .biBitCount      = 8+8+8     ,
        .biCompression   = 0         ,
        .biSizeImage     = (uint32_t)(p->height*p->width*sizeof(__PixelRGB888_t) + eps*(p->height)) ,
        .biClrUsed       = 0         ,
        .biClrImportant  = 0         ,
        .biXPelsPerMeter = 0         ,
        .biYPelsPerMeter = 0         ,
    };
    printf("%d\n" ,infoHead.biSizeImage    );
    bmp = fopen(path,"wb");

    if(bmp == NULL) return NULL;

    // RGB Sequence should be reversed.
    fseek(bmp,0L,SEEK_SET);
    fwrite(&fileHead ,1 ,sizeof(BITMAPFILEHEADER) , bmp);
    fwrite(&infoHead ,1 ,sizeof(BITMAPINFOHEADER) , bmp);
    fseek(bmp,54L,SEEK_SET);
    for (int row = 0; row < p->height; row++) {
        for (int col = 0; col < p->width; col++) {
            fwrite( &p->pBuffer[(infoHead.biHeight - row - 1) * infoHead.biWidth + col] ,sizeof(__PixelRGB888_t) ,1 ,bmp );
        }
        int eps = (4-(infoHead.biWidth*sizeof(__PixelRGB888_t))%4)%4;
        uint8_t dummyByte = 0x00;
        while(eps--){
            fwrite(&dummyByte,sizeof(char) ,1 , bmp);
        }
    }
    
    fclose(bmp);
    return p;
}

__ImageRGB888_t* __FreeBuffer_ImgRGB888(__ImageRGB888_t* ptr){
    __free(ptr->pBuffer);
    ptr->height  = 0;
    ptr->width   = 0;
    ptr->pBuffer = NULL;
    return ptr;
}

void __Free_ImgRGB888(__ImageRGB888_t* ptr){
    __free(__FreeBuffer_ImgRGB888(ptr));
}

__ImageRGB888_t* __Filter_Gray_ImgRGB888(const __ImageRGB888_t* src,__ImageRGB888_t* dst,uint32_t br_100){
    
    if(src != NULL && dst != NULL){
        if(src->pBuffer != NULL && dst->pBuffer != NULL){
            for (int row = 0; row < src->height; row++) {
                for (int col = 0; col < src->width; col++) {
                    long temp = lroundl(0.299 * src->pBuffer[row * src->width + col].R + 0.587 * src->pBuffer[row * src->width + col].G + 0.114 * src->pBuffer[row * src->width + col].B);
                    dst->pBuffer[row * src->width + col].data = (uint32_t)(((temp&0xff)<<16)|((temp&0xff)<<8)|((temp&0xff)));
                }
            }
        }
    }
    return dst;
}

__ImageRGB888_t* __Filter_Warm_ImgRGB888(const __ImageRGB888_t* src,__ImageRGB888_t* dst,uint32_t br_100){
    return dst;
}

__ImageRGB888_t* __Filter_Cold_ImgRGB888(const __ImageRGB888_t* src,__ImageRGB888_t* dst,uint32_t br_100){
    
    return dst;
}

__ImageRGB888_t* __Filter_OTUS_ImgRGB888(const __ImageRGB888_t* src,__ImageRGB888_t* dst,uint32_t br_100){
    uint32_t threshold = 0;
    __exitReturn( src==NULL         , NULL);
    __exitReturn( src->pBuffer==NULL, NULL);
    __exitReturn( dst==NULL         , NULL);
    __exitReturn( dst->pBuffer==NULL, NULL);
    __Analyze_OTUS_ImgRGB888(src, &threshold);
    __exitReturn(threshold == -1, NULL);
    
    for(int y=0;y<src->height;y++){
        for(int x=0;x<src->width;x++){
            uint8_t temp = ( __array1D(src->pBuffer, src->width, y, x)->R*19595 + \
                             __array1D(src->pBuffer, src->width, y, x)->G*38469 + \
                             __array1D(src->pBuffer, src->width, y, x)->B*7472 )>>16;
            if( temp > threshold )
                __array1D(dst->pBuffer, dst->width, y, x)->data = 0x00ffffff;
            else
                __array1D(dst->pBuffer, dst->width, y, x)->data = 0x00000000;
        }
    }
    
//    printf("otus threshold = %d\n", threshold);
    
    
    return NULL;
}
     
__ImageRGB888_t* __Trans_Mirror_ImgRGB888(const __ImageRGB888_t* src,__ImageRGB888_t* dst,uint8_t HV){
    if( src == NULL || src->pBuffer == NULL ){
        return NULL;
    }
    if(dst == NULL){
        dst = (__ImageRGB888_t*)__malloc(sizeof(__ImageRGB888_t));
        if(dst == NULL) // Not enough space :-(
            return dst;
        dst->pBuffer = (__UNION_PixelRGB888_t*)__malloc(src->width * src->height * sizeof(__UNION_PixelRGB888_t));
        if(dst->pBuffer == NULL) // Not enough space :-(
            return dst;
    }
    if(dst->pBuffer == NULL){
        dst->pBuffer = (__UNION_PixelRGB888_t*)__malloc(src->width * src->height * sizeof(__UNION_PixelRGB888_t));
        if(dst->pBuffer == NULL) // Not enough space :-(
            return dst;
    }
    
    switch(HV){
        case 0:
            for(size_t y=0;y<src->height;y++){
                for(size_t x=0;x<((src->width+1)>>1);x++){
                    __UNION_PixelRGB888_t tmp_fore = src->pBuffer[y*src->width + x];
                    __UNION_PixelRGB888_t tmp_back = src->pBuffer[y*src->width + src->width - 1 - x];
                    dst->pBuffer[y*src->width+x]                    = tmp_back;
                    dst->pBuffer[y*src->width + src->width - 1 - x] = tmp_fore;
                }
            }

            break;
        case 1:
            for(size_t y=0;y<src->height;y++){
                memmove(&dst->pBuffer[(src->height-1-y)*dst->width], &src->pBuffer[y*src->width], src->width*sizeof(__UNION_PixelRGB888_t));
            }
            break;
        default:
            //...//
            break;
    }
    
    return NULL;
}

__ImageRGB888_t* __Blur_Gussian_ImgRGB888(const __ImageRGB888_t* src,__ImageRGB888_t* dst,uint32_t radSize, uint16_t br_100){
    static __Kernel_t gus_kernel = {
        .pBuffer = NULL,
        .order   = 0,
        .sum     = 0,
    };
    static uint16_t radSize_old      = 0;
    
    if( radSize_old != radSize ){
        if( gus_kernel.pBuffer != NULL ){
            __free( gus_kernel.pBuffer );
        }
        double sigma  = __map(radSize, 0, 65535, 0.0, 10.0); // convert a normal value to sigma
        size_t order  = lround(sigma*6); // 6 times sigma includes 99% area.
        gus_kernel.pBuffer = (uint16_t*)__malloc( order*order*sizeof(uint16_t) );
        
        if((order & 0x01) == 0) // order should be an odd number.
            order--;
        if(order>=31) // too big!!!
            order = 31;
        __gussianKernel(sigma,order,&gus_kernel);
    }

    __ImageRGB888_t* pImg = __Conv2D_ImgRGB888(src, dst, &gus_kernel,br_100);
    
    radSize_old      = radSize;

    return pImg;

}

__ImageRGB888_t* __Blur_Average_ImgRGB888(const __ImageRGB888_t* src,__ImageRGB888_t* dst,uint32_t radSize, uint16_t br_100){
//    __exitReturn(src == NULL || dst == NULL , NULL);
    
    double sigma  = __map(radSize, 0, 65535, 0.0, 10.0); // convert a normal value to sigma
    size_t order  = lround(sigma*6); // 6 times sigma includes 99% area.
    
    order = __limit(order, 3, 101);
    if((order & 0x01) == 0) // order should be an odd number.
        order--;
    
    unsigned long sum_R = 0, sum_G = 0, sum_B = 0;
    unsigned long div = 0;
    
    // Pre-Calculation
    int half_order  = (int)((order+1)>>1); // Kernel
    div = half_order * half_order;
    for(int n=0;n<half_order;n++){
        for(int m=0;m<half_order;m++){
            sum_R += __array1D(src->pBuffer, src->width, n, m)->R;
            sum_G += __array1D(src->pBuffer, src->width, n, m)->G;
            sum_B += __array1D(src->pBuffer, src->width, n, m)->B;
        }
    }
    div = half_order * half_order;
//    printf("div = %ld , sum_R = %ld\n",div,sum_R);
    // Average Filter
    for(int j=0;j<src->height;j++){
        for(int i=0;i<src->width;i++){
            if(j%2 == 0){ // Scan Direction:  [old] -->--> [new]
                
                if(i!=0){ // No need to do when it reachs the left-edge because it has been done when moving to the next row.
                    
                    // Remove leftmost column because it is old.
                    if(i-half_order >= 0){                                          // [!] no cross the broad [0,src->width-1]
                        for(int row=j-half_order+1;row<=j+half_order-1;row++){
                            if( row<src->height && row>=0 ){                       // [!] no cross the broad [0,src->height-1]
                                sum_R -= __array1D(src->pBuffer, src->width, row, i-half_order)->R;
                                sum_G -= __array1D(src->pBuffer, src->width, row, i-half_order)->G;
                                sum_B -= __array1D(src->pBuffer, src->width, row, i-half_order)->B;
                                div--;
                            }
                        }
                    }
                    
                    // Add rightmost column because it is new.
                    if( i+half_order-1 < src->width ){                             // [!] no cross the broad [0,src->width-1]
                        for(int row=j-half_order+1;row<=j+half_order-1;row++){
                            if( row<src->height && row>=0 ){                       // [!] no cross the broad [0,src->height-1]
                                sum_R += __array1D(src->pBuffer, src->width, row, i+half_order-1)->R;
                                sum_G += __array1D(src->pBuffer, src->width, row, i+half_order-1)->G;
                                sum_B += __array1D(src->pBuffer, src->width, row, i+half_order-1)->B;
                                div++;
                            }
                        }
                    }
                    
                }
//                printf("[%2ld , %4ld] ",div,sum_R);
                __array1D(dst->pBuffer, src->width, j, i)->R = sum_R*br_100/(div*100);
                __array1D(dst->pBuffer, src->width, j, i)->G = sum_G*br_100/(div*100);
                __array1D(dst->pBuffer, src->width, j, i)->B = sum_B*br_100/(div*100);
            }else{ // Scan Direction:  [new] <--<-- [old]
                int k = (int)(src->width)-i-1; // reverse i
                // Remove rightmost column because it is old.
                if( k != src->width-1 ){ // No need to do when it reachs the right-edge because it has been done when moving to the next row.
                    if(k+half_order < src->width ){                                // [!] no cross the broad [0,src->width-1]
                        for(int row=j-half_order+1;row<=j+half_order-1;row++){
                            if( row<src->height && row>=0 ){                       // [!] no cross the broad [0,src->height-1]
                                sum_R -= __array1D(src->pBuffer, src->width, row, k+half_order)->R;
                                sum_G -= __array1D(src->pBuffer, src->width, row, k+half_order)->G;
                                sum_B -= __array1D(src->pBuffer, src->width, row, k+half_order)->B;
                                div--;
                            }
                        }
                    }
                    
                    // Add leftmost column because it is new.
                    if(k-half_order+1 >=0 ){                                       // [!] no cross the broad [0,src->width-1]
                        for(int row=j-half_order+1;row<=j+half_order-1;row++){
                            if( row<src->height && row>=0 ){                       // [!] no cross the broad [0,src->height-1]
                                sum_R += __array1D(src->pBuffer, src->width, row, k-half_order+1)->R;
                                sum_G += __array1D(src->pBuffer, src->width, row, k-half_order+1)->G;
                                sum_B += __array1D(src->pBuffer, src->width, row, k-half_order+1)->B;
                                div++;
                            }
                        }
                    }
                }
//                printf("[%2ld , %4ld] ",div,sum_R);
                __array1D(dst->pBuffer, src->width, j, k)->R = sum_R*br_100/(div*100);
                __array1D(dst->pBuffer, src->width, j, k)->G = sum_G*br_100/(div*100);
                __array1D(dst->pBuffer, src->width, j, k)->B = sum_B*br_100/(div*100);
            }
            // End of scanning of this row.
        }
        
//        printf("\n");
        // Remove topmost row because it is old.
        if( j-half_order+1 >= 0 ){         // [!] no cross the broad [0,src->height-1]
            
            if(j%2 == 0){ // Scan Direction:  [old] -->--> [new]. Now it is reaching the rightmost.
                for(int col=(int)(src->width-half_order);col<src->width;col++){
                    sum_R -= __array1D(src->pBuffer, src->width, j-half_order+1, col)->R;
                    sum_G -= __array1D(src->pBuffer, src->width, j-half_order+1, col)->G;
                    sum_B -= __array1D(src->pBuffer, src->width, j-half_order+1, col)->B;
                    div--;
                }
            }else{        // Scan Direction:  [new] <--<-- [old]. Now it is reaching the leftmost.
                for(int col=0;col<half_order;col++){
                    sum_R -= __array1D(src->pBuffer, src->width, j-half_order+1, col)->R;
                    sum_G -= __array1D(src->pBuffer, src->width, j-half_order+1, col)->G;
                    sum_B -= __array1D(src->pBuffer, src->width, j-half_order+1, col)->B;
                    div--;
                }
            }

        }
        // Add downmost row because it is new.
        if(j+half_order < src->height ){         // [!] no cross the broad [0,src->height-1]
            
            if(j%2 == 0){ // Scan Direction:  [old] -->--> [new]. Now it is reaching the rightmost.
                for(int col=(int)(src->width-half_order);col<src->width;col++){
                    sum_R += __array1D(src->pBuffer, src->width, j+half_order, col)->R;
                    sum_G += __array1D(src->pBuffer, src->width, j+half_order, col)->G;
                    sum_B += __array1D(src->pBuffer, src->width, j+half_order, col)->B;
                    div++;
                }
            }else{        // Scan Direction:  [new] <--<-- [old]. Now it is reaching the leftmost.
                for(int col=0;col<half_order;col++){
                    sum_R += __array1D(src->pBuffer, src->width, j+half_order, col)->R;
                    sum_G += __array1D(src->pBuffer, src->width, j+half_order, col)->G;
                    sum_B += __array1D(src->pBuffer, src->width, j+half_order, col)->B;
                    div++;
                }
            }
        }
        
    }
    
    return NULL;
}

__ImageRGB888_t* __Interpo_NstNeighbor_ImgRGB888(const __ImageRGB888_t* src,__ImageRGB888_t* dst,size_t height,size_t width){
    if(src == NULL || src->pBuffer == NULL || dst == NULL) // Bad address
        return NULL;
    if(height < src->height || width < src->width) // Image of "dst" should be larger than image of "src" in both dimension.
        return NULL;
    if(dst->pBuffer == NULL){
        dst->pBuffer = (__UNION_PixelRGB888_t*)__malloc(width*height*sizeof(__UNION_PixelRGB888_t));
        if(dst->pBuffer == NULL){
            return NULL;  // There is no space to malloc.
        }
    }
    dst->height = height;
    dst->width  = width;
    memset(dst->pBuffer, 0, width*height*sizeof(__UNION_PixelRGB888_t));
    
    size_t eps_x = 0; // Record errors no bigger than width.
    size_t eps_y = 0; // Record errors no bigger than height.
    size_t fx    = 0; // Pixel cordination of original image in x direction.
    size_t fy    = 0; // Pixel cordination of original image in y direction.
    for(size_t gy=0;gy<height;gy++){
        eps_y += src->height;
        if(eps_y >= height){
            fx = 0;
            for(size_t gx=0;gx<width;gx++){
                eps_x += src->width;
                if( eps_x >= width ){
                    // This is the position to copy pixels from original image.
                    (dst->pBuffer + (width*gy) + gx)->data = (src->pBuffer + (src->width*fy) + fx)->data;
//                    printf("%ld,%ld ",fx,fy);
                    eps_x-=width;
                    fx++;
                }else{
                    // This is the position needs to interpolate.
                    //...//
                }
            }
//            printf("\n");
            eps_y -= height;
            fy++;
        }else
            continue;
    }
    
    return dst;
}

__ImageRGB565_t* __Conv2D_ImgRGB565(const __ImageRGB565_t* src,__ImageRGB565_t* dst,const __Kernel_t* k,uint16_t br_100){
    if( src == NULL || src->pBuffer == NULL || k == NULL){
        return dst;
    }
        
    if(dst == NULL){
        dst = (__ImageRGB565_t*)__malloc(sizeof(__ImageRGB565_t));
        if(dst == NULL) // Not enough space :-(
            return dst;
        dst->pBuffer = (__UNION_PixelRGB565_t*)__malloc(src->width * src->height * sizeof(__UNION_PixelRGB565_t));
        if(dst->pBuffer == NULL) // Not enough space :-(
            return dst;
    }
    
    if(dst->pBuffer == NULL){
        dst->pBuffer = (__UNION_PixelRGB565_t*)__malloc(src->width * src->height * sizeof(__UNION_PixelRGB565_t));
        if(dst->pBuffer == NULL) // Not enough space :-(
            return dst;
    }
    
    if(dst == NULL){
        dst = (__ImageRGB565_t*)__malloc(sizeof(__ImageRGB565_t));
        if(dst == NULL) // Not enough space :-(
            return dst;
        dst->pBuffer = (__UNION_PixelRGB565_t*)__malloc(src->width * src->height * sizeof(__UNION_PixelRGB565_t));
    }

    for(int j=0;j<src->height;j++){
        for(int i=0;i<src->width;i++){
            int div = k->sum;
            
            // Here comes the convolution part.
            unsigned long tmp_R = 0,tmp_G = 0,tmp_B = 0; // Preparation for RGB data.
            for(int n=0;n<k->order;n++){
                for(int m=0;m<k->order;m++){
                    size_t offset_y  = j-(k->order>>1)+n;
                    size_t offset_x  = i-(k->order>>1)+m;
                    int selectKernel = *( k->pBuffer + n       * k->order + m       );
                    if(offset_x<0||offset_y<0||offset_x>=src->width||offset_y>=src->height){
                        div -= selectKernel;
                    }else{
                        uint8_t select_R  = (src->pBuffer + offset_y*src->width + offset_x)->R;
                        uint8_t select_G  = (src->pBuffer + offset_y*src->width + offset_x)->G;
                        uint8_t select_B  = (src->pBuffer + offset_y*src->width + offset_x)->B;
                        
                        tmp_R += ( (select_R) * (selectKernel) );
                        tmp_G += ( (select_G) * (selectKernel) );
                        tmp_B += ( (select_B) * (selectKernel) );
                    }
                }
            }
            size_t offset = (j*src->width)+i;
            if(offset < dst->width*dst->height){
                (dst->pBuffer+offset)->R = (div==0)?((1<<5)-1):(tmp_R*br_100/(div*100));
                (dst->pBuffer+offset)->G = (div==0)?((1<<6)-1):(tmp_G*br_100/(div*100));
                (dst->pBuffer+offset)->B = (div==0)?((1<<5)-1):(tmp_B*br_100/(div*100));
            }
        }
    }
    
    return dst;
}

__ImageRGB888_t* __Conv2D_ImgRGB888(const __ImageRGB888_t* src,__ImageRGB888_t* dst,const __Kernel_t* k,uint16_t br_100){
    if( src == NULL || src->pBuffer == NULL || k == NULL ){
        return dst;
    }
    
    if(dst == NULL){
        dst = (__ImageRGB888_t*)__malloc(sizeof(__ImageRGB888_t));
        if(dst == NULL) // Not enough space :-(
            return dst;
        dst->pBuffer = (__UNION_PixelRGB888_t*)__malloc(src->width * src->height * sizeof(__UNION_PixelRGB888_t));
        if(dst->pBuffer == NULL) // Not enough space :-(
            return dst;
    }
    
    if(dst->pBuffer == NULL){
        dst->pBuffer = (__UNION_PixelRGB888_t*)__malloc(src->width * src->height * sizeof(__UNION_PixelRGB888_t));
        if(dst->pBuffer == NULL) // Not enough space :-(
            return dst;
    }
    
    if(dst == NULL){
        dst = (__ImageRGB888_t*)__malloc(sizeof(__ImageRGB888_t));
        if(dst == NULL) // Not enough space :-(
            return dst;
        dst->pBuffer = (__UNION_PixelRGB888_t*)__malloc(src->width * src->height * sizeof(__UNION_PixelRGB888_t));
    }

    for(int j=0;j<src->height;j++){
        for(int i=0;i<src->width;i++){
            int div = k->sum;
            
            // Here comes the convolution part.
            unsigned long tmp_R = 0,tmp_G = 0,tmp_B = 0; // Preparation for RGB data.
            for(int n=0;n<k->order;n++){
                for(int m=0;m<k->order;m++){
                    size_t offset_y  = j-(k->order>>1)+n;
                    size_t offset_x  = i-(k->order>>1)+m;
                    int selectKernel = *( k->pBuffer + n       * k->order + m       );
                    if(offset_x<0||offset_y<0||offset_x>=src->width||offset_y>=src->height){
                        div -= selectKernel;
                    }else{
                        uint8_t select_R  = (src->pBuffer + offset_y*src->width + offset_x)->R;
                        uint8_t select_G  = (src->pBuffer + offset_y*src->width + offset_x)->G;
                        uint8_t select_B  = (src->pBuffer + offset_y*src->width + offset_x)->B;
                        
                        tmp_R += ( (select_R) * (selectKernel) );
                        tmp_G += ( (select_G) * (selectKernel) );
                        tmp_B += ( (select_B) * (selectKernel) );
                    }
                }
            }
            size_t offset = (j*src->width)+i;
            if(offset < dst->width*dst->height){
                unsigned long temp = 0;
                
                temp = (tmp_R*br_100)/(div*100);
                if( div == 0 || temp >= (1<<8) )    (dst->pBuffer+offset)->R = (uint8_t)((1<<8)-1);
                else                                (dst->pBuffer+offset)->R = (uint8_t)(temp);
                
                temp = (tmp_G*br_100)/(div*100);
                if( div == 0 || temp >= (1<<8) )    (dst->pBuffer+offset)->G = (uint8_t)((1<<8)-1);
                else                                (dst->pBuffer+offset)->G = (uint8_t)(temp);
                
                temp = (tmp_B*br_100)/(div*100);
                if( div == 0 || temp >= (1<<8) )    (dst->pBuffer+offset)->B = (uint8_t)((1<<8)-1);
                else                                (dst->pBuffer+offset)->B = (uint8_t)(temp);
                
            }
        }
    }
    
    return dst;
}
      
void __Analyze_OTUS_ImgRGB888(const __ImageRGB888_t* src,uint32_t* threshold){
    *threshold = -1;
    __exit( src          == NULL );
    __exit( src->pBuffer == NULL );
    
    int    threshold_temp;
    long   NumOf_bckPixel = 0;    //Number of pixels defined as background
    long   SumOf_bckPixel = 0;    //Sum of pixels defined as background
    float  AvgOf_bckPixel = 0.0;  //Average value of pixels defined as background

    long   NumOf_objPixel = 0;    //Number of pixels defined as background
    long   SumOf_objPixel = 0;    //Sum of pixels defined as background
    float  AvgOf_objPixel = 0.0;  //Average value of pixels defined as background
    
    float  g = 0.0, g_max = 0.0;  //Variance between obj and bck
    
    const long NumOf_allPixel = (src->width) * (src->height);
    
    // Make Statistic...

    long gray_cnt[255] = {0};
    for (int row = 0; row < src->height; row++){
        for (int col = 0; col < src->width; col++){
            uint8_t temp = ( __array1D(src->pBuffer, src->width, row, col)->R*19595 + \
                             __array1D(src->pBuffer, src->width, row, col)->G*38469 + \
                             __array1D(src->pBuffer, src->width, row, col)->B*7472 )>>16;
            gray_cnt[temp]++;
        }
    }
    
    for (threshold_temp = 0; threshold_temp < 255; threshold_temp++){
        NumOf_bckPixel = 0; SumOf_bckPixel = 0;
        NumOf_objPixel = 0; SumOf_objPixel = 0;
        
        // Calculation for Background.
        for (uint8_t i = 0; i < threshold_temp; i++){
            NumOf_bckPixel += gray_cnt[i];
            SumOf_bckPixel += (i * gray_cnt[i]);
        }
        if (NumOf_bckPixel == 0)
            continue;
        AvgOf_bckPixel = SumOf_bckPixel / NumOf_bckPixel;
        
        // Calculation for object.
        NumOf_objPixel = NumOf_allPixel - NumOf_bckPixel;
        for (uint8_t i = threshold_temp; i < 255; i++){
            SumOf_objPixel += (i * gray_cnt[i]);
        }
        if (NumOf_objPixel == 0)
            continue;
        AvgOf_objPixel = SumOf_objPixel / NumOf_objPixel;
        
        //In case of overloading,calculate step by step.
        float w_bck   = (float)NumOf_bckPixel / NumOf_allPixel;
        float w_obj   = (float)NumOf_objPixel / NumOf_allPixel;
        float mu_diff = AvgOf_objPixel - AvgOf_bckPixel;

        g = w_bck * mu_diff * w_obj * mu_diff;
        if (g > g_max){
            g_max = g;
            *threshold = threshold_temp;
        }
    }
}


#ifdef __cplusplus
}
#endif
