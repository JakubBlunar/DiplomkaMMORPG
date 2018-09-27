#include "Random.h"


Random::Random():
	seed(rd())
{
	
}


Random::~Random()
{
}

int Random::randomUniformInt(int from, int to) {
	std::uniform_int_distribution<> distr(from, to);
	return distr(seed);
}

float Random::randomUniformFloat(float from, float to)
{
	std::uniform_real_distribution<float> distr(from, to);
	return distr(seed);
}
