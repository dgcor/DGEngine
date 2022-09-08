#include "Random.h"

std::random_device RandomGenerator::rd;
std::mt19937 RandomGenerator::generator(rd());
