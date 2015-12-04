//
//  Functions.cpp
//  VM
//
//  Created by Štěpánek Jindřich on 02/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include <sstream>

#include "VM.h"
#include "CallStack.h"
#include "BuiltinFunctions.h"
#include "Error.h"

/*************

BuiltinFunction
 
*************/

BuiltinFunction::BuiltinFunction(string name) : VmObject(TAG_BUILTIN_FUNCTION) {
    this->name = name;
}

string BuiltinFunction::getName() {
    return name;
}

string BuiltinFunction::toString() {
    return getName();
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
    return enviroment->getVariable(identifier);
}

void LoadFunction::readArguments(Reader * reader) {
    identifier = reader->getShortInt();
}

/*********
 
 At
 
 *********/

AtFunction::AtFunction() : BuiltinFunction("at") {
    object = NULL;
    idx = 0;
}

VmObject * AtFunction::eval(Enviroment * enviroment) {
    stringstream outOfBounds;
    outOfBounds << "index out of bounds: " << idx;
    
    if (!object)
        return error("at: not valid object");
    
    if (object->getTag() == TAG_STRING) {
        VmString * str = (VmString *) object;
        
        if (idx >= str->getValue().length())
            return error(outOfBounds.str());
        
        string s = "";
        s += str->getValue()[idx];
        return new VmString(s);
    }
    
    if (object->getTag() == TAG_LIST) {
        VmList * list = (VmList *)object;
        
        if (idx >= list->size())
            return error(outOfBounds.str());
        
        return list->at(idx);
    }
    
    return error("at: not valid object");
}

void AtFunction::readArguments(Reader * reader) {
    idx = reader->getShortInt();
    object = CallStack::INSTANCE()->pop();
}
