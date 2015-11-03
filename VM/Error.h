//
//  Error.h
//  VM
//
//  Created by Štěpánek Jindřich on 03/11/15.
//  Copyright (c) 2015 stepajin. All rights reserved.
//

#ifndef __Vm__Error__
#define __Vm__Error__

#include <iostream>
#include "Objects.h"

VmObject * error(std::string message); // returns NULL

#endif /* defined(__Vm__Error__) */
