//
//  main.h
//  REPL
//
//  Created by Štěpánek Jindřich on 26/10/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef REPL_main_h
#define REPL_main_h

#include "Reader.h"
#include "Enviroment.h"

LangObject * repl(Reader * reader, Enviroment * enviroment);
LangObject * error(string message); // returns NULL

#endif
