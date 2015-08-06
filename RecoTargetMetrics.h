/**
 * @brief The definitions of metrics
 * 
 * @author TG, GP, MW
 * @date 2015
 * 
*/

#ifndef RECO_TARGET_METRICS_H
#define RECO_TARGET_METRICS_H

namespace RecoTarget
{
  const unsigned int nMetrics = 3; //!< number of implemented metrics
  extern const char *listOfMetrics[]; //!< list of implemented metrics
  //! metrics enumerator
  enum Metric {EUCLIDEAN, MANHATTAN, COSINE};
  
  //! Eucliean metric
  double metricEuclidean (const double &x, const double &y,
                          const double &w = 1.0);
  //! Manhattan metric
  double metricManhattan (const double &x, const double &y,
                          const double &w = 1.0);
  //! Cosine similarity
  double metricCosine (const double &x, const double &y,
                       const double &w = 1.0);
}

#endif
