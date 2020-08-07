#version 330

in INTERFACE {
	float grad;
} In ;

uniform vec3 waveColor;
uniform float waveOpacity;

out vec4 fragColor;

void main(){
	// Fade out on the edges.
	float intensity = (1.0-abs(In.grad));
	// Premultiplied alpha.
	fragColor = waveOpacity * intensity * vec4(waveColor, 1.0);
}
