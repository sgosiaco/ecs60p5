#include "braincell.h"

using namespace std;

BrainCell::BrainCell()
{
  fed = 0;
  outgoing = 0;
}


void BrainCell::create(Vessel2 in[], int count)
{
  fed = 0;
  outgoing = count;
  out = new Vessel2[count];
  for(int i = 0; i < count; i++)
  {
    out[i] = in[i];
  }
}

void Vessel2::copy(Vessel in, int count)
{
  src = in.src;
  dest = in.dest;
  capacity = in.capacity;
  ID = count;
  carrying = 0;
}
