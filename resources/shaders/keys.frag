#version 330

in INTERFACE {
	vec2 uv;
} In ;

uniform vec2 inverseScreenSize;
uniform float minorsWidth = 1.0;
uniform vec3 keysColor = vec3(0.0);

const bool isMinor[88] = bool[](true, false, true, true, false, true, true, true, false, true, true, false, true, true, true, false, true, true, false, true, true, true, false, true, true, false, true, true, true, false, true, true, false, true, true, true, false, true, true, false, true, true, true, false, true, true, false, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false);
const float octaveLinesPositions[8] = float[](2.0/52.0, 9.0/52.0, 16.0/52.0, 23.0/52.0, 30.0/52.0, 37.0/52.0, 44.0/52.0, 51.0/52.0);
			

out vec3 fragColor;




void main(){
	
	// White keys, and separators.
	float intensity = int(abs(fract(In.uv.x*52.0)) >= 2.0 * 52.0 * inverseScreenSize.x);
		
	// Upper keyboard.
	if(gl_FragCoord.y > 0.10/inverseScreenSize.y){
		// Handle black keys.
		int index = int(floor(In.uv.x*52.0+0.5))-1;
	
		if(index > 0 && isMinor[index]){
			// If the minor keys are not thinner, preserve a 1 px margin on each side.
			float marginSize = minorsWidth != 1.0 ? minorsWidth : 1.0 - (2.0*52.0*inverseScreenSize.x);
			intensity = step(marginSize, abs(fract(In.uv.x*52.0+0.5)*2.0-1.0));
		}
	}
	fragColor = mix(keysColor, vec3(1.0), intensity);
	
}
