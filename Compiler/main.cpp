//
//  main.cpp
//  Compiler
//
//  Created by Štěpánek Jindřich on 02/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>

#include <map>
#include <vector>

using namespace std;

#define BYTECODE vector<short>
#define NUMBER_LENGTH 2

enum BYTE {
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
    
    FLAG_END = 999
};

bool isNumber(string str);
bool isBoolean(string str);
double toNumber(string str);
bool validIdentifier(string str);
string toString(int i);
unsigned char * toBytes(int length, int number);

int variableCode(string var);
int functionCode(string f);
bool isFunction(string f);
int numberOfArguments(string f);
void setNumberOfArguments(string f, int i);

BYTECODE * append(BYTECODE * v1, BYTECODE * v2);
BYTECODE * compile(string s, ifstream & in);

string readWord(ifstream & in);
string readString(string str, ifstream & in);
void rewind();

void printBuffer();
void printVector(BYTECODE * v);

BYTECODE * compileBlock(ifstream & in) {
    BYTECODE * bc = new BYTECODE;
    
    string s;
    while (true) {
        s = readWord(in);
        if (s == ")") {
            return bc;
        }
        
        BYTECODE * bc2 = compile(s, in);
        bc = append(bc, bc2);
    }
    
    return bc;
}

BYTECODE * compileLoop(ifstream & in) {
    string s = readWord(in);
    if (s != "(") {
        cout << "loop has to be a block" << endl;
        exit(1);
    }
    
    BYTECODE * bc = new BYTECODE;
    bc->push_back(BC_STACK_MARK);
    BYTECODE * block = compileBlock(in);
    bc = append(bc, block);
    bc->push_back(BC_STACK_MARK_RETURN);
    
    int toRewind = bc->size() + 3;
    unsigned char * toRewindBytes = toBytes(2, toRewind);
    
    bc->push_back(BC_REWIND);
    bc->push_back(toRewindBytes[0]);
    bc->push_back(toRewindBytes[1]);
    
    int loopLength = toRewind;
    
    for (int i = 0; i < loopLength; i++) {
        if (bc->at(i) == FLAG_END) {
            int toSkip = loopLength - i - 3;
            unsigned char * toSkipBytes = toBytes(2, toSkip);
            
            bc->at(i) = BC_SKIP;
            bc->at(i+1) = toSkipBytes[0];
            bc->at(i+2) = toSkipBytes[1];
        }
    }
 
    return bc;
}

BYTECODE * compileList(ifstream & in, int & numberOfItemsReturn) {
    BYTECODE * bc = new BYTECODE;
    
    int items = 0;
    string s;
    while (true) {
        s = readWord(in);
        if (s == "]") {
            numberOfItemsReturn = items;
            return bc;
        }
        
        BYTECODE * bc2 = compile(s, in);
        bc = append(bc, bc2);
        items++;
    }
    
    return bc;
}


BYTECODE * compile(string s, ifstream & in) {
    BYTECODE * bc = new BYTECODE;
    
    if (isNumber(s)) {
        bc->push_back(BC_NUMBER);
        int number = atoi(s.c_str());
        unsigned char * bytes = toBytes(2, number);
        
        bc->push_back(bytes[0]);
        bc->push_back(bytes[1]);
        return bc;
    }
    
    if (s[0] == '"') {
        string str = readString(s, in);
        
        bc->push_back(BC_STRING);
        unsigned char * bytes = toBytes(2, str.length());
        bc->push_back(bytes[0]);
        bc->push_back(bytes[1]);

        for (int i = 0; i < str.length(); i++) {
            bc->push_back(str[i]);
        }
        
        return bc;
    }
    
    if (s == "yes") {
        bc->push_back(BC_YES);
        return bc;
    }
    
    if (s == "no") {
        bc->push_back(BC_NO);
        return bc;
    }
    
    if (s == "void") {
        bc->push_back(BC_VOID);
        return bc;
    }
    
    if (s == "==") {
        BYTECODE * bc1 = compile(readWord(in), in);
        BYTECODE * bc2 = compile(readWord(in), in);
        bc = append(bc1, bc2);
        
        bc->push_back(BC_EQ);
        return bc;
    }

    if (s == ">=") {
        BYTECODE * bc1 = compile(readWord(in), in);
        BYTECODE * bc2 = compile(readWord(in), in);
        bc = append(bc1, bc2);
        
        bc->push_back(BC_EQ_MORE);
        return bc;
    }
    
    if (s == ">") {
        BYTECODE * bc1 = compile(readWord(in), in);
        BYTECODE * bc2 = compile(readWord(in), in);
        bc = append(bc1, bc2);
        
        bc->push_back(BC_MORE);
        return bc;
    }
    
    if (s == "<") {
        BYTECODE * bc1 = compile(readWord(in), in);
        BYTECODE * bc2 = compile(readWord(in), in);
        bc = append(bc1, bc2);
        
        bc->push_back(BC_LESS);
        return bc;
    }

    if (s == "<=") {
        BYTECODE * bc1 = compile(readWord(in), in);
        BYTECODE * bc2 = compile(readWord(in), in);
        bc = append(bc1, bc2);
        
        bc->push_back(BC_EQ_LESS);
        return bc;
    }

    if (s == "=") {
        string id = readWord(in);
        if (!validIdentifier(id))
            exit(1);
        int idCode = variableCode(id);
        unsigned char * idBytes = toBytes(2, idCode);
        
        BYTECODE * val = compile(readWord(in), in);
        bc = append(bc, val);
        bc->push_back(BC_ASSIGN);
        bc->push_back(idBytes[0]);
        bc->push_back(idBytes[1]);
        return bc;
    }
    
    if (s == "+") {
        BYTECODE * bc1 = compile(readWord(in), in);
        BYTECODE * bc2 = compile(readWord(in), in);
        bc = append(bc1, bc2);
        bc->push_back(BC_PLUS);
        return bc;
    }
    
    if (s == "-") {
        BYTECODE * bc1 = compile(readWord(in), in);
        BYTECODE * bc2 = compile(readWord(in), in);
        bc = append(bc1, bc2);
        bc->push_back(BC_MINUS);
        return bc;
    }
    
    if (s == "/") {
        BYTECODE * bc1 = compile(readWord(in), in);
        BYTECODE * bc2 = compile(readWord(in), in);
        bc = append(bc1, bc2);
        bc->push_back(BC_DIVIDE);
        return bc;
    }
    
    if (s == "*") {
        BYTECODE * bc1 = compile(readWord(in), in);
        BYTECODE * bc2 = compile(readWord(in), in);
        bc = append(bc1, bc2);
        bc->push_back(BC_MULTIPLY);
        return bc;
    }
    
    if (s == "and") {
        BYTECODE * bc1 = compile(readWord(in), in);
        BYTECODE * bc2 = compile(readWord(in), in);
        bc = append(bc1, bc2);
        bc->push_back(BC_AND);
        return bc;
    }

    if (s == "or") {
        BYTECODE * bc1 = compile(readWord(in), in);
        BYTECODE * bc2 = compile(readWord(in), in);
        bc = append(bc1, bc2);
        bc->push_back(BC_OR);
        return bc;
    }
    
    if (s == "print") {
        BYTECODE * bc1 = compile(readWord(in), in);
        bc = bc1;
        bc->push_back(BC_PRINT);
        return bc;
    }
    
    if (s == "return") {
        bc = compile(readWord(in), in);
        bc->push_back(BC_RETURN);
        return bc;
    }

    if (s == "(") {
        return compileBlock(in);
    }
    
    if (s == "if") {
        BYTECODE * cond = compile(readWord(in), in);
        BYTECODE * ifBlock = compile(readWord(in), in);
        
        int elseBlockLength = 0;
        BYTECODE * elseBlock = NULL;
        
        if (readWord(in) == "else") {
            elseBlock = compile(readWord(in), in);
            elseBlockLength = elseBlock->size();
        } else {
            rewind();
        }
        
        if (elseBlockLength > 0) {
            ifBlock->push_back(BC_SKIP);
            unsigned char * lengthBytes = toBytes(2, elseBlockLength);
            ifBlock->push_back(lengthBytes[0]);
            ifBlock->push_back(lengthBytes[1]);
        }
        
        int ifBlockLength = ifBlock->size();
        
        bc = append(bc, cond);
        bc->push_back(BC_SKIP_IF_FALSE);
        
        unsigned char * lengthBytes = toBytes(2, ifBlockLength);
        bc->push_back(lengthBytes[0]);
        bc->push_back(lengthBytes[1]);
        
        bc = append(bc, ifBlock);
        if (elseBlock)
            bc = append(bc, elseBlock);
        return bc;
    }
    
    if (s == "[") {
        int listLength;
        BYTECODE * list = compileList(in, listLength);
        unsigned char * lengthBytes = toBytes(2, listLength);

        bc = list;
        bc->push_back(BC_LIST);
        bc->push_back(lengthBytes[0]);
        bc->push_back(lengthBytes[1]);
        return bc;
    }
    
    if (s == "at") {
        BYTECODE * obj = compile(readWord(in), in);
        string idxStr = readWord(in);
        
        if (!isNumber(idxStr)) {
            cout << "at: " << idxStr << ": index has to be a number" << endl;
            exit(1);
        }

        int idx = atoi(idxStr.c_str());
        unsigned char * idxBytes = toBytes(2, idx);
        
        bc = obj;
        bc->push_back(BC_AT);
        bc->push_back(idxBytes[0]);
        bc->push_back(idxBytes[1]);
        return bc;
    }
    
    
    if (s == "func") {
        string name = readWord(in);
        if (!validIdentifier(name))
            exit(1);
        
        int code = functionCode(name);
        
        string arg;
        
        vector<int> args;
        
        while(true) {
            arg = readWord(in);
            if (arg == "(")
                break;
            
            if (!validIdentifier(arg)) {
                cout << "wrong name to argument " << endl;
                exit(1);
            }
            
            int argCode = variableCode(arg);
            args.push_back(argCode);
        }
        
        setNumberOfArguments(name, args.size());

        BYTECODE * block = compileBlock(in);
        block->insert(block->begin(), BC_STACK_MARK);
        block->push_back(BC_STACK_MARK_RETURN);
        
        unsigned char * codeBytes = toBytes(2, code);
        unsigned char * blockSizeBytes = toBytes(2, block->size());
        unsigned char * numberOfArgsBytes = toBytes(2, args.size());

        bc->push_back(BC_FUNC_DEF);
        bc->push_back(codeBytes[0]);
        bc->push_back(codeBytes[1]);
        bc->push_back(numberOfArgsBytes[0]);
        bc->push_back(numberOfArgsBytes[1]);
        for (int i = 0; i < args.size(); i++) {
            unsigned char * argBytes = toBytes(2, args.at(i));
            bc->push_back(argBytes[0]);
            bc->push_back(argBytes[1]);
        }
        bc->push_back(blockSizeBytes[0]);
        bc->push_back(blockSizeBytes[1]);
        bc = append(bc, block);
        return bc;
    }
    
    if (s == "loop") {
        return compileLoop(in);
    }
    
    if (s == "end") {
        bc->push_back(FLAG_END);
        bc->push_back(0);
        bc->push_back(0);
        return bc;
    }
    
    if (isFunction(s)) {
        int code = functionCode(s);
        unsigned char * codeBytes = toBytes(2, code);
        
        for (int i = 0; i < numberOfArguments(s); i++) {
            BYTECODE * bcArg = compile(readWord(in), in);
            
            if (!bcArg) {
                cout << s << ": wrong number of args" << endl;
                exit(1);
            }
            
            bc = append(bc, bcArg);
        }
        
        bc->push_back(BC_FUNC_CALL);
        bc->push_back(codeBytes[0]);
        bc->push_back(codeBytes[1]);
        
        return bc;
    }
    
    if (validIdentifier(s)) {
        bc->push_back(BC_LOAD);
        unsigned char * bytes = toBytes(2, variableCode(s));
        bc->push_back(bytes[0]);
        bc->push_back(bytes[1]);
        return bc;
    }
    
    return NULL;
}


/********************
 
 
 MAIN
 
 
 ***********************/


int main(int argc, char ** argv) {
    string fileName = "/Users/stepanek/School/RUN/Compiler/code";
    if (argc >= 2) {
        fileName = argv[1];
    }
    
    ifstream in(fileName);
    
    ofstream out("/Users/stepanek/School/RUN/Compiler/BC.out", ios::out|ios::binary);
    
    if (!in.is_open()) {
        cout << "blbej file" << endl;
        return 1;
    }
    
    BYTECODE * v = NULL;
    
    while (true) {
        if (in.eof()) {
            break;
        }
        
        
        BYTECODE * v2 = compile(readWord(in), in);
        //printVector(v2);
        
        v = append(v, v2);
        //printVector(v);
    }
    
    
    printVector(v);
    
    char buffer[1024];
    for (int i = 0; i < v->size(); i++) {
        buffer[i] = v->at(i);
    }
    
    out.write(buffer, v->size());
    
    return 0;
}



/********************
 
 
 SUPPORT FUNCTIONS
 
 
 ***********************/

bool isNumber(string str) {
    if (str.length() == 0)
        return false;
    
    int i = 0;
    if (str[i] == '-' && str.length() > 1)
        i++;
    
    if (str[i] == '.')
        return false;
    
    int dots = 0;
    
    for (; i < str.length(); i++) {
        char c = str[i];
        if (c == '.') {
            if (++dots > 1 || i == str.length() - 1)
                return false;
            continue;
        }
        
        if (c < '0' || c > '9')
            return false;
    }
    
    return true;
}

bool isBoolean(string str) {
    return str == "yes" || str == "no";
}

double toNumber(string str) {
    return stod(str);
}

string toString(int i) {
    stringstream ss;
    ss << i;
    return ss.str();
}


bool validIdentifier(string str) {
    if (str.length() == 0)
        return false;
    
    
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        
        if (c >= 'a' && c <= 'z')
            continue;
        
        if (c >= 'A' && c <= 'Z')
            continue;
        
        if (i > 0 && (c == '?' || c == '!'))
            continue;
        
        if (i > 0 && (c >= '0' && c <= '9'))
            continue;
        
        return false;
    }
    
    return true;
}


void printVector(BYTECODE * v) {
    for (int i = 0; i < v->size(); i++) {
        cout << i << ": " << v->at(i) << endl;
    }
}

BYTECODE * append(BYTECODE * bc1, BYTECODE * bc2) {
    BYTECODE * vc = new BYTECODE;
    
    if (!bc1)
        return bc2;
    if (!bc2)
        return bc1;
    
    for (int i = 0; i < bc1->size(); i++) {
        vc->push_back(bc1->at(i));
    }
    
    for (int i = 0; i < bc2->size(); i++) {
        vc->push_back(bc2->at(i));
    }
    
    return vc;
}

string readString(string str, ifstream & in) {
    while (str[str.length() - 1] != str[0] || str.length() == 1) {
        str += " ";
        str += readWord(in);
    }
    
    return str.substr(1, str.length() - 2);
}

unsigned char * toBytes(int length, int number) {
    unsigned char * bytes = new unsigned char[NUMBER_LENGTH];
    // for (int i = 0; i < 4; i++)
    //      arrayOfByte[3 - i] = (paramInt >> (i * 8));
    
    for (int i = 0; i < length; i++)
        bytes[length - 1 - i] = (number >> (i * 8));
    // bytes[1] = number;
    // bytes[0] = number >> 8;
    //     cout << (int)bytes[0] << ", " << (int)bytes[1] << endl;
    
    return bytes;
}



/*********************
 *
 *
 *  BUFFER
 *
 *
 *********************/

#define BUFFER_SIZE 4
string buffer[BUFFER_SIZE];
int bufferPos = 0;

void rewind() {
    bufferPos = (bufferPos + (BUFFER_SIZE - 1)) % BUFFER_SIZE;
}

void printBuffer() {
    cout << bufferPos << ": ";
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (i == bufferPos) {
            cout << "_" << buffer[i] << "_ ";
        } else if (buffer[i] == "") {
            cout << ". ";
        } else {
            cout << buffer[i] << " ";
        }
    }
    cout << endl;
}

string readWord(ifstream & in) {
    if (buffer[bufferPos] != "") {
        string s = buffer[bufferPos];
        bufferPos = (bufferPos + 1) % BUFFER_SIZE;
        return s;
    }
    
    string s;
    in >> s;
    
    buffer[bufferPos] = s;
    bufferPos = (bufferPos + 1) % BUFFER_SIZE;
    buffer[bufferPos] = "";
    
    return s;
}

/**********************
 
 
 VARIABLES
 
 
 *********************/

map<string, int> VARIABLES;

int variableCode(string var) {
    int i = VARIABLES[var];
    if (i != 0)
        return i;
    
    i = VARIABLES.size();
    VARIABLES[var] = i;
    return i;
}

map<string, int> FUNCTIONS;
map<string, int> NUMBERS_OF_ARGUMENTS;

int functionCode(string f) {
    int i = FUNCTIONS[f];
    if (i != 0)
        return i;
    
    i = FUNCTIONS.size();
    FUNCTIONS[f] = i;
    return i;
}

bool isFunction(string f) {
    return FUNCTIONS[f] != 0;
}

int numberOfArguments(string f) {
    return NUMBERS_OF_ARGUMENTS[f];
}

void setNumberOfArguments(string f, int i) {
    NUMBERS_OF_ARGUMENTS[f] = i;
}