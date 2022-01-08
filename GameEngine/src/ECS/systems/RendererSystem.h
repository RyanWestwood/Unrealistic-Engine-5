#pragma once
#include <memory>

#include "Camera.h"

#include "ECS.h"
#include "./ECS/components/ModelComponent.h"

// namespace UE{

//     extern Coordinator g_Coordinator;

//     class RendererSystem : public System{
//         public:
//             void Init(std::shared_ptr<Camera> camera){
//                 m_Camera = camera;
//                 Signature signature;
//                 signature.set(g_Coordinator.GetComponentType<ModelComponent>());
//                 g_Coordinator.SetSystemSignature<RendererSystem>(signature);

//             }

//             void Draw(){
//                 for(auto const& entity : mEntities){
//                     auto& model = g_Coordinator.GetComponent<ModelComponent>(entity);

//                     model.model.Draw(m_Camera);                    
//                 }
//             }

//         private:
//             std::shared_ptr<Camera> m_Camera;
//     };

// } // namespace UE