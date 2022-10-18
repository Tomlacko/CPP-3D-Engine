#version 450

#define PI 3.1415926538

layout(binding = 0, std140) uniform Camera {
	mat4 projection;
	mat4 view;
	vec3 position;
} camera;

//not needed in vertex shader
/*struct Light {
	vec4 position;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
};

layout(binding = 1, std430) buffer Lights {
	Light lights[];
};*/

layout(binding = 2, std140) uniform Object {
	mat4 model_matrix;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
} object;


//0001b = diffuse, 0010b = normal, 0100b = height, 1000b = specular
//layout(location = 0) uniform uint textureFlags;
layout(location = 1) uniform int handedness; //1 = lefthanded, -1 = righthanded
//To convert a model from a right handed coordinate system to left handed:
//Invert the z-axis by multiplying it with -1.0f (vertex position, normal, etc.)
//Invert the v axis of texture coordinates by subtracting it from one (eg. texcoord.v = 1 - texcoord.v)

layout(location = 2) uniform float time;



layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coordinate; //maps texture position to vertex

layout(location = 0) out vec3 fs_position;
layout(location = 1) out vec3 fs_normal;
layout(location = 2) out vec2 fs_texture_coordinate;




float rand(float k){
	return fract(sin((k+12.9898)*78.233)) * 43758.5453;
}



void main()
{
	vec4 transformedPos = object.model_matrix * vec4(
		position.x,
		position.y,//+sin(time*PI + rand(position.x+(position.y*position.z)))*0.1,
		position.z*handedness,
	1.0);

	fs_position = vec3(transformedPos);
	fs_normal = transpose(inverse(mat3(object.model_matrix))) * (normal*vec3(1.0,1.0,handedness));
	fs_texture_coordinate = vec2(texture_coordinate.s, handedness<0.0 ? 1.0-texture_coordinate.t : texture_coordinate.t);

    gl_Position = camera.projection * camera.view * transformedPos;
}
