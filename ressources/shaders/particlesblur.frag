#version 330

in INTERFACE {
	vec2 uv;
} In ;

uniform sampler2D screenTexture;
uniform vec2 inverseScreenSize;

out vec3 fragColor;


void main(){
	
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
	fragColor = color*0.99 / 25.0;
	
}
