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
#include "Environment.h"
#include "VM.h"
#include "Heap.h"

using namespace std;

void REPL(Reader * reader, Environment * environment) {
    Heap::INSTANCE()->addEnvironment(environment);
    
    VmObject * result = VmVoid::VOID();
    
    while (true) {
        VmObject * obj = reader->getObject();
        
        if (obj && obj->getTag() == TAG_EOF) {
            break;
        }
        
        if (obj == NULL) {
            cout << "error repl - read" << endl;
            exit(1);
        }
        
        // EVAL
        VmObject * eval = obj->eval(environment);
        
        if (!eval) {
            cout << "error repl - eval" << endl;
            exit(1);
        }
        
        if (obj->getTag() == TAG_RETURN) {
            break;
        }

        if (eval->getTag() == TAG_MOVE_BUFFER) {
            VmMoveBuffer * move = (VmMoveBuffer *) eval;
            
            if (move->getDirection() == FORWARD) {
                reader->skip(move->getSteps());
            } else {
                reader->rewind(move->getSteps());
            }
            
            continue;
        }
        
        if (eval != VmVoid::VOID()) {
            CallStack::INSTANCE()->push(eval);
        }
    
        Heap::INSTANCE()->collectIfNeeded();
    }
    
    Heap::INSTANCE()->removeEnvironment(environment);
}


int main(int argc, const char * argv[]) {
    string filePath = "/Users/stepanek/School/RUN/Compiler/BC.out";
    
    if (argc >= 2) {
        filePath = argv[1];
    }
    
    cout << filePath << endl;
    
    ifstream input(filePath.c_str(), std::ios::binary );
    if (!input.is_open()) {
        cout << "can't open file " << filePath << endl;
    }
    
    FileDataSource * dataSource = new FileDataSource(&input);
    
    Environment * environment = new Environment();

    Reader * reader = new Reader(dataSource, environment);
    
    REPL(reader, environment);
    
    VmObject * result = CallStack::INSTANCE()->pop();
    if (result)
        cout << "result: " << result->toString() << endl;
        
    delete dataSource;
    delete environment;
    delete reader;
    delete Heap::INSTANCE();
    delete CallStack::INSTANCE();
    
    return 0;
}
