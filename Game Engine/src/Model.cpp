#include "Model.h"
#include <iostream>

UE::Model::Model(const char* modelPath, const char* texturePath)
{
	std::string modelDir = "resources/models/";
	std::string textureDir = "resources/textures/";

	m_Model = new Mesh();
	bool result = m_Model->LoadFromFile((modelDir + std::string(modelPath)).c_str());
	if (!result) {
		std::cerr << "Failed to load model!\n";
	}

	m_Texture = new Texture((textureDir + std::string(texturePath)).c_str());

	m_ModelRenderer = new Renderer(m_Model);
	m_ModelRenderer->Init();
	m_ModelRenderer->SetRotation({ 0.0f, 0.0f, 0.0f });
	m_ModelRenderer->SetScale({ 3,3,3 });
	m_ModelRenderer->SetMaterial(m_Texture);
}

void UE::Model::Draw(Camera* camera) {
	m_ModelRenderer->Draw(camera);
}
