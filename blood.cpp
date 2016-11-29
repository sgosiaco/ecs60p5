#include "blood.h"
#include "bloodRunner.h"
#include "StackAr.h"
#include "QueueAr.h"
#include "vector.h"
#include <iostream>
#include <cstring>

using namespace std;

Blood::Blood(Vessel vessels[], int vesselCount, int cellCount, int depth) : vec(cellCount), queue(vesselCount + 1)
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

  delete [] count;
  for(int i = 0; i < cellCount; i++)
    delete [] temp[i];
  delete [] temp;
  delete [] vessel;
} // Blood()


int Blood::calcFlows(int fullFlows[], int emptyFlows[])
{
  for(int i = 0; i < vesselCount; i++)
    fullFlows[i] = emptyFlows[i] = 0;

  if(!pathsCreated)
  {
    /*
    BrainCell* inPathTemp = new BrainCell[depth];
    BrainCell* outPathTemp = new BrainCell[depth];
    int inLengthTemp = 0;
    int outLengthTemp = 0;
    */

    for(int i = 1; i < cellCount - 1; i++)
    {
      brain[i].inPath = new Vessel2[depth];
      brain[i].outPath = new Vessel2[depth];
      //generatePathStack(brain[0], inPathTemp, inLengthTemp, i);
      generatePathQueue(brain[0], brain[i].inPath, brain[i].inLength, i);
      generatePathQueue(brain[i], brain[i].outPath, brain[i].outLength, cellCount - 1);
      //generatePath3(brain[0], inPathTemp, inLengthTemp, i);
      //generatePath4(brain[i], outPathTemp, outLengthTemp, cellCount-1, i);

      if(debug)
      {
        /*
        cout << " Path3: ";
        for(int op = 0; op < inLengthTemp; op++)
        {
          cout << inPathTemp[op].ID << ' ';
        }
        cout << endl;
        */
        //printPath(inPathTemp, inLengthTemp);
        cout << " InPath: ";
        printPath(brain[i].inPath, brain[i].inLength);
        cout << " inLength: " << brain[i].inLength << endl;
        cout << " OutPath: ";
        printPath(brain[i].outPath, brain[i].outLength);
        //cout << inLength << ' ' << outLength << endl;
      }
    }
    brain[0].outPath = new Vessel2[depth];
    generatePathQueue(brain[0], brain[0].outPath, brain[0].outLength, cellCount - 1);
    pathsCreated = 1;

    for(int k = 0; k < brain[0].outLength; k++)
    {
      (emptyFlows[brain[0].outPath[k].ID])++;
    }

    brain[0].fed = 1;
    totalFed++;
  }

  /*
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
  */

  //feed everything except first node.
  for(int i = 1; i < cellCount - 1; i++)
  {
    int dontFeed = 0;

    if(debug)
      cout << "Blood cell #" << i << endl;

    if(brain[i].fed == 0)
    {

      for(int k = brain[i].inLength - 1; k >= 0; k--)
      {
        //cout << brain[i].inPath[k].src << ' ' << brain[i].inPath[k].dest << endl;
        int index = brain[i].inPath[k].src;
        if(brain[index].fed == 0)
        {
          dontFeed = 1;

          if(checkCapacityIn(brain[index].inPath, brain[index].inLength, fullFlows, emptyFlows) && checkCapacity(brain[index].outPath, brain[index].outLength, fullFlows, emptyFlows))
          {
            for(int p = brain[index].inLength - 1; p >= 0; p--)
              (fullFlows[brain[index].inPath[p].ID])++;
            for(int kp = brain[index].outLength - 1; kp >= 0; kp--)
              (emptyFlows[brain[index].outPath[kp].ID])++;
            brain[index].fed = 1;
            totalFed++;
          }


        }
      }

      if(checkCapacityIn(brain[i].inPath, brain[i].inLength, fullFlows, emptyFlows) && checkCapacity(brain[i].outPath, brain[i].outLength, fullFlows, emptyFlows) && !dontFeed)
      {
        for(int p = brain[i].inLength - 1; p >= 0; p--)
          (fullFlows[brain[i].inPath[p].ID])++;
        for(int k = brain[i].outLength - 1; k >= 0; k--)
          (emptyFlows[brain[i].outPath[k].ID])++;
        brain[i].fed = 1;
        totalFed++;
      }

    }
  }

  if(totalFed == cellCount - 1 && brain[cellCount - 1].fed == 0)
  {
    if(checkCapacity(brain[0].outPath, brain[0].outLength, fullFlows, emptyFlows))
    {
      for(int k = 0; k < brain[0].outLength; k++)
      {
        (fullFlows[brain[0].outPath[k].ID])++;
      }
      brain[cellCount - 1].fed = 1;
      totalFed++;
    }
  }

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

  return totalFed;  // to avoid warning for now
} // calcFlows()

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

void Blood::generatePath3(BrainCell &cell, BrainCell* p, int &length, int end)
{
  cell.visited = 1;
  p[length++] = cell;
  if(cell.ID == end)
  {
    if(debug)
    {
      for(int i = 0; i < length; i++)
        cout << p[i].ID << ' ';
      cout << endl;
    }
    brain[end].updateInPath(p, length);
  }
  else
  {
    for(int i = 0; i < cell.outgoing; i++)
    {
      if(brain[cell.out[i].dest].visited == 0)
        generatePath3(brain[cell.out[i].dest], p, length, end);
    }
  }
  length--;
  cell.visited = 0;
}

void Blood::generatePath4(BrainCell &cell, BrainCell* p, int &length, int end, int start)
{
  cell.visited = 1;
  p[length++] = cell;
  if(cell.ID == end)
  {
    if(debug)
    {
      for(int i = 0; i < length; i++)
        cout << p[i].ID << ' ';
      cout << endl;
    }
    brain[start].updateOutPath(p, length);
  }
  else
  {
    for(int i = 0; i < cell.outgoing; i++)
    {
      if(brain[cell.out[i].dest].visited == 0)
        generatePath4(brain[cell.out[i].dest], p, length, end, start);
    }
  }
  length--;
  cell.visited = 0;
}

void Blood::generatePathQueue(BrainCell &cell, Vessel2* p, int &length, int end)
{
  //Queue <BrainCell*> queue(vesselCount + 1);
  for(int i = 0; i < cellCount; i++)
    vec[i] = -1;
  queue.enqueue(&cell);
  vec[cell.ID] = cell.ID;
  while(!queue.isEmpty())
  {
    BrainCell *temp = queue.dequeue();
    if(temp->ID == end)
    {

      do {
        //cout << "source is" << vec[temp->ID] << " end is " << temp->ID << endl;
        for(int i = 0; i < brain[vec[temp->ID]].outgoing; i++)
        {
          if(brain[vec[temp->ID]].out[i].dest == temp->ID)
            p[length++] = brain[vec[temp->ID]].out[i];
        }
        temp = &(brain[vec[temp->ID]]);
      } while(temp->ID != cell.ID);

      while(!queue.isEmpty())
        queue.dequeue();

      return;
    }
    for(int i = 0; i < temp->outgoing; i++)
    {
      if(vec[temp->out[i].dest] == -1)
      {
        vec[temp->out[i].dest] = temp->ID;
        queue.enqueue(&(brain[temp->out[i].dest]));
      }
    }
  }
}

int Blood::checkCapacity(Vessel2* temp, int length, int full[], int empty[])
{
  for(int i = length - 1; i >= 0; i--)
  {
    //cout << "ID " << temp[i].ID << endl;
    if(full[temp[i].ID] + empty[temp[i].ID] + 1 > temp[i].capacity)
      return 0;
  }
  return 1;
}

int Blood::checkCapacityIn(Vessel2* temp, int length, int full[], int empty[])
{
  for(int i = length - 1; i >= 0; i--)
  {
    //cout << "ID " << temp[i].ID << endl;
    if(full[temp[i].ID] + empty[temp[i].ID] + 1 > temp[i].capacity || brain[temp[i].src].fed == 0)
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
