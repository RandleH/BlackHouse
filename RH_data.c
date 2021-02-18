
#include "RH_data.h"

#ifdef __cplusplus
extern "C" {
#endif

/*=====================================================================
 > Data Structure Reference
======================================================================*/
E_Status_t MAKE_FUNC( LINK_Loop , createNode            )  (__LinkLoopNode **      ptr ){
    __exitReturn(ptr==NULL, kStatus_BadAccess);
  
    *ptr =  (__LinkLoopNode*)__malloc(sizeof(__LinkLoopNode));
    if(*ptr == NULL)
        return kStatus_NoSpace;

    return kStatus_Success;
}
      
E_Status_t MAKE_FUNC( LINK_Loop , createHeadNode        )  (__LinkLoopNode **      ptr ){
    __LinkLoopNode* pNode = (__LinkLoopNode*)__malloc(sizeof(__LinkLoopNode));
    *ptr = pNode;
  
    if( pNode == NULL )
        return kStatus_NoSpace;

    __SET_STRUCT_MB(__LinkLoopNode,void*       ,pNode,object,NULL );
    __SET_STRUCT_MB(__LinkLoopNode,__LinkLoopNode*,pNode,pNext ,pNode);
    __SET_STRUCT_MB(__LinkLoopNode,__LinkLoopNode*,pNode,pPrev ,pNode);

    // Same Effect: pNode->object = NULL;
    // Same Effect: pNode->pNext  = pNode;  // But to cope with <const>.
    // Same Effect: pNode->pPrev  = pNode;  // But to cope with <const>.
    return kStatus_Success;
}

E_Status_t MAKE_FUNC( LINK_Loop , addNode_tail          )  (__LinkLoopNode *const* ppHeadNode ,__LinkLoopNode *const* ppNewNode){
  __exitReturn(  ppHeadNode == NULL ||  ppNewNode == NULL , kStatus_BadAccess);
  __exitReturn( *ppHeadNode == NULL || *ppNewNode == NULL , kStatus_BadAccess);
  
  const __LinkLoopNode* pHeadNode = *ppHeadNode;
  const __LinkLoopNode* pNewNode  = *ppNewNode;
  // Check whether it was already exist.
  const __LinkLoopNode* pTmp = pHeadNode;
  do{
      if( pTmp == pNewNode )
          return kStatus_Exist;
      pTmp = pTmp->pNext;
  }while( pTmp != pHeadNode );
  // Every thing is OK.
  
  // Things to do for the new Node.
  __SET_STRUCT_MB(__LinkLoopNode,__LinkLoopNode*,pNewNode        ,pPrev,pHeadNode->pPrev   );
  __SET_STRUCT_MB(__LinkLoopNode,__LinkLoopNode*,pNewNode        ,pNext,pHeadNode          );

  // Things to do for the neighbour.
  __SET_STRUCT_MB(__LinkLoopNode,__LinkLoopNode*,pHeadNode->pPrev,pNext,pNewNode           );
  __SET_STRUCT_MB(__LinkLoopNode,__LinkLoopNode*,pHeadNode       ,pPrev,pNewNode           );
  
  // Same Effect: pNewNode->pPrev = pHeadNode->pPrev; // But to cope with <const>.
  // Same Effect: pNewNode->pNext = pHeadNode;        // But to cope with <const>.
  
  // Same Effect: pHeadNode->pPrev->pNext = pNewNode; // But to cope with <const>.
  // Same Effect: pHeadNode->pPrev        = pNewNode; // But to cope with <const>.
  
  return kStatus_Success;
}
      
E_Status_t MAKE_FUNC( LINK_Loop , addNode_tail_newhead  )  (__LinkLoopNode **      ppHeadNode ,__LinkLoopNode *const* ppNewNode){
  
    E_Status_t state = MAKE_FUNC( LINK_Loop , addNode_tail   )(ppHeadNode, ppNewNode);
    __exitReturn(state != kStatus_Success, state);
    
    // New Head
    *ppHeadNode = *ppNewNode;

    return kStatus_Success;
}

E_Status_t MAKE_FUNC( LINK_Loop , addNode_front         )  (__LinkLoopNode *const* ppHeadNode ,__LinkLoopNode *const* ppNewNode){
    __exitReturn(  ppHeadNode == NULL ||  ppNewNode == NULL , kStatus_BadAccess);
    __exitReturn( *ppHeadNode == NULL || *ppNewNode == NULL , kStatus_BadAccess);
    
    const __LinkLoopNode* pHeadNode = *ppHeadNode;
    const __LinkLoopNode* pNewNode  = *ppNewNode;
    // Check whether it was already exist.
    const __LinkLoopNode* pTmp = pHeadNode;
    do{
        if( pTmp == pNewNode )
            return kStatus_Exist;
        pTmp = pTmp->pNext;
    }while( pTmp != pHeadNode );
    // Every thing is OK.
    
    // Things to do for the new Node.
    __SET_STRUCT_MB(__LinkLoopNode,__LinkLoopNode*,pNewNode        ,pPrev,pHeadNode          );
    __SET_STRUCT_MB(__LinkLoopNode,__LinkLoopNode*,pNewNode        ,pNext,pHeadNode->pNext   );
  
    // Things to do for the neighbour.
    __SET_STRUCT_MB(__LinkLoopNode,__LinkLoopNode*,pHeadNode->pNext,pPrev,pNewNode           );
    __SET_STRUCT_MB(__LinkLoopNode,__LinkLoopNode*,pHeadNode       ,pNext,pNewNode           );
    
    // Same Effect: pNewNode->pPrev = pHeadNode;        // But to cope with <const>.
    // Same Effect: pNewNode->pNext = pHeadNode->pNext; // But to cope with <const>.
    
    // Same Effect: pHeadNode->pNext->pPrev = pNewNode; // But to cope with <const>.
    // Same Effect: pHeadNode->pNext        = pNewNode; // But to cope with <const>.
    
    return kStatus_Success;
}

E_Status_t MAKE_FUNC( LINK_Loop , addNode_front_newhead )  (__LinkLoopNode **      ppHeadNode ,__LinkLoopNode *const* ppNewNode){
    E_Status_t state = MAKE_FUNC(LINK_Loop , addNode_front)(ppHeadNode, ppNewNode);
    __exitReturn(state!=kStatus_Success, state);
    
    // New Head
    *ppHeadNode = *ppNewNode;

    return kStatus_Success;
}

E_Status_t MAKE_FUNC( LINK_Loop , findNode              )  (__LinkLoopNode *const* ppHeadNode ,__LinkLoopNode *const* ppTarget ){
    __exitReturn(  ppHeadNode == NULL ||  ppTarget == NULL , kStatus_BadAccess);
    const __LinkLoopNode* pHeadNode = *ppHeadNode;
    const __LinkLoopNode* pTarget   = *ppTarget;
    __exitReturn(pHeadNode==NULL, kStatus_BadAccess);
    __exitReturn(pTarget==NULL  , kStatus_NotFound);
    
    const __LinkLoopNode* pTmp  = pHeadNode;
    
    do{
        if (pTmp == pTarget) {
            return kStatus_Success;
        }
        pTmp = pTmp->pNext;
    }while( pTmp != pHeadNode );
    return kStatus_NotFound;
}

E_Status_t MAKE_FUNC( LINK_Loop , checkLoopNode         )  (__LinkLoopNode *const* ppHeadNode){
    __exitReturn(ppHeadNode == NULL , kStatus_BadAccess);
    __LinkLoopNode* pHeadNode = *ppHeadNode;
    __exitReturn(pHeadNode == NULL  , kStatus_BadAccess);
    const __LinkLoopNode* pFast = pHeadNode;
    const __LinkLoopNode* pSlow = pHeadNode;
  
    // Search the entire chain. o(n)  :-)
    while(pSlow->pNext != pHeadNode){
        pSlow = pSlow->pNext;
        pFast = pFast->pNext->pNext;
        if( pSlow==pFast )
            return kStatus_Exist;
    }
    
    return kStatus_Success;
}

E_Status_t MAKE_FUNC( LINK_Loop , deleteNode            )  (__LinkLoopNode *const* ppHeadNode ,__LinkLoopNode **      ppTarget ){
    E_Status_t state = MAKE_FUNC(LINK_Loop , removeNode)(ppHeadNode, ppTarget);
    __exitReturn(state!=kStatus_Success , state             );
    
    __LinkLoopNode* pTarget   = *ppTarget;
    __free(pTarget);  // You should release anything in this node before deleting it.
    *ppTarget = NULL;
    return kStatus_Success;
}

E_Status_t MAKE_FUNC( LINK_Loop , removeNode            )  (__LinkLoopNode *const* ppHeadNode ,__LinkLoopNode *const* ppTarget ){
    __exitReturn( ppHeadNode==NULL || ppTarget==NULL,kStatus_BadAccess );
    __LinkLoopNode* pHeadNode = *ppHeadNode;
    __LinkLoopNode* pTarget   = *ppTarget;
    __exitReturn( pHeadNode==NULL||pTarget==NULL    , kStatus_BadAccess);
    __exitReturn( pHeadNode==pTarget                ,kStatus_Denied    );
    
    E_Status_t state = MAKE_FUNC( LINK_Loop,findNode)(ppHeadNode,ppTarget);
    __exitReturn(state != kStatus_Success           ,state             );
  
    // Connect the neighbour and isolate the <pTarget>.
    __SET_STRUCT_MB(__LinkLoopNode, __LinkLoopNode*, pTarget->pPrev, pNext, pTarget->pNext);
    __SET_STRUCT_MB(__LinkLoopNode, __LinkLoopNode*, pTarget->pNext, pPrev, pTarget->pPrev);
    // Same Effect: pTarget->pPrev->pNext = pTarget->pNext; // But to cope with <const>.
    // Same Effect: pTarget->pNext->pPrev = pTarget->pPrev; // But to cope with <const>.
    
    __SET_STRUCT_MB(__LinkLoopNode, __LinkLoopNode*, pTarget, pNext, NULL);
    __SET_STRUCT_MB(__LinkLoopNode, __LinkLoopNode*, pTarget, pPrev, NULL);
    // Same Effect: pTarget->pNext = NULL; // But to cope with <const>.
    // Same Effect: pTarget->pPrev = NULL; // But to cope with <const>.
    
    return kStatus_Success;
}

E_Status_t MAKE_FUNC( LINK_Loop , removeAllNodes        )  (__LinkLoopNode *const* ppHeadNode){
    __exitReturn(ppHeadNode==NULL, kStatus_BadAccess);
    
    E_Status_t state = MAKE_FUNC(LINK_Loop,checkLoopNode)(ppHeadNode);
    __exitReturn(state!=kStatus_Success ,state            );
    
    __LinkLoopNode* pHeadNode = *ppHeadNode;
    __LinkLoopNode* pTmpCur  = (__LinkLoopNode*)(pHeadNode->pNext);
    __LinkLoopNode* pTmpNxt  = (__LinkLoopNode*)(pTmpCur->pNext);
    while(pTmpCur != pHeadNode){
//        pTmpCur->pNext  = NULL;
//        pTmpCur->pPrev  = NULL;
        __SET_STRUCT_MB(__LinkLoopNode, __LinkLoopNode*, pTmpCur, pNext, NULL);
        __SET_STRUCT_MB(__LinkLoopNode, __LinkLoopNode*, pTmpCur, pPrev, NULL);
        pTmpCur = pTmpNxt;
        pTmpNxt = (__LinkLoopNode*)(pTmpNxt->pNext);
    }
  
    
    __SET_STRUCT_MB(__LinkLoopNode, __LinkLoopNode*, pHeadNode, pNext, pHeadNode);
    __SET_STRUCT_MB(__LinkLoopNode, __LinkLoopNode*, pHeadNode, pPrev, pHeadNode);
//    pHeadNode->pNext  = pHeadNode;
//    pHeadNode->pPrev  = pHeadNode;
  
    return kStatus_Success;
}

E_Status_t MAKE_FUNC( LINK_Loop , printAllNodesAdr      )  (__LinkLoopNode *const* ppHeadNode,int(*PRINTF_FUNC)(const char*,...)){
  
    __exitReturn(ppHeadNode == NULL, kStatus_BadAccess);
    
    const __LinkLoopNode* pHeadNode = *ppHeadNode;
    const __LinkLoopNode* pTmp      = pHeadNode;
    size_t cnt = 0;
    do{
        (*PRINTF_FUNC)("NODE[%d]: %p\n",cnt,pTmp);
        cnt++;
        pTmp = pTmp->pNext;
    }while( pTmp != pHeadNode );
        
    return kStatus_Success;
}
      
#ifdef __cplusplus
}
#endif

