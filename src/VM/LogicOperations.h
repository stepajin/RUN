//
//  LogicOperations.h
//  VM
//
//  Created by Štěpánek Jindřich on 02/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef __Vm__LogicOperations__
#define __Vm__LogicOperations__


#include "BuiltinFunctions.h"

class LogicOperation : public BuiltinFunction {
    VmObject * l;
    VmObject * r;
    
public:
    LogicOperation();
    
    virtual VmObject * eval(Environment * e);
    virtual bool evalBool(VmBoolean * l, VmBoolean * r) = 0;
    virtual void readArguments(Reader * reader);
};

class AndOperation : public LogicOperation {
    virtual bool evalBool(VmBoolean * l, VmBoolean * r);
};

class OrOperation : public LogicOperation {
    virtual bool evalBool(VmBoolean * l, VmBoolean * r);
};

#endif /* defined(__Vm__LogicOperations__) */
