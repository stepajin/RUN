//
//  Closure.cpp
//  VM
//
//  Created by Štěpánek Jindřich on 19/12/15.
//  Copyright © 2015 stepajin. All rights reserved.
//

#include "Closure.h"

VmClosure::VmClosure(int length, BYTE * bytecode, Environment * environment) : VmObject(TAG_CLOSURE)
{
    this->length = length;
    this->bytecode = bytecode;
    this->environment = environment;

    environment->retain();
}

VmClosure::~VmClosure() {
    delete [] bytecode;
    
    environment->release();
}

VmObject * VmClosure::eval(Environment * environment) {
    return this;
}

VmObject * VmClosure::execute() {
    BytecodeDataSource * data = new BytecodeDataSource(bytecode, length);
    Reader * reader = new Reader(data, environment);
    VmObject * result = REPL(reader, environment);

    delete data;
    delete reader;

    return result;
}

string VmClosure::toString() {
    return "closure";
}
