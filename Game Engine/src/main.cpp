#define SDL_MAIN_HANDLED

#include "GameEngine.h"
#include <iostream>
#include <string>
//#include "ObjectPooler.h"
//#include "Sound.h"

using namespace UE;
constexpr int c_Delta = 1000 / 60;

int main(int argv, char* argc[]) {

	// TODO: Test this pool works. Optimize to minimise if branching. 
	// ObjectPool<Test> objPool = ObjectPool<Test>(100);

	GameEngine gameEngine;

	if (!gameEngine.Init(false)) {
		DisplayInfoMessages("Couldn't load SDL! Check console output for more details.");
	}

	uint32_t last_time = SDL_GetTicks();
	uint32_t current_time = 0;
	uint16_t frame_count = 0;
	uint8_t steps = 0;
	
	while (gameEngine.IsRunning()) {
		if (!gameEngine.IsPaused()) {
			current_time = SDL_GetTicks();

			gameEngine.Input();
			if (current_time - last_time > c_Delta) {
				gameEngine.Update();
				last_time = current_time;
				steps++;
				if(steps >= 60){
					steps = 0;
					gameEngine.SetWindowTitle(std::string("Unrealistic Engine 5 - FPS: " + std::to_string(frame_count)).c_str());
					gameEngine.SetFps(frame_count);
					frame_count = 0;
				} 
			}
			gameEngine.Draw();
			frame_count++;
		}
	}
	gameEngine.Free();

	return 0;
}