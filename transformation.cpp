//
//  transformation.cpp
//  imageRegistration
//
//  Created by Geza Bohus on 12/27/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <math.h>
#include "ppm.h"
#include "transformation.h"

namespace imageRegistration
{
  template<> std::pair < int, int > transformation< ppm<drgb> >::_origin = std::pair < int, int >(0, 0);
  template<> std::pair < int, int > transformation< ppm<dGray> >::_origin = std::pair < int, int >(0, 0);
} // namespace imageRegistration
