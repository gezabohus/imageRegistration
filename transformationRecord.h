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

  template < class TransformT >
  class transformationRecord
  {
  public:

    transformationRecord(TransformT & transformation_,
      std::string & name1_,
      std::string & name2_) :
      _tr(transformation_), _name1(name1_), _name2(name2_)
    {}

    void write(std::ostream & out_)
    {
      out_ << _name1 << "\n" << _name2 << "\n";
      _tr.write(out_);
    }

    void read(std::istream & in_)
    {
    }

  private:
    TransformT & _tr;
    std::string & _name1, & _name2;
  };

} // namespace
#endif
