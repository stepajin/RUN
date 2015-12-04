//
//  Reader.h
//  VM
//
//  Created by Štěpánek Jindřich on 02/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef __Vm__Reader__
#define __Vm__Reader__

#include <iostream>
#include <fstream>

#include "Objects.h"
#include "Environment.h"

class VmObject;
class Environment;

class ReaderDataSource {

public:
    virtual BYTE getByte() = 0;
    virtual bool isEOF() = 0;
};

class Reader {
    ReaderDataSource * dataSource;
    Environment * environment;
    
    BYTE * buffer;
    int bufferSize;
    int bufferPos;
    int bufferRewinded;
    
    //void readObject();
    
public:
    Reader(ReaderDataSource * dataSource, Environment * environment);

    BYTE getByte();
    int getShortInt();
    
    void skip(int steps);
    void rewind(int steps);
    
    VmObject * getObject();
};

class FileDataSource : public ReaderDataSource {
    std::ifstream * input;
    
public:
    FileDataSource(std::ifstream * input);
    
    virtual BYTE getByte();
    virtual bool isEOF();
};

class BytecodeDataSource : public ReaderDataSource {
    BYTE * bytecode;
    int length;
    int position;
    
public:
    BytecodeDataSource(BYTE * bytecode, int length);

    virtual BYTE getByte();
    virtual bool isEOF();
};

int toInt(int length, unsigned char * bytes);

#endif /* defined(__Vm__Reader__) */
