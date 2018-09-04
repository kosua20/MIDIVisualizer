#version 330

in INTERFACE {
	float isMinor;
	vec2 uv;
	vec2 noteSize;
} In;

uniform vec3 baseColor;
uniform vec2 inverseScreenSize;

#define cornerRadius 0.01
#define bottomLimit 0.25

out vec3 fragColor;


void main(){
	
	// If lower area of the screen, discard fragment as it should be hidden behind the keyboard.
	if(gl_FragCoord.y < bottomLimit/inverseScreenSize.y){
		discard;
	}
	
	// Rounded corner (super-ellipse equation).
	float radiusPosition = pow(abs(In.uv.x/(0.5*In.noteSize.x)), In.noteSize.x/cornerRadius) + pow(abs(In.uv.y/(0.5*In.noteSize.y)), In.noteSize.y/cornerRadius);
	
	if(	radiusPosition > 1.0){
		discard;
	}
	
	// Fragment color.
	fragColor = (0.8+0.2*(1.0-In.isMinor))*baseColor;
	
	if(	radiusPosition > 0.8){
		fragColor *= 1.05;
	}
	
}
