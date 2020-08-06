#version 330

in INTERFACE {
	float id;
} In ;


uniform vec2 inverseScreenSize;

uniform vec3 pedalColor;
uniform ivec3 pedalFlags; // sostenuto, damper, soft
uniform float pedalOpacity;
uniform bool mergePedals;

out vec4 fragColor;


void main(){

	float vis = pedalOpacity;

	// When merging, only display the center pedal.
	if(mergePedals && (int(In.id) != 0)){
		discard;
	}

	// Else find if the current pedal (or any if merging) is active.
	for(int i = 0; i < 3; ++i){
		if((mergePedals || int(In.id) == i) && pedalFlags[i] > 0){
			vis = 1.0;
			break;
		}
	}
	
	fragColor = vec4(pedalColor, vis);
}
