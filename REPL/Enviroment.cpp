//
//  Enviroment.cpp
//  REPL
//
//  Created by Štěpánek Jindřich on 26/10/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include "Enviroment.h"

Enviroment::Enviroment() {
    store = new map<string, LangObject *>;
}

Enviroment::Enviroment(Enviroment * parent) {
    store = new map<string, LangObject *>(*(parent->store));
}

void Enviroment::set(string key, LangObject * value) {
    (*store)[key] = value;
}

LangObject * Enviroment::get(string key) {
    return (*store)[key];
}

bool Enviroment::isSet(string key) {
    LangObject * value = get(key);
    return value != NULL;
}
