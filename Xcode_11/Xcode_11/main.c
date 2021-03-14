//
//  main.c
//  Xcode_11
//
//  Created by Randle Helmslay on 2021/2/27.
//  Copyright © 2021 Randle Helmslay. All rights reserved.
//

#include <stdio.h>
#include "RH_data.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    
    __HashMap_t* pMap;
    __Hash_createMap(&pMap);
    __Hash_put( &pMap, 1, "1");
    __Hash_put( &pMap, 2, "2");
    
    
    void* p = NULL;
    __Hash_get( &pMap, 1, &p);
    __Hash_get( &pMap, 3, &p);
    __Hash_put( &pMap, 2, "1");
    __Hash_get( &pMap, 2, &p);
    __Hash_remove(&pMap, 2);
    __Hash_get( &pMap, 2, &p);
    
    return 0;
}
