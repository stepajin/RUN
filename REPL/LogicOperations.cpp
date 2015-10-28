//
//  LogicOperations.cpp
//  REPL
//
//  Created by Štěpánek Jindřich on 28/10/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include "LogicOperations.h"
#include "main.h"

bool LogicOperation::readArgs(Reader * reader) {
    LangObject * arg1 = reader->getObject();
    LangObject * arg2 = reader->getObject();
    
    if (arg1 == NULL || arg2 == NULL) {
        return false;
    }
    
    
    l = arg1;
    r = arg2;
    
    return true;
}

LangObject * LogicOperation::eval(Enviroment * enviroment) {
    LangObject * eval1 = l->eval(enviroment);
    LangObject * eval2 = r->eval(enviroment);
    
    if (!eval1 || !eval2) {
        return NULL;
    }
    
    if (eval1->getTag() != TAG_BOOLEAN || eval2->getTag() != TAG_BOOLEAN) {
        error("both operands must be logic values");
        return NULL;
    }
    
    LangBoolean * b1 = (LangBoolean *) eval1;
    LangBoolean * b2 = (LangBoolean *) eval2;
    
    bool res = evalBool(b1, b2);
    return LangBoolean::INSTANCE(res);
}

bool AndOperation::evalBool(LangBoolean * l, LangBoolean * r) {
    return l->getValue() && r->getValue();
}

bool OrOperation::evalBool(LangBoolean * l, LangBoolean * r) {
    return l->getValue() || r->getValue();
}
