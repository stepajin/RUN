//
//  LangFunctions.h
//  REPL
//
//  Created by Štěpánek Jindřich on 26/10/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef REPL_LangFunctions_h
#define REPL_LangFunctions_h

#include "Objects.h"
#include "Reader.h"
#include "Blocks.h"

class LangFunction : public LangObject {
protected:
    string name;
    
    LangList * argKeysList;
    LangList * argList;
    
    LangBlock * block;
    
    LangFunction();

public:
    LangFunction(string name);
    
    void setBlock(LangBlock * block);
    string getName();
    bool hasBody();
    
    virtual bool readArgs(Reader * reader);
    
    virtual LangObject * eval(Enviroment * enviroment);
};



class IfFunction : public LangFunction {
private:
    LangBlock * condBlock;
    
    bool success;
    
public:
    IfFunction();
    
    bool succeeded();
    
    virtual LangObject * eval(Enviroment * enviroment);
    virtual bool readArgs(Reader * reader);
};

class ElseFunction : public LangFunction {
private:
    IfFunction * ifFunction;
    
public:
    ElseFunction();
    
    void setIfFunction(IfFunction * ifFunction);
    
    virtual LangObject * eval(Enviroment * enviroment);
    virtual bool readArgs(Reader * reader);
};

class LoopFunction : public LangFunction {
    
public:
    LoopFunction();
    
    virtual LangObject * eval(Enviroment * enviroment);
    virtual bool readArgs(Reader * reader);
};

class AtFunction : public LangFunction {
    string listKey;
    int idx;
    
public:
    AtFunction();
    
    virtual LangObject * eval(Enviroment * enviroment);
    virtual bool readArgs(Reader * reader);
};

#endif