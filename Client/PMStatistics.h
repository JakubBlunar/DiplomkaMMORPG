#ifndef PM_STATISTICS_H
#define PM_STATISTICS_H

#include <SFML/System/Mutex.hpp>
#include <chrono>
#include <map>
#include "RunningAverage.h"

class PMStatistics {
	size_t recieved;
	size_t sended;
	sf::Mutex timesMutex;
	std::map<int, std::chrono::high_resolution_clock::time_point> times;
	RunningAverage latencyCalc;

public:
	PMStatistics();
	~PMStatistics();

	void packetSend(int id);
	long long packetRecieve(int id);

	double latency() const;
	double returnRatio() const;
	void reset();

	long long lastLatency;
};

#endif
