#include "RecoTargetUserOptions.h"
#include "RecoTargetMetrics.h"
#include <algorithm>
#include <iostream>
#include <getopt.h>

using namespace RecoTarget;
using namespace std;

/*! <ul>
 *  <li> parse command line arguments
 *  <li> check if all required arguments was passed 
 *  <li> set up RecoTargetUserOptions based on arguments
 *  <\ul>
 */ 
RecoTargetUserOptions :: RecoTargetUserOptions (int argc, char **argv) : showSummary (false)
{
  if (argc == 1) usage (); // return usage() if no arguments were passed
  
  // short options triggers
  static const char *shortOpts = "p:t:l:k:m:x:y:sh";
  // long options triggers
  static const struct option longOpts[]
  {
    {"path", required_argument, NULL, 'p'},
    {"ntesting", required_argument, NULL, 't'},
    {"nlearning", required_argument, NULL, 'l'},
    {"nneighbors", required_argument, NULL, 'k'},
    {"metric", required_argument, NULL, 'm'},
    {"ttargets", required_argument, NULL, 'x'},
    {"ltargets", required_argument, NULL, 'y'},
    {"summary", no_argument, NULL, 's'},
    {"help", no_argument, NULL, 'h'}
  };

  // flags for required arguments
  bool isPathDefined            = false;
  bool isTestingDefined         = false;
  bool isLearningDefined        = false;
  bool isNeighborsDefined       = false;
  bool isMetricDefined          = false;
  bool isTestingTargetsDefined  = false;
  bool isLearningTargetsDefined = false;
  
  // set initital values of targets flag to off
  fill_n (isTestingTarget, nTargets, false);
  fill_n (isLearningTarget, nTargets, false);
    
  int o, id; // for get_opt (option and option index)

  while ((o = getopt_long (argc, argv, shortOpts, longOpts, &id)) != -1)
    switch (o)
    {
      case 'p':
        pathToFiles = optarg;
        isPathDefined = true;
        break;
      case 't':
        nTestingSamples = atoi (optarg);
        isTestingDefined = true;
        break;
      case 'l':
        nLearningSamples = atoi (optarg);
        isLearningDefined = true;
        break;
      case 'k':
        nNearestNeighbors = atoi (optarg);
        isNeighborsDefined = true;
        break;
      case 'm':
        idMetric = atoi (optarg);
        isMetricDefined = true;
        break;
      case 'x':
        codeToFlags (atoi (optarg), isTestingTarget);
        isTestingTargetsDefined = true;
        break;
      case 'y':
        codeToFlags (atoi (optarg), isLearningTarget);
        isLearningTargetsDefined = true;
        break;
      case 's':
        showSummary = true;
        break;
      case 'h':
        usage();
        break;
      case '?':
        if (optopt == 'c' or isprint (optopt)) usage();
      default:
        usage();
    }
    
  if (!isPathDefined)
    usage ("The path was not defined.");
  if (!isTestingDefined)
    usage ("The size of a testing sample was not defined.");
  if (!isLearningDefined)
    usage ("The size of a learning samples was not defined.");
  if (!isNeighborsDefined)
    usage ("The number of nearest neighbors was not defined.");
  if (!isMetricDefined)
    usage ("The metric was not defined.");
  if (idMetric >= nMetrics)
    usage ("Undefined metric.");
  if (!isTestingTargetsDefined)
    usage ("The list of testing targets was not defined.");
  if (!isLearningTargetsDefined)
    usage ("The list of learning targets was not defined.");
    
  if (showSummary) summary();
}

//! print all available options with explanations
void RecoTargetUserOptions :: usage (const char *error)
{
  if (error[0] != '\0') cerr << "\nERROR: " << error << "\n";
  
  cout << "\n########## USAGE ##########\n";
  
  cout << "\nUsage: ./RecoTarget [options]:\n\n";
  cout << "\t -p, --path       "
       << "\t [path_to_samples] (see example below)\n";
  cout << "\t -t, --ntesting   "
       << "\t [size of a testing sample]\n";
  cout << "\t -l, --nlearning  "
       << "\t [size of a learning sample]\n";
  cout << "\t -x, --ttargets   "
       << "\t [learning target code] (see examples below)\n";
  cout << "\t -y, --ltargets   "
       << "\t [testing target code] (see examples below)\n";
  cout << "\t -k, --nneighbors "
       << "\t [number of nearest neighbors]\n";
  cout << "\t -m, --metric     "
       << "\t [metric] (see the options below)\n";
  cout << "\t -s, --summary    "
       << "\t (use to see your options summary)\n";
  cout << "\t -h, --help       "
       << "\t (show what you are reading now)\n";
  
  cout << "\n########## PATH ##########\n";

  cout << "\nUse path without run number, e.g.: "
       << "/minerva/data/users/goran/mc_production/nogrid/"
       << "central_value/minerva/ana/v10r8p1/";
  
  cout << "\nThe following convention is assumed: "
       << "path/00/00/00/0X -> files for target X\n";
            
  cout << "\n########## TARGETS ##########\n";          
            
  cout << "\nTarget code examples:\n\n";
  cout << "\t x -t 145  \t (proceed targets 1, 4, 5)\n";
  cout << "\t y -t 2514 \t (learn from targets 1, 2, 4, 5)\n";
  
  cout << "\n########## METRICS ##########\n";
  
  cout << "\nAvailable metrics:\n\n";
  
  for (unsigned int i = 0; i < nMetrics; i++)
    cout << "\t" << i << " - " << listOfMetrics[i] << "\n";
    
  cout << "\n";
  
  exit (1);
}

//! print options chosen by user and ask if program should continue
void RecoTargetUserOptions :: summary ()
{
  cout << "\nThis is your setup:\n\n";
  cout << "The path to ana files: \033[1m"
       << pathToFiles << "\033[0m\n";
  cout << "The size of your testing sample = \033[1m"
       << nTestingSamples << "\033[0m\n";
  cout << "The size of your learning sample = \033[1m"
       << nLearningSamples << "\033[0m\n";
  cout << "The number of nearest neighbors = \033[1m"
       << nNearestNeighbors << "\033[0m\n";
  cout << "Targets to proceed: \033[1m";
  
  for (unsigned int i = 0; i < 5; i++)
    if (isTestingTarget[i]) cout << i+1 << " ";

  cout << "\n\033[0mTargets to learn from: \033[1m";
  
  for (unsigned int i = 0; i < 5; i++)
    if (isLearningTarget[i]) cout << i+1 << " ";
    
  cout << "\n\033[0mYour metric: \033[1m"
       << listOfMetrics[idMetric] << "\033[0m\n";
  
  char answer;
  
  cout << "\nDo you want to proceed [y/n]? "; cin >> answer;
  
  if (answer != 'Y' and answer != 'y') exit (2); 
}

//! target code = sum_i^#targets (TARGET * 10^i)
void RecoTargetUserOptions::codeToFlags (int code, bool *flags)
{
  while (code > 0)
  {
    const unsigned int target = code % 10;
    
    if (target < 1 or target > 5) usage ("Wrong target.");
    
    flags [target-1] = true;    
    
    code /= 10;
  };
}
