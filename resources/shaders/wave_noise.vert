#version 330

layout(location = 0) in vec2 v;

uniform float keyboardSize;
uniform float scale;
uniform bool horizontalMode;
uniform float noiseScale;
uniform vec2 inverseScreenSize;
uniform float offset;

vec2 flipIfNeeded(vec2 inPos){
	return horizontalMode ? vec2(inPos.y, -inPos.x) : inPos;
}

out INTERFACE {
	vec2 uv;
	float grad;
} Out ;


void main(){
	// Rescale as a thin quad
	vec2 pos = 2.0 * v.xy * vec2(1.0, 2.0 * scale);
	// Apply amplitude and translate to put on top of the keyboard.
	pos.y +=  -1.0 + 2.0 * keyboardSize + 2.0 * scale;

	gl_Position = vec4(flipIfNeeded(pos), 0.8, 1.0);

	float ratio = horizontalMode ? (inverseScreenSize.y / inverseScreenSize.x) : (inverseScreenSize.x / inverseScreenSize.y);
	Out.uv = (v.xy + 0.5) * vec2(1.0, scale) * noiseScale * vec2(1.0, ratio);
	Out.uv.y -= offset;

	Out.grad = 0.5-v.y;
}
