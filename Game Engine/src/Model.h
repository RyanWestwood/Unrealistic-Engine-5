#pragma once
#include "Mesh.h"
#include "ModelRenderer.h"
#include "Texture.h"

namespace UE {

	//	TODO: Have this as a subsitute somehow in model renderer.
	struct Transform {
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};

	class Model {
	public:
		Model(const char* modelPath, const char* texturePath);

		void Draw(Camera* camera);

		void SetPosition(glm::vec3 position) { m_ModelRenderer->SetPosition(position); }
		void SetRotation(glm::vec3 rotation) { m_ModelRenderer->SetRotation(rotation); }
		void SetScale(glm::vec3 scale) { m_ModelRenderer->SetScale(scale); }

		//void SetPosition(glm::vec3 position) { m_Transform.position = position; }
		//void SetRotation(glm::vec3 rotation) { m_Transform.rotation = rotation; }
		//void SetScale(glm::vec3 scale) { m_Transform.scale = scale; }

		//glm::vec3 GetPosition() { return m_Transform.position; }
		//glm::vec3 GetRotation() { return m_Transform.rotation; }
		//glm::vec3 GetScale() { return m_Transform.scale; }

	private:
		Renderer* m_ModelRenderer;
		Mesh* m_Model;
		Texture* m_Texture;
		//	TODO: pass as reference?
		Transform m_Transform;
	};
}