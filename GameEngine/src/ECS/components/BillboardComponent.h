#pragma once
#include <glm/glm.hpp>
#include <memeory.h>
#include "Billboard.h"

namespace UE{

    struct BillboardComponent{
        std::shared_ptr<Billboard> billboard;
        glm::vec3 position; 
    };
    
}