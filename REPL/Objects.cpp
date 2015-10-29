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
    return this;
}

string LangObject::toString() {
    return "unprintable object";
}

/* Identifier */

LangIdentifier::LangIdentifier(string identifier) : LangObject(TAG_IDENTIFIER) {
    this->identifier = identifier;
}

string LangIdentifier::getValue() {
    return identifier;
}

string LangIdentifier::toString() {
    string s = "#";
    s += getValue();
    return s;
}

LangObject * LangIdentifier::eval(Enviroment * enviroment) {
    LangObject * obj = enviroment->get(identifier);
    if (!obj) {
        stringstream ss;
        ss << "identifier " << identifier << " not set";
        error(ss.str());
        return NULL;
    }

    return obj->eval(enviroment);
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

/* Number */

LangNumber::LangNumber(double value) : LangObject(TAG_INTEGER) {
    this->value = value;
}

void LangNumber::setValue(double value) {
    this->value = value;
}

double LangNumber::getValue() {
    return value;
}

string LangNumber::toString() {
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

bool LangBoolean::getValue() {
    return this == YES();
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
