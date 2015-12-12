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
#include "Heap.h"

#include <typeinfo>

using namespace std;

/***********
 
 Object
 
 ***********/

VmObject::VmObject(VmObjectTag tag) {
    this->tag = tag;
    
    this->markFlag = false;
    this->retainFlag = false;
    
    Heap::INSTANCE()->alloc(this);
}

VmObject::~VmObject() {

}

void VmObject::readArguments(Reader * reader) {
    return;
}

VmObjectTag VmObject::getTag() {
    return tag;
}

string VmObject::toString() {
    return typeid(*this).name();
//    return "unprintable object";
}

VmObject * VmObject::eval(Environment * environment) {
    return this;
}

void VmObject::mark() {
    if (isMarked())
        return;
    
    markFlag = true;

    markChildren();
}

void VmObject::unmark() {
    markFlag = false;
}

void VmObject::markChildren() {
    
}

bool VmObject::isMarked() {
    return markFlag;
}

bool VmObject::isRetained() {
    return retainFlag;
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

VmObject * VmReturn::eval(Environment * environment) {
    if (!value)
        return NULL;
    
    return value->eval(environment);
}

string VmReturn::toString() {
    return "return";
};

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

VmVoid::VmVoid() : VmObject(TAG_VOID) {
    this->retainFlag = true;
}

string VmVoid::toString() {
    return "void";
}

/************
 
 EOF
 
 ************/

VmEOF * VmEOF::INSTANCE;

VmEOF * VmEOF::getEOF() {
    if (INSTANCE == NULL) {
        INSTANCE = new VmEOF();
    }
    
    return INSTANCE;
}

VmEOF::VmEOF() : VmObject(TAG_EOF) {
    this->retainFlag = true;
}

string VmEOF::toString() {
    return "eof";
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

VmObject * VmNumber::eval(Environment * environment) {

    return this;
}

/**************
 
 String
 
 ***************/


VmString::VmString() : VmObject(TAG_STRING) {
    value = "";
}

VmString::VmString(string value) : VmObject(TAG_STRING) {
    this->value = value;
}

VmString * VmString::EOF_STRING_INSTANCE;
VmString * VmString::EOF_STRING() {
    if (EOF_STRING_INSTANCE == NULL) {
        EOF_STRING_INSTANCE = new VmString("EOF");
        EOF_STRING_INSTANCE->retainFlag = true;
        
        stringstream ss;
        ss << EOF;
        ss >> EOF_STRING_INSTANCE->value;
    }
    
    return EOF_STRING_INSTANCE;
}

void VmString::setValue(string value) {
    this->value = value;
}

unsigned long VmString::size() {
    return getValue().size();
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
    this->retainFlag = true;
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

VmList::~VmList() {
    delete list;
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

VmObject * VmList::pop() {
    VmObject * obj = list->back();
    list->pop_back();
    
    return obj;
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

VmObject * VmList::eval(Environment * environment) {
    for (int i = 0; i < size(); i++) {
        (*list)[i] = (*list)[i]->eval(environment);
    }
    
    return this;
}

void VmList::markChildren() {
    for (vector<VmObject *>::iterator it = list->begin() ; it != list->end(); ++it) {
        VmObject * obj = *it;
        if (!obj->isMarked()) {
            obj->mark();
        }
    }
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

VmObject * VmMoveBuffer::eval(Environment * environment) {
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

VmObject * VmSkipIfFalse::eval(Environment * environment) {
    VmObject * e = arg->eval(environment);
    
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
