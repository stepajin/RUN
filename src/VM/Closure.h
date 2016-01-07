//
//  Closure.h
//  VM
//
//  Created by Štěpánek Jindřich on 19/12/15.
//  Copyright © 2015 stepajin. All rights reserved.
//

#ifndef Closure_h
#define Closure_h

#include "Objects.h"

class VmClosure : public VmObject {
    BYTE * bytecode;
    int length;
    
    Environment * environment;
    
public:
    VmClosure(int length, BYTE * bytecode, Environment * environment);
    virtual ~VmClosure();
    
    virtual VmObject * eval(Environment * environment);
    VmObject * execute();
    
    virtual string toString();
};

#endif /* Closure_h */
