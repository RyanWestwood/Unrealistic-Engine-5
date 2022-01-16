#pragma once
#include "ECS/ECS.h"
#include "ECS/components/TransformComponent.h"

 namespace UE{

     extern Coordinator g_Coordinator;

     // #TODO Add rigidy body and shiz
     class PhysicsSystem : public System{
         public:
            
             void SetSystem()
             {
				 std::bitset<MAX_COMPONENTS> signature;
                 signature.set(g_Coordinator.GetComponentType<TransformComponent>());
				 g_Coordinator.SetSystemSignature<PhysicsSystem>(signature);
             }

             void Update(){
                 for(auto const& entity : mEntities){
                     auto& transformComponent = g_Coordinator.GetComponent<TransformComponent>(entity);
                     transformComponent.position += glm::vec3(1);
                 }
             }
     };

 } // namespace UE