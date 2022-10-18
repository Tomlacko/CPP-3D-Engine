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

//https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL)
layout(binding = 1, std430) buffer Lights {
	Light lights[]; //last shader_storage member, can use unspecified range
};

layout(binding = 2, std140) uniform Object {
	mat4 model_matrix;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
} object;


layout(binding = 0) uniform sampler2D diffuse_texture;
layout(binding = 1) uniform sampler2D normal_map;
layout(binding = 2) uniform sampler2D height_map;
layout(binding = 3) uniform sampler2D specular_map;



//0001b = diffuse, 0010b = normal, 0100b = height, 1000b = specular
layout(location = 0) uniform uint textureFlags;
//layout(location = 1) uniform int handedness; //1 = lefthanded, -1 = righthanded

layout(location = 2) uniform float time;




layout(location = 0) in vec3 fs_position;
layout(location = 1) in vec3 fs_normal;
layout(location = 2) in vec2 fs_texture_coordinate;

layout(location = 0) out vec4 final_color;



vec3 fragToCamVec = normalize(camera.position - fs_position);
vec2 effectiveTexCoords = fs_texture_coordinate; //default, may be updated with parallax


//http://www.thetenthplanet.de/archives/1180
mat3 TBN(vec3 N, vec3 p, vec2 uv) {
	//get edge vectors of the pixel triangle
	vec3 dp1 = dFdx( p );
	vec3 dp2 = dFdy( p );
	vec2 duv1 = dFdx( uv );
	vec2 duv2 = dFdy( uv );

	//solve the linear system
	vec3 dp2perp = cross( dp2, N );
	vec3 dp1perp = cross( N, dp1 );
	vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
	vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;

	//construct a scale-invariant frame
	float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
	return mat3( T * invmax, B * invmax, N );
}

vec3 perturb_normal(vec3 N, vec3 V, vec2 texcoord) {
	// assume N, the interpolated vertex normal and
	// V, the view vector (vertex to eye)
	return normalize(TBN(N, -V, texcoord) * normalize(texture(normal_map, texcoord).xyz*2.0 - 1.0));
}


//fs_texture_coordinate
vec3 getDiffuse() {
	return (textureFlags&1u)!=0 ? texture(diffuse_texture, fs_texture_coordinate).rgb : object.diffuse_color.rgb;
}

vec3 getNormal() {
	return (textureFlags&2u)!=0 ? perturb_normal(fs_normal, fragToCamVec, fs_texture_coordinate) : normalize(fs_normal);
}

/*
float getHeight() {
	return (textureFlags&4u)!=0 ? 1 : 0;
}

vec3 getSpecular() {
	return (textureFlags&8u)!=0 ? texture(specular_map, fs_texture_coordinate).rgb : object.specular_color.rgb;
}*/


/*
vec3 getDiffuse(vec2 texcoords) {
	return (textureFlags&1u) > 0 ? texture(diffuse_texture, fs_texture_coordinate).rgb : object.diffuse_color.rgb;
}

vec3 getNormal(vec2 texcoords) {
	return (textureFlags&2u) > 0 ? perturb_normal(fs_normal, fragToCamVec, fs_texture_coordinate) : normalize(fs_normal);
}*/



//fragToCamVec = E
//fragNormalVec = N

void main()
{
	vec3 fragNormalVec = perturb_normal(fs_normal, fragToCamVec, fs_texture_coordinate);
	//vec3 fragNormalVec = normalize(fs_normal);

	vec3 lights_sum = vec3(0.0);
	for(int i = 0; i < lights.length(); i++) {
		Light light = lights[i];


		vec3 light_vector = light.position.xyz - fs_position; //* light.position.w; //dependant on light type
		vec3 L = normalize(light_vector);
		vec3 H = normalize(L + fragToCamVec);

		float NdotH = max(dot(fragNormalVec, H), 0.0);
		float NdotL = max(dot(fragNormalVec, L), 0.0);

		//w == 1 is point light, 0 is directional
		float distance2 = light.position.w == 0.0 ? 1.0 : max(1.0, pow(length(light_vector)/light.position.w, 2));

		//TODO - replace with specular and bump-map texture, (ambient texture would be always glowing?)
		vec3 ambient = object.ambient_color.rgb * light.ambient_color.rgb;
		vec3 diffuse = texture(diffuse_texture, fs_texture_coordinate).rgb * object.diffuse_color.rgb * light.diffuse_color.rgb;
		vec3 specular = object.specular_color.rgb * light.specular_color.rgb;

		//custom, wrong coloring
		//vec3 ambient = /*object.ambient_color.rgb * */ light.ambient_color.rgb;
		//vec3 diffuse = texture(diffuse_texture, fs_texture_coordinate).rgb * /*object.diffuse_color.rgb * */ light.diffuse_color.rgb;
		//vec3 specular = /*object.specular_color.rgb * */ light.specular_color.rgb;

		vec3 color = ambient.rgb
			+ NdotL * diffuse.rgb
			+ pow(NdotH, object.specular_color.w) * specular;
		color /= distance2;

		lights_sum += color;
	}

	final_color = vec4(lights_sum, 1.0);
}
