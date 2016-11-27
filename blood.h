// Author: Sean Davis
#ifndef bloodH
#define bloodH

#include "bloodRunner.h"
#include "braincell.h"

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
  Vessel2* vessel;
  int path(BrainCell &cell, int flow, Vessel2 &prev);
  Vessel2* currentPath;
  int generatePath(BrainCell &cell, Vessel2* p, int &length, int end);
  int generatePath2(BrainCell &cell, Vessel2* p, int &length, int end);
  void generatePathStack(BrainCell &cell, Vessel2* p, int &length, int end);
  int checkCapacity(Vessel2 *temp, int length, int full[], int empty[]);
  void printPath(Vessel2 *temp, int length);
}; // class Blood

#endif
