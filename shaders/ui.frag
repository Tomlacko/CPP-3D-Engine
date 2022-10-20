#version 450


layout(binding = 0) uniform sampler2D sourceTexture;


layout(location = 0) uniform ivec2 resolution;
layout(location = 1) uniform float time;
//layout(location = 2) uniform uint settingFlags; //0=img, 1=text


layout(location = 0) in vec2 texture_coordinate;

layout(location = 0) out vec4 final_color;



void main() {
    //final_color = vec4(0.0, 0.0, 1.0, 0.5);
    final_color = texelFetch(sourceTexture, ivec2(texture_coordinate), 0);
}
