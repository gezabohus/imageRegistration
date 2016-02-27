#ifndef PIXEL_H
#define PIXEL_H
//struct rgb
//{
//  int r, g, b;
//  rgb(): r(0), g(0), b(0) {}
//  rgb(int barmi_): r(barmi_), g(barmi_), b(barmi_) {}
//};

namespace imageRegistration {

  /// A red-green-blue pixel. Levels are doubles!
struct drgb
{
  double r, g, b;
  drgb() : r(0), g(0), b(0) {}
  drgb(double barmi_) : r(barmi_), g(barmi_), b(barmi_) {}
  drgb(double ar_, double ag_, double ab_) : r(ar_), g(ag_), b(ab_) {}
  void write(char& cr, char& cg, char& cb) const;
  double gray() const;
  static drgb geomMean(const drgb& a, const drgb& b);
  static void piecewiseDiv(drgb& a, const drgb& b);
};

drgb operator * (const drgb & a, const drgb & b);
//drgb operator + (const drgb & a, const drgb & b);
//drgb operator - (const drgb & a, const drgb & b);
//drgb operator / (const drgb & a, int b);
drgb & operator += (drgb & a, const drgb & b);
drgb & operator -= (drgb & a, const drgb & b);
drgb & operator *= (drgb & a, const double & b);
drgb & operator /= (drgb & a, const int & b);
int lMax(drgb point);

struct dGray
{
  double scale;
  dGray() : scale(0) {}
  dGray(double scale_) : scale(scale_) {}
  dGray(double ar_, double ag_, double ab_, double wr_ = 1.0, double wg_ = 1.0, double wb_ = 1.0) : scale( (ar_ * wr_ + ag_ * wg_ + ab_ * wb_) / (wr_ + wg_ + wb_) ) {}
  void write(char& cr, char& cg, char& cb) const;
  double gray() const;
  static dGray geomMean(const dGray& a, const dGray& b);
  static void piecewiseDiv(dGray& a, const dGray& b);
};

dGray operator * (const dGray & a, const dGray & b);
//dGray operator + (const dGray & a, const dGray & b);
//dGray operator - (const dGray & a, const dGray & b);
//dGray operator / (const dGray & a, int b);
dGray & operator += (dGray & a, const dGray & b);
dGray & operator -= (dGray & a, const dGray & b);
dGray & operator *= (dGray & a, const double & b);
dGray & operator /= (dGray & a, const int & b);
int lMax(dGray point);

} // namespace
#endif
