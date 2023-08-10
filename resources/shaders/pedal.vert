#version 330

layout(location = 0) in vec2 v;

uniform vec2 shift;
uniform vec2 scale;
uniform bool mirror;

out INTERFACE {
	vec2 uv;
} Out ;

void main(){

	// Translate to put on top of the keyboard.
	gl_Position = vec4(v.xy * scale + shift, 0.5, 1.0);
	Out.uv = v.xy + 0.5;
	if(mirror){
		Out.uv.x = 1.0 - Out.uv.x;
	}
}
