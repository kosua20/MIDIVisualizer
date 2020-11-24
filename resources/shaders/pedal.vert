#version 330

layout(location = 0) in vec2 v;

uniform vec2 shift;
uniform vec2 scale;

out INTERFACE {
	float id;
} Out ;

#define SOSTENUTO 33
#define DAMPER 65
#define SOFT 97
#define EXPRESSION -1 damper, soft, expression

void main(){

	// Translate to put on top of the keyboard.
	gl_Position = vec4(v.xy * scale + shift, 0.5, 1.0);

	// Detect which pedal this vertex belong to.
	Out.id = gl_VertexID < SOSTENUTO ? 0.0 :
			(gl_VertexID < DAMPER ? 1.0 :
			(gl_VertexID < SOFT ? 2.0 :
			3.0
			));
	
}
