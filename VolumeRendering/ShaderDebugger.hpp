//
//  ShaderDebugger.hpp
//  VolumeRendering
//
//  Created by 須之内俊樹 on 2024/09/20.
//

#ifndef ShaderDebugger_hpp
#define ShaderDebugger_hpp

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image_write.h"
#include <memory>
#include <new>
#include <filesystem>
#include "Command/ComputeCommand.hpp"


struct RGBA {
    unsigned char r, g, b, a; //赤, 緑, 青, 透過
    RGBA() = default;
    constexpr RGBA(const unsigned char r_, const unsigned char g_, const unsigned char b_, const unsigned char a_) :r(r_), g(g_), b(b_), a(a_) {}
};

int buffer_write_png(unsigned int buffer_width, unsigned int buffer_height, unsigned int buffer_depth, unsigned int z_range, float* buffer, std::string &bufferName);
#endif /* ShaderDebugger_hpp */
