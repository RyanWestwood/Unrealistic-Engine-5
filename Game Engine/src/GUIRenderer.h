#pragma once
#include "Texture.h"
#include "CrossPlatform.h"
#include <glm/glm.hpp>
#include <memory>

namespace UE {

	struct GUIRenderData {
		GLuint programId = 0;
		GLint vertexLocation = 0;
		GLint vertexUVLocation = 0;

		GLuint projectionUniformId = 0;
		GLuint samplerId = 0;
		glm::mat4 projectionMat = glm::mat4(1.0f);
	};

	class GUIRenderer
	{
	public:
		GUIRenderer(int window_width, int window_height);
		void DrawTexture(int x, int y, std::shared_ptr<Texture> tex);

	private:
		GUIRenderData m_RenderData;
	};
}

