#include "PMStatistics.h"

using namespace std;

PMStatistics::PMStatistics() :recieved(0), sended(0)
{
	latencyCalc.reset();
}


PMStatistics::~PMStatistics()
{
}

void PMStatistics::packetSend(const int id)
{
	timesMutex.lock();
	sended++;
	times[id] = chrono::high_resolution_clock::now();
	timesMutex.unlock();
}

long long PMStatistics::packetRecieve(const int id)
{
	if (id == 0)
		return -1;

	const auto t2 = chrono::high_resolution_clock::now();

	timesMutex.lock();
	recieved++;

	const auto duration = chrono::duration_cast<chrono::microseconds>(t2 - times[id]).count();
	latencyCalc.update(duration);
	lastLatency = duration;

	times.erase(id);

	timesMutex.unlock();

	return duration;
}

double PMStatistics::latency() const
{
	return latencyCalc.get();
}

double PMStatistics::returnRatio() const
{
	return recieved / static_cast<double>(sended);
}

void PMStatistics::reset()
{
	latencyCalc.reset();
	sended = 0;
	recieved = 0;
}
