//
//  UserFunction.h
//  VM
//
//  Created by Štěpánek Jindřich on 03/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef __Vm__UserFunction__
#define __Vm__UserFunction__

#include "Objects.h"

class UserFunction : public VmObject {
    int numberOfArgs;
    int * argIdentifiers;
    
    BYTE * bytecode;
    int length;
    
    VmObject ** arguments;
    
    
public:
    UserFunction(int numberOfArgs, int * argIdentifiers, int length, BYTE * bytecode);
    virtual ~UserFunction();

    virtual void readArguments(Reader * reader);
    virtual VmObject * eval(Enviroment * enviroment);
    
    virtual string toString();
};

#endif /* defined(__Vm__UserFunction__) */
