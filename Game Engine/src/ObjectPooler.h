#pragma once
#include <iostream>
#include <vector>

// This code was written by Ryan Westwood on November 10th 2020 for C++ Programming for games. 

namespace UE {

	template<typename T>
	class ObjectPool {
	public:
		ObjectPool(const int POOL_SIZE) : POOL_SIZE(POOL_SIZE) {};

		// TODO: Test this way of initalising the object pool. delete un neeed memeber var.
		// template <typename ...Args>
		// ObjectPool(const int POOL_SIZE, Args && ...args) {
		//     for (int i = 0; i < POOL_SIZE; i++)
		//         arr.push_back(new T(std::forward<Args>(args)...));
		// };

		template <typename ...Args>
		void Init(Args && ...args) {
			for (int i = 0; i < POOL_SIZE; i++)
				arr.push_back(new T(std::forward<Args>(args)...));
		}

		~ObjectPool() {
			for (T* item : arr) {
				item->Free();
				delete item;
			}
		}

		void Draw() {
			for (T* item : arr)
				item->Draw();
		}

		void Update() {
			for (T* item : arr)
				item->Update();
		}

		template <typename ...Args>
		void Create(Args && ...args) {
			for (T* item : arr) {
				if (!item->Active()) {
					item->Init(std::forward<Args>(args)...);
					return;
				}
			}
		}

		std::vector<T*>& GetObjects() {
			return arr;
		}

	protected:
		std::vector<T*> arr;
		const int POOL_SIZE;
	};

	class ObjectPoolBase {
	public:
		virtual void Init() {};
		virtual void Free() = 0;
		virtual bool Active() const = 0;
		virtual void Deactivate() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;

	private:
		bool m_Active;
	};

	class Test : public ObjectPoolBase {
	public:
		Test(int a, char b) {
			std::cout << a << ", " << b << " Class created!\n";
		}

		void Free() override {}
		bool Active() const override { return true; }
		void Deactivate() override {}
		void Update() override {}
		void Draw() override {}


	private:

	};
}// namespace UE