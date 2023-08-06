#version 330
#define SETS_COUNT 12

in INTERFACE {
	vec2 uv;
	vec2 noteSize;
	flat vec2 noteCorner;
	float isMinor;
	float channel;
} In;

uniform vec3 baseColor[SETS_COUNT];
uniform vec3 minorColor[SETS_COUNT];
uniform vec2 inverseScreenSize;
uniform float time;
uniform float mainSpeed;
uniform float colorScale;
uniform float keyboardHeight = 0.25;
uniform float fadeOut;
uniform float edgeWidth;
uniform float edgeBrightness;
uniform float cornerRadius;
uniform bool horizontalMode;
uniform bool reverseMode;


uniform sampler2D majorTexture;
uniform sampler2D minorTexture;
uniform bool useMajorTexture;
uniform bool useMinorTexture;
uniform vec2 texturesScale;
uniform vec2 texturesStrength;
uniform bool scrollMajorTexture;
uniform bool scrollMinorTexture;

out vec4 fragColor;

vec2 flipIfNeeded(vec2 inPos){
	return horizontalMode ? vec2(inPos.y, -inPos.x) : inPos;
}

void main(){
	
	vec2 normalizedCoord = vec2(gl_FragCoord.xy) * inverseScreenSize;

	vec3 tinting = vec3(1.0);
	vec2 tintingUV = 2.0 * normalizedCoord - 1.0;

	// Preserve screen pixel density, corrected for aspect ratio (on X so that preserving scrolling speed is easier).
	vec2 aspectRatio = vec2(inverseScreenSize.y / inverseScreenSize.x, 1.0);
	vec2 tintingScale = aspectRatio;
	tintingScale.x *= (horizontalMode && !reverseMode ? -1.0 : 1.0);
	tintingScale.y *= (!horizontalMode && reverseMode ? -1.0 : 1.0);

	if(useMajorTexture){
		vec2 texUVOffset = scrollMajorTexture ? In.noteCorner : vec2(0.0);
		vec2 texUV = texturesScale.x * tintingScale * (tintingUV - texUVOffset);
		texUV = flipIfNeeded(texUV);
		// Only on major notes.
		float intensity = (1.0 - In.isMinor) * texturesStrength.x;
		tinting = mix(tinting, texture(majorTexture, texUV).rgb, intensity);
	}
	if(useMinorTexture){
		vec2 texUVOffset = scrollMinorTexture ? In.noteCorner : vec2(0.0);
		vec2 texUV = texturesScale.y * tintingScale * (tintingUV - texUVOffset);
		texUV = flipIfNeeded(texUV);
		// Only on minor notes.
		float intensity = In.isMinor * texturesStrength.y;
		tinting = mix(tinting, texture(minorTexture, texUV).rgb, intensity);
	}
	
	// If lower area of the screen, discard fragment as it should be hidden behind the keyboard.
	if((horizontalMode ? normalizedCoord.x : normalizedCoord.y) < keyboardHeight){
		discard;
	}
	
	// Rounded corner (super-ellipse equation).
	vec2 ellipseCoords = abs(In.uv / (0.5 * In.noteSize));
	vec2 ellipseExps = In.noteSize / max(cornerRadius, 1e-3);
	float radiusPosition = pow(ellipseCoords.x, ellipseExps.x) + pow(ellipseCoords.y, ellipseExps.y);
	
	if(	radiusPosition > 1.0){
		discard;
	}

	// Fragment color.
	int cid = int(In.channel);
	fragColor.rgb = tinting * colorScale * mix(baseColor[cid], minorColor[cid], In.isMinor);

	// Apply scaling factor to edge.
	float deltaPix = fwidth(In.uv.x) * 4.0;
	float edgeIntensity = smoothstep(1.0 - edgeWidth - deltaPix, 1.0 - edgeWidth + deltaPix, radiusPosition);
	fragColor.rgb *= (1.0f + (edgeBrightness - 1.0f) * edgeIntensity);

	float distFromBottom = horizontalMode ? normalizedCoord.x : normalizedCoord.y;
	float fadeOutFinal = min(fadeOut, 0.9999);
	distFromBottom = max(distFromBottom - fadeOutFinal, 0.0) / (1.0 - fadeOutFinal);
	float alpha = 1.0 - distFromBottom;

	fragColor.a = alpha;
}
