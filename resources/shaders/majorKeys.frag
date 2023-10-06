#version 330

in INTERFACE {
	vec2 uv;
} In ;

#define SETS_COUNT 12
#define MAJOR_COUNT 75

uniform bool highlightKeys;
uniform bool horizontalMode = false;
uniform float notesCount; // (maxNoteMajor - minNoteMajor + 1)
uniform int actives[128];
uniform int minNoteMajor;
uniform vec2 inverseScreenSize;
uniform vec3 edgeColor = vec3(0.0);
uniform vec3 keyColor = vec3(1.0);

uniform vec3 majorColor[SETS_COUNT];

const int majorIds[MAJOR_COUNT] = int[](0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21, 23, 24, 26, 28, 29, 31, 33, 35, 36, 38, 40, 41, 43, 45, 47, 48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67, 69, 71, 72, 74, 76, 77, 79, 81, 83, 84, 86, 88, 89, 91, 93, 95, 96, 98, 100, 101, 103, 105, 107, 108, 110, 112, 113, 115, 117, 119, 120, 122, 124, 125, 127);

out vec4 fragColor;


void main(){

	// White keys, and separators.
	float widthScaling = horizontalMode ? inverseScreenSize.y : inverseScreenSize.x;
	float majorUvX = fract(In.uv.x * notesCount);

	// Edges on the sides
	float centerIntensity = 1.0 - step( 1.0 - 2.0 * notesCount * widthScaling, abs(majorUvX * 2.0 - 1.0));

	// If the current major key is active, the majorColor is specific.
	int majorId = majorIds[clamp(int(In.uv.x * notesCount) + minNoteMajor, 0, 74)];
	int cidMajor = actives[majorId];
	vec3 frontColor = (highlightKeys && cidMajor >= 0) ? majorColor[cidMajor] : keyColor;

	// Mix.
	fragColor.rgb = mix(edgeColor, frontColor, centerIntensity);
	fragColor.a = 1.0;
}
