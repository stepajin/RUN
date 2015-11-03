//
//  Enviroment.cpp
//  VM
//
//  Created by Štěpánek Jindřich on 03/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include "Enviroment.h"

Enviroment::Enviroment() {
    store = new map<int, VmObject *>;
    this->parentEnviroment = NULL;
}

Enviroment::Enviroment(Enviroment * parent) {
    store = new map<int, VmObject *>;
    this->parentEnviroment = parent;
}

bool Enviroment::changeInParent(int key, VmObject * value) {
    if (isSetInThis(key)) {
        (*store)[key] = value;
        return true;
    }
    
    if (!parentEnviroment)
        return false;
    
    return parentEnviroment->changeInParent(key, value);
}

void Enviroment::set(int key, VmObject * value) {
    if (changeInParent(key, value))
        return;
    
    (*store)[key] = value;
}

VmObject * Enviroment::get(int key) {
    VmObject * obj = (*store)[key];
    
    if (obj) {
        return obj;
    }
    
    if (parentEnviroment) {
        return parentEnviroment->get(key);
    }
    
    return NULL;
}

bool Enviroment::isSetInThis(int key) {
    return false;
    return (*store)[key] != NULL;
}

bool Enviroment::isSet(int key) {
    return get(key) != NULL;
}
