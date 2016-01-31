//
//  transformationRecord.h
//  imageRegistration
//
//  Created by Geza Bohus on 1/2/16.
//
//

#ifndef imageRegistration_transformationRecord_h
#define imageRegistration_transformationRecord_h

#include <iostream>
#include <string>

#include "transformation.h"

namespace imageRegistration
{

class transformationRecord
{
public:
  transformationRecord(transformation & transformation_,
                       std::string & name1_,
                       std::string & name2_);
  void write(std::ostream & out_);
  void read(std::istream & in_);

private:
  transformation & _tr;
  std::string & _name1, & _name2;
};

} // namespace
#endif
