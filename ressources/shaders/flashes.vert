#version 330

layout(location = 0) in vec2 v;
layout(location = 1) in int isOn;

uniform float time;
uniform vec2 inverseScreenSize;

#define notesCount 52.0

const float shifts[88] = float[](0, 0.5, 1, 2, 2.5, 3, 3.5, 4, 5, 5.5, 6, 6.5, 7, 7.5, 8, 9, 9.5, 10, 10.5, 11, 12, 12.5, 13, 13.5, 14, 14.5, 15, 16, 16.5, 17, 17.5, 18, 19, 19.5, 20, 20.5, 21, 21.5, 22, 23, 23.5, 24, 24.5, 25, 26, 26.5, 27, 27.5, 28, 28.5, 29, 30, 30.5, 31, 31.5, 32, 33, 33.5, 34, 34.5, 35, 35.5, 36, 37, 37.5, 38, 38.5, 39, 40, 40.5, 41, 41.5, 42, 42.5, 43, 44, 44.5, 45, 45.5, 46, 47, 47.5, 48, 48.5, 49, 49.5, 50, 51);
const vec2 scale = 0.9*vec2(3.5,3.0);

out INTERFACE {
	vec2 uv;
	float on;
	float id;
} Out;


void main(){
	
	// Scale quad, keep the square ratio.
	vec2 scaledPosition = v * 2.0 * scale /notesCount * vec2(1.0, inverseScreenSize.y/inverseScreenSize.x);
	// Shift based on note/flash id.
	vec2 globalShift = vec2(-1.0 + (shifts[gl_InstanceID] * 2.0 + 1.0) / notesCount,-0.5);
	
	gl_Position = vec4(scaledPosition + globalShift, 0.0 , 1.0) ;
	
	// Pass infos to the fragment shader.
	Out.uv = v;
	Out.on = float(isOn>0);
	Out.id = float(gl_InstanceID);
	
}
