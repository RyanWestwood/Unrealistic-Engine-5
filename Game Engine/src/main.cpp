#define SDL_MAIN_HANDLED

#include "GameEngine.h"
#include <iostream>
#include <string>
#include <glm/glm.hpp>

using namespace UE;

int main(int argv, char* argc[]) {
	GameEngine gameEngine;

	if (!gameEngine.Init(true)) {
		UE::DisplayInfoMessages("Couldn't load SDL! Check console output for more details.");
	}

	uint32_t last_time = SDL_GetTicks();
	uint32_t current_time = 0;
	uint16_t frame_count = 0;

	while (gameEngine.IsRunning()) {
		gameEngine.Input();
		gameEngine.Update();
		gameEngine.Draw();

		frame_count++;
		current_time = SDL_GetTicks();

		if (current_time - last_time > 1000) {
			gameEngine.SetWindowTitle(std::string("Unrealistic Engine 5 - FPS: " + std::to_string(frame_count)).c_str());
			frame_count = 0;
			last_time = current_time;
		}
	}
	gameEngine.Free();

	return 0;
}