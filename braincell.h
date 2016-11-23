#ifndef cellH
#define cellH

#include "bloodRunner.h"

class Vessel2
{
 public:
 	int src;
  int dest;
  int capacity;
  int ID;
  int carrying;
  void copy(Vessel in, int count);
};  // class Vessel

class BrainCell
{
public:
  BrainCell();
  void create(Vessel2 in[], int count);
  int outgoing;
  int fed;
  Vessel2* out;
}; //class BrainCell

#endif
