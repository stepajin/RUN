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
#include "Enviroment.h"

#include <set>
#include <vector>

class Heap {
    int size;
    const int maxSize = 4;
    
    Heap();
    
    static Heap * INST;
    
    set<Enviroment *> * rootSet;
    
    vector<VmObject *> * heap;
    
    void collect();
    
public:
    static Heap * INSTANCE();

    void alloc(VmObject * obj);

    void addEnviroment(Enviroment * enviroment);
    void removeEnviroment(Enviroment * enviroment);
    
    void collectIfNeeded();
};

#endif /* VmHeap_h */
