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

layout(binding = 2, std140) uniform Object {
	mat4 model_matrix;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
} object;

layout(binding = 0) uniform sampler2D diffuse_texture;

layout(location = 0) in vec3 fs_position;
layout(location = 1) in vec3 fs_normal;
layout(location = 2) in vec2 fs_texture_coordinate;

layout(location = 0) out vec4 final_color;

void main()
{
	vec3 lights_sum = vec3(0.0);
	for(int i = 0; i < lights.length(); i++) {
		Light light = lights[i];

		vec3 light_vector = light.position.xyz - fs_position * light.position.w;
		vec3 L = normalize(light_vector);
		vec3 N = normalize(fs_normal);
		vec3 E = normalize(camera.position - fs_position); 
		vec3 H = normalize(L + E);

		float NdotL = max(dot(N, L), 0.0);
		float NdotH = max(dot(N, H), 0.0);

		float distance2 = light.position.w == 1.0 ? pow(length(light_vector), 2) : 1.0;

		vec3 ambient = object.ambient_color.rgb * light.ambient_color.rgb;
		vec3 diffuse = texture(diffuse_texture, fs_texture_coordinate).rgb * object.diffuse_color.rgb * light.diffuse_color.rgb;
		vec3 specular = object.specular_color.rgb * light.specular_color.rgb;

		vec3 color = ambient.rgb
			+ NdotL * diffuse.rgb
			+ pow(NdotH, object.specular_color.w) * specular;
		color /= distance2;

		lights_sum += color;
	}

	final_color = vec4(lights_sum, 1.0);
}
