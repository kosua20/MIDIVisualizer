#version 330

in INTERFACE {
	vec2 uv;
	float id;
} In ;

uniform sampler2D font;
uniform vec3 color;
uniform float digitCount;
uniform int firstMeasure;
uniform bool reverseMode;

out vec4 fragColor;

void main(){
	vec2 globalUV = In.uv;

	float digitLoc = max(0.0, digitCount - floor(globalUV.x) - 1.0);
	int measure = (reverseMode ? -1 : 1) * (firstMeasure + int(In.id));

	int powTen = int(pow(10, digitLoc));
	int number = (measure / powTen) % 10;
	// Remap 0,1 to 0.0, 1.0-padding on X,
	const float paddingInTexture = 0.12;
	vec2 localUV = fract(globalUV) * vec2(1.0-paddingInTexture, 1.0);
	vec2 pixelSize = 0.5 / vec2(200.0, 256.0);
	localUV = clamp(localUV, pixelSize, 1.0 - pixelSize);
	localUV += vec2(number % 5, 1 - (number / 5));
	localUV *= vec2(0.2, 0.5);

	vec2 distancesFont = textureLod(font, localUV, 0).rg;
	float sdfFont = distancesFont.r - distancesFont.g;
	// Clamp to a reasonable value here to avoid jumps at the edge between two distance fields.
	// TODO: (MV) instead, merge distance fields when comuting them.
	float deltaStep = min(fwidth(sdfFont), 0.1);
	float alpha = 1.0 - smoothstep(0.01 - deltaStep, 0.01 + deltaStep, sdfFont);
	fragColor = vec4(color, alpha);

	if(measure < 0){
		discard;
	}

	if((measure < powTen) && (digitLoc != 0)){
		discard;
	}


}
