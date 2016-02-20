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

  //drgb operator * (const drgb & a, const drgb & b)
  //{
  //  drgb c(a);
  //  c.r *= b.r;
  //  c.g *= b.g;
  //  c.b *= b.b;
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
    return(int)(__max(__max(__max(point.r, -point.r), __max(point.g, -point.g)), __max(point.b, -point.b)));
  }

}