#ifndef __VEHICLE_H__
#define __VEHICLE_H__

#include "VehicleBase.h"
#include "Random.h"

enum class Turning {straight, right, left};

class Vehicle: public VehicleBase
{
  private:
    Turning turning;

    static double carRightTurnProb;
    static double carLeftTurnProb;
    static double suvRightTurnProb;
    static double suvLeftTurnProb;
    static double truckRightTurnProb;
    static double truckLeftTurnProb;

    static double carProb;
    static double suvProb;

    void setTurningDirection();
    VehicleType setVehicleType();

  public:
    Vehicle(Direction vehicleDirection);
    Vehicle(const Vehicle& other);
    ~Vehicle();

    int getVehicleLength();
    inline Turning getVehicleTurningDirection() const { return this->turning; }
    static void setProbabilityofVehicleType(double car, double suv);
    static void setProbabilityofTurningDirection(double carRight, double carLeft,
      double suvRight, double suvLeft, double truckRight, double truckLeft);
};
#endif
