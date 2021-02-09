#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "Camera.h"
#include "Model.h"

namespace Divide {

	class ModelRenderer {
	public:
		ModelRenderer(Model* model);
		~ModelRenderer();

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


	private:
		GLuint m_ProgramId;
		GLuint m_VboModel;
		GLint m_VertexPos3DLocation;
		GLint m_VertexFragmentColourLocation;

		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;

		GLuint m_TransformUniformID;
		GLuint m_ViewUniformID;
		GLuint m_ProjectionUniformID;

		Model* m_Model;
	};
}