#ifndef RANDOM_H
#define RANDOM_H
#include <random>


class Random
{
public:
	int randomUniformInt(int from, int to);
	float randomUniformFloat(float from, float to);

	Random(const Random&) = delete;
	Random& operator=(const Random) = delete;

	static Random* instance() {
		static Random instance;
		return &instance;
	}

private:
	Random();
	~Random();

	std::random_device rd;
    std::mt19937 seed;


};

#endif