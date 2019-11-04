#include "Engine/engine.h"

int main() {

	Game * devictus = new Game();

	Engine engine(WINDOW_WIDTH, WINDOW_HEIGHT);
	if (engine.init()) {
		devictus->init();
		engine.start(devictus);
	}

	return 0;
}