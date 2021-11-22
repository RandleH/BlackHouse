

#include "BLK_math.h"

#ifdef __cplusplus
extern "C" {}
#endif
/*===========================================================================================================================
 > Algebra Reference
============================================================================================================================*/
long BLK_FUNC( Math, sign )(long x){
    return (x>=0)?(1):(-1);
}
      
long BLK_FUNC( Math, step )(long x){
    return (long)( x >= 0 );
}

long BLK_FUNC( Math, sqrt )(long x){
    if(x <= 0) return 0;
    long l   = 1;
    long r   = x;
    long res = 0;
    while(l <= r){
        long mid=(l+r)>>1;
        if(mid <= x/mid){
          l   = mid+1;
          res = mid;
      }else{
          r = mid-1;
      }
    }
    if( ((res+1)*(res+1) - x) > (x - res*res) )
        return res;
    return (res+1);
}

/*========================================================================================================
 > DEC                        RAD                          TAN
 =========================================================================================================
 > Φ = 45                     0.78539816339744828          tan(Φ) = 1/1    
 > Φ = 26.565051177078        0.46364760900080609          tan(Φ) = 1/2    
 > Φ = 14.0362434679265       0.24497866312686414          tan(Φ) = 1/4    
 > Φ = 7.1250163489018        0.12435499454676144          tan(Φ) = 1/8    
 > Φ = 3.57633437499735       0.06241880999595735          tan(Φ) = 1/16   
 > Φ = 1.78991060824607       0.031239833430268277         tan(Φ) = 1/32   
 > Φ = 0.8951737102111        0.015623728620476831         tan(Φ) = 1/64   
 > Φ = 0.4476141708606        0.0078123410601011111        tan(Φ) = 1/128  
 > Φ = 0.2238105003685        0.0039062301319669718        tan(Φ) = 1/256  
 > Φ = 0.1119056770662        0.0019531225164788188        tan(Φ) = 1/512  
 > Φ = 0.0559528918938        0.00097656218955931946       tan(Φ) = 1/1024 
 > Φ = 0.027976452617         0.00048828121119489829       tan(Φ) = 1/2048 
 > Φ = 0.01398822714227       0.00024414062014936177       tan(Φ) = 1/4096 
 > Φ = 0.006994113675353      0.00012207031189367021       tan(Φ) = 1/8192 
 > Φ = 0.003497056850704      0.000061035156174208773      tan(Φ) = 1/16384
//========================================================================================================*/
    
const static uint16_t angle_dec_256[] = {
    // When 1 deg = 256, then...
    11520 , 
    6801  , 
    3593  , 
    1824  , 
    916   , 
    458   , 
    229   , 
    115   , 
    57    , 
    29    , 
    14    , 
    7     , 
    4     , 
    2     , 
    1       
};


double        
BLK_FUNC( Math, tan           )( long   dec ){

    long x = 0xffff;
    long y = 0;
    long angle_tar = dec<<8;
    long angle_sum = 0;
    long x_tmp = 0,y_tmp = 0;
    
    for( int i=0; i<16; i++ ){
        if( angle_tar < angle_sum ){ // cw
            x_tmp = (x + (y>>i));
            y_tmp = (y - (x>>i));
            x  = x_tmp;
            y  = y_tmp;
            angle_sum -= angle_dec_256[i];
        }else if(angle_tar > angle_sum){ // ccw
            x_tmp = (x - (y>>i));
            y_tmp = (y + (x>>i));
            x  = x_tmp;
            y  = y_tmp;
            angle_sum += angle_dec_256[i];
        }else{
            break;
        }
    }
    return y/((double)(x));
}

double        
BLK_FUNC( Math, atan          )( long   y, long   x ){
    
    __exitReturn( x==0 && y==0 ,    0 );
    __exitReturn( x==0 && y<0  ,  -90 );
    __exitReturn( x==0 && y>0  ,   90 );
    __exitReturn( y==0 && x<0  ,    0 );
    __exitReturn( y==0 && x>0  ,    0 );
    __exitReturn( x==y         ,   45 );
    __exitReturn( x==-y        ,  -45 );

    long angle_sum = 0;
    long x_tmp,y_tmp;
    
    y<<=10;
    x<<=10;
    
    for( int i=0; i<15; i++ ){
        if( y>0 ){ // cw
            x_tmp = (x + (y>>i));
            y_tmp = (y - (x>>i));
            x  = x_tmp;
            y  = y_tmp;
            angle_sum += angle_dec_256[i];
        }else if(y<0){ // ccw
            x_tmp = (x - (y>>i));
            y_tmp = (y + (x>>i));
            x  = x_tmp;
            y  = y_tmp;
            angle_sum -= angle_dec_256[i];
        }
    }
    
    return (angle_sum/256.0);
}
      
double        
BLK_FUNC( Math, sigmold       )( double x ){
    return (double)(1.0/(1+exp(-x)));
}

long          
BLK_FUNC( Math, factorial     )( long   x ){ 
    // [!] Limitation: x should be smaller than 20
    __exitReturn(x<0 , -1);
    __exitReturn(x==0,  1);
    long res = 1;
    while(--x){
        res*=(x+1);
    }
    return res;
}
      
long          
BLK_FUNC( Math, fibonacci     )( long   n ){
    __exitReturn(n<0, -1);
    
    if(n==0)
        return 1;
    long res = 0;
    long fnm1 = 1,fnm2 = 0;
    for (int i=2; i<=n+1; i++) {
        res  = fnm1+fnm2;
        fnm2 = fnm1;
        fnm1 = res;
    }
    return res;
}
    
long
BLK_FUNC( Math, gcd           )( long   a, long b  ){
#define THIS  BLK_FUNC(Math, gcd)
    if( b>a )
        __swap(a, b);
    
    if( b==0 )
        return a;
    
    if( !(a&1) ){
        if( !(b&1) ) return (long)(THIS(a>>1, b>>1) <<1);
        else         return (long)(THIS(a>>1, b   )    );
    }else{
        if( !(b&1) ) return (long)(THIS(a   , b>>1)    );
        else         return (long)(THIS(a-b , b   )    );
    }
    
#undef THIS
    
}

unsigned long 
BLK_FUNC( Math, combinatorial )( unsigned long n, unsigned long r ){
    __exitReturn( r>n , 0 );
    #if 0
    { // 初级算法
        unsigned long son = 1, mum = 1;
        for(unsigned long i=0; i<r;  ){
            son*=(n-i);
            mum*=(++i);
        }
        return (unsigned long)(son/mum);
    }
    #elif 1
    { // 进阶算法 1
        uint16_t *temp = alloca(r*sizeof(uint16_t));
        for( uint16_t i=0; i<r; i++ ){
            temp[i] = (n-r+i+1);
        }
        
        unsigned long mum=1, son=1;
        // 先进行约分, 否则被除数太大
        for( int32_t i=(int32_t)(r-1); i>=0; i-- ){
            int32_t j=(int32_t)(r-1);
            
            for( ; j>=0; j-- ){
                if( temp[j]%(i+1)==0 ){
                    temp[j]/=(i+1);
                    break;
                }
            }
            if( j<0 )
                mum *= i+1;
        }
        
        for( uint16_t i=0; i<r; i++ ){
            son*=temp[i];
        }
        
        return (unsigned long)(son/mum);
    }
    
    #else // 进阶算法 2
    {
        unsigned long son = 1, mum = 1;
        for(unsigned long i=0; i<r;  ){
            son*=(n-i);
            mum*=(++i);
            long gcd = BLK_FUNC(Math, gcd)( son, mum );
            son /= gcd;
            mum /= gcd;
        }
        return son;
        
    }
    #endif
}
    
int           
BLK_FUNC( Math, rand          )( void    ){
    static uint32_t a = (uint32_t)M_MATH_SEED; /*Seed*/

    /*Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs"*/
    uint32_t x = a;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    a = x;

    return (int)a;
}
    
int           
BLK_FUNC( Math, rand_in       )( int min, int max ){
    return ( (unsigned)BLK_FUNC(Math, rand)() % (max - min + 1)) + min;
}

BLK_ENUM(Monotonicity)
BLK_FUNC( Math, dir_line      )( int x1,int y1,int x2,int y2 ){
    if(x1==x2)
        return kBLK_Monotonicity_ver;
    if(y1==y2)
        return kBLK_Monotonicity_hor;

    return ( (((x2-x1)<0)&&((y2-y1)<0)) || (((x2-x1)>0)&&((y2-y1)>0)) )?(kBLK_Monotonicity_inc):(kBLK_Monotonicity_dec);
}




long              
BLK_FUNC( Math, area_triangle )( int x1,int y1,int x2,int y2,int x3,int y3 ){
    
   /* 
     可用的测试用例:
     Params                  | Answers
    ========================================
     ( 20,50, -40,25,  15, 15)  = 987.5
     (-40,25,  20,50,  15, 15)  = ...
     (-40,25,  15,15,  20, 50)  = ...
     (-40,25,  15,15, -20,-60)  = 2237.5
     (-40,25, -20,-60, 15, 15)  = ...
     (-400,25, -200,-60, 150, 15)
   */
    
    int type = ((x1<x2)<<2) | ((x1<x3)<<1) | ((x2<x3)<<0);
    int area = -1;
    switch(type){
        case 0: // x1 >= x2 >= x3
            area = __abs(( (y2+y3)*(x2-x3)+(y1+y2)*(x1-x2)-(y1+y3)*(x1-x3) ));
            break;
        case 1: // x1 >= x3 >= x2
            area = __abs(( (y3+y2)*(x3-x2)+(y1+y3)*(x1-x3)-(y1+y2)*(x1-x2) ));
            break;
        default:// Should not be runned here.
        case 2: // impossible
            RH_ASSERT(0);
        case 3: // x3 >  x1 >= x2
            area = __abs(( (y1+y2)*(x1-x2)+(y3+y1)*(x3-x1)-(y3+y2)*(x3-x2) ));
            break;
        case 4: // x2 >  x1 >= x3
            area = __abs(( (y1+y3)*(x1-x3)+(y2+y1)*(x2-x1)-(y2+y3)*(x2-x3) ));
            break;
        case 5: // x2 >  x1 >= x3
            area = __abs(( (y1+y3)*(x1-x3)+(y2+y1)*(x2-x1)-(y2+y3)*(x2-x3) ));
            break;
        case 6: // x2 >  x3 >  x1
            area = __abs(( (y3+y1)*(x3-x1)+(y2+y3)*(x2-x3)-(y2+y1)*(x2-x1) ));
            break;
        case 7: // x3 >  x2 >  x1
            area = __abs(( (y2+y1)*(x2-x1)+(y3+y2)*(x3-x2)-(y3+y1)*(x3-x1) ));
            break;
    }

    return (int)((area>>1) + (area&0x00));    
}

long              
BLK_FUNC( Math, area_rectangular )(int x1,int y1,int x2,int y2){
    return __abs((x2-x1)*(y2-y1));
}

float
BLK_FUNC( Math, poisson          )( int xs, int xe, int lamda ){
    float res = 0.0;
    for( int i=xs; i<=xe; i++ ){
        res += powf(lamda,i)/(float)BLK_FUNC(Math,factorial)(i);
    }
    res *= exp(-lamda);
    return res;
}

double
BLK_FUNC( Gussian, func_1D    )( long x,long __miu,double __sigma ){
    // Same Effect but slower,only suitable when __sigma is a value of double.
    #if 1
    return ((__sigma==0)?(0):(double)((M_2_SQRTPI/((__sigma*2)*(M_SQRT2)))*exp(-(x-__miu)*(x-__miu)/(double)((__sigma*__sigma)*2))));
    #else
    return ((__sigma==0)?(0):(double)((M_2_SQRTPI/((__sigma<<1)*(M_SQRT2)))*exp(-(x-__miu)*(x-__miu)/(double)((__sigma*__sigma)<<1))));
    #endif
}

double
BLK_FUNC( Gussian, func_2D    )( long x,long y,double __sigma ){
    // Same Effect but slower,only suitable when __sigma is a value of double.
    #if 1
    return ((__sigma==0)?(0):((double)((1/(2*M_PI*__sigma*__sigma))*exp(-((x*x)+(y*y))/(double)((__sigma*__sigma)*2)))));
    #else
    return ((__sigma==0)?(0):((double)((1/(M_PI*__sigma*(__sigma<<1)))*exp(-((x*x)+(y*y))/(double)((__sigma*__sigma<<1))))));
    #endif
}

double
BLK_FUNC( Gussian, func_2D_XY )( long x,long y,double sigma_x,double sigma_y,double miu_x, double miu_y){
    double b = exp(-0.5*( ((x-miu_x)*(x-miu_x))/(sigma_x*sigma_x) + ((y-miu_y)*(y-miu_y))/(sigma_y*sigma_y) ));
    return (double)(b/(2.0*M_PI*sigma_x*sigma_y));
}

BLK_SRCT(Kernel)*
BLK_FUNC( Gussian, kernel     )( double __sigma,size_t order,BLK_SRCT(Kernel)* pKernel ){
    if( pKernel == NULL || pKernel->pBuffer == NULL ){
        return NULL;
    }
    
    if( __sigma == 0){ // In case of divider being zero.
        memset(pKernel->pBuffer,0,order*order*sizeof(*(pKernel->pBuffer)));
        pKernel->order = order;
        pKernel->sum = 0;
        return pKernel;
    }
    
    size_t half_order = ((order-1)>>1); // The index of the middle element. eg:  x x x ^ x x x ,which is "3".
    uint16_t* pCenter = pKernel->pBuffer + (half_order * order) + half_order;
    double coe        = 1.0/BLK_FUNC( Gussian, func_2D )(half_order,half_order,__sigma); // Make sure every element is larger than 0.
    
    pKernel->order = order;
    pKernel->sum = 0;
    for(int x=0;x<=half_order;x++){
        for(int y=0;y<=x;y++){
            uint16_t temp = lround(coe*BLK_FUNC( Gussian, func_2D )(x,y,__sigma));
            *(pCenter + (y*order) + x) = temp;
            *(pCenter - (y*order) + x) = temp;
            *(pCenter + (y*order) - x) = temp;
            *(pCenter - (y*order) - x) = temp;
            *(pCenter + (x*order) + y) = temp;
            *(pCenter - (x*order) + y) = temp;
            *(pCenter + (x*order) - y) = temp;
            *(pCenter - (x*order) - y) = temp;
            if(x == 0 && y == 0)
                pKernel->sum += temp;
            else if( x==0 || y == 0 || x==y )
                pKernel->sum += (temp<<2);
            else
                pKernel->sum += (temp<<3);
            
        }
    }
    return pKernel;
}
      
long
BLK_FUNC( Pascal, triangle    )( long row, long col ){

    /*        

                1 --------------- row = 0
              1   1 ------------- row = 1    
            1   2   1 ----------- row = 2
          1   3   3   1 --------- row = 3
        1   4   6   4   1 ------- row = 4
        |      ...      |
        |               |
        |               |
        |               |
    col = 0         col = 4

    */


    __exitReturn( col>row || col<0 || row<0 , -1 );
    return BLK_FUNC(Math,combinatorial)(row,col);
}
      
bool
BLK_FUNC( Mandelbrot, set     )( float complex c, size_t nitersLimit ){
    __exitReturn( cabs(c)<= 0.25, true  );
    __exitReturn( cabs(c) > 2.0 , false );

    float complex z = 0 + 0*I;

    while( nitersLimit-- && cabs(z)<=2 ){
        z*=z;
        z+=c;
    }
    
    return (nitersLimit == 0)?true:false;
}

#include "BLK_image.h"
void 
BLK_FUNC( Mandelbrot, image   )( float complex center, int img_w, int img_h, float scale_x, float scale_y, void* buf, void (*func)(int x,int y,uint32_t nIter, void* buf) ){
    uint32_t nitersLimit = 0xff;

    float x0 = creal(center);
    float y0 = cimag(center);

    
    for( int y=0; y<img_h; y++){
        for ( int x=0; x<img_w; x++){
           
            double px = (x-(img_w>>1))*scale_x-x0;
            double py = (y-(img_h>>1))*scale_y-y0;
            
            if(x == 533 && y==462){
                printf("%.22f,%.22f\n",px,py);
            }
            float complex c = px + py*I;

            // 检查是否在集合内
            {
                float complex z      = 0 + 0*I;
                uint32_t      niters = nitersLimit;
                while( niters-- && cabs(z)<=2 ){
                    z*=z;
                    z+=c;
                }
                if(nitersLimit == 0){
                    (*func)(x,y,     -1, buf);
                }else{
                    (*func)(x,y, nitersLimit-niters, buf);
                }
            }
            
        }
    }

    BLK_SRCT(Img888)* dst = (BLK_SRCT(Img888)*)buf;

    (dst->ptr + (dst->h>>1)*dst->w + (dst->w>>1))->R = 0x00;
    (dst->ptr + (dst->h>>1)*dst->w + (dst->w>>1))->G = 0x00;
    (dst->ptr + (dst->h>>1)*dst->w + (dst->w>>1))->B = 0x00;

    
}

    
/*===========================================================================================================================
 > Geometry Reference
============================================================================================================================*/

BLK_SRCT(Point2D)  BLK_FUNC( Point2D, find_Ymax )( BLK_SRCT(Point2D)* pts, size_t size ){
    RH_ASSERT(pts);
    RH_ASSERT(size);
    
    BLK_SRCT(Point2D) pt = {.x = pts->x, .y = pts->y};
    for(size_t i=0; i<size; i++,pts++){
        if( pts->y > pt.y ){
            pt = *pts;
        }
    }
    return pt;
}
BLK_SRCT(Point2D)  BLK_FUNC( Point2D, find_Ymin )( BLK_SRCT(Point2D)* pts, size_t size ){
    RH_ASSERT(pts);
    RH_ASSERT(size);
    
    BLK_SRCT(Point2D) pt = {.x = pts->x, .y = pts->y};
    for(size_t i=0; i<size; i++,pts++){
        if( pts->y < pt.y ){
            pt = *pts;
        }
    }
    return pt;
}
BLK_SRCT(Point2D)  BLK_FUNC( Point2D, find_Xmax )( BLK_SRCT(Point2D)* pts, size_t size ){
    RH_ASSERT(pts);
    RH_ASSERT(size);
    
    BLK_SRCT(Point2D) pt = {.x = pts->x, .y = pts->y};
    for(size_t i=0; i<size; i++,pts++){
        if( pts->x > pt.x ){
            pt = *pts;
        }
    }
    return pt;
}
BLK_SRCT(Point2D)  BLK_FUNC( Point2D, find_Xmin )( BLK_SRCT(Point2D)* pts, size_t size ){
    RH_ASSERT(pts);
    RH_ASSERT(size);
    
    BLK_SRCT(Point2D) pt = {.x = pts->x, .y = pts->y};
    for(size_t i=0; i<size; i++,pts++){
        if( pts->x < pt.x ){
            pt = *pts;
        }
    }
    return pt;
}


Point3D_t __findPoint_VectorDistance   (const Point3D_t*  A,const Point3D_t*  B,int dist_AP){
    long dist_AB = lroundl(sqrt( (B->x - A->x)*(B->x - A->x) + \
                                (B->y - A->y)*(B->y - A->y) + \
                                (B->z - A->z)*(B->z - A->z)  ));
    Point3D_t result = {
        .x = (B->x - A->x)*dist_AP/dist_AB + A->x ,
        .y = (B->y - A->y)*dist_AP/dist_AB + A->y ,
        .z = (B->z - A->z)*dist_AP/dist_AB + A->z ,
    };

    return result;
}

Point3D_t __findPoint_VectorProportion (const Point3D_t*  A,const Point3D_t*  B,double scale){
    Point3D_t result = {
        .x = (B->x - A->x)*scale + A->x ,
        .y = (B->y - A->y)*scale + A->y ,
        .z = (B->z - A->z)*scale + A->z ,
    };
    return result;
}

int
BLK_FUNC( Vector2D, dot       )( const BLK_SRCT(Vector2D)* vect1,const BLK_SRCT(Vector2D)* vect2){
    return (int)((vect1->x*vect2->x)+(vect1->y*vect2->y));
}

int
BLK_FUNC( Vector3D, dot       )( const BLK_SRCT(Vector3D)* vect1,const BLK_SRCT(Vector3D)* vect2){
    return (int)((vect1->x*vect2->x)+(vect1->y*vect2->y)+(vect1->z*vect2->z));
}

BLK_SRCT(Vector3D)
BLK_FUNC( Vector3D, cross     )( const BLK_SRCT(Vector3D)* vect1,const BLK_SRCT(Vector3D)* vect2){
    BLK_SRCT(Vector3D) vecResult = {.x = ( vect1->y*vect2->z - vect1->z*vect2->y),\
                            .y = ( vect1->z*vect2->x - vect1->x*vect2->z),\
                            .z = ( vect1->x*vect2->y - vect1->y*vect2->x)};
    return vecResult;
}

BLK_ENUM(PtPos)
BLK_FUNC( Math, pt_citcle     )( int xc,int yc,int r,                       int px,int py){
    long key = (xc-px)*(xc-px)+(yc-py)*(yc-py);
    long r_2 = r*r;
    if(key > r_2)
        return kBLK_PtPos_outside;
    else if(key < r_2)
        return kBLK_PtPos_inside;
    else
        return kBLK_PtPos_righton;
}

BLK_ENUM(PtPos)
BLK_FUNC( Math, pt_line       )( int x1,int y1,int x2,int y2,               int px,int py ){
    int param_1 = (x2>x1)?( (x2-x1)*py ):( (x1-x2)*py );
    int param_2 = (x2>x1)?( (y2-y1)*px+(y2*(x2-x1)-x2*(y2-y1)) ):( (y1-y2)*px+(y2*(x1-x2)-x2*(y1-y2)) );

    if(param_1 > param_2)
        return kBLK_PtPos_above;
    else if(param_1 < param_2)
        return kBLK_PtPos_beneath;
    else
        return kBLK_PtPos_righton;
}
    

BLK_ENUM(PtPos)
BLK_FUNC( Math, pt_cord2D     )( int px,int py){
    if( px==0&&py==0 ) return kBLK_PtPos_at_orgin;
    if( py==0 ){
        if( px>0 )  return kBLK_PtPos_at_axisXp;
        else        return kBLK_PtPos_at_axisXm;
    } 
    if( px==0 ){
        if( py>0 )  return kBLK_PtPos_at_axisYp;
        else        return kBLK_PtPos_at_axisYm;
    }
    uint8_t key = ((py<0)<<1)|(px<0);
    return (BLK_ENUM(PtPos))(((key>>1)^key) +4);
}

BLK_ENUM(PtPos)
BLK_FUNC( Math, pt_triangle   )( int x1,int y1,int x2,int y2,int x3,int y3, int px,int py ){
 #if 0
    // Condition:
    // P = A + u*(CA) + v*(BA)
    // u >= 0 && v >= 0 && u+v <= 1
    
    // Any point can be represented by: (PA) = u*(CA) + v*(BA)
    //
    // When both multiply by (CA) and (BA):
    // (PA)·(CA) = u*[(CA)·(CA)] + v*[(BA)·(CA)]
    // (PA)·(BA) = u*[(BA)·(CA)] + v*[(BA)·(BA)]
    
    // Then:
    //         [(BA)·(BA)]*[(PA)·(CA)] - [(BA)·(CA)]*[(PA)·(BA)]
    // u = ---------------------------------------------------------
    //         [(CA)·(CA)]*[(BA)·(BA)] - [(CA)·(BA)]*[(BA)·(CA)]
    
    //         [(CA)·(CA)]*[(PA)·(BA)] - [(CA)·(CA)]*[(PA)·(CA)]
    // v = ---------------------------------------------------------
    //         [(CA)·(CA)]*[(BA)·(BA)] - [(CA)·(BA)]*[(BA)·(CA)]
    
    // Assume A = (x1,y1) | B = (x2,y2) | C = (x3,y3) :
    struct BLK_SRCT(Vector2D) v0 = {.x = x3-x1,.y = y3-y1};
    struct BLK_SRCT(Vector2D) v1 = {.x = x2-x1,.y = y2-y1};
    struct BLK_SRCT(Vector2D) v2 = {.x = px-x1,.y = py-y1};

    int v00 = BLK_FUNC(Vector2D, dot)(&v0,&v0);
    int v01 = BLK_FUNC(Vector2D, dot)(&v0,&v1);
    int v02 = BLK_FUNC(Vector2D, dot)(&v0,&v2);
    int v11 = BLK_FUNC(Vector2D, dot)(&v1,&v1);
    int v12 = BLK_FUNC(Vector2D, dot)(&v1,&v2);

    int u = v11*v02-v01*v12;
    int v = v00*v12-v01*v02;
    int d = v00*v11-v01*v01;
    if(u<0 || v<0)
        return -1;
    else if(u==0 || v==0)
        return 0;

    if(u+v > d)
        return -1;
    else if(u+v < d)
        return 1;
    else
        return 0;
 #endif

 #if 0 // Wrong
    int signOfTrig = (x2 - x1)*(y3 - y1) - (y2 - y1)*(x3 - x1);
    int signOfAB   = (x2 - x1)*(py - y1) - (y2 - y1)*(px - x1);
    int signOfCA   = (x1 - x3)*(py - y3) - (y1 - y3)*(px - x3);
    int signOfBC   = (x3 - x2)*(py - y3) - (y3 - y2)*(px - x2);
  
    bool d1 = (signOfAB<0&&signOfTrig<0) || (signOfAB>0&&signOfTrig>0);
    bool d2 = (signOfCA<0&&signOfTrig<0) || (signOfCA>0&&signOfTrig>0);
    bool d3 = (signOfBC<0&&signOfTrig<0) || (signOfBC>0&&signOfTrig>0);
    
    return d1 && d2 && d3;
    
 #endif
 #if 1
    long p_area = BLK_FUNC(Math,area_triangle)( px,py, x1,y1, x2,y2  ) + \
                  BLK_FUNC(Math,area_triangle)( px,py, x2,y2, x3,y3  ) + \
                  BLK_FUNC(Math,area_triangle)( px,py, x1,y1, x3,y3  ) ;
    long t_area = BLK_FUNC(Math,area_triangle)( x1,y1, x2,y2, x3,y3  );
    
    if( p_area > t_area )
        return kBLK_PtPos_outside;
    else
        return kBLK_PtPos_inside;
    
 #endif
}




static float __blk_std_normal_table[] = {
//         .00      .01      .02      .03      .04      .05      .06      .07      .08      .09
/* -3.9 */ 0.00005, 0.00005, 0.00004, 0.00004, 0.00004, 0.00004, 0.00004, 0.00004, 0.00003, 0.00003,\
/* -3.8 */ 0.00007, 0.00007, 0.00007, 0.00006, 0.00006, 0.00006, 0.00006, 0.00005, 0.00005, 0.00005,\
/* -3.7 */ 0.00011, 0.00010, 0.00010, 0.00010, 0.00009, 0.00009, 0.00008, 0.00008, 0.00008, 0.00008,\
/* -3.6 */ 0.00016, 0.00015, 0.00015, 0.00014, 0.00014, 0.00013, 0.00013, 0.00012, 0.00012, 0.00011,\
/* -3.5 */ 0.00023, 0.00022, 0.00022, 0.00021, 0.00020, 0.00019, 0.00019, 0.00018, 0.00017, 0.00017,\
/* -3.4 */ 0.00034, 0.00032, 0.00031, 0.00030, 0.00029, 0.00028, 0.00027, 0.00026, 0.00025, 0.00024,\
/* -3.3 */ 0.00048, 0.00047, 0.00045, 0.00043, 0.00042, 0.00040, 0.00039, 0.00038, 0.00036, 0.00035,\
/* -3.2 */ 0.00069, 0.00066, 0.00064, 0.00062, 0.00060, 0.00058, 0.00056, 0.00054, 0.00052, 0.00050,\
/* -3.1 */ 0.00097, 0.00094, 0.00090, 0.00087, 0.00084, 0.00082, 0.00079, 0.00076, 0.00074, 0.00071,\
/* -3.0 */ 0.00135, 0.00131, 0.00126, 0.00122, 0.00118, 0.00114, 0.00111, 0.00107, 0.00104, 0.00100,\
/* -2.9 */ 0.00187, 0.00181, 0.00175, 0.00169, 0.00164, 0.00159, 0.00154, 0.00149, 0.00144, 0.00139,\
/* -2.8 */ 0.00256, 0.00248, 0.00240, 0.00233, 0.00226, 0.00219, 0.00212, 0.00205, 0.00199, 0.00193,\
/* -2.7 */ 0.00347, 0.00336, 0.00326, 0.00317, 0.00307, 0.00298, 0.00289, 0.00280, 0.00272, 0.00264,\
/* -2.6 */ 0.00466, 0.00453, 0.00440, 0.00427, 0.00415, 0.00402, 0.00391, 0.00379, 0.00368, 0.00357,\
/* -2.5 */ 0.00621, 0.00604, 0.00587, 0.00570, 0.00554, 0.00539, 0.00523, 0.00508, 0.00494, 0.00480,\
/* -2.4 */ 0.00820, 0.00798, 0.00776, 0.00755, 0.00734, 0.00714, 0.00695, 0.00676, 0.00657, 0.00639,\
/* -2.3 */ 0.01072, 0.01044, 0.01017, 0.00990, 0.00964, 0.00939, 0.00914, 0.00889, 0.00866, 0.00842,\
/* -2.2 */ 0.01390, 0.01355, 0.01321, 0.01287, 0.01255, 0.01222, 0.01191, 0.01160, 0.01130, 0.01101,\
/* -2.1 */ 0.01786, 0.01743, 0.01700, 0.01659, 0.01618, 0.01578, 0.01539, 0.01500, 0.01463, 0.01426,\
/* -2.0 */ 0.02275, 0.02222, 0.02169, 0.02118, 0.02068, 0.02018, 0.01970, 0.01923, 0.01876, 0.01831,\
/* -1.9 */ 0.02872, 0.02807, 0.02743, 0.02680, 0.02619, 0.02559, 0.02500, 0.02442, 0.02385, 0.02330,\
/* -1.8 */ 0.03593, 0.03515, 0.03438, 0.03362, 0.03288, 0.03216, 0.03144, 0.03074, 0.03005, 0.02938,\
/* -1.7 */ 0.04457, 0.04363, 0.04272, 0.04182, 0.04093, 0.04006, 0.03920, 0.03836, 0.03754, 0.03673,\
/* -1.6 */ 0.05480, 0.05370, 0.05262, 0.05155, 0.05050, 0.04947, 0.04846, 0.04746, 0.04648, 0.04551,\
/* -1.5 */ 0.06681, 0.06552, 0.06426, 0.06301, 0.06178, 0.06057, 0.05938, 0.05821, 0.05705, 0.05592,\
/* -1.4 */ 0.08076, 0.07927, 0.07780, 0.07636, 0.07493, 0.07353, 0.07215, 0.07078, 0.06944, 0.06811,\
/* -1.3 */ 0.09680, 0.09510, 0.09342, 0.09176, 0.09012, 0.08851, 0.08691, 0.08534, 0.08379, 0.08226,\
/* -1.2 */ 0.11507, 0.11314, 0.11123, 0.10935, 0.10749, 0.10565, 0.10383, 0.10204, 0.10027, 0.09853,\
/* -1.1 */ 0.13567, 0.13350, 0.13136, 0.12924, 0.12714, 0.12507, 0.12302, 0.12100, 0.11900, 0.11702,\
/* -1.0 */ 0.15866, 0.15625, 0.15386, 0.15151, 0.14917, 0.14686, 0.14457, 0.14231, 0.14007, 0.13786,\
/* -0.9 */ 0.18406, 0.18141, 0.17879, 0.17619, 0.17361, 0.17106, 0.16853, 0.16602, 0.16354, 0.16109,\
/* -0.8 */ 0.21186, 0.20897, 0.20611, 0.20327, 0.20045, 0.19766, 0.19489, 0.19215, 0.18943, 0.18673,\
/* -0.7 */ 0.24196, 0.23885, 0.23576, 0.23270, 0.22965, 0.22663, 0.22363, 0.22065, 0.21770, 0.21476,\
/* -0.6 */ 0.27425, 0.27093, 0.26763, 0.26435, 0.26109, 0.25785, 0.25463, 0.25143, 0.24825, 0.24510,\
/* -0.5 */ 0.30854, 0.30503, 0.30153, 0.29806, 0.29460, 0.29116, 0.28774, 0.28434, 0.28096, 0.27760,\
/* -0.4 */ 0.34458, 0.34090, 0.33724, 0.33360, 0.32997, 0.32636, 0.32276, 0.31918, 0.31561, 0.31207,\
/* -0.3 */ 0.38209, 0.37828, 0.37448, 0.37070, 0.36693, 0.36317, 0.35942, 0.35569, 0.35197, 0.34827,\
/* -0.2 */ 0.42074, 0.41683, 0.41294, 0.40905, 0.40517, 0.40129, 0.39743, 0.39358, 0.38974, 0.38591,\
/* -0.1 */ 0.46017, 0.45620, 0.45224, 0.44828, 0.44433, 0.44038, 0.43644, 0.43251, 0.42858, 0.42465,\
/* -0.0 */ 0.50000, 0.49601, 0.49202, 0.48803, 0.48405, 0.48006, 0.47608, 0.47210, 0.46812, 0.46414,\
};

RH_PROTOTYPE float   __BLK_Math_std_normal          (float x){
    bool rev = false;
    
    if(x>0){
        x = -x;
        rev = true;
    }
    
    int m = (int)(roundf(-x*100));
    int i = m%10;
    int j = m/10;
    
    if( j>39 ){
        if( rev ) return 1.0;
        else      return 0.0;
    }
    
    RH_ASSERT( j<=39 && j>=0 );
        
    if( rev )  return (float)(1.0f-__blk_std_normal_table[ (39-j)*10+i ]);
               return              __blk_std_normal_table[ (39-j)*10+i ];
}

RH_PROTOTYPE float   __BLK_Math_prb_possion         (float lmda,                       int xs, int xe){
    float res = 0.0;
    for( int i=xs; i<=xe; i++ ){
//        printf("%f\n",powf(lmda,i));
//        printf("%f\n",(float)BLK_FUNC(Math,factorial)(i));
        
        float num = powf(lmda,i);
        float den = BLK_FUNC(Math,factorial)(i);
#define MAX_FACTORIAL   19
        
        
        switch ( ((isfinite(num))<<1)|(isfinite(den))  ) {
            case 0x00: // num = inf; den = inf;
                
                break;
            case 0x01: // num = inf;
                
                break;
            case 0x02: // den = inf;
                
                break;
            case 0x03: //
                res += num/den;
                break;
            default:
                RH_ASSERT(0);
        }
        
        
        if( i<MAX_FACTORIAL ){
            
        }else{
            float tmp =  powf(lmda,i)/(float)BLK_FUNC(Math,factorial)(MAX_FACTORIAL);
            for(int j=MAX_FACTORIAL+1; j<=i; j++){
                tmp /= j;
            }
            res += tmp;
        }
    }
    res *= exp(-lmda);
    
#undef MAX_FACTORIAL
    return res;
}
    
RH_PROTOTYPE float   __BLK_Math_prb_binormial       (int n, float p,                   int xs, int xe){
    float res = 0.0;
    
    for( int x=xs; x<=xe; x++ ){
        res += BLK_FUNC(Math,combinatorial)(n, x) * powf(p,x) * powf(1-p,n-x);
    }
    return res;
}

RH_PROTOTYPE float   __BLK_Math_prb_geomatric       (float p,                          int xs, int xe){
    float res = 0.0;
    
    for( int x=xs; x<=xe; x++ ){
        res += p * powf(1-p,x-1);
    }
    return res;
}

RH_PROTOTYPE float   __BLK_Math_prb_hypergeomatric  (int N, int K, int n,              int xs, int xe){
    float res = 0.0;
    
    for( int x=xs; x<=xe && x<=K; x++ ){
        res += BLK_FUNC(Math, combinatorial)(K,x) * BLK_FUNC(Math, combinatorial)(N-K,n-x) / BLK_FUNC(Math, combinatorial)(N,n);
    }
    return res;
}

RH_PROTOTYPE float   __BLK_Math_prb_negbinormial    (int r, float p,                   int xs, int xe){
    float res = 0.0;
    
    for( int x=xs; x<=xe; x++ ){
        res += BLK_FUNC(Math,combinatorial)(r+x-1, r-1) * powf(p,r) * powf(1-p,x);
    }
    return res;
}

RH_PROTOTYPE float   __BLK_Math_prb_exponential     (int lmda,                         int xs, int xe){
    return (float)(expf(-xe*lmda) - expf(-xs*lmda));
}

RH_PROTOTYPE float   __BLK_Math_prb_uniform         (int a, int b,                     int xs, int xe){
    return (float)((float)(xe-xs)/(float)(b-a));
}

RH_PROTOTYPE float   __BLK_Math_prb_normal          (float mu, float sig,              int xs, int xe){
    float _xs = ((float)xs-mu)/(float)sig;
    float _xe = ((float)xe-mu)/(float)sig;
    
    return (float)(__BLK_Math_std_normal(_xe)-__BLK_Math_std_normal(_xs));
}
    


RH_PROTOTYPE float   __BLK_Math_inf_entropy         (float p[], size_t nitems ){
    float res = 0.0;
    for(size_t i=0; i< nitems; i++){
        if(p[i] == 0) continue;
        res -= p[i]*log2f(p[i]);
    }
    return res;
}


RH_PROTOTYPE float*  __BLK_Math_fuzzy_deg_and       (const float a[], const float b[], float r[], size_t cnt){
    RH_ASSERT( a && b && r);
    for( size_t i=0; i< cnt ;i++ ){
        r[i] = RH_MIN(a[i], b[i]);
    }
    return r;
}
    
RH_PROTOTYPE float*  __BLK_Math_fuzzy_deg_or        (const float a[], const float b[], float r[], size_t cnt){
    RH_ASSERT( a && b && r);
    for( size_t i=0; i< cnt ;i++ ){
        r[i] = RH_MAX(a[i], b[i]);
    }
    return r;
}
    
RH_PROTOTYPE float*  __BLK_Math_fuzzy_deg_not       (const float a[],                  float r[], size_t cnt){
    RH_ASSERT( a && r);
    for( size_t i=0; i< cnt ;i++ ){
        r[i] = 1.0 - a[i];
    }
    return r;
}

RH_PROTOTYPE float   __BLK_Math_fuzzy_count         (const float a[],                             size_t cnt){
    float res = 0.0;
    for(size_t i=0; i< cnt; i++){
        res += a[i];
    }
    return res;
}

RH_PROTOTYPE float   __BLK_Math_fuzzy_degree        (const float a[],                             size_t cnt){

    float* tmp_1 = alloca(cnt*sizeof(float));
    float* tmp_2 = alloca(cnt*sizeof(float));
    float* tmp_3 = alloca(cnt*sizeof(float));
    
    
#define SET_NOT ((float*)(__BLK_Math_fuzzy_deg_not(a,tmp_1,      cnt)))
#define SET_OR  ((float*)(__BLK_Math_fuzzy_deg_or (a,tmp_1,tmp_2,cnt)))
#define SET_AND ((float*)(__BLK_Math_fuzzy_deg_and(a,tmp_1,tmp_3,cnt)))
    
    tmp_1 = SET_NOT;

    
    return (float)(__BLK_Math_fuzzy_count(SET_AND, cnt) / __BLK_Math_fuzzy_count(SET_OR,cnt));
}

RH_PROTOTYPE float   __BLK_Math_fuzzy_equality      (const float a[], const float b[],            size_t cnt){
    float* tmp = alloca(cnt*sizeof(float));
    
    float cnt_and = __BLK_Math_fuzzy_count( __BLK_Math_fuzzy_deg_and( a, b, tmp, cnt ), cnt );
    float cnt_or  = __BLK_Math_fuzzy_count( __BLK_Math_fuzzy_deg_or ( a, b, tmp, cnt ), cnt );
    
    float res = 1.0/ ( 1.0/__BLK_Math_fuzzy_subsethood(a, b, cnt)+1.0/__BLK_Math_fuzzy_subsethood(b, a, cnt) - 1);
    RH_ASSERT(res - (float)(cnt_and/cnt_or)<0.0000001);
    
    return (float)(cnt_and/cnt_or);
}

RH_PROTOTYPE float   __BLK_Math_fuzzy_subsethood    (const float a[], const float b[],            size_t cnt){
    float *tmp = alloca(cnt*sizeof(float));
    return (float)(  __BLK_Math_fuzzy_count( __BLK_Math_fuzzy_deg_and(a, b, tmp, cnt) , cnt)/__BLK_Math_fuzzy_count(a, cnt) );
    
}



RH_PROTOTYPE float   __BLK_Math_decibel             (float vo, float vi){
    float v = fabsf(vo/vi);
    return 20*logf(v);
}

RH_PROTOTYPE float   __BLK_Math_stirling            (int   n){
    return sqrtf(2*M_2_PI*n)*pow(n/M_E, n);
}

#ifdef __cplusplus
}
#endif
