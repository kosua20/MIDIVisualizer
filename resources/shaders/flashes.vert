#version 330

layout(location = 0) in vec2 v;
layout(location = 1) in int onChan;

uniform float time;
uniform vec2 inverseScreenSize;
uniform float userScale = 1.0;
uniform float keyboardHeight = 0.25;

uniform int minNote;
uniform float notesCount;

const float shifts[128] = float[](
	0,0.5,1,1.5,2,3,3.5,4,4.5,5,5.5,6,7,7.5,8,8.5,9,10,10.5,11,11.5,12,12.5,13,14,14.5,15,15.5,16,17,17.5,18,18.5,19,19.5,20,21,21.5,22,22.5,23,24,24.5,25,25.5,26,26.5,27,28,28.5,29,29.5,30,31,31.5,32,32.5,33,33.5,34,35,35.5,36,36.5,37,38,38.5,39,39.5,40,40.5,41,42,42.5,43,43.5,44,45,45.5,46,46.5,47,47.5,48,49,49.5,50,50.5,51,52,52.5,53,53.5,54,54.5,55,56,56.5,57,57.5,58,59,59.5,60,60.5,61,61.5,62,63,63.5,64,64.5,65,66,66.5,67,67.5,68,68.5,69,70,70.5,71,71.5,72,73,73.5,74
);

const vec2 scale = 0.9*vec2(3.5,3.0);

out INTERFACE {
	vec2 uv;
	float onChannel;
	float id;
} Out;


void main(){
	
	// Scale quad, keep the square ratio.
	vec2 scaledPosition = v * 2.0 * scale * userScale/notesCount * vec2(1.0, inverseScreenSize.y/inverseScreenSize.x);
	// Shift based on note/flash id.
	vec2 globalShift = vec2(-1.0 + ((shifts[gl_InstanceID] - shifts[minNote]) * 2.0 + 1.0) / notesCount, 2.0 * keyboardHeight - 1.0);
	
	gl_Position = vec4(scaledPosition + globalShift, 0.0 , 1.0) ;
	
	// Pass infos to the fragment shader.
	Out.uv = v;
	Out.onChannel = float(onChan);
	Out.id = float(gl_InstanceID);
	
}
