#pragma once
#include <glm/gtc/type_ptr.hpp>

namespace UE{

    struct Sprite{

        Sprite(){}
        ~Sprite(){}

        void Draw(){}


        glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
    };
}