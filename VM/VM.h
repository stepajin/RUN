//
//  main.h
//  VM
//
//  Created by Štěpánek Jindřich on 02/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef Vm_main_h
#define Vm_main_h

#define BYTE unsigned char

#include <iostream>

enum BYTECODES {
    BC_CONST = 1,
    BC_CHAR = 2,
    BC_STRING = 3,
    BC_EQ = 4,
    BC_ASSIGN = 5,
    BC_PLUS = 6,
    BC_MINUS = 7,
    BC_DIVIDE = 8,
    BC_MULTIPLY = 9,
    BC_YES = 10,
    BC_NO = 11,
    BC_LIST = 12,
    BC_LOAD = 13,
    BC_SKIP = 14,
    BC_SKIP_IF_FALSE = 15,
    BC_FUNC_DEF = 16,
    BC_FUNC_CALL = 17,
    BC_AND = 18,
    BC_OR = 19,
    BC_PRINT = 20,
    BC_RETURN = 21,
    BC_AT = 22,
    BC_MORE = 23,
    BC_EQ_MORE = 24,
    BC_LESS = 25,
    BC_EQ_LESS = 26
};

enum VmObjectTag {
    TAG_NUMBER,
    TAG_STRING,
    TAG_BOOLEAN,
    TAG_BUILTIN_FUNCTION,
    TAG_USER_FUNCTION,
    TAG_EOF,
    TAG_BLOCK,
    TAG_VOID,
    TAG_LIST,
    TAG_RETURN,
    TAG_SKIP
};

#include "Objects.h"
#include "Reader.h"
#include "Enviroment.h"

class VmObject;
class Reader;
class Enviroment;

VmObject * REPL(Reader * reader, Enviroment * enviroment);

#endif
