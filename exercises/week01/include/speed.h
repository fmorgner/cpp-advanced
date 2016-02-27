#ifndef __SPEED_SPEED_H
#define __SPEED_SPEED_H

#include "distance.h"

namespace speed
  {
  constexpr double operator""_ms(long double x)
    {
    return x;
    }

  constexpr double operator""_ms(unsigned long long x)
    {
    return x;
    }

  constexpr double operator""_kmh(long double x)
    {
    return distance_in_meters::operator""_km(x) / 3600;
    }

  constexpr double operator""_kmh(unsigned long long x)
    {
    return distance_in_meters::operator""_km(x) / 3600;
    }

  constexpr double operator""_mph(long double x)
    {
    return distance_in_meters::operator""_mi(x) / 3600;
    }

  constexpr double operator""_mph(unsigned long long x)
    {
    return distance_in_meters::operator""_mi(x) / 3600;
    }

  constexpr double operator""_c(long double x)
    {
    return x * 299'792'458;
    }

  constexpr double operator""_c(unsigned long long x)
    {
    return x * 299'792'458;
    }
  }

#endif

