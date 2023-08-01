#version 330

layout(location = 0) in vec2 v;
layout(location = 1) in vec4 id; //note id, start, duration, is minor
layout(location = 2) in float channel; //note id, start, duration, is minor

uniform float time;
uniform float mainSpeed;
uniform float minorsWidth = 1.0;
uniform float keyboardHeight = 0.25;
uniform bool reverseMode = false;
uniform bool horizontalMode = false;

vec2 flipIfNeeded(vec2 inPos){
	return horizontalMode ? vec2(inPos.y, -inPos.x) : inPos;
}

uniform int minNoteMajor;
uniform float notesCount;

out INTERFACE {
	vec2 uv;
	vec2 noteSize;
	float isMinor;
	float channel;
} Out;

#define MAJOR_COUNT 75
const int minorIds[MAJOR_COUNT] = int[](1, 3, 0, 6, 8, 10, 0, 13, 15, 0, 18, 20, 22, 0, 25, 27, 0, 30, 32, 34, 0, 37, 39, 0, 42, 44, 46, 0, 49, 51, 0, 54, 56, 58, 0, 61, 63, 0, 66, 68, 70, 0, 73, 75, 0, 78, 80, 82, 0, 85, 87, 0, 90, 92, 94, 0, 97, 99, 0, 102, 104, 106, 0, 109, 111, 0, 114, 116, 118, 0, 121, 123, 0, 126, 0);

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
	
	float scalingFactor = id.w != 0.0 ? minorsWidth : 1.0;
	// Size of the note : width, height based on duration and current speed.
	Out.noteSize = vec2(0.9*2.0/notesCount * scalingFactor, id.z*mainSpeed);
	
	// Compute note shift.
	// Horizontal shift based on note id, width of keyboard, and if the note is minor or not.
	// Vertical shift based on note start time, current time, speed, and height of the note quad.
	//float a = (1.0/(notesCount-1.0)) * (2.0 - 2.0/notesCount);
	//float b = -1.0 + 1.0/notesCount;
	// This should be in -1.0, 1.0.
	// input: id.x is in [0 MAJOR_COUNT]
	// we want minNote to -1+1/c, maxNote to 1-1/c
	float a = 2.0;
	float b = -notesCount + 1.0 - 2.0 * float(minNoteMajor);

	float horizLoc = (id.x * a + b + id.w) / notesCount;
	float vertLoc = 2.0 * keyboardHeight - 1.0;
	vertLoc += (reverseMode ? -1.0 : 1.0) * (Out.noteSize.y * 0.5 + mainSpeed * (id.y - time));
	vec2 noteShift = vec2(horizLoc, vertLoc);

	noteShift.x += id.w * minorShift(minorIds[int(id.x)] % 12) * Out.noteSize.x;

	// Scale uv.
	Out.uv = Out.noteSize * v;
	Out.isMinor = id.w;
	Out.channel = channel;
	// Output position.
	gl_Position = vec4(flipIfNeeded(Out.noteSize * v + noteShift), 0.0 , 1.0) ;
	
}
