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
    BC_NUMBER = 255,
    BC_STRING = 254,
    BC_EQ = 253,
    BC_ASSIGN = 252,
    BC_PLUS = 251,
    BC_MINUS = 251,
    BC_DIVIDE = 250,
    BC_MULTIPLY = 249,
    BC_YES = 248,
    BC_NO = 247,
    BC_LIST = 246,
    BC_LOAD = 245,
    BC_SKIP = 244,
    BC_SKIP_IF_FALSE = 243,
    BC_FUNC_DEF = 242,
    BC_FUNC_CALL = 241,
    BC_AND = 240,
    BC_OR = 239,
    BC_PRINT = 238,
    BC_RETURN = 237,
    BC_AT = 236,
    BC_MORE = 235,
    BC_EQ_MORE = 234,
    BC_LESS = 233,
    BC_EQ_LESS = 232,
    BC_REWIND = 231,
    BC_STACK_MARK = 230,
    BC_STACK_MARK_RETURN = 229
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
    TAG_MOVE_BUFFER
};

#define FORWARD true
#define BACK false
#define DIRECTION bool

#include "Objects.h"
#include "Reader.h"
#include "Enviroment.h"

class VmObject;
class Reader;
class Enviroment;

VmObject * REPL(Reader * reader, Enviroment * enviroment);

#endif
