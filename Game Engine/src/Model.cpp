#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <iostream>

namespace Divide {

	bool Divide::Model::LoadFromFile(const char* filename)
	{
		std::vector<Vertex> loadedVertices;
		Assimp::Importer importer;

		const aiScene* pScene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_Quality);

		if (!pScene) {
			return false;
		}

		for (int MeshIdx = 0; MeshIdx < pScene->mNumMeshes; MeshIdx++) {
			const aiMesh* mesh = pScene->mMeshes[MeshIdx];
			for (int faceIdx = 0; faceIdx < mesh->mNumFaces; faceIdx++)
			{
				const aiFace& face = mesh->mFaces[faceIdx];
				for (int vertexIdx = 0; vertexIdx < 3; vertexIdx++)
				{
					const aiVector3D* pos = &mesh->mVertices[face.mIndices[vertexIdx]];
					loadedVertices.push_back(Vertex(pos->x, pos->y, pos->z, 1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
		} 

		m_NumVertices = loadedVertices.size();
		m_Vertices = new Vertex[m_NumVertices];

		std::copy(loadedVertices.begin(), loadedVertices.end(), m_Vertices);

		return true;
	}
}
