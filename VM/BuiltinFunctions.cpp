//
//  Functions.cpp
//  VM
//
//  Created by Štěpánek Jindřich on 02/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include "VM.h"
#include "CallStack.h"
#include "BuiltinFunctions.h"

/*************

BuiltinFunction
 
*************/

BuiltinFunction::BuiltinFunction(string name) : VmObject(TAG_BUILTIN_FUNCTION) {
    this->name = name;
}

string BuiltinFunction::getName() {
    return name;
}

/**************
 
 Print
 
 ************/

PrintFunction::PrintFunction() : BuiltinFunction("print") {
    arg = NULL;
}

VmObject * PrintFunction::eval(Enviroment * enviroment) {
    if (arg == NULL) {
        cout << "NULL" << endl;
        return this;
    }
    
    VmObject * e = arg->eval(enviroment);

    if (!e) {
        cout << "NULL" << endl;
        return this;
    }

    cout << "> " << e->toString() << endl;
    
    return VmVoid::VOID();
}

void PrintFunction::readArguments(Reader * reader) {
    arg = CallStack::INSTANCE()->pop();
}


/*********
 
 Load
 
 *********/

LoadFunction::LoadFunction() : BuiltinFunction("load") {
}

VmObject * LoadFunction::eval(Enviroment * enviroment) {
    return enviroment->get(identifier);
}

void LoadFunction::readArguments(Reader * reader) {
    identifier = reader->getShortInt();
}