/**
 * @brief Global const varibales for detector geometry
 * 
 * @author TG, GP, MW
 * @date 2015
 * 
*/

#ifndef RECO_TARGET_DETECTOR_PROPERTIES_H
#define RECO_TARGET_DETECTOR_PROPERTIES_H

#include <map>

namespace RecoTarget
{
  const unsigned int nPlanes = 208; //!< number of planes in detector
  
  const unsigned int nTargets = 5; //!< number of targets
  
  //! the map of plane positions in the detector
  extern std::map <int, double> planePositions;
  
  //! the map of plane id order
  extern std::map <int, int> idOrderMap;
};

#endif
