//
//  AssignFunctions.cpp
//  VM
//
//  Created by Štěpánek Jindřich on 03/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include <sstream>

#include "AssignFunctions.h"
#include "Environment.h"
#include "VM.h"
#include "CallStack.h"
#include "Error.h"


/***************
 
 Assign
 
 **************/

AssignFunction::AssignFunction() : BuiltinFunction("assign") {
}

void AssignFunction::readArguments(Reader * reader) {
    value = CallStack::INSTANCE()->pop();
    identifier = reader->getShortInt();
}

VmObject * AssignFunction::eval(Environment * environment) {
    
    VmObject * e = value->eval(environment);
    environment->setVariable(identifier, e);
    
    return VmVoid::VOID();
    //return e;
}

VmObject * AssignArithmeticFunction::eval(Environment * environment) {
    VmObject * obj = environment->getVariable(identifier);
    
    if (!obj) {
        stringstream ss;
        ss << "object " << identifier << " not found";
        return error(ss.str());
    }
    
    VmObject * e = value->eval(environment);
    
    if (e->getTag() != TAG_NUMBER || obj->getTag() != TAG_NUMBER) {
        return error("both operands must be numbers");
    }
    
    VmNumber * l = (VmNumber *) obj;
    VmNumber * r = (VmNumber *) e;
    
    VmNumber * newValue = countNewValue(l, r);
    environment->setVariable(identifier, newValue);
    
    //return VmVoid::VOID();
    return newValue;
}

VmNumber * AssignPlusFunction::countNewValue(VmNumber * l, VmNumber * r) {
    return new VmNumber(l->getValue() + r->getValue());
}

VmNumber * AssignMinusFunction::countNewValue(VmNumber * l, VmNumber * r) {
    return new VmNumber(l->getValue() - r->getValue());
}

VmNumber * AssignMultiplyFunction::countNewValue(VmNumber * l, VmNumber * r) {
    return new VmNumber(l->getValue() * r->getValue());
}

VmNumber * AssignDivideFunction::countNewValue(VmNumber * l, VmNumber * r) {
    if (r->getValue() == 0) {
        return new VmNumber(0);
    }
    
    return new VmNumber(l->getValue() / r->getValue());
}
