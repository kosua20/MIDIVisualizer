#version 330

in INTERFACE {
	vec2 uv;
} In ;

uniform sampler2D screenTexture;
uniform vec2 inverseScreenSize;
uniform float attenuationFactor = 0.99;

out vec4 fragColor;


void main(){
	
	// We have to unroll the box blur loop manually.
	// 5x5 blur, using a sparse sample grid.
	vec4 color = texture(screenTexture, In.uv);
	
	color += textureOffset(screenTexture, In.uv, 2*ivec2(-2,-2));
	color += textureOffset(screenTexture, In.uv, 2*ivec2(-2, 2));
	color += textureOffset(screenTexture, In.uv, 2*ivec2(-1, 0));
	color += textureOffset(screenTexture, In.uv, 2*ivec2( 0,-1));
	color += textureOffset(screenTexture, In.uv, 2*ivec2( 0, 1));
	color += textureOffset(screenTexture, In.uv, 2*ivec2( 1, 0));
	color += textureOffset(screenTexture, In.uv, 2*ivec2( 2,-2));
	color += textureOffset(screenTexture, In.uv, 2*ivec2( 2, 2));
	
	// Include decay for fade out.
	fragColor = mix(vec4(0.0), color/9.0, attenuationFactor);
	
}
