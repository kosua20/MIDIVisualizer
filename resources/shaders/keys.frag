#version 330

in INTERFACE {
	vec2 uv;
} In ;


layout(std140) uniform ActiveNotes {
	ivec4 actives[32];
};

#define CHANNELS_COUNT 8
#define MAJOR_COUNT 75

uniform vec2 inverseScreenSize;
uniform float minorsWidth = 1.0;
uniform vec3 keysColor = vec3(0.0);
uniform vec3 minorColor[CHANNELS_COUNT];
uniform vec3 majorColor[CHANNELS_COUNT];
uniform bool highlightKeys;

uniform int minNoteMajor;
uniform float notesCount;

const bool isMinor[MAJOR_COUNT] = bool[](true, true, false, true, true, true, false,  true, true, false, true, true, true, false,  true, true, false, true, true, true, false,  true, true, false, true, true, true, false,  true, true, false, true, true, true, false,  true, true, false, true, true, true, false,  true, true, false, true, true, true, false,  true, true, false, true, true, true, false,  true, true, false, true, true, true, false,  true, true, false, true, true, true, false,  true, true, false, true, false);

const int majorIds[MAJOR_COUNT] = int[](0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21, 23, 24, 26, 28, 29, 31, 33, 35, 36, 38, 40, 41, 43, 45, 47, 48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67, 69, 71, 72, 74, 76, 77, 79, 81, 83, 84, 86, 88, 89, 91, 93, 95, 96, 98, 100, 101, 103, 105, 107, 108, 110, 112, 113, 115, 117, 119, 120, 122, 124, 125, 127);
const int minorIds[MAJOR_COUNT] = int[](1, 3, 0, 6, 8, 10, 0, 13, 15, 0, 18, 20, 22, 0, 25, 27, 0, 30, 32, 34, 0, 37, 39, 0, 42, 44, 46, 0, 49, 51, 0, 54, 56, 58, 0, 61, 63, 0, 66, 68, 70, 0, 73, 75, 0, 78, 80, 82, 0, 85, 87, 0, 90, 92, 94, 0, 97, 99, 0, 102, 104, 106, 0, 109, 111, 0, 114, 116, 118, 0, 121, 123, 0, 126, 0);

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
	float intensity = int(abs(fract(In.uv.x * notesCount)) >= 2.0 * notesCount * inverseScreenSize.x);
	
	// If the current major key is active, the majorColor is specific.
	int majorId = majorIds[clamp(int(In.uv.x * notesCount) + minNoteMajor, 0, 74)];
	int cidMajor = isIdActive(majorId);
	vec3 backColor = (highlightKeys && cidMajor >= 0) ? majorColor[cidMajor] : vec3(1.0);

	vec3 frontColor = keysColor;
	// Upper keyboard.
	if(In.uv.y > 0.4){
		int minorLocalId = min(int(floor(In.uv.x * notesCount + 0.5) + minNoteMajor) - 1, 74);
		// Handle black keys.
		if(minorLocalId >= 0 && isMinor[minorLocalId]){
			// If the minor keys are not thinner, preserve a 1 px margin on each side.
			float marginSize = minorsWidth != 1.0 ? minorsWidth : 1.0 - (2.0 * notesCount * inverseScreenSize.x);
			intensity = step(marginSize, abs(fract(In.uv.x * notesCount + 0.5) * 2.0 - 1.0));
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
