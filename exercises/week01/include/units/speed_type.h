#ifndef __FMO__SPEED_TYPE_H
#define __FMO__SPEED_TYPE_H

#include "distance_type.h"

#include <chrono>

namespace fmo
  {

  struct distance;

  struct speed
    {
    constexpr speed() noexcept = default;
    constexpr speed(speed const &) noexcept = default;
    constexpr speed(speed &&) noexcept = default;

    explicit constexpr speed(long double const ms) noexcept;
    explicit constexpr speed(unsigned long long const ms) noexcept;

    template<typename Rep, typename Period>
    constexpr speed(distance const & distance, std::chrono::duration<Rep, Period> const & duration)
      {
      auto const seconds = std::chrono::seconds(duration);
      m_rep = distance / seconds.count();
      }

    constexpr bool operator > (speed const & other) const;
    constexpr bool operator < (speed const & other) const;
    constexpr bool operator >= (speed const & other) const;
    constexpr bool operator <= (speed const & other) const;
    constexpr bool operator == (speed const & other) const;

    constexpr speed operator + (speed const & other);
    constexpr speed operator - (speed const & other);
    constexpr speed operator / (speed const & other);
    constexpr speed operator * (speed const & other);

    constexpr speed & operator = (speed const & other) = default;

    constexpr speed & operator += (speed const & other);
    constexpr speed & operator -= (speed const & other);
    constexpr speed & operator /= (speed const & other);
    constexpr speed & operator *= (speed const & other);

    private:
      long double m_rep{};
    };

  constexpr speed operator""_ms(long double ms)
    {
    return speed{ms};
    }

  constexpr speed operator""_ms(unsigned long long ms)
    {
    return speed{ms};
    }

  }

#endif
