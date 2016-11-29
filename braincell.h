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
  void create(Vessel2 in[], int count, int ID);
  void updateInPath(BrainCell *p, int length);
  void updateOutPath(BrainCell *p, int length);
  int outgoing;
  int fed;
  int ID;
  Vessel2* out;
  Vessel2* inPath;
  Vessel2* outPath;
  int inLength;
  int outLength;
  int visited;
  int source;
}; //class BrainCell

#endif
