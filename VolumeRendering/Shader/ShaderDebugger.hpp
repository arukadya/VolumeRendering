//
//  ShaderDebugger.hpp
//  VolumeRendering
//
//  Created by 須之内俊樹 on 2024/09/19.
//

#ifndef ShaderDebugger_hpp
#define ShaderDebugger_hpp

#include "stb_image_write.h"
#include <cstddef>
#include <memory>
#include <new>

struct RGBA {
    unsigned char r, g, b, a; //赤, 緑, 青, 透過
    RGBA() = default;
    constexpr RGBA(const unsigned char r_, const unsigned char g_, const unsigned char b_, const unsigned char a_) :r(r_), g(g_), b(b_), a(a_) {}
};

int generateImage()
{
    constexpr std::size_t width{ 1280 }, height{ 720 }; //幅と高さ

        std::unique_ptr<RGBA[][width]> rgba(new(std::nothrow) RGBA[height][width]);
        if (!rgba) return -1;

        for (std::size_t row{}; row < height; ++row)
            for (std::size_t col{}; col < width; ++col) {
                rgba[row][col].r = 255; //赤
                rgba[row][col].g = 0;
                rgba[row][col].b = 0;
                rgba[row][col].a = 255; //不透過
            }

        stbi_write_png("picture_1.png", static_cast<int>(width), static_cast<int>(height), static_cast<int>(sizeof(RGBA)), rgba.get(), 0);
    return 0;
}
#endif /* ShaderDebugger_hpp */
