#include <iostream>
#include <stdlib.h>
#include "Optimizer.h"
#include "ppm.h"
#include "utilities.h"
#include "math.h"

namespace imageRegistration
{

   
  Optimizer::Optimizer(const ppm & l_, const ppm & r_) :
    _leftImage(l_), _rightImage(r_)
  {
  }

  Optimizer::~Optimizer(void)
  {

  }
 
//  size_t Optimizer::search (size_t where_[2], std::vector < int > & holmax_, double & var_)
//  {
//
//    if ( (_leftImage._w - 2 * _margin < 3 * _sampleSize ) || (_leftImage._h - 2 * _margin < 3 * _sampleSize) )
//      return(1);
//
//    if ((where_[0] > 2) || (where_[1] > 2) )
//      return(2);
//
//    size_t i, j, k, l;
//    std::vector< drgb > sampleImage(_sampleSize * _sampleSize);
//    double maxVar = 0;
//    size_t harmadw = (_leftImage._w - 2 * _margin)/3;
//    size_t harmadh = (_leftImage._h - 2 * _margin)/3;
//    double var;
//    int holmax[2];
//    bool egyszinu = true;
//
//    std::cout << where_[0] << ", " << where_[1] << std::endl;
//    for (k = _margin + where_[0] * harmadw; k < _margin + (where_[0] + 1) * harmadw - _sampleSize; k += 1)
//      for (l = _margin + where_[1] * harmadh; l < _margin + (where_[1] + 1) * harmadh - _sampleSize; l += 1)
//      {
//        for (i=0; i<_sampleSize; ++i)
//          for (j=0; j<_sampleSize; ++j)
//          {
//            sampleImage[i * _sampleSize + j] = _leftImage._dataInt[l+j][k+i];
//          };
//        if(maxVar < (var = coVar(sampleImage, sampleImage).r + coVar(sampleImage, sampleImage).g 
//                     + coVar(sampleImage, sampleImage).b /*= lMax(coVar(sampleImage, sampleImage))*/))
//        {
//          maxVar = var;
//          holmax[0] = l+i;
//          holmax[1] = k+j;
//          egyszinu = false;
//        }
//      };
//    var_ = maxVar;
//    if (egyszinu)
//    {
//      holmax_[0] = l;
//      holmax_[1] = k;
//    }
//    else
//    {
//      holmax_[0] = holmax[0];
//      holmax_[1] = holmax[1];
//    }
//    return(0);
//  }
//
//  void Optimizer::search (std::vector < std::vector < int > > & holmax_, std::vector < double > & var_)
//  {
//    int i, j, k=0;
//    std::vector < double > var(9);
//    std::vector < int > dummy(2);
//    std::vector < std::vector < int > > holmax(9, dummy);
//    size_t hol[2];
//    for(i=0; i<3; ++i)
//      for(j=0; j<3; ++j)
//      {
//        hol[0] = i;
//        hol[1] = j;
//        search(hol, holmax[k], var[k]);
//        ++k;
//      }
//    var_ = var;
//    holmax_ = holmax;
//  }

//  void Optimizer::searchForMatch (std::vector < int > & holmax_, size_t radius_, std::vector < int > & holmax2_, double & mennyimax_)
//  {
//    std::pair < int, int > holmax;
//    holmax.first = holmax_[0] - _sampleSize/2;
//    holmax.second = holmax_[1] - _sampleSize/2;
//    std::pair < int, int > sampleSize;
//    sampleSize.first = sampleSize.second = _sampleSize;
//    size_t i, j;
//    int x, y;
//    std::pair < int, int > r;
//    double m=0, corv;
//    int xmax, ymax;
//    for (i = 0; i < radius_; ++i)
//      for (j = 0; j < 2 * i + 1; ++j)
//      {
//        //upper edge
//        x = holmax.first + j - radius_ + 1;
//        y = holmax.second - i;
//        r.first = x;
//        r.second = y;
//        corv = ppm::corr(_leftImage, _rightImage, holmax, r, sampleSize);
//        if (m < corv) {m=corv; xmax = x; ymax = y;}
//        //lower edge
//        x = holmax.first + j - radius_ + 1;
//        y = holmax.second + i;
//        r.first = x;
//        r.second = y;
//        corv = ppm::corr(_leftImage, _rightImage, holmax, r, sampleSize);
//        if (m < corv) {m=corv; xmax = x; ymax = y;}
//        //left edge
//        x = holmax.first - i;
//        y = holmax.second + j - radius_ + 1;
//        r.first = x;
//        r.second = y;
//        corv = ppm::corr(_leftImage, _rightImage, holmax, r, sampleSize);
//        if (m < corv) {m=corv; xmax = x; ymax = y;}
//        //right edge
//        x = holmax.first + i;
//        y = holmax.second + j - radius_ + 1;
//        r.first = x;
//        r.second = y;
//        corv = ppm::corr(_leftImage, _rightImage, holmax, r, sampleSize);
//        if (m < corv) {m=corv; xmax = x; ymax = y;}
//      }
////      std::cout << m << "\n";
//      mennyimax_ = m;
//      holmax2_[0] = xmax;
//      holmax2_[1] = ymax;
//  }

//  ppm Optimizer::positioner (void)
//  {
//    std::vector < int > dummy(2);
//    std::vector < std::vector < int > > holmax(9, dummy);
//    std::vector < double > var(9);
//    this->search(holmax, var);
//    std::vector < std::vector < int > > holmax2(9, dummy);
//    std::vector < double > mennyimax(9);
//    for (int i=0; i<9; ++i)
//      this->searchForMatch(holmax[i], _sampleSize*2, holmax2[i], mennyimax[i]);
//    std::vector < std::vector < int > > holmaxAholJoBal(9, dummy);
//    std::vector < std::vector < int > > holmaxAholJoJobb(9, dummy);
//    int jokSzama = 0;
//      int ratio = 100;
//      while(jokSzama < 3)
//      {
//      ratio -= 1;
//      jokSzama = 0;
//        for(int i = 0; i<9; ++i)
//          {
//        if((i == 4) || (i == 1) || (i == 3) || (i == 5) || (i == 7)) continue;
//              if(mennyimax[i] > 0.01 * (double)ratio)
//              {
//                  holmaxAholJoBal[jokSzama] = holmax[i];
//                  holmaxAholJoJobb[jokSzama] = holmax2[i];
//                  ++jokSzama;
//              }
//          }
//      }
//    std::cout << "3arany kozul legkisebb: " << ratio << "\n";
//    holmaxAholJoBal.resize(jokSzama);
//    holmaxAholJoJobb.resize(jokSzama);
//    std::cout << "legnagyobb terulet?\n";
//    int threeBest[3];
//#define elso threeBest[0]
//#define masodik threeBest[1]
//#define harmadik threeBest[2]
//    elso = -1;
//    masodik = -1;
//    harmadik = -1;
//   
//    int maxT = -1;
//    int T;
//    for (int i = 0; i<jokSzama; ++i)
//      for (int j = i+1; j<jokSzama; ++j)
//        for (int k = j+1; k<jokSzama; ++k)
//        {
//          T = abs((holmaxAholJoBal[j][0]-holmaxAholJoBal[i][0]) * (holmaxAholJoBal[k][1]-holmaxAholJoBal[i][1]) - (holmaxAholJoBal[j][1]-holmaxAholJoBal[i][1]) * (holmaxAholJoBal[k][0]-holmaxAholJoBal[i][0]));
//          if ( T > maxT )
//          {
//            maxT = T;
//            elso = i;
//            masodik = j;
//            harmadik = k;
//          }
//        }
//    std::vector<double> B1(2), B2(2), B3(2), J1(2), J2(2), J3(2), O1(2), O2(2);
//    O1[0] = (double) (holmaxAholJoBal[elso][0] + holmaxAholJoBal[masodik][0] + holmaxAholJoBal[harmadik][0])/3;
//    O1[1] = (double) (holmaxAholJoBal[elso][1] + holmaxAholJoBal[masodik][1] + holmaxAholJoBal[harmadik][1])/3;
//    O2[0] = (double) (holmaxAholJoJobb[elso][0] + holmaxAholJoJobb[masodik][0] + holmaxAholJoJobb[harmadik][0])/3;
//    O2[1] = (double) (holmaxAholJoJobb[elso][1] + holmaxAholJoJobb[masodik][1] + holmaxAholJoJobb[harmadik][1])/3;
//    B1[0] = (double) holmaxAholJoBal[elso][0] - O1[0];
//    B1[1] = (double) holmaxAholJoBal[elso][1] - O1[1];
//    B2[0] = (double) holmaxAholJoBal[masodik][0] - O1[0];
//    B2[1] = (double) holmaxAholJoBal[masodik][1] - O1[1];
//    B3[0] = (double) holmaxAholJoBal[harmadik][0] - O1[0];
//    B3[1] = (double) holmaxAholJoBal[harmadik][1] - O1[1];
//    J1[0] = (double) holmaxAholJoJobb[elso][0] - O2[0];
//    J1[1] = (double) holmaxAholJoJobb[elso][1] - O2[1];
//    J2[0] = (double) holmaxAholJoJobb[masodik][0] - O2[0];
//    J2[1] = (double) holmaxAholJoJobb[masodik][1] - O2[1];
//    J3[0] = (double) holmaxAholJoJobb[harmadik][0] - O2[0];
//    J3[1] = (double) holmaxAholJoJobb[harmadik][1] - O2[1];
//    std::cout << "2. kep trafoja\n";
//    double g1, g2, g3;
//    g1 = acos(((B1[0]*B1[0]+B1[1]*B1[1]) + (J1[0]*J1[0]+J1[1]*J1[1]) - (J1[0]-B1[0])*(J1[0]-B1[0]) - (J1[1]-B1[1])*(J1[1]-B1[1])) / (2*sqrt(B1[0]*B1[0]+B1[1]*B1[1])*sqrt(J1[0]*J1[0]+J1[1]*J1[1])));
//    g2 = acos(((B2[0]*B2[0]+B2[1]*B2[1]) + (J2[0]*J2[0]+J2[1]*J2[1]) - (J2[0]-B2[0])*(J2[0]-B2[0]) - (J2[1]-B2[1])*(J2[1]-B2[1])) / (2*sqrt(B2[0]*B2[0]+B2[1]*B2[1])*sqrt(J2[0]*J2[0]+J2[1]*J2[1])));
//    g3 = acos(((B3[0]*B3[0]+B3[1]*B3[1]) + (J3[0]*J3[0]+J3[1]*J3[1]) - (J3[0]-B3[0])*(J3[0]-B3[0]) - (J3[1]-B3[1])*(J3[1]-B3[1])) / (2*sqrt(B3[0]*B3[0]+B3[1]*B3[1])*sqrt(J3[0]*J3[0]+J3[1]*J3[1])));
//    if(B1[0]*J1[1]-B1[1]*J1[0] < 0) g1 *= -1;
//    if(B2[0]*J2[1]-B2[1]*J2[0] < 0) g2 *= -1;
//    if(B3[0]*J3[1]-B3[1]*J3[0] < 0) g3 *= -1;
//    double gm = (g1+g2+g3)/3;
//    ppm transformed(_leftImage);  //<-not needed, but the easiest way at this point...
//    drgb gray(27), red(255, 0, 0);
//    for(size_t i = 0; i < _leftImage._h; ++i)
//      for(size_t j = 0; j < _leftImage._w; ++j)
//      {
//        std::pair<int, int> point, redpoint;
//        std::vector< int > point2(2);
//        std::vector< double > point3(2);
//        point.first = i;
//        point.second = j;
//        point3[0] = ((double) i - O1[0]) * cos(gm) + ((double) j - O1[1]) * (-1) * sin(gm) + O2[0];
//        point3[1] = ((double) i - O1[0]) * sin(gm) + ((double) j - O1[1]) * cos(gm) + O2[1];
//        point2[0] = (int) (point3[0] + 0.5);
//        point2[1] = (int) (point3[1] + 0.5);
//        if( (point2[0] >= 0) && (point2[0] < (int)_rightImage._h) && (point2[1] >=0) && (point2[1] < (int)_rightImage._w) )
//        {
//          drgb color = _rightImage.getColor(point2);
//          transformed.reWritePoint( point, color );
//        }
//        else
//        {
//          transformed.reWritePoint( point, gray );
//        }
//        // make a red mark where the best region was found
//        for (size_t k = 0; k< _sampleSize; ++k)
//          //for (int l=0; l+k<10; ++l)
//          {
//            for (int tbInd = 0; tbInd < 3; ++tbInd)
//            {
//              redpoint.first = holmaxAholJoJobb[threeBest[tbInd]][0] + k;
//              redpoint.second = holmaxAholJoJobb[threeBest[tbInd]][1];
//              transformed.reWritePoint( redpoint, red );
//              redpoint.second = holmaxAholJoJobb[threeBest[tbInd]][1] + _sampleSize;
//              transformed.reWritePoint( redpoint, red );
//              redpoint.first = holmaxAholJoJobb[threeBest[tbInd]][0];
//              redpoint.second = holmaxAholJoJobb[threeBest[tbInd]][1] + k;
//              transformed.reWritePoint( redpoint, red );
//              redpoint.first = holmaxAholJoJobb[threeBest[tbInd]][0] + _sampleSize;
//              transformed.reWritePoint( redpoint, red );
//            }
////            redpoint.first = holmaxAholJoJobb[masodik][0]+k;
////            redpoint.second = holmaxAholJoJobb[masodik][1]+l;
////            transformed.reWritePoint( redpoint, red );
////            redpoint.first = holmaxAholJoJobb[harmadik][0]+k;
////            redpoint.second = holmaxAholJoJobb[harmadik][1]+l;
////            transformed.reWritePoint( redpoint, red );
//          }
//
////        std::cout << i << ", " << j << "\n";
//      }
//    return transformed;
//  }


  void Optimizer::setSize (int n_)
  {
    _sampleSize = n_;
  }
  void Optimizer::setMargin (int n_)
  {
    _margin = n_;
  }
  
  double Optimizer::similarity() const
  {
    
    return absAverageDiff(_leftImage._dataInt, _rightImage._dataInt);
  }
  
} //namespace imageRegistration
