#version 330

in INTERFACE {
	vec2 uv;
	float id;
} In ;

uniform sampler2D font;
uniform vec3 color;

out vec4 fragColor;

void main(){
	vec2 globalUV = In.uv;

	int digitLoc = int(floor(globalUV.x));
	int measure = int(In.id);

	if(measure < 0){
		discard;
	}

	int powTen = 1;
	for(int i = 0; i < digitLoc; ++i){
		powTen *= 10;
	}

	if((measure < powTen) && (digitLoc != 0)){
		discard;
	}

	int number = (measure / powTen) % 10;
	// Remap 0,1 to 0.0, 1.0-padding on X,
	const float paddingInTexture = 0.12;
	// Flip UVs back.
	vec2 localUV = (1.0-fract(globalUV)) * vec2(1.0-paddingInTexture, 1.0);
	vec2 pixelSize = 0.5 / vec2(200.0, 256.0);
	localUV = clamp(localUV, pixelSize, 1.0 - pixelSize);
	localUV += vec2(number % 5, 1 - (number / 5));
	localUV *= vec2(0.2, 0.5);

	vec2 distancesFont = textureLod(font, localUV, 0).rg;
	float sdfFont = distancesFont.r - distancesFont.g;
	// Clamp to a reasonable value here to avoid edge artifacts?
	float deltaStep = min(fwidth(sdfFont), 0.1);
	float alpha = 1.0 - smoothstep(0.01 - deltaStep, 0.01 + deltaStep, sdfFont);
	fragColor = vec4(color, alpha);
}
