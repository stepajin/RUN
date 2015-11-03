//
//  Enviroment.h
//  VM
//
//  Created by Štěpánek Jindřich on 03/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef __Vm__Enviroment__
#define __Vm__Enviroment__

#include "Objects.h"
#include <map>

using namespace std;

class VmObject;

class Enviroment {
    Enviroment * parentEnviroment;
    
    map<int, VmObject *> * variableStore;
    map<int, VmObject *> * userFunctionStore;

    bool changeInParent(int key, VmObject * value);
    bool isVariableSetInThis(int key);
    bool isUserFunctionSetInThis(int key);
    
public:
    Enviroment();
    Enviroment(Enviroment * parent);
    
    void setVariable(int key, VmObject * value);
    VmObject * getVariable(int key);
    bool isVariableSet(int key);

    void setUserFunction(int key, VmObject * func);
    VmObject * getUserFunction(int key);
    bool isUserFunctionSet(int key);

};

#endif /* defined(__Vm__Enviroment__) */
