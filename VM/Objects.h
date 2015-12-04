//
//  Objects.h
//  VM
//
//  Created by Štěpánek Jindřich on 02/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef __Vm__Objects__
#define __Vm__Objects__

#include <iostream>
#include <vector>

#include "VM.h"
#include "Reader.h"
#include "Enviroment.h"

using namespace std;

class Reader;
class Enviroment;

class VmObject {
    VmObjectTag tag;
    
    bool markFlag;
    
protected:
    VmObject(VmObjectTag tag);
    
    static VmObject * EOF_INSTANCE;

    bool retainFlag;
    
public:
    VmObjectTag getTag();
    
    virtual void readArguments(Reader * reader);
    
    static VmObject * getEOF();
    
    virtual string toString();
    
    virtual VmObject * eval(Enviroment * enviroment);
    
    void mark();
    void unmark();
    bool isMarked();
    bool isRetained();
};

class VmReturn : public VmObject {
    VmObject * value;
    
public:
    VmReturn();
    
    virtual void readArguments(Reader * reader);
    virtual VmObject * eval(Enviroment * enviroment);
    virtual string toString();
};

class VmVoid : public VmObject {
private:
    static VmVoid * INSTANCE;
    VmVoid();
    
public:
    static VmVoid * VOID();
    virtual string toString();
};

class VmNumber : public VmObject {
    double value;
public:
    VmNumber();
    VmNumber(double value);
    
    void setValue(double value);
    double getValue();
    
    virtual void readArguments(Reader * reader);
    
    virtual VmObject * eval(Enviroment * enviroment);
    virtual string toString();
};

class VmString : public VmObject {
    string value;
public:
    VmString();
    VmString(string value);
    
    void setValue(string value);
    string getValue();
    
    virtual string toString();
    virtual void readArguments(Reader * reader);
};

class VmBoolean : public VmObject {
    bool value;
    
    static VmBoolean * YES_INSTANCE;
    static VmBoolean * NO_INSTANCE;
    VmBoolean(bool value);
    
public:
    virtual string toString();
    
    static VmBoolean * YES();
    static VmBoolean * NO();
    static VmBoolean * INSTANCE(bool value);
    
    bool getValue();
};

class VmList : public VmObject {
    vector<VmObject *> * list;
    
public:
    VmList();
    
    VmObject * at(int idx);
    unsigned long size();
    void push(VmObject * obj);
    
    virtual void readArguments(Reader * reader);

    virtual VmObject * eval(Enviroment * enviroment);
    virtual string toString();
};

class VmMoveBuffer : public VmObject {
protected:
    int steps;
    DIRECTION direction;
    
public:
    VmMoveBuffer();
    
    virtual VmObject * eval(Enviroment * enviroment);
    virtual void readArguments(Reader * reader);
    
    void setDirection(DIRECTION direction);
    DIRECTION getDirection();
    
    int getSteps();
};


class VmSkipIfFalse : public VmMoveBuffer {
    VmObject * arg;
    
public:
    VmSkipIfFalse();
    
    virtual VmObject * eval(Enviroment * enviroment);
    virtual void readArguments(Reader * reader);
};

#endif /* defined(__Vm__Objects__) */
