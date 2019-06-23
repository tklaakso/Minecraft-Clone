#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aOffset;
layout (location = 3) in int aTexture;
layout (location = 4) in int aLightmap;

out vec2 TexCoord;
out int Texture;
out int Lightmap;
out float fog;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 playerPosition;
uniform float fogStart;
uniform float fogEnd;

void main()
{
	gl_Position = projection * view * vec4(aPos + aOffset, 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	Texture = aTexture;
	Lightmap = aLightmap;
	float dist = length((aPos + aOffset) - playerPosition);
	float fogValue = clamp((fogEnd - dist) / (fogEnd - fogStart), 0.0, 1.0);
	fog = fogValue;
}