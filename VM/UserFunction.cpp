//
//  UserFunction.cpp
//  VM
//
//  Created by Štěpánek Jindřich on 03/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include "UserFunction.h"
#include "CallStack.h"

UserFunction::UserFunction(int numberOfArgs, int * argIdentifiers, int length, BYTE * bytecode) : VmObject(TAG_USER_FUNCTION)
{
    this->numberOfArgs = numberOfArgs;
    this->argIdentifiers = argIdentifiers;
    
    this->arguments = new VmObject * [numberOfArgs];
    
    this->length = length;
    this->bytecode = bytecode;
}

UserFunction::~UserFunction() {
    delete [] argIdentifiers;
    delete arguments;
    delete [] bytecode;
}

void UserFunction::readArguments(Reader * reader) {
    for (int i = numberOfArgs - 1; i >= 0; i--) {
        arguments[i] = CallStack::INSTANCE()->pop();
    }
    
    return;
}

VmObject * UserFunction::eval(Environment * environment) {

    BytecodeDataSource * data = new BytecodeDataSource(bytecode, length);
    Environment * newEnvironment = new Environment(environment);
    Reader * reader = new Reader(data, environment);

    for (int i = 0; i < numberOfArgs; i++) {
        VmObject * e = arguments[i]->eval(environment);
        newEnvironment->setVariable(argIdentifiers[i], e);
    }
    
    VmObject * result = REPL(reader, newEnvironment);

    delete data;
    delete newEnvironment;
    delete reader;
    
    return result;
}

string UserFunction::toString() {
    return "func";
}