#version 450


const vec2 quad_verts[4] = {
    vec2(-1, 1),
    vec2(-1, -1),
    vec2(1, 1),
    vec2(1, -1)
};



struct Element {
    vec2 position;
    vec2 scale;
    ivec4 texCoords;
    vec2 anchoring;
};
layout(binding = 0, std430) buffer Elements {
    Element elements[];
};


layout(location = 0) uniform ivec2 resolution;
layout(location = 1) uniform float time;
//layout(location = 2) uniform uint settingFlags; //0=img, 1=text



layout(location = 0) out vec2 texture_coordinate;


const float aspectRatio = float(resolution.y)/float(resolution.x);
//wide: <1
//tall: >1

vec2 adjustToSquare(vec2 coord) {
    coord.x *= aspectRatio<1 ? aspectRatio : 1.0;
    coord.y /= aspectRatio>1 ? aspectRatio : 1.0;
    return coord;
}

vec2 getSize() {
    vec2 rawSize = elements[gl_InstanceID].texCoords.zw - elements[gl_InstanceID].texCoords.xy;
    vec2 realSize = rawSize/min(resolution.x, resolution.y);
    vec2 finalSize = realSize*elements[gl_InstanceID].scale;
    return finalSize;
}

vec2 getPos() {
    return adjustToSquare((quad_verts[gl_VertexID]-elements[gl_InstanceID].anchoring)*getSize()) + elements[gl_InstanceID].position;
}

vec2 getTexCoords() {
    vec2 a = elements[gl_InstanceID].texCoords.xy;
    vec2 b = elements[gl_InstanceID].texCoords.zw;
    return vec2((gl_VertexID&2u)!=0u ? b.x : a.x, (gl_VertexID&1u)!=0u ? b.y : a.y);
}



void main() {
    texture_coordinate = getTexCoords();
    vec2 pos = getPos();
    gl_Position = vec4(pos, 0.0, 1.0);
}