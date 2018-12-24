#version 330

in INTERFACE {
	vec2 uv;
} In ;

uniform sampler2D screenTexture;
uniform vec2 inverseScreenSize;
uniform vec3 backgroundColor = vec3(0.0, 0.0, 0.0);

out vec3 fragColor;


void main(){
	
	const float attenuationFactor = 0.99;
	
	// We have to unroll the box blur loop manually.
	// 5x5 box blur, taking every other pixel.
	vec3 color = textureOffset(screenTexture, In.uv, 2*ivec2(-2,-2)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(-2,-1)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(-2,0)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(-2,1)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(-2,2)).rgb;
	
	color += textureOffset(screenTexture, In.uv, 2*ivec2(-1,-2)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(-1,-1)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(-1,0)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(-1,1)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(-1,2)).rgb;
	
	color += textureOffset(screenTexture, In.uv, 2*ivec2(0,-2)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(0,-1)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(0,0)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(0,1)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(0,2)).rgb;
	
	color += textureOffset(screenTexture, In.uv, 2*ivec2(1,-2)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(1,-1)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(1,0)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(1,1)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(1,2)).rgb;
	
	color += textureOffset(screenTexture, In.uv, 2*ivec2(2,-2)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(2,-1)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(2,0)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(2,1)).rgb;
	color += textureOffset(screenTexture, In.uv, 2*ivec2(2,2)).rgb;
	
	// Include decay for fade out.
	fragColor = mix(backgroundColor, color/25.0, attenuationFactor);
	
}
