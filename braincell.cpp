#include <iostream>
#include "braincell.h"
#include <cstring>

using namespace std;

BrainCell::BrainCell()
{
  fed = 0;
  outgoing = 0;
  ID = 0;
  inPath = outPath = NULL;
  inLength = outLength = 0;
  visited = 0;
  source = 0;
}


void BrainCell::create(Vessel2 in[], int count, int ID)
{
  fed = 0;
  visited = 0;
  source = 0;
  inLength = outLength = 0;
  inPath = outPath = NULL;
  this->ID = ID;
  outgoing = count;
  out = new Vessel2[count];
  memcpy(out, in, sizeof(Vessel2)*count);
  /*
  for(int i = 0; i < count; i++)
  {
    out[i] = in[i];
  }
  */
}

void Vessel2::copy(Vessel in, int count)
{
  src = in.src;
  dest = in.dest;
  capacity = in.capacity;
  ID = count;
  carrying = 0;
}
