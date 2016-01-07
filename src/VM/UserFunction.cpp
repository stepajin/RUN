//
//  UserFunction.cpp
//  VM
//
//  Created by Štěpánek Jindřich on 03/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include "UserFunction.h"
#include "CallStack.h"

UserFunction::UserFunction(int numberOfArgs, int * argIdentifiers, int length, BYTE * bytecode, Environment * parentEnvironment) : VmObject(TAG_USER_FUNCTION)
{
    this->numberOfArgs = numberOfArgs;
    this->argIdentifiers = argIdentifiers;
    
    this->arguments = new VmObject * [numberOfArgs];
    
    this->length = length;
    this->bytecode = bytecode;
    this->parentEnvironment = parentEnvironment;
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
    
    Environment * newEnvironment = new Environment(parentEnvironment);
    newEnvironment->retain();
    
    Reader * reader = new Reader(data, newEnvironment);

    for (int i = 0; i < numberOfArgs; i++) {
        VmObject * e = arguments[i]->eval(parentEnvironment);
        newEnvironment->setVariable(argIdentifiers[i], e);
    }
    
    VmObject * result = REPL(reader, newEnvironment);
    newEnvironment->release();
    
    delete data;
    delete reader;
    
    return result;
}

string UserFunction::toString() {
    return "func";
}
