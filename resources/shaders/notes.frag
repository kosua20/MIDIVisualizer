#version 330
#define SETS_COUNT 12

in INTERFACE {
	vec2 uv;
	vec2 noteSize;
	float isMinor;
	float channel;
} In;

uniform vec3 baseColor[SETS_COUNT];
uniform vec3 minorColor[SETS_COUNT];
uniform vec2 inverseScreenSize;
uniform float colorScale;
uniform float keyboardHeight = 0.25;
uniform float fadeOut = 0.0;
uniform bool horizontalMode = false;

#define cornerRadius 0.01

out vec4 fragColor;


void main(){
	
	// If lower area of the screen, discard fragment as it should be hidden behind the keyboard.
	vec2 normalizedCoord = vec2(gl_FragCoord.xy) * inverseScreenSize;

	if((horizontalMode ? normalizedCoord.x : normalizedCoord.y) < keyboardHeight){
		discard;
	}
	
	// Rounded corner (super-ellipse equation).
	float radiusPosition = pow(abs(In.uv.x/(0.5*In.noteSize.x)), In.noteSize.x/cornerRadius) + pow(abs(In.uv.y/(0.5*In.noteSize.y)), In.noteSize.y/cornerRadius);
	
	if(	radiusPosition > 1.0){
		discard;
	}
	
	// Fragment color.
	int cid = int(In.channel);
	fragColor.rgb = colorScale * mix(baseColor[cid], minorColor[cid], In.isMinor);
	
	if(	radiusPosition > 0.8){
		fragColor.rgb *= 1.05;
	}


	float distFromBottom = horizontalMode ? normalizedCoord.x : normalizedCoord.y;
	float fadeOutFinal = min(fadeOut, 0.9999);
	distFromBottom = max(distFromBottom - fadeOutFinal, 0.0) / (1.0 - fadeOutFinal);
	float alpha = 1.0 - distFromBottom;
	fragColor.a = alpha;
}
