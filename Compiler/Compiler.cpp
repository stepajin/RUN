#include <iostream>
#include <fstream>
#include <sstream>

#include <stdio.h>
#include <stdlib.h> 

#include <map>
#include <vector>

using namespace std;

#define BYTECODE vector<int>
#define NUMBER_LENGTH 2

enum BYTE {
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
    BC_FUNC_CALL = 17
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
        bc->push_back(BC_CONST);
        int number = atoi(s.c_str());
        unsigned char * bytes = toBytes(2, number);

        bc->push_back(bytes[0]);
        bc->push_back(bytes[1]);
        return bc;
     }

     if (s[0] == '"') {
        string str = readString(s, in);

        for (int i = 0; i < str.length(); i++) {
            bc->push_back(BC_CHAR);
            bc->push_back(str[i]);
        }

        bc->push_back(BC_STRING);
        bc->push_back(str.length());
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

     if (s == "==") {
        BYTECODE * bc1 = compile(readWord(in), in);
        BYTECODE * bc2 = compile(readWord(in), in);
        bc = append(bc1, bc2);
       
        bc->push_back(BC_EQ);
        return bc;
     }

     if (s == "=") {
        string id = readWord(in);
        if (!validIdentifier(id))
            exit(1);

        BYTECODE * val = compile(readWord(in), in);
        bc = append(bc, val);
        bc->push_back(/*id*/ 999);
        bc->push_back(BC_ASSIGN);
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
            ifBlock->push_back(elseBlockLength);
        }

        int ifBlockLength = ifBlock->size();

        bc = append(bc, cond);
        bc->push_back(BC_SKIP_IF_FALSE);

        bc->push_back(ifBlockLength);

        bc = append(bc, ifBlock);
        if (elseBlock)
            bc = append(bc, elseBlock);
        return bc;
     }

     if (s == "[") {
        int listLength;
        BYTECODE * list = compileList(in, listLength);
        bc = list;
        bc->push_back(BC_LIST);
        bc->push_back(listLength);
        return bc;
     }

     if (s == "func") {
        string name = readWord(in);
        if (!validIdentifier(name))
            exit(1);

        string arg;
        int argsCnt = 0;
        while(true) {
            arg = readWord(in);
            if (arg == "(")
                break;
            argsCnt++;
        }

        BYTECODE * block = compileBlock(in);
        int blockSize = block->size();

        int code = functionCode(name);
        unsigned char * codeBytes = toBytes(2, code);

        bc->push_back(BC_FUNC_DEF);
        bc->push_back(codeBytes[0]);
        bc->push_back(codeBytes[1]);

        setNumberOfArguments(s, argsCnt);

        bc->push_back(blockSize);
        bc = append(bc, block);
        return bc;
     }

     if (isFunction(s)) {
        int code = functionCode(s);
        unsigned char * codeBytes = toBytes(2, code);

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
    if (argc < 2) {
        cout << "wrong usage" << endl;
        return 1;
    }

    ifstream in(argv[1]);

    ofstream out("bin", ios::out|ios::binary);

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
    
    cout << "string " << str.substr(1, str.length() - 2) << endl;
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
    cout << "code pro " << f << endl;

    int i = FUNCTIONS[f];
    if (i != 0)
        return i;

    i = FUNCTIONS.size();
    FUNCTIONS[f] = i;
    cout << i << endl;
    return i;    
}

bool isFunction(string f) {
    cout << "isfUnc " << f << endl;
    cout << (FUNCTIONS[f] != 0) << ".." << endl;
    return FUNCTIONS[f] != 0;
}

int numberOfArguments(string f) {
    return NUMBERS_OF_ARGUMENTS[f];
}

void setNumberOfArguments(string f, int i) {
    NUMBERS_OF_ARGUMENTS[f] = i;
}




