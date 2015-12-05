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

VmObject * PrintFunction::eval(Environment * environment) {
    if (arg == NULL) {
        cout << "NULL" << endl;
        return this;
    }
    
    VmObject * e = arg->eval(environment);

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

VmObject * LoadFunction::eval(Environment * environment) {
    return environment->getVariable(identifier);
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

VmObject * AtFunction::eval(Environment * environment) {
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

/*********
 
 Size
 
 *********/

SizeFunction::SizeFunction() : BuiltinFunction("size") {
    object = NULL;
}

VmObject * SizeFunction::eval(Environment * environment) {
    if (!object)
        return error("size: null object");
    
    if (object->getTag() == TAG_STRING) {
        VmString * str = (VmString *) object;
        return new VmNumber(str->size());
    }
    
    if (object->getTag() == TAG_LIST) {
        VmList * list = (VmList *)object;
        return new VmNumber(list->size());
    }
    
    return error("at: not valid object");
}

void SizeFunction::readArguments(Reader * reader) {
    object = CallStack::INSTANCE()->pop();
}

/*********
 
 Not 
 
 *********/

NotFunction::NotFunction() : BuiltinFunction("not") {
    object = NULL;
}

VmObject * NotFunction::eval(Environment * environment) {
    if (!object)
        return error("not: null object");
    
    if (object->getTag() == TAG_BOOLEAN) {
        VmBoolean * boo = (VmBoolean *) object;
        return VmBoolean::INSTANCE(!boo->getValue());
    }
    
    if (object->getTag() == TAG_NUMBER) {
        VmNumber * number = (VmNumber *)object;
        return VmBoolean::INSTANCE(number->getValue() != 0);
    }
    
    return error("not: not valid object");
}

void NotFunction::readArguments(Reader * reader) {
    object = CallStack::INSTANCE()->pop();
}
