//
//  LangFunctions.cpp
//  REPL
//
//  Created by Štěpánek Jindřich on 26/10/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include "Functions.h"
#include "Enviroment.h"
#include "main.h"

LangFunction::LangFunction() : LangObject(TAG_FUNCTION) {
    argIdentifiers = new vector<LangIdentifier *>;
    //args = new vector<LangObject *>;
}

LangFunction::LangFunction(string name) : LangFunction() {
    this->name = name;
}

bool LangFunction::readArgs(Reader * reader) {
    if (argIdentifiers->size() == 0)
        return true;
    
    LangObject * obj = reader->getObject();
    
    if (!obj || obj->getTag() != TAG_LIST) {
        stringstream ss;
        ss << "function " << name << " requires arguments list";
        error(ss.str());
        return false;
    }
    
    args = (LangList *) obj;
    
    if (args->size() != argIdentifiers->size()) {
        stringstream ss;
        ss << "function " << name << " arguments: given " << args->size();
        ss << " expected " << argIdentifiers->size();
        error(ss.str());
        args = NULL;
        return false;
    }
    
    return true;
}

void LangFunction::setBlock(LangBlock * block) {
    this->block = block;
}

void LangFunction::addArgIdentifier(LangIdentifier * identifier) {
    argIdentifiers->push_back(identifier);
}

string LangFunction::getName() {
    return name;
}

bool LangFunction::hasBody() {
    return block != NULL;
}

void LangFunction::addArgumentsToEnviroment(Enviroment * enviroment) {
    if (!args)
        return;
    
    for (int i = 0; i < args->size(); i++) {
        LangIdentifier * id = argIdentifiers->at(i);
        LangObject * arg = args->at(i);
        enviroment->set(id->getValue(), arg);
    }
}

LangObject * LangFunction::eval(Enviroment * enviroment) {
    Enviroment * newEnviroment = new Enviroment(enviroment);
    
    addArgumentsToEnviroment(newEnviroment);

    if (!block) {
        error("function has no body");
        return NULL;
    }
    
    LangObject * blockEval = block->eval(newEnviroment);

    return blockEval;
}

/***************
 
    If
 
 **************/

IfFunction::IfFunction() : LangFunction("if") {
    success = false;
}

bool IfFunction::succeeded() {
    return success;
}

bool IfFunction::readArgs(Reader * reader) {
    LangObject * obj = reader->getBlock();
    if (obj == NULL)
        return false;
    
    if (obj->getTag() == TAG_BLOCK)
        condBlock = (LangBlock *) obj;
    
    obj = reader->getBlock();
    if (obj == NULL || obj->getTag() != TAG_BLOCK)
        return false;
    
    block = (LangBlock *) obj;
    
    return true;
}

LangObject * IfFunction::eval(Enviroment * enviroment) {
    LangObject * condRes = condBlock->eval(enviroment);
    
    if (condRes == NULL || condRes->getTag() != TAG_BOOLEAN) {
        error("condition block result is not boolean value");
        return NULL;
    }
    
    if (condRes == LangBoolean::YES()) {
        success = true;
        LangObject * obj = block->eval(enviroment);
        if (obj->getTag() == TAG_END)
            return obj;
        
        return obj;
    } else {
        success = false;
        return LangVoid::VOID();
    }
}

/***************
 
    Else
 
 **************/


ElseFunction::ElseFunction() : LangFunction("else") {
    ifFunction = NULL;
}

void ElseFunction::setIfFunction(IfFunction * ifFunction) {
    this->ifFunction = ifFunction;
}

bool ElseFunction::readArgs(Reader * reader) {
    LangObject * obj = reader->getBlock();
    if (obj == NULL)
        return false;
    
    if (obj->getTag() == TAG_BLOCK)
        block = (LangBlock *) obj;
    
    return true;
}

LangObject * ElseFunction::eval(Enviroment * enviroment) {
    if (ifFunction == NULL) {
        error("else with no previous if");
        return LangVoid::VOID();
    }
    
    if (!ifFunction->succeeded()) {
        LangObject * obj = block->eval(enviroment);
        if (obj->getTag() == TAG_END)
            return obj;
        
        return obj;
    }
    
    return LangVoid::VOID();
}

/***************
 
    Loop
 
 **************/

LoopFunction::LoopFunction() : LangFunction("loop") {
}

LangObject * LoopFunction::eval(Enviroment * enviroment) {
    for (int i = 0; ; i++) {
        LangObject * obj = block->eval(enviroment);
        
        if (obj && obj->getTag() == TAG_END) {
            break;
        }
    }
    
    return LangVoid::VOID();
}

bool LoopFunction::readArgs(Reader * reader) {
    LangObject * obj = reader->getBlock();
    if (obj == NULL)
        return false;
    
    if (obj->getTag() == TAG_BLOCK)
        block = (LangBlock *) obj;
    
    return true;
}

/***************
 
    At
 
 **************/

AtFunction::AtFunction() : LangFunction("at") {}

LangObject * AtFunction::eval(Enviroment * enviroment) {
    stringstream outOfBounds;
    outOfBounds << "index out of bounds: " << idx;

    LangObject * obj = object;
    
    if (object->getTag() == TAG_IDENTIFIER) {
        LangIdentifier * id = (LangIdentifier *) object;
        obj = enviroment->get(id->getValue());
    }
    
    if (!obj)
        return error("at: not valid object");
    
    if (obj->getTag() == TAG_STRING) {
        LangString * str = (LangString *) obj;
        
        if (idx >= str->getValue().length())
            return error(outOfBounds.str());
    
        string s = "";
        s += str->getValue()[idx];
        return new LangString(s);
    }
    
    if (obj->getTag() == TAG_LIST) {
        LangList * list = (LangList *)obj;
        
        if (idx >= list->size())
            return error(outOfBounds.str());

        return list->at(idx);
    }
    
    return error("at: not valid object");
}

bool AtFunction::readArgs(Reader * reader) {
    LangObject * obj = reader->getObject();
    
    if (obj->getTag() != TAG_IDENTIFIER && obj->getTag() != TAG_STRING && obj->getTag() != TAG_LIST) {
        error("at: first argument must be a list, identifier or string");
        return false;
    }
    
    this->object = obj;
    
    obj = reader->getObject();
    
    if (obj == NULL || obj->getTag() != TAG_INTEGER) {
        error("index is not a number");
        return false;
    }

    idx = ((LangInteger *) obj)->getValue();
    return true;
}

