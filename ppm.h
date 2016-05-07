#ifndef _PPM_H_
#define _PPM_H_

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include "pixel.h"

namespace imageRegistration
{
  


  /// Holds the image data read from a ppm file. First coordinate goes 0..h, second goes 0..w!
  template <class PixelT>
  class ppm
  {
    //class Optimizer<PixelT>;
    //friend class Optimizer<PixelT>;

  public:
    typedef PixelT pixelType;
    ppm(const std::string & filename_) : _fileName(filename_)
    {
      if (!read(filename_))
        throw std::ios_base::failure("Could not open " + filename_ + "\n");
    }

    ppm(const ppm & image_)
    {
      _w = image_._w;
      _h = image_._h;
      _dataInt = image_._dataInt;
      _fileName = image_._fileName;
    }

    ppm(const ppm & image_, int scale_)
    {
      _w = image_._w / scale_;
      _h = image_._h / scale_;
      std::vector < PixelT > dummy(_w);
      _dataInt = std::vector < std::vector < PixelT > >(_h, dummy);
      PixelT black(0);

      for (size_t i = 0; i < _h; ++i)
        for (size_t j = 0; j < _w; ++j)
        {
          _dataInt[i][j] = black;
          for (size_t k = 0; k < scale_; ++k)
            for (size_t l = 0; l < scale_; ++l)
            {
              //_dataInt[i][j].r += image_._dataInt[i*scale_ + k][j*scale_ + l].r;
              //_dataInt[i][j].g += image_._dataInt[i*scale_ + k][j*scale_ + l].g;
              //_dataInt[i][j].b += image_._dataInt[i*scale_ + k][j*scale_ + l].b;
              _dataInt[i][j] += image_._dataInt[i*scale_ + k][j*scale_ + l];
            }
          _dataInt[i][j] /= scale_ * scale_;
        }
    }

    ppm(size_t width, size_t height)
    {
      setSize(width, height);
    }


    //ppm(const std::string & filename_);
    //ppm(const ppm & image_);
    
    /// Create a scaled down version of the input ppm to hold.
    //ppm(const ppm & image_, size_t scale_);
    
    /// Create an blank image of given widht and height. The image data will be noise. 
    /// (Probably not faster than copying an existing image. (?)
    //ppm(size_t width, size_t height);
    ppm() {}
    //~ppm();
    ~ppm() {}

    //ppm operator = (const ppm & osKep);
    /// Read image data from filename_
    //void operator= (const std::string & filename_);
    void operator= (const std::string & filename_)
    {
      read(filename_);
    }

    //void setSize(size_t width, size_t height);
    void setSize(size_t width, size_t height)
    {
      _w = width;
      _h = height;
      // todo: could be optimized by allocating the memory for _dataInt and setting
      // the pointers.
      std::vector < PixelT > dummy(_w);
      _dataInt = std::vector < std::vector < PixelT > >(_h, dummy);
    }

    /// Swap in the image data from otherImage_ using vector swap, ie for cheap
    //void swap(ppm & otherImage_);
    void swap(ppm & otherImage_)
    {
      _w = otherImage_._w;
      _h = otherImage_._h;
      _dataInt.swap(otherImage_._dataInt);

    }

    //size_t getW() const;
    //size_t getH() const;
    size_t getW() const
    {
      return _w;
    }

    size_t getH() const
    {
      return _h;
    }

    //static double corr (const ppm & p1_, const ppm & p2_, std::pair <int, int> tl1_, std::pair <int, int> tl2_, std::pair <int, int> wh_); 
    //static double corr(const ppm & p1_, const ppm & p2_, std::pair <int, int> tl1_, std::pair <int, int> tl2_, std::pair <int, int> wh_)
    //{
    //  const std::vector< std::vector<PixelT> > & d1(p1_._dataInt), &d2(p2_._dataInt);
    //  PixelT m1(0), v1(0), m2(0), v2(0), cov(0);
    //  double cor(0);
    //  int i, j;
    //  for (i = 0; i < wh_.first - 1; ++i)
    //    for (j = 0; j < wh_.second - 1; ++j)
    //    {
    //      m1 += d1[tl1_.first + i][tl1_.second + j];
    //      m2 += d2[tl2_.first + i][tl2_.second + j];
    //      v1 += d1[tl1_.first + i][tl1_.second + j] * d1[tl1_.first + i][tl1_.second + j];
    //      v2 += d2[tl2_.first + i][tl2_.second + j] * d2[tl2_.first + i][tl2_.second + j];
    //      cov += d1[tl1_.first + i][tl1_.second + j] * d2[tl2_.first + i][tl2_.second + j];
    //    }
    //  m1 /= wh_.first * wh_.second;
    //  m2 /= wh_.first * wh_.second;
    //  v1 /= wh_.first * wh_.second;
    //  v2 /= wh_.first * wh_.second;
    //  v1 -= m1 * m1;
    //  v2 -= m2 * m2;
    //  cov /= wh_.first * wh_.second;
    //  cov -= m1 * m2;
    //  PixelT corv(cov.r / pow(v1.r * v2.r, .5), cov.g / pow(v1.g * v2.g, .5), cov.b / pow(v1.b * v2.b, .5));
    //  //double r(cov.r), g(cov.g), b(cov.b);
    //  //r /= pow((double) v1.r * v2.r, 1/2);
    //  //g /= pow((double) v1.g * v2.g, 1/2);
    //  //b /= pow((double) v1.b * v2.b, 1/2);
    //  cor = (corv.r + corv.g + corv.b) / 3;
    //  return cor;
    //}

    //size_t read(const std::string & filename_); 
    //size_t write(const std::string & filename_) const;
    size_t read(const std::string &filename_)
    {
      _fileName = filename_;
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
      while (true)
      {
        ifs.getline(buffer, readBufferSize);
        if (buffer[0] != '#')
          break;
      }
      std::stringstream buffstream(buffer);
      buffstream >> _w >> _h;

      ifs.getline(buffer, readBufferSize);

      size_t imagePixs = _w * _h;
      size_t imageBytes = imagePixs * 3;
      std::vector<PixelT> dummy(_w);
      _dataInt = std::vector<std::vector<PixelT> >(_h, dummy);
      char *data = new char[imageBytes];
      ifs.read(data, imageBytes);
      for (size_t i = 0; i<imagePixs; ++i)
      {
        _dataInt[i / _w][i%_w] = PixelT((int)(unsigned char)data[3 * i], (int)(unsigned char)data[3 * i + 1], (int)(unsigned char)data[3 * i + 2]);
        //_dataInt[i / _w][i%_w].r = (int)(unsigned char)data[3 * i];
        //_dataInt[i / _w][i%_w].g = (int)(unsigned char)data[3 * i + 1];
        //_dataInt[i / _w][i%_w].b = (int)(unsigned char)data[3 * i + 2];
      }
      ifs.close();


      return imageBytes;
    }

    size_t write(const std::string &filename_) const
    {
      std::ofstream image(filename_.c_str(), std::ios_base::binary);
      if (!image.good())
        return 0;
      image << _magicword << "\n" << "# proba\n" << _w << " " << _h << "\n" << "255\n";
      if (!image.good())
        return 0;
      for (size_t i = 0; i< _w * _h; ++i)
      {
        char cr, cg, cb;
        _dataInt[i / _w][i%_w].write(cr, cg, cb);
        //const char cr = (const char)(_dataInt[i / _w][i%_w].r);
        //const char cg = (const char)(_dataInt[i / _w][i%_w].g);
        //const char cb = (const char)(_dataInt[i / _w][i%_w].b);
        image.write(&cr, 1);
        image.write(&cg, 1);
        image.write(&cb, 1);
      }
      return 3 * _w * _h;
    }


    //PixelT getColor( std::vector< int > point ) const;
    //PixelT getColor( int x, int y ) const;
    PixelT getColor(std::vector< int > point) const
    {
      return _dataInt[point[0]][point[1]];
    }

    PixelT getColor(int x, int y) const
    {
      return _dataInt[x][y];
    }

    //void hulyeszin();
    //void hulyeszin()
    //{
    //  for (size_t i = 0; i < _w * _h; ++i)
    //  {
    //    double j = (_dataInt[i / _w][i%_w].r + _dataInt[i / _w][i%_w].g + _dataInt[i / _w][i%_w].b) / 3;
    //    _dataInt[i / _w][i%_w].r = j;
    //    _dataInt[i / _w][i%_w].g = j;
    //    _dataInt[i / _w][i%_w].b = j;
    //  }
    //}

    //void setColor(int x_, int y_, PixelT color_);
    //void reWritePoint(std::pair <int, int> point_, PixelT szin_);
    void setColor(int x_, int y_, PixelT color_)
    {
      _dataInt[x_][y_] = color_;
    }

    void reWritePoint(std::pair <int, int> point_, PixelT color_)
    {
      _dataInt[point_.first][point_.second] = color_;
    }


    const std::vector< std::vector<PixelT> > & getData(){return _dataInt;}
    
    const std::string & getName() const {return _fileName;};
    
    static void imageRemapSmooth(const ppm & image_, double x, double y, ppm & target_, int i, int j)
    {
      size_t h(image_.getH());
      size_t w(image_.getW());
      PixelT gray(127);
      if ((x >= 0) && (x < h - 1) && (y >= 0) && (y < w - 1)) //TODO: not sure if boundaries are correct: h-1=>h, w-1=>w ?
      {
        int xl((int)x), xh((int)std::ceil(x)), yl((int)y), yh((int)std::ceil(y));
        PixelT colorll = image_.getColor(xl, yl);
        PixelT colorlh = image_.getColor(xl, yh);
        PixelT colorhl = image_.getColor(xh, yl);
        PixelT colorhh = image_.getColor(xh, yh);
        double wll = (1 - x + xl)*(1 - y + yl);
        double wlh = (1 - x + xl)*(1 + y - yh);
        double whl = (1 + x - xh)*(1 - y + yl);
        double whh = (1 + x - xh)*(1 + y - yh);
        colorll *= wll;
        colorlh *= wlh;
        colorhl *= whl;
        colorhh *= whh;
        PixelT color = colorll;
        color += colorlh;
        color += colorhl;
        color += colorhh;
        target_.setColor(i, j, color);
      }
      else
      {
        target_.setColor(i, j, gray);
      }
    }

    static void imageRemap(const ppm & image_, double x, double y, ppm & target_, int i, int j)
    {
      size_t h(image_.getH());
      size_t w(image_.getW());
      PixelT gray(127);
      if ((x >= 0) && (x < h) && (y >= 0) && (y < w))
      {
        PixelT color = image_.getColor((int)x, (int)y);
        target_.setColor(i, j, color);
      }
      else
      {
        target_.setColor(i, j, gray);
      }
    }

  private:
    size_t _w, _h; // width, height
    std::vector< std::vector<PixelT> > _dataInt; // picture data
    static std::string _magicword;
    std::string _fileName;
  };

  /// Holds an array of ppms. The intent is to hold different resolutions of a ppm.
  template <class PictureT>
  class ppmArray
  {
  public:
    /// Construct with one ppm to be scaled down.
    //ppmArray(const PictureT & image_, size_t minSize_);
    ppmArray(const PictureT & image_, size_t minSize_)
    {
      // todo: needs rewrite, suboptimal use of vectors
      _minSize = minSize_;
      //std::vector < const PictureT* > pics(1, & image_);
      size_t dummy = image_.getW() < image_.getH() ? image_.getW() : image_.getH();
      size_t numIter = 1;
      while ((dummy /= 2) > _minSize)
      {
        ++numIter;
      }
      _numLevels = numIter;
      _pics.resize(numIter);
      _pics[0] = &image_;
      scale();
      writeAll(image_.getName());
    }

    ppmArray() {}
    //~ppmArray();
    ~ppmArray() {}

    /// Create all(!) scaled down versions of the original.
    //void scale();
    void scale()
    {
      for (size_t i = 0; i < _numLevels - 1; ++i)
      {
        PictureT *pic = new PictureT(*_pics[i], 2);
        _pics[i + 1] = pic;
      }
    }

    PictureT getPic(size_t which_) const { return *_pics[which_]; }
    size_t getNumLevels() const { return _numLevels; }
    void picToFile(size_t which_, const std::string & filename_) { (*_pics[which_]).write(filename_); }
    const PictureT & getLastPic() const { return *_pics[_numLevels - 1]; }
    /// For debugging purposes.
    //void writeAll() const;
    void writeAll(const std::string &prefix) const
    {
      //char buffer[500];
      for (int k = 0; k < _pics.size(); ++k)
      {
        std::stringstream fns;
        fns<< "/tmp/dummy-" << prefix << "_" << k << ".ppm";
        //std::string fileName(buffer);
        std::string fileName = fns.str().c_str();
        _pics[k]->write(fileName);
      }
    }

    
  private:
    size_t _minSize;
    std::vector < const PictureT* > _pics;
    size_t _numLevels;
  };

} //namespace imageRegistration

#endif //_PPM_H_

