#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;
uniform vec3 color;

void main()
{             
	//vec4 sampled = vec4(1.0, 1.0, 1.0, texture(image, TexCoords).r);
    FragColor = vec4(color, 1.0) * texture(image, TexCoords);
}