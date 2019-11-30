#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	float Durability;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float durability;

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal; 
	vs_out.TexCoords = aTexCoords;
	vs_out.Durability = durability;
    gl_Position = projection * view * vec4(vs_out.FragPos, 1.0);
}