#ifndef GRAPH_H
#define GRAPH_H

  #include "EvacRunner.h"
  #include <iostream>
  #include "BinaryHeap.h"

  using namespace std;

  class RoadNode
  {
  public:
    int destinationCityID;
    int peoplePerHour;
    int ID;
    int sourceID;
    void print()
    {
      cout << "destinationCityID: "  << destinationCityID << " sourceID: " << sourceID <<  " peoplePerHour: " << peoplePerHour << " ID: " << ID << "\n";
    }
  };

  class CityNode
  {
    public:
    int ID;
    RoadNode* roads;
    int x;
    int y;
    //maybe we can calculate the distance between each city for the most optimal path?
    int population;
    int evacuees;
    int roadCount;
    bool isVisited;
    int depth;
    //we might eve need this, but its worth a shot

    CityNode();
    CityNode(int I, RoadNode* r, int xe, int ye, int pop, int evacu, int rc, bool visited);
    void print()
    {
      cout << "ID: " << ID << " \n";
      for(int i = 0; i < roadCount; i++)
      {
        roads[i].print();
      }
      cout << "X: " << x << " Y: " << y << " pop: " << population << " evacuees: " << evacuees << " roadCount: " << roadCount << "\n";

    }
  }; //CityNode


  class Graph
  {

  public:
    CityNode adjList[200000];

    void print(int numCities);
  }; //Graph


#endif
