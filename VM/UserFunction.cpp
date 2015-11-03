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

void UserFunction::readArguments(Reader * reader) {
    for (int i = numberOfArgs - 1; i >= 0; i--) {
        arguments[i] = CallStack::INSTANCE()->pop();
    }
    
    return;
}

VmObject * UserFunction::eval(Enviroment * enviroment) {

    BytecodeDataSource * data = new BytecodeDataSource(bytecode, length);
    Enviroment * newEnviroment = new Enviroment(enviroment);
    Reader * reader = new Reader(data, enviroment);

    for (int i = 0; i < numberOfArgs; i++) {
        VmObject * e = arguments[i]->eval(enviroment);
        newEnviroment->set(argIdentifiers[i], e);
    }
    
    int stackPointer = CallStack::INSTANCE()->getStackPointer();
    
    VmObject * result = REPL(reader, newEnviroment);

    CallStack::INSTANCE()->setStackPointer(stackPointer);
    
    return result;
}
