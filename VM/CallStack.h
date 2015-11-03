//
//  CallStack.h
//  VM
//
//  Created by Štěpánek Jindřich on 02/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef __Vm__CallStack__
#define __Vm__CallStack__

#include "Objects.h"

class CallStack {
    VmObject ** stack;
    int size;
    int allocSize;
    
    int stackPointer;
    
    void resize();

    CallStack();
    
    static CallStack * INST;

public:
    static CallStack * INSTANCE();
    
    void push(VmObject * obj);
    VmObject * pop();
    
    void setStackPointer(int stackPointer);
    int getStackPointer();
    
    void printStack();
};

#endif /* defined(__Vm__CallStack__) */