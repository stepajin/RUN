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
    countdown = COUNTDOWN_BEGIN;
    
    rootSet = new set<Enviroment *>();
    heap = new vector<VmObject *>();
}

void Heap::alloc(VmObject * obj) {
    heap->push_back(obj);
}

void Heap::addEnviroment(Enviroment * enviroment) {
    rootSet->insert(enviroment);
}

void Heap::removeEnviroment(Enviroment * enviroment) {
    rootSet->erase(enviroment);
}

void Heap::printHeap() {
    vector<VmObject *>::iterator it;
    
    cout << "----- heap" << endl;
    for (it = heap->begin(); it != heap->end(); it++) {
        VmObject * obj = *it;
        
        cout << obj->toString();
        if (obj->isMarked())
            cout << "(x)";
        if (obj->isRetained())
            cout << "(R)";
        
        cout << ", ";
    }
    cout << endl << "-----" << endl;
}

void Heap::collectIfNeeded() {
    if (--countdown == 0) {
        collect();
        countdown = COUNTDOWN_BEGIN;
    }
}

void Heap::forceCollect() {
    collect();
}

void Heap::collect() {
    bool print = true;
    
    if (print) {
        cout << endl<< "COLLECT" << endl;
        printHeap();
        if (CallStack::INSTANCE()->getSize() > 0)
            CallStack::INSTANCE()->printStack();
    }
    
    // Mark
    for (set<Enviroment *>::iterator it = rootSet->begin(); it != rootSet->end(); ++it) {
        
        Enviroment * env = *it;

        env->markChildren();
    }
    
    CallStack::INSTANCE()->markChildren();
    
    if (print)
        printHeap();
    
    // Sweep
    vector<VmObject *>::iterator it = heap->begin();

    while(it != heap->end()) {
        VmObject * obj = *it;
        
        if(!obj->isMarked() && !obj->isRetained()) {
            it = heap->erase(it);
            delete obj;
        } else {
            obj->unmark();
            ++it;
        }
    }
    if (print)
        printHeap();
}
