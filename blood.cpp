#include "blood.h"
#include "bloodRunner.h"
#include <iostream>
#include <cstring>

using namespace std;

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
} // Blood()


int Blood::calcFlows(int fullFlows[], int emptyFlows[])
{
  /*
  for(int i = 0; i < brain[0].outgoing; i++) //look at each path out of the origin cell
  {
    int temp = brain[0].out[i].dest; //get each destination cell from the ith vessel
    int capacity = brain[0].out[i].capacity; //get capacity of the ith vessel
    for(int k = 0; k < capacity; k++) //continue down a path while we still have capacity
    {
      if(!(brain[temp].fed))
      {
        brain[temp].fed = 1;
      }
      temp = brain[temp].out[0].dest;
    }

  }
  */
  /*
  for(int i = 0; i < brain[0].outgoing; i++)
  {
    int pos = brain[0].out[i].dest;
    int cap = brain[0].out[i].capacity;
    path(brain[pos], cap, brain[0].out[i]);
  }
  */
  for(int i = 0; i < vesselCount; i++)
    fullFlows[i] = emptyFlows[i] = 0;

  int numFed = 0;
  if(!brain[0].fed)
    brain[0].fed = 1;

  path(brain[0], brain[0].out[0].capacity, brain[0].out[0]);

  cout << ">Fed ";
  for(int i = 0; i < cellCount; i++)
  {
    cout << brain[i].fed << ' ';
    if(brain[i].fed)
      numFed++;
  }
  cout << endl;

  /*
  for(int i = 0; i < vesselCount; i++)
  {
    cout << vessel[i].ID << ' ' << vessel[i].carrying << endl;
  }
  */

  for(int i = 0; i < cellCount; i++)
  {
    for(int k = 0; k < brain[i].outgoing; k++)
    {
      //scout << i << ' ' << brain[i].out[k].ID << ' ' << brain[i].out[k].carrying << endl;
      if(brain[i].out[k].carrying > 0)
      {
        int ID = brain[i].out[k].ID;
        int out = brain[i].out[k].dest;
        int ez = 0;
        (fullFlows[ID])++;
        if(out != cellCount - 1)
        {
          //find path to end node.
          for(int p = 0; p < brain[out].outgoing; p++)
          {
            if(brain[out].out[p].dest == cellCount - 1)
            {
              emptyFlows[brain[out].out[p].ID] += brain[i].out[k].carrying;
              ez = 1;
              break;
            }
          }
        }
        brain[i].out[k].carrying = 0;
      }
    }
  }

  /*
  file 6-12-1
  if(brain[4].fed == 0)
  {
    emptyFlows[6] = 1;
    emptyFlows[8] = 2;
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

int Blood::path(BrainCell &cell, int flow, Vessel2 &prev)
{
  for(int i = 0; i < cell.outgoing; i++)
  {
    Vessel2 *vessel = &(cell.out[i]);
    int dest = vessel->dest;
    if(flow == 0)
      flow = vessel->capacity;
    cout << "Vessel:" << vessel->ID << ' ' << vessel->src << ' ' << dest << ' ' << brain[dest].fed << ' ' << vessel->carrying << ' ' << flow << endl;
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
