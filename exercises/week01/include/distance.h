#ifndef __DISTANCE_IN_METERS__DISTANCE_H
#define __DISTANCE_IN_METERS__DISTANCE_H

namespace distance_in_meters
  {
  constexpr double operator""_m(long double x)
    {
    return x;
    }

  constexpr double operator""_m(unsigned long long x)
    {
    return x;
    }

  constexpr double operator""_km(long double x)
    {
    return x * 1'000;
    }

  constexpr double operator""_km(unsigned long long x)
    {
    return x * 1'000;
    }

  constexpr double operator""_mm(long double x)
    {
    return x / 1'000;
    }

  constexpr double operator""_mm(unsigned long long x)
    {
    return x / 1'000;
    }

  constexpr double operator""_nm(long double x)
    {
    return x / 1'000'000'000;
    }

  constexpr double operator""_nm(unsigned long long x)
    {
    return x / 1'000'000'000;
    }

  constexpr double operator""_ft(long double x)
    {
    return x * 0.3048;
    }

  constexpr double operator""_ft(unsigned long long x)
    {
    return x * 0.3048;
    }

  constexpr double operator""_mi(long double x)
    {
    return x * 5280;
    }

  constexpr double operator""_mi(unsigned long long x)
    {
    return x * 5280;
    }

  constexpr double operator""_yd(long double x)
    {
    return x * 3;
    }

  constexpr double operator""_yd(unsigned long long x)
    {
    return x * 3;
    }
  }

#endif

