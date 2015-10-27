//
//  ArithmeticOperations.h
//  REPL
//
//  Created by Štěpánek Jindřich on 26/10/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef __REPL__ArithmeticOperations__
#define __REPL__ArithmeticOperations__

#include "Functions.h"

class ArithmeticOperation : public LangFunction {
protected:
    LangObject * l;
    LangObject * r;
    
    ArithmeticOperation();
    
    bool evalOperands(Enviroment * enviroment);
    virtual LangObject * evalOperation(LangInteger * l, LangInteger * r) = 0;
    
public:
    virtual bool readArgs(Reader * reader);
    virtual LangObject * eval(Enviroment * enviroment);
};

class PlusOperation : public ArithmeticOperation {
    
public:
    virtual LangObject * evalOperation(LangInteger * l, LangInteger * r);
};

class MinusOperation : public ArithmeticOperation {
    
public:
    virtual LangObject * evalOperation(LangInteger * l, LangInteger * r);
};

class MultiplyOperation : public ArithmeticOperation {
    
public:
    virtual LangObject * evalOperation(LangInteger * l, LangInteger * r);
};

class DivideOperation : public ArithmeticOperation {
    
public:
    virtual LangObject * evalOperation(LangInteger * l, LangInteger * r);
};

/***********************
 
 Compare operations
 
 ************************/

class EqualsMoreOperation : public ArithmeticOperation {
    
public:
    virtual LangObject * evalOperation(LangInteger * l, LangInteger * r);
};

class MoreOperation : public ArithmeticOperation {
    
public:
    virtual LangObject * evalOperation(LangInteger * l, LangInteger * r);
};

class EqualsLessOperation : public ArithmeticOperation {
    
public:
    virtual LangObject * evalOperation(LangInteger * l, LangInteger * r);
};

class LessOperation : public ArithmeticOperation {
    
public:
    virtual LangObject * evalOperation(LangInteger * l, LangInteger * r);
};

class EqualsOperation : public LangFunction {
    LangObject * l;
    LangObject * r;
    
    virtual LangObject * eval(Enviroment * enviroment);
    virtual bool readArgs(Reader * reader);
    
};

#endif /* defined(__REPL__ArithmeticOperations__) */