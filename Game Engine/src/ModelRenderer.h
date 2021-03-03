#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"

namespace UE {

	class Renderer {
	public:
		Renderer(Mesh* model);
		~Renderer();

		void Init();

		void Update();
		void Draw(Camera* camera);

		void Free();

		glm::vec3 GetPosition() { return m_Position; }
		glm::vec3 GetRotation() { return m_Rotation; }
		glm::vec3 GetScale() { return m_Scale; }

		void SetPosition(glm::vec3 position) { m_Position = position; }
		void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; }
		void SetScale(glm::vec3 scale) { m_Scale = scale; }
		void SetMaterial(Texture* texture) { m_Texture = texture; }

	private:
		GLuint m_ProgramID;
		GLuint m_VboModel;
		GLint m_VertexPos3DLocation;
		GLint m_VertexUVLocation;

		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;

		GLuint m_TransformUniformID;
		GLuint m_ViewUniformID;
		GLuint m_ProjectionUniformID;
		GLuint m_SamplerID;

		Mesh* m_Model;
		Texture* m_Texture;
	};
}