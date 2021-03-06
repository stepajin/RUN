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
    BC_MINUS = 250,
    BC_DIVIDE = 249,
    BC_MULTIPLY = 248,
    BC_YES = 247,
    BC_NO = 246,
    BC_LIST = 245,
    BC_LOAD = 244,
    BC_SKIP = 243,
    BC_SKIP_IF_FALSE = 242,
    BC_FUNC_DEF = 241,
    BC_FUNC_CALL = 240,
    BC_AND = 239,
    BC_OR = 238,
    BC_PRINT = 237,
    BC_AT = 236,
    BC_MORE = 235,
    BC_EQ_MORE = 234,
    BC_LESS = 233,
    BC_EQ_LESS = 232,
    BC_REWIND = 231,
    BC_STACK_MARK = 230,
    BC_STACK_MARK_RETURN = 229,
    BC_VOID = 228,
    BC_RETURN = 227,
    BC_SIZE = 226,
    BC_NOT = 225,
    BC_OPEN = 224,
    BC_WRITE = 223,
    BC_READ = 222,
    BC_EOF_STRING = 221,
    BC_ASSIGN_PLUS = 220,
    BC_ASSIGN_MINUS = 219,
    BC_ASSIGN_DIVIDE = 218,
    BC_ASSIGN_MULTIPLY = 217,
    BC_PUSH = 216,
    BC_POP = 215,
    BC_CLOSURE = 214,
    BC_ARGS = 213
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
    TAG_MOVE_BUFFER,
    TAG_FILE,
    TAG_CLOSURE
};

#define FORWARD true
#define BACK false
#define DIRECTION bool

#include "Objects.h"
#include "Reader.h"
#include "Environment.h"

class VmObject;
class Reader;
class Environment;
class VmList;

extern VmList * ARGUMENTS;

VmObject * REPL(Reader * reader, Environment * environment);

#endif
