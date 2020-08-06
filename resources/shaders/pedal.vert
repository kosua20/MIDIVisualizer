#version 330

layout(location = 0) in vec2 v;

uniform vec2 shift;
uniform vec2 scale;

out INTERFACE {
	float id;
} Out ;

void main(){

	// Translate to put on top of the keyboard.
	gl_Position = vec4(v.xy * scale + shift, 0.5, 1.0);

	// Detect which pedal this vertex belong to.
	Out.id = gl_VertexID < 33 ? 0.0 : (gl_VertexID > 64 ? 2.0 : 1.0);
	
}
