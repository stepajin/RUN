//
//  Environment.h
//  VM
//
//  Created by Štěpánek Jindřich on 03/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef __Vm__Environment__
#define __Vm__Environment__

#include "Objects.h"
#include <map>

using namespace std;

class VmObject;

class Environment {
    Environment * parentEnvironment;
    
    map<int, VmObject *> * variableStore;
    map<int, VmObject *> * userFunctionStore;

    void assignValue(int key, VmObject * value);
    bool changeInParent(int key, VmObject * value);
    bool isVariableSetInThis(int key);
    bool isUserFunctionSetInThis(int key);
    
public:
    int REFERENCES_CNT;

    int level; // just for debug

    Environment();
    Environment(Environment * parent);
    ~Environment();
    
    void retain();
    void release();
    
    void setVariable(int key, VmObject * value);
    VmObject * getVariable(int key);
    bool isVariableSet(int key);

    void setUserFunction(int key, VmObject * func);
    VmObject * getUserFunction(int key);
    bool isUserFunctionSet(int key);
    
    void markChildren();

};

#endif /* defined(__Vm__Environment__) */
