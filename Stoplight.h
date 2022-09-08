#ifndef __STOPLIGHT_H__
#define __STOPLIGHT_H__

#include "VehicleBase.h"

class Stoplight
{
  private:
    LightColor nsColor;
    LightColor ewColor;
    int time;
    int counter;

    static int green_north_south;
    static int yellow_north_south;
    static int green_east_west;
    static int yellow_east_west;

  public:
    Stoplight();
    ~Stoplight();

    static void setTimes(int greenNorthSouth, int yellowNorthSouth,
      int greenEastWest, int yellowEastWest);

    void updateStoplights();

    LightColor getNSLightColor();
    LightColor getEWLightColor();
    int timeUntilNSRed();
    int timeUntilEWRed();
};

  #endif
