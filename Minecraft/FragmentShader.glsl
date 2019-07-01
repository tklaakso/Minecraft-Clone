#version 450 core
out vec4 FragColor;
in vec2 TexCoord;
flat in int Texture;
flat in int Lightmap;
in float fog;

uniform sampler2DArray textures;

void main()
{
	if (Texture < 0){
		discard;
	}
	float scale = float(Lightmap) / 255.0;
	FragColor = texture(textures, vec3(TexCoord, Texture)) * scale * fog + vec4(1.0, 1.0, 0.0, 1.0) * (1.0 - fog);
}