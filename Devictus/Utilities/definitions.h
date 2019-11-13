#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define DEBUG 
#define FPS_LIMIT 60

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

enum KEY {
	KEY_ENTER,
	KEY_ESCAPE,
	KEY_SPACE,
	KEY_LEFT_SHIFT,
	KEY_W,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_PAUSE,
};

#define MOUSE_SENSITIVITY 0.1f

#define NUM_MOVE_KEYS 5
#define MOVE_FORWARD 0
#define MOVE_BACKWARD 1
#define MOVE_LEFT 2
#define MOVE_RIGHT 3
#define MOVE_UP 4

#endif