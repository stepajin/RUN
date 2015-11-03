//
//  BinaryOperations.cpp
//  VM
//
//  Created by Štěpánek Jindřich on 02/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include "BinaryOperations.h"
#include "CallStack.h"
#include "Vm.h"
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

VmObject * EqualsOperation::eval(Enviroment * enviroment) {
    VmObject * arg1 = l->eval(enviroment);
    VmObject * arg2 = r->eval(enviroment);
    
    if (arg1->getTag() == TAG_NUMBER && arg2->getTag() == TAG_NUMBER) {
        bool b = ((VmNumber *)arg1)->getValue() == ((VmNumber *)arg2)->getValue();
        return VmBoolean::INSTANCE(b);
    }
    
    if (arg1->getTag() == TAG_STRING && arg2->getTag() == TAG_STRING) {
        bool b = ((VmString *)arg1)->getValue() == ((VmString *)arg2)->getValue();
        return VmBoolean::INSTANCE(b);
    }
    
    // compare pointers
    return VmBoolean::INSTANCE(arg1 == arg2);
}

/*****************************
 
 Plus
 
 ******************************/


VmObject * PlusOperation::eval(Enviroment * enviroment) {
    VmObject * arg1 = l->eval(enviroment);
    VmObject * arg2 = r->eval(enviroment);
    
    if (arg1->getTag() == TAG_NUMBER && arg2->getTag() == TAG_NUMBER) {
        double d = ((VmNumber *)arg1)->getValue() + ((VmNumber *)arg2)->getValue();
        return new VmNumber(d);
    }
    
    if (arg1->getTag() == TAG_STRING && arg2->getTag() == TAG_STRING) {
        string s = "";
        s += ((VmString *)arg1)->getValue();
        s += ((VmString *)arg2)->getValue();
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

VmObject * NumbersBinaryOperation::eval(Enviroment * enviroment) {
    if (!evalOperands(enviroment)) {
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
