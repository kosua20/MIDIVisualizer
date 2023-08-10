#version 330

in INTERFACE {
	vec2 uv;
} In ;


uniform vec2 inverseScreenSize;

uniform vec3 pedalColor;
uniform float pedalOpacity;

uniform sampler2D pedalTexture;
uniform float pedalFlag;

out vec4 fragColor;


void main(){

	vec2 uv = In.uv;
	float pedalMask = texture(pedalTexture, uv).r;
	float finalOpacity = pedalMask * mix(pedalOpacity, 1.0, pedalFlag);
	fragColor = vec4(pedalColor, finalOpacity);
}
