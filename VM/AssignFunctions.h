//
//  AssignFunctions.h
//  VM
//
//  Created by Štěpánek Jindřich on 03/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef __Vm__AssignFunctions__
#define __Vm__AssignFunctions__


#include "BuiltinFunctions.h"

class AssignFunction : public BuiltinFunction {
protected:
    int identifier;
    VmObject * value;

public:
    AssignFunction();
    virtual void readArguments(Reader * reader);
    virtual VmObject * eval(Environment * environment);
};

class AssignArithmeticFunction : public AssignFunction {
    virtual VmNumber * countNewValue(VmNumber * l, VmNumber * r) = 0;

public:
    virtual VmObject * eval(Environment * environment);
};

class AssignPlusFunction : public AssignArithmeticFunction {
    virtual VmNumber * countNewValue(VmNumber * l, VmNumber * r);
};

class AssignMinusFunction : public AssignArithmeticFunction {
    virtual VmNumber * countNewValue(VmNumber * l, VmNumber * r);
};

class AssignMultiplyFunction : public AssignArithmeticFunction {
    virtual VmNumber * countNewValue(VmNumber * l, VmNumber * r);
};

class AssignDivideFunction : public AssignArithmeticFunction {
    virtual VmNumber * countNewValue(VmNumber * l, VmNumber * r);
};

#endif /* defined(__Vm__AssignFunctions__) */
