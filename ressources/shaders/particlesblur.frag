#version 330

in INTERFACE {
	vec2 uv;
} In ;

uniform sampler2D screenTexture;
uniform vec2 inverseScreenSize;
uniform vec3 backgroundColor = vec3(0.0, 0.0, 0.0);
uniform float attenuationFactor = 0.99;

out vec3 fragColor;


void main(){
	
	// We have to unroll the box blur loop manually.
	// 5x5 blur, using a sparse sample grid.
	vec3 color = texture(screenTexture, In.uv).rgb;
	
	color += textureOffset(screenTexture, In.uv, 2*ivec2(-2,-2)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(-2,2)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(-1,0)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(0,-1)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(0,1)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(1,0)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(2,-2)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(2,2)).rgb;
	
	// Include decay for fade out.
	fragColor = mix(backgroundColor, color/9.0, attenuationFactor);
	
}
