//
//  CallStack.h
//  VM
//
//  Created by Štěpánek Jindřich on 02/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef __Vm__CallStack__
#define __Vm__CallStack__

#include <stack>

#include "Objects.h"

class CallStack {
    VmObject ** stack;
    int size;
    int allocSize;
    
    int stackPointer;
    
    std::stack<int> * markStack;
    
    void resize();

    CallStack();
    
    static CallStack * INST;

public:
    static CallStack * INSTANCE();
    
    void push(VmObject * obj);
    VmObject * pop();
    
    void addMark();
    void returnToLastMark();
    
    void markChildren();
    
    void printStack();
    int getSize();
};

#endif /* defined(__Vm__CallStack__) */