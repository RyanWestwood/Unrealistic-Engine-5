#pragma once
#include <glm/glm.hpp>
#include "Texture.h"

namespace UE {
	class Billboard {
	public:

		Billboard(std::string texturePath) {
			std::string textureDir = "resources/textures/";
			m_Texture = new Texture((textureDir + std::string(texturePath)).c_str());
			m_Position = { 0.0f,0.0f,0.0f };
			m_Scale = { 0.0f,0.0f,0.0f };
		}

		~Billboard(){
			delete m_Texture;
		}

		void SetPosition(const glm::vec3 position) { m_Position = position; }
		void SetScale(const glm::vec3 scale) { m_Scale = scale; }

		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetScale() const { return m_Scale; }
		Texture* GetTexture() const { return m_Texture; }

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		Texture* m_Texture;
	};
}