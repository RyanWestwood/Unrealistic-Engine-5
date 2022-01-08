#pragma once

namespace UE {

	template<typename T>
	struct Vertex {
		Vertex(T x, T y, T z, T u, T v) : x(x), y(y), z(z), u(u), v(v) {}
		Vertex() { x = y = z = NULL; u = v = NULL; }

		T x, y, z;
		T u, v;
	};
}