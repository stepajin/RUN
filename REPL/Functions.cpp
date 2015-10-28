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
}

LangFunction::LangFunction(string name) : LangObject(TAG_FUNCTION) {
    this->name = name;
}

bool LangFunction::readArgs(Reader * reader) {
    // TODO: args
    return true;
}

void LangFunction::setBlock(LangBlock * block) {
    this->block = block;
}

string LangFunction::getName() {
    return name;
}

bool LangFunction::hasBody() {
    return block != NULL;
}

LangObject * LangFunction::eval(Enviroment * enviroment) {
    if (!block) {
        error("function has no body");
        return NULL;
    }
    
    LangObject * blockEval = block->eval(enviroment);
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
        
        if (obj->getTag() == TAG_END) {
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
    LangObject * obj = enviroment->get(listKey);
    
    if (!obj) {
        stringstream ss;
        ss << "list " << listKey << " not found";
        error(ss.str());
        return NULL;
    }
    
    if (obj->getTag() != TAG_LIST) {
        stringstream ss;
        ss << listKey << " is not a list";
        error(ss.str());
        return NULL;
    }

    LangList * list = (LangList *)obj;
    
    if (idx >= list->size()) {
        stringstream ss;
        ss  << "index out of bounds: " << idx;
        error(ss.str());
        return NULL;
    }
    
    return list->at(idx);
}

bool AtFunction::readArgs(Reader * reader) {
    listKey = reader->getIdentifier();
    LangObject * obj = reader->getObject();
    
    if (obj == NULL || obj->getTag() != TAG_INTEGER) {
        error("index is not a number");
        return false;
    }

    idx = ((LangInteger *) obj)->getValue();
    return true;
}

