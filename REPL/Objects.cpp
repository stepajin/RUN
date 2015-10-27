//
//  LangObjects.cpp
//  REPL
//
//  Created by Štěpánek Jindřich on 26/10/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//
#include <sstream>

#include "Objects.h"
#include "main.h"

using namespace std;

LangObject::LangObject(LangObjectTag tag) {
    this->tag = tag;
}

LangObject * LangObject::EOF_INSTANCE;
LangObject * LangObject::getEOF() {
    if (EOF_INSTANCE == NULL)
        EOF_INSTANCE = new LangObject(TAG_EOF);
    return EOF_INSTANCE;
}

LangObject * LangObject::END_INSTANCE;
LangObject * LangObject::getEND() {
    if (END_INSTANCE == NULL)
        END_INSTANCE = new LangObject(TAG_END);
    return END_INSTANCE;
}

LangObjectTag LangObject::getTag() {
    return tag;
}

LangObject * LangObject::eval(Enviroment * enviroment) {
    // default implementation
    return this;
}

string LangObject::toString() {
    return "unprintable object";
}

/* Void */

LangVoid * LangVoid::INSTANCE;

LangVoid * LangVoid::VOID() {
    if (INSTANCE == NULL) {
        INSTANCE = new LangVoid();
    }
    
    return INSTANCE;
}

LangVoid::LangVoid() : LangObject(TAG_VOID) {}

string LangVoid::toString() {
    return "";
}

/* Integer */

LangInteger::LangInteger(int value) : LangObject(TAG_INTEGER) {
    this->value = value;
}

void LangInteger::setValue(int value) {
    this->value = value;
}

int LangInteger::getValue() {
    return value;
}

string LangInteger::toString() {
    stringstream ss;
    ss << value;
    return ss.str();
}

/* String */

LangString::LangString(string value) : LangObject(TAG_STRING) {
    this->value = value;
}

void LangString::setValue(string value) {
    this->value = value;
}

string LangString::getValue() {
    return value;
}

string LangString::toString() {
    string s = "'";
    s += value;
    s += "'";
    return s;
}

/* Boolean */

LangBoolean::LangBoolean(bool value) : LangObject(TAG_BOOLEAN) {
    this->value = value;
}

LangBoolean * LangBoolean::YES_INSTANCE;
LangBoolean * LangBoolean::NO_INSTANCE;

LangBoolean * LangBoolean::INSTANCE(bool value) {
    return value ? YES() : NO();
}

LangBoolean * LangBoolean::YES() {
    if (YES_INSTANCE == NULL) {
        YES_INSTANCE = new LangBoolean(true);
    }
    
    return YES_INSTANCE;
}

LangBoolean * LangBoolean::NO() {
    if (NO_INSTANCE == NULL) {
        NO_INSTANCE = new LangBoolean(false);
    }
    
    return NO_INSTANCE;
}

string LangBoolean::toString() {
    return value ? "#YES" : "#NO";
}

/* List */

LangList::LangList() : LangObject(TAG_LIST) {
    list = new vector<LangObject *>;
}

LangObject * LangList::at(int idx) {
    if (idx >= size()) {
        return NULL;
    }
    
    return list->at(idx);
}

unsigned long LangList::size() {
    return list->size();
}

void LangList::push(LangObject * obj) {
    list->push_back(obj);
}

string LangList::toString() {
    string s;
    
    s += "#[ ";
    for (vector<LangObject *>::iterator it = list->begin() ; it != list->end(); ++it) {
        LangObject * obj = *it;
        s += obj->toString();
        s += " ";
    }
    s += "]";
    return s;
}
