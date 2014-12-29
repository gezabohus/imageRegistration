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
  
//  std::pair < int, int > transformation::_origin = std::pair < int, int >(0,0);
void transformation::setAll(double a_, int ox_, int oy_, int vx_, int vy_)
{
  _angle = a_;
  _origin.first = ox_;
  _origin.second = oy_;
  _vector.first = vx_;
  _vector.second = vy_;
}

ppm transformation::operator() (const ppm & image_ ) const
{
  ppm image(image_.getW(), image_.getH());
  drgb gray(27);
  double c(cos(_angle));
  double s(sin(_angle));
  
  for (size_t i = 0; i < image.getH(); ++i)
    for (size_t j = 0; j < image.getW(); ++j)
    {
      std::pair < int, int > point, imagePoint;
      std::vector< int > rPoint(2);
      imagePoint.first = (int)i;
      imagePoint.second = (int)j;
      point.first  = (int) ((((int)i - _origin.first)) * c + (((int)j - _origin.second)) * (-1) * s + _origin.first - _vector.first + 0.5);
      point.second = (int) ((((int)i - _origin.first)) * s + (((int)j - _origin.second)) *        c + _origin.second - _vector.second + 0.5);
      if( (point.first >= 0) && (point.first < (int) image_.getH()) && (point.second >=0 ) && (point.second < (int) image_.getW()) )
      {
        rPoint[0] = point.first;
        rPoint[1] = point.second;
        drgb color = image_.getColor(rPoint);
        image.reWritePoint( imagePoint, color );
      }
      else
      {
        image.reWritePoint( imagePoint, gray );
      }
    }
  return(image);
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

double transformationCorrelation(const ppm & firstPic, const ppm & secondPic, transformation transf_)
{
	drgb firstM(0), firstV(0), secondM(0), secondV(0), coVar(0), corr(0);
	int howManyPoints = 0;
	for (size_t i = 0; i < secondPic.getH(); ++i)
		for (size_t j = 0; j < secondPic.getW(); ++j)
		{
      std::pair < size_t, size_t > ijpoint;
      ijpoint.first = i;
      ijpoint.second = j;
      std::pair < int, int > point;
      std::vector< int > firstPoint(2), secondPoint(2);
      point = transf_(ijpoint);
      //point.first  = (int) ((((int)i - _origin.first)) * cos(_angle) + (((int)j - _origin.second)) * (-1) * sin(_angle) + _origin.first - _vector.first + 0.5);
      //point.second = (int) ((((int)i - _origin.first)) * sin(_angle) + (((int)j - _origin.second)) *        cos(_angle) + _origin.second - _vector.second + 0.5);
      if( (point.first >= 0) && (point.first < (int) secondPic.getH()) && (point.second >=0 ) && (point.second < (int) secondPic.getW()) )
      {
        secondPoint[0] = (int) i;
        secondPoint[1] = (int) j;
        firstPoint[0] = point.first;
        firstPoint[1] = point.second;
        firstM  += firstPic.getColor(firstPoint);
        secondM += secondPic.getColor(secondPoint);
        firstV  += firstPic.getColor(firstPoint) * firstPic.getColor(firstPoint);
        secondV += secondPic.getColor(secondPoint) * secondPic.getColor(secondPoint);
        coVar   += firstPic.getColor(firstPoint) * secondPic.getColor(secondPoint);
        ++howManyPoints;
      }
		}
	if(howManyPoints == 0)
	{
		std::cout << "A trafoban nincs metszet!!!\n";
	}
	else
	{
		firstM /= howManyPoints;
		secondM /= howManyPoints;
		firstV /= howManyPoints;
		secondV /= howManyPoints;
		firstV -= firstM * firstM;
		secondV -= secondM * secondM;
		coVar /= howManyPoints;
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
  const int originXmin = (int) secondPic.getH() / 2;
  const int originYmin = (int) secondPic.getW() / 2;
  const int originXmax = originXmin + 1;
  const int originYmax = originYmin + 1;
  const int vectorXmin = -((int) secondPic.getH() / 5) - 1;
  const int vectorYmin = -((int) secondPic.getW() / 5) - 1;
  const int vectorXmax = (int) secondPic.getH() / 5 + 1;
  const int vectorYmax = (int) secondPic.getW() / 5 + 1;
  double angleMin = -1;
  double angleMax = 1;
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
          // we want to allow angle to be zero and it's a double, so we g from zero to up and down
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

transformation oneStep (const ppm & firstPic, const ppm & secondPic, transformation trans)
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
      for ( double ang = -2 * anglestep; ang < 3 * anglestep; ang += anglestep / 2 )
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
	return bestTraf;
  
}

transformation findBest(ppm & firstPic, ppm & secondPic)
{
	ppmArray firstPics(firstPic, 8);
	ppmArray secondPics(secondPic, 8);
	size_t depth = firstPics.howManyLevels();
	std::cout << depth << "   ";
  firstPics.getLastPic().write("/tmp/1.ppm");
  secondPics.getLastPic().write("/tmp/2.ppm");
	transformation goodtraf = bruteForceBase(firstPics.getLastPic(), secondPics.getLastPic());
	goodtraf.setLevel(depth-1);
	for (size_t i = 1; (int) i < (int) depth; ++i)
	{
		std::cout << i << " ";
    std::cout.flush();
		goodtraf.goOut(2);
		goodtraf = oneStep(firstPics.getPic(depth-1-i), secondPics.getPic(depth-1-i), goodtraf);
		goodtraf.setLevel(depth-1-i);
	}
	std::cout << "\n";
	return goodtraf;
}
  
} // namespace imageRegistration
