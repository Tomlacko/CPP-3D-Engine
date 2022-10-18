#version 450

layout(location = 0) in vec3 fs_position;
layout(location = 1) in vec3 fs_color;

layout(location = 0) out vec4 final_color;

void main()
{
	final_color = vec4(fs_color, 1.0);
}
