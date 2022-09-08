#ifndef __ROADWAY_CPP__
#define __ROADWAY_CPP__

#include "Roadway.h"
#include <fstream>
#include <vector>

#include <iostream>


Roadway::Roadway(int number_of_sections_before_intersection, double prob_new_vehicle_northbound,
  double prob_new_vehicle_southbound, double prob_new_vehicle_eastbound, double prob_new_vehicle_westbound)
  : roadLength(number_of_sections_before_intersection),
    totalLength(2 * roadLength + 2),
    prob_new_vehicle_northbound(prob_new_vehicle_northbound),
    prob_new_vehicle_southbound(prob_new_vehicle_southbound),
    prob_new_vehicle_eastbound(prob_new_vehicle_eastbound),
    prob_new_vehicle_westbound(prob_new_vehicle_westbound)
{
  northBoundPointer = 0;
  northBoundCounter = 0;
  southBoundPointer = 0;
  southBoundCounter = 0;
  eastBoundPointer = 0;
  eastBoundCounter = 0;
  westBoundPointer = 0;
  westBoundCounter = 0;

  total = totalLength - 1;
  sw = roadLength;
  ne = roadLength + 1;

  // set roadway vector to null
  // note that most entries will be null since the four lanes only take up
  // a small portion of the entire 2D vector
  roadway = std::vector<std::vector<Vehicle*>>(totalLength, std::vector<Vehicle*>(totalLength, nullptr));
}

Roadway::~Roadway(){}

Vehicle* Roadway::getRoadwayAt(int pos1, int pos2)
{
  return roadway.at(pos1).at(pos2);
}

LightColor Roadway::getNSLightColor()
{
  return stoplight.getNSLightColor();
}

LightColor Roadway::getEWLightColor()
{
  return stoplight.getEWLightColor();
}

// moves all the traffic on the roadway
// Starts by moving the last spot on each lane and deleting the vehicle if it
// goes completely off the roadway.
// Then moves all the vehicles that have gone through the intersection.
// Next, it moves all the vehicles in the intersection.
// Then it moves vehicles at the stoplight (right before intersection).
// Last it move vehicles before stoplight.
void Roadway::moveTraffic()
{
  // Move vehicles off the last section of each lane
  // Delete Vehicles that move completely off the roadway
  moveAndDeleteVehiclesAtEnd();


  // Move traffic beyond stoplight
  for (int i = 1; i < roadLength; i ++)
  {
    //move traffic beyond stoplight in southbound lane
    if(canMoveSouth(sw, total - i))
    {
      moveSouth(sw, total - i);
    }
    //move traffic beyond stoplight in northbound lane
    if(canMoveNorth(ne, i))
    {
      moveNorth(ne, i);
    }
    //move traffic beyond stoplight in easthbound lane
    if(canMoveEast(total - i, ne))
    {
      moveEast(total - i, ne);
    }
    //move traffic beyond stoplight in westhbound lane
    if(canMoveWest(i, sw))
    {
      moveWest(i, sw);
    }
  }


  // Move traffic inside intersection

  int m = sw;
  int n = ne;
  Vehicle* vehicle = roadway.at(m).at(n);

  if (vehicle != nullptr)
  {
    if (vehicle->getVehicleOriginalDirection() == Direction::north)// could only be turning left
    {
      if (checkNTurnLeft(m, n))
      {
        moveNLeft(m, n);
      }
    }
    else if (vehicle->getVehicleOriginalDirection() == Direction::south)// could only be going straight
    {
      if (canMoveSouth(m, n))
      {
        moveSouth(m, n);
      }
    }
  }

  m = ne;
  n = sw;
  vehicle = roadway.at(m).at(n);

  if (vehicle != nullptr)
  {
    if (vehicle->getVehicleOriginalDirection() == Direction::south)
    {
      if (checkSTurnLeft(m, n))
      {
        moveSLeft(m, n);
      }
    }
    else if (vehicle->getVehicleOriginalDirection() == Direction::north)
    {
      if (canMoveNorth(m, n))
      {
        moveNorth(m, n);
      }
    }
  }

  m = sw;
  n = sw;
  vehicle = roadway.at(m).at(n);

  if (vehicle != nullptr)
  {
    if (vehicle->getVehicleOriginalDirection() == Direction::east)
    {
      if (checkETurnLeft(m, n))
      {
        moveELeft(m, n);
      }
    }
    else if (vehicle->getVehicleOriginalDirection() == Direction::west)
    {
      if (canMoveWest(m, n))
      {
        moveWest(m, n);
      }
    }
    else // vehicle should be moving south
    {
      if (vehicle->getVehicleTurningDirection() == Turning::right)
      {
        if (canMoveWest(m, n))
        {
          moveWest(m, n);
        }
      }
      else // going straight
      {
        // make sure it is not going to run over a vehicle turning left
        bool check = roadway.at(m + 1).at(n + 2) != nullptr && roadway.at(m + 1).at(n + 2) == roadway.at(m - 1).at(n);
        if (canMoveSouth(m, n) && !check)
        {
          moveSouth(m, n);
        }
      }
    }
  }

  m = ne;
  n = ne;
  vehicle = roadway.at(m).at(n);

  if (vehicle != nullptr)
  {
    if (vehicle->getVehicleOriginalDirection() == Direction::west)
    {
      if (checkWTurnLeft(m, n))
      {
        moveWLeft(m, n);
      }
    }
    else if (vehicle->getVehicleOriginalDirection() == Direction::east)
    {
      if (canMoveEast(m, n))
      {
        moveEast(m, n);
      }
    }
    else // vehicle should be moving north
    {
      if (vehicle->getVehicleTurningDirection() == Turning::right)
      {
        if (canMoveEast(m, n))
        {
          moveEast(m, n);
        }
      }
      else
      {
        // make sure it is not going to run over a vehicle turning left
        bool check = roadway.at(m - 1).at(n - 2) != nullptr && roadway.at(m - 1).at(n - 2) == roadway.at(m + 1).at(n);
        if (canMoveNorth(m, n) && !check)
        {
          moveNorth(m, n);
        }
      }
    }
  }

  m = sw;
  n = ne;
  vehicle = roadway.at(m).at(n);

  if (vehicle != nullptr)
  {
    if (vehicle->getVehicleOriginalDirection() == Direction::east)
    {
      if (vehicle->getVehicleTurningDirection() == Turning::right)
      {
        if (canMoveSouth(m, n))
        {
          moveSouth(m, n);
        }
      }
      else
      {
        // make sure it is not going to run over a vehicle turning left
        bool check = roadway.at(m + 2).at(n - 1) != nullptr && roadway.at(m + 2).at(n - 1) == roadway.at(m).at(n + 1);
        if (canMoveEast(m, n) && !check)
        {
          moveEast(m, n);
        }
      }
    }
  }

  m = ne;
  n = sw;
  vehicle = roadway.at(m).at(n);

  if (vehicle != nullptr)
  {
    if (vehicle->getVehicleOriginalDirection() == Direction::west)
    {
      if (vehicle->getVehicleTurningDirection() == Turning::right)
      {
        if (canMoveNorth(m, n))
        {
          moveNorth(m, n);
        }
      }
      else
      {
        // make sure it is not going to run over a vehicle turning left
        bool check = roadway.at(m - 2).at(n + 1) != nullptr && roadway.at(m - 2).at(n + 1) == roadway.at(m).at(n - 1);
        if (canMoveWest(m, n) && !check)
        {
          moveWest(m, n);
        }
      }
    }
  }


  // Move traffic at stoplights

  // Traffic moving south
  if (roadway.at(sw).at(sw - 1) != nullptr )
  {
    int k = sw;
    int l = sw - 1;
    Vehicle* vehicle = roadway.at(k).at(l);

    if (vehicle->getVehicleTurningDirection() == Turning::left)
    {
      if (canSTurnLeft(k, l) && checkSTurnLeft(k, l))
      {
        moveSLeft(k, l);
      }
    }
    else if(vehicle->getVehicleTurningDirection() == Turning::straight)
    {
      if (canGoStraightThroughSLight(k, l) && canMoveSouth(k, l))
      {
        moveSouth(k, l);
      }
    }
    else // vehicle is turning right (seperate from straight because it can get through the light faster)
    {
      if (canSTurnRight(k, l) && canMoveSouth(k, l))
      {
        moveSouth(k, l);
      }
    }
  }

  // Traffic moving north
  if (roadway.at(ne).at(ne + 1) != nullptr )
  {
    int k = ne;
    int l = ne + 1;
    Vehicle* vehicle = roadway.at(k).at(l);

    if (vehicle->getVehicleTurningDirection() == Turning::left)
    {
      if (canNTurnLeft(k, l) && checkNTurnLeft(k, l))
      {
        moveNLeft(k, l);
      }
    }
    else if(vehicle->getVehicleTurningDirection() == Turning::straight)
    {
      if (canGoStraightThroughNLight(k, l) && canMoveNorth(k, l))
      {
        moveNorth(k, l);
      }
    }
    else // vehicle is turning right
    {
      if (canNTurnRight(k, l) && canMoveNorth(k, l))
      {
        moveNorth(k, l);
      }
    }
  }

  // Traffic moving east
  if (roadway.at(roadLength - 1).at(ne) != nullptr )
  {
    int k = roadLength - 1;
    int l = ne;
    Vehicle* vehicle = roadway.at(k).at(l);

    if (vehicle->getVehicleTurningDirection() == Turning::left)
    {
      if (canETurnLeft(k, l) && checkETurnLeft(k, l))
      {
        moveELeft(k, l);
      }
    }
    else if(vehicle->getVehicleTurningDirection() == Turning::straight)
    {
      if (canGoStraightThroughELight(k, l) && canMoveEast(k, l))
      {
        moveEast(k, l);
      }
    }
    else // vehicle is turning right
    {
      if (canETurnRight(k, l) && canMoveEast(k, l))
      {
        moveEast(k, l);
      }
    }
  }

  // traffic moving west
  if (roadway.at(roadLength + 2).at(sw) != nullptr )
  {
    int k = roadLength + 2;
    int l = sw;
    Vehicle* vehicle = roadway.at(k).at(l);

    if (vehicle->getVehicleTurningDirection() == Turning::left)
    {
      if (canWTurnLeft(k, l) && checkWTurnLeft(k, l))
      {
        moveWLeft(k, l);
      }
    }
    else if(vehicle->getVehicleTurningDirection() == Turning::straight)
    {
      if (canGoStraightThroughWLight(k, l) && canMoveWest(k, l))
      {
        moveWest(k, l);
      }
    }
    else // vehicle is turning right
    {
      if (canWTurnRight(k, l) && canMoveWest(k, l))
      {
        moveWest(k, l);
      }
    }
  }


  // Move traffic before stoplight
  for (int i = 0; i < roadLength - 1; i ++)
  {
    //move traffic before stoplight in southbound lane
    if(canMoveSouth(sw, roadLength - 2 - i))
    {
      moveSouth(sw, roadLength - 2 - i);
    }
    //move traffic before stoplight in northbound lane
    if(canMoveNorth(ne, roadLength + 3 + i))
    {
      moveNorth(ne, roadLength + 3 + i);
    }
    //move traffic before stoplight in easthbound lane
    if(canMoveEast(roadLength - 2 - i, ne))
    {
      moveEast(roadLength - 2 - i, ne);
    }
    //move traffic before stoplight in westhbound lane
    if(canMoveWest(roadLength + 3 + i, sw))
    {
      moveWest(roadLength + 3 + i, sw);
    }
  }


  // add any new vehicle coming onto roadway
  newVehicles();


  //update stoplights
  stoplight.updateStoplights();
}


// helper method to move and delete vehicles at the end of the lanes
void Roadway::moveAndDeleteVehiclesAtEnd()
{
  // southbound
  int k = sw;
  int l = total;

  if (roadway.at(k).at(l) != nullptr)
  {
    if (roadway.at(k).at(l - 1) != roadway.at(k).at(l)) // check if this is the last section of the vehicle
    {
      Direction dir = roadway.at(k).at(l)->getVehicleOriginalDirection();
      if (dir == Direction::south)
      {
        southBoundVehicles.erase(southBoundVehicles.begin()); //should always be the first vehicle

        southBoundPointer --;
      }
      else if (dir == Direction::east)
      {
         eastBoundVehicles.erase(eastBoundVehicles.begin());

        eastBoundPointer --;
      }
      else if (dir == Direction::west)
      {
        westBoundVehicles.erase(westBoundVehicles.begin());

        westBoundPointer --;
      }
      // No U turns, so do not need to check if it is from north bound lane
      Vehicle* vehicleToDelete = roadway.at(k).at(l);
      delete vehicleToDelete;
    }
    roadway.at(k).at(l) = nullptr;
  }

  // northbound
  k = ne;
  l = 0;

  if (roadway.at(k).at(l) != nullptr)
  {
    if (roadway.at(k).at(l + 1) != roadway.at(k).at(l)) // check if this is the last section of the vehicle
    {
      Direction dir = roadway.at(k).at(l)->getVehicleOriginalDirection();
      if (dir == Direction::north)
      {
        northBoundVehicles.erase(northBoundVehicles.begin()); //should always be the first vehicle
        northBoundPointer --;
      }
      else if (dir == Direction::east)
      {
        eastBoundVehicles.erase(eastBoundVehicles.begin());
        eastBoundPointer --;
      }
      else if (dir == Direction::west)
      {
        westBoundVehicles.erase(westBoundVehicles.begin());
        westBoundPointer --;
      }
      // No U turns, so do not need to check if it is from south bound lane
      Vehicle* vehicleToDelete = roadway.at(k).at(l);
      delete vehicleToDelete;
    }
    roadway.at(k).at(l) = nullptr;
  }

  // eastbound
  k = total;
  l = ne;

  if (roadway.at(k).at(l) != nullptr)
  {
    if (roadway.at(k - 1).at(l) != roadway.at(k).at(l)) // check if this is the last section of the vehicle
    {
      Direction dir = roadway.at(k).at(l)->getVehicleOriginalDirection();

      if (dir == Direction::east)
      {
        eastBoundVehicles.erase(eastBoundVehicles.begin());
        eastBoundPointer --;
      }
      else if (dir == Direction::north)
      {
        northBoundVehicles.erase(northBoundVehicles.begin()); //should always be the first vehicle
        northBoundPointer --;
      }
      else if (dir == Direction::south)
      {
        southBoundVehicles.erase(southBoundVehicles.begin());
        southBoundPointer --;
      }
      // No U turns, so do not need to check if it is from west bound lane
      Vehicle* vehicleToDelete = roadway.at(k).at(l);
      delete vehicleToDelete;
    }
    roadway.at(k).at(l) = nullptr;
  }

  // westbound
  k = 0;
  l = sw;

  if (roadway.at(k).at(l) != nullptr)
  {
    if (roadway.at(k + 1).at(l) != roadway.at(k).at(l)) // check if this is the last section of the vehicle
    {
      Direction dir = roadway.at(k).at(l)->getVehicleOriginalDirection();
      if (dir == Direction::west)
      {
        westBoundVehicles.erase(westBoundVehicles.begin());
        westBoundPointer --;
      }
      else if (dir == Direction::north)
      {
        northBoundVehicles.erase(northBoundVehicles.begin()); //should always be the first vehicle

        northBoundPointer --;
      }
      else if (dir == Direction::south)
      {
        southBoundVehicles.erase(southBoundVehicles.begin());

        southBoundPointer --;
      }
      // No U turns, so do not need to check if it is from east bound lane
      Vehicle* vehicleToDelete = roadway.at(k).at(l);
      delete vehicleToDelete;
    }
    roadway.at(k).at(l) = nullptr;
  }

  // if not deleting vehicles, can use these four lines instead
  // roadway.at(sw).at(total) = nullptr;
  // roadway.at(ne).at(0) = nullptr;
  // roadway.at(total).at(ne) = nullptr;
  // roadway.at(0).at(sw) = nullptr;
}

// helper method to add the new vehicles
void Roadway::newVehicles()
{
  // northbound vehicles
  if (Random::getNextRandom() <= prob_new_vehicle_northbound)
  {
    Vehicle* vehicle = new Vehicle(Direction::north);
    northBoundVehicles.push_back(vehicle);
  }

  // southbound vehicles
  if (Random::getNextRandom() <= prob_new_vehicle_southbound)
  {
    Vehicle* vehicle = new Vehicle(Direction::south);
    southBoundVehicles.push_back(vehicle);
  }

  // eastbound vehicles
  if (Random::getNextRandom() <= prob_new_vehicle_eastbound)
  {
    Vehicle* vehicle = new Vehicle(Direction::east);
    eastBoundVehicles.push_back(vehicle);
  }

  // westbound vehicles
  if (Random::getNextRandom() <= prob_new_vehicle_westbound)
  {
    Vehicle* vehicle = new Vehicle(Direction::west);
    westBoundVehicles.push_back(vehicle);
  }

  // add vehicles to lanes from vectors
  // northbound vehicles
  if (northBoundPointer < northBoundVehicles.size() && roadway.at(ne).at(total) == nullptr)
  {
    roadway.at(ne).at(total) = northBoundVehicles.at(northBoundPointer);
    northBoundCounter++;
    if(northBoundCounter == northBoundVehicles.at(northBoundPointer)->getVehicleLength())

    {
      northBoundPointer++;
      northBoundCounter = 0;
    }
  }

  //southbound
  if (southBoundPointer < southBoundVehicles.size() && roadway.at(sw).at(0) == nullptr)
  {

    roadway.at(sw).at(0) = southBoundVehicles.at(southBoundPointer);

    southBoundCounter++;
    if(southBoundCounter == southBoundVehicles.at(southBoundPointer)->getVehicleLength())
    {
      southBoundPointer++;
      southBoundCounter = 0;
    }
  }

  //eastbound
  if (eastBoundPointer < eastBoundVehicles.size() && roadway.at(0).at(ne) == nullptr)
  {

    roadway.at(0).at(ne) = eastBoundVehicles.at(eastBoundPointer);
    eastBoundCounter++;
    if(eastBoundCounter == eastBoundVehicles.at(eastBoundPointer)->getVehicleLength())
    {
      eastBoundPointer++;
      eastBoundCounter = 0;
    }
  }

  //westbound
  if (westBoundPointer < westBoundVehicles.size() && roadway.at(total).at(sw) == nullptr)
  {

    roadway.at(total).at(sw) = westBoundVehicles.at(westBoundPointer);

    westBoundCounter++;
    if(westBoundCounter == westBoundVehicles.at(westBoundPointer)->getVehicleLength())
    {
      westBoundPointer++;
      westBoundCounter = 0;
    }
  }
}


//helper methods to move vehicle at positions pos1 and pos2 one section forward.
void Roadway::moveSouth(int pos1, int pos2)
{
  roadway.at(pos1).at(pos2 + 1) = roadway.at(pos1).at(pos2);
  roadway.at(pos1).at(pos2) = nullptr;
}

void Roadway::moveNorth(int pos1, int pos2)
{
  roadway.at(pos1).at(pos2 - 1) = roadway.at(pos1).at(pos2);
  roadway.at(pos1).at(pos2) = nullptr;
}

void Roadway::moveEast(int pos1, int pos2)
{
  roadway.at(pos1 + 1).at(pos2) = roadway.at(pos1).at(pos2);
  roadway.at(pos1).at(pos2) = nullptr;
}

void Roadway::moveWest(int pos1, int pos2)
{
  roadway.at(pos1 - 1).at(pos2) = roadway.at(pos1).at(pos2);
  roadway.at(pos1).at(pos2) = nullptr;
}


//helper methods to see if the vehicle has space to move forward
bool Roadway::canMoveSouth(int pos1, int pos2)
{
  return roadway.at(pos1).at(pos2 + 1) == nullptr;
}

bool Roadway::canMoveNorth(int pos1, int pos2)
{
  return roadway.at(pos1).at(pos2 - 1) == nullptr;
}

bool Roadway::canMoveEast(int pos1, int pos2)
{
  return roadway.at(pos1 + 1).at(pos2) == nullptr;
}

bool Roadway::canMoveWest(int pos1, int pos2)
{
  return roadway.at(pos1 - 1).at(pos2) == nullptr;
}


// helper methods to check weather a vehicle has time to go through the stoplight
bool Roadway::canGoStraightThroughNLight(int pos1, int pos2)
{
  //check if this is a part of a vehicle that already checked if it could go through.
  if (roadway.at(pos1).at(pos2 - 2) == roadway.at(pos1).at(pos2))
  {
    return true;
  }
  //check if there is enough time to get through stoplight
  if (roadway.at(pos1).at(pos2)->getVehicleLength() + 2 > stoplight.timeUntilNSRed())
  {
    return false;
  }
  return true;
}

bool Roadway::canGoStraightThroughSLight(int pos1, int pos2)
{
  //check if this is a part of a vehicle that already checked if it could go through.
  if (roadway.at(pos1).at(pos2 + 2) == roadway.at(pos1).at(pos2))
  {
    return true;
  }
  //check if there is enough time to get through stoplight
  if (roadway.at(pos1).at(pos2)->getVehicleLength() + 2 > stoplight.timeUntilNSRed())
  {
    return false;
  }
  return true;
}

bool Roadway::canGoStraightThroughELight(int pos1, int pos2)
{
  //check if this is a part of a vehicle that already checked if it could go through.
  if (roadway.at(pos1 + 2).at(pos2) == roadway.at(pos1).at(pos2))
  {
    return true;
  }
  //check if there is enough time to get through stoplight
  if (roadway.at(pos1).at(pos2)->getVehicleLength() + 2 > stoplight.timeUntilEWRed())
  {
    return false;
  }
  return true;
}

bool Roadway::canGoStraightThroughWLight(int pos1, int pos2)
{
  //check if this is a part of a vehicle that already checked if it could go through.
  if (roadway.at(pos1 - 2).at(pos2) == roadway.at(pos1).at(pos2))
  {
    return true;
  }
  //check if there is enough time to get through stoplight
  if (roadway.at(pos1).at(pos2)->getVehicleLength() + 2 > stoplight.timeUntilEWRed())
  {
    return false;
  }
  return true;
}


// helper methods to see if vehicles have enough time to turn right through the intersection
bool Roadway::canNTurnRight(int pos1, int pos2)
{
  //check if this is a part of a vehicle that already checked if it could go through.
  if (roadway.at(pos1 + 1).at(pos2 - 1) == roadway.at(pos1).at(pos2))
  {
    return true;
  }
  //check if there is enough time to get through stoplight
  if (roadway.at(pos1).at(pos2)->getVehicleLength() + 1 > stoplight.timeUntilNSRed())
  {
    return false;
  }
  return true;
}

bool Roadway::canSTurnRight(int pos1, int pos2)
{
  //check if this is a part of a vehicle that already checked if it could go through.
  if (roadway.at(pos1 - 1).at(pos2 + 1) == roadway.at(pos1).at(pos2))
  {
    return true;
  }
  //check if there is enough time to get through stoplight
  if (roadway.at(pos1).at(pos2)->getVehicleLength() + 1 > stoplight.timeUntilNSRed())
  {
    return false;
  }
  return true;
}

bool Roadway::canETurnRight(int pos1, int pos2)
{
  //check if this is a part of a vehicle that already checked if it could go through.
  if (roadway.at(pos1 + 1).at(pos2 + 1) == roadway.at(pos1).at(pos2))
  {
    return true;
  }
  //check if there is enough time to get through stoplight
  if (roadway.at(pos1).at(pos2)->getVehicleLength() + 1 > stoplight.timeUntilEWRed())
  {
    return false;
  }
  return true;
}

bool Roadway::canWTurnRight(int pos1, int pos2)
{
  //check if this is a part of a vehicle that already checked if it could go through.
  if (roadway.at(pos1 - 1).at(pos2 - 1) == roadway.at(pos1).at(pos2))
  {
    return true;
  }
  //check if there is enough time to get through stoplight
  if (roadway.at(pos1).at(pos2)->getVehicleLength() + 1 > stoplight.timeUntilEWRed())
  {
    return false;
  }
  return true;
}


// helper methods to check if a vehicle can make a left turn based on incoming
// traffic and the time left on the traffic light.
// if two vehicles coming from opposite directions both turn left, both turn at once.
// since the first section of the vehicle will check whether it can make it through
// the light and intersection each method returns true if it is not the first section
// of the vehicle. This means that the position two forward and two to its left will
// be filled with an earlier section of the vehicle since it has already moved.
bool Roadway::canNTurnLeft(int pos1, int pos2)
{
  //check if this is a portion of a vehicle that already checked if it could turn.
  if (roadway.at(pos1 - 2).at(pos2 - 2) == roadway.at(pos1).at(pos2))
  {
    return true;
  }
  //check if there is enough time to get through stoplight
  if (roadway.at(pos1).at(pos2)->getVehicleLength() + 1 > stoplight.timeUntilNSRed())
  {
    return false;
  }
  // check if a vehicle from the opposite direction is also turning left
  if (roadway.at(pos1 - 1).at(pos2 - 3) != nullptr)
  {
    if (roadway.at(pos1 - 1).at(pos2 - 3)->getVehicleTurningDirection() == Turning::left)
    {
      return true;
    }
  }
  // southbound vehicles move first, so if it is turning left it would be here
  else if (roadway.at(pos1).at(pos2 - 2) != nullptr)
  {
    if (roadway.at(pos1).at(pos2 - 2)->getVehicleTurningDirection() == Turning::left)
    {
      return true;
    }
  }
  // check if there is a gap in the traffic of the opposite lane
  for (int i = 0; i < roadway.at(pos1).at(pos2)->getVehicleLength(); i ++)
  {
    if (!canMoveNorth(pos1 - 1, pos2 - 1 - i))
    {
      return false;
    }
  }
  return true;
}

bool Roadway::canSTurnLeft(int pos1, int pos2)
{
  //check if this is a portion of a vehicle that already checked if it could turn.
  if (roadway.at(pos1 + 2).at(pos2 + 2) == roadway.at(pos1).at(pos2))
  {
    return true;
  }
  //check if there is enough time to get through stoplight
  if (roadway.at(pos1).at(pos2)->getVehicleLength() + 1 > stoplight.timeUntilNSRed())
  {
    return false;
  }
  // check if a vehicle from the opposite direction is also turning left
  if (roadway.at(pos1 + 1).at(pos2 + 3) != nullptr)
  {
    if (roadway.at(pos1 + 1).at(pos2 + 3)->getVehicleTurningDirection() == Turning::left)
    {
      return true;
    }
  }
  // check if there is a gap in the traffic of the opposite lane
  for (int i = 0; i < roadway.at(pos1).at(pos2)->getVehicleLength(); i ++)
  {
    if (!canMoveSouth(pos1 + 1, pos2 + 1 + i))
    {
      return false;
    }
  }

  return true;
}

bool Roadway::canETurnLeft(int pos1, int pos2)
{
  //check if this is a portion of a vehicle that already checked if it could turn.
  if (roadway.at(pos1 + 2).at(pos2 - 2) == roadway.at(pos1).at(pos2))
  {
    return true;
  }
  //check if there is enough time to get through stoplight
  if (roadway.at(pos1).at(pos2)->getVehicleLength() + 1 > stoplight.timeUntilEWRed())
  {
    return false;
  }
  // check if a vehicle from the opposite direction is also turning left
  if (roadway.at(pos1 + 3).at(pos2 - 1) != nullptr)
  {
    if (roadway.at(pos1 + 3).at(pos2 - 1)->getVehicleTurningDirection() == Turning::left)
    {
      return true;
    }
  }
  // check if there is a gap in the traffic of the opposite lane
  for (int i = 0; i < roadway.at(pos1).at(pos2)->getVehicleLength(); i ++)
  {
    if (!canMoveEast(pos1 + 1 + i, pos2 - 1))
    {
      return false;
    }
  }
  return true;
}

bool Roadway::canWTurnLeft(int pos1, int pos2)
{
  //check if this is a portion of a vehicle that already checked if it could turn.
  if (roadway.at(pos1 - 2).at(pos2 + 2) == roadway.at(pos1).at(pos2))
  {
    return true;
  }
  //check if there is enough time to get through stoplight
  if (roadway.at(pos1).at(pos2)->getVehicleLength() + 1 > stoplight.timeUntilEWRed())
  {
    return false;
  }
  // check if a vehicle from the opposite direction is also turning left
  if (roadway.at(pos1 - 3).at(pos2 + 1) != nullptr)
  {
    if (roadway.at(pos1 - 3).at(pos2 + 1)->getVehicleTurningDirection() == Turning::left)
    {
      return true;
    }
  }
  // eastbound vehicles move first, so if it is turning left it would be here
  else if (roadway.at(pos1 - 2).at(pos2) != nullptr)
  {
    if (roadway.at(pos1 - 2).at(pos2)->getVehicleTurningDirection() == Turning::left)
    {
      return true;
    }
  }
  // check if there is a gap in the traffic of the opposite lane
  for (int i = 0; i < roadway.at(pos1).at(pos2)->getVehicleLength(); i ++)
  {
    if (!canMoveWest(pos1 - 1 - i, pos2 + 1))
    {
      return false;
    }
  }
  return true;
}


// helper methods to check if vehicles have space to turn left
bool Roadway::checkNTurnLeft(int pos1, int pos2)
{
  return (roadway.at(pos1 - 1).at(pos2 - 1) == nullptr);
}

bool Roadway::checkSTurnLeft(int pos1, int pos2)
{
  return (roadway.at(pos1 + 1).at(pos2 + 1) == nullptr);
}

bool Roadway::checkETurnLeft(int pos1, int pos2)
{
  return (roadway.at(pos1 + 1).at(pos2 - 1) == nullptr);
}

bool Roadway::checkWTurnLeft(int pos1, int pos2)
{
  return (roadway.at(pos1 - 1).at(pos2 + 1) == nullptr);
}


// helper methods for moving a vehicle left
void Roadway::moveNLeft(int pos1, int pos2)
{
  roadway.at(pos1 - 1).at(pos2 - 1) = roadway.at(pos1).at(pos2);
  roadway.at(pos1).at(pos2) = nullptr;
}

void Roadway::moveSLeft(int pos1, int pos2)
{
  roadway.at(pos1 + 1).at(pos2 + 1) = roadway.at(pos1).at(pos2);
  roadway.at(pos1).at(pos2) = nullptr;
}

void Roadway::moveELeft(int pos1, int pos2)
{
  roadway.at(pos1 + 1).at(pos2 - 1) = roadway.at(pos1).at(pos2);
  roadway.at(pos1).at(pos2) = nullptr;
}

void Roadway::moveWLeft(int pos1, int pos2)
{
  roadway.at(pos1 - 1).at(pos2 + 1) = roadway.at(pos1).at(pos2);
  roadway.at(pos1).at(pos2) = nullptr;
}


#endif
