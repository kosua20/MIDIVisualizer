#version 330

in INTERFACE {
	float id;
} In ;


uniform vec2 inverseScreenSize;

uniform vec3 pedalColor;
uniform vec4 pedalFlags; // sostenuto, damper, soft, expression
uniform float pedalOpacity;
uniform bool mergePedals;

out vec4 fragColor;


void main(){

	// When merging, only display the center pedal.
	if(mergePedals && (int(In.id) != 0)){
		discard;
	}

	// Else find if the current pedal (or any if merging) is active.
	float maxIntensity = 0.0f;

	for(int i = 0; i < 4; ++i){
		if(mergePedals || int(In.id) == i){
			maxIntensity = max(maxIntensity, pedalFlags[i]);
		}
	}

	float finalOpacity = mix(pedalOpacity, 1.0, maxIntensity);
	fragColor = vec4(pedalColor, finalOpacity);
}
