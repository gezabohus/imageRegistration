#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "ppm.h"
#include "Optimizer.h"
#include "utilities.h"
#include "transformation.h"
#include "transformationRecord.h"

int main(int argc_, char** argv_)
{
  using namespace imageRegistration;
	std::string source1, source2;
  std::vector<std::string> filenames;
  
  if (argc_ > 2) {
    filenames[0] = argv_[1];
    filenames[1] = argv_[2];
  }
  else if (argc_ == 2) // we have a listfile with filenames
  {
    std::ifstream listFile(argv_[1]);
    while(listFile.good())
    {
#define bufsize 1000
      char buf[bufsize];
      listFile.getline(buf, bufsize-1); // for now we assume it's only filename on the line
      if(buf[0] == 0) // stop reading
        break;
      if(buf[0] == '#') // skip
        continue;
      filenames.push_back(buf); // store
    }
  }
  else // if not enough input, add two images
  {
    filenames.push_back("C:/Users/bohus_000/Documents/Visual Studio 2015/Projects/imageRegistration/ConvertedFiles_grey/DSCN7314grey_.ppm");
    filenames.push_back("C:/Users/bohus_000/Documents/Visual Studio 2015/Projects/imageRegistration/ConvertedFiles_grey/DSCN7315grey_.ppm");
  }

  // "filenames" now has the list of filenames to process
  
  if(filenames.size() < 2)
  {
    std::cout << "You need to provide at least two filenames." << std::endl;
    exit(1);
  }

  //typedef drgb pixT;
  typedef dGray pixT;

  /// We walk on the list of names. For a pair of images (starting with the first two, we
  /// transform the second one to match the first. We write it out and make it the first
  /// image of the next pair.
  
  source1 = *filenames.begin();
	ppm< pixT > kep1(source1), kep2;
  std::string name1(*filenames.begin());
	std::string destination;
  std::string transformationFileName("trans.bin");
  
  for(std::vector<std::string>::iterator i = filenames.begin() + 1; i != filenames.end(); ++i)
  {
    std::string & name2(*i);
    kep2 = name2;
    transformation< ppm<pixT> > goodtraf = findBest(kep1, kep2);
    //transformationRecord< pixT > trR(goodtraf, name1, name2);
    goodtraf(kep2, true, kep1);
    destination = transformString(name2);
    kep2.write(destination);
    std::cout << "Wrote " << destination.c_str() << ".\n";
    kep1 = kep2;
    name1 = name2;
  }
  
//  ppm image0(filenames[0]);
//  for(size_t fIndex = 1; fIndex < filenames.size(); ++fIndex)
//  {
//    ppm image1(filenames[fIndex]);
//    ppmArray pa0(image0, 50);
//    pa0.writeAll();
//    ppmArray pa1(image1, 50);
//    pa1.writeAll();
    
//    Optimizer opt0(image0, image1);
//    opt0.setSize(21); // should depend on size
//    opt0.setMargin(50); // should depend on size
//    ppm kep3 = opt0.positioner();   // transform of #2
//    std::string destination(transformString(source2));
//    kep3.write(destination);
//  }
  
//  ppm kep(source1);

//  
//  for(int i=0; i<9; ++i)
//  {
//    char buf2[1000];
//    //sprintf(buf1,"Na%d.ppm", i);
//    source1 = destination;
////    sprintf(buf2 , "D:/Projects/ImageRegistration/ImageRegistration/construction/DSCN%d.ppm", 7378 + i); 
//    sprintf(buf2 , "/Users/gbohus/Pictures/construction/DSCN%d.ppm", 7325 + i);
//    source2 = buf2;
//    destination = transformString(source2);
//    kep = source1;
//    kep2 = source2;
//    Optimizer opt(kep, kep2);
//    opt.setSize(21);
//    opt.setMargin(50);
//    kep3 = opt.positioner();
//    kep3.write(destination);
//  }
//  return 0;
}

/* {
  using namespace imageRegistration;
  std::string source1 = "D:/Projects/ImageRegistration/ImageRegistration/construction/DSCN7376.ppm";
  std::string source2 = "D:/Projects/ImageRegistration/ImageRegistration/construction/DSCN7377.ppm";
  ppm kep(source1);
  
  
  ppm kep2(source2);
  
  transformation goodtraf = findBest(kep, kep2);
  
  goodtraf.getTraf();
  
  goodtraf(kep2).write("D:/Projects/ImageRegistration/ImageRegistration/construction/0DSCN73760.ppm");
  
  return 0;
}
 */

