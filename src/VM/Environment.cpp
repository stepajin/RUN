//
//  Environment.cpp
//  VM
//
//  Created by Štěpánek Jindřich on 03/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include "Environment.h"
#include "Heap.h"

bool contains(map<int, VmObject *> * store, int key) {
    if ((*store)[key] != NULL)
        return true;
    
    store->erase(key);
    return false;
}

Environment::Environment() {
    level = 0;
    
    variableStore = new map<int, VmObject *>;
    userFunctionStore = new map<int, VmObject *>;
    this->parentEnvironment = NULL;
    
    this->REFERENCES_CNT = 0;
    Heap::INSTANCE()->addEnvironment(this);
}

Environment::Environment(Environment * parent) {
    level = parent->level + 1;
    
    variableStore = new map<int, VmObject *>;
    userFunctionStore = new map<int, VmObject *>;
    this->parentEnvironment = parent;
    
    this->REFERENCES_CNT = 0;
    Heap::INSTANCE()->addEnvironment(this);
}

Environment::~Environment() {
    delete variableStore;
    delete userFunctionStore;
}

void Environment::retain() {
    this->REFERENCES_CNT++;
}

void Environment::release() {
    this->REFERENCES_CNT--;
}

/*****************
 
 Variable Store
 
 ****************/

void Environment::assignValue(int key, VmObject * value) {
//    cout << key << " -> " << value->toString() << " level " << level << endl;
    
    (*variableStore)[key] = value;
}

bool Environment::changeInParent(int key, VmObject * value) {
    if (isVariableSetInThis(key)) {
        assignValue(key, value);
        return true;
    }
    
    if (!parentEnvironment)
        return false;
    
    return parentEnvironment->changeInParent(key, value);
}

void Environment::setVariable(int key, VmObject * value) {
    if (changeInParent(key, value))
        return;
    
    assignValue(key, value);
}

VmObject * Environment::getVariable(int key) {
    if (contains(variableStore, key)) {
        return (*variableStore)[key];
    }
    
    if (parentEnvironment) {
        return parentEnvironment->getVariable(key);
    }
    
    return NULL;
}

bool Environment::isVariableSetInThis(int key) {
    return contains(variableStore, key);
}

bool Environment::isVariableSet(int key) {
    return getVariable(key) != NULL;
}

/*****************
 
 User Functions Store
 
 ****************/

void Environment::setUserFunction(int key, VmObject * func) {
    (*userFunctionStore)[key] = func;
}

VmObject * Environment::getUserFunction(int key) {
    if (contains(userFunctionStore, key)) {
        return (*userFunctionStore)[key];
    }
    
    if (parentEnvironment) {
        return parentEnvironment->getUserFunction(key);
    }
    
    return NULL;
}

bool Environment::isUserFunctionSetInThis(int key) {
    return contains(userFunctionStore, key);
}

bool Environment::isUserFunctionSet(int key) {
    return getUserFunction(key) != NULL;
}

/********************
 
 Garbage Collection
 
 *******************/

void Environment::markChildren() {
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
