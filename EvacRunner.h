#ifndef EVACRUNNER_H
  #define EVACRUNNER_H

#include <iostream> // for NULL

using namespace std;

class Road
{
public:
  int destinationCityID;
  int peoplePerHour;
  int ID;
  void print()
  {
    cout << "destinationCityID: " << destinationCityID << " peoplePerHour: " << peoplePerHour << " ID: " << ID << "\n"; 
  }
}; // class Road

class Road2
{
public:
  int destinationCityID;
  int sourceCityID;
  int peoplePerHour;
  int peopleThisHour;
  Road2():peopleThisHour(0){}
}; //class Road2

class City
{
public:
  int ID;
  int x;
  int y;
  int population;
  int evacuees;
  Road *roads;
  int roadCount;

  City() : evacuees(0), roads(NULL), roadCount(0){}
  ~City(){delete [] roads;}
}; // class City

class EvacRoute
{
public:
  int roadID;
  int time;
  int numPeople;
  void print()
  {
    cout << "roadID: " << roadID << " " << "time: " << time << " numPpl: " << numPeople << "\n";
  }
  bool operator< (const EvacRoute &rhs) const;
}; // EvacRoute;

#endif
