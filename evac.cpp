#include <cstdlib>
#include "evac.h"
#include "EvacRunner.h"
#include <math.h>

using namespace std;

int compare (const void * a, const void * b)
{
  const Road* p = (Road*)a;
  const Road* q = (Road*)b;
  return ( q->peoplePerHour  -  p->peoplePerHour);
}


Evac::Evac(City *citie, int numCitie, int numRoads) : numCities(numCitie)
{

  for(int i = 0; i < numCitie; i++)
  {
    RoadNode* tempRoads = new RoadNode[numRoads];

    for(int j = 0; j < citie[i].roadCount; j++)
    {
      tempRoads[j].destinationCityID = citie[i].roads[j].destinationCityID;
      tempRoads[j].sourceID = i;
      tempRoads[j].peoplePerHour = citie[i].roads[j].peoplePerHour;
      tempRoads[j].ID = citie[i].roads[j].ID;


      tracker[tempRoads[j].ID] = tempRoads[j];
    } //copy the roads


    qsort (tempRoads, citie[i].roadCount, sizeof(RoadNode), compare); //Q sort here

    CityNode tempCity(citie[i].ID, tempRoads, citie[i].x, citie[i].y, citie[i].population, citie[i].evacuees, citie[i].roadCount, false);
    graph.adjList[citie[i].ID] = tempCity;
  } //copy the city
  //cout << numCities << "\n";
  //graph.print(numCities);
} // Evac()

void Evac::addBackFlow(int destination, int orig, int flow)
{
  for(int i = 0; i < graph.adjList[destination].roadCount; i++)
  {
    RoadNode road = graph.adjList[destination].roads[i];
    if(road.destinationCityID == orig)
    {
      graph.adjList[destination].roads[i].peoplePerHour += flow;
    }
  }
}

void Evac::evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes,
    int &routeCount)
{

  routeCount = 0;
  int curDistance = 0;
  int furthestCity = -99;
  int sink = findFarestCity(evacIDs[0], curDistance, furthestCity, evacIDs[0]);
  int smallest = 999999;
  int* path = new int[200000];
  int pathCount = 0;
  int t = 1;
  int total = 0;
  int offSet = 0;
  int initCount  = 0;

    activeCities[sink] = true;

  while(true)
  {//finds first path

    int check = findPathTo(evacIDs[0], sink, -99, smallest, path, pathCount);

    if (check == 1)
      break;
    //evac maintence
    for(int i = 0; i < pathCount; i++)
    {

      evacRoutes[routeCount].roadID = path[i];
      evacRoutes[routeCount].time = 1;
      evacRoutes[routeCount].numPeople = smallest;
      routeCount++;
    }

    total += smallest;
    smallest = 999999;
    offSet += pathCount;
    pathCount = 0;
  }

  graph.adjList[evacIDs[0]].population -= total;
  graph.adjList[sink].evacuees += total;
  cout << "city 1's pop is: " << graph.adjList[evacIDs[0]].population << "and it's evacuees is :" << graph.adjList[sink].evacuees << "which is at city: " << sink << endl;


  int orig  = 0;

  while(graph.adjList[evacIDs[0]].population > 0 )
  {
    t++;

    for(int i = initCount; i < offSet ;i++ )
    {
      evacRoutes[routeCount].roadID = evacRoutes[i].roadID;
      evacRoutes[routeCount].time = t;
      evacRoutes[routeCount].numPeople = evacRoutes[i].numPeople;

      if(orig != evacRoutes[i].numPeople)
      {
        orig = evacRoutes[i].numPeople;
        graph.adjList[evacIDs[0]].population -= orig;
        graph.adjList[sink].evacuees += orig;
        cout << "city 1's pop is: " << graph.adjList[evacIDs[0]].population << " and it's evacuees is :" << graph.adjList[sink].evacuees << "  which is at city:  " << sink << endl;
        cout << "this many evacuees on this path: " << orig << endl;
        if(graph.adjList[sink].evacuees > graph.adjList[sink].population)
        {//we know our sinks max capacity has been reached by this iteration so we need to find a new sink
          graph.adjList[sink].evacuees -= orig;
          break;
          cout << "we fucked up but it's chill   < << < << < < < " << endl;
        }

        if(graph.adjList[evacIDs[0]].population < 0)
        {
          cout << orig << graph.adjList[evacIDs[0]].population << endl;
          evacRoutes[routeCount].numPeople = orig + graph.adjList[evacIDs[0]].population;
          routeCount++;

          break;
        }
      }
      routeCount++;
    }


  }

  graph.adjList[sink].evacuees += graph.adjList[evacIDs[0]].population;

  cout << "city 1's pop is: " << graph.adjList[evacIDs[0]].population << "  and it's evacuees is :" << graph.adjList[sink].evacuees << "  which is at city:  " << sink << endl;

  initCount += offSet;

float loadFactor = (float) graph.adjList[5].population / graph.adjList[5].evacuees;
if(loadFactor >  .90)
{
  //set that city to innactive
  activeCities[sink] = false;
}

//road maintenance
  for(int i = 0; i < numCities; i++)
  {
    for(int j = 0; j < graph.adjList[i].roadCount; j++)
    {
      tracker[graph.adjList[i].roads[j].ID] = graph.adjList[i].roads[j];
    }
  }

//  cout << "******************************************\n";
  curDistance = 0;
  furthestCity = -99;
  sink = findFarestCity(evacIDs[1], curDistance, furthestCity, evacIDs[1]);

//  cout << "BEFORE RUNNING NEXT THING BEFORE B4   "<<graph.adjList[5].population << endl;


  while(true)
  {

  int check = findPathTo(evacIDs[1], sink, evacIDs[1], smallest, path, pathCount);

    if (check == 1)
      break;
    //evac maintence



    for(int i = 0; i < pathCount; i++)
    {

      evacRoutes[routeCount].roadID = path[i];
      evacRoutes[routeCount].time = t;
      evacRoutes[routeCount].numPeople = smallest;
      cout << evacRoutes[routeCount].numPeople << endl;
      routeCount++;
    }

      total += smallest;
      smallest = 999999;
      offSet += pathCount;
      pathCount = 0;
  }


  graph.adjList[evacIDs[1]].population -= total;
/*
  orig  = 0;
  int b = t;


  //   cout << "BEFORE RUNNING NEXT THING "<<graph.adjList[5].evacuees << endl;

  while(t <= b + 1)
  {
    t++;
    cout << "INITCOUNT : " << initCount << endl;
    for(int i = initCount; i < offSet ;i++ )
    {


      evacRoutes[routeCount].roadID = evacRoutes[i].roadID;
      evacRoutes[routeCount].time = t;
      evacRoutes[routeCount].numPeople = evacRoutes[i].numPeople;
      cout << graph.adjList[5].population << endl;

      if(orig != evacRoutes[i].numPeople)
      {
        orig = evacRoutes[i].numPeople;

        graph.adjList[evacIDs[1]].population -= orig;

        if(graph.adjList[evacIDs[1]].population < 0)
        {
          evacRoutes[routeCount].numPeople = orig + graph.adjList[evacIDs[1]].population;
          routeCount++;

          break;
        }
      }
      routeCount++;
    }


  }

  initCount += offSet;
  cout << t << endl;
  cout << graph.adjList[evacIDs[0]].evacuees << endl;
*/
} // evacuate

int Evac::findPathTo(int cityID, int sink, int prevCity, int& smallest, int* paths, int& pathCount)
{
  for(int i = 0; i < graph.adjList[cityID].roadCount; i++)
  {
    RoadNode roadToTake = graph.adjList[cityID].roads[i];

  //  cout << graph.adjList[5].population << endl;
//cout << graph.adjList[5].evacuees << endl;
    if(  roadToTake.destinationCityID == 5 && activeCities[roadToTake.destinationCityID] == 0 )   //if this is true, we need to find another path to our sink
    {
      for(int i = 0; i < graph.adjList[cityID].roadCount; i++)
      {
        cout <<"i COULD GO TO : " <<graph.adjList[cityID].roads[i].destinationCityID << "    asdf "<< endl;
      }
      cout << activeCities[roadToTake.destinationCityID] << "   destinationCityID : " << roadToTake.destinationCityID << endl;
      cout << "this city is full af and i cant use it" << endl;
      cout << graph.adjList[cityID].roadCount << endl;
      return 0;

    }
    if(roadToTake.destinationCityID == sink && tracker[roadToTake.ID].peoplePerHour > 0)
    {

      if(tracker[roadToTake.ID].peoplePerHour < smallest)
      {
        smallest = tracker[roadToTake.ID].peoplePerHour;
      }
  //    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`\n";
//      cout << "SMALLEST: " << smallest << "\n";
//      cout << "I took the path from city " << cityID << " to " << roadToTake.destinationCityID << " and took " << smallest << " people with me" <<"\n";
      tracker[roadToTake.ID].peoplePerHour -= smallest;
//      cout << "I had: " << roadToTake.peoplePerHour << " and ";
      if(tracker[roadToTake.ID + 1].destinationCityID == cityID)
      {
//         cout << "my backflow was: " << tracker[roadToTake.ID + 1].peoplePerHour << "\n";
        tracker[roadToTake.ID + 1].peoplePerHour += smallest;
//         cout << "now it is: " << tracker[roadToTake.ID + 1].peoplePerHour << "\n";

      }
      else
      {
  //      cout << "my backflow was: " << tracker[roadToTake.ID - 1].peoplePerHour << "\n";
        tracker[roadToTake.ID - 1].peoplePerHour += smallest;
  //      cout << "now it is: " << tracker[roadToTake.ID - 1].peoplePerHour << "\n";
      }

  //    cout << " now my forward flow is: " << tracker[roadToTake.ID].peoplePerHour << "\n";

  //    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`\n";
      //tracker[roadToTake.]
      paths[pathCount] = roadToTake.ID;
      pathCount++;
      return 0 ;
    }
  }

  graph.adjList[cityID].isVisited = true;
  RoadNode roadToTake = graph.adjList[cityID].roads[0];
  int i = 0;
  while(roadToTake.destinationCityID == prevCity || tracker[roadToTake.ID].peoplePerHour <= 0 || graph.adjList[roadToTake.destinationCityID].isVisited)
  {
    roadToTake = tracker[graph.adjList[cityID].roads[i].ID];
    if(i == graph.adjList[cityID].roadCount)
    {
      cout << "best possible path\n";
      return 1;
    }
    i++;
  }

  if(tracker[roadToTake.ID].peoplePerHour < smallest)
  {
    smallest = tracker[roadToTake.ID].peoplePerHour;
  }
  paths[pathCount] = roadToTake.ID;
  pathCount++;
  int check = findPathTo(roadToTake.destinationCityID, sink, cityID, smallest, paths, pathCount);
  if(check == 1)
  {
    return 1;
  }
  //cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`\n";
  //cout << "I took the path from city " << cityID << " to " << roadToTake.destinationCityID << " and took " << smallest << " people with me" <<"\n";
  tracker[roadToTake.ID].peoplePerHour -= smallest;
  //cout << "I had: " << roadToTake.peoplePerHour << " and ";
  if(tracker[roadToTake.ID + 1].destinationCityID == cityID)
  {
    //cout << "my backflow was: " << tracker[roadToTake.ID + 1].peoplePerHour << "\n";
    tracker[roadToTake.ID + 1].peoplePerHour += smallest;

  }
  else
  {
    //cout << "my backflow was: " << tracker[roadToTake.ID - 1].peoplePerHour << "\n";
    tracker[roadToTake.ID - 1].peoplePerHour += smallest;

  }
  //cout << " now my forward flow is: " << tracker[roadToTake.ID].peoplePerHour << "\n";
  //cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`\n";

  return 0;
}

int Evac::findFarestCity(int cityID, int& curDistance, int& furthestCity, int origCity)
{

  for(int i = 0; i< graph.adjList[cityID].roadCount; i++)
  {
    RoadNode roadToCheck = graph.adjList[cityID].roads[i];

    int x = graph.adjList[roadToCheck.destinationCityID].x;
    int y = graph.adjList[roadToCheck.destinationCityID].y;
    int x2 = graph.adjList[origCity].x;
    int y2 = graph.adjList[origCity].y;
    int distance = sqrt((pow(x - x2, 2) + pow(y - y2, 2)));

    if(distance > curDistance)
    {
      curDistance = distance;
      furthestCity = roadToCheck.destinationCityID;
      findFarestCity(furthestCity, curDistance, furthestCity, origCity);
    }
  }
  return furthestCity;

} //findFarestCity
