#version 450

//screen-covering triangle
/*const vec2 quad_verts[3] = {
    vec2(-1, -1),
    vec2(3, -1),
    vec2(-1, 3)
};*/

//screen-covering quad
/*const vec2 quad_verts[6] = {
    vec2(-1, 1),
    vec2(-1, -1),
    vec2(1, -1),
    vec2(1, -1),
    vec2(1, 1),
    vec2(-1, 1)
};*/

//quad from GL_TRIANGLE_STRIP
/*const vec2 quad_verts[4] = {
    vec2(-1, 1),
    vec2(-1, -1),
    vec2(1, 1),
    vec2(1, -1)
};*/



void main()
{
    // https://rauwendaal.net/2014/06/14/rendering-a-screen-covering-triangle-in-opengl/
    float x = -1.0 + float((gl_VertexID & 1) << 2);  // -1   3  -1
    float y = -1.0 + float((gl_VertexID & 2) << 1);  // -1  -1   3

    gl_Position = vec4(x, y, 0, 1);

    //gl_Position = vec4(quad_verts[gl_VertexID], 0, 1);
}