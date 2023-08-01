#version 330

layout(location = 0) in vec2 v;

out INTERFACE {
	vec2 uv;
	float id;
} Out ;

uniform bool horizontalMode = false;
uniform float keyboardHeight = 0.25;
uniform float notesCount; // (maxNoteMajor - minNoteMajor + 1)
uniform int actives[128];
uniform int minNoteMajor;

uniform float minorsHeight = 0.6;
uniform float minorsWidth = 1.0;

vec2 flipIfNeeded(vec2 inPos){
	return horizontalMode ? vec2(inPos.y, -inPos.x) : inPos;
}

#define MINOR_COUNT 53

const int minorIds[MINOR_COUNT] = int[](1, 3, 6, 8, 10, 13, 15, 18, 20, 22, 25, 27, 30, 32, 34, 37, 39, 42, 44, 46, 49, 51, 54, 56, 58, 61, 63, 66, 68, 70, 73, 75, 78, 80, 82, 85, 87, 90, 92, 94,  97, 99,  102, 104, 106, 109, 111, 114, 116, 118, 121, 123, 126);

const int noteDelta[12] = int[](0, 0, 1, 1, 2, 3, 3, 4, 4, 5, 5, 6);


float minorShift(int id){
   if(id == 1 || id == 6){
	   return -0.1;
   }
   if(id == 3 || id == 10){
	   return 0.1;
   }
   return 0.0;
}

void main(){

	float noteWidth = 2.0 / notesCount;
	// Size of the note : width, height based on duration and current speed.
	vec2 noteSize = vec2(0.9 * noteWidth * minorsWidth, 2.0 * minorsHeight * keyboardHeight);

	// Compute note shift.
	// Horizontal shift based on note id, width of keyboard, and if the note is minor or not.
	//float a = (1.0/(notesCount-1.0)) * (2.0 - 2.0/notesCount);
	//float b = -1.0 + 1.0/notesCount;
	// This should be in -1.0, 1.0.
	// input: noteId is in [0 MAJOR_COUNT]
	// we want minNote to -1+1/c, maxNote to 1-1/c
	float a = 2.0;
	float b = -notesCount + 1.0 - 2.0 * float(minNoteMajor);

	int minorId = minorIds[gl_InstanceID];
	int noteId = (minorId/12) * 7 + noteDelta[minorId % 12];
	float horizLoc = (float(noteId) * a + b + 1.0) / notesCount;

	float vertLoc = 2.0 * (1.0 - minorsHeight) * keyboardHeight - 1.0 + noteSize.y * 0.5;
	vec2 noteShift = vec2(horizLoc, vertLoc);

	noteShift.x += minorShift(minorId % 12) * noteSize.x;

	// Output position.
	gl_Position = vec4(flipIfNeeded(noteSize * v + noteShift), 0.0 , 1.0) ;

	// Discard keys that are too close to the screen edges.
	if(abs(noteShift.x) >= 1.0 - 0.5 * noteWidth){
		gl_Position = vec4(-40000.0);
	}
	
	// Output the UV coordinates computed from the positions.
	Out.uv = v.xy + 0.5;
	// And the active channel if present.
	Out.id = float(actives[minorId]);
	
}
