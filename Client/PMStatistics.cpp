#include "PMStatistics.h"


PMStatistics::PMStatistics() : recieved(0), sended(0) {
	latencyCalc.reset();
	lastLatency = 0;
}


PMStatistics::~PMStatistics() {
}

void PMStatistics::packetSend(const int id) {
	timesMutex.lock();
	sended++;
	times[id] = std::chrono::high_resolution_clock::now();
	timesMutex.unlock();
}

long long PMStatistics::packetRecieve(const int id) {
	const auto t2 = std::chrono::high_resolution_clock::now();

	timesMutex.lock();
	recieved++;

	const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - times[id]).count();
	latencyCalc.update(static_cast<double>(duration));
	lastLatency = duration;

	times.erase(id);

	timesMutex.unlock();

	return duration;
}

double PMStatistics::latency() const {
	return latencyCalc.get();
}

double PMStatistics::returnRatio() const {
	return recieved / static_cast<double>(sended);
}

void PMStatistics::reset() {
	latencyCalc.reset();
	sended = 0;
	recieved = 0;
}
