//
//  transformationRecord.cpp
//  imageRegistration
//
//  Created by Geza Bohus on 1/2/16.
//
//

#include "transformationRecord.h"

namespace imageRegistration
{
  transformationRecord::transformationRecord(transformation & transformation_,
                                             std::string & name1_,
                                             std::string & name2_) :
  _tr(transformation_), _name1(name1_), _name2(name2_)
  {}
    
  void transformationRecord::write(std::ostream & out_)
  {
    out_ << _name1 << "\n" << _name2 << "\n";
    _tr.write(out_);
  }
  
  void transformationRecord::read(std::istream & in_)
  {
  }
  
} // namespace
