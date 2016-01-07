//
//  BinaryOperations.cpp
//  VM
//
//  Created by Štěpánek Jindřich on 02/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include "BinaryOperations.h"
#include "CallStack.h"
#include "VM.h"
#include "Error.h"

BinaryOperation::BinaryOperation() : BuiltinFunction("binary") {}

void BinaryOperation::readArguments(Reader * reader) {
    VmObject * arg1 = CallStack::INSTANCE()->pop();
    VmObject * arg2 = CallStack::INSTANCE()->pop();
    
    if (arg1 == NULL || arg2 == NULL) {
        return;
    }
    
    l = arg2;
    r = arg1;
}

/*****************************
 
 Equals
 
 ******************************/

VmObject * EqualsOperation::eval(Environment * environment) {
    VmObject * arg1 = l->eval(environment);
    VmObject * arg2 = r->eval(environment);
    
    if (arg1->getTag() == TAG_NUMBER && arg2->getTag() == TAG_NUMBER) {
        bool b = ((VmNumber *)arg1)->getValue() == ((VmNumber *)arg2)->getValue();
        return VmBoolean::INSTANCE(b);
    }
    
    if (arg1->getTag() == TAG_STRING && arg2->getTag() == TAG_STRING) {
        
        VmString * str1 = (VmString *)arg1;
        VmString * str2 = (VmString *)arg2;
        
        if (str1 == VmString::EOF_STRING() || str2 == VmString::EOF_STRING()) {
            // compare pointers
            return VmBoolean::INSTANCE(str1 == str2);
        }
        
        bool b = (str1)->getValue() == (str2)->getValue();
        return VmBoolean::INSTANCE(b);
    }
    
    // compare pointers
    return VmBoolean::INSTANCE(arg1 == arg2);
}

/*****************************
 
 Plus
 
 ******************************/


VmObject * PlusOperation::eval(Environment * environment) {
    VmObject * arg1 = l->eval(environment);
    VmObject * arg2 = r->eval(environment);
    
    if (arg1->getTag() == TAG_NUMBER && arg2->getTag() == TAG_NUMBER) {
        double d = ((VmNumber *)arg1)->getValue() + ((VmNumber *)arg2)->getValue();
        return new VmNumber(d);
    }
    
    if ((arg1->getTag() == TAG_STRING || arg1->getTag() == TAG_NUMBER) &&
        (arg2->getTag() == TAG_STRING || arg2->getTag() == TAG_NUMBER))
    {
        string s = arg1->toString();
        s += arg2->toString();
        return new VmString(s);
    }
    
    if (arg1->getTag() == TAG_LIST && arg2->getTag() == TAG_LIST) {
        VmList * list1 = (VmList *) arg1;
        VmList * list2 = (VmList *) arg2;
        VmList * list = new VmList();
        
        for (int i = 0; i < list1->size(); i++)
            list->push(list1->at(i));
        
        for (int i = 0; i < list2->size(); i++)
            list->push(list2->at(i));
        
        return list;
    } else if (arg1->getTag() == TAG_LIST) {
        VmList * list = (VmList *) arg1;
        list->push(arg2);
        return list;
    } else if (arg2->getTag() == TAG_LIST) {
        VmList * list = (VmList *) arg2;
        list->push(arg1);
        return list;
    }
    
    return error("plus: uncompatible operands");
}

/*****************************
 
 Binary of two numbers
 
 ******************************/


bool NumbersBinaryOperation::evalOperands(Environment * environment) {
    l = l->eval(environment);
    r = r->eval(environment);
    
    if (l->getTag() != TAG_NUMBER || l->getTag() != TAG_NUMBER) {
        return false;
    }
    
    return true;
}

VmObject * NumbersBinaryOperation::eval(Environment * environment) {
    if (!evalOperands(environment)) {
        return error("one of the operands is not a number");
    }
    
    return evalOperation((VmNumber *) l, (VmNumber *) r);
}

/******************
 
 Operations
 
 ******************/

VmObject * MinusOperation::evalOperation(VmNumber * l, VmNumber * r) {
    return new VmNumber(l->getValue() - r->getValue());
}

VmObject * MultiplyOperation::evalOperation(VmNumber * l, VmNumber * r) {
    return new VmNumber(l->getValue() * r->getValue());
}

VmObject * DivideOperation::evalOperation(VmNumber * l, VmNumber * r) {
    if (r->getValue() == 0)
        return new VmNumber(0);
    
    return new VmNumber(l->getValue() / r->getValue());
}

/***********************
 
 Compare operations
 
 ************************/

VmObject * EqualsMoreOperation::evalOperation(VmNumber * l, VmNumber * r) {
    return VmBoolean::INSTANCE(l->getValue() >= r->getValue());
}

VmObject * MoreOperation::evalOperation(VmNumber * l, VmNumber * r) {
    return VmBoolean::INSTANCE(l->getValue() > r->getValue());
}

VmObject * EqualsLessOperation::evalOperation(VmNumber * l, VmNumber * r) {
    return VmBoolean::INSTANCE(l->getValue() <= r->getValue());
}

VmObject * LessOperation::evalOperation(VmNumber * l, VmNumber * r) {
    return VmBoolean::INSTANCE(l->getValue() < r->getValue());
}
