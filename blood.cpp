#include "blood.h"
#include "bloodRunner.h"
#include <iostream>
#include <cstring>

using namespace std;

Blood::Blood(Vessel vessels[], int vesselCount, int cellCount, int depth)
{
  vessel = new Vessel[vesselCount];
  for(int i = 0; i < vesselCount; i++)
    vessel[i] = vessels[i];

  temp = new Vessel*[cellCount];
  for(int i = 0; i < vesselCount; i++)
    temp[i] = new Vessel[vesselCount];

  count = new int[cellCount];
  for(int i = 0; i < cellCount; i++)
    count[i] = 0;

  for(int i = 0; i < vesselCount; i++)
  {
    int pos = vessels[i].src;
    //cout << pos << endl;
    temp[pos][count[pos]++] = vessels[i];
  }

  for(int i = 0; i < cellCount; i++)
  {
    cout << i << ' ' << count[i] << ':';
    for(int k = 0; k < count[i]; k++)
      cout << ' ' << temp[i][k].dest << '(' << temp[i][k].capacity << ')';
    cout << endl;
  }

  this->cellCount = cellCount;
} // Blood()


int Blood::calcFlows(int fullFlows[], int emptyFlows[])
{
  for(int i = 0; i < cellCount; i++)
  {
    for(int k = 0; k < count[i]; k++)
    {
      temp[i][k].capacity--;
    }
  }

  return 0;  // to avoid warning for now
} // calcFlows()
