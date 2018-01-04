// Author: Sean Davis
#ifndef evacH
#define evacH

#include "EvacRunner.h"
#include "graph.h"
#include "BinaryHeap.h"
#include "QueueAr.h"

class Evac //we can make this our graph?
{
  int numCities;
  Graph graph;
  RoadNode tracker[200000];
  RoadNode reset[200000];
  bool activeCities[20000];
public:
	Evac(City *cities, int numCities, int numRoads);
	void addBackFlow(int destination, int orig, int flow);
	void evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes,
	int &routeCount); // student sets evacRoutes and routeCount
  int findFarestCity(int cityID, int& curDistance, int& furthestCity, int origCity);

	int findPathTo(int cityID, int sink, int prevCity, int& smallest, int* paths, int& pathCount);
};



#endif
