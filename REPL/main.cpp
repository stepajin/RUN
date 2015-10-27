//
//  main.cpp
//  REPL
//
//  Created by Štěpánek Jindřich on 26/10/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include <iostream>
#include "Objects.h"
#include "Reader.h"
#include "main.h"
#include "Enviroment.h"
#include "Functions.h"

using namespace std;

void error(string message) {
    cout << "error: " << message << endl;
}

LangObject * repl(Reader * reader, Enviroment * enviroment, bool print) {
    LangObject * lastEval = NULL;
    LangObject * lastObject = NULL;
    
    while (true) {
        bool printEval = print;
        
        LangObject * obj = reader->getObject();
        
        if (!obj) {
            continue;
        }
        
        if (obj->getTag() == TAG_EOF) {
            break;
        }
        
        /* don't print result if object is function with body */
        if (obj->getTag() == TAG_FUNCTION) {
            LangFunction * func = (LangFunction *)obj;
            
            if (func->hasBody())
                printEval = false;
        }
        
        /* if-else? */
        if (obj->getTag() == TAG_FUNCTION && lastObject && lastObject->getTag() == TAG_FUNCTION) {
            LangFunction * func = (LangFunction *)obj;
            LangFunction * lastFunc = (LangFunction *)lastObject;
            
            if (func->getName() == "else" && lastFunc->getName() == "if") {
                ElseFunction * elseFunction = (ElseFunction *) func;
                IfFunction * ifFunction = (IfFunction *) lastFunc;

                elseFunction->setIfFunction(ifFunction);
            }
        }
        
        LangObject * eval = obj->eval(enviroment);

        if (eval && eval->getTag() == TAG_END) {
            return LangObject::getEND();
        }

        /*if (eval && eval->getTag() == TAG_BLOCK) {
            cout << "block??" << endl;
        } */
        
        if (eval && printEval) {
            string s = eval->toString();
            if (s != "")
                cout << s << endl;
        }
        
        lastObject = obj;
        lastEval = eval;
    }
    
    return lastEval;
}

int main(int argc, const char * argv[]) {
    Enviroment * enviroment = new Enviroment();
    Reader * reader = new ConsoleReader(enviroment);

    repl(reader, enviroment, true);

    return 0;
}
