//
//  Enviroment.cpp
//  REPL
//
//  Created by Štěpánek Jindřich on 26/10/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include "Enviroment.h"

Enviroment::Enviroment() {
    store = new map<string, LangObject *>;
    this->parentEnviroment = NULL;
}

Enviroment::Enviroment(Enviroment * parent) : Enviroment() {
    this->parentEnviroment = parent;
}

bool Enviroment::changeInParent(string key, LangObject * value) {
    if (isSetInThis(key)) {
        (*store)[key] = value;
        return true;
    }
    
    if (!parentEnviroment)
        return false;
    
    return parentEnviroment->changeInParent(key, value);
}

void Enviroment::set(string key, LangObject * value) {
    if (changeInParent(key, value))
        return;
    
    (*store)[key] = value;
}

LangObject * Enviroment::get(string key) {
    LangObject * obj = (*store)[key];

    if (obj) {
        return obj;
    }
    
    if (parentEnviroment) {
        return parentEnviroment->get(key);
    }
    
    return NULL;
}

bool Enviroment::isSetInThis(string key) {
    return (*store)[key] != NULL;
}

bool Enviroment::isSet(string key) {
    return get(key) != NULL;
}
