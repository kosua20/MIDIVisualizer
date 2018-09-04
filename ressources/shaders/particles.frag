#version 330

in INTERFACE {
	vec4 color;
} In;

out vec4 fragColor;


void main(){
	
	fragColor = In.color;
	
}
