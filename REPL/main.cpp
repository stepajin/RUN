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

LangObject * error(string message) {
    cout << "error: " << message << endl;
    return NULL;
}

// TODO error type
// TODO identifiers singletons

LangObject * repl(Reader * reader, Enviroment * enviroment) {
    LangObject * lastEval = NULL;
    LangObject * lastObject = NULL;
    
    while (true) {
        bool printEval = true;
        
        
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
        
        if (obj->getTag() == TAG_BLOCK) {
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
        
        /* eval */
        
        LangObject * eval = obj->eval(enviroment);

        if (eval && eval->getTag() == TAG_END) {
            return LangObject::getEND();
        }

        if (eval && printEval) {
            string s = eval->toString();
            
            if (s != "") {
                cout << s << endl;
            }
        }
        
        lastObject = obj;
        lastEval = eval;
    }
    
    return lastEval;
}

int main(int argc, const char * argv[]) {
    Enviroment * enviroment = new Enviroment();
    Reader * reader = new ConsoleReader(enviroment);

    repl(reader, enviroment);

    return 0;
}
