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
    
    BuiltinFunction(string name);
    
public:
    string getName();
    
    virtual VmObject * eval(Environment * environment) = 0;
    virtual void readArguments(Reader * reader) = 0;
    virtual string toString();
};

class PrintFunction : public BuiltinFunction {
    VmObject * arg;
    
public:
    PrintFunction();

    virtual VmObject * eval(Environment * environment);
    virtual void readArguments(Reader * reader);
};

class LoadFunction : public BuiltinFunction {
protected:
    int identifier;
    
public:
    LoadFunction();
    
    virtual VmObject * eval(Environment * environment);
    virtual void readArguments(Reader * reader);
};

class AtFunction : public BuiltinFunction {
    VmObject * arg1;
    VmObject * arg2;
    
public:
    AtFunction();
    
    virtual VmObject * eval(Environment * environment);
    virtual void readArguments(Reader * reader);
};

class PushFunction : public BuiltinFunction {
    VmObject * arg1;
    VmObject * arg2;
    
public:
    PushFunction();
    
    virtual VmObject * eval(Environment * environment);
    virtual void readArguments(Reader * reader);
};

class PopFunction : public BuiltinFunction {
    VmObject * arg1;
    
public:
    PopFunction();
    
    virtual VmObject * eval(Environment * environment);
    virtual void readArguments(Reader * reader);
};

class SizeFunction : public BuiltinFunction {
    VmObject * object;
    
public:
    SizeFunction();
    
    virtual VmObject * eval(Environment * environment);
    virtual void readArguments(Reader * reader);
};

class NotFunction : public BuiltinFunction {
    VmObject * object;
    
public:
    NotFunction();
    
    virtual VmObject * eval(Environment * environment);
    virtual void readArguments(Reader * reader);
};




#endif /* defined(__Vm__Functions__) */
