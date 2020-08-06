#version 330

in INTERFACE {
	float grad;
} In ;


uniform vec2 inverseScreenSize;


uniform vec3 waveColor;

out vec4 fragColor;


void main(){

	float intensity = (1.0-abs(In.grad));
	fragColor = vec4((1.0+2.0*intensity)*waveColor, intensity);
}
