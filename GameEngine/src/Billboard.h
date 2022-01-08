#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <string>

namespace UE {
	class Camera;
	class Texture;

	struct BillboardRenderData {
		GLuint programId			= 0;
		GLint vertexLocation		= 0;
		GLint vertexUVLocation		= 0;
		GLuint vboQuad				= 0;

		GLuint transformUniformId	= 0;
		GLuint viewUniformId		= 0;
		GLuint projectionUniformId	= 0;
		GLuint samplerId			= 0;
	};

	class Billboard {
	public:
		Billboard(std::string texturePath, std::shared_ptr<Camera> camera);

		void Draw();

		void SetPosition(const glm::vec3 position) { m_Position = position; }
		void SetScale(const glm::vec3 scale) { m_Scale = scale; }

		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetScale() const { return m_Scale; }
		const std::unique_ptr<Texture>& GetTexture() const { return m_Texture; }

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		std::unique_ptr<Texture> m_Texture;
		std::shared_ptr<Camera> m_Camera;
		BillboardRenderData m_RenderData;
	};
}