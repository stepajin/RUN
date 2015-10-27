//
//  lang.h
//  REPL
//
//  Created by Štěpánek Jindřich on 26/10/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef REPL_LangObjects_h
#define REPL_LangObjects_h

#include <iostream>
#include <vector>

using namespace std;

class Enviroment;

enum LangObjectTag {
    TAG_INTEGER,
    TAG_STRING,
    TAG_BOOLEAN,
    TAG_FUNCTION,
    TAG_EOF,
    TAG_BLOCK,
    TAG_VOID,
    TAG_LIST,
    TAG_END,
};

class LangObject {
    LangObjectTag tag;
    
protected:
    LangObject(LangObjectTag tag);
    
    static LangObject * EOF_INSTANCE;
    static LangObject * END_INSTANCE;

public:
    LangObjectTag getTag();
    
    static LangObject * getEOF();
    static LangObject * getEND();

    virtual LangObject * eval(Enviroment * enviroment);
    virtual string toString();
};

class LangVoid : public LangObject {
private:
    static LangVoid * INSTANCE;
    LangVoid();

public:
    static LangVoid * VOID();
    virtual string toString();
};

class LangInteger : public LangObject {
    int value;
public:
    LangInteger(int value);
    
    void setValue(int value);
    int getValue();
    
    virtual string toString();
};

class LangString : public LangObject {
    string value;
public:
    LangString(string value);
    
    void setValue(string value);
    string getValue();
    
    virtual string toString();
};

class LangBoolean : public LangObject {
    bool value;
    
    static LangBoolean * YES_INSTANCE;
    static LangBoolean * NO_INSTANCE;
    LangBoolean(bool value);
    
public:
    virtual string toString();

    static LangBoolean * YES();
    static LangBoolean * NO();
    static LangBoolean * INSTANCE(bool value);
};

class LangList : public LangObject {
    vector<LangObject *> * list;
public:
    LangList();
    
    LangObject * at(int idx);
    unsigned long size();
    void push(LangObject * obj);
    
    virtual string toString();
};

#endif