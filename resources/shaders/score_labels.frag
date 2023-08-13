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
	vec2 globalUV = In.uv * vec2(digitCount, 1.0);

	float digitLoc = max(0.0, digitCount - floor(globalUV.x) - 1.0);
	int measure = (reverseMode ? -1 : 1) * (firstMeasure + int(In.id));
	if(measure < 0){
		discard;
	}
	int powTen = int(pow(10, digitLoc));

	if((measure < powTen) && (digitLoc != 0)){
		discard;
	}

	int number = (measure / powTen) % 10;

	vec2 localUV = fract(globalUV);
	localUV += vec2(number % 5, 1 - (number / 5));
	localUV *= vec2(0.2, 0.5);

	float distanceFont = textureLod(font, localUV, 0).r;
	float alpha = 1.0 - smoothstep(0.05 - fwidth(distanceFont), 0.05 + fwidth(distanceFont), distanceFont);
	fragColor = vec4(color, alpha);
}
