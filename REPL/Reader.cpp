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
int toNumber(string str);
bool validIdentifier(string str);

string readWord();

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

LangObject * Reader::readObject(string s) {
    if (isEOF())
        return LangObject::getEOF();
    
    if (s == "end")
        return LangObject::getEND();
    
    if (allDigits(s)) {
        int i = toNumber(s);
        return new LangInteger(i);
    }
    
    if (s == "func") {
        LangObject * obj = getObject();
        if (!obj || obj->getTag() != TAG_IDENTIFIER)
            return error("not valid function identifier");

        LangIdentifier * id = (LangIdentifier *)obj;
        LangFunction * function = new LangFunction(id->getValue());
        
        LangObject * b = getBlock();
        
        if (!b || b->getTag() != TAG_BLOCK)
            return error("wrong function block");
            
        LangBlock * block = (LangBlock *)b;
        function->setBlock(block);
        
        enviroment->set(id->getValue(), function);
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
        return readList();
    }
    
    if (s == "(") {
        return readBlock();
    }
    
    LangFunction * func = getFunctionIfIndetifier(s, enviroment);
    if (func) {
        bool argsOk = func->readArgs(this);
        if (!argsOk)
            return error("wrong arguments");
            
        return func;
    }
    
    if (validIdentifier(s))
        return new LangIdentifier(s);
    
    stringstream e;
    e << "wrong command '" << s << "'";
    return error(e.str());
}

LangObject * Reader::getObject() {
    string s = readWord();
    
    return readObject(s);
}


LangObject * Reader::readList() {
    LangList * list = new LangList();
    
    while (true) {
        string s = readWord();
        if (s == "]")
            break;
        
        LangObject * obj = readObject(s);
        list->push(obj);
    }
    
    return list;
}

LangObject * Reader::readBlock() {
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

/*****************
 
    Reader
 
 ****************/

Reader::Reader(Enviroment * enviroment) {
    this->enviroment = enviroment;
}

string Reader::getIdentifier() {
    return readWord();
}

LangObject * Reader::getBlock() {
    string s = readWord();
    if (s != "(")
        return NULL;
    
    return readBlock();
}

/*****************
 
    ConsoleReader
 
*****************/

ConsoleReader::ConsoleReader(Enviroment * enviroment) : Reader(enviroment) {}

string ConsoleReader::readWord() {
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

string StringReader::readWord() {
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

int toNumber(string str) {
    return atoi(str.c_str());
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
