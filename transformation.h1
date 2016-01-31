//
//  transformation.h
//  imageRegistration
//
//  Created by Geza Bohus on 12/27/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//


#ifndef imageRegistration_transformation_h
#define imageRegistration_transformation_h

#include <utility>

namespace imageRegistration
{

  /// A transformation is a translation and rotation of the plane.
class transformation
{
public:
  void setAngle(double angle_) { _angle = angle_; }
  void setOrigin(std::pair < int, int > origin_) { _origin.first = origin_.first; _origin.second = origin_.second; }
  void setVector(std::pair < int, int > vector_) { _vector.first = vector_.first; _vector.second = vector_.second; }
  void setLevel(size_t level_) { _level = level_; }
  void setAll(double a_, int ox_, int oy_, int vx_, int vy_);
  double getAngle() { return _angle; }
  std::pair < int, int > getOrigin() const { return _origin; }
  std::pair < int, int > getVector() const { return _vector; }
  size_t getLevel() const { return _level; }
  void getTraf() { std::cout << _angle << ", (" << _origin.first << ", " << _origin.second << "), (" << _vector.first << ", " << _vector.second << ")\n"; }
  transformation( double angle_, std::pair < int , int > origin_, std::pair < int , int > vector_, size_t level_ ) :
  _angle(angle_), _origin(origin_), _vector(vector_), _level(level_){ }
  // transformation();
  //    ~transformation();

  /// Inverse transform an image by rotating around _origin and translating by _vector. 
  /// The source image is 
  ppm operator() (const ppm & image_ ) const ; 
  
  /// Transform a point by rotating it around the _origin and translating by _vector. Does not
  /// check if we're outside of the image. Output the new coordinates.
  std::pair < int, int > operator() (const std::pair < size_t, size_t > &) const ;
  transformation inverse();
  void goOut(size_t scale_);
  void write(std::string fn_) const ;
  void read(std::string fn_);

private:
  double _angle;
  /*static*/ std::pair < int, int > _origin;
  std::pair < int, int > _vector;
  size_t _level;
};

double transformationCorrelation(const ppm & firstPic, const ppm & secondPic, transformation transf_);

transformation bruteForceBase(const ppm & firstPic, const ppm & secondPic);
transformation oneStep (const ppm & firstPic, const ppm & secondPic, transformation trans);
transformation findBest(ppm & firstPic, ppm & secondPic);

} //namespace imageRegistration

#endif
