// PacketTypes.h - Contains declarations of packet types
#pragma once

#ifdef SHARED_EXPORTS
#define SHARED __declspec(dllexport)
#else
#define SHARED __declspec(dllimport)
#endif

extern "C" SHARED void fibonacci_init(
	const unsigned long long a, const unsigned long long b);

extern "C" SHARED bool fibonacci_next();

extern "C" SHARED unsigned long long fibonacci_current();

extern "C" SHARED unsigned fibonacci_index();

namespace pt
{

	enum PacketType
	{
		ERR = 1,
		WRONGPACKETTYPE = 2,
		POSITION = 3,
		MOVEMENT = 4
	};


}
