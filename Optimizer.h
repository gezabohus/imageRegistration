#ifndef _OPTIMIZER_H_
#define _OPTIMIZER_H_
#include <vector>

#include "ppm.h"

namespace imageRegistration
{
  
  
  /// Holds two images and tries to find a transform that applied to one will make it 
  /// look most like the other. The transformations are rotations and translations only.
template <typename PixelT>
  class Optimizer
  {
    //class ppm<PixelT>;

  public:
    Optimizer(const ppm< PixelT > & l_, const ppm< PixelT > & r_) :
      _leftImage(l_), _rightImage(r_)
    {
    }

    ~Optimizer(void)
    {
    }

//    /// Very specialized search function. Looks at one predefined area of the first 
//    /// image (selected by where_) and looks for the most similar area in the second image. 
//    size_t search (size_t where_[2], std::vector < int > & holmax_, double & var_);
    
//    void search (std::vector < std::vector < int > > & holmax_, std::vector < double > & var_);
    //void searchForMatch (std::vector < int > & holmax_, size_t radius_, std::vector < int > & holmax2_, double & mennyimax_);
//    ppm positioner (void);
    //void setSize (int n_);
    //void setMargin (int n_);
    
    /// One possible similarity measure between two images.
    //double similarity() const;

    void setSize(int n_)
    {
      _sampleSize = n_;
    }
    void setMargin(int n_)
    {
      _margin = n_;
    }

    double similarity() const
    {

      return absAverageDiff(_leftImage._dataInt, _rightImage._dataInt);
    }

  private:
    size_t _sampleSize;
    size_t _margin;
    size_t _searchRadius;
    const ppm< PixelT > & _leftImage;
    const ppm< PixelT > & _rightImage;
  };

} //namespace imageRegistration

#endif //_OPTIMIZER_H_
