//
//  ArithmeticOperations.cpp
//  REPL
//
//  Created by Štěpánek Jindřich on 26/10/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include "ArithmeticOperations.h"
#include "main.h"

ArithmeticOperation::ArithmeticOperation() : LangFunction("arithmetic") {}

bool ArithmeticOperation::readArgs(Reader * reader) {
    LangObject * arg1 = reader->getObject();
    LangObject * arg2 = reader->getObject();
    
    if (arg1 == NULL || arg2 == NULL) {
        return false;
    }
    
    l = arg1;
    r = arg2;
    
    return true;
}

bool ArithmeticOperation::evalOperands(Enviroment * enviroment) {
    l = l->eval(enviroment);
    r = r->eval(enviroment);
    
    if (l->getTag() != TAG_INTEGER || l->getTag() != TAG_INTEGER) {
        return false;
    }
    
    return true;
}

LangObject * ArithmeticOperation::eval(Enviroment * enviroment) {
    if (!evalOperands(enviroment)) {
        error("one of the operands is not a number");
        return NULL;
    }

    return evalOperation((LangInteger *) l, (LangInteger *) r);
}

/******************
 
    Operations
 
 ******************/

LangObject * PlusOperation::evalOperation(LangInteger * l, LangInteger * r) {
    return new LangInteger(l->getValue() + r->getValue());
}

LangObject * MinusOperation::evalOperation(LangInteger * l, LangInteger * r) {
    return new LangInteger(l->getValue() - r->getValue());
}

LangObject * MultiplyOperation::evalOperation(LangInteger * l, LangInteger * r) {
    return new LangInteger(l->getValue() * r->getValue());
}

LangObject * DivideOperation::evalOperation(LangInteger * l, LangInteger * r) {
    if (r->getValue() == 0)
        return new LangInteger(0);
    
    return new LangInteger(l->getValue() / r->getValue());
}

/***********************
 
    Compare operations

 ************************/

LangObject * EqualsMoreOperation::evalOperation(LangInteger * l, LangInteger * r) {
    return LangBoolean::INSTANCE(l->getValue() >= r->getValue());
}

LangObject * MoreOperation::evalOperation(LangInteger * l, LangInteger * r) {
    return LangBoolean::INSTANCE(l->getValue() > r->getValue());
}

LangObject * EqualsLessOperation::evalOperation(LangInteger * l, LangInteger * r) {
    return LangBoolean::INSTANCE(l->getValue() <= r->getValue());
}

LangObject * LessOperation::evalOperation(LangInteger * l, LangInteger * r) {
    return LangBoolean::INSTANCE(l->getValue() < r->getValue());
}

bool EqualsOperation::readArgs(Reader * reader) {
    LangObject * arg1 = reader->getObject();
    LangObject * arg2 = reader->getObject();
    
    if (arg1 == NULL || arg2 == NULL) {
        return false;
    }
    
    l = arg1;
    r = arg2;
    
    return true;
}

LangObject * EqualsOperation::eval(Enviroment * enviroment) {
    l = l->eval(enviroment);
    r = r->eval(enviroment);

    
    if (l->getTag() == TAG_INTEGER && r->getTag() == TAG_INTEGER) {
        bool b = ((LangInteger *)l)->getValue() == ((LangInteger *)r)->getValue();
        return LangBoolean::INSTANCE(b);
    }
    
    if (l->getTag() == TAG_STRING && r->getTag() == TAG_STRING) {
        bool b = ((LangString *)l)->getValue() == ((LangString *)r)->getValue();
        return LangBoolean::INSTANCE(b);
    }
    
    // compare pointers
    return LangBoolean::INSTANCE(l == r);
}



