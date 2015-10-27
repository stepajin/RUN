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
    string k = reader->getIdentifier();
    key = k;
    
    LangObject * v = reader->getObject();
    if (v == NULL) {
        return false;
    }
    
    value = v;
    
    return true;
}

LangObject * AssignFunction::eval(Enviroment * enviroment) {
    LangObject * e = value->eval(enviroment);
    enviroment->set(key, e);
    
    return e;
}

LangObject * AssignArithmeticFunction::eval(Enviroment * enviroment) {
    LangObject * obj = enviroment->get(key);

    if (!obj) {
        stringstream ss;
        ss << "object " << key << " not found";
        error(ss.str());
        return NULL;
    }
    
    LangObject * e = value->eval(enviroment);

    if (e->getTag() != TAG_INTEGER || obj->getTag() != TAG_INTEGER) {
        error("both operands mut be numbers");
        return NULL;
    }
    
    LangInteger * l = (LangInteger *) obj;
    LangInteger * r = (LangInteger *) e;

    LangInteger * newValue = countNewValue(l, r);
    enviroment->set(key, newValue);
    
    return newValue;
}

LangInteger * AssignPlusFunction::countNewValue(LangInteger * l, LangInteger * r) {
    return new LangInteger(l->getValue() + r->getValue());
}

LangInteger * AssignMinusFunction::countNewValue(LangInteger * l, LangInteger * r) {
    return new LangInteger(l->getValue() - r->getValue());
}

LangInteger * AssignMultiplyFunction::countNewValue(LangInteger * l, LangInteger * r) {
    return new LangInteger(l->getValue() * r->getValue());
}

LangInteger * AssignDivideFunction::countNewValue(LangInteger * l, LangInteger * r) {
    if (r->getValue() == 0) {
        return new LangInteger(0);
    }
    
    return new LangInteger(l->getValue() / r->getValue());
}