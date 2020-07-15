#version 330

in INTERFACE {
	vec2 uv;
} In ;

uniform sampler2D screenTexture;
uniform float textureAlpha;
uniform bool behindKeyboard;

out vec4 fragColor;


void main(){
	fragColor = texture(screenTexture, In.uv);
	fragColor.a *= textureAlpha;
}
