#version 330

in INTERFACE {
	vec2 uv;
	float grad;
} In ;

uniform vec3 waveColor;
uniform float waveOpacity;
uniform sampler2D textureNoise;

out vec4 fragColor;

void main(){
	// Premultiplied alpha.
	vec2 uv = In.uv;
	float noise = texture(textureNoise, uv).r;
	fragColor = noise * waveOpacity * vec4(waveColor, 1.0) * In.grad * 4.0;
}
