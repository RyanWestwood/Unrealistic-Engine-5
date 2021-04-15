#pragma once
#include <glm/gtc/type_ptr.hpp>

namespace UE{

    struct Transform{
        glm::vec3 position;
        glm::vec3 scale;
        glm::quat rotation;
    };

}