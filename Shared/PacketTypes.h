// PacketTypes.h - Contains declarations of packet types
#pragma once

namespace pt
{

	enum PacketType
	{
		ERR = 1,
		WRONGPACKETTYPE = 2,
		POSITION = 3,
		MOVEMENT = 4,
		LATENCY = 5
	};


}
