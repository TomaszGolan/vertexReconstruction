#include "RecoTargetMetrics.h"
#include <cmath>
#include <iostream>

namespace RecoTarget
{
  //! use for understandable cout's
  const char *listOfMetrics[] =
  {
    "Euclidean",
    "Manhattan",
    "Cosine similarity"
  };
  
  //! return (x-y)^2 multiplied by weight (1.0 by default)
  double metricEuclidean (const double &x, const double &y,
                          const double &w)
  {    
    return w * pow (x - y, 2.0);
  }

  //! return |x-y| multiplied by weight (1.0 by default)
  double metricManhattan (const double &x, const double &y,
                          const double &w)
  {
    return w * fabs (x - y);
  }
  
  //! return -xy multiplied by weight (1.0 by default)
  double metricCosine (const double &x, const double &y,
                       const double &w)
  {
    return -w * x * y;
  }
}
