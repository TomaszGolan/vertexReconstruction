#include "RecoTargetSampleHandler.h"
#include <iostream>
#include <cstdlib>

using namespace RECOTRACKS_ANA;
using namespace RecoTarget;

RecoTargetSampleHandler :: RecoTargetSampleHandler (const int &n)
  : nSamples (n)
{
  samples = new Sample[nSamples];
}

/*! <ul>
 *  <li> loop over "recoTracks"
 *  <li> take "nSamples" entries (starting from entry = "start", 
 *  every "step" entry
 *  <li> fill "samples"
 *  </ul> 
 */ 
void RecoTargetSampleHandler :: fillSamples (
  RECOTRACKS_ANA::RecoTracks *recoTracks,
  const unsigned int &start, const unsigned int &step)
{
  for (unsigned int i = 0; i < nSamples; i++)
  {
    recoTracks->GetEntry (start + i * step);
    
    samples[i].fill (recoTracks->plane_visible_energy, 
                     recoTracks->plane_id,
                     recoTracks->plane_id_sz);
  }
}

/*! <ul>
 *  <li> loop over planes 
 *  <li> note: recoTracks store only non-zero entries
 *  <li> translate plane id to order id
 *  <li> save visible energy for current order id
 *  <li> normalize distribution to 1
 *  </ul>
 */
void RecoTargetSampleHandler :: Sample :: fill (
  const double *planeVisibleEnergy, 
  const int *planeId, 
  const unsigned int &nFilledPlanes)
{
  double totalEnergy = 0.0; // sum of energy in each plane
  
  // initial energy distribution = 0
  std::fill_n (energyPerPlane, nPlanes, 0.0);
    
  // copy energy plane distribution to array
  for (unsigned int i = 0; i < nFilledPlanes; i++)
  {
    // get id order based on plane id
    const int idPlaneZorder = idOrderMap[planeId[i]];
    // save energy in proper slot        
    energyPerPlane[idPlaneZorder] = planeVisibleEnergy[i];
    // add current plane enegry to the total energy
    totalEnergy += planeVisibleEnergy[i];
  }
            
  // normalize distribution to 1
  if (totalEnergy  > 0.0)
    for (unsigned int i = 0; i < nPlanes; i++)
      energyPerPlane[i] /= totalEnergy;
}

/*! <ul>
 *  <li> set a pointer to proper metric function
 *  <li> loop over planes
 *  <li> sum the "distance" between all planes
 *  <li> "distance" is measured in terms of the energy difference
 *  (from normalized energy plane distributions)
 *  </ul>
 */
double RecoTargetSampleHandler :: Sample :: distance
  (const Sample &sample, const Metric &metric)
{
  double distance = 0.0; // total "distance"

  /* at this point each distance comes with the same weight
   * in the future it will be a pointer to weight function
   * to calculate weight based on the physical distance between planes
   */ 
  const double weight = 1.0; 
  
  // pointer to metric function (x, y, weight)
  double (*pMetric)(const double&, const double&, const double&);

  switch (metric) // set up pointer to metric
  {
    case EUCLIDEAN:
      pMetric = metricEuclidean;
      break;
    case MANHATTAN:
      pMetric = metricManhattan;
      break;
    case COSINE:
      pMetric = metricCosine;
      break;
    default:
      std::cerr << "\nERROR: undefined metric\n\n";
      exit (4);
      break;
  }
  
  for (unsigned int i = 0; i < nPlanes; i++) // loop over planes
    distance += pMetric (energyPerPlane[i], sample.energyPerPlane[i], 
                         weight);
   
  return distance;
}

/*! <ul>
 *  <li> loop over samples
 *  <li> for each sample loop over training samples
 *  <li> save distance between samples
 *  </ul>
 */
void RecoTargetSampleHandler :: fillNeighbors
  (RecoTargetSampleHandler *sampleHandler,
  const unsigned int &target,
  const Metric &metric)
{
  for (unsigned int i = 0; i < nSamples; i++) // loop over samples
  {
    // loop over training samples
    for (unsigned int j = 0; j < sampleHandler->nSamples; j++)
    {
      // calculalte distance between testing and learning samples
      const double distance = 
        samples[i].distance (sampleHandler->samples[j], metric);
      
      // save neighbor
      samples[i].neighbors.push_back
        (std::pair <double, unsigned int> (distance, target));
    }
  }  
}

//! sort neighbors, count score for each target and return the best
int RecoTargetSampleHandler :: Sample :: closestTarget
  (const unsigned int &k)
{
  neighbors.sort(); // sort neighbors respect to the distance
  
  unsigned int targetScore[nTargets] = {0};
  
  // set up iterator at the begininng of the neighbors list
  std::list < std::pair <double, unsigned int> > :: iterator it = 
    neighbors.begin();
  
  // count how many entires per target is up to k 
  for (unsigned int i = 0; i < k; i++) targetScore[(*it++).second]++;
  
  // find the best match (target with highest score)
  
  unsigned int bestScore = 0;
  unsigned int bestTarget = 0;
  
  for (unsigned int i = 0; i < nTargets; i++)
    if (targetScore[i] > bestScore)
    {
      bestScore = targetScore[i];
      bestTarget = i;
    }
    
  return bestTarget;
}

//! target = what target it should be, k = #nearest neighbors
double RecoTargetSampleHandler :: getScore (const unsigned int &target,
                                            const unsigned int &k)
{
  unsigned int score = 0; // final score = #goodGuesses / #samples
  
  // loop over sample to check how many was guessed correctly
  for (unsigned int i = 0; i < nSamples; i++)
    if (samples[i].closestTarget (k) == target) score++;
    
  return 1.0 * score / nSamples;
}
