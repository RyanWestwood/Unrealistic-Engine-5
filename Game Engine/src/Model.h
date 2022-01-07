#pragma once
#include <GL/glew.h>
#include "CrossPlatform.h"

#include "Texture.h"
#include "Camera.h"
#include "Vertex.h"
#include "Utils.h"

#include <memory>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace UE {
	class Mesh {
	public:
		Mesh();

		bool LoadFromFile(const char* filename);

		std::vector<Vertex>& GetVertices() { return m_Vertices; }
		const int& GetNumVertices() { return m_NumVertices; }

	private:
		std::vector<Vertex> m_Vertices;
		int m_NumVertices;
	};

	class MeshRenderer {
	public:
		MeshRenderer(std::shared_ptr<Mesh> model);
		~MeshRenderer();

		void Init();
		void Draw(const std::shared_ptr<Camera>& camera);

		glm::vec3 GetPosition() { return m_Position; }
		glm::vec3 GetRotation() { return m_Rotation; }
		glm::vec3 GetScale() { return m_Scale; }

		void SetPosition(glm::vec3 position) { m_Position = position; }
		void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; }
		void SetScale(glm::vec3 scale) { m_Scale = scale; }
		void SetMaterial(std::shared_ptr<Texture> texture) { m_Texture = texture; }

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

		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<Texture> m_Texture;
	};

	class Model {
	public:
		Model(const char* modelPath, const char* texturePath);

		void Draw(const std::shared_ptr<Camera>& camera);

		void SetPosition(glm::vec3 position) { m_ModelRenderer->SetPosition(position); }
		void SetRotation(glm::vec3 rotation) { m_ModelRenderer->SetRotation(rotation); }
		void SetScale(glm::vec3 scale) { m_ModelRenderer->SetScale(scale); }

	private:
		std::shared_ptr<Mesh> m_Model;
		std::unique_ptr<MeshRenderer> m_ModelRenderer;
		std::shared_ptr<Texture> m_Texture;
	};
}