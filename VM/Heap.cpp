//
//  Heap.cpp
//  VM
//
//  Created by Štěpánek Jindřich on 04/12/15.
//  Copyright © 2015 stepajin. All rights reserved.
//

#include <stdio.h>

#include "Heap.h"

Heap * Heap::INST;
Heap * Heap::INSTANCE() {
    if (INST == NULL)
        INST = new Heap();
    
    return INST;
}

Heap::Heap() {
    size = 0;
    
    rootSet = new set<Enviroment *>();
}

void Heap::alloc(VmObject * obj) {


}

void Heap::addEnviroment(Enviroment * enviroment) {
    rootSet->insert(enviroment);
}

void Heap::removeEnviroment(Enviroment * enviroment) {
    rootSet->erase(enviroment);
}

