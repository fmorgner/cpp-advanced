#ifndef __FMO_UNITS__DISTANCE_TYPE_H
#define __FMO_UNITS__DISTANCE_TYPE_H

#include "util.h"

#include <ratio>
#include <type_traits>
#include <chrono>

namespace fmo
  {

  namespace units
    {

    template<typename Rep, typename Ratio = std::ratio<1, 1>>
    struct distance;

    template<typename>
    struct is_distance : std::false_type {};

    template<typename Rep, typename Ratio>
    struct is_distance<distance<Rep, Ratio>> : std::true_type {};

    namespace impl
      {

      }

    template<typename TargetType, typename Rep, typename Ratio>
    constexpr std::enable_if_t<is_distance<TargetType>::value, TargetType> distance_cast(distance<Rep, Ratio> const & source)
      {
      using Factor = std::ratio_divide<Ratio, typename TargetType::ratio>;
      return TargetType{source.count() * Factor::num / Factor::den};
      }

    template<typename Rep, typename Ratio>
    struct distance
      {
      using rep = Rep;
      using ratio = Ratio;

      constexpr distance() = default;

      constexpr distance(distance const &) = default;

      constexpr distance(Rep const & val) : m_count{val} { }

      template<typename Rep2, class Ratio2>
      constexpr distance(distance<Rep2, Ratio2> const & other)
        {
        distance_cast<Rep, distance<Rep2, Ratio2>, Ratio>(other);
        }

      constexpr rep count() const
        {
        return m_count;
        }

      constexpr distance & operator = (distance const & other) = default;

      private:
        rep m_count{};
      };

    using nanometers  = distance<float, std::ratio<1, 1000000000>>;
    using micrometers = distance<float, std::ratio<1,    1000000>>;
    using millimeters = distance<float, std::ratio<1,       1000>>;
    using centimeters = distance<float, std::ratio<1,        100>>;
    using decimeters  = distance<float, std::ratio<1,         10>>;
    using meters      = distance<float, std::ratio<1,          1>>;
    using decameters  = distance<float, std::ratio<10,         1>>;
    using hectometers = distance<float, std::ratio<100,        1>>;
    using kilometers  = distance<float, std::ratio<1000,       1>>;
    using megameters  = distance<float, std::ratio<1000000,    1>>;
    using gigameters  = distance<float, std::ratio<1000000000, 1>>;

    using feet        = distance<float, std::ratio<3048,   10000>>;
    using yard        = distance<float, std::ratio<9144,   10000>>;
    using miles       = distance<float, std::ratio<1609344, 1000>>;

    using au          = distance<float, std::ratio<149597870700,      1>>;
    using parsec      = distance<float, std::ratio<30860000000000000, 1>>;

    using ningi_side  = distance<float, std::ratio<10944000,          1>>;
    }

  }

namespace std
  {

  template<typename Rep1, typename Ratio1, typename Rep2, typename Ratio2>
  struct common_type<fmo::units::distance<Rep1, Ratio1>, fmo::units::distance<Rep2, Ratio2>>
    {
    using type = fmo::units::distance<std::common_type<Rep1, Rep2>,
                                      std::ratio<fmo::util::gcd(Ratio1::num, Ratio2::num),
                                                 fmo::util::lcm(Ratio1::den, Ratio2::den)>>;
    };

  }

#endif

