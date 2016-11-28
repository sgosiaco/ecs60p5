// Author: Sean Davis
#ifndef bloodH
#define bloodH

#include "bloodRunner.h"
#include "braincell.h"
#include "vector.h"

class Blood
{
public:
  Blood(Vessel vessels[], int vesselCount, int cellCount, int depth);
  int calcFlows(int fullFlows[], int emptyFlows[]);
  Vessel2** temp;
  BrainCell* brain;
  int* count;
  int cellCount;
  int vesselCount;
  int depth;
  int debug;
  int pathsCreated;
  int totalFed;
  Vessel2* vessel;
  vector<int> vec;
  int generatePath2(BrainCell &cell, Vessel2* p, int &length, int end);
  void generatePath3(BrainCell &cell, BrainCell* p, int &length, int end);
  void generatePath4(BrainCell &cell, BrainCell* p, int &length, int end, int start);
  void generatePathQueue(BrainCell &cell, Vessel2* p, int &length, int end);
  int checkCapacity(Vessel2 *temp, int length, int full[], int empty[]);
  void printPath(Vessel2 *temp, int length);
}; // class Blood

#endif
