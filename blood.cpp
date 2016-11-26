#include "blood.h"
#include "bloodRunner.h"
#include <iostream>
#include <cstring>

using namespace std;

int Blood::path(BrainCell &cell, int flow, Vessel2 &prev)
{
  for(int i = 0; i < cell.outgoing; i++)
  {
    Vessel2 *vessel = &(cell.out[i]);
    int dest = vessel->dest;
    if(flow == 0)
      flow = vessel->capacity;
    //cout << "Vessel:" << vessel->ID << ' ' << vessel->src << ' ' << dest << ' ' << brain[dest].fed << ' ' << vessel->carrying << ' ' << flow << endl;
    if(!(brain[dest].fed))
    {
      brain[dest].fed = 1;
      //(vessel->carrying)++;
      flow--;
    }
    (vessel->carrying)++;
    //cout << "Vessel:" << vessel->ID << ' ' << vessel->src << ' ' << dest << ' ' << brain[dest].fed << ' ' << vessel->carrying << ' ' << flow << endl;
    if(flow != 0)
      path(brain[dest], flow, *vessel);
  }
  return 1;
}

Blood::Blood(Vessel vessels[], int vesselCount, int cellCount, int depth)
{
  //copying vessels
  vessel = new Vessel2[vesselCount];
  for(int i = 0; i < vesselCount; i++)
    vessel[i].copy(vessels[i], i);

  //creating 2d array of Vessels
  temp = new Vessel2*[cellCount];
  for(int i = 0; i < cellCount; i++)
    temp[i] = new Vessel2[vesselCount];

  //creating count for each brain cell
  count = new int[cellCount];
  for(int i = 0; i < cellCount; i++)
    count[i] = 0;
  //creating adjacency list for each brain cell
  for(int i = 0; i < vesselCount; i++)
  {
    int pos = vessels[i].src;
    //cout << pos << endl;
    temp[pos][count[pos]++] = vessel[i];
  }


  brain = new BrainCell[cellCount];
  for(int i = 0; i < cellCount; i++)
  {
    brain[i].create(temp[i], count[i]);
  }

  //printing out list
  /*
  for(int i = 0; i < cellCount; i++)
  {
    cout << i << ' ' << count[i] << ':';
    for(int k = 0; k < count[i]; k++)
      cout << ' ' << temp[i][k].dest << '(' << temp[i][k].capacity << ')';
    cout << endl;
  }
  */

  cout << "ID Out Fed" << endl;
  for(int i = 0; i < cellCount; i++)
  {
    cout << i << ' ' << brain[i].outgoing << ' ' << brain[i].fed << ':';
    for(int k = 0; k < brain[i].outgoing; k++)
    {
      cout << ' ' << brain[i].out[k].dest << '(' << brain[i].out[k].ID << ',' << brain[i].out[k].capacity << ')';
    }
    cout << endl;
  }

  this->cellCount = cellCount;
  this->vesselCount = vesselCount;
  this->depth = depth;

  currentPath = new Vessel2[depth];
} // Blood()


int Blood::calcFlows(int fullFlows[], int emptyFlows[])
{
  for(int i = 0; i < vesselCount; i++)
    fullFlows[i] = emptyFlows[i] = 0;

  int numFed = 0;
  //if(!brain[0].fed)
    //brain[0].fed = 1;

  //path(brain[0], brain[0].out[0].capacity, brain[0].out[0]);


  //feed everything except first node.
  for(int i = 1; i < cellCount - 1; i++)
  {
    cout << "Blood cell #" << i << endl;
    Vessel2* inPath = new Vessel2[depth];
    Vessel2* outPath = new Vessel2[depth];
    int inLength = 0;
    int outLength = 0;
    if(brain[i].fed == 0)
    {
      generatePath(brain[0], inPath, inLength, i);
      generatePath(brain[i], outPath, outLength, cellCount - 1);
      cout << " InPath: ";
      printPath(inPath, inLength);
      cout << " OutPath: ";
      printPath(outPath, outLength);
      //cout << inLength << ' ' << outLength << endl;
      if(checkCapacity(inPath, inLength, fullFlows, emptyFlows) && checkCapacity(outPath, outLength, fullFlows, emptyFlows))
      {
        cout << " Full Path: ";
        for(int p = 0; p < inLength; p++)
        {
          cout << inPath[p].dest << ' ';
          (fullFlows[inPath[p].ID])++;
        }
        cout << endl;
        cout << " Empty Path: ";
        for(int k = 0; k < outLength; k++)
        {
          cout << outPath[k].dest << ' ';
          (emptyFlows[outPath[k].ID])++;
        }
        cout << endl;

        brain[i].fed = 1;
      }
    }
  }
  if(brain[0].fed == 0)
  {
    cout << "Blood cell #0" << endl;
    Vessel2* outPath = new Vessel2[depth];
    int outLength = 0;
    generatePath(brain[0], outPath, outLength, cellCount - 1);
    cout << " OutPath: ";
    printPath(outPath, outLength);
    //cout << inLength << ' ' << outLength << endl;
    if(checkCapacity(outPath, outLength, fullFlows, emptyFlows))
    {
      cout << " Empty Path: ";
      for(int k = 0; k < outLength; k++)
      {
        cout << outPath[k].dest << ' ';
        (emptyFlows[outPath[k].ID])++;
      }
      cout << endl;

      brain[0].fed = 1;
    }
  }

  if(brain[cellCount - 1].fed == 0)
  {
    cout << "Blood cell #" << cellCount - 1 << endl;
    Vessel2* outPath = new Vessel2[depth];
    int outLength = 0;
    generatePath(brain[0], outPath, outLength, cellCount - 1);
    cout << " InPath: ";
    printPath(outPath, outLength);
    //cout << inLength << ' ' << outLength << endl;
    if(checkCapacity(outPath, outLength, fullFlows, emptyFlows))
    {
      cout << " Full Path: ";
      for(int k = 0; k < outLength; k++)
      {
        cout << outPath[k].dest << ' ';
        (fullFlows[outPath[k].ID])++;
      }
      cout << endl;

      brain[cellCount - 1].fed = 1;
    }
  }

  cout << ">Fed ";
  for(int i = 0; i < cellCount; i++)
  {
    cout << brain[i].fed << ' ';
    if(brain[i].fed)
      numFed++;
  }
  cout << endl;



  /*
  for(int i = 0; i < cellCount - 1; i++)
  {
    cout << "Path " << i << ": ";
    Vessel2* temp = new Vessel2[vesselCount];
    int t = 0;
    generatePath(brain[i], temp, t, cellCount - 1);
    cout << "Vessel array " << i << ": ";
    for(int k = 0; k < t; k++)
    {
      cout  << temp[k].dest << ' ';
    }
    cout << endl;
  }
  */

  //debug printing

  cout << ">Indx: ";
  for(int i = 0; i < vesselCount; i++)
  {
    cout << i << ' ';
  }
  cout << endl;

  cout << ">Full: ";
  for(int i = 0; i < vesselCount; i++)
  {
    cout << fullFlows[i] << ' ';
  }
  cout << endl;

  cout << ">Empt: ";
  for(int i = 0; i < vesselCount; i++)
  {
    cout << emptyFlows[i] << ' ';
  }
  cout << endl;


  return numFed;  // to avoid warning for now
} // calcFlows()f

int Blood::generatePath(BrainCell &cell, Vessel2* p, int &length, int end)
{
  //int end = cellCount - 1;
  for(int i = 0; i < cell.outgoing; i++)
  {
    if(cell.out[i].dest != end)
    {
      if(end != cellCount - 1)
      {
        if(cell.out[i].dest != cellCount - 1)
        {
          //cout << cell.out[i].ID << ',' << cell.out[i].dest << ' ';
          //cout << cell.out[i].dest << ' ';
          p[length++] = cell.out[i];
          if(generatePath(brain[cell.out[i].dest], p, length, end))
            return 1;
        }
      }
      else
      {
        //cout << cell.out[i].ID << ',' << cell.out[i].dest << ' ';
        //cout << cell.out[i].dest << ' ';
        p[length++] = cell.out[i];
        if(generatePath(brain[cell.out[i].dest], p, length, end))
          return 1;
      }

    }
    else
    {
      //cout << cell.out[i].ID << ',' << cell.out[i].dest << endl;
      //cout << cell.out[i].dest << endl;
      p[length++] = cell.out[i];
      return 1;
    }
  }
  return -1;
}

int Blood::checkCapacity(Vessel2* temp, int length, int full[], int empty[])
{
  for(int i = 0; i < length; i++)
  {
    //cout << "ID " << temp[i].ID << endl;
    if(full[temp[i].ID] + empty[temp[i].ID] + 1 > temp[i].capacity)
      return 0;
  }
  return 1;
}

void Blood::printPath(Vessel2* temp, int length)
{
  for(int i = 0; i < length; i++)
    cout << temp[i].dest << ' ';
  cout << endl;
}
