double __cordic_tan(double a){
     const static uint16_t angle_256[] = {
        // When 1 deg = 256, then...
        
        11520 , // Φ = 45               tan(Φ) = 1/1
        6801  , // Φ = 26.565051177078  tan(Φ) = 1/2
        3593  , // Φ = 14.036243467927  tan(Φ) = 1/4
        1824  , // Φ = 26.565051177078  tan(Φ) = 1/8
        916   , // Φ = 26.565051177078  tan(Φ) = 1/16
        458   , // Φ = 26.565051177078  tan(Φ) = 1/32
        229   , // Φ = 26.565051177078  tan(Φ) = 1/64
        115   , // Φ = 26.565051177078  tan(Φ) = 1/128
        57    , // Φ = 26.565051177078  tan(Φ) = 1/256
        29    , // Φ = 26.565051177078  tan(Φ) = 1/512
        14    , // Φ = 26.565051177078  tan(Φ) = 1/1024
        7     , // Φ = 26.565051177078  tan(Φ) = 1/2048
        4     , // Φ = 26.565051177078  tan(Φ) = 1/4096
        2     , // Φ = 26.565051177078  tan(Φ) = 1/8192
        1       // Φ = 26.565051177078  tan(Φ) = 1/16384
    };

    long x = 0xffff;
    long y = 0;
    
    long angle_tar = lround(a*14667.719555349074144);
    long angle_sum = 0;
    long x_tmp = 0,y_tmp = 0;
    
    
//    uint8_t sgn = 0;
//    while(angle_tar > 23040){
//        angle_tar-=23040;
//        if( sgn >= 4 )
//            sgn = 0;
//        sgn++;
//    }
//
//    while(angle_tar < -23040){
//        angle_tar+=23040;
//        if( sgn >= 4 )
//            sgn = 0;
//        sgn++;
//    }
//
//    __exitReturn( angle_tar >  22810 , tan(a) ); // Accuracy cannot meet by this algorithm.
//    __exitReturn( angle_tar < -22810 , tan(a) );
//    printf("tar = %ld\n",angle_tar);
    
    for( int i=0; i<15; i++ ){
        if( angle_tar < angle_sum ){ // cw
            x_tmp = (x + (y>>i));
            y_tmp = (y - (x>>i));
            x  = x_tmp;
            y  = y_tmp;
            angle_sum -= angle_256[i];
        }else{ // ccw
            x_tmp = (x - (y>>i));
            y_tmp = (y + (x>>i));
            x  = x_tmp;
            y  = y_tmp;
            angle_sum += angle_256[i];
        }
    }
//    printf("%d\n",sgn);
    
//    switch(sgn){
//        case 0:
//        case 2:
//        case 4: return y/((double)(x));
//        case 1: return x/((double)(y));
//
//        case 3: return x/((double)(-y));
//
//        default:while(1); // error
//    }
    
    return y/((double)(x));
}



__ImageRGB888_t* __Blur_Average_ImgRGB888(const __ImageRGB888_t* src,__ImageRGB888_t* dst,uint32_t radSize, uint16_t br_100){
    __exitReturn(src == NULL || dst == NULL , NULL);
    
    __UNION_PixelRGB888_t* pSrcData = src->pBuffer;
    __UNION_PixelRGB888_t* pDstData = dst->pBuffer;
    if(pSrcData == dst->pBuffer){
        pDstData = __malloc(src->height*src->width*sizeof(__UNION_PixelRGB888_t));
    }
    
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
            sum_R += __array1D(pSrcData, src->width, n, m)->R;
            sum_G += __array1D(pSrcData, src->width, n, m)->G;
            sum_B += __array1D(pSrcData, src->width, n, m)->B;
        }
    }
    div = half_order * half_order;

    // Average Filter
    for(int j=0;j<src->height;j++){
        for(int i=0;i<src->width;i++){
            if(j%2 == 0){ // Scan Direction:  [old] -->--> [new]
                
                if(i!=0){ // No need to do when it reachs the left-edge because it has been done when moving to the next row.
                    
                    // Remove leftmost column because it is old.
                    if(i-half_order >= 0){                                          // [!] no cross the broad [0,src->width-1]
                        for(int row=j-half_order+1;row<=j+half_order-1;row++){
                            if( row<src->height && row>=0 ){                       // [!] no cross the broad [0,src->height-1]
                                sum_R -= __array1D(pSrcData, src->width, row, i-half_order)->R;
                                sum_G -= __array1D(pSrcData, src->width, row, i-half_order)->G;
                                sum_B -= __array1D(pSrcData, src->width, row, i-half_order)->B;
                                div--;
                            }
                        }
                    }
                    
                    // Add rightmost column because it is new.
                    if( i+half_order-1 < src->width ){                             // [!] no cross the broad [0,src->width-1]
                        for(int row=j-half_order+1;row<=j+half_order-1;row++){
                            if( row<src->height && row>=0 ){                       // [!] no cross the broad [0,src->height-1]
                                sum_R += __array1D(pSrcData, src->width, row, i+half_order-1)->R;
                                sum_G += __array1D(pSrcData, src->width, row, i+half_order-1)->G;
                                sum_B += __array1D(pSrcData, src->width, row, i+half_order-1)->B;
                                div++;
                            }
                        }
                    }
                    
                }
                __array1D(pDstData, src->width, j, i)->R = sum_R*br_100/(div*100);
                __array1D(pDstData, src->width, j, i)->G = sum_G*br_100/(div*100);
                __array1D(pDstData, src->width, j, i)->B = sum_B*br_100/(div*100);
            }else{ // Scan Direction:  [new] <--<-- [old]
                int k = (int)(src->width)-i-1; // reverse i
                // Remove rightmost column because it is old.
                if( k != src->width-1 ){ // No need to do when it reachs the right-edge because it has been done when moving to the next row.
                    if(k+half_order < src->width ){                                // [!] no cross the broad [0,src->width-1]
                        for(int row=j-half_order+1;row<=j+half_order-1;row++){
                            if( row<src->height && row>=0 ){                       // [!] no cross the broad [0,src->height-1]
                                sum_R -= __array1D(pSrcData, src->width, row, k+half_order)->R;
                                sum_G -= __array1D(pSrcData, src->width, row, k+half_order)->G;
                                sum_B -= __array1D(pSrcData, src->width, row, k+half_order)->B;
                                div--;
                            }
                        }
                    }
                    
                    // Add leftmost column because it is new.
                    if(k-half_order+1 >=0 ){                                       // [!] no cross the broad [0,src->width-1]
                        for(int row=j-half_order+1;row<=j+half_order-1;row++){
                            if( row<src->height && row>=0 ){                       // [!] no cross the broad [0,src->height-1]
                                sum_R += __array1D(pSrcData, src->width, row, k-half_order+1)->R;
                                sum_G += __array1D(pSrcData, src->width, row, k-half_order+1)->G;
                                sum_B += __array1D(pSrcData, src->width, row, k-half_order+1)->B;
                                div++;
                            }
                        }
                    }
                }
                __array1D(pDstData, src->width, j, k)->R = sum_R*br_100/(div*100);
                __array1D(pDstData, src->width, j, k)->G = sum_G*br_100/(div*100);
                __array1D(pDstData, src->width, j, k)->B = sum_B*br_100/(div*100);
            }
            // End of scanning of this row.
        }
        
        // Remove topmost row because it is old.
        if( j-half_order+1 >= 0 ){         // [!] no cross the broad [0,src->height-1]
            
            if(j%2 == 0){ // Scan Direction:  [old] -->--> [new]. Now it is reaching the rightmost.
                for(int col=(int)(src->width-half_order);col<src->width;col++){
                    sum_R -= __array1D(pSrcData, src->width, j-half_order+1, col)->R;
                    sum_G -= __array1D(pSrcData, src->width, j-half_order+1, col)->G;
                    sum_B -= __array1D(pSrcData, src->width, j-half_order+1, col)->B;
                    div--;
                }
            }else{        // Scan Direction:  [new] <--<-- [old]. Now it is reaching the leftmost.
                for(int col=0;col<half_order;col++){
                    sum_R -= __array1D(pSrcData, src->width, j-half_order+1, col)->R;
                    sum_G -= __array1D(pSrcData, src->width, j-half_order+1, col)->G;
                    sum_B -= __array1D(pSrcData, src->width, j-half_order+1, col)->B;
                    div--;
                }
            }

        }
        // Add downmost row because it is new.
        if(j+half_order < src->height ){         // [!] no cross the broad [0,src->height-1]
            
            if(j%2 == 0){ // Scan Direction:  [old] -->--> [new]. Now it is reaching the rightmost.
                for(int col=(int)(src->width-half_order);col<src->width;col++){
                    sum_R += __array1D(pSrcData, src->width, j+half_order, col)->R;
                    sum_G += __array1D(pSrcData, src->width, j+half_order, col)->G;
                    sum_B += __array1D(pSrcData, src->width, j+half_order, col)->B;
                    div++;
                }
            }else{        // Scan Direction:  [new] <--<-- [old]. Now it is reaching the leftmost.
                for(int col=0;col<half_order;col++){
                    sum_R += __array1D(pSrcData, src->width, j+half_order, col)->R;
                    sum_G += __array1D(pSrcData, src->width, j+half_order, col)->G;
                    sum_B += __array1D(pSrcData, src->width, j+half_order, col)->B;
                    div++;
                }
            }
        }
        
    }
    if(src->pBuffer == dst->pBuffer){
        memcpy(dst->pBuffer,pDstData,src->height*src->width*sizeof(__UNION_PixelRGB888_t));
        __free(pDstData);
    }
    return dst;
}








__ImageRGB888_t* __Blur_Average_ImgRGB888(const __ImageRGB888_t* src,__ImageRGB888_t* dst,uint32_t radSize, uint16_t br_100){
    __exitReturn(src == NULL || dst == NULL , NULL);
    
    __UNION_PixelRGB888_t* pSrcData = src->pBuffer;
    __UNION_PixelRGB888_t* pDstData = dst->pBuffer;
    if(src->pBuffer == dst->pBuffer){
        pDstData = __malloc(src->height*src->width*sizeof(__UNION_PixelRGB888_t));
    }
    
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
    if(src->pBuffer == dst->pBuffer){
        //...//
        __free(pDstData);
    }
    return NULL;
}



















