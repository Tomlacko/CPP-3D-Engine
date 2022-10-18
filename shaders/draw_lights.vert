#version 450

layout(binding = 0, std140) uniform Camera {
	mat4 projection;
	mat4 view;
	vec3 position;
} camera;

struct Light {
	vec4 position;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
};

layout(binding = 1, std430) buffer Lights {
	Light lights[];
};

layout(location = 0) in vec3 position;

layout(location = 0) out vec3 fs_position;
layout(location = 1) out vec3 fs_color;

void main()
{
	vec4 light_position = lights[gl_InstanceID].position;

	fs_position = position * 0.1 + light_position.xyz;
	// we can get rid of directional light this way
	fs_position *= light_position.w;                    
	fs_color = lights[gl_InstanceID].diffuse_color.rgb;	

    gl_Position = camera.projection * camera.view * vec4(fs_position, 1.0);
}
