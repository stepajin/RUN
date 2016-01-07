//
//  LogicOperations.cpp
//  VM
//
//  Created by Štěpánek Jindřich on 02/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include "VM.h"
#include "LogicOperations.h"
#include "CallStack.h"
#include "Error.h"

LogicOperation::LogicOperation() : BuiltinFunction("logic") {
}

void LogicOperation::readArguments(Reader * reader) {
    VmObject * arg1 = CallStack::INSTANCE()->pop();
    VmObject * arg2 = CallStack::INSTANCE()->pop();
    
    if (arg1 == NULL || arg2 == NULL) {
        return;
    }
    
    l = arg2;
    r = arg1;
}

VmObject * LogicOperation::eval(Environment * environment) {
    VmObject * eval1 = l->eval(environment);
    VmObject * eval2 = r->eval(environment);
    
    if (!eval1 || !eval2) {
        return NULL;
    }
    
    if (eval1->getTag() != TAG_BOOLEAN || eval2->getTag() != TAG_BOOLEAN) {
        return error("both operands must be logic values");
    }
    
    VmBoolean * b1 = (VmBoolean *) eval1;
    VmBoolean * b2 = (VmBoolean *) eval2;
    
    bool res = evalBool(b1, b2);
    return VmBoolean::INSTANCE(res);
}

bool AndOperation::evalBool(VmBoolean * l, VmBoolean * r) {
    return l->getValue() && r->getValue();
}

bool OrOperation::evalBool(VmBoolean * l, VmBoolean * r) {
    return l->getValue() || r->getValue();
}
