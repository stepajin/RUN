//
//  BinaryOperations.h
//  REPL
//
//  Created by Štěpánek Jindřich on 26/10/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef __REPL__BinaryOperations__
#define __REPL__BinaryOperations__

#include "Functions.h"

class BinaryOperation : public LangFunction {
protected:
    LangObject * l;
    LangObject * r;
    
    BinaryOperation();
    
public:
    virtual bool readArgs(Reader * reader);
};

class EqualsOperation : public BinaryOperation {

public:
    virtual LangObject * eval(Enviroment * enviroment);
};

class PlusOperation : public BinaryOperation {
    
public:
    virtual LangObject * eval(Enviroment * enviroment);
};


/*****************************
 
 Binary of two numbers
 
 ******************************/

class NumbersBinaryOperation : public BinaryOperation {
    bool evalOperands(Enviroment * enviroment);
    virtual LangObject * evalOperation(LangNumber * l, LangNumber * r) = 0;
public:
    virtual LangObject * eval(Enviroment * enviroment);
};


class MinusOperation : public NumbersBinaryOperation {
    
public:
    virtual LangObject * evalOperation(LangNumber * l, LangNumber * r);
};

class MultiplyOperation : public NumbersBinaryOperation {
    
public:
    virtual LangObject * evalOperation(LangNumber * l, LangNumber * r);
};

class DivideOperation : public NumbersBinaryOperation {
    
public:
    virtual LangObject * evalOperation(LangNumber * l, LangNumber * r);
};

/***********************
 
 Compare operations
 
 ************************/

class EqualsMoreOperation : public NumbersBinaryOperation {
    
public:
    virtual LangObject * evalOperation(LangNumber * l, LangNumber * r);
};

class MoreOperation : public NumbersBinaryOperation {
    
public:
    virtual LangObject * evalOperation(LangNumber * l, LangNumber * r);
};

class EqualsLessOperation : public NumbersBinaryOperation {
    
public:
    virtual LangObject * evalOperation(LangNumber * l, LangNumber * r);
};

class LessOperation : public NumbersBinaryOperation {
    
public:
    virtual LangObject * evalOperation(LangNumber * l, LangNumber * r);
};


#endif /* defined(__REPL__BinaryOperations__) */
