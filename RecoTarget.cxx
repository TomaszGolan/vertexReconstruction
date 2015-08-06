/**
 * @brief Target reconstruction by kNN classifier
 * 
 * @author TG, GP, MW
 * @date 2015
 * 
*/

#include "RecoTargetSampleHandler.h"
#include "RecoTargetUserOptions.h"
#include "RecoTargetUtils.h"
#include <iostream>
#include <cstring>

using namespace RECOTRACKS_ANA;
using namespace RecoTarget;
using std::cout;
using std::vector;

int main (int argc, char *argv[])
{
  // parse and save user's command arguments
  RecoTargetUserOptions userOptions (argc, argv);
  
  // arrays for testing and learning samples
  RecoTargetSampleHandler *testingSamples[nTargets] = {NULL};
  RecoTargetSampleHandler *learningSamples[nTargets] = {NULL};
  
  // load sample from ana files with options specified by user
  loadSamples (testingSamples, learningSamples, userOptions);
  
  // loop over samples, check if it is selected and fill neighbors  
  for (unsigned int i = 0; i < nTargets; i++)
    if (userOptions.getFlagTestingTarget (i))
      for (unsigned int j = 0; j < nTargets; j++)
        if (userOptions.getFlagLearningTarget (j))
          testingSamples[i]->fillNeighbors
            (learningSamples[j], j, (Metric)userOptions.getMetric());
  
  for (unsigned int i = 0; i < nTargets; i++)
    cout << "Target " << i + 1 << " -> "
         << testingSamples[i]->getScore (i, userOptions.getNeighbors())
         << "\n";
      
  return 0;
}
