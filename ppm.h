#ifndef _PPM_H_
#define _PPM_H_

#include <string>
#include <iostream>
#include <vector>

namespace imageRegistration
{
#ifndef __max
#define __max(x,y) (((x) < (y)) ? (y) : (x))
#endif
  
  class Optimizer;

  //struct rgb
  //{
  //  int r, g, b;
  //  rgb(): r(0), g(0), b(0) {}
  //  rgb(int barmi_): r(barmi_), g(barmi_), b(barmi_) {}
  //};

  /// A red-green-blue pixel. Levels are doubles!
  struct drgb
  {
    double r, g, b;
    drgb(): r(0), g(0), b(0) {}
    drgb(double barmi_): r(barmi_), g(barmi_), b(barmi_) {}
    drgb(double ar_, double ag_, double ab_) : r(ar_), g(ag_), b(ab_) {}
  };

  drgb operator * (const drgb & a, const drgb & b);
  drgb operator + (const drgb & a, const drgb & b);
  drgb operator - (const drgb & a, const drgb & b);
  drgb operator / (const drgb & a, int b);
  drgb & operator += (drgb & a, const drgb & b);
  drgb operator /= (drgb & a, int b);
  drgb operator -= (drgb & a, const drgb & b);
  int lMax (drgb point);

  /// Holds the image data read from a ppm file.
  class ppm
  {
  public:
    ppm(const std::string & filename_);
    ppm(const ppm & image_);
    
    /// Create a scaled down version of the input ppm to hold.
    ppm(const ppm & image_, size_t scale_);
    
    /// Create an blank image of given widht and height. The image data will be noise. 
    /// (Probably not faster than copying an existing image. (?)
    ppm(size_t width, size_t height);
    ppm() {};
    ~ppm();
    //ppm operator = (const ppm & osKep);
    /// Read image data from filename_
    void operator= (const std::string & filename_);
    
    /// Swap in the image data from otherImage_ using vector swap, ie for cheap
    void swap(ppm & otherImage_);

    size_t getW() const;
    size_t getH() const;
    static double corr (const ppm & p1_, const ppm & p2_, std::pair <int, int> tl1_, std::pair <int, int> tl2_, 
      std::pair <int, int> wh_); 
    size_t read(const std::string & filename_); 
    size_t write(const std::string & filename_) const;
    drgb getColor( std::vector< int > point ) const;
    void hulyeszin();
    void reWritePoint(std::pair <int, int> point_, drgb szin_);

    const std::vector< std::vector<drgb> > & getData(){return _dataInt;}
    
    friend class Optimizer;
  private:
    size_t _w, _h; // width, height
    std::vector< std::vector<drgb> > _dataInt; // picture data
    static std::string _magicword;
  };

  /// Holds an array of ppms. The intent is to hold different resolutions of a ppm.
  class ppmArray
  {
  public:
    /// Construct with one ppm to be scaled down.
    ppmArray(const ppm & image_, size_t minSize_);
    ppmArray() {};
    ~ppmArray();
    /// Create all(!) scaled down versions of the original.
    void scale();
    ppm getPic(size_t which_) { return *_pics[which_]; }
    size_t howManyLevels() { return _numLevels; }
    void picToFile(size_t which_, const std::string & filename_) { (*_pics[which_]).write(filename_); }
    const ppm & getLastPic() const { return *_pics[_numLevels - 1]; }
    /// For debugging purposes.
    void writeAll() const;
    
  private:
    size_t _minSize;
    std::vector < const ppm* > _pics;
    size_t _numLevels;
  };

} //namespace imageRegistration

#endif //_PPM_H_

