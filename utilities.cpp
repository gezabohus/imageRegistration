/*
*  utilities.cpp
*  imageRegistration
*
*  Created by gbohus on 7/14/10.
*  Copyright 2010 gbohus. All rights reserved.
*
*/

#include <string>
#include "ppm.h"
#include "utilities.h"

namespace imageRegistration {

std::string transformString(const std::string & s_, const char * tr_)
{
  const char * lastDot = strrchr(s_.c_str(), '.');
  int offSet(0);
  if(lastDot)
    offSet = lastDot - (s_.c_str());
  std::string destination(s_);
  destination.resize(offSet);
  destination += tr_;
  destination += s_.substr(offSet, s_.size() - offSet);
  return destination;
 
}

  template <>
  double absAverageDiff<double>(const double & t0, const double & t1)
  {
    return abs(t0 - t1);
  }
  
  template <>
  double absAverageDiff<drgb>(const drgb & t0, const drgb & t1)
  {
    return abs(t0.r - t1.r);
  }
  
} //namespace imageRegistration
