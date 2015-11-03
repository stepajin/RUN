//
//  Enviroment.cpp
//  VM
//
//  Created by Štěpánek Jindřich on 03/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include "Enviroment.h"

Enviroment::Enviroment() {
    variableStore = new map<int, VmObject *>;
    userFunctionStore = new map<int, VmObject *>;
    this->parentEnviroment = NULL;
}

Enviroment::Enviroment(Enviroment * parent) {
    variableStore = new map<int, VmObject *>;
    userFunctionStore = new map<int, VmObject *>;
    this->parentEnviroment = parent;
}

/*****************
 
 Variable Store
 
 ****************/

bool Enviroment::changeInParent(int key, VmObject * value) {
    if (isVariableSetInThis(key)) {
        (*variableStore)[key] = value;
        return true;
    }
    
    if (!parentEnviroment)
        return false;
    
    return parentEnviroment->changeInParent(key, value);
}

void Enviroment::setVariable(int key, VmObject * value) {
    if (changeInParent(key, value))
        return;
    
    (*variableStore)[key] = value;
}

VmObject * Enviroment::getVariable(int key) {
    VmObject * obj = (*variableStore)[key];
    
    if (obj) {
        return obj;
    }
    
    if (parentEnviroment) {
        return parentEnviroment->getVariable(key);
    }
    
    return NULL;
}

bool Enviroment::isVariableSetInThis(int key) {
    //return false;
    return (*variableStore)[key] != NULL;
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
    VmObject * func = (*userFunctionStore)[key];
    
    if (func) {
        return func;
    }
    
    if (parentEnviroment) {
        return parentEnviroment->getUserFunction(key);
    }
    
    return NULL;
}

bool Enviroment::isUserFunctionSetInThis(int key) {
    return (*userFunctionStore)[key] != NULL;
}

bool Enviroment::isUserFunctionSet(int key) {
    return getUserFunction(key) != NULL;
}

