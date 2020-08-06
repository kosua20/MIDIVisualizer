#version 330

layout(location = 0) in vec2 v;

uniform float amplitude;
uniform float time;
uniform float shift;
uniform float freqPos = 10.0;
uniform float freqTime = 10.0;
uniform float hash = 1.0;

out INTERFACE {
	float grad;
} Out ;

void main(){

	// Translate to put on top of the keyboard.
	vec2 pos = vec2(1.0, 0.02) * v.xy;
	float waveShift = amplitude * sin(freqPos * v.x + freqTime * time + hash * 7.31);
	pos += vec2(0.0, waveShift + (-1.0 + 2.0 * shift));
	gl_Position = vec4(pos, 0.5, 1.0);
	Out.grad = v.y;
}
