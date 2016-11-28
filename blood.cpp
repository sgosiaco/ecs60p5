#include "blood.h"
#include "bloodRunner.h"
#include "StackAr.h"
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
  debug = 0;
  pathsCreated = 0;
  totalFed = 0;
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
    brain[i].create(temp[i], count[i], i);
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

  if(debug)
  {
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

  //path(brain[0], brain[0].out[0].capacity, brain[0].out[0]);

  if(!pathsCreated)
  {
    Vessel2* inPathTemp = new Vessel2[depth];
    int inLengthTemp = 0;
    
    for(int i = 1; i < cellCount - 1; i++)
    {
      brain[i].inPath = new Vessel2[depth];
      brain[i].outPath = new Vessel2[depth];
      generatePath2(brain[0], brain[i].inPath, brain[i].inLength, i);
      generatePathStack(brain[0], inPathTemp, inLengthTemp, i);
      generatePath2(brain[i], brain[i].outPath, brain[i].outLength, cellCount - 1);
      if(debug)
      {
        cout << " Stack: ";
        printPath(inPathTemp, inLengthTemp);
        cout << " InPath: ";
        printPath(brain[i].inPath, brain[i].inLength);
        cout << " inLength: " << brain[i].inLength << endl;
        cout << " OutPath: ";
        printPath(brain[i].outPath, brain[i].outLength);
        //cout << inLength << ' ' << outLength << endl;
      }
    }
    brain[0].outPath = new Vessel2[depth];
    generatePath(brain[0], brain[0].outPath, brain[0].outLength, cellCount - 1);
    pathsCreated = 1;
  }

  if(brain[0].fed == 0)
  {
    if(checkCapacity(brain[0].outPath, brain[0].outLength, fullFlows, emptyFlows))
    {
      for(int k = 0; k < brain[0].outLength; k++)
      {
        (emptyFlows[brain[0].outPath[k].ID])++;
      }

      brain[0].fed = 1;
      totalFed++;
    }
  }

  //feed everything except first node.
  for(int i = 1; i < cellCount - 1; i++)
  {
    int dontFeed = 0;

    if(debug)
      cout << "Blood cell #" << i << endl;

    if(brain[i].fed == 0)
    {
      for(int k = 0; k < brain[i].inLength - 1; k++)
      {
        if(brain[brain[i].inPath[k].src].fed == 0)
          dontFeed = 1;
      }
      if(checkCapacity(brain[i].inPath, brain[i].inLength, fullFlows, emptyFlows) && checkCapacity(brain[i].outPath, brain[i].outLength, fullFlows, emptyFlows) && !dontFeed)
      {
        if(debug)
          cout << " Full Path: ";
        for(int p = brain[i].inLength - 1; p >= 0; p--)
        {
          if(debug)
            cout << brain[i].inPath[p].dest << ' ';
          (fullFlows[brain[i].inPath[p].ID])++;
        }
        if(debug)
        {
          cout << endl;
          cout << " Empty Path: ";
        }
        for(int k = brain[i].outLength - 1; k >= 0; k--)
        {
          if(debug)
            cout << brain[i].outPath[k].dest << ' ';
          (emptyFlows[brain[i].outPath[k].ID])++;
        }
        if(debug)
          cout << endl;


        brain[i].fed = 1;
        totalFed++;
      }
    }
  }

  if(brain[cellCount - 1].fed == 0 && totalFed == cellCount - 1)
  {
    //cout << "Blood cell #0" << endl;
    //cout << " InPath: ";
    //printPath(brain[0].outPath, brain[0].outLength);
    //cout << inLength << ' ' << outLength << endl;
    if(checkCapacity(brain[0].outPath, brain[0].outLength, fullFlows, emptyFlows))
    {
      //cout << " Full Path: ";
      for(int k = 0; k < brain[0].outLength; k++)
      {
        //cout << brain[0].outPath[k].dest << ' ';
        (fullFlows[brain[0].outPath[k].ID])++;
      }
      //cout << endl;

      brain[cellCount - 1].fed = 1;
    }
  }

  for(int i = 0; i < cellCount; i++)
  {
    if(brain[i].fed)
      numFed++;
  }

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

  if(debug)
  {
    cout << ">Fed ";
    for(int i = 0; i < cellCount; i++)
    {
      cout << brain[i].fed << ' ';
    }
    cout << endl;

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
  }

  return numFed;  // to avoid warning for now
} // calcFlows()f

int Blood::generatePath(BrainCell &cell, Vessel2* p, int &length, int end)
{
  //int end = cellCount - 1;
  for(int i = 0; i < cell.outgoing; i++)
  {
    //cout << "P " << cell.out[0].src << ',' << i << endl;
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
          else
            length--;
        }
      }
      else
      {
        //cout << cell.out[i].ID << ',' << cell.out[i].dest << ' ';
        //cout << cell.out[i].dest << ' ';
        p[length++] = cell.out[i];
        if(generatePath(brain[cell.out[i].dest], p, length, end))
          return 1;
        else
          length--;
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

int Blood::generatePath2(BrainCell &cell, Vessel2* p, int &length, int end)
{
  //cout << "Cell ID#" << cell.ID << endl;
  //int end = cellCount - 1;
  if(cell.ID == end)
  {
    length = 0;
    return 1;
  }

  if(cell.ID == cellCount - 1)
    return 0;

  for(int i = 0; i < cell.outgoing; i++)
  {
    if(generatePath2(brain[cell.out[i].dest], p, length, end) == 1)
    {
      p[length++] = cell.out[i];
      return 1;
    }
  }

  return -1;
}

void Blood::generatePathStack(BrainCell &cell, Vessel2* p, int &length, int end)
{
  //cout << "Cell ID#" << cell.ID << endl;
  //int end = cellCount - 1;
  StackAr <Vessel2*> stack(100);

  for(int i = 0; i < cell.outgoing; i++)
  {
    cout << "PUSHED " << cell.out[i].ID << endl;
    stack.push(&(cell.out[i]));

  }

  while(!stack.isEmpty())
  {
    Vessel2 *cur = stack.topAndPop();
    cout << "POPPED " << cur->ID << endl;
    if(cur->dest == end)
    {
      cout << "MATCH " << cur->ID << ' ' << length << endl;
      if(length == 0)
        p[length++] = *cur;
      else
      {
        p[length - 1] = *cur;
      }
      return;
    }

    if(cur->dest == cellCount - 1 && cur->src != p[length].src)
      length--;
    else
      if(cur->dest != cellCount - 1)
        p[length++] = *cur;

    if(cur->dest != cellCount - 1)
      for(int k = 0; k < brain[cur->dest].outgoing; k++)
      {
        cout << "PUSHED " << brain[cur->dest].out[k].ID << endl;
        stack.push(&(brain[cur->dest].out[k]));
      }
    cout << "Length: " << length << " P: ";
    for(int i = 0; i < length; i++)
    {
      cout << p[i].ID << ' ';
    }
    cout << endl;
  }
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
  for(int i = length - 1; i >= 0; i--)
    cout << temp[i].dest << ',' << temp[i].ID << ' ';
  cout << endl;
}
