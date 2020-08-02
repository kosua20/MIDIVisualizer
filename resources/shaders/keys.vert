#version 330

layout(location = 0) in vec2 v;

out INTERFACE {
	vec2 uv;
} Out ;

uniform float keyboardHeight = 0.25;

void main(){
	// Input are in -0.5,0.5
	// We directly output the position.
	// [-0.5, 0.5] to [-1, 2.0*keyboardHeight-1.0]
	float yShift = keyboardHeight * (2.0 * v.y + 1.0) - 1.0;
	gl_Position = vec4(v.x*2.0, yShift, 0.0, 1.0);
	// Output the UV coordinates computed from the positions.
	Out.uv = v.xy + 0.5;
	
}
