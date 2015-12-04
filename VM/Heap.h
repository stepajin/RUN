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

class Heap {
    int size;
    
    Heap();
    
    static Heap * INST;
    
    set<Enviroment *> * rootSet;
    
public:
    static Heap * INSTANCE();

    void alloc(VmObject * obj);

    void addEnviroment(Enviroment * enviroment);
    void removeEnviroment(Enviroment * enviroment);    
};

#endif /* VmHeap_h */
