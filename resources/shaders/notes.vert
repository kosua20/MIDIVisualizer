#version 330

layout(location = 0) in vec2 v;
layout(location = 1) in vec4 id; //note id, start, duration, is minor
layout(location = 2) in float channel; //note id, start, duration, is minor

uniform float time;
uniform float mainSpeed;
uniform float minorsWidth = 1.0;
uniform float keyboardHeight = 0.25;
uniform bool reverseMode = false;

uniform int minNoteMajor;
uniform float notesCount;

out INTERFACE {
	vec2 uv;
	vec2 noteSize;
	float isMinor;
	float channel;
} Out;


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
	
	// Scale uv.
	Out.uv = Out.noteSize * v;
	Out.isMinor = id.w;
	Out.channel = channel;
	// Output position.
	gl_Position = vec4(Out.noteSize * v + noteShift, 0.0 , 1.0) ;
	
}
