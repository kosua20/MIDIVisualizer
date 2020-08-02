#version 330

in INTERFACE {
	vec2 uv;
} In ;


layout(std140) uniform ActiveNotes {
	ivec4 actives[22];
};

#define CHANNELS_COUNT 8

uniform vec2 inverseScreenSize;
uniform float minorsWidth = 1.0;
uniform vec3 keysColor = vec3(0.0);
uniform vec3 minorColor[CHANNELS_COUNT];
uniform vec3 majorColor[CHANNELS_COUNT];
uniform bool highlightKeys;

const bool isMinor[52] = bool[](true, false, true, true, false, true, true, true, false, true, true, false, true, true, true, false, true, true, false, true, true, true, false, true, true, false, true, true, true, false, true, true, false, true, true, true, false, true, true, false, true, true, true, false, true, true, false, true, true, true, false, false);
const int majorIds[52] = int[](0, 2, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 20, 22, 24, 26, 27, 29, 31, 32, 34, 36, 38, 39, 41, 43, 44, 46, 48, 50, 51, 53, 55, 56, 58, 60, 62, 63, 65, 67, 68, 70, 72, 74, 75, 77, 79, 80, 82, 84, 86, 87);
const int minorIds[52] = int[](1, 0, 4, 6, 0, 9, 11, 13, 0, 16, 18, 0, 21, 23, 25, 0, 28, 30, 0, 33, 35, 37, 0, 40, 42, 0, 45, 47, 49, 0, 52, 54, 0, 57, 59, 61, 0, 64, 66, 0, 69, 71, 73, 0, 76, 78, 0, 81, 83, 85, 0, 0);

out vec4 fragColor;


int isIdActive(int id){
	return actives[id/4][id%4];
}

void main(){
	// White keys: white
	// Black keys: keyColor
	// Lines between keys: keyColor
	// Active key: activeColor

	// White keys, and separators.
	float intensity = int(abs(fract(In.uv.x*52.0)) >= 2.0 * 52.0 * inverseScreenSize.x);
	
	// If the current major key is active, the majorColor is specific.
	int majorId = majorIds[clamp(int(In.uv.x*52.0), 0, 51)];
	int cidMajor = isIdActive(majorId);
	vec3 backColor = (highlightKeys && cidMajor >= 0) ? majorColor[cidMajor] : vec3(1.0);

	vec3 frontColor = keysColor;
	// Upper keyboard.
	if(In.uv.y > 0.4){
		int minorLocalId = min(int(floor(In.uv.x*52.0+0.5))-1, 51);
		// Handle black keys.
		if(minorLocalId >= 0 && isMinor[minorLocalId]){
			// If the minor keys are not thinner, preserve a 1 px margin on each side.
			float marginSize = minorsWidth != 1.0 ? minorsWidth : 1.0 - (2.0*52.0*inverseScreenSize.x);
			intensity = step(marginSize, abs(fract(In.uv.x*52.0+0.5)*2.0-1.0));
			int minorId = minorIds[minorLocalId];
			int cidMinor = isIdActive(minorId);
			if(highlightKeys && cidMinor >= 0){
				frontColor = minorColor[cidMinor];
			}
		}
	}
	
	fragColor.rgb = mix(frontColor, backColor, intensity);
	fragColor.a = 1.0;
}
