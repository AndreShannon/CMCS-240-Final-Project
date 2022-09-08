#ifndef __STOPLIGHT_CPP__
#define __STOPLIGHT_CPP__

#include "Stoplight.h"

int Stoplight::green_north_south = 0;
int Stoplight::yellow_north_south = 0;
int Stoplight::green_east_west = 0;
int Stoplight::yellow_east_west = 0;

Stoplight::Stoplight()
  : nsColor(LightColor::green), ewColor(LightColor::red), time(0), counter(0)
{}

Stoplight::~Stoplight(){}

void Stoplight::updateStoplights()
{
  time ++;
  if (counter == 0) // north south light is green
  {
    if (time == green_north_south)
    {
      nsColor = LightColor::yellow;
      counter ++;
      time = 0;
    }
  }
  else if (counter == 1) // north south light is yellow
  {
    if (time == yellow_north_south)
    {
      nsColor = LightColor::red;
      ewColor = LightColor::green;
      counter ++;
      time = 0;
    }
  }
  else if (counter == 2) // east west light is green
  {
    if (time == green_east_west)
    {
      ewColor = LightColor::yellow;
      counter ++;
      time = 0;
    }
  }
  else // counter == 3 and east west light is yellow
  {
    if (time == yellow_east_west)
    {
      ewColor = LightColor::red;
      nsColor = LightColor::green;
      counter = 0;
      time = 0;
    }
  }
}

LightColor Stoplight::getNSLightColor()
{
  return nsColor;
}

LightColor Stoplight::getEWLightColor()
{
  return ewColor;
}

int Stoplight::timeUntilNSRed()
{
  if (nsColor == LightColor::green)
  {
    return green_north_south + yellow_north_south - time;
  }
  else if (nsColor == LightColor::yellow)
  {
    return yellow_north_south - time;
  }
  return 0;
}

int Stoplight::timeUntilEWRed()
{
  if (ewColor == LightColor::green)
  {
    return green_east_west + yellow_east_west - time;
  }
  else if (ewColor == LightColor::yellow)
  {
    return yellow_east_west - time;
  }
  return 0;
}

void Stoplight::setTimes(int greenNorthSouth, int yellowNorthSouth,
  int greenEastWest, int yellowEastWest)
{
  green_north_south = greenNorthSouth;
  yellow_north_south = yellowNorthSouth;
  green_east_west = greenEastWest;
  yellow_east_west = yellowEastWest;
}

#endif
