// #pragma once

// #include <vector>
// #include <string>
// #include <memory>
// #include <queue>
// #include <unordered_map>
// #include <bitset>

// constexpr uint MAX_ENTITIES = 1000;
// constexpr ComponentType MAX_COMPONENTS = 32;

// using Entity = std::uint32_t;
// using ComponentType = std::uint8_t;
// using Signature = std::bitset<MAX_COMPONENTS>;

// class EntityManager{
//     public:
//         EntityManager(){
//             for (Entity entity = 0; entity < MAX_ENTITIES; entity++)
//             {
//                 m_AvailableEntities.push(entity);
//             }            
//         }

//         Entity CreateEntity(){
//             Entity id = m_AvailableEntities.front();
//             m_AvailableEntities.pop();
//             m_LivingEntityCount++;
//             return id;
//         }


//     private:
//         std::queue<Entity> m_AvailableEntities;
//         uint m_LivingEntityCount;
// };

// class IComponentArray{
// 	public:
// 		virtual ~IComponentArray() = default;
// 		virtual void EntityDestroyed(Entity entity) = 0;
// 	};


// template<typename T>
// class ComponentArray : public IComponentArray{
// 	public:
// 		void InsertData(Entity entity, T component)
// 		{
// 			assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to same entity more than once.");

// 			// Put new entry at end and update the maps
// 			size_t newIndex = mSize;
// 			mEntityToIndexMap[entity] = newIndex;
// 			mIndexToEntityMap[newIndex] = entity;
// 			mComponentArray[newIndex] = component;
// 			++mSize;
// 		}

// 		void RemoveData(Entity entity)
// 		{
// 			assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing non-existent component.");

// 			// Copy element at end into deleted element's place to maintain density
// 			size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
// 			size_t indexOfLastElement = mSize - 1;
// 			mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

// 			// Update map to point to moved spot
// 			Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
// 			mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
// 			mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

// 			mEntityToIndexMap.erase(entity);
// 			mIndexToEntityMap.erase(indexOfLastElement);

// 			--mSize;
// 		}

// 		T& GetData(Entity entity)
// 		{
// 			assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Retrieving non-existent component.");

// 			// Return a reference to the entity's component
// 			return mComponentArray[mEntityToIndexMap[entity]];
// 		}

// 		void EntityDestroyed(Entity entity) override
// 		{
// 			if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
// 			{
// 				// Remove the entity's component if it existed
// 				RemoveData(entity);
// 			}
// 		}
// };

// class ComponentManager{
//     public:

// };

// class System{
//     public:
//         std::vector<Entity> entities;
// };

// class SystemManager{
//     public: 

//         template<typename T>
//         std::shared_ptr<T> CreateSystem(){
//             const char* typeName = typeid(T).name();
//             std::shared_ptr<T> system = std::make_shared<T>();
//             m_Systems.insert({typeName, system)};            
//         }

//     private:
//         std::unordered_map<const char*, std::shared_ptr<System>> m_Systems{};
// };

// class Organizer{
//     public:

//     Organizer(){
//         m_ComponentManager = std::make_unique<ComponentManager>();
// 		m_EntityManager = std::make_unique<EntityManager>();
// 		m_SystemManager = std::make_unique<SystemManager>();
//     }


//     void RegisterSystem(){

//     }

//     private:
//     std::unique_ptr<ComponentManager> m_ComponentManager;
// 	std::unique_ptr<EntityManager> m_EntityManager;
// 	std::unique_ptr<SystemManager> m_SystemManager;
// };
