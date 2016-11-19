// Author: Sean Davis
#ifndef bloodH
#define bloodH

#include "bloodRunner.h"

class Blood
{
public:
  Blood(Vessel vessels[], int vesselCount, int cellCount, int depth);
  int calcFlows(int fullFlows[], int emptyFlows[]);
}; // class Blood


#endif
