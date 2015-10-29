//
//  AssignFunctions.cpp
//  REPL
//
//  Created by Štěpánek Jindřich on 27/10/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include "AssignFunctions.h"
#include "Enviroment.h"
#include "main.h"

/***************
 
    Assign
 
 **************/

bool AssignFunction::readArgs(Reader * reader) {
    LangObject * obj = reader->getObject();
    if (obj->getTag() != TAG_IDENTIFIER) {
        error("assign : first argument must be identifier");
        return false;
    }
    
    this->identifier = (LangIdentifier *)obj;
    
    obj = reader->getObject();
    if (!obj)
        return false;
    
    value = obj;
    return true;
}

LangObject * AssignFunction::eval(Enviroment * enviroment) {
    LangObject * e = value->eval(enviroment);
    enviroment->set(identifier->getValue(), e);
    
    return LangVoid::VOID();
//    return e;
}

LangObject * AssignArithmeticFunction::eval(Enviroment * enviroment) {
    LangObject * obj = enviroment->get(identifier->getValue());

    if (!obj) {
        stringstream ss;
        ss << "object " << identifier->getValue() << " not found";
        error(ss.str());
        return NULL;
    }
    
    LangObject * e = value->eval(enviroment);

    if (e->getTag() != TAG_NUMBER || obj->getTag() != TAG_NUMBER) {
        error("both operands must be numbers");
        return NULL;
    }
    
    LangNumber * l = (LangNumber *) obj;
    LangNumber * r = (LangNumber *) e;

    LangNumber * newValue = countNewValue(l, r);
    enviroment->set(identifier->getValue(), newValue);
    
    return LangVoid::VOID();
//    return newValue;
}

LangNumber * AssignPlusFunction::countNewValue(LangNumber * l, LangNumber * r) {
    return new LangNumber(l->getValue() + r->getValue());
}

LangNumber * AssignMinusFunction::countNewValue(LangNumber * l, LangNumber * r) {
    return new LangNumber(l->getValue() - r->getValue());
}

LangNumber * AssignMultiplyFunction::countNewValue(LangNumber * l, LangNumber * r) {
    return new LangNumber(l->getValue() * r->getValue());
}

LangNumber * AssignDivideFunction::countNewValue(LangNumber * l, LangNumber * r) {
    if (r->getValue() == 0) {
        return new LangNumber(0);
    }
    
    return new LangNumber(l->getValue() / r->getValue());
}