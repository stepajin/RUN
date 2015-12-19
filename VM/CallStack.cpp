//
//  CallStack.cpp
//  VM
//
//  Created by Štěpánek Jindřich on 02/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include "CallStack.h"

CallStack * CallStack::INST;
CallStack * CallStack::INSTANCE() {
    if (INST == NULL)
        INST = new CallStack();
    
    return INST;
}

CallStack::CallStack() {
    allocSize = 1000000;
    size = 0;
    stackPointer = -1;
    
    stack = new VmObject * [allocSize];
    
    markStack = new std::stack<int>;
}

CallStack::~CallStack() {
    delete markStack;
    delete stack;
}


void CallStack::resize() {
//    cout << "resize size: " << size << " alloced: " << allocSize << endl;
//    
//    cout << "FROM " << endl;
//    printStack();
    
    int newAllocSize = allocSize * 2;
    VmObject ** newStack = new VmObject * [allocSize];
    
    for (int i = 0; i < allocSize; i++)
        newStack[i] = stack[i];
    
    delete [] stack;
    
    stack = newStack;
    allocSize = newAllocSize;

//    cout << "TO " << endl;
//    printStack();
    
}

void CallStack::push(VmObject * obj) {
    if (size + 1 > allocSize) {
        resize();
    }
    
    stackPointer++;
    stack[stackPointer] = obj;
    
    if (stackPointer == size)
        size++;
    
   // cout << "stack size " << size << endl;
   // cout << "push " << obj->toString() << endl;
   // printStack();
    
}

VmObject * CallStack::pop() {
    if (stackPointer < 0 || size == 0)
        return NULL;
    
    VmObject * obj = stack[stackPointer];
    stack[stackPointer] = NULL;
    stackPointer--;
    size--;
    
//    cout << "pop " << obj->toString() << endl;
//    printStack();

    return obj;
}

void CallStack::addMark() {
//    cout << "add " << stackPointer << endl;
    markStack->push(stackPointer);
}

void CallStack::returnToLastMark() {
    if (markStack->size() == 0)
        return;
    
    stackPointer = markStack->top();
    markStack->pop();
    
//    cout << "return to " << stackPointer << endl;
}

void CallStack::printStack() {
    //cout << "stack size " << size << " alloced: " << allocSize << endl;
    
    cout << "----- stack" << endl;
    for (int i = 0; i <= stackPointer; i++) {
        if (stack[i] == NULL) {
            cout << "wtf" << endl; continue;
        }
        
        cout << i << ": " << stack[i]->toString() << endl;
    }
    cout << "-----" << endl;
}

int CallStack::getSize() {
    return size;
}

/********************
 
 Garbage Collection
 
 *******************/

void CallStack::markChildren() {
    for (int i = 0; i <= stackPointer; i++) {
        VmObject * obj = stack[i];
        
        if (!obj->isMarked()) {
            obj->mark();
        }
    }

}
