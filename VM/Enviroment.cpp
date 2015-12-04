//
//  Enviroment.cpp
//  VM
//
//  Created by Štěpánek Jindřich on 03/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include "Enviroment.h"

bool contains(map<int, VmObject *> * store, int key) {
    if ((*store)[key] != NULL)
        return true;
    
    store->erase(key);
    return false;
}

Enviroment::Enviroment() {
    level = 0;
    
    variableStore = new map<int, VmObject *>;
    userFunctionStore = new map<int, VmObject *>;
    this->parentEnviroment = NULL;
}

Enviroment::Enviroment(Enviroment * parent) {
    level = parent->level + 1;
    
    variableStore = new map<int, VmObject *>;
    userFunctionStore = new map<int, VmObject *>;
    this->parentEnviroment = parent;
}

/*****************
 
 Variable Store
 
 ****************/

void Enviroment::assignValue(int key, VmObject * value) {
//    cout << key << " -> " << value->toString() << " level " << level << endl;
    
    (*variableStore)[key] = value;
}

bool Enviroment::changeInParent(int key, VmObject * value) {
    if (isVariableSetInThis(key)) {
        assignValue(key, value);
        return true;
    }
    
    if (!parentEnviroment)
        return false;
    
    return parentEnviroment->changeInParent(key, value);
}

void Enviroment::setVariable(int key, VmObject * value) {
    if (changeInParent(key, value))
        return;
    
    assignValue(key, value);
}

VmObject * Enviroment::getVariable(int key) {
    if (contains(variableStore, key)) {
        return (*variableStore)[key];
    }
    
    if (parentEnviroment) {
        return parentEnviroment->getVariable(key);
    }
    
    return NULL;
}

bool Enviroment::isVariableSetInThis(int key) {
    return contains(variableStore, key);
}

bool Enviroment::isVariableSet(int key) {
    return getVariable(key) != NULL;
}

/*****************
 
 User Functions Store
 
 ****************/

void Enviroment::setUserFunction(int key, VmObject * func) {
    (*userFunctionStore)[key] = func;
}

VmObject * Enviroment::getUserFunction(int key) {
    if (contains(userFunctionStore, key)) {
        return (*userFunctionStore)[key];
    }
    
    if (parentEnviroment) {
        return parentEnviroment->getUserFunction(key);
    }
    
    return NULL;
}

bool Enviroment::isUserFunctionSetInThis(int key) {
    return contains(userFunctionStore, key);
}

bool Enviroment::isUserFunctionSet(int key) {
    return getUserFunction(key) != NULL;
}

/********************
 
 Garbage Collection
 
 *******************/

void Enviroment::markChildren() {
    for (map<int, VmObject *>::iterator it = variableStore->begin(); it != variableStore->end(); it++) {
        
        VmObject * obj = it->second;
        
        if (!obj->isMarked()) {
            obj->mark();
        }
    }

    for (map<int, VmObject *>::iterator it = userFunctionStore->begin(); it != userFunctionStore->end(); it++) {
        
        VmObject * obj = it->second;
        
        if (!obj->isMarked()) {
            obj->mark();
        }
    }
}
