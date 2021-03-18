
#include "RH_data.h"

#ifdef __cplusplus
extern "C" {
#endif

/*=====================================================================
 > Data Structure Reference
======================================================================*/
      
__LinkLoop_t * __LINK_Loop_createHead        ( void* object ){
    __LinkLoop_t *ptr = (__LinkLoop_t*)__malloc(sizeof(__LinkLoop_t));
#ifdef RH_DEBUG
    ASSERT( ptr );
#endif
    __SET_STRUCT_MB(__LinkLoop_t,__LinkLoop_t*,ptr,pNext  ,ptr);
    __SET_STRUCT_MB(__LinkLoop_t,__LinkLoop_t*,ptr,pPrev  ,ptr);
    ptr->object = object;

    return ptr;
}
      
__LinkLoop_t * __LINK_Loop_add               ( const __LinkLoop_t *pHead , void* object ){
#ifdef RH_DEBUG
    ASSERT( pHead );
#endif

    __LinkLoop_t* pNewNode  = (__LinkLoop_t*)__malloc( sizeof(__LinkLoop_t) );
#ifdef RH_DEBUG
    ASSERT( pNewNode );
#endif

    pNewNode->object = object;

    // Things to do for the new Node.
    __SET_STRUCT_MB(__LinkLoop_t,__LinkLoop_t*,pNewNode    ,pPrev,pHead->pPrev   );
    __SET_STRUCT_MB(__LinkLoop_t,__LinkLoop_t*,pNewNode    ,pNext,pHead          );

    // Things to do for the neighbour.
    __SET_STRUCT_MB(__LinkLoop_t,__LinkLoop_t*,pHead->pPrev,pNext,pNewNode           );
    __SET_STRUCT_MB(__LinkLoop_t,__LinkLoop_t*,pHead       ,pPrev,pNewNode           );
    
    return pNewNode;
}
    
__LinkLoop_t * __LINK_Loop_find              ( const __LinkLoop_t *pHead , void* object ){
#ifdef RH_DEBUG
    ASSERT( pHead );
#endif
    
    const __LinkLoop_t* pTmp  = pHead;
    
    do{
        if (pTmp->object ==  object ) {
            return (__LinkLoop_t*)pTmp;
        }
        pTmp = pTmp->pNext;
    }while( pTmp != pHead );
    
    return NULL;
}
    
void           __LINK_Loop_remove            ( __LinkLoop_t *pHead , void* object ){

#ifdef RH_DEBUG
    ASSERT( pHead );
#endif

    const __LinkLoop_t* pTmp      = pHead;
    
    do{
        if (pTmp->object ==  object ) {
            if( pTmp == pHead ){
                if( pTmp->pNext==pTmp->pPrev && pTmp->pPrev==pHead ){
                    pHead = NULL;
                    __free((void*)pTmp);
                }else{
                    pHead = (__LinkLoop_t*)( pHead->pNext );
                    __SET_STRUCT_MB(__LinkLoop_t, __LinkLoop_t*, pTmp->pPrev, pNext, pTmp->pNext);
                    __SET_STRUCT_MB(__LinkLoop_t, __LinkLoop_t*, pTmp->pNext, pPrev, pTmp->pPrev);
                    __free((void*)pTmp);
                }
            }else{
                // Connect the neighbour and isolate the <pTarget>.
                __SET_STRUCT_MB(__LinkLoop_t, __LinkLoop_t*, pTmp->pPrev, pNext, pTmp->pNext);
                __SET_STRUCT_MB(__LinkLoop_t, __LinkLoop_t*, pTmp->pNext, pPrev, pTmp->pPrev);
                // Same Effect: pTarget->pPrev->pNext = pTarget->pNext; // But to cope with <const>.
                // Same Effect: pTarget->pNext->pPrev = pTarget->pPrev; // But to cope with <const>.
                
                __SET_STRUCT_MB(__LinkLoop_t, __LinkLoop_t*, pTmp, pNext, NULL);
                __SET_STRUCT_MB(__LinkLoop_t, __LinkLoop_t*, pTmp, pPrev, NULL);
                // Same Effect: pTarget->pNext = NULL; // But to cope with <const>.
                // Same Effect: pTarget->pPrev = NULL; // But to cope with <const>.
                __free((void*)pTmp);
            }

            return;
        }
        pTmp = pTmp->pNext;
    }while( pTmp != pHead );
    
}
    
void           __LINK_Loop_removeAll         ( __LinkLoop_t *pHead ){

#ifdef RH_DEBUG
    ASSERT( pHead );
#endif
    const __LinkLoop_t* pTmp  = pHead;
    do{
        pTmp = pTmp->pNext;
        __free( (void*)(pTmp->pPrev) );
    }while( pTmp != pHead );
    
}

void           __LINK_Loop_printAllNodesAdr  ( const __LinkLoop_t *pHead , int(*PRINTF_FUNC)(const char*,...)){
  
#ifdef RH_DEBUG
    ASSERT( pHead );
#endif
    const __LinkLoop_t* pTmp      = pHead;
    size_t cnt = 0;
    do{
        (*PRINTF_FUNC)("NODE[%d]: %p\n",cnt,pTmp);
        cnt++;
        pTmp = pTmp->pNext;
    }while( pTmp != pHead );

}
    

    
__LinkBiTree_t* __LINK_BiTree_createHead     ( void* object ){
    __LinkBiTree_t* pNewHead = (__LinkBiTree_t*)__malloc(sizeof(__LinkBiTree_t));
#ifdef RH_DEBUG
    ASSERT( pNewHead );
#endif
    __SET_STRUCT_MB(__LinkBiTree_t, __LinkBiTree_t*,pNewHead,pLeft  ,NULL);
    __SET_STRUCT_MB(__LinkBiTree_t, __LinkBiTree_t*,pNewHead,pRight ,NULL);
    __SET_STRUCT_MB(__LinkBiTree_t, __LinkBiTree_t*,pNewHead,pPrev  ,NULL);
    pNewHead->object = object;

    return pNewHead;
}
    
__LinkBiTree_t* __LINK_BiTree_add_l2r        ( const __LinkBiTree_t *pHead , __LinkBiTree_t *pTarget , void* object ){

#ifdef RH_DEBUG
    ASSERT( pHead );
    ASSERT( !(pHead->pPrev) );  // Head Node shouldn't have a previous node. So...Not a headnode.
    ASSERT( pTarget );
#endif
    __LinkBiTree_t* pNewNode = (__LinkBiTree_t*)__malloc(sizeof(__LinkBiTree_t));
#ifdef RH_DEBUG
    ASSERT( pNewNode );
#endif
    pNewNode->object = object;

    __SET_STRUCT_MB( __LinkBiTree_t, __LinkBiTree_t*, pNewNode , pPrev , pTarget        );
    __SET_STRUCT_MB( __LinkBiTree_t, __LinkBiTree_t*, pNewNode , pRight, pTarget->pLeft );
    
    if( pTarget->pLeft!=NULL )
        __SET_STRUCT_MB( __LinkBiTree_t, __LinkBiTree_t*, pTarget->pLeft, pPrev , pNewNode );
    
    __SET_STRUCT_MB( __LinkBiTree_t, __LinkBiTree_t*, pTarget  , pLeft , pNewNode );
    
    return pNewNode;
}
    
__LinkBiTree_t* __LINK_BiTree_add_l2l        ( const __LinkBiTree_t *pHead , __LinkBiTree_t *pTarget , void* object ){
#ifdef RH_DEBUG
    ASSERT( pHead );
    ASSERT( !(pHead->pPrev) );  // Head Node shouldn't have a previous node. So...Not a headnode.
    ASSERT( pTarget );
#endif
    __LinkBiTree_t* pNewNode = (__LinkBiTree_t*)__malloc(sizeof(__LinkBiTree_t));
#ifdef RH_DEBUG
    ASSERT( pNewNode );
#endif
    pNewNode->object = object;
    
    __SET_STRUCT_MB( __LinkBiTree_t, __LinkBiTree_t*, pNewNode , pPrev , pTarget        );
    __SET_STRUCT_MB( __LinkBiTree_t, __LinkBiTree_t*, pNewNode , pLeft , pTarget->pLeft );
    
    if( pTarget->pLeft!=NULL )
        __SET_STRUCT_MB( __LinkBiTree_t, __LinkBiTree_t*, pTarget->pLeft, pPrev , pNewNode );
    
    __SET_STRUCT_MB( __LinkBiTree_t, __LinkBiTree_t*, pTarget, pLeft, pNewNode );
    
    return pNewNode;
}
    
__LinkBiTree_t* __LINK_BiTree_add_r2l        ( const __LinkBiTree_t *pHead , __LinkBiTree_t *pTarget , void* object ){
#ifdef RH_DEBUG
    ASSERT( pHead );
    ASSERT( !(pHead->pPrev) );  // Head Node shouldn't have a previous node. So...Not a headnode.
    ASSERT( pTarget );
#endif
    __LinkBiTree_t* pNewNode = (__LinkBiTree_t*)__malloc(sizeof(__LinkBiTree_t));
#ifdef RH_DEBUG
    ASSERT( pNewNode );
#endif
    pNewNode->object = object;
    
    __SET_STRUCT_MB( __LinkBiTree_t, __LinkBiTree_t*, pNewNode , pPrev , pTarget        );
    __SET_STRUCT_MB( __LinkBiTree_t, __LinkBiTree_t*, pNewNode , pLeft , pTarget->pLeft );
    
    if( pTarget->pLeft!=NULL )
        __SET_STRUCT_MB( __LinkBiTree_t, __LinkBiTree_t*, pTarget->pRight, pPrev , pNewNode );
    
    __SET_STRUCT_MB( __LinkBiTree_t, __LinkBiTree_t*, pTarget, pRight, pNewNode );
    
    return pNewNode;
}
    
__LinkBiTree_t* __LINK_BiTree_add_r2r        ( const __LinkBiTree_t *pHead , __LinkBiTree_t *pTarget , void* object ){
#ifdef RH_DEBUG
    ASSERT( pHead );
    ASSERT( !(pHead->pPrev) );  // Head Node shouldn't have a previous node. So...Not a headnode.
    ASSERT( pTarget );
#endif
    __LinkBiTree_t* pNewNode = (__LinkBiTree_t*)__malloc(sizeof(__LinkBiTree_t));
#ifdef RH_DEBUG
    ASSERT( pNewNode );
#endif
    pNewNode->object = object;
    
    __SET_STRUCT_MB( __LinkBiTree_t, __LinkBiTree_t*, pNewNode, pPrev , pTarget         );
    __SET_STRUCT_MB( __LinkBiTree_t, __LinkBiTree_t*, pNewNode, pRight, pTarget->pRight );

    if( pTarget->pLeft!=NULL )
        __SET_STRUCT_MB( __LinkBiTree_t, __LinkBiTree_t*, pTarget->pRight, pPrev , pNewNode );

    __SET_STRUCT_MB( __LinkBiTree_t, __LinkBiTree_t*, pTarget, pRight, pNewNode );

    return pNewNode;
}
    
__LinkBiTree_t* __LINK_BiTree_find           ( const __LinkBiTree_t *pHead , void* object ){
    
#ifdef RH_DEBUG
    ASSERT( pHead );
#endif
    
    __LinkBiTree_t* pTarget = NULL;
    
    if( pHead->object == object ){
        return (__LinkBiTree_t*)pHead;
    }else{
        pTarget = __LINK_BiTree_find( pHead->pLeft , object );
    }
    
    if( !pTarget ){
        pTarget = __LINK_BiTree_find( pHead->pLeft , object );
    }
    
    return pTarget;
}
    
    
__Stack_t* __Stack_createBase  ( void* object ){
    __Stack_t *ptr = (__Stack_t*)__malloc(sizeof(__Stack_t));
#ifdef RH_DEBUG
    ASSERT( ptr );
#endif
    __SET_STRUCT_MB( __Stack_t, __Stack_t*, ptr, pPrev , ptr   );
    __SET_STRUCT_MB( __Stack_t, __Stack_t*, ptr, pNext , ptr   );
    __SET_STRUCT_MB( __Stack_t, void*     , ptr, object, object);

    return ptr;
}
    
__Stack_t* __Stack_push        ( const __Stack_t *pBase , void* object ){

#ifdef RH_DEBUG
    ASSERT( pBase );
#endif
    
    __Stack_t* pNew = (__Stack_t*)__malloc(sizeof(__Stack_t));
#ifdef RH_DEBUG
    ASSERT( pNew );
#endif
    
    __SET_STRUCT_MB(__Stack_t, void*     , pNew            , object, object       );
    __SET_STRUCT_MB(__Stack_t, __Stack_t*, pNew            , pPrev , pBase->pPrev );
    __SET_STRUCT_MB(__Stack_t, __Stack_t*, pNew            , pNext , pBase        );
    
    __SET_STRUCT_MB(__Stack_t, __Stack_t*, pBase->pPrev    , pNext , pNew         );
    __SET_STRUCT_MB(__Stack_t, __Stack_t*, pBase           , pPrev , pNew         );
    
    return pNew;
}
    
void*      __Stack_pop         ( const __Stack_t *pBase ){

#ifdef RH_DEBUG
    ASSERT( pBase );
#endif
    
    __exitReturn(pBase->pNext==pBase->pPrev && pBase->pNext==pBase , (void*)pBase->object );
    
    void* dummy_ptr = (__Stack_t*)(pBase->pPrev);
    
    void* object = (void*)(pBase->pPrev->object);
    
    __SET_STRUCT_MB(__Stack_t, __Stack_t*, pBase->pPrev->pPrev , pNext, pBase               );
    __SET_STRUCT_MB(__Stack_t, __Stack_t*, pBase               , pPrev, pBase->pPrev->pPrev );
    
    __free(dummy_ptr);
    
    return object;
}
    
size_t     __Stack_size        ( const __Stack_t *pBase ){

#ifdef RH_DEBUG
    ASSERT( pBase );
#endif
    size_t cnt = 0;
    const __Stack_t *p = pBase;
    while(p->pNext != pBase){
        cnt++;
        p = p->pNext;
    }
    
    return cnt;
}
    
void*      __Stack_top         ( const __Stack_t *pBase ){

#ifdef RH_DEBUG
    ASSERT( pBase );
#endif
    return (void*)(pBase->pPrev->object);
}
    
bool       __Stack_empty       ( const __Stack_t *pBase ){

#ifdef RH_DEBUG
    ASSERT( pBase );
#endif
    return (pBase->pNext==pBase->pPrev && pBase->pNext==pBase);
}
    
void*      __Stack_deleteBase  ( __Stack_t *pBase    ){

#ifdef RH_DEBUG
    ASSERT( pBase );
#endif
    __Stack_t *p = pBase;
    void* object = (void*)pBase->object;
    if(p->pNext==pBase){
        __free((void*)(p));
    }else{
        while(p->pNext!=pBase){
            p = (__Stack_t*)(p->pNext);
            __free((void*)(p->pPrev));
            __SET_STRUCT_MB( __Stack_t, __Stack_t*, p, pPrev , NULL );
        }
    }

    return object;
}
    
    
E_Status_t __Queue_createHead  ( __Queue_t ** ptr    ){
    
    return kStatus_Success;
}
    
    
static size_t RH_FUNCONST __HashFunc( size_t key ){
    return (key%RH_HASH_MAP_SIZE);
}
    
__HashMap_t* __Hash_createMap  ( void ){

    __HashMap_t *pHashHead = __malloc( sizeof(__HashMap_t) );
#ifdef RH_DEBUG
    ASSERT( pHashHead );
#endif
    
    __HashList_t* pHashList = __malloc(RH_HASH_MAP_SIZE*sizeof(__HashList_t));
#ifdef RH_DEBUG
    ASSERT( pHashList );
#endif
    memset(pHashList, 0, RH_HASH_MAP_SIZE*sizeof(__HashList_t));
    
    __SET_STRUCT_MB(__HashMap_t, __HashList_t*, pHashHead, pList, pHashList);
//    (*ptr)->pList = pHashList;
    
    return pHashHead;
}
    
void*        __Hash_find       ( const __HashMap_t *pHead, size_t key ){
#ifdef RH_DEBUG
    ASSERT(pHead);
#endif
    
    const __HashList_t *pList = pHead->pList[ __HashFunc(key)].pNext ;
    
    while( pList != NULL ){
        if( pList->key == key )
            return (void*)(pList->object);
        pList = pList->pNext;
    }
    
    return NULL;
}
    
void         __Hash_put        ( const __HashMap_t *pHead, size_t key, void* object ){

#ifdef RH_DEBUG
    ASSERT( pHead );
    ASSERT( pHead->pList );
#endif
    
    
    const __HashList_t *pList     =  (pHead->pList[ __HashFunc(key) ].pNext);
    const __HashList_t *pList_old = &(pHead->pList[ __HashFunc(key) ]);
    while( pList != NULL ){
        if( pList->key == key ){
            __SET_STRUCT_MB(__HashList_t, void*, pList, object, object);
            return ;
        }
        pList_old = pList;
        pList     = (__HashList_t*)pList->pNext;
    }
    pList = pList_old;

    // This key is not exsist in current HashList.
    // Allocate a new node for this pair.
    __SET_STRUCT_MB(__HashList_t, __HashList_t* , pList       , pNext , __malloc(sizeof(__HashList_t)));
    __SET_STRUCT_MB(__HashList_t, size_t        , pList->pNext, key   , key);
    __SET_STRUCT_MB(__HashList_t, void*         , pList->pNext, object, object);
    __SET_STRUCT_MB(__HashList_t, void*         , pList->pNext, pNext , NULL);
    
}
    
void*        __Hash_get        ( const __HashMap_t *pHead, size_t key ){

#ifdef RH_DEBUG
    ASSERT( pHead );
    ASSERT( pHead->pList );
#endif
    
    const __HashList_t *pList = pHead->pList[ (long)__HashFunc(key) ].pNext;
    
    while( pList != NULL ){
        if( pList->key == key ){
            return (void*)pList->object;
        }
        pList     = (__HashList_t*)pList->pNext;
    }
    
    return NULL;
}
    
void*        __Hash_remove     ( const __HashMap_t *pHead, size_t key ){
#ifdef RH_DEBUG
    ASSERT( pHead );
    ASSERT( pHead->pList );
#endif
    const __HashList_t *pList     =  (pHead->pList[ (long)__HashFunc(key) ].pNext);
    const __HashList_t *pList_old = &(pHead->pList[ (long)__HashFunc(key) ]);
    while( pList != NULL ){
        if( pList->key == key ){
            void* object = (void*)pList->object;
            __SET_STRUCT_MB(__HashList_t, __HashList_t*, pList_old, pNext, pList->pNext);
            __free((void*)pList);
            return object;
        }
        pList_old = pList;
        pList     = pList->pNext;
    }
    
    return NULL;
}
    
void         __Hash_removeAll  ( __HashMap_t *pHead ){
#ifdef RH_DEBUG
    ASSERT( pHead );
    ASSERT( pHead->pList );
#endif
    
    size_t cnt = RH_HASH_MAP_SIZE;
    while(cnt--){
        const __HashList_t *pList_tmp = (pHead->pList[ cnt ].pNext);
        const __HashList_t *pList     = (pHead->pList[ cnt ].pNext);
        while( pList != NULL ){
            pList_tmp = pList->pNext;
            __free((void*)pList);
            pList = pList_tmp;
        }
    }
    __free( (void*)pHead->pList );

}
    
#ifdef __cplusplus
}
#endif

