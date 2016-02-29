#include <algorithm>
#include <cmath>

#include "pixel.h"

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

  /// Find the maximum color level of a double rgb pixel.
  int lMax(drgb point)
  {
    return(int)(std::max(std::max(std::max(point.r, -point.r), std::max(point.g, -point.g)), std::max(point.b, -point.b)));
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
    return drgb (std::sqrt(a.r * b.r), std::sqrt(a.g * b.g), std::sqrt(a.b * b.b));
  }

  void drgb::piecewiseDiv(drgb& a, const drgb& b)
  {
    if (b.r * b.g * b.b == 0.0)
      throw("division with 0, exiting");
    a.r /= b.r;
    a.g /= b.g;
    a.b /= b.b;
    return;
  }

  dGray operator * (const dGray & a, const dGray & b)
  {
    dGray c(a);
    c.scale *= b.scale;
    return c;
  }

  dGray & operator += (dGray & a, const dGray & b)
  {
    a.scale += b.scale;
    return a;
  }

  dGray & operator -= (dGray & a, const dGray & b)
  {
    a.scale -= b.scale;
    return a;
  }

  dGray & operator /= (dGray & a, const int & b)
  {
    a.scale /= b;
    return a;
  }

  dGray & operator *= (dGray & a, const double & b)
  {
    a.scale *= b;
    return a;
  }

  /// Find the maximum color level of a double rgb pixel.
  int lMax(dGray point)
  {
    return(int)(std::max(-point.scale, point.scale));
  }

  void dGray::write(char & cr, char & cg, char & cb) const
  {
    cr = (char)(scale);
    cg = (char)(scale);
    cb = (char)(scale);
  }

  double dGray::gray() const
  {
    return scale;
  }

  dGray dGray::geomMean(const dGray& a, const dGray& b)
  {
    return dGray(std::sqrt(a.scale * b.scale));
  }

  void dGray::piecewiseDiv(dGray& a, const dGray& b)
  {
    if (b.scale == 0.0)
      throw("division with 0, exiting");
    a.scale /= b.scale;
    return;
  }

}
