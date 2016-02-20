#include "ppm.h"
#include "utilities.h"

#include "math.h"
#include <iostream>
#include <fstream>

namespace imageRegistration
{
  
  std::string  ppm<drgb>::_magicword = std::string("P6");
  std::string  ppm<dGray>::_magicword = std::string("P5"); //pgm magic word





  

  //ppm operator = (ppm & image, const ppm & osKep)
  //{
  //  image._w = osKep.getW();
  //  image._h = osKep.getH();
  //  image._dataInt = osKep.getData();
  //}






  

  




} //namespace imageRegistration
