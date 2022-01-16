#pragma once
#include <memory.h>
#include "GUIRenderer.h"
#include "Texture.h"

namespace UE {

	struct SpriteComponent {
		std::shared_ptr<GUIRenderer> renderer;
		std::shared_ptr<Texture> sprite;
	};
}