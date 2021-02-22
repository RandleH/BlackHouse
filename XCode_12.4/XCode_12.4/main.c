//
//  main.c
//  XCode_12.4
//
//  Created by Randle.Helmslay on 2021/2/18.
//


#include <stdio.h>
#include "RH_data.h"
#include "RH_lib.h"
int main(int argc, const char * argv[]) {
    // insert code here...
    
    __LinkLoopNode *PH,*P1,*P2,*P3,*P4,*P5,*P6;
    __LINK_Loop_createHeadNode(&PH);
    __LINK_Loop_createNode(&P1);
    __LINK_Loop_createNode(&P2);
    __LINK_Loop_createNode(&P3);
    __LINK_Loop_createNode(&P4);
    __LINK_Loop_createNode(&P5);
    __LINK_Loop_createNode(&P6);
    __LINK_Loop_addNode_front(&PH, &P1);
    __LINK_Loop_addNode_front(&PH, &P2);
    __LINK_Loop_addNode_front(&PH, &P3);
    __LINK_Loop_addNode_front(&PH, &P4);
    __LINK_Loop_addNode_front(&PH, &P5);
    __LINK_Loop_addNode_front(&PH, &P6);
    __LINK_Loop_printAllNodesAdr(&PH, printf);
    
    __btoa_BIN(0x12);
    float i =10.9;
    printf("%s\n",__ftoa_BIN(i));    
    
    return 0;
}
