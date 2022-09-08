#ifndef __VEHICLE_CPP__
#define __VEHICLE_CPP__

#include "Vehicle.h"
#include <iostream>

double Vehicle::carProb;
double Vehicle::suvProb;

double Vehicle::carRightTurnProb;
double Vehicle::carLeftTurnProb;
double Vehicle::suvRightTurnProb;
double Vehicle::suvLeftTurnProb;
double Vehicle::truckRightTurnProb;
double Vehicle::truckLeftTurnProb;

//Constructor
Vehicle::Vehicle(Direction vehicleDirection):VehicleBase(setVehicleType(), vehicleDirection){
  setTurningDirection();
}

//Copy Constructor
Vehicle::Vehicle(const Vehicle& other)
  : VehicleBase(other), turning(other.getVehicleTurningDirection())
  {}

//Destructor
Vehicle::~Vehicle() {}

//Sets the turning Direction using the random class
void Vehicle::setTurningDirection()
{
  double randNum = Random::getNextRandom();
  if(this->getVehicleType() == VehicleType::car)
  {
    if(randNum < carLeftTurnProb){
      this->turning = Turning::left;
    }
    else if(randNum < (carRightTurnProb + carLeftTurnProb)){
      this->turning = Turning::right;
    }
    else{
      this->turning = Turning::straight;
    }
  }

  else if(this->getVehicleType() == VehicleType::suv)
  {
    if(randNum < suvLeftTurnProb){
      this->turning = Turning::left;
    }
    else if(randNum < (suvRightTurnProb + suvLeftTurnProb)){
      this->turning = Turning::right;
    }
    else{
      this->turning = Turning::straight;
    }
  }

  else
  { // if(this->getVehicleType() == VehicleType::truck){
    if(randNum < truckLeftTurnProb){
      this->turning = Turning::left;
    }
    else if(randNum < (truckRightTurnProb + truckLeftTurnProb)){
      this->turning = Turning::right;
    }
    else{
      this->turning = Turning::straight;
    }
  }
}

//sets the vehicle type randomly
VehicleType Vehicle::setVehicleType()
{
  VehicleType temp;
  double randNum = Random::getNextRandom();
  if(randNum < carProb){
    temp = VehicleType::car;
  }
  else if (randNum < (carProb + suvProb)){
    temp = VehicleType::suv;
  }
  else{
    temp = VehicleType::truck;
  }

  return temp;
}

// Returns the length of the vehicle based on type
int Vehicle::getVehicleLength()
{
  if(this->getVehicleType() == VehicleType::car){
    return 2;
  }
  if(this->getVehicleType() == VehicleType::suv){
    return 3;
  }
  return 4;
}

// Takes in the proportion of vehicle type
void Vehicle::setProbabilityofVehicleType(double car, double suv)
{
  carProb = car;
  suvProb = suv;
}

//Takes in probability of turning right or left for each vehicle type
void Vehicle::setProbabilityofTurningDirection(double carRight, double carLeft,
  double suvRight, double suvLeft, double truckRight, double truckLeft)
{
  carRightTurnProb = carRight;
  carLeftTurnProb = carLeft;
  suvRightTurnProb = suvLeft;
  suvLeftTurnProb = suvRight;
  truckRightTurnProb = truckRight;
  truckLeftTurnProb = truckLeft;

}

#endif
