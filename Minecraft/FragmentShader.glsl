#version 450 core
out vec4 FragColor;
in vec2 TexCoord;
flat in int Texture;
in float fog;

uniform sampler2DArray textures;

void main()
{
	if (Texture < 0){
		discard;
	}
	FragColor = texture(textures, vec3(TexCoord, Texture)) * (1.0 - fog) + vec3(1.0, 1.0, 1.0, 1.0) * fog;
}