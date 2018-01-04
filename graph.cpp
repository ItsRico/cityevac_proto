#include "graph.h"


CityNode::CityNode() : roads(NULL), x(0), y(0), population(0), evacuees(0), roadCount(0), isVisited(false)
{

}

CityNode::CityNode(int I, RoadNode* r, int xe, int ye, int pop, int evacu, int rc, bool visited)
: ID(I), roads(new RoadNode[rc]), x(xe), y(ye), population(pop), evacuees(evacu), roadCount(rc),
 isVisited(visited), depth(-1)
{
  for(int i = 0; i < rc; i++)
  {
    roads[i].destinationCityID = r[i].destinationCityID;
    roads[i].peoplePerHour = r[i].peoplePerHour;
    roads[i].ID = r[i].ID;
    roads[i].sourceID = r[i].sourceID;
  }
}

void Graph::print(int numCities)
{
  for(int i = 0; i < numCities; i++)
  {
    adjList[i].print();
  }
}
