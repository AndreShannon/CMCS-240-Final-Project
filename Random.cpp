#ifndef __RANDOM_CPP__
#define __RANDOM_CPP__

#include "Random.h"

std::mt19937 Random::randomNumberGenerator;
std::uniform_real_distribution<double> Random::rand_double = std::uniform_real_distribution<double>(0,1);

Random::Random(){}

Random::~Random(){}

void Random::setInitialSeed(int initialSeed)
{
  randomNumberGenerator.seed(initialSeed);
}

double Random::getNextRandom()
{
  return rand_double(randomNumberGenerator);
}

#endif
