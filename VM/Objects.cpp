//
//  Objects.cpp
//  VM
//
//  Created by Štěpánek Jindřich on 02/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include <sstream>

#include "Objects.h"
#include "CallStack.h"
#include "VM.h"
#include "Error.h"


using namespace std;

/***************
 
 Singletons
 
 ***************/

VmObject * VmObject::EOF_INSTANCE;
VmObject * VmObject::getEOF() {
    if (EOF_INSTANCE == NULL)
        EOF_INSTANCE = new VmObject(TAG_EOF);
    return EOF_INSTANCE;
}

/***********
 
 Object
 
 ***********/

VmObject::VmObject(VmObjectTag tag) {
    this->tag = tag;
    
    this->markFlag = false;
}

void VmObject::readArguments(Reader * reader) {
    return;
}

VmObjectTag VmObject::getTag() {
    return tag;
}

string VmObject::toString() {
    return "unprintable object";
}

VmObject * VmObject::eval(Enviroment * enviroment) {
    return this;
}

void VmObject::mark() {
    markFlag = true;
}

void VmObject::unmark() {
    markFlag = false;
}

bool VmObject::isMarked() {
    return markFlag;
}

/************
 
 Return
 
 ***********/

VmReturn::VmReturn() : VmObject(TAG_RETURN) {
    value = NULL;
}

void VmReturn::readArguments(Reader * reader) {
    value = CallStack::INSTANCE()->pop();
}

VmObject * VmReturn::eval(Enviroment * enviroment) {
    if (!value)
        return NULL;
    
    return value->eval(enviroment);
}

/************
 
 Void
 
 ************/

VmVoid * VmVoid::INSTANCE;

VmVoid * VmVoid::VOID() {
    if (INSTANCE == NULL) {
        INSTANCE = new VmVoid();
    }
    
    return INSTANCE;
}

VmVoid::VmVoid() : VmObject(TAG_VOID) {}

string VmVoid::toString() {
    return "void";
}

/***********
 
 Number
 
 ***********/

VmNumber::VmNumber() : VmObject(TAG_NUMBER) {
    value = 0;
}

VmNumber::VmNumber(double value) : VmObject(TAG_NUMBER) {
    this->value = value;
}

void VmNumber::setValue(double value) {
    this->value = value;
}

double VmNumber::getValue() {
    return value;
}

string VmNumber::toString() {
    stringstream ss;
    ss << value;
    return ss.str();
}

void VmNumber::readArguments(Reader * reader) {    
    int number = reader->getShortInt();
    
    value = number;
    return;
}

VmObject * VmNumber::eval(Enviroment * enviroment) {
    return this;
}

/**************
 
 String
 
 ***************/

VmString::VmString() : VmObject(TAG_STRING) {
    value = "abc";
}

VmString::VmString(string value) : VmObject(TAG_STRING) {
    this->value = value;
}

void VmString::setValue(string value) {
    this->value = value;
}

string VmString::getValue() {
    return value;
}

string VmString::toString() {
    return value;
    
//    string s = "'";
//    s += value;
//    s += "'";
//    return s;
}

void VmString::readArguments(Reader * reader) {
    value = "";

    int length = reader->getShortInt();
    for (int i = 0; i < length; i++) {
        unsigned char c = reader->getByte();
        value += c;
    }
}

/*************
 
 Boolean 
 
 *************/

VmBoolean::VmBoolean(bool value) : VmObject(TAG_BOOLEAN) {
    this->value = value;
}

VmBoolean * VmBoolean::YES_INSTANCE;
VmBoolean * VmBoolean::NO_INSTANCE;

VmBoolean * VmBoolean::INSTANCE(bool value) {
    return value ? YES() : NO();
}

VmBoolean * VmBoolean::YES() {
    if (YES_INSTANCE == NULL) {
        YES_INSTANCE = new VmBoolean(true);
    }
    
    return YES_INSTANCE;
}

VmBoolean * VmBoolean::NO() {
    if (NO_INSTANCE == NULL) {
        NO_INSTANCE = new VmBoolean(false);
    }
    
    return NO_INSTANCE;
}

bool VmBoolean::getValue() {
    return this == YES();
}

string VmBoolean::toString() {
    return value ? "#YES" : "#NO";
}

/**********
 
 List 
 
 **********/

VmList::VmList() : VmObject(TAG_LIST) {
    list = new vector<VmObject *>;
}

VmObject * VmList::at(int idx) {
    if (idx >= size()) {
        return NULL;
    }
    
    return list->at(idx);
}

unsigned long VmList::size() {
    return list->size();
}

void VmList::push(VmObject * obj) {
    list->push_back(obj);
}

void VmList::readArguments(Reader * reader) {
    int size = reader->getShortInt();
    
    vector<VmObject *> args;
    
    for (int i = 0; i < size; i++) {
        VmObject * obj = CallStack::INSTANCE()->pop();
        args.push_back(obj);
    }
    
    // flip over
    for (int i = size - 1; i >= 0; i--)
        push(args[i]);
}

VmObject * VmList::eval(Enviroment * enviroment) {
    for (int i = 0; i < size(); i++)
        (*list)[i] = (*list)[i]->eval(enviroment);
    
    return this;
}

string VmList::toString() {
    string s;
    
    s += "#[ ";
    for (vector<VmObject *>::iterator it = list->begin() ; it != list->end(); ++it) {
        VmObject * obj = *it;
        s += obj->toString();
        s += " ";
    }
    s += "]";
    return s;
}

/***********
 
 Move buffer
 
 **********/

VmMoveBuffer::VmMoveBuffer() : VmObject(TAG_MOVE_BUFFER) {
    steps = 0;
    direction = FORWARD;
}

VmObject * VmMoveBuffer::eval(Enviroment * enviroment) {
    return this;
}

void VmMoveBuffer::readArguments(Reader * reader) {
    steps = reader->getShortInt();
};

int VmMoveBuffer::getSteps() {
    return steps;
}

void VmMoveBuffer::setDirection(DIRECTION direction) {
    this->direction = direction;
}

DIRECTION VmMoveBuffer::getDirection() {
    return direction;
}

/***************
 
 Skip if false
 
 ***************/

VmSkipIfFalse::VmSkipIfFalse() : VmMoveBuffer() {
    arg = NULL;
    direction = FORWARD;
}

VmObject * VmSkipIfFalse::eval(Enviroment * enviroment) {
    VmObject * e = arg->eval(enviroment);
    
    if (!e || e->getTag() != TAG_BOOLEAN) {
        return error("SkipIfFalse: argument has to be logic value");
    }
    
    VmBoolean * b = (VmBoolean *) e;
    
    if (b->getValue())
        return VmVoid::VOID();
    
    return this;
}

void VmSkipIfFalse::readArguments(Reader * reader) {
    steps = reader->getShortInt();
    arg = CallStack::INSTANCE()->pop();
}
