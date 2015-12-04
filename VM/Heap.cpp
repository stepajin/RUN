//
//  Heap.cpp
//  VM
//
//  Created by Štěpánek Jindřich on 04/12/15.
//  Copyright © 2015 stepajin. All rights reserved.
//

#include <stdio.h>

#include "Heap.h"
#include "CallStack.h"

Heap * Heap::INST;
Heap * Heap::INSTANCE() {
    if (INST == NULL)
        INST = new Heap();
    
    return INST;
}

Heap::Heap() {
    size = 0;
    
    rootSet = new set<Enviroment *>();
    heap = new vector<VmObject *>();
}

void Heap::alloc(VmObject * obj) {
    heap->push_back(obj);
    size++; // TODO: actual size
}

void Heap::addEnviroment(Enviroment * enviroment) {
    rootSet->insert(enviroment);
}

void Heap::removeEnviroment(Enviroment * enviroment) {
    rootSet->erase(enviroment);
}

void Heap::printHeap(bool printMark) {
    vector<VmObject *>::iterator it;
    
    cout << "----- heap" << endl;
    for (it = heap->begin(); it != heap->end(); it++) {
        VmObject * obj = *it;
        
        cout << obj->toString();
        if (printMark && obj->isMarked())
            cout << "(x)";
        cout << ", ";
    }
    cout << endl << "-----" << endl;
}

void Heap::collectIfNeeded() {
    // TODO: Count also with stack size
    
    if (size >= maxSize) {
        collect();
    }
}

void Heap::collect() {
    cout << endl<< "COLLECT" << endl;
    printHeap(true);
    if (CallStack::INSTANCE()->getSize() > 0)
        CallStack::INSTANCE()->printStack();
    
    // Mark
    for (set<Enviroment *>::iterator it = rootSet->begin(); it != rootSet->end(); ++it) {
        Enviroment * env = *it;
        
        env->markChildren();
    }
    
    CallStack::INSTANCE()->markChildren();
    
    printHeap(true);
    
    // Sweep
    vector<VmObject *>::iterator it = heap->begin();

    while(it != heap->end()) {
        VmObject * obj = *it;
        
        if(!obj->isMarked()) {
            it = heap->erase(it);
            delete obj;
            size--;
        } else {
            obj->unmark();
            ++it;
        }
    }
    
    printHeap(true);
}
