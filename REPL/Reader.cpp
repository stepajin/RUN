//
//  Reader.cpp
//  REPL
//
//  Created by Štěpánek Jindřich on 26/10/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include "main.h"

#include "Reader.h"
#include "Functions.h"
#include "ArithmeticOperations.h"
#include "Blocks.h"
#include "Enviroment.h"
#include "AssignFunctions.h"
#include "LogicOperations.h"

using namespace std;

bool allDigits(string str);
bool isString(string str);
bool isBoolean(string str);
string readSymbol();
int toNumber(string symbol);

LangFunction * getFunctionIfIndetifier(string s, Enviroment * enviroment) {
    if (s == "+")
        return new PlusOperation();
    
    if (s == "-")
        return new MinusOperation();
        
    if (s == "*")
        return new MultiplyOperation();
    
    if (s == "/")
        return new DivideOperation();
    
    if (s == "==")
        return new EqualsOperation();
    
    if (s == ">=")
        return new EqualsMoreOperation();

    if (s == ">")
        return new MoreOperation();

    if (s == "<=")
        return new EqualsLessOperation();

    if (s == "<")
        return new LessOperation();
    
    if (s == "if")
        return new IfFunction();
    
    if (s == "else")
        return new ElseFunction();
    
    if (s == "=")
        return new AssignFunction();
    
    if (s == "loop")
        return new LoopFunction();
    
    if (s == "at")
        return new AtFunction();
    
    if (s == "+=")
        return new AssignPlusFunction();

    if (s == "-=")
        return new AssignMinusFunction();
    
    if (s == "*=")
        return new AssignMultiplyFunction();
    
    if (s == "/=")
        return new AssignDivideFunction();
    
    if (s == "and")
        return new AndOperation();

    if (s == "or")
        return new OrOperation();

    
    LangObject * userFunction = enviroment->get(s);
    if (userFunction && userFunction->getTag() == TAG_FUNCTION) {
        return (LangFunction *)userFunction;
    }
    
    
    return NULL;
}

LangList * readList(Reader * reader) {
    LangList * list = new LangList();
    
    while (true) {
        LangObject * obj = reader->getObject();
        
        if (obj->getTag() == TAG_STRING) {
            LangString * str = (LangString *) obj;
            if (str->getValue() == "]") {
                delete obj;
                break;
            }
        }
        
        list->push(obj);
    }
    
    return list;
}

/*****************
 
    Reader
 
 ****************/

Reader::Reader(Enviroment * enviroment) {
    this->enviroment = enviroment;
}

string Reader::getIdentifier() {
    return readSymbol();
}

LangObject * Reader::getBlock() {
    string s = readSymbol();
    if (s != "(")
        return NULL;
    
    int openParenthesis = 1;
    
    string block = "";
    while (true) {
        if (isEOF()) {
            return NULL;
        }
        
        char c = readChar();
        if (c == '(')
            openParenthesis++;
        
        if (c == ')' && --openParenthesis == 0)
            return new LangBlock(block);
        
        block += c;
    }
    
    return NULL;
}

LangObject * Reader::getObject() {
    string s = readSymbol();
    
    if (isEOF())
        return LangObject::getEOF();

    if (s == "end")
        return LangObject::getEND();
    
    if (allDigits(s)) {
        int i = toNumber(s);
        return new LangInteger(i);
    }
    
    if (s == "func") {
        string name = readSymbol();
        LangFunction * function = new LangFunction(name);
        
        LangObject * b = getBlock();
        if (!b || b->getTag() != TAG_BLOCK) {
            error("wrong function block");
            return NULL;
        }
        
        LangBlock * block = (LangBlock *)b;
        function->setBlock(block);
        
        enviroment->set(name, function);
        return LangVoid::VOID();
    }
    
    if (isString(s)) {
        string str = s.substr(1, s.length()-2);
        return new LangString(str);
    }
    
    if (isBoolean(s)) {
        return s == "yes" ? LangBoolean::YES() : LangBoolean::NO();
    }
    
    if (s == "[") {
        return readList(this);
    }
    
    LangFunction * func = getFunctionIfIndetifier(s, enviroment);
    if (func) {
        bool argsOk = func->readArgs(this);
        if (!argsOk) {
            error("wrong arguments");
            return NULL;
        }
        
        return func;
    }
    
    LangObject * enviromentValue = enviroment->get(s);
    if (enviromentValue) {
        return enviromentValue;
    }
    
/*    string e = "wrong command '";
    e += s;
    e += "'";
    error(e);
    return NULL;*/
    
    return new LangString(s);
}

/*****************
 
    ConsoleReader
 
*****************/

ConsoleReader::ConsoleReader(Enviroment * enviroment) : Reader(enviroment) {}

string ConsoleReader::readSymbol() {
    string s;
    cin >> s;
    return s;
}

char ConsoleReader::readChar() {
    return cin.get();
}


bool ConsoleReader::isEOF() {
    return false;
}

/*****************
 
    StringReader
 
 *****************/

StringReader::StringReader(string s, Enviroment * enviroment) : Reader(enviroment) {
    stream << s;
}

string StringReader::readSymbol() {
    string s;
    stream >> s;
    return s;
}

char StringReader::readChar() {
    return stream.get();
}


bool StringReader::isEOF() {
    return stream.eof();
}

/*****************
 
    Support Functions
 
 *****************/

bool allDigits(string str) {
    if (str.length() == 0)
        return false;
    
    int i = 0;
    if (str[i] == '-' && str.length() > 1)
        i++;
    
    for (; i < str.length(); i++) {
        char c = str[i];
        
        if (c < '0' || c > '9')
            return false;
    }
    
    return true;
}

bool isString(string str) {
    if (str.length() == 0)
        return false;
    
    return str[0] == '"' && str[str.length()-1] == '"';
}

bool isBoolean(string str) {
    return str == "yes" || str == "no";
}

int toNumber(string symbol) {
    return atoi(symbol.c_str());
}

