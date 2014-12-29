#include "ppm.h"
#include "utilities.h"

#include "math.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace imageRegistration
{

  std::string  ppm::_magicword = std::string("P6");

  ppm::ppm(const std::string & filename_)
  {
    if(!read(filename_))
      throw std::ios_base::failure("Could not open " + filename_ + "\n");
  }

  void ppm::operator= (const std::string & filename_)
  {
    read(filename_);
  }

  void ppm::swap(ppm & otherImage_)
  {
    _w = otherImage_._w;
    _h = otherImage_._h;
    _dataInt.swap(otherImage_._dataInt);
    
  }

  ppm::ppm(const ppm & image_)
  {
    _w = image_._w;
    _h = image_._h;
    _dataInt = image_._dataInt;
  }

  ppm::ppm(const ppm & image_, size_t scale_)
  {
    _w = image_._w / scale_;
    _h = image_._h / scale_;
    std::vector < drgb > dummy(_w);
    _dataInt = std::vector < std::vector < drgb > >(_h, dummy);
    drgb black(0);

    for (size_t i = 0; i < _h; ++i)
      for (size_t j = 0; j < _w; ++j)
      {
        _dataInt[i][j] = black;
        for (size_t k = 0; k < scale_; ++k)
          for (size_t l = 0; l < scale_; ++l)
          {
            _dataInt[i][j].r += image_._dataInt[i*scale_ + k][j*scale_ + l].r;
            _dataInt[i][j].g += image_._dataInt[i*scale_ + k][j*scale_ + l].g;
            _dataInt[i][j].b += image_._dataInt[i*scale_ + k][j*scale_ + l].b;
          }
        _dataInt[i][j] /= scale_ * scale_;
      }
  }

  ppm::ppm(size_t width, size_t height) : _w(width), _h(height)
  {
    // todo: could be optimized by allocating the memory for _dataInt and setting
    // the pointers.
    std::vector < drgb > dummy(_w);
    _dataInt = std::vector < std::vector < drgb > >(_h, dummy);
  }

  drgb ppm::getColor( std::vector< int > point ) const
  {
    return _dataInt[point[0]][point[1]];
  }

  size_t ppm::getW() const 
  {
    return _w;
  }

  size_t ppm::getH() const 
  {
    return _h;
  }

  //ppm operator = (ppm & image, const ppm & osKep)
  //{
  //  image._w = osKep.getW();
  //  image._h = osKep.getH();
  //  image._dataInt = osKep.getData();
  //}

  size_t ppm::read(const std::string &filename_)
  {
    const int readBufferSize = 1000;
    std::ifstream ifs(filename_.c_str(), std::ios_base::binary);
	  char buffer[readBufferSize];

    if (!ifs.good())
      return 0;

	  ifs.getline(buffer, readBufferSize);
    if (!ifs.good())
      return 0;
	  if (buffer != _magicword)
      return 0;
    while( true )
    {
      ifs.getline(buffer, readBufferSize);
      if (buffer[0] != '#')
        break;
    }
    std::stringstream buffstream (buffer);
    buffstream >> _w >> _h;

    ifs.getline(buffer, readBufferSize);

	  size_t imagePixs = _w * _h;
	  size_t imageBytes = imagePixs * 3;
    std::vector<drgb> dummy(_w);
    _dataInt = std::vector<std::vector<drgb> > (_h, dummy);
    char *data = new char[imageBytes];
    ifs.read ( data , imageBytes );
    for (size_t i=0; i<imagePixs; ++i)
    {
      _dataInt[i/_w][i%_w].r = (int) (unsigned char)data[3*i];
      _dataInt[i/_w][i%_w].g = (int) (unsigned char)data[3*i+1];
      _dataInt[i/_w][i%_w].b = (int) (unsigned char)data[3*i+2];
    }
	  ifs.close();


    return imageBytes; 
  }

  size_t ppm::write(const std::string &filename_) const
  {
    std::ofstream image (filename_.c_str(), std::ios_base::binary);
    if (!image.good())
      return 0;
    image << _magicword << "\n" << "# proba\n" << _w << " " << _h << "\n" << "255\n" ;
    if (!image.good())
      return 0;
    for(size_t i=0; i< _w * _h; ++i)
    {
      const char cr = (const char)(_dataInt[i/_w][i%_w].r);
      image.write( &cr, 1 );
      const char cg = (const char)(_dataInt[i/_w][i%_w].g);
      image.write( &cg, 1 );
      const char cb = (const char)(_dataInt[i/_w][i%_w].b);
      image.write( &cb, 1 );
    }
    return 3 * _w * _h;
  }

  double ppm::corr (const ppm & p1_, const ppm & p2_, std::pair <int, int> tl1_, std::pair <int, int> tl2_, 
    std::pair <int, int> wh_) 
  {
    const std::vector< std::vector<drgb> > & d1(p1_._dataInt), & d2(p2_._dataInt);
    drgb m1(0), v1(0), m2(0), v2(0), cov(0);
    double cor(0);
    int i, j;
    for (i = 0; i < wh_.first - 1; ++i)
      for (j = 0; j < wh_.second - 1; ++j)
      {
        m1 += d1[tl1_.first + i][tl1_.second + j];
        m2 += d2[tl2_.first + i][tl2_.second + j];
        v1 += d1[tl1_.first + i][tl1_.second + j] * d1[tl1_.first + i][tl1_.second + j];
        v2 += d2[tl2_.first + i][tl2_.second + j] * d2[tl2_.first + i][tl2_.second + j];
        cov += d1[tl1_.first + i][tl1_.second + j] * d2[tl2_.first + i][tl2_.second + j];
      }
    m1 /= wh_.first * wh_.second;
    m2 /= wh_.first * wh_.second;
    v1 /= wh_.first * wh_.second;
    v2 /= wh_.first * wh_.second;
    v1 -= m1 * m1;
    v2 -= m2 * m2;
    cov /= wh_.first * wh_.second;
    cov -= m1 * m2;
    drgb corv(cov.r / pow(v1.r * v2.r, .5), cov.g / pow(v1.g * v2.g, .5), cov.b / pow(v1.b * v2.b, .5));
    //double r(cov.r), g(cov.g), b(cov.b);
    //r /= pow((double) v1.r * v2.r, 1/2);
    //g /= pow((double) v1.g * v2.g, 1/2);
    //b /= pow((double) v1.b * v2.b, 1/2);
    cor = (corv.r + corv.g + corv.b) / 3;
    return cor;
  }

  ppm::~ppm()
  {

  }

  void ppm::hulyeszin()
  {
    for (size_t i = 0; i < _w * _h; ++i)
    {
      double j=(_dataInt[i/_w][i%_w].r+_dataInt[i/_w][i%_w].g+_dataInt[i/_w][i%_w].b)/3;
      _dataInt[i/_w][i%_w].r = j;
      _dataInt[i/_w][i%_w].g = j;
      _dataInt[i/_w][i%_w].b = j;
    }
  }

  drgb operator + (const drgb & a, const drgb & b)
  {
    drgb c(a);
    c.r += b.r;
    c.g += b.g;
    c.b += b.b;
    return c;
  }

  drgb & operator += (drgb & a, const drgb & b)
  {
    a.r +=  b.r;
    a.g +=  b.g;
    a.b +=  b.b;
    return a;
  }

  drgb operator - (const drgb & a, const drgb & b)
  {
    drgb c(a);
    c.r -= b.r;
    c.g -= b.g;
    c.b -= b.b;
    return c;
  }

  drgb operator -= (drgb & a, const drgb & b)
  {
    a.r -= b.r;
    a.g -= b.g;
    a.b -= b.b;
    return a;
  }

  drgb operator * (const drgb & a, const drgb & b)
  {
    drgb c(a);
    c.r *= b.r;
    c.g *= b.g;
    c.b *= b.b;
    return c;
  }

  drgb operator / (const drgb & a, int b)
  {
    drgb c(a);
    c.r /= b;
    c.g /= b;
    c.b /= b;
    return c;
  }
  
  drgb operator - (const drgb & a, int b)
  {
    drgb c(a);
    c.r -= b;
    c.g -= b;
    c.b -= b;
    return c;
  }
  
  drgb operator /= (drgb & a, int b)
  {
    a.r /= b;
    a.g /= b;
    a.b /= b;
    return a;
  }

  /// Find the maximum color level of a double rgb pixel.
  int lMax (drgb point)
  {
    return(int)( __max( __max( __max(point.r, -point.r) , __max(point.g, -point.g) ) , __max(point.b, -point.b) ) );
  }
  
  void ppm::reWritePoint(std::pair <int, int> point_, drgb szin_)
  {
    _dataInt[point_.first][point_.second] = szin_;
  }

  ppmArray::ppmArray(ppm & image_, size_t minSize_)
  {
    // todo: needs rewrite, suboptimal use of vectors
    _minSize = minSize_;
    std::vector < ppm* > pics(1, & image_);
    size_t dummy = image_.getW() < image_.getH() ? image_.getW() : image_.getH();
    size_t numIter = 1;
    while ( (dummy /= 2) > _minSize )
    {
      ++numIter;
    }
    _numLevels = numIter;
    pics.resize(numIter);
    _pics = pics;
    scale();
  }
  
  ppmArray::~ppmArray()
  {

  }

  void ppmArray::scale()
  {
    for( size_t i = 0; i < _numLevels - 1; ++i )
    {
      ppm *pic = new ppm(*_pics[i], 2);
      _pics[i+1] = pic;
    }
  }

#include <stdlib.h>
  void ppmArray::writeAll() const
  {
    char buffer[5]; 
    for(int k = 0; k < _pics.size(); ++k)
    {
      sprintf(buffer, "/tmp/dummy-%d.ppm", k);
      std::string fileName(buffer);
      _pics[k]->write(fileName);
    }
  }


} //namespace imageRegistration
