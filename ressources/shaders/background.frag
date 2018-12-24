#version 330

in INTERFACE {
	vec2 uv;
} In ;

uniform float time;
uniform float secondsPerMeasure;
uniform vec2 inverseScreenSize;
uniform bool useDigits = true;
uniform bool useHLines = true;
uniform bool useVLines = true;
uniform bool useKeys = true;
uniform float minorsWidth = 1.0;
uniform sampler2D screenTexture;

const bool isMinor[88] = bool[](true, false, true, true, false, true, true, true, false, true, true, false, true, true, true, false, true, true, false, true, true, true, false, true, true, false, true, true, true, false, true, true, false, true, true, true, false, true, true, false, true, true, true, false, true, true, false, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false);
const float octaveLinesPositions[8] = float[](2.0/52.0, 9.0/52.0, 16.0/52.0, 23.0/52.0, 30.0/52.0, 37.0/52.0, 44.0/52.0, 51.0/52.0);
			
uniform float mainSpeed;
#define bottomLimit 0.25

out vec3 fragColor;


float printDigit(int digit, vec2 uv){
	// Clamping to avoid artifacts.
	if(uv.x < 0.01 || uv.x > 0.99 || uv.y < 0.01 || uv.y > 0.99){
		return 0.0;
	}
	
	// UV from [0,1] to local tile frame.
	vec2 localUV = uv * vec2(50.0/256.0,0.5);
	// Select the digit.
	vec2 globalUV = vec2( mod(digit,5)*50.0/256.0,digit < 5 ? 0.5 : 0.0);
	// Combine global and local shifts.
	vec2 finalUV = globalUV + localUV;
	
	// Read from font atlas. Return if above a threshold.
	float isIn = texture(screenTexture, finalUV).r;
	return isIn < 0.5 ? 0.0 : isIn ;
	
}


float printNumber(float num, vec2 position, vec2 uv, vec2 scale){
	
	if(position.y > 1.0 || position.y < 0.0){
		return 0.0;
	}
	
	// We limit to the [0,999] range.
	float number = min(999.0, max(0.0,num));
	
	// Extract digits.
	int hundredDigit = int(floor( number / 100.0 ));
	int tenDigit	 = int(floor( number / 10.0 - hundredDigit * 10.0));
	int unitDigit	 = int(floor( number - hundredDigit * 100.0 - tenDigit * 10.0));
	
	// Position of the text.
	vec2 initialPos = scale*(uv-position);
	
	// Get intensity for each digit at the current fragment.
	float hundred = printDigit(hundredDigit, initialPos);
	float ten	  =	printDigit(tenDigit,	 initialPos - vec2(scale.x * 0.009,0.0));
	float unit	  = printDigit(unitDigit,	 initialPos - vec2(scale.x * 0.009 * 2.0,0.0));
	
	// If hundred digit == 0, hide it.
	float hundredVisibility = (1.0-step(float(hundredDigit),0.5));
	hundred *= hundredVisibility;
	// If ten digit == 0 and hundred digit == 0, hide ten.
	float tenVisibility = max(hundredVisibility,(1.0-step(float(tenDigit),0.5)));
	ten*= tenVisibility;
	
	return hundred + ten + unit;
}


void main(){
	
	float intensity = 0.0;
	
	// Keys
	if(useKeys && gl_FragCoord.y < bottomLimit/inverseScreenSize.y){
		
		
			// White keys, and separators.
			intensity = int(abs(fract(In.uv.x*52.0)) >= 2.0 * 52.0 * inverseScreenSize.x);
		
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
			fragColor = vec3(intensity);
			// Done.
			return;
		
		
		
	}
	
	// Octaves lines.
	for(int i = 0; i < 8; i++){
		float lineIntensity = useVLines ? (0.7 * step(abs(In.uv.x - octaveLinesPositions[i]),inverseScreenSize.x)) : 0.0;
		intensity = max(intensity, lineIntensity);
	}
	
	vec2 scale = 1.5*vec2(64.0,50.0*inverseScreenSize.x/inverseScreenSize.y);
	
	// Text on the side.
	int currentMesure = int(floor(time/secondsPerMeasure));
	// How many mesures do we check.
	int count = int(ceil(0.75*(2.0/mainSpeed)))+2;
	
	for(int i = 0; i < count; i++){
		// Compute position of the measure currentMesure+i.
		vec2 position = vec2(0.005,bottomLimit + (secondsPerMeasure*(currentMesure+i) - time)*mainSpeed*0.5);
		
		// Compute intensity for the number display, and for the horizontal line.
		float numberIntensity = useDigits ? printNumber(currentMesure + i,position, In.uv, scale) : 0.0;
		float lineIntensity = useHLines ? (0.25*(step(abs(In.uv.y - position.y - 0.5 / scale.y), inverseScreenSize.y))) : 0.0;
		
		intensity = max(intensity, max(numberIntensity, lineIntensity));
	}
	
	if(intensity == 0.0){
		// Transparent background.
		discard;
	}
	
	fragColor = vec3(intensity);
	
}
