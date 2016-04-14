#ifndef __TL
#define __TL

#include <functional>

namespace times
  {

  namespace literal
    {

    struct invoker {
      constexpr invoker(unsigned long long n) :
        n { n } {
        }
      void operator()(std::function<void()> f) {
        for (auto i = 0u; i < n; i++) {
          f();
        }
      }
      private:
      unsigned long long const n;
    };

    constexpr invoker operator"" _times(unsigned long long n) {
      return invoker { n };
    }

    }
  }


#endif
