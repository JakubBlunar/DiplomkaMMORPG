#ifndef RUNNING_AVERAGE_H
#define RUNNING_AVERAGE_H

#include "SFML\System.hpp"

class RunningAverage
{
	double val;
	size_t n;
	sf::Mutex addMutex;
public:
	RunningAverage()
	{
		reset();
	}

	double update(double valIn)
	{
		addMutex.lock();
		const double scaling = 1. / static_cast<double>(n + 1);
		val = valIn * scaling + val * (1. - scaling);
		n++;
		addMutex.unlock();
		return val;
	}

	double get() const
	{
		return val;
	}

	size_t count() const
	{
		return n;
	}

	void reset()
	{
		addMutex.lock();
		val = 0.;
		n = 0;
		addMutex.unlock();
	}
};

#endif