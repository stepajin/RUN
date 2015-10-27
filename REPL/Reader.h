//
//  Reader.h
//  REPL
//
//  Created by Štěpánek Jindřich on 26/10/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef REPL_Reader_h
#define REPL_Reader_h

#include "Objects.h"
#include <sstream> 

class Reader {
protected:
    Enviroment * enviroment;
    
    Reader(Enviroment * enviroment);
    virtual string readSymbol() = 0;
    virtual char readChar() = 0;
    
    virtual bool isEOF() = 0;
    
public:
    LangObject * getObject();
    LangObject * getBlock();
    string getIdentifier();
};

class ConsoleReader : public Reader {
    virtual string readSymbol();
    virtual char readChar();
    virtual bool isEOF();

public:
    ConsoleReader(Enviroment * enviroment);
};

class StringReader : public Reader {
    stringstream stream;
    
    virtual string readSymbol();
    virtual char readChar();
    virtual bool isEOF();
    
public:
    StringReader(string s, Enviroment * enviroment);
};


#endif
