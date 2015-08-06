#include "RecoTargetUtils.h"
#include "RecoTargetDetectorProperties.h"
#include <cstring>
#include <iostream>
#include <cstdlib>

using namespace RECOTRACKS_ANA;
using std::strcpy;
using std::strcat;

namespace RecoTarget
{
  //! create a new char of the length "a"+"b", copy "a" and add "b" 
  char* mergeChar (const char *a, const char *b)
  {   
    return strcat (strcpy (new char[strlen(a) + strlen(b)], a), b);
  }
  
  /*! <ul>
   *  <li> create a TChain from all files in the path
   *  <li> check if TChain is not empty 
   *  <li> make RecoTracks
   *  </ul>
   */ 
  RecoTracks* loadFiles (const char *pathToFiles)
  {
    // create temporary TChain
    TChain *tChain = new TChain ("RecoTracks");
    // add all files from pathToFiles to the chain
    tChain->Add (pathToFiles);

    if (tChain -> GetEntries() == 0)
    {
      std::cerr << "\nERROR: There is no files in "
                << pathToFiles << "\n\n";
      exit (3);
    }
        
    // create and return RecoTracks from TChain
    return new RecoTracks (tChain);
  }

  /*! <ul>
   *  <li> create path to files for each target
   *  <li> loop over targets
   *  <li> load RecoTracks
   *  <li> fill samples with defined number of entries
   *  </ul>
   */ 
  void loadSamples (RecoTargetSampleHandler **testingSamples,
                    RecoTargetSampleHandler **learningSamples,
                    const RecoTargetUserOptions &userOptions)
  {
    // path to input ana files assuming 'path/00/00/00/0#TARGET'
    const char *pathToFiles[nTargets] =
    {
      mergeChar (userOptions.getPath(), "/00/00/00/01/*.root"),
      mergeChar (userOptions.getPath(), "/00/00/00/02/*.root"),
      mergeChar (userOptions.getPath(), "/00/00/00/03/*.root"),
      mergeChar (userOptions.getPath(), "/00/00/00/04/*.root"),
      mergeChar (userOptions.getPath(), "/00/00/00/05/*.root")
    };
    
    for (unsigned int i = 0; i < nTargets; i++) // loop over targets
    {
      // check if i-th target was selected 
      if (not (userOptions.getFlagTestingTarget (i) or 
               userOptions.getFlagLearningTarget (i))) continue;
            
      // get RecoTracks for current target
      RecoTracks *recoTracks = loadFiles (pathToFiles[i]);
      
      // create testing samples handler for current target
      if (userOptions.getFlagTestingTarget (i))
        testingSamples[i] =
        createSample (recoTracks, userOptions.getNTestingSamples(), 1);

      // create learning samples handler for current target
      if (userOptions.getFlagLearningTarget (i))
        learningSamples[i] =
        createSample (recoTracks, userOptions.getNLearningSamples(), 0);

      delete recoTracks;    
    }
  }
    
  //! create a sample, set up step for looping events, load events
  RecoTargetSampleHandler* createSample (RecoTracks *recoTracks,
                                         const unsigned int &sampleSize,
                                         const bool &isTesting)
  {
    // create a sample handler
    RecoTargetSampleHandler *sample = 
      new RecoTargetSampleHandler (sampleSize);
    
    // number of entries in RecoTracks
    const unsigned int nEntries = recoTracks->fChain->GetEntries();
    
    // step for events loop (must be even, because it will take
    // even events for testing sample and odd event for learning)
    const unsigned int step = (nEntries / sampleSize / 2) * 2;
    
    // initial event in the loop
    unsigned int start = 0;
    
    if (isTesting) start = 1;
        
    // fill sample using "sampleSize" events, starting from "start"
    // with step = "step"
    sample->fillSamples (recoTracks, start, step);   
    
    return sample;
  }
}
