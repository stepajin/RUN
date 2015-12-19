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

VmObject * REPL(Reader * reader, Environment * environment) {
    //Heap::INSTANCE()->addEnvironment(environment);
    
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
            result = eval;
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

        CallStack::INSTANCE()->push(eval);

        Heap::INSTANCE()->collectIfNeeded();

        result = eval;
    }
    
    //Heap::INSTANCE()->removeEnvironment(environment);
    
    //CallStack::INSTANCE()->printStack();
    
    return result;
}


int main(int argc, const char * argv[]) {
    string filePath;
    
    if (argc >= 2) {
        filePath = argv[1];
    } else {
        cout << "no argument given" << endl;
        exit(1);
    }
    
    cout << filePath << endl;
    
    ifstream input(filePath.c_str(), std::ios::binary );
    if (!input.is_open()) {
        cout << "can't open file " << filePath << endl;
    }
    
    FileDataSource * dataSource = new FileDataSource(&input);
    
    Environment * environment = new Environment();
    environment->retain();
    
    Reader * reader = new Reader(dataSource, environment);
    
    VmObject * result = REPL(reader, environment);
    
    if (result)
        cout << "result: " << result->toString() << endl;
    
    delete dataSource;
    delete environment;
    delete reader;
    delete Heap::INSTANCE();
    delete CallStack::INSTANCE();
    
    return 0;
}
