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
  Vessel2* vessel;
  int path(BrainCell &cell, int flow, Vessel2 &prev);
}; // class Blood

#endif
