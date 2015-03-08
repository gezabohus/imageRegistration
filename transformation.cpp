//
//  transformation.cpp
//  imageRegistration
//
//  Created by Geza Bohus on 12/27/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <math.h>
#include "ppm.h"
#include "transformation.h"

namespace imageRegistration
{
  
std::pair < int, int > transformation::_origin = std::pair < int, int >(0,0);
void transformation::setAll(double a_, int ox_, int oy_, int vx_, int vy_)
{
  _angle = a_;
  if(std::abs(_angle > 1)) throw;
  //  _origin.first = ox_;
//  _origin.second = oy_;
  _vector.first = vx_;
  _vector.second = vy_;
}

ppm transformation::operator() (const ppm & image_, bool smooth, ppm & target_) const
{
  int h(image_.getH());
  int w(image_.getW());
  target_.setSize(w, h);
  drgb gray(127);
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
      if(std::abs(int(x-i)) > 50 || std::abs(int(y-j)) > 50)
        throw;
      if( smooth)
      {
        if((x >= 0) && (x < h-1) && (y >=0) && (y < w-1) )
        {
          int xl(x), xh(ceil(x)), yl(y), yh(ceil(y));
          drgb colorll = image_.getColor(xl, yl);
          drgb colorlh = image_.getColor(xl, yh);
          drgb colorhl = image_.getColor(xh, yl);
          drgb colorhh = image_.getColor(xh, yh);
          double wll = (1 - x + xl)*(1 - y + yl);
          double wlh = (1 - x + xl)*(1 + y - yh);
          double whl = (1 + x - xh)*(1 - y + yl);
          double whh = (1 + x - xh)*(1 + y - yh);
          drgb color = colorll * wll + colorlh * wlh + colorhl * whl + colorhh * whh;
          target_.setColor(i, j, color);
        }
        else
        {
          target_.setColor(i, j, gray);
        }
      }
      else
      {
        if( (x >= 0) && (x < h) && (y >=0 ) && (y < w) )
        {
          drgb color = image_.getColor((int)x, (int)y);
          target_.setColor(i, j, color);
        }
        else
        {
          target_.setColor(i, j, gray);
        }
      }
    }
  return target_;
}

  std::pair < int, int > transformation::operator() (const std::pair < size_t, size_t > & point_) const
  {
    std::pair < int, int > point;
    double c(cos(_angle));
    double s(sin(_angle));
    point.first  = (int) ((((int)point_.first - _origin.first)) *        c + (((int)point_.second - _origin.second)) *  s + _origin.first + _vector.first + 0.5);
    point.second = (int) ((((int)point_.first - _origin.first)) * (-1) * s + (((int)point_.second - _origin.second)) *  c + _origin.second + _vector.second + 0.5);
    return point;
  }
  
  std::vector <int > transformation::operator() (const std::vector  <int > & point_) const
  {
    std::vector < int> point(2);
    double c(cos(_angle));
    double s(sin(_angle));
    point[0] = (((point_[0] - _origin.first)) *        c + ((point_[1] - _origin.second)) *  s + _origin.first + _vector.first + 0.5);
    point[1] = (((point_[0] - _origin.first)) * (-1) * s + ((point_[1] - _origin.second)) *  c + _origin.second + _vector.second + 0.5);
    return point;
  }
  
transformation transformation::inverse ()
{
	double angle;
  std::pair < int, int > origin;
  std::pair < int, int > vector;
	size_t level;
	angle = -_angle;
	origin = _origin;
	vector.first  = (int) (-_vector.first * cos(_angle) + _vector.second * sin(_angle));
	vector.second = (int) (-_vector.first * sin(_angle) - _vector.second * cos(_angle));
	level = _level;
	transformation transf(angle, origin, vector, level);
	return transf;
}

void transformation::goOut(size_t scale_)
{
	if (_level != 0)
	{
	  --_level;
	  _origin.first   *= scale_;
	  _origin.second  *= scale_;
	  _vector.first  *= scale_;
	  _vector.second *= scale_;
	}
	else 
	{
		printf("Already at level 0!\n");
	}
}

  /// oldschool write
void transformation::write(std::string fn_) const
{
  std::FILE *file;
  file = std::fopen(fn_.c_str(), "wb");
  fwrite(&_angle, sizeof(double), 1, file);
  fwrite(&_origin.first, sizeof(int), 1, file);
  fwrite(&_origin.second, sizeof(int), 1, file);
  fwrite(&_vector.first, sizeof(int), 1, file);
  fwrite(&_vector.second, sizeof(int), 1, file);
  fwrite(&_level, sizeof(size_t), 1, file);
}

void transformation::read(std::string fn_)
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

double transformationCorrelation(const ppm & firstPic, const ppm & secondPic, const transformation & transf_)
{
  
  // These will collect  (partial sums of) means and variances for the two images.
  drgb firstM(0), firstV(0), secondM(0), secondV(0), coVar(0), corr(0);
	int numPoints = 0;
  std::vector<int> ijpoint(2); // this will index the second image
  int &i(ijpoint[0]);
  int &j(ijpoint[1]);
  std::vector<int> point(2);
  size_t h(secondPic.getH()), w(secondPic.getW());
	for (i = 0.1*h; i < .9*h; ++i)
		for (j = 0.1*w; j < .9*w ; ++j)
		{
      //point.first  = (int) ((((int)i - _origin.first)) * cos(_angle) + (((int)j - _origin.second)) * (-1) * sin(_angle) + _origin.first - _vector.first + 0.5);
      //point.second = (int) ((((int)i - _origin.first)) * sin(_angle) + (((int)j - _origin.second)) *        cos(_angle) + _origin.second - _vector.second + 0.5);
      point = transf_(ijpoint);
      if( (point[0] >= 0) && (point[0] < (int) secondPic.getH()) && (point[1] >=0 ) && (point[1] < (int) secondPic.getW()) )
      {
        drgb fc, sc;
        firstM  += (fc = firstPic.getColor(point));
        secondM += (sc = secondPic.getColor(i, j));
        firstV  += fc * fc;
        secondV += sc * sc;
        coVar   += fc * sc;
        ++numPoints;
      }
		}
	if(numPoints == 0)
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
		corr.r /= pow((double) (firstV.r * secondV.r), 0.5);
		corr.g /= pow((double) (firstV.g * secondV.g), 0.5);
		corr.b /= pow((double) (firstV.b * secondV.b), 0.5);
	}
	return( (corr.r + corr.g + corr.b) / 3 );
}

transformation bruteForceBase(const ppm & firstPic, const ppm & secondPic)
{
  firstPic.write("/tmp/1.ppm");
  secondPic.write("/tmp/2.ppm");
  const int originXmin = 0; //(int) secondPic.getH() / 2;
  const int originYmin = 0; //(int) secondPic.getW() / 2;
  const int originXmax = originXmin + 1;
  const int originYmax = originYmin + 1;
  const int vectorXmin = -((int) secondPic.getH() / 5) - 1;
  const int vectorYmin = -((int) secondPic.getW() / 5) - 1;
  const int vectorXmax = (int) secondPic.getH() / 5 + 1;
  const int vectorYmax = (int) secondPic.getW() / 5 + 1;
  double angleMin = -.3;
  double angleMax = .3;
  double anglestep = asin((double) 2 / __max((int) secondPic.getH(), (int) secondPic.getW()));
  
  std::pair < int, int > orig(0,0);
  
  transformation traf(0, orig, orig, 0);
  std::pair < int, int > bestOri(originXmin, originYmin), bestVec(vectorXmin, vectorYmin);
  double bestAng(angleMin), bestCorr, tempCorr;
  
  bestAng = angleMin;
  bestCorr = -2;
  
  for( int orix = originXmin; orix < originXmax; ++orix )
	  for( int oriy = originYmin; oriy < originYmax; ++oriy )
		  for ( int vecx = vectorXmin; vecx < vectorXmax; ++vecx )
        for ( int vecy = vectorYmin; vecy < vectorYmax; ++vecy )
          // we want to allow angle to be zero and it's a double, so we go from zero to up and down
          for ( double ang = 0.; ang < angleMax + anglestep; ang += anglestep ) // these are not ints
          {
            traf.setAll(ang, orix, oriy, vecx, vecy);
            if (bestCorr < (tempCorr = transformationCorrelation(firstPic, secondPic, traf)))
            {
				  	  bestOri = std::pair < int, int > (orix, oriy);
						  bestVec = std::pair < int, int > (vecx, vecy);
						  bestAng = ang;
						  bestCorr = tempCorr;
            }
            
            traf.setAll(-ang, orix, oriy, vecx, vecy);
            if (bestCorr < (tempCorr = transformationCorrelation(firstPic, secondPic, traf)))
            {
				  	  bestOri = std::pair < int, int > (orix, oriy);
						  bestVec = std::pair < int, int > (vecx, vecy);
						  bestAng = -ang;
						  bestCorr = tempCorr;
            }
            
          }
  transformation bestTraf(bestAng, bestOri, bestVec, 0);
  return bestTraf;
}

transformation oneStep (const ppm & firstPic, const ppm & secondPic, const transformation & trans)
{
  std::pair < int, int > orig;
  orig.first = orig.second = 0;
  
  transformation traf(0, orig, orig, 0);
  std::pair < int, int > ori, vec, bestOri, bestVec;
  double bestAng, bestCorr, tempCorr;
  
  bestOri = trans.getOrigin();
  bestVec = trans.getVector();
  bestAng = trans.getAngle();
  bestCorr = -2;
  
  double anglestep = asin((double) 2 / __max((int) secondPic.getH(), (int) secondPic.getW()));
  ori = trans.getOrigin();
	
	for ( int vecx = trans.getVector().first - 1; vecx < trans.getVector().first + 2; ++vecx )
    for ( int vecy = trans.getVector().second - 1; vecy < trans.getVector().second + 2; ++vecy )
      // Since the best guess for the angle is the previous one, we start with that only go in the direction
      // where there is an improvement. This takes some logic.
      for ( double ang = trans.getAngle() -2 * anglestep; ang < trans.getAngle() + 3 * anglestep; ang += anglestep / 2 )
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
	transformation bestTraf(bestAng, bestOri, bestVec, 0);
  // dump the two pics
  firstPic.write("/tmp/1.ppm");
  ppm sp;
  trans(secondPic, false, sp);
  sp.write("/tmp/2.ppm");
	return bestTraf;
  
}

transformation findBest(ppm & firstPic, ppm & secondPic)
{
	ppmArray firstPics(firstPic, 8);
	ppmArray secondPics(secondPic, 8);
	size_t depth = firstPics.howManyLevels();
	std::cout << depth << "   ";
//  firstPics.getLastPic().write("/tmp/1.ppm");
//  secondPics.getLastPic().write("/tmp/2.ppm");
	transformation goodtraf = bruteForceBase(firstPics.getLastPic(), secondPics.getLastPic());
	goodtraf.setLevel(depth-1);
	for (size_t i = 1; (int) i < (int) depth; ++i)
	{
    std::cout << i << " ";
    std::cout.flush();
    goodtraf.goOut(2);
    goodtraf = oneStep(firstPics.getPic(depth-1-i), secondPics.getPic(depth-1-i), goodtraf);
//    pic
    goodtraf.setLevel(depth-1-i);
	}
  // write out pics for debugging purposes
//  firstPics.writeAll();
//  secondPics.writeAll();
	std::cout << "\n";
	return goodtraf;
}
  
} // namespace imageRegistration
