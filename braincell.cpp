#include <iostream>
#include "braincell.h"

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
  for(int i = 0; i < count; i++)
  {
    out[i] = in[i];
  }
}

void BrainCell::create(int count, int ID)
{
  fed = 0;
  visited = 0;
  source = 0;
  inLength = outLength = 0;
  inPath = outPath = NULL;
  this->ID = ID;
  outgoing = 0;
  out = new Vessel2[count];
}

void Vessel2::copy(Vessel in, int count)
{
  src = in.src;
  dest = in.dest;
  capacity = in.capacity;
  ID = count;
  carrying = 0;
}


void BrainCell::updateInPath(BrainCell *p, int length)
{
  int temp = 0;
  for(int i = 0; i < length - 1; i++)
  {
    for(int k = 0; k < p[i].outgoing; k++)
    {
      if(p[i].out[k].dest == p[i + 1].ID)
      {
        inPath[temp++] = p[i].out[k];
        break;
      }
    }
  }
  inLength = temp;
}

void BrainCell::updateOutPath(BrainCell *p, int length)
{
  int temp = 0;
  for(int i = 0; i < length - 1; i++)
  {
    for(int k = 0; k < p[i].outgoing; k++)
    {
      if(p[i].out[k].dest == p[i + 1].ID)
      {
        outPath[temp++] = p[i].out[k];
        break;
      }
    }
  }
  outLength = temp;
}
