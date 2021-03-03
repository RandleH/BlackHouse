
#ifndef _RH_DATA_H
#define _RH_DATA_H

#include "RH_common.h"

#ifdef __cplusplus
extern "C" {
#endif


/*=====================================================================
> Data Structure Reference
======================================================================*/
struct __LinkLoopNode{
    void*        object;
    const struct __LinkLoopNode* const pNext; // Can NOT be modified by user.
    const struct __LinkLoopNode* const pPrev; // Can NOT be modified by user.
};
typedef struct __LinkLoopNode __LinkLoopNode;

E_Status_t MAKE_FUNC( LINK_Loop , createNode            ) ( __LinkLoopNode  **      ptr        );
E_Status_t MAKE_FUNC( LINK_Loop , createHeadNode        ) ( __LinkLoopNode  **      ptr        );
E_Status_t MAKE_FUNC( LINK_Loop , addNode_tail          ) ( __LinkLoopNode  *const* ppHeadNode , __LinkLoopNode *const* ppNewNode  );
E_Status_t MAKE_FUNC( LINK_Loop , addNode_tail_newhead  ) ( __LinkLoopNode  **      ppHeadNode , __LinkLoopNode *const* ppNewNode  );
E_Status_t MAKE_FUNC( LINK_Loop , addNode_front         ) ( __LinkLoopNode  *const* ppHeadNode , __LinkLoopNode *const* ppNewNode  );
E_Status_t MAKE_FUNC( LINK_Loop , addNode_front_newhead ) ( __LinkLoopNode  **      ppHeadNode , __LinkLoopNode *const* ppNewNode  );
E_Status_t MAKE_FUNC( LINK_Loop , findNode              ) ( __LinkLoopNode  *const* ppHeadNode , __LinkLoopNode *const* ppTarget   );
E_Status_t MAKE_FUNC( LINK_Loop , checkLoopNode         ) ( __LinkLoopNode  *const* ppHeadNode );
E_Status_t MAKE_FUNC( LINK_Loop , deleteNode            ) ( __LinkLoopNode  *const* ppHeadNode , __LinkLoopNode **      ppTarget   );
E_Status_t MAKE_FUNC( LINK_Loop , removeNode            ) ( __LinkLoopNode  *const* ppHeadNode , __LinkLoopNode *const* ppTarget   );
E_Status_t MAKE_FUNC( LINK_Loop , removeAllNodes        ) ( __LinkLoopNode  *const* ppHeadNode );
E_Status_t MAKE_FUNC( LINK_Loop , printAllNodesAdr      ) ( __LinkLoopNode  *const* ppHeadNode , int(*PRINTF_FUNC)(const char*,...));

struct __LinkBiTreeNode{
    void*        object;
    const struct __LinkBiTreeNode* const pLeft;  // Can NOT be modified by user.
    const struct __LinkBiTreeNode* const pRight; // Can NOT be modified by user.
    const struct __LinkBiTreeNode* const pPrev;  // Can NOT be modified by user.
};
typedef struct __LinkBiTreeNode __LinkBiTreeNode;

E_Status_t MAKE_FUNC( LINK_BiTree , createNode          ) ( __LinkBiTreeNode ** ptr );
E_Status_t MAKE_FUNC( LINK_BiTree , createHeadNode      ) ( __LinkBiTreeNode ** ptr );
E_Status_t MAKE_FUNC( LINK_BiTree , addNode_l2r         ) ( __LinkBiTreeNode ** ppHead , __LinkBiTreeNode ** ppTarget , __LinkBiTreeNode ** ppNew);
E_Status_t MAKE_FUNC( LINK_BiTree , addNode_l2l         ) ( __LinkBiTreeNode ** ppHead , __LinkBiTreeNode ** ppTarget , __LinkBiTreeNode ** ppNew);
E_Status_t MAKE_FUNC( LINK_BiTree , addNode_r2l         ) ( __LinkBiTreeNode ** ppHead , __LinkBiTreeNode ** ppTarget , __LinkBiTreeNode ** ppNew);
E_Status_t MAKE_FUNC( LINK_BiTree , addNode_r2r         ) ( __LinkBiTreeNode ** ppHead , __LinkBiTreeNode ** ppTarget , __LinkBiTreeNode ** ppNew);
E_Status_t MAKE_FUNC( LINK_BiTree , findNode            ) ( __LinkBiTreeNode ** ppHead , __LinkBiTreeNode ** ppTarget );

struct __Stack_t{
    const void*             const object; // Can NOT be modified by user.
    const struct __Stack_t* const pNext;  // Can NOT be modified by user.
    const struct __Stack_t* const pPrev;  // Can NOT be modified by user.
};
typedef struct __Stack_t __Stack_t;

E_Status_t MAKE_FUNC( Stack , createBase ) ( __Stack_t ** ptr    );
E_Status_t MAKE_FUNC( Stack , push       ) ( __Stack_t ** ppBase  , void      *  pObj  );
E_Status_t MAKE_FUNC( Stack , pop        ) ( __Stack_t ** ppBase  , void     **  ppObj );
E_Status_t MAKE_FUNC( Stack , size       ) ( __Stack_t ** ppBase  , size_t    *  result);
E_Status_t MAKE_FUNC( Stack , empty      ) ( __Stack_t ** ppBase );
E_Status_t MAKE_FUNC( Stack , top        ) ( __Stack_t ** ppBase  , void     **  ppObj );
E_Status_t MAKE_FUNC( Stack , deleteBase ) ( __Stack_t ** ptr    );//



struct __Queue_t{
    const void*             const object;
    const struct __Queue_t* const pNext ;
};
typedef struct __Queue_t __Queue_t;

E_Status_t MAKE_FUNC( Queue , createHead ) ( __Queue_t ** ptr    );




#ifdef __cplusplus
}
#endif

#endif

