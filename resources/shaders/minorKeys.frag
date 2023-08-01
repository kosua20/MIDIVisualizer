#version 330

in INTERFACE {
	vec2 uv;
	float id;
} In ;

#define SETS_COUNT 12

uniform bool highlightKeys;
uniform bool horizontalMode = false;
uniform float keyboardHeight = 0.25;
uniform float notesCount; // (maxNoteMajor - minNoteMajor + 1)
uniform int minNoteMajor;
uniform vec2 inverseScreenSize;
uniform vec3 edgeColor = vec3(0.0);

uniform vec3 minorColor[SETS_COUNT];
uniform bool edgeOnMinors;
uniform float minorsHeight = 0.6;
uniform float minorsWidth = 1.0;

vec2 minorShift(int id){
	if(id == 1 || id == 6){
		return vec2(0.0, 0.3);
	}
	if(id == 3 || id == 10){
		return vec2(0.3, 0.0);
	}
	return vec2(0.1,0.1);
}

out vec4 fragColor;


void main(){

	// Center uvs
	vec2 ndc = 2.0 * In.uv - 1.0;

	float widthScaling = horizontalMode ? inverseScreenSize.y : inverseScreenSize.x;
	float heightScaling = horizontalMode ? inverseScreenSize.x : inverseScreenSize.y;
	float uvWidth = minorsWidth / float(notesCount) * 0.5;
	float uvHeight = minorsHeight * keyboardHeight * 0.5;

	// Edges on the sides and bottom.
	float xStep = step(1.0 - 2.0 * widthScaling / uvWidth, abs(ndc.x));
	float yStep = step(1.0 - 2.0 * heightScaling / uvHeight, -ndc.y);
	float centerIntensity = edgeOnMinors ? ((1.0 - xStep) * (1.0 - yStep)) : 1.0;

	// Key color changes when active.
	int cidMinor = int(In.id);
	vec3 frontColor = (highlightKeys && cidMinor >= 0) ? minorColor[cidMinor] : edgeColor;

	// Mix
	fragColor.rgb = mix(edgeColor, frontColor, centerIntensity);
	fragColor.a = 1.0;
}
