#version 330

layout(location = 0) in vec2 v;

uniform float keyboardHeight = 0.25;
uniform vec2 scale;

out INTERFACE {
	float id;
} Out ;

void main(){

	// Translate to put on top of the keyboard.
	vec2 shift = vec2(1.0 - 1.5*scale.x, -1.0 + 2.0 * keyboardHeight + scale.y);
	gl_Position = vec4(v.xy * scale + shift, 0.5, 1.0);

	// Detect which pedal this vertex belong to.
	Out.id = gl_VertexID < 33 ? 0.0 : (gl_VertexID > 64 ? 2.0 : 1.0);
	
}
