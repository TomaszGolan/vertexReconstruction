/**
 * @brief Load samples from file etc
 * 
 * @author TG, GP, MW
 * @date 2015
 * 
*/

#ifndef RECO_TARGET_SAMPLE_HANDLER_H
#define RECO_TARGET_SAMPLE_HANDLER_H

#include "RecoTargetDetectorProperties.h"
#include "RecoTracks.h"
#include "RecoTargetMetrics.h"
#include <list>

class RecoTargetSampleHandler
{
  public:
  
  RecoTargetSampleHandler (const int &n); //!< constructor

  //! fill samples from recoTracks
  void fillSamples (RECOTRACKS_ANA::RecoTracks *recoTracks,
                    const unsigned int &start = 0,
                    const unsigned int &step = 2);
  
  //! fill neighbors list for each sample                  
  void fillNeighbors (RecoTargetSampleHandler *sampleHandler,
                      const unsigned int &target,
                      const RecoTarget::Metric &metric);
  //! check how many times the target is predicted correctly
  double getScore (const unsigned int &target, const unsigned int &k);

  private:
      
  //! single sample contains a table with energy in each plane
  struct Sample
  {
    double energyPerPlane[RecoTarget::nPlanes]; //!< plane energy distr
   
    //! fill energyPerPlane in proper order
    void fill (const double *planeVisibleEnergy, const int *planeId, 
               const unsigned int &nFilledPlanes);
   
    //! calculate distance between two samples
    double distance (const Sample &sample,
                     const RecoTarget::Metric &metric);
                     
    //! get the target having k nearest neighbors to the sample
    int closestTarget (const unsigned int &k);
                     
    //! nighbors list (pair <distance, target>)
    std::list < std::pair <double, unsigned int> > neighbors;
  };
    
  Sample *samples; //!< table with all samples
  
  unsigned int nSamples;  
};

#endif
