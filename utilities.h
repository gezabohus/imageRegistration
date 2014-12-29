#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <vector>
#include <assert.h>

namespace imageRegistration
{
template <class T>
T mean(const std::vector<T> & v)
{
  class std::vector<T>::const_iterator it = v.begin();
  T m(0);
  for(; it != v.end(); ++it)
    m = m + *it;

  return m/v.size();
}

template <class T>
T coVar(const std::vector<T> & v, const std::vector<T> & w)
{
  size_t sv = v.size(); 
  assert(sv == w.size());
  T cV(0);
  for(size_t it = 0; it < sv; ++it)
    cV = cV + v[it] * w[it];
  cV = cV / sv;
  cV = cV - mean(v) * mean(w);

  return cV;
}

template <class T>
double corr(const std::vector<T> & v, const std::vector<T> & w)
{
  size_t sv = v.size(); 
  assert(sv == w.size());

  double corr = coVar(v, w);
  corr /= pow(coVar(v) * coVar(w), 1/2);

  return corr;
}


  /// Insert tr_ at the beginning of a filename, even if given by full path. (Slash delimited.)
std::string transformString(const std::string & s_, const char * tr_ = "_tr_");


  template <class T>
  double absAverageDiff(const T & t0, const T & t1)
  {
    assert(t0.size() == t1.size());
    
    typename T::const_iterator it0(t0.begin());
    typename T::const_iterator it1(t1.begin());
    double sum(0);
    for(; it0 != t0.end(); ++it0)
      sum += absAverageDiff(*it0, *it1);
    return sum/t0.size();
  }
  
  template <>
  double absAverageDiff<double>(const double & t0, const double & t1);
  template <>
  double absAverageDiff<drgb>(const drgb & t0, const drgb & t1);
  
} //namespace imageRegistration


#endif // _UTILITIES_H_

