#include "pixel.h"
#include "ppm.h"
#include "utilities.h"

#include "math.h"
#include <iostream>
#include <fstream>

namespace imageRegistration
{
  
  template<> std::string  ppm<drgb>::_magicword = std::string("P6");
  template<> std::string  ppm<dGray>::_magicword = std::string("P6"); //pgm magic word





  

  //ppm operator = (ppm & image, const ppm & sourcePic)
  //{
  //  image._w = sourcePic.getW();
  //  image._h = sourcePic.getH();
  //  image._dataInt = sourcePic.getData();
  //}






  

  




} //namespace imageRegistration
