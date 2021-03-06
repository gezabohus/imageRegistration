#include <algorithm>
#include <cmath>

#include "pixel.h"

using namespace std;

namespace imageRegistration
{

  //drgb operator + (const drgb & a, const drgb & b)
  //{
  //  drgb c(a);
  //  c.r += b.r;
  //  c.g += b.g;
  //  c.b += b.b;
  //  return c;
  //}

  //drgb operator - (const drgb & a, const drgb & b)
  //{
  //  drgb c(a);
  //  c.r -= b.r;
  //  c.g -= b.g;
  //  c.b -= b.b;
  //  return c;
  //}

  //drgb operator / (const drgb & a, int b)
  //{
  //  drgb c(a);
  //  c.r /= b;
  //  c.g /= b;
  //  c.b /= b;
  //  return c;
  //}

  //drgb operator - (const drgb & a, int b)
  //{
  //  drgb c(a);
  //  c.r -= b;
  //  c.g -= b;
  //  c.b -= b;
  //  return c;
  //}

  drgb operator * (const drgb & a, const drgb & b)
  {
    drgb c(a);
    c.r *= b.r;
    c.g *= b.g;
    c.b *= b.b;
    return c;
  }

  drgb & operator += (drgb & a, const drgb & b)
  {
    a.r += b.r;
    a.g += b.g;
    a.b += b.b;
    return a;
  }

  drgb & operator -= (drgb & a, const drgb & b)
  {
    a.r -= b.r;
    a.g -= b.g;
    a.b -= b.b;
    return a;
  }

  drgb & operator /= (drgb & a, const int & b)
  {
    a.r /= b;
    a.g /= b;
    a.b /= b;
    return a;
  }

  drgb & operator *= (drgb & a, const double & b)
  {
    a.r *= b;
    a.g *= b;
    a.b *= b;
    return a;
  }

  drgb& operator /= (drgb& a, const drgb& b)
  {
    if ((b.r == 0.0) || (b.g == 0.0) || (b.b == 0.0))
      throw("division with 0, exiting");
    a.r /= b.r;
    a.g /= b.g;
    a.b /= b.b;
    return a;
  }

  /// Find the maximum color level of a double rgb pixel.
  int lMax(drgb point)
  {
    return(int)(max(max(max(point.r, -point.r), max(point.g, -point.g)), max(point.b, -point.b)));
  }

  void drgb::write(char & cr, char & cg, char & cb) const
  {
    cr = (char)(r);
    cg = (char)(g);
    cb = (char)(b);
  }

  double drgb::gray() const
  {
    return (r + g + b) / 3.0;
  }

  drgb drgb::geomMean(const drgb& a, const drgb& b)
  {
    return drgb (sqrt(a.r * b.r), sqrt(a.g * b.g), sqrt(a.b * b.b));
  }

  //void drgb::piecewiseDiv(drgb& a, const drgb& b)
  //{
  //  if (b.r * b.g * b.b == 0.0)
  //    throw("division with 0, exiting");
  //  a.r /= b.r;
  //  a.g /= b.g;
  //  a.b /= b.b;
  //  return;
  //}

  dGray operator * (const dGray & a, const dGray & b)
  {
    dGray c(a);
    c.grayValue *= b.grayValue;
    return c;
  }

  dGray & operator += (dGray & a, const dGray & b)
  {
    a.grayValue += b.grayValue;
    return a;
  }

  dGray & operator -= (dGray & a, const dGray & b)
  {
    a.grayValue -= b.grayValue;
    return a;
  }

  dGray & operator /= (dGray & a, const int & b)
  {
    a.grayValue /= b;
    return a;
  }

  dGray & operator *= (dGray & a, const double & b)
  {
    a.grayValue *= b;
    return a;
  }

  dGray & operator /= (dGray& a, const dGray& b)
  {
    if (b.grayValue == 0.0)
      throw("division with 0, exiting");
    a.grayValue /= b.grayValue;
    return a;
  }

  /// Find the maximum color level of a double rgb pixel.
  int lMax(dGray point)
  {
    return(int)(max(-point.grayValue, point.grayValue));
  }

  void dGray::write(char & cr, char & cg, char & cb) const
  {
    cr = (char)(grayValue);
    cg = (char)(grayValue);
    cb = (char)(grayValue);
  }

  double dGray::gray() const
  {
    return grayValue;
  }

  dGray dGray::geomMean(const dGray& a, const dGray& b)
  {
    return dGray(sqrt(a.grayValue * b.grayValue));
  }

  //void dGray::piecewiseDiv(dGray& a, const dGray& b)
  //{
  //  if (b.grayValue == 0.0)
  //    throw("division with 0, exiting");
  //  a.grayValue /= b.grayValue;
  //  return;
  //}

}
