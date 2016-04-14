#include "tracer.h"
#include <vector>

int main()
  {
  auto v = std::vector<fmo::Tracer>{};

  v.push_back(fmo::Tracer{"T1"});
  v.push_back(fmo::Tracer{"T2"});
  v.push_back(fmo::Tracer{"T3"});
  v.push_back(fmo::Tracer{"T4"});

  auto c = v;
  }
