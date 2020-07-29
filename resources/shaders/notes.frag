#version 330
#define CHANNELS_COUNT 8

in INTERFACE {
	float isMinor;
	vec2 uv;
	vec2 noteSize;
} In;

uniform vec3 baseColor[CHANNELS_COUNT];
uniform vec3 minorColor[CHANNELS_COUNT];
uniform vec2 inverseScreenSize;
uniform float colorScale;

#define cornerRadius 0.01
#define bottomLimit 0.25

out vec4 fragColor;


void main(){
	
	// If lower area of the screen, discard fragment as it should be hidden behind the keyboard.
	if(gl_FragCoord.y < bottomLimit/inverseScreenSize.y){
		discard;
	}
	
	// Rounded corner (super-ellipse equation).
	float radiusPosition = pow(abs(In.uv.x/(0.5*In.noteSize.x)), In.noteSize.x/cornerRadius) + pow(abs(In.uv.y/(0.5*In.noteSize.y)), In.noteSize.y/cornerRadius);
	
	if(	radiusPosition > 1.0){
		discard;
	}
	
	// Fragment color.
	fragColor.rgb = colorScale * mix(baseColor[0], minorColor[0], In.isMinor);
	
	if(	radiusPosition > 0.8){
		fragColor.rgb *= 1.05;
	}
	fragColor.a = 1.0;
}
