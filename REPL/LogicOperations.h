//
//  LogicOperations.h
//  REPL
//
//  Created by Štěpánek Jindřich on 28/10/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef __REPL__LogicOperations__
#define __REPL__LogicOperations__

#include "Functions.h"

class LogicOperation : public LangFunction {
    LangObject * l;
    LangObject * r;

public:
    virtual LangObject * eval(Enviroment * enviroment);
    virtual bool evalBool(LangBoolean * l, LangBoolean * r) = 0;
    virtual bool readArgs(Reader * reader);
};

class AndOperation : public LogicOperation {
    virtual bool evalBool(LangBoolean * l, LangBoolean * r);
};

class OrOperation : public LogicOperation {
    virtual bool evalBool(LangBoolean * l, LangBoolean * r);
};

#endif /* defined(__REPL__LogicOperations__) */
