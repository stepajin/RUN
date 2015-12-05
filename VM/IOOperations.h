//
//  IOOperations.h
//  VM
//
//  Created by Štěpánek Jindřich on 05/12/15.
//  Copyright © 2015 stepajin. All rights reserved.
//

#ifndef IOOperations_h
#define IOOperations_h

#include <stdio.h>
#include "BuiltinFunctions.h"

class VmFile : public VmObject {
    VmObject * arg;
    
    string * name;
    std::ifstream * input;
    std::ofstream * output;
    
public:
    VmFile();
    ~VmFile();
    
    virtual void readArguments(Reader * reader);
    virtual VmObject * eval(Environment * environment);
    
    void write(string s);
    VmString * read();
};

class WriteFunction : public BuiltinFunction {
    VmObject * arg1;
    VmObject * arg2;
    
public:
    WriteFunction();
    
    virtual VmObject * eval(Environment * environment);
    virtual void readArguments(Reader * reader);
};


class ReadFunction : public BuiltinFunction {
    VmObject * arg;
    
public:
    ReadFunction();
    
    virtual VmObject * eval(Environment * environment);
    virtual void readArguments(Reader * reader);
};


#endif /* IOOperations_h */
