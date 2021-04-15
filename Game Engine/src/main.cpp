#define SDL_MAIN_HANDLED

#include "GameEngine.h"
#include <iostream>
#include <string>
//#include "ObjectPooler.h"

using namespace UE;
constexpr int c_Delta = 1000 / 60;


int main(int argv, char* argc[]) {

	// TODO: Test this pool works. Optimize to minimise if branching. 
	// ObjectPool<Test> objPool = ObjectPool<Test>(100);

	GameEngine gameEngine;

	if (!gameEngine.Init(true)) {
		DisplayInfoMessages("Couldn't load SDL! Check console output for more details.");
	}

	uint32_t last_time = SDL_GetTicks();
	uint32_t current_time = 0;
	uint16_t frame_count = 0;

	while (gameEngine.IsRunning()) {
		if (!gameEngine.IsPaused()) {
			frame_count++;
			current_time = SDL_GetTicks();

			gameEngine.Input();
			if (current_time - last_time > c_Delta) {
				gameEngine.Update();
				last_time = current_time;
				//TODO: This is 144/60 so 3.
				gameEngine.SetWindowTitle(std::string("Unrealistic Engine 5 - FPS: " + std::to_string(frame_count)).c_str());
				frame_count = 0;
			}
			gameEngine.Draw();
		}
	}
	gameEngine.Free();

	return 0;
}