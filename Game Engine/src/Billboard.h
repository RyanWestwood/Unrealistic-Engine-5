#pragma once
#include <GL/glew.h>
//#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <list>
#include <vector>
#include <array>
#include <memory>
#include "Texture.h"
#include "Camera.h"
#include "Vertex.h"
#include "Utils.h"

namespace UE {
	class BillboardRenderer;

	class Billboard : public std::enable_shared_from_this<Billboard> {
	public:

		Billboard(std::string texturePath, std::shared_ptr<Camera> camera);
		void Init();

		void SetPosition(const glm::vec3 position) { m_Position = position; }
		void SetScale(const glm::vec3 scale) { m_Scale = scale; }
		void Draw();

		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetScale() const { return m_Scale; }
		const std::unique_ptr<Texture>& GetTexture() const { return m_Texture; }

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<BillboardRenderer> m_Renderer;
		std::shared_ptr<Camera> m_Camera;
	};

	class BillboardRenderer {
	public:
		BillboardRenderer(std::shared_ptr<Billboard> billboard, std::shared_ptr<Camera> camera) : m_Billboard(billboard), m_Camera(camera) {}

		~BillboardRenderer() { glDeleteBuffers(1, &vboQuad); }

		void Init();
		void Draw();

	private:
		std::shared_ptr<Billboard> m_Billboard;
		std::shared_ptr<Camera> m_Camera;

		GLuint programId;
		GLint vertexLocation;
		GLint vertexUVLocation;
		GLuint vboQuad;

		GLuint transformUniformId;
		GLuint viewUniformId;
		GLuint projectionUniformId;
		GLuint samplerId;
	};
}