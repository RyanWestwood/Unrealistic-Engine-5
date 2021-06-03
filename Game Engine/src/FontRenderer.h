#pragma once
#include "Texture.h"
#include "CrossPlatform.h"
#include <string>

namespace UE{

    class FontRenderer{
        public:

        FontRenderer() = default;
        ~FontRenderer() = default;

        void Init();
        void Destroy();

        Texture* RenderText(std::string text, std::string fontname, int size);
    };
}