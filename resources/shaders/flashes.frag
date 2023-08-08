#version 330
#define SETS_COUNT 12


in INTERFACE {
	vec2 uv;
	float onChannel;
	float id;
} In;

uniform sampler2D textureFlash;
uniform float time;
uniform vec3 baseColor[SETS_COUNT];
uniform float haloIntensity;
uniform float haloInnerRadius;
uniform float haloOuterRadius;
uniform int texRowCount;
uniform int texColCount;

out vec4 fragColor;


float rand(vec2 co){
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


void main(){
	
	// If not on, discard flash immediatly.
	int cid = int(In.onChannel);
	if(cid < 0){
		discard;
	}
	float mask = 0.0;

	const float atlasSpeed = 15.0;
	const float safetyMargin = 0.05;
	// If up half, read from texture atlas.
	if(In.uv.y > 0.0){
		// Select a sprite, depending on time and flash id.
		int atlasShift = int(floor(mod(atlasSpeed * time, texRowCount*texColCount)) + floor(rand(In.id * vec2(time,1.0))));
		ivec2 atlasIndex = ivec2(atlasShift % texColCount, atlasShift / texColCount);

		// Scale UV to fit in one sprite from atlas.
		vec2 localUV = clamp(In.uv * vec2(1.0, 2.0) + vec2(0.5, 0.0), safetyMargin, 1.0-safetyMargin);
		
		// Read in black and white texture do determine opacity (mask).
		vec2 finalUV = (vec2(atlasIndex) + localUV)/vec2(texColCount, texRowCount);
		mask = texture(textureFlash,finalUV).r;
	}

	// Colored sprite.
	vec4 spriteColor = vec4(baseColor[cid], mask);
	
	// Circular halo effect.

	float haloAlpha = 1.0 - smoothstep(haloInnerRadius, haloOuterRadius, length(In.uv) / 0.5);

	vec4 haloColor;
	haloColor.rgb = baseColor[cid] + haloIntensity * vec3(1.0);
	haloColor.a = haloAlpha * 0.92;
	
	// Mix the sprite color and the halo effect.
	fragColor = mix(spriteColor, haloColor, haloColor.a);
	
	// Boost intensity.
	fragColor *= 1.1;
	// Premultiplied alpha.
	fragColor.rgb *= fragColor.a;
}
