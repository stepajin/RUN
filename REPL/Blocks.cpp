//
//  LangBlocks.cpp
//  REPL
//
//  Created by Štěpánek Jindřich on 26/10/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include "main.h"
#include "Blocks.h"

LangBlock::LangBlock(string block) : LangObject(TAG_BLOCK) {
    this->block = block;
    this->printable = true;
}

string LangBlock::getBlock() {
    return block;
}

void LangBlock::setPrintable(bool printable) {
    this->printable = printable;
}

LangObject * LangBlock::eval(Enviroment * enviroment) {
    Enviroment * newEnviroment = new Enviroment(enviroment);

    StringReader * reader = new StringReader(block, newEnviroment);
    LangObject * obj = repl(reader, newEnviroment);

    return obj;
}

string LangBlock::toString() {
    string s = "block ";
    s += block;
    return s;
}

