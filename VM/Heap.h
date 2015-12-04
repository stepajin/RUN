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
    const int COUNTDOWN_BEGIN = 20;
    int countdown;
    
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
    void forceCollect();
    
    void printHeap();
};

#endif /* VmHeap_h */
