#pragma once
namespace Divide {
	struct Vertex {
		Vertex(float x, float y, float z, float r, float g, float b, float a) : x(x), y(y), z(z), r(r), g(g), b(b), a(a) {}
		Vertex() { x = y = z = 0.0f; r = g = b = a = 0.0f; }

		float x, y, z;
		float r, g, b, a;
	};

	class Model {
	public:
		Model() {
			m_Vertices = nullptr;
			m_NumVertices = 0;
		}

		~Model() {
			delete m_Vertices;
		}

		bool LoadFromFile(const char* filename);

		void* GetVertices() {
			return (void*)m_Vertices;
		}

		int GetNumVertices() {
			return m_NumVertices;
		}

	private:
		Vertex* m_Vertices;
		int m_NumVertices;
	};

}