#ifndef PLAYER_H
#define PLAYER_H

#include "sprite.h"

struct mouseData {
	long button;

	long relx;
	long rely;

	long x;
	long y;
};

class player: public sprite {
public:

	player();
	void InitPlayer ();

	int test;
	int DoMoves();

	bool keys[323];

	mouseData mouse;
};



#endif