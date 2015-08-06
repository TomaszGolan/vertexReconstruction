/**
 * @brief Utils for RecoTarget 
 * 
 * @author TG, GP, MW
 * @date 2015
 * 
*/

#ifndef RECO_TARGET_UTILS_H
#define RECO_TARGET_UTILS_H

#include "RecoTracks.h"
#include "RecoTargetSampleHandler.h"
#include "RecoTargetUserOptions.h"

namespace RecoTarget
{
  //! return char* + char*
  char* mergeChar (const char *a, const char *b);
  
  //! load recotracks tree from files
  RECOTRACKS_ANA::RecoTracks* loadFiles (const char *pathToFiles);
  
  //! load testing and learning samples
  void loadSamples (RecoTargetSampleHandler **testingSamples,
                    RecoTargetSampleHandler **learningSamples,
                    const RecoTargetUserOptions &userOptions);
  
  //! make a sample from RecoTracks                  
  RecoTargetSampleHandler* createSample 
    (RECOTRACKS_ANA::RecoTracks *recoTracks,
     const unsigned int &sampleSize,
     const bool &isTesting);
}

#endif
