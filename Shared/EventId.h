// EventId.h - Contains declarations of event types

#ifndef EVENT_ID_H
#define EVENT_ID_H

enum EventId
{
	ERR = 1,
	WRONGPACKETTYPE = 2,
	POSITION = 3,
	MOVEMENT = 4,
	LATENCY = 5,
	LOGINREQUEST = 6,
	LOGINRESPONSE = 7,

};

#endif // !EVENT_ID_H
