//
//  Reader.cpp
//  VM
//
//  Created by Štěpánek Jindřich on 02/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include "Reader.h"

#include "Objects.h"
#include "BuiltinFunctions.h"
#include "BinaryOperations.h"
#include "LogicOperations.h"
#include "AssignFunctions.h"
#include "UserFunction.h"
#include "IOOperations.h"
#include "CallStack.h"
#include "Closure.h"

using namespace std;

#define EMPTY 99999

Reader::Reader(ReaderDataSource * dataSource, Environment * environment) {
    bufferSize = 10000;
    bufferPos = 0;
    bufferRewinded = 0;
    buffer = new BYTE[bufferSize];
    
    this->dataSource = dataSource;
    this->environment = environment;
}

Reader::~Reader() {
    delete [] buffer;
}

VmObject * Reader::getObject() {
    BYTE byte = getByte();

    if (dataSource->isEOF())
        return VmEOF::getEOF();
    
    if (byte == BC_STACK_MARK) {
        CallStack::INSTANCE()->addMark();
        return getObject();
    }
    
    if (byte == BC_STACK_MARK_RETURN) {
        CallStack::INSTANCE()->returnToLastMark();
        return getObject();
    }
    
    if (byte == BC_NUMBER) {
        VmNumber * number = new VmNumber();
        number->readArguments(this);
        return number;
    }
    
    if (byte == BC_STRING) {
        VmString * str = new VmString();
        str->readArguments(this);
        return str;
    }
    
    if (byte == BC_EOF_STRING) {
        return VmString::EOF_STRING();
    }
    
    if (byte == BC_VOID) {
        return VmVoid::VOID();
    }
    
    if (byte == BC_ASSIGN) {
        AssignFunction * assign = new AssignFunction();
        assign->readArguments(this);
        return assign;
    }
    
    if (byte == BC_ASSIGN_PLUS) {
        AssignPlusFunction * assign = new AssignPlusFunction();
        assign->readArguments(this);
        return assign;
    }

    if (byte == BC_ASSIGN_MINUS) {
        AssignMinusFunction * assign = new AssignMinusFunction();
        assign->readArguments(this);
        return assign;
    }

    if (byte == BC_ASSIGN_DIVIDE) {
        AssignDivideFunction * assign = new AssignDivideFunction();
        assign->readArguments(this);
        return assign;
    }
    
    if (byte == BC_ASSIGN_MULTIPLY) {
        AssignMultiplyFunction * assign = new AssignMultiplyFunction();
        assign->readArguments(this);
        return assign;
    }

    if (byte == BC_LOAD) {
        LoadFunction * load = new LoadFunction();
        load->readArguments(this);
        return load;
    }
    
    if (byte == BC_YES) {
        return VmBoolean::YES();
    }
    
    if (byte == BC_NO) {
        return VmBoolean::NO();
    }

    if (byte == BC_NOT) {
        NotFunction * no = new NotFunction();
        no->readArguments(this);
        return no;
    }
    
    if (byte == BC_LIST) {
        VmList * list = new VmList();
        list->readArguments(this);
        return list;
    }
    
    if (byte == BC_AT) {
        AtFunction * at = new AtFunction();
        at->readArguments(this);
        return at;
    }
    
    if (byte == BC_PUSH) {
        PushFunction * push = new PushFunction();
        push->readArguments(this);
        return push;
    }
    
    if (byte == BC_POP) {
        PopFunction * pop = new PopFunction();
        pop->readArguments(this);
        return pop;
    }
    
    if (byte == BC_SIZE) {
        SizeFunction * size = new SizeFunction();
        size->readArguments(this);
        return size;
    }
    
    if (byte == BC_EQ) {
        EqualsOperation * eq = new EqualsOperation();
        eq->readArguments(this);
        return eq;
    }

    if (byte == BC_MORE) {
        MoreOperation * more = new MoreOperation();
        more->readArguments(this);
        return more;
    }

    if (byte == BC_EQ_MORE) {
        EqualsMoreOperation * eqMore = new EqualsMoreOperation();
        eqMore->readArguments(this);
        return eqMore;
    }
    
    if (byte == BC_LESS) {
        LessOperation * less = new LessOperation();
        less->readArguments(this);
        return less;
    }
    
    if (byte == BC_EQ_LESS) {
        EqualsLessOperation * eqLess = new EqualsLessOperation();
        eqLess->readArguments(this);
        return eqLess;
    }
    
    if (byte == BC_PLUS) {
        PlusOperation * plus = new PlusOperation();
        plus->readArguments(this);
        return plus;
    }

    if (byte == BC_MINUS) {
        MinusOperation * minus = new MinusOperation();
        minus->readArguments(this);
        return minus;
    }

    if (byte == BC_MULTIPLY) {
        MultiplyOperation * mult = new MultiplyOperation();
        mult->readArguments(this);
        return mult;
    }
    
    if (byte == BC_DIVIDE) {
        DivideOperation * divide = new DivideOperation();
        divide->readArguments(this);
        return divide;
    }
    
    if (byte == BC_AND) {
        AndOperation * a = new AndOperation();
        a->readArguments(this);
        return a;
    }

    if (byte == BC_OR) {
        OrOperation * o = new OrOperation();
        o->readArguments(this);
        return o;
    }
    
    if (byte == BC_PRINT) {
        PrintFunction * prt = new PrintFunction();
        prt->readArguments(this);
        return prt;
    }
    
    if (byte == BC_SKIP) {
        VmMoveBuffer * move = new VmMoveBuffer();
        move->setDirection(FORWARD);
        move->readArguments(this);
        return move;
    }

    if (byte == BC_SKIP_IF_FALSE) {
        VmMoveBuffer * move = new VmSkipIfFalse();
        move->setDirection(FORWARD);
        move->readArguments(this);
        return move;
    }
    
    if (byte == BC_REWIND) {
        VmMoveBuffer * move = new VmMoveBuffer();
        move->setDirection(BACK);
        move->readArguments(this);
        return move;
    }
    
    if (byte == BC_RETURN) {
        VmReturn * ret = new VmReturn();
        ret->readArguments(this);
        return ret;
    }
    
    if (byte == BC_OPEN) {
        VmFile * file = new VmFile();
        file->readArguments(this);
        return file;
    }

    if (byte == BC_WRITE) {
        WriteFunction * write = new WriteFunction();
        write->readArguments(this);
        return write;
    }

    if (byte == BC_READ) {
        ReadFunction * read = new ReadFunction();
        read->readArguments(this);
        return read;
    }
    
    if (byte == BC_CLOSURE) {
        int bcLength = getShortInt();
        
        BYTE * bytecode = new BYTE[bcLength];
        for (int i = 0; i < bcLength; i++)
            bytecode[i] = getByte();
        
        VmClosure * closure = new VmClosure(bcLength, bytecode, environment);
        
        return closure;
    }
    
    if (byte == BC_FUNC_DEF) {
        int identifier = getShortInt();
        int numberOfArgs = getShortInt();
        int * args = new int[numberOfArgs];
        for (int i = 0; i < numberOfArgs; i++)
            args[i] = getShortInt();
        int bcLength = getShortInt();

        BYTE * bytecode = new BYTE[bcLength];
        for (int i = 0; i < bcLength; i++)
            bytecode[i] = getByte();
        
        UserFunction * func = new UserFunction(numberOfArgs, args, bcLength, bytecode, environment);
        
        environment->setUserFunction(identifier, func);
        
        return VmVoid::VOID();
    }
    
    if (byte == BC_FUNC_CALL) {
        int identifier = getShortInt();

        VmObject * func = environment->getUserFunction(identifier);
        func->readArguments(this);
        return func;
    }
    
    return NULL;
}

void Reader::rewind(int steps) {
    bufferPos = (bufferPos + (bufferSize - steps)) % bufferSize;
    bufferRewinded += steps;
}

void Reader::skip(int steps) {
    for (int i = 0; i < steps; i++) {
        getByte();
    }
}

BYTE Reader::getByte() {
    BYTE b;
    
    if (bufferRewinded > 0) {
        bufferRewinded--;
        b = buffer[bufferPos];
    } else {
        b = dataSource->getByte();
        buffer[bufferPos] = b;
    }
    
//cout << "readbyte " << (int)b << endl;
    bufferPos = (bufferPos + 1) % bufferSize;
    return b;
}

int Reader::getShortInt() {
    BYTE * b = new BYTE[2];
    
    b[0] = getByte();
    b[1] = getByte();

    int i = toInt(2, b);
    
    delete b;
    
    return i;
}

/******************
 
 FileDataSource
 
 ******************/

FileDataSource::FileDataSource(ifstream * input) {
    this->input = input;
}

BYTE FileDataSource::getByte() {
    char c;
    (*input).get(c);
    return c;
}

bool FileDataSource::isEOF() {
    return input->eof();
}

int toInt(int length, unsigned char * bytes) {
    int number = bytes[length - 1];
    
    for (int i = 1; i < length; i++) {
        int idx = length - 1 - i;
        int shift = 8 * i;
        number = ((bytes[idx] << shift) | number);
    }
    
    return number;
}

/********************
 
 BytecodeDataSource
 
 ********************/

BytecodeDataSource::BytecodeDataSource(BYTE * bytecode, int length) {
    this->bytecode = bytecode;
    this->length = length;
    position = -1;
}

BYTE BytecodeDataSource::getByte() {
    ++position;
    
    if (isEOF())
        return 0;
    
    return bytecode[position];
}

bool BytecodeDataSource::isEOF() {
    return position >= length;
}
