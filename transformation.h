//
//  transformation.h
//  imageRegistration
//
//  Created by Geza Bohus on 12/27/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//


#ifndef imageRegistration_transformation_h
#define imageRegistration_transformation_h

#include <algorithm>
#include <utility>
#include <vector>

#include "ppm.h"

namespace imageRegistration
{

  
  
  /// A transformation is a translation and rotation of the plane.
  template <class PictureT>
  class transformation
  {
  public:
    void setAngle(double angle_) { _angle = angle_; if(std::abs(_angle > 1)) throw; }
    void setOrigin(std::pair < int, int > origin_) { _origin.first = origin_.first; _origin.second = origin_.second; }
    void setVector(std::pair < int, int > vector_) { _vector.first = vector_.first; _vector.second = vector_.second; }
    void setLevel(size_t level_) { _level = level_; }
    //void setAll(double a_, int ox_, int oy_, int vx_, int vy_);
    void setAll(double a_, int ox_, int oy_, int vx_, int vy_)
    {
      _angle = a_;
      if (std::abs(_angle > 1)) throw;
      //  _origin.first = ox_;
      //  _origin.second = oy_;
      _vector.first = vx_;
      _vector.second = vy_;
    }
    double getAngle() const { return _angle; }
    std::pair < int, int > getOrigin() const { return _origin; }
    std::pair < int, int > getVector() const { return _vector; }
    size_t getLevel() const { return _level; }
    void getTraf() { std::cout << _angle << ", (" << _origin.first << ", " << _origin.second << "), (" << _vector.first << ", " << _vector.second << ")\n"; }
    transformation( double angle_, std::pair < int , int > origin_, std::pair < int , int > vector_, size_t level_ ) :
    _angle(angle_), /*_origin(origin_),*/ _vector(vector_), _level(level_){if(std::abs(_angle > 1)) throw; }
    // transformation();
    //    ~transformation();

    /// Inverse transform an image by rotating around _origin and translating by _vector. 
    /// The source image is 
    //PictureT operator() (const PictureT & image_, bool smooth, PictureT & target_) const ;
    PictureT operator() (const PictureT & image_, bool smooth, PictureT & target_) const
    {
      size_t h(image_.getH());
      size_t w(image_.getW());
      target_.setSize(w, h);
      double c(cos(_angle));
      double s(sin(_angle));
      int i;
      int j;
      int & ox(_origin.first);
      int & oy(_origin.second);
      //    std::vector< int > rPoint(2);
      double x, y;

      for (i = 0; i < h; ++i)
        for (j = 0; j < w; ++j)
        {
          x = (((i - ox)) * c + ((j - oy)) * (-1) * s + ox - _vector.first + 0.5);
          y = (((i - ox)) * s + ((j - oy)) *        c + oy - _vector.second + 0.5);
          //      if(std::abs(int(x-i)) > 50 || std::abs(int(y-j)) > 50)
          //        throw;
          if (smooth)
          {
            PictureT::imageRemapSmooth(image_, x, y, target_, i, j);
          }
          else
          {
            PictureT::imageRemap(image_, x, y, target_, i, j);
          }
        }
      return target_;
    }

    /// Transform a point by rotating it around the _origin and translating by _vector. Does not
    /// check if we're outside of the image. Output the new coordinates.
    //std::pair < int, int > operator() (const std::pair < size_t, size_t > &) const ;
    std::pair < int, int > operator() (const std::pair < size_t, size_t > & point_) const
    {
      std::pair < int, int > point;
      double c(cos(_angle));
      double s(sin(_angle));
      point.first = (int)((((int)point_.first - _origin.first)) *        c + (((int)point_.second - _origin.second)) *  s + _origin.first + _vector.first + 0.5);
      point.second = (int)((((int)point_.first - _origin.first)) * (-1) * s + (((int)point_.second - _origin.second)) *  c + _origin.second + _vector.second + 0.5);
      return point;
    }
    //std::vector <int > operator() (const std::vector  <int > &) const ;
    std::vector <int > operator() (const std::vector  <int > & point_) const
    {
      std::vector < int> point(2);
      double c(cos(_angle));
      double s(sin(_angle));
      point[0] = (int)(((point_[0] - _origin.first)) *        c + ((point_[1] - _origin.second)) *  s + _origin.first + _vector.first + 0.5);
      point[1] = (int)(((point_[0] - _origin.first)) * (-1) * s + ((point_[1] - _origin.second)) *  c + _origin.second + _vector.second + 0.5);
      return point;
    }
    //transformation inverse();
    transformation inverse()
    {
      double angle;
      std::pair < int, int > origin;
      std::pair < int, int > vector;
      size_t level;
      angle = -_angle;
      origin = _origin;
      vector.first = (int)(-_vector.first * cos(_angle) + _vector.second * sin(_angle));
      vector.second = (int)(-_vector.first * sin(_angle) - _vector.second * cos(_angle));
      level = _level;
      transformation transf(angle, origin, vector, level);
      return transf;
    }
    //void goOut(size_t scale_);
    void goOut(int scale_)
    {
      if (_level != 0)
      {
        --_level;
        _origin.first *= scale_;
        _origin.second *= scale_;
        _vector.first *= scale_;
        _vector.second *= scale_;
      }
      else
      {
        printf("Already at level 0!\n");
      }
    }
    //void write(std::string fn_) const ;
    /// oldschool write
    void write(std::string fn_) const
    {
      std::FILE *file;
      file = std::fopen(fn_.c_str(), "ab");
      fwrite(&_angle, sizeof(double), 1, file);
      fwrite(&_origin.first, sizeof(int), 1, file);
      fwrite(&_origin.second, sizeof(int), 1, file);
      fwrite(&_vector.first, sizeof(int), 1, file);
      fwrite(&_vector.second, sizeof(int), 1, file);
      fwrite(&_level, sizeof(size_t), 1, file);
    }
    //void read(std::string fn_);
    void read(std::string fn_)
    {
      std::FILE *file;
      file = std::fopen(fn_.c_str(), "rb");
      fread(&_angle, sizeof(double), 1, file);
      fread(&_origin.first, sizeof(int), 1, file);
      fread(&_origin.second, sizeof(int), 1, file);
      fread(&_vector.first, sizeof(int), 1, file);
      fread(&_vector.second, sizeof(int), 1, file);
      fread(&_level, sizeof(size_t), 1, file);
    }
    //void write(std::ostream & out_) const;
    void write(std::ostream & out_) const
    {
      out_ << _angle << _origin.first << _origin.second << _vector.first << _vector.second <<
        _level;
    }

  private:
    double _angle;
    static std::pair < int, int > _origin;
    std::pair < int, int > _vector;
    size_t _level;
  };

  /// Compute the correlation between the first and the transformed second image.
  template < class PictureT >
  double transformationCorrelation(const PictureT & firstPic, const PictureT & secondPic, const transformation< PictureT > & transf_)
  {

    // These will collect  (partial sums of) means and variances for the two images.
    typename PictureT::pixelType firstM(0), firstV(0), secondM(0), secondV(0), coVar(0), corr(0);
    int numPoints = 0;
    std::vector<int> ijpoint(2); // this will index the second image
    int &i(ijpoint[0]);
    int &j(ijpoint[1]);
    std::vector<int> point(2);
    size_t h(secondPic.getH()), w(secondPic.getW());
    for (i = (int)(0.1*h); i < (int)(0.9*h); ++i)
      for (j = (int)(0.1*w); j < (int)(0.9*w); ++j)
      {
        //point.first  = (int) ((((int)i - _origin.first)) * cos(_angle) + (((int)j - _origin.second)) * (-1) * sin(_angle) + _origin.first - _vector.first + 0.5);
        //point.second = (int) ((((int)i - _origin.first)) * sin(_angle) + (((int)j - _origin.second)) *        cos(_angle) + _origin.second - _vector.second + 0.5);
        point = transf_(ijpoint);
        if ((point[0] >= 0) && (point[0] < (int)firstPic.getH()) && (point[1] >= 0) && (point[1] < (int)firstPic.getW()))
        {
          typename PictureT::pixelType fc, sc;
          fc = firstPic.getColor(point);
          sc = secondPic.getColor(i, j);
          firstM += fc;
          secondM += sc;
          firstV += fc * fc;
          secondV += sc * sc;
          coVar += fc * sc;
          ++numPoints;
        }
      }
    if (numPoints == 0)
    {
      std::cout << "The transformed image does not intersect the first image.\n";
    }
    else
    {
      firstM /= numPoints;
      secondM /= numPoints;
      firstV /= numPoints;
      secondV /= numPoints;
      firstV -= firstM * firstM;
      secondV -= secondM * secondM;
      coVar /= numPoints;
      coVar -= firstM * secondM;
      corr = coVar;
      typename PictureT::pixelType devProd = PictureT::pixelType::geomMean(firstV, secondV);
      PictureT::pixelType::piecewiseDiv(corr, devProd);
      //corr.r /= pow((double)(firstV.r * secondV.r), 0.5);
      //corr.g /= pow((double)(firstV.g * secondV.g), 0.5);
      //corr.b /= pow((double)(firstV.b * secondV.b), 0.5);
    }
    return corr.gray();
  }

  template < class PictureT >
  transformation< PictureT > bruteForceBase(const PictureT & firstPic, const PictureT & secondPic)
  {
//    firstPic.write("/tmp/1.ppm");
//    secondPic.write("/tmp/2.ppm");
    const int originXmin = 0; //(int) secondPic.getH() / 2;
    const int originYmin = 0; //(int) secondPic.getW() / 2;
    const int originXmax = originXmin + 1;
    const int originYmax = originYmin + 1;
    const int vectorXmin = -((int)secondPic.getH() / 5) - 1;
    const int vectorYmin = -((int)secondPic.getW() / 5) - 1;
    const int vectorXmax = (int)secondPic.getH() / 5 + 1;
    const int vectorYmax = (int)secondPic.getW() / 5 + 1;
    double angleMin = -.3;
    double angleMax = .3;
    double anglestep = asin((double)2 / std::max((int)secondPic.getH(), (int)secondPic.getW()));

    std::pair < int, int > orig(0, 0);

    transformation< PictureT > traf(0, orig, orig, 0);
    std::pair < int, int > bestOri(originXmin, originYmin), bestVec(vectorXmin, vectorYmin);
    double bestAng(angleMin), bestCorr, tempCorr;

    bestCorr = -2;

    for (int orix = originXmin; orix < originXmax; ++orix)
      for (int oriy = originYmin; oriy < originYmax; ++oriy)
        for (int vecx = vectorXmin; vecx < vectorXmax; ++vecx)
          for (int vecy = vectorYmin; vecy < vectorYmax; ++vecy)
            // we want to allow angle to be zero and it's a double, so we go from zero to up and down
            for (double ang = 0.; ang < angleMax + anglestep; ang += anglestep) // these are not ints
            {
              traf.setAll(ang, orix, oriy, vecx, vecy);
              if (bestCorr < (tempCorr = transformationCorrelation(firstPic, secondPic, traf)))
              {
                bestOri = std::pair < int, int >(orix, oriy);
                bestVec = std::pair < int, int >(vecx, vecy);
                bestAng = ang;
                bestCorr = tempCorr;
              }

              traf.setAll(-ang, orix, oriy, vecx, vecy);
              if (bestCorr < (tempCorr = transformationCorrelation(firstPic, secondPic, traf)))
              {
                bestOri = std::pair < int, int >(orix, oriy);
                bestVec = std::pair < int, int >(vecx, vecy);
                bestAng = -ang;
                bestCorr = tempCorr;
              }

            }
    transformation< PictureT > bestTraf(bestAng, bestOri, bestVec, 0);
    return bestTraf;
  }
  
  template < class PictureT >
  transformation< PictureT > oneStep(const PictureT & firstPic, const PictureT & secondPic, const transformation< PictureT > & trans)
  {
    std::pair < int, int > orig;
    orig.first = orig.second = 0;

    transformation< PictureT > traf(0, orig, orig, 0);
    std::pair < int, int > ori, vec, bestOri, bestVec;
    double bestAng, bestCorr, tempCorr;

    bestOri = trans.getOrigin();
    bestVec = trans.getVector();
    bestAng = trans.getAngle();
    bestCorr = -2;

    double anglestep = asin((double)2 / std::max((int)secondPic.getH(), (int)secondPic.getW()));
    ori = trans.getOrigin();

    for (int vecx = trans.getVector().first - 1; vecx < trans.getVector().first + 2; ++vecx)
      for (int vecy = trans.getVector().second - 1; vecy < trans.getVector().second + 2; ++vecy)
        // Since the best guess for the angle is the previous one, we start with that only go in the direction
        // where there is an improvement. This takes some logic.
        for (double ang = trans.getAngle() - 2 * anglestep; ang < trans.getAngle() + 3 * anglestep; ang += anglestep / 2)
        {
          traf.setAngle(ang);
          traf.setOrigin(ori);
          vec.first = vecx;
          vec.second = vecy;
          traf.setVector(vec);

          if (bestCorr < (tempCorr = transformationCorrelation(firstPic, secondPic, traf)))
          {
            bestOri = ori;
            bestVec = vec;
            bestAng = ang;
            bestCorr = tempCorr;
          }
        }
    transformation< PictureT > bestTraf(bestAng, bestOri, bestVec, 0);
    // dump the two pics
    firstPic.write("/tmp/1.ppm");
    PictureT sp;
    trans(secondPic, false, sp);
    sp.write("/tmp/2.ppm");
    return bestTraf;

  }

  template < class PictureT >
  //transformation< PictureT > findBest(PictureT & firstPic, PictureT & secondPic);
  transformation< PictureT > findBest(PictureT & firstPic, PictureT & secondPic)
  {
    ppmArray< PictureT > firstPics(firstPic, 8);
    ppmArray< PictureT > secondPics(secondPic, 8);
    size_t depth = firstPics.howManyLevels();
    std::cout << depth << "   ";
    firstPics.getLastPic().write("/tmp/1.ppm");
    secondPics.getLastPic().write("/tmp/2.ppm");
    transformation< PictureT > goodtraf = bruteForceBase(firstPics.getLastPic(), secondPics.getLastPic());
    goodtraf.setAll(0, 0, 0, 0, 0);
    goodtraf.setLevel(depth - 1);
    for (size_t i = 1; (int)i < (int)depth; ++i)
    {
      std::cout << i << " ";
      std::cout.flush();
      goodtraf.goOut(2);
      goodtraf = oneStep(firstPics.getPic(depth - 1 - i), secondPics.getPic(depth - 1 - i), goodtraf);
      //    pic
      goodtraf.setLevel(depth - 1 - i);
    }
    // write out pics for debugging purposes
    //  firstPics.writeAll();
    //  secondPics.writeAll();
    std::cout << "\n";
    return goodtraf;
  }






} //namespace imageRegistration

#endif
