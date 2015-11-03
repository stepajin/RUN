//
//  Error.cpp
//  VM
//
//  Created by Štěpánek Jindřich on 03/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#include "Error.h"

VmObject * error(string message) {
    cout << "error: " << message << endl;
    return NULL;
}

