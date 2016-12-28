#version 330

in INTERFACE {
	vec2 uv;
} In ;

uniform sampler2D screenTexture;
uniform vec2 inverseScreenSize;

out vec3 fragColor;


void main(){
	
	fragColor = texture(screenTexture,In.uv).rgb;
	
}
