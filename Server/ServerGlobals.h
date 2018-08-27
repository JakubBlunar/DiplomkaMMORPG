#ifndef SERVER_GLOBALS_H_INCLUDED
#define SERVER_GLOBALS_H_INCLUDED

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define METTOPIX 30.f
#define PIXTOMET 0.03333333333333333f

#define FIELD_SIZE 48.f

#define ceilNumber(x) ((float)(long)(x))

namespace s {

	enum EntityCategory {
		BOUNDARY = 0x0001,
		PLAYER = 0x0002,
		GAME_OBJECT = 0x0008,
		SENSOR = 0x0010
	};

}

#endif // GLOBALS_H_INCLUDED
