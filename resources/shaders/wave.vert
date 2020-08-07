#version 330

layout(location = 0) in vec2 v;

uniform float amplitude;
uniform float keyboardSize;
uniform float freq;
uniform float phase;
uniform float spread;

out INTERFACE {
	float grad;
} Out ;


void main(){
	// Rescale as a thin line.
	vec2 pos = vec2(1.0, spread*0.02) * v.xy;
	// Sin perturbation.
	float waveShift = amplitude * sin(freq * v.x + phase);
	// Apply wave and translate to put on top of the keyboard.
	pos += vec2(0.0, waveShift + (-1.0 + 2.0 * keyboardSize));
	gl_Position = vec4(pos, 0.5, 1.0);
	Out.grad = v.y;
}
