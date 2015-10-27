//
//  Enviroment.h
//  REPL
//
//  Created by Štěpánek Jindřich on 26/10/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef __REPL__Enviroment__
#define __REPL__Enviroment__

#include "Objects.h"
#include <map>

class Enviroment {
    map<std::string, LangObject *> * store;    
    
public:
    Enviroment();
    Enviroment(Enviroment * parent);
    
    void set(string key, LangObject * value);
    LangObject * get(string key);
    bool isSet(string key);
};

#endif /* defined(__REPL__Enviroment__) */
