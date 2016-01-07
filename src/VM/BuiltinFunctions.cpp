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
#include "Closure.h"

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
    VmObject * load = environment->getVariable(identifier);
    if (load->getTag() != TAG_CLOSURE) {
        return load;
    }

    // closure
    VmClosure * closure = (VmClosure *)load;
    return closure->execute();
}

void LoadFunction::readArguments(Reader * reader) {
    identifier = reader->getShortInt();
}

/*********
 
 At
 
 *********/

AtFunction::AtFunction() : BuiltinFunction("at") {
    arg1 = NULL;
    arg2 = NULL;
}

VmObject * AtFunction::eval(Environment * environment) {
    VmObject * object = arg1->eval(environment);
    VmObject * index = arg2->eval(environment);
    
    if (index->getTag() != TAG_NUMBER) {
        return error("at: index is not a number");
    }
    
    int idx = (int)((VmNumber *) index)->getValue();
    
    stringstream outOfBounds;
    outOfBounds << "at: index out of bounds: " << idx;
    
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
    arg2 = CallStack::INSTANCE()->pop();
    arg1 = CallStack::INSTANCE()->pop();
}

/***********
 
 Push
 
 ***********/

PushFunction::PushFunction() : BuiltinFunction("push") {
    arg1 = NULL;
    arg2 = NULL;
}

void PushFunction::readArguments(Reader * reader) {
    arg2 = CallStack::INSTANCE()->pop();
    arg1 = CallStack::INSTANCE()->pop();
}

VmObject * PushFunction::eval(Environment * environment) {
    VmObject * listEval = arg1->eval(environment);
    VmObject * obj = arg2->eval(environment);
    
    if (listEval->getTag() != TAG_LIST) {
        return error("push: first argument has to be list");
    }
    
    VmList * list = (VmList *) listEval;
    list->push(obj);
    
    return VmVoid::VOID();
}

/*********
 
 Pop
 
**********/

PopFunction::PopFunction() : BuiltinFunction("pop") {
    arg1 = NULL;
}

void PopFunction::readArguments(Reader *reader) {
    arg1 = CallStack::INSTANCE()->pop();
}

VmObject * PopFunction::eval(Environment *environment) {
    VmObject * listEval = arg1->eval(environment);
    
    if (listEval->getTag() != TAG_LIST) {
        return error("pop: argument has to be list");
    }
    
    VmList * list = (VmList *) listEval;
    
    if (list->size() == 0) {
        return error("pop: empty list");
    }
    
    VmObject * obj = list->pop();
    
    return obj;
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
    
    return error("size: not valid object");
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
