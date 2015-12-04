//
//  VmHeap.h
//  VM
//
//  Created by Štěpánek Jindřich on 04/12/15.
//  Copyright © 2015 stepajin. All rights reserved.
//

#ifndef VmHeap_h
#define VmHeap_h

#include "Objects.h"

class Heap {
    int size;
    
    Heap();
    
    static Heap * INST;
    
public:
    static Heap * INSTANCE();

    void alloc(VmObject * obj);

};

#endif /* VmHeap_h */
