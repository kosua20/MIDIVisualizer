#version 330

in INTERFACE {
	vec2 uv;
} In ;

uniform sampler2D screenTexture;
uniform vec2 inverseScreenSize;
uniform float attenuationFactor = 0.99;
uniform float time;

out vec4 fragColor;

vec4 blur(vec2 uv, bool vert){
	vec4 color = 0.2270270270 * texture(screenTexture, uv);
	vec2 pixelOffset = vert ? vec2(0.0, inverseScreenSize.y) : vec2(inverseScreenSize.x, 0.0);

	vec2 texCoordOffset0 = 1.3846153846 * pixelOffset;
	vec4 col0 = texture(screenTexture, uv + texCoordOffset0) + texture(screenTexture, uv - texCoordOffset0);
	color += 0.3162162162 * col0;

	vec2 texCoordOffset1 = 3.2307692308 * pixelOffset;
	vec4 col1 = texture(screenTexture, uv + texCoordOffset1) + texture(screenTexture, uv - texCoordOffset1);
	color += 0.0702702703 * col1;

	return color;
}


void main(){
	
	// Gaussian blur separated in two 1D convolutions, relying on bilinear interpolation to
	// sample multiple pixels at once with the proper weights.

	vec4 color = blur(In.uv, time > 0.5);

	// Include decay for fade out.
	fragColor = mix(vec4(0.0), color, attenuationFactor);
	
}
