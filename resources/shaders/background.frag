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
uniform float minorsWidth = 1.0;
uniform sampler2D screenTexture;
uniform vec3 textColor = vec3(1.0);
uniform vec3 linesColor = vec3(1.0);
uniform bool reverseMode = false;

#define MAJOR_COUNT 75.0

const float octaveLinesPositions[11] = float[](0.0/75.0, 7.0/75.0, 14.0/75.0, 21.0/75.0, 28.0/75.0, 35.0/75.0, 42.0/75.0, 49.0/75.0, 56.0/75.0, 63.0/75.0, 70.0/75.0);
			
uniform float mainSpeed;
uniform float keyboardHeight = 0.25;

uniform int minNoteMajor;
uniform float notesCount;

out vec4 fragColor;


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
	if(num < -0.1){
		return 0.0f;
	}
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
	
	vec4 bgColor = vec4(0.0);
	// Octaves lines.
	if(useVLines){
		// send 0 to (minNote)/MAJOR_COUNT
		// send 1 to (maxNote)/MAJOR_COUNT
		float a = (notesCount) / MAJOR_COUNT;
		float b = float(minNoteMajor) / MAJOR_COUNT;
		float refPos = a * In.uv.x + b;

		for(int i = 0; i < 11; i++){
			float linePos = octaveLinesPositions[i];
			float lineIntensity = 0.7 * step(abs(refPos - linePos), inverseScreenSize.x / MAJOR_COUNT * notesCount);
			bgColor = mix(bgColor, vec4(linesColor, 1.0), lineIntensity);
		}
	}
	
	vec2 scale = 1.5*vec2(64.0,50.0*inverseScreenSize.x/inverseScreenSize.y);
	
	// Text on the side.
	int currentMesure = int(floor(time/secondsPerMeasure));
	// How many mesures do we check.
	int count = int(ceil(0.75*(2.0/mainSpeed)))+2;

	// We check two extra measures to avoid sudden disappearance below the keyboard.
	for(int i = -2; i < count; i++){
		// Compute position of the measure currentMesure+-i.
		int mesure = currentMesure + (reverseMode ? -1 : 1) * i;
		vec2 position = vec2(0.005, keyboardHeight + (reverseMode ? -1.0 : 1.0) * (secondsPerMeasure * mesure - time)*mainSpeed*0.5);
		//position.y *= ;

		// Compute color for the number display, and for the horizontal line.
		float numberIntensity = useDigits ? printNumber(mesure, position, In.uv, scale) : 0.0;
		bgColor = mix(bgColor, vec4(textColor, 1.0), numberIntensity);
		float lineIntensity = useHLines ? (0.25*(step(abs(In.uv.y - position.y - 0.5 / scale.y), inverseScreenSize.y))) : 0.0;
		bgColor = mix(bgColor, vec4(linesColor, 1.0), lineIntensity);
	}
	
	if(all(equal(bgColor.xyz, vec3(0.0)))){
		// Transparent background.
		discard;
	}
	
	fragColor = bgColor;
	
}
