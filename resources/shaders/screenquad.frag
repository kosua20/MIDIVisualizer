#version 330

in INTERFACE {
	vec2 uv;
} In ;

uniform sampler2D screenTexture;
uniform vec2 inverseScreenSize;

out vec4 fragColor;


void main(){
	
	fragColor = texture(screenTexture,In.uv);
	
}
