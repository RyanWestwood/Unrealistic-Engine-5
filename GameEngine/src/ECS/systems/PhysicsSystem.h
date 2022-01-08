#pragma once
#include "ECS.h"

// namespace UE{

//     extern Coordinator g_Coordinator;

//     class PhysicsSystem : public System{
//         public:
//             void Init(){
//                 Signature signature;
//                 signature.set(g_Coordinator.GetComponentType<Gravity>());
//                 signature.set(g_Coordinator.GetComponentType<RigidBody>());
//                 signature.set(g_Coordinator.GetComponentType<Transform>());
//                 g_Coordinator.SetSystemSignature<PhysicsSystem>(signature);

//             }

//             void Update(){
//                 for(auto const& entity : mEntities){
//                     auto& rigidBody = g_Coordinator.GetComponent<RigidBody>(entity);
//                     auto& transform = g_Coordinator.GetComponent<Transform>(entity);
//                     auto const& gravity = g_Coordinator.GetComponent<Gravity>(entity);

//                     transform.position += rigidBody.velocity;
//                     rigidBody.velocity += gravity.force;
//                 }
//             }
//     };

// } // namespace UE