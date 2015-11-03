//
//  BinaryOperations.h
//  VM
//
//  Created by Štěpánek Jindřich on 02/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef __Vm__BinaryOperations__
#define __Vm__BinaryOperations__

#include "BuiltinFunctions.h"

class BinaryOperation : public BuiltinFunction {
protected:
    VmObject * l;
    VmObject * r;
    
    BinaryOperation();
    
public:
    virtual void readArguments(Reader * reader);
};

class EqualsOperation : public BinaryOperation {
    
public:
    virtual VmObject * eval(Enviroment * enviroment);
};

class PlusOperation : public BinaryOperation {
    
public:
    virtual VmObject * eval(Enviroment * enviroment);
};


/*****************************
 
 Binary of two numbers
 
 ******************************/

class NumbersBinaryOperation : public BinaryOperation {
    bool evalOperands(Enviroment * enviroment);
    virtual VmObject * evalOperation(VmNumber * l, VmNumber * r) = 0;
public:
    virtual VmObject * eval(Enviroment * enviroment);
};


class MinusOperation : public NumbersBinaryOperation {
    
public:
    virtual VmObject * evalOperation(VmNumber * l, VmNumber * r);
};

class MultiplyOperation : public NumbersBinaryOperation {
    
public:
    virtual VmObject * evalOperation(VmNumber * l, VmNumber * r);
};

class DivideOperation : public NumbersBinaryOperation {
    
public:
    virtual VmObject * evalOperation(VmNumber * l, VmNumber * r);
};

/***********************
 
 Compare operations
 
 ************************/

class EqualsMoreOperation : public NumbersBinaryOperation {
    
public:
    virtual VmObject * evalOperation(VmNumber * l, VmNumber * r);
};

class MoreOperation : public NumbersBinaryOperation {
    
public:
    virtual VmObject * evalOperation(VmNumber * l, VmNumber * r);
};

class EqualsLessOperation : public NumbersBinaryOperation {
    
public:
    virtual VmObject * evalOperation(VmNumber * l, VmNumber * r);
};

class LessOperation : public NumbersBinaryOperation {
    
public:
    virtual VmObject * evalOperation(VmNumber * l, VmNumber * r);
};

#endif /* defined(__Vm__BinaryOperations__) */
