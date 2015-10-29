//
//  BinaryOperations.cpp
//  REPL
//
//  Created by Štěpánek Jindřich on 26/10/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include "BinaryOperations.h"
#include "main.h"

BinaryOperation::BinaryOperation() : LangFunction("binary") {}

bool BinaryOperation::readArgs(Reader * reader) {
    LangObject * arg1 = reader->getObject();
    LangObject * arg2 = reader->getObject();
    
    if (arg1 == NULL || arg2 == NULL) {
        return false;
    }
    
    l = arg1;
    r = arg2;
    
    return true;
}

/*****************************
 
    Equals
 
 ******************************/

LangObject * EqualsOperation::eval(Enviroment * enviroment) {
    LangObject * arg1 = l->eval(enviroment);
    LangObject * arg2 = r->eval(enviroment);
    
    if (arg1->getTag() == TAG_NUMBER && arg2->getTag() == TAG_NUMBER) {
        bool b = ((LangNumber *)arg1)->getValue() == ((LangNumber *)arg2)->getValue();
        return LangBoolean::INSTANCE(b);
    }
    
    if (arg1->getTag() == TAG_STRING && arg2->getTag() == TAG_STRING) {
        bool b = ((LangString *)arg1)->getValue() == ((LangString *)arg2)->getValue();
        return LangBoolean::INSTANCE(b);
    }
    
    // compare pointers
    return LangBoolean::INSTANCE(arg1 == arg2);
}

/*****************************
 
    Plus
 
 ******************************/


LangObject * PlusOperation::eval(Enviroment * enviroment) {
    LangObject * arg1 = l->eval(enviroment);
    LangObject * arg2 = r->eval(enviroment);
    
    if (arg1->getTag() == TAG_NUMBER && arg2->getTag() == TAG_NUMBER) {
        double d = ((LangNumber *)arg1)->getValue() + ((LangNumber *)arg2)->getValue();
        return new LangNumber(d);
    }
    
    if (arg1->getTag() == TAG_STRING && arg2->getTag() == TAG_STRING) {
        string s = "";
        s += ((LangString *)arg1)->getValue();
        s += ((LangString *)arg2)->getValue();
        return new LangString(s);
    }
    
    if (arg1->getTag() == TAG_LIST && arg2->getTag() == TAG_LIST) {
        LangList * list1 = (LangList *) arg1;
        LangList * list2 = (LangList *) arg2;
        LangList * list = new LangList();
        
        for (int i = 0; i < list1->size(); i++)
            list->push(list1->at(i));

        for (int i = 0; i < list2->size(); i++)
            list->push(list2->at(i));
        
        return list;
    }
    
    
    return error("plus: uncompatible operands");
}

/*****************************
 
    Binary of two numbers
 
******************************/


bool NumbersBinaryOperation::evalOperands(Enviroment * enviroment) {
    l = l->eval(enviroment);
    r = r->eval(enviroment);
    
    if (l->getTag() != TAG_NUMBER || l->getTag() != TAG_NUMBER) {
        return false;
    }
    
    return true;
}

LangObject * NumbersBinaryOperation::eval(Enviroment * enviroment) {
    if (!evalOperands(enviroment)) {
        error("one of the operands is not a number");
        return NULL;
    }

    return evalOperation((LangNumber *) l, (LangNumber *) r);
}

/******************
 
    Operations
 
 ******************/

LangObject * MinusOperation::evalOperation(LangNumber * l, LangNumber * r) {
    return new LangNumber(l->getValue() - r->getValue());
}

LangObject * MultiplyOperation::evalOperation(LangNumber * l, LangNumber * r) {
    return new LangNumber(l->getValue() * r->getValue());
}

LangObject * DivideOperation::evalOperation(LangNumber * l, LangNumber * r) {
    if (r->getValue() == 0)
        return new LangNumber(0);
    
    return new LangNumber(l->getValue() / r->getValue());
}

/***********************
 
    Compare operations

 ************************/

LangObject * EqualsMoreOperation::evalOperation(LangNumber * l, LangNumber * r) {
    return LangBoolean::INSTANCE(l->getValue() >= r->getValue());
}

LangObject * MoreOperation::evalOperation(LangNumber * l, LangNumber * r) {
    return LangBoolean::INSTANCE(l->getValue() > r->getValue());
}

LangObject * EqualsLessOperation::evalOperation(LangNumber * l, LangNumber * r) {
    return LangBoolean::INSTANCE(l->getValue() <= r->getValue());
}

LangObject * LessOperation::evalOperation(LangNumber * l, LangNumber * r) {
    return LangBoolean::INSTANCE(l->getValue() < r->getValue());
}