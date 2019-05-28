#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aOffset;
layout (location = 3) in int aTexture;

out vec2 TexCoord;
out int Texture;
out float fog;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 playerPosition;
uniform float fogStart;
uniform float fogEnd;

void main()
{
	vec3 pos = projection * view * vec4(aPos + aOffset, 1.0);
	gl_Position = pos;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	Texture = aTexture;
	float dist = length(pos.xyz - playerPosition);
	float fogValue = clamp((fogEnd - dist) / (fogEnd - fogStart), 0.0, 1.0);
	fog = fogValue;
}