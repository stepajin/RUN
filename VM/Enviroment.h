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
    
    map<int, VmObject *> * store;
    
    bool changeInParent(int key, VmObject * value);
    bool isSetInThis(int key);
    
public:
    Enviroment();
    Enviroment(Enviroment * parent);
    
    void set(int key, VmObject * value);
    VmObject * get(int key);
    bool isSet(int key);
};

#endif /* defined(__Vm__Enviroment__) */
