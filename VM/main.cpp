//
//  main.cpp
//  VM
//
//  Created by Štěpánek Jindřich on 02/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <fstream>

#include "Reader.h"
#include "CallStack.h"
#include "BuiltinFunctions.h"
#include "Enviroment.h"
#include "VM.h"

using namespace std;

VmObject * REPL(Reader * reader, Enviroment * enviroment) {
    VmObject * result = VmVoid::VOID();
    
    while (true) {
        VmObject * obj = reader->getObject();
        
        if (obj && obj->getTag() == TAG_EOF)
            break;
        
        if (obj == NULL) {
            cout << "error " << endl;
            return NULL;
        }
        
        // EVAL
        
        VmObject * eval = obj->eval(enviroment);
        
        if (!eval) {
            cout << "error " << endl;
            return NULL;
        }
        
        if (obj->getTag() == TAG_RETURN) {
            return eval;
        }

//        if (eval->getTag() == TAG_VOID)
//            continue;
        
        if (eval->getTag() == TAG_MOVE_BUFFER) {
            VmMoveBuffer * move = (VmMoveBuffer *) eval;
            
            if (move->getDirection() == FORWARD) {
                reader->skip(move->getSteps());
            } else {
                reader->rewind(move->getSteps());
            }
            
            continue;
        }
        
        CallStack::INSTANCE()->push(eval);
    
        result = eval;
    }
    
    return result;
}


int main(int argc, const char * argv[]) {
    string filePath = "/Users/stepanek/School/RUN/Compiler/BC.out";
    
    if (argc >= 2) {
        filePath = argv[1];
    }
    
    cout << filePath << endl;
    
    ifstream input(filePath, std::ios::binary );
    if (!input.is_open()) {
        cout << "can't open file " << filePath << endl;
    }
    
    FileDataSource * dataSource = new FileDataSource(&input);
    Enviroment * enviroment = new Enviroment();

    Reader * reader = new Reader(dataSource, enviroment);
    
    VmObject * result = REPL(reader, enviroment);
    
    if (result)
        cout << "result: " << result->toString() << endl;
    
    return 0;
}
