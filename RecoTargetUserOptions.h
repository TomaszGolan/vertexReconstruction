/**
 * @brief Process and store user command line options
 * 
 * @author TG, GP, MW
 * @date 2015
 * 
*/

#ifndef RECO_TARGET_USER_OPTIONS_H
#define RECO_TARGET_USER_OPTIONS_H

#include "RecoTargetDetectorProperties.h"

class RecoTargetUserOptions
{
  public:
  
  RecoTargetUserOptions (int argc, char **argv); //!< constructor
    
  //! return path to files
  inline char* getPath () const
  {
    return pathToFiles;
  };
  
  //! return testing target on/off flag
  inline bool getFlagTestingTarget (int id) const 
  {
    return isTestingTarget[id];
  };

  //! return learning target on/off flag
  inline bool getFlagLearningTarget (int id) const
  {
    return isLearningTarget[id];
  };
  
  //! return number of testing samples
  inline unsigned int getNTestingSamples () const
  {
    return nTestingSamples;
  };

  //! return number of learning samples
  inline unsigned int getNLearningSamples () const
  {
    return nLearningSamples;
  };
  
  //! return chosen metric
  inline unsigned int getMetric () const
  {
    return idMetric;
  };

  //! return the number of nearest neighbors for kNN
  inline unsigned int getNeighbors () const
  {
    return nNearestNeighbors;
  };
  
  private:

  //! path to the ana files to process
  char *pathToFiles;

  //! number of samples to process
  unsigned int nTestingSamples;
  //! size of learning sample
  unsigned int nLearningSamples;
  //! number of nearest neighbors that matters
  unsigned int nNearestNeighbors;

  unsigned int idMetric; //!< id of the chosen metric

  //!< on/off flag for testing targets
  bool isTestingTarget[RecoTarget::nTargets];

  //!< on/off flag for learning targets
  bool isLearningTarget[RecoTarget::nTargets];

  bool showSummary; //!< true if summary should be displayed before run
  
  void usage (const char *error = ""); //!< print usage and exit program
  void summary (); //!< print user setup
  //! convert target code to array of flags
  void codeToFlags (int code, bool *flags);
};

#endif
