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
#include "Environment.h"

#include <set>
#include <vector>

class Heap {
    static const int COUNTDOWN_BEGIN = 50;
    int countdown;
    
    Heap();
    
    static Heap * INST;
    
    set<Environment *> * environments;
    
    vector<VmObject *> * heap;
    
    void collect();
    
public:
    ~Heap();
    
    static Heap * INSTANCE();

    void alloc(VmObject * obj);

    void addEnvironment(Environment * environment);
    
    void collectIfNeeded();
    void forceCollect();
    
    void printHeap();
};

#endif /* VmHeap_h */
