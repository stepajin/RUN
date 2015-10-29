//
//  AssignFunctions.h
//  REPL
//
//  Created by Štěpánek Jindřich on 27/10/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef __REPL__AssignFunctions__
#define __REPL__AssignFunctions__

#include "Functions.h"

class AssignFunction : public LangFunction {
protected:
    LangIdentifier * identifier;
    LangObject * value;
    
public:
    virtual bool readArgs(Reader * reader);
    virtual LangObject * eval(Enviroment * enviroment);
};

class AssignArithmeticFunction : public AssignFunction {
    virtual LangNumber * countNewValue(LangNumber * l, LangNumber * r) = 0;

public:
    virtual LangObject * eval(Enviroment * enviroment);
};

class AssignPlusFunction : public AssignArithmeticFunction {
    virtual LangNumber * countNewValue(LangNumber * l, LangNumber * r);
};

class AssignMinusFunction : public AssignArithmeticFunction {
    virtual LangNumber * countNewValue(LangNumber * l, LangNumber * r);
};

class AssignMultiplyFunction : public AssignArithmeticFunction {
    virtual LangNumber * countNewValue(LangNumber * l, LangNumber * r);
};

class AssignDivideFunction : public AssignArithmeticFunction {
    virtual LangNumber * countNewValue(LangNumber * l, LangNumber * r);
};
#endif /* defined(__REPL__AssignFunctions__) */
