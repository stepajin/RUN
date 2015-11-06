//
//  Functions.h
//  VM
//
//  Created by Štěpánek Jindřich on 02/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef __Vm__Functions__
#define __Vm__Functions__

#include "Objects.h"

class BuiltinFunction : public VmObject {
protected:
    string name;
    
    //BuiltinFunction();
    BuiltinFunction(string name);
    
public:
    string getName();
    
    virtual VmObject * eval(Enviroment * enviroment) = 0;
    virtual void readArguments(Reader * reader) = 0;
};

class PrintFunction : public BuiltinFunction {
    VmObject * arg;
    
public:
    PrintFunction();

    virtual VmObject * eval(Enviroment * enviroment);
    virtual void readArguments(Reader * reader);
};

class LoadFunction : public BuiltinFunction {
protected:
    int identifier;
    
public:
    LoadFunction();
    
    virtual VmObject * eval(Enviroment * enviroment);
    virtual void readArguments(Reader * reader);
};

class AtFunction : public BuiltinFunction {
    VmObject * object;
    int idx;
    
public:
    AtFunction();
    
    virtual VmObject * eval(Enviroment * enviroment);
    virtual void readArguments(Reader * reader);
};




#endif /* defined(__Vm__Functions__) */