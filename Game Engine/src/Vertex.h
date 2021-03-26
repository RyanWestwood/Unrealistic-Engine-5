#pragma once

namespace UE {
	struct Vertex {
		Vertex(float x, float y, float z, float u, float v) : x(x), y(y), z(z), u(u), v(v) {}
		Vertex() { x = y = z = 0.0f; u = v = 0.0f; }

		float x, y, z;
		float u, v;
	};
}