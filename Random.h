#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <random>

class Random
{
private:
  static std::mt19937 randomNumberGenerator; // Mersenne twister
  static std::uniform_real_distribution<double> rand_double;

public:
  Random();
  ~Random();

  static void setInitialSeed(int initialSeed);
  static double getNextRandom();

};

#endif
