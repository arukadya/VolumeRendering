#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

// ボリュームテクスチャのサンプラ
uniform sampler3D volume;

// テクスチャ座標
in vec3 t;
// フレームバッファに出力するデータ
layout (location = 0) out vec4 fc;
//透明度の閾値
uniform float threshold;

void main()
{
//    float transparency = texture(volume, t).r;
    float opacity = texture(volume, t).r;
//    if(opacity - threshold < 0.0)discard;
//    vec3 smoke_color = vec3(1.0, 0.0, 0.0);
    vec3 smoke_color = vec3(1.0, 0.0, 0.0);
//    fc = vec4(smoke_color, transparency);
    fc = vec4(smoke_color, opacity*5);
//    fc = vec4(smoke_color, 0.1);
}

