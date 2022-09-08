#ifndef __ROADWAY_H__
#define __ROADWAY_H__

#include <vector>
#include <list>
#include "Vehicle.h"
#include "Stoplight.h"
#include "Random.h"

class Roadway
{
private:
  std::vector<std::vector<Vehicle*>> roadway;

  std::vector<Vehicle*> northBoundVehicles;
  std::vector<Vehicle*> southBoundVehicles;
  std::vector<Vehicle*> eastBoundVehicles;
  std::vector<Vehicle*> westBoundVehicles;


  int northBoundPointer;
  int northBoundCounter;
  int southBoundPointer;
  int southBoundCounter;
  int eastBoundPointer;
  int eastBoundCounter;
  int westBoundPointer;
  int westBoundCounter;

  Stoplight stoplight;

  double prob_new_vehicle_northbound;
  double prob_new_vehicle_southbound;
  double prob_new_vehicle_eastbound;
  double prob_new_vehicle_westbound;

  // not actual road lenght, it is num of road sections before intersection
  // but I wanted a shorter name.
  int roadLength;
  //total length of lanes
  int totalLength;

  // correspond to positions in the roadway vector
  int total;
  int sw;
  int ne;

public:
  Roadway(int number_of_sections_before_intersection, double prob_new_vehicle_northbound,
    double prob_new_vehicle_southbound, double prob_new_vehicle_eastbound, double prob_new_vehicle_westbound);
  ~Roadway();

  Vehicle* getRoadwayAt(int pos1, int pos2);

  LightColor getNSLightColor();
  LightColor getEWLightColor();

  void moveTraffic();

private:
  void newVehicles();
  void moveAndDeleteVehiclesAtEnd();

  void moveSouth(int pos1, int pos2);
  void moveNorth(int pos1, int pos2);
  void moveEast(int pos1, int pos2);
  void moveWest(int pos1, int pos2);

  bool canMoveSouth(int pos1, int pos2);
  bool canMoveNorth(int pos1, int pos2);
  bool canMoveEast(int pos1, int pos2);
  bool canMoveWest(int pos1, int pos2);

  bool canGoStraightThroughNLight(int pos1, int pos2);
  bool canGoStraightThroughSLight(int pos1, int pos2);
  bool canGoStraightThroughELight(int pos1, int pos2);
  bool canGoStraightThroughWLight(int pos1, int pos2);

  bool canNTurnRight(int pos1, int pos2);
  bool canSTurnRight(int pos1, int pos2);
  bool canETurnRight(int pos1, int pos2);
  bool canWTurnRight(int pos1, int pos2);

  bool canNTurnLeft(int pos1, int pos2);
  bool canSTurnLeft(int pos1, int pos2);
  bool canETurnLeft(int pos1, int pos2);
  bool canWTurnLeft(int pos1, int pos2);

  bool checkNTurnLeft(int pos1, int pos2);
  bool checkSTurnLeft(int pos1, int pos2);
  bool checkETurnLeft(int pos1, int pos2);
  bool checkWTurnLeft(int pos1, int pos2);

  void moveNLeft(int pos1, int pos2);
  void moveSLeft(int pos1, int pos2);
  void moveELeft(int pos1, int pos2);
  void moveWLeft(int pos1, int pos2);
};

#endif
