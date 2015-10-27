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
    string key;
    LangObject * value;
    
public:
    virtual bool readArgs(Reader * reader);
    virtual LangObject * eval(Enviroment * enviroment);
};

class AssignArithmeticFunction : public AssignFunction {
    virtual LangInteger * countNewValue(LangInteger * l, LangInteger * r) = 0;

public:
    virtual LangObject * eval(Enviroment * enviroment);
};

class AssignPlusFunction : public AssignArithmeticFunction {
    virtual LangInteger * countNewValue(LangInteger * l, LangInteger * r);
};

class AssignMinusFunction : public AssignArithmeticFunction {
    virtual LangInteger * countNewValue(LangInteger * l, LangInteger * r);
};

class AssignMultiplyFunction : public AssignArithmeticFunction {
    virtual LangInteger * countNewValue(LangInteger * l, LangInteger * r);
};

class AssignDivideFunction : public AssignArithmeticFunction {
    virtual LangInteger * countNewValue(LangInteger * l, LangInteger * r);
};
#endif /* defined(__REPL__AssignFunctions__) */
