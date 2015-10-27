//
//  LangBlocks.h
//  REPL
//
//  Created by Štěpánek Jindřich on 26/10/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef __REPL__Blocks__
#define __REPL__Blocks__

#include <iostream>
#include "Objects.h"

class LangBlock : public LangObject {
    string block;
    bool printable;

public:
    LangBlock(string block);
    string getBlock();
    void setPrintable(bool printable);
    
    virtual LangObject * eval(Enviroment * enviroment);
    virtual string toString();
};

#endif /* defined(__REPL__Blocks__) */
