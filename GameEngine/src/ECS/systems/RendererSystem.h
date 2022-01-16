#pragma once
#include "ECS/ECS.h"
#include "ECS/components/SpriteComponent.h"

 namespace UE{

     extern Coordinator g_Coordinator;

     class RendererSystem : public System{
         public:
			 
			 void SetSystem()
			 {
				 std::bitset<MAX_COMPONENTS> signature;
				 signature.set(g_Coordinator.GetComponentType<TransformComponent>());
				 signature.set(g_Coordinator.GetComponentType<SpriteComponent>());
				 g_Coordinator.SetSystemSignature<RendererSystem>(signature);
			 }

			 void Draw() {
				 for (auto const& entity : mEntities) {
					 auto transformComponent = g_Coordinator.GetComponent<TransformComponent>(entity);
					 auto& spriteComponent = g_Coordinator.GetComponent<SpriteComponent>(entity);
					 spriteComponent.renderer->DrawTexture(transformComponent.position, spriteComponent.sprite);
				 }
			 }
     };

 } // namespace UE