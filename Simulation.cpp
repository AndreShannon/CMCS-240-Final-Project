#ifndef __SIMULATION_CPP__
#define __SIMULATION_CPP__

#include <fstream>
#include <iostream>
#include <string>

#include "Animator.h"
#include "VehicleBase.h"
#include "Roadway.h"
#include "Random.h"
#include "Stoplight.h"
#include "Vehicle.h"

int main(int argc, char* argv[])
{
  // check for the correct number of arguments
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0]
              << "<input file><initial seed>" << std::endl;
    exit(0);
  }

  // check if the file can be opened
  std::ifstream infile { argv[1] };
  if (!infile) {
    std::cerr << "Unable to open file: " << argv[1] << std::endl;
    exit(0);
  }

  // initialize varaibles
  int maximum_simulated_time = 0;
  int number_of_sections_before_intersection = 0;
  int green_north_south = 0;
  int yellow_north_south = 0;
  int green_east_west = 0;
  int yellow_east_west = 0;
  double prob_new_vehicle_northbound = 0;
  double prob_new_vehicle_southbound = 0;
  double prob_new_vehicle_eastbound = 0;
  double prob_new_vehicle_westbound = 0;
  double proportion_of_cars = 0;
  double proportion_of_SUVs = 0;
  double proportion_right_turn_cars = 0;
  double proportion_left_turn_cars = 0;
  double proportion_right_turn_SUVs = 0;
  double proportion_left_turn_SUVs = 0;
  double proportion_right_turn_trucks = 0;
  double proportion_left_turn_trucks = 0;

  // read the file and save the input data
  int i = 0;
  std::string::size_type sz;
  std::string line;

  while(!infile.eof())
  {
    std::getline(infile, line);
    i++;

    switch (i) {
      case 1: maximum_simulated_time = std::stoi(line.substr(line.find(':')+1), &sz); break;
      case 2: number_of_sections_before_intersection = std::stoi(line.substr(line.find(':')+1), &sz); break;
      case 3: green_north_south = std::stoi(line.substr(line.find(':')+1), &sz); break;
      case 4: yellow_north_south = std::stoi(line.substr(line.find(':')+1), &sz); break;
      case 5: green_east_west = std::stoi(line.substr(line.find(':')+1), &sz); break;
      case 6: yellow_east_west = std::stoi(line.substr(line.find(':')+1), &sz); break;
      case 7: prob_new_vehicle_northbound = std::stod(line.substr(line.find(':')+1), &sz); break;
      case 8: prob_new_vehicle_southbound = std::stod(line.substr(line.find(':')+1), &sz); break;
      case 9: prob_new_vehicle_eastbound = std::stod(line.substr(line.find(':')+1), &sz); break;
      case 10: prob_new_vehicle_westbound = std::stod(line.substr(line.find(':')+1), &sz); break;
      case 11: proportion_of_cars = std::stod(line.substr(line.find(':')+1), &sz); break;
      case 12: proportion_of_SUVs = std::stod(line.substr(line.find(':')+1), &sz); break;
      case 13: proportion_right_turn_cars = std::stod(line.substr(line.find(':')+1), &sz); break;
      case 14: proportion_left_turn_cars = std::stod(line.substr(line.find(':')+1), &sz); break;
      case 15: proportion_right_turn_SUVs = std::stod(line.substr(line.find(':')+1), &sz); break;
      case 16: proportion_left_turn_SUVs = std::stod(line.substr(line.find(':')+1), &sz); break;
      case 17: proportion_right_turn_trucks = std::stod(line.substr(line.find(':')+1), &sz); break;
      case 18: proportion_left_turn_trucks = std::stod(line.substr(line.find(':')+1), &sz); break;
    }
  }

  infile.close();

  // set up variables
  Roadway road = Roadway(number_of_sections_before_intersection, prob_new_vehicle_northbound,
  prob_new_vehicle_southbound, prob_new_vehicle_eastbound, prob_new_vehicle_westbound);

  Stoplight::setTimes(green_north_south, yellow_north_south, green_east_west, yellow_east_west);

  Vehicle::setProbabilityofVehicleType(proportion_of_cars, proportion_of_SUVs);
  Vehicle::setProbabilityofTurningDirection(proportion_right_turn_cars, proportion_left_turn_cars,
    proportion_right_turn_SUVs, proportion_left_turn_SUVs, proportion_right_turn_trucks, proportion_left_turn_trucks);

  Random::setInitialSeed(std::stoi(argv[2], &sz));

  int total = number_of_sections_before_intersection * 2 + 1; // last element in vector since vector starts at 0
  int sw = number_of_sections_before_intersection;
  int ne = number_of_sections_before_intersection + 1;

  // Animation
  Animator animator(number_of_sections_before_intersection);

  std::vector<VehicleBase*> northbound(total + 1, nullptr);
  std::vector<VehicleBase*> southbound(total + 1, nullptr);
  std::vector<VehicleBase*> eastbound(total + 1, nullptr);
  std::vector<VehicleBase*> westbound(total + 1, nullptr);

  animator.setVehiclesNorthbound(northbound);
  animator.setVehiclesSouthbound(southbound);
  animator.setVehiclesEastbound(eastbound);
  animator.setVehiclesWestbound(westbound);

  animator.setLightNorthSouth(road.getNSLightColor());
  animator.setLightEastWest(road.getEWLightColor());

  animator.draw(0);

  // for now so we are not going through a thousand iterations.
  // maximum_simulated_time = 200;

  // loop
  char dummy;
  std::cin.get(dummy);

  for (int j = 1; j < maximum_simulated_time; j ++)
  {
    // update vehicle positions
    road.moveTraffic();

    for (int k = 0; k < total + 1; k ++)
    {
      northbound.at(k) = road.getRoadwayAt(ne, total - k);
      southbound.at(k) = road.getRoadwayAt(sw, k);
      eastbound.at(k) = road.getRoadwayAt(k, ne);
      westbound.at(k) = road.getRoadwayAt(total - k, sw);
    }

    animator.setVehiclesNorthbound(northbound);
    animator.setVehiclesSouthbound(southbound);
    animator.setVehiclesEastbound(eastbound);
    animator.setVehiclesWestbound(westbound);

    animator.setLightNorthSouth(road.getNSLightColor());
    animator.setLightEastWest(road.getEWLightColor());

    animator.draw(j);
    std::cin.get(dummy);
  }

  return 0;
};


#endif
