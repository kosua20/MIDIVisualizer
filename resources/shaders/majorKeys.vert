#version 330

layout(location = 0) in vec2 v;

out INTERFACE {
	vec2 uv;
} Out ;

uniform bool horizontalMode = false;
uniform float keyboardHeight = 0.25;

vec2 flipIfNeeded(vec2 inPos){
	return horizontalMode ? vec2(inPos.y, -inPos.x) : inPos;
}

void main(){
	// Input are in -0.5,0.5
	// We directly output the position.
	// [-0.5, 0.5] to [-1, -1+2.0*keyboardHeight]
	float yShift = keyboardHeight * (2.0 * v.y + 1.0) - 1.0;

	vec2 pos2D = vec2(v.x * 2.0, yShift);

	gl_Position.xy = flipIfNeeded(pos2D);
	gl_Position.zw = vec2(0.0, 1.0);

	// Output the UV coordinates computed from the positions.
	Out.uv = v.xy + 0.5;
	
}
