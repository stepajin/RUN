//
//  IOOperations.cpp
//  VM
//
//  Created by Štěpánek Jindřich on 05/12/15.
//  Copyright © 2015 stepajin. All rights reserved.
//

#include "IOOperations.h"
#include "Error.h"
#include "CallStack.h"

/*************
 
 File
 
 *************/

VmFile::VmFile() : VmObject(TAG_FILE) {
    this->input = NULL;
    this->output = NULL;
    this->name = NULL;
}

VmFile::~VmFile() {
    delete name;
    
    if (input) {
        input->close();
        delete input;
    }
    
    if (output) {
        output->close();
        delete output;
    }
}

void VmFile::readArguments(Reader * reader) {
    arg = CallStack::INSTANCE()->pop();
}

VmObject * VmFile::eval(Environment * environment) {
    if (name) {
        // already evaled
        return this;
    }
    
    VmObject * e = arg->eval(environment);
    
    if (e->getTag() != TAG_STRING) {
        return error("file identifier has to be string");
    }
    
    VmString * str = (VmString *) e;
    name = new string(str->getValue());
    
    return this;
}


void VmFile::write(string s) {
    if (input) {
        input->close();
        delete input;
        input = NULL;
    }
    
    if (!output) {
        output = new ofstream(*name);
    }
    
    output->write(s.c_str(), s.length());
}

VmString * VmFile::read() {
    if (output) {
        output->close();
        delete output;
        output = NULL;
    }
    
    if (!input) {
        input = new ifstream(*name);
    }
    
    string line;

    if (std::getline(*input, line)) {
        return new VmString(line);
    } else {
        return VmString::EOF_STRING();
    }
}

/*************
 
 Write
 
 *************/

WriteFunction::WriteFunction() : BuiltinFunction("write") {
    arg1 = NULL;
    arg2 = NULL;
}

VmObject * WriteFunction::eval(Environment * environment) {
    if (!arg1 || !arg2)
        return error("write: null arg");
    
    VmObject * f = arg1->eval(environment);
    VmObject * obj = arg2->eval(environment);
    
    if (f->getTag() != TAG_FILE ) {
        return error("write: not a file");
    }
    
    VmFile * file = (VmFile *) f;
    
    string s = obj->toString();
    
    //cout << "write " << s << endl;
    
    file->write(s);
    return VmVoid::VOID();
}

void WriteFunction::readArguments(Reader * reader) {
    arg2 = CallStack::INSTANCE()->pop();
    arg1 = CallStack::INSTANCE()->pop();
}


/*************
 
 Read
 
 *************/

ReadFunction::ReadFunction() : BuiltinFunction("read") {
    arg = NULL;
}

VmObject * ReadFunction::eval(Environment * environment) {
    if (!arg)
        return error("read: null arg");
    
    VmObject * f = arg->eval(environment);
    
    if (f->getTag() != TAG_FILE ) {
        return error("read: not a file");
    }
    
    VmFile * file = (VmFile *) f;
    VmString * str = file->read();
    
    //cout << "read " << str->getValue() << endl;
    
    return str;
}

void ReadFunction::readArguments(Reader * reader) {
    arg = CallStack::INSTANCE()->pop();
}
