#ifndef __FMO_CPLA__TRACER
#define __FMO_CPLA__TRACER

#include <iostream>
#include <string>

namespace fmo
  {

  struct Tracer
    {
    explicit Tracer(std::string const & name = "") : m_name{name}
      {
      std::cout << "TRACER - " << m_name << " -: create\n";
      }

    Tracer(Tracer const & other) : m_name{other.m_name}
      {
      std::cout << "TRACER - " << m_name << " -: copy\n";
      }

    ~Tracer()
      {
      std::cout << "TRACER - " << m_name << " -: destroy\n";
      }

    void show() const
      {
      std::cout << "TRACER - " << m_name << " -: show\n";
      }

    private:
      std::string const m_name;
    };

  }

#endif
